from __future__ import division
import cv2
#to show the image
from matplotlib import pyplot as plt
import subprocess
import numpy as np
from math import cos, sin
import time

# import libraries
import serial
import time

port1 = '/dev/armArduino'
# this creates the connection and opens the port
armArduino = serial.Serial(port1, 9600)

port2 = '/dev/motorArduino'
# this creates the connection and opens the port
motorsArduino = serial.Serial(port2, 9600)

time.sleep(1)

green = (0, 255, 0)

##################
#useage:
#cd /media/ubuntu/D232B91032B8FA95/jetson-inference/build/aarch64/bin
#python ~/distance/demo.py
#
##################

def distance_to_camera(knownWidth, focalLength, perWidth):
    # compute and return the distance from the maker to the camera
    return (knownWidth * focalLength) / perWidth

def show(image):
    # Figure size in inches
    plt.figure(figsize=(10, 10))

    # Show image, with nearest neighbour interpolation
    plt.imshow(image, interpolation='nearest')

def overlay_mask(mask, image):
    #make the mask rgb
    rgb_mask = cv2.cvtColor(mask, cv2.COLOR_GRAY2RGB)
    #calculates the weightes sum of two arrays. in our case image arrays
    #input, how much to weight each. 
    #optional depth value set to 0 no need
    img = cv2.addWeighted(rgb_mask, 0.5, image, 0.5, 0)
    return img

def find_biggest_contour(image):
    # Copy
    image = image.copy()
    #input, gives all the contours, contour approximation compresses horizontal, 
    #vertical, and diagonal segments and leaves only their end points. For example, 
    #an up-right rectangular contour is encoded with 4 points.
    #Optional output vector, containing information about the image topology. 
    #It has as many elements as the number of contours.
    #we dont need it
    contours, hierarchy = cv2.findContours(image, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    # Isolate largest contour
    contour_sizes = [(cv2.contourArea(contour), contour) for contour in contours]
    try:
         biggest_contour = max(contour_sizes, key=lambda x: x[0])[1]
    except:
         box1=np.int0([[5,5], [10,10]])
         print("No strawberry")
         biggest_contour = box1

    mask = np.zeros(image.shape, np.uint8)
    cv2.drawContours(mask, [biggest_contour], -1, 255, -1)
    return biggest_contour, mask

def circle_contour(image, contour, scale):
    # Bounding ellipse
    image_with_ellipse = image.copy()
    
    #easy function
    #ellipse = cv2.fitEllipse(contour)
    ellipse = cv2.minAreaRect(contour)
    box = np.int0(cv2.cv.BoxPoints(ellipse))
    #box1=np.int0([[1280*scale,720*scale]])

    #####print("Box: ", box/scale)
    #print("Width of box in pixels: ", abs(box[0][0]-box[3][0])/scale )
    large=max(box[0][0], box[1][0], box[2][0], box[3][0])
    small=min(box[0][0], box[1][0], box[2][0], box[3][0])
    #print box[1]
    #print white_box_distance
    #print "Distance from top left in inches:", box[1]*KNOWN_WIDTH/abs(large-small)
    #print(box1/scale)

    cv2.drawContours(image_with_ellipse, [box], -1, green, 2)
    #cv2.drawContours(image_with_ellipse, [box1], -1, green, 2)
    
    #add it
    #cv2.ellipse(image_with_ellipse, ellipse, green, 2, cv2.CV_AA)
    
    #cv2.imwrite('ellipse.jpg', ellipse)
    results = [image_with_ellipse, box]
    return results

def find_strawberry(image):
    #RGB stands for Red Green Blue. Most often, an RGB color is stored 
    #in a structure or unsigned integer with Blue occupying the least 
    #significant (a byte in 32-bit and 24-bit formats), Green the 
    #second least, and Red the third least. BGR is the same, except the 
    #order of areas is reversed. Red occupies the least significant area,
    # Green the second (still), and Blue the third.
    # we'll be manipulating pixels directly
    #most compatible for the transofrmations we're about to do
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    # Make a consistent size
    #get largest dimension
    max_dimension = max(image.shape)
    #The maximum window size is 700 by 660 pixels. make it fit in that
    scale = 700/max_dimension
    #resize it. same width and hieght none since output is 'image'.
    image = cv2.resize(image, None, fx=scale, fy=scale)
    
    #we want to eliminate noise from our image. clean. smooth colors without
    #dots
    # Blurs an image using a Gaussian filter. input, kernel size, how much to filter, empty)
    image_blur = cv2.GaussianBlur(image, (7, 7), 0)
    #t unlike RGB, HSV separates luma, or the image intensity, from
    # chroma or the color information.
    #just want to focus on color, segmentation
    image_blur_hsv = cv2.cvtColor(image_blur, cv2.COLOR_RGB2HSV)

    # Filter by colour
    # 0-10 hue
    #minimum red amount, max red amount
    min_red = np.array([0, 100, 80])
    max_red = np.array([10, 256, 256])

    #for blueberry
    #min_red = np.array([100, 100, 1])
    #max_red = np.array([139, 157, 203])
    #layer
    mask1 = cv2.inRange(image_blur_hsv, min_red, max_red)

    #birghtness of a color is hue
    # 170-180 hue
    min_red2 = np.array([170, 100, 80])
    max_red2 = np.array([180, 256, 256])
    mask2 = cv2.inRange(image_blur_hsv, min_red2, max_red2)

    #looking for what is in both ranges
    # Combine masks
    mask = mask1 + mask2
 
    # Clean up
    #we want to circle our strawberry so we'll circle it with an ellipse
    #with a shape of 15x15
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (15, 15))
    #morph the image. closing operation Dilation followed by Erosion. 
    #It is useful in closing small holes inside the foreground objects, 
    #or small black points on the object.
    mask_closed = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
    #erosion followed by dilation. It is useful in removing noise
    mask_clean = cv2.morphologyEx(mask_closed, cv2.MORPH_OPEN, kernel)

    # Find biggest strawberry
    #get back list of segmented strawberries and an outline for the biggest one
    big_strawberry_contour, mask_strawberries = find_biggest_contour(mask_clean)

    # Overlay cleaned mask on image
    # overlay mask on image, strawberry now segmented
    overlay = overlay_mask(mask_clean, image)

    # Circle biggest strawberry
    #circle the biggest one
    output = circle_contour(overlay, big_strawberry_contour, scale)
    circled = output[0]
    show(circled)
    
    #we're done, convert back to original color scheme
    bgr = cv2.cvtColor(circled, cv2.COLOR_RGB2BGR)
    
    #need box[1] of the strawberry to do distance evaluation with the cues
    results = [bgr,output[1]]

    return results

def find_marker(image):
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    max_dimension = max(image.shape)
    #scale = 700/max_dimension
    scale=1
    image = cv2.resize(image, None, fx=scale, fy=scale)
    image_blur = cv2.GaussianBlur(image, (7, 7), 0)
    image_blur_hsv = cv2.cvtColor(image_blur, cv2.COLOR_RGB2HSV)
    min_red = np.array([0, 100, 80])
    max_red = np.array([10, 256, 256])
    mask1 = cv2.inRange(image_blur_hsv, min_red, max_red)
    min_red2 = np.array([170, 100, 80])
    max_red2 = np.array([180, 256, 256])
    mask2 = cv2.inRange(image_blur_hsv, min_red2, max_red2)
    mask = mask1 + mask2
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (15, 15))
    mask_closed = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
    mask_clean = cv2.morphologyEx(mask_closed, cv2.MORPH_OPEN, kernel)

    # Find biggest strawberry
    #get back list of segmented strawberries and an outline for the biggest one
    big_strawberry_contour, mask_strawberries = find_biggest_contour(mask_clean)
    return cv2.minAreaRect(big_strawberry_contour)
    
def moveArm(first, second, third):
    print "Starting move arm..." 
    print first
    print third
    first = int( first * 190 / 640 ) - 110
    third = int( abs( third - 480 ) * 95 / 480 ) + 5
    command = './ArmFinalProduct/a.out ' + str(first) + ' ' + str(second) + ' ' + str(third)
    print command
    process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
    output, error = process.communicate()
    splitOutput = output.split()
    
    # check for errors from the output
    if len(splitOutput) == 4:
        # write and read with arduino
        for variable in splitOutput:
            armArduino.write(variable + '\n')
            print 'To Arm Arduino: ' + variable
            time.sleep(0.5)
        fromArduino = armArduino.readline()
        print 'From Arm Arduino: ' + fromArduino
    elif len(splitOutput) == 1:
        if splitOutput[0] == '-1\n': # too many arguments
            print 'Too many arguments to converter.'
        elif splitOutput == '-2\n': # too few arguments
            print 'Too few arguments to converter.'
        elif splitOutput == '-3\n': # cannot convert for arm
            print 'Illegal coordinates, this may damage the arm.'
    else:
        print 'Unknown error'    
    return output

def moveWheels(_dir):
    time.sleep(1)
    motorsArduino.write(str(_dir))
    time.sleep(1)
    return _dir


print 'Stopping Wheels - moving: '
print moveWheels(1)

cap = cv2.VideoCapture(1)
cap2 = cv2.VideoCapture(2)
print("camera initialized")
if cap.isOpened() and cap2.isOpened():
    moveArm(150, 150, 150)
    print("camera successfully opened")
    #grabs a frame and then decodes it.
    retval, image = cap.read(cap.grab())
    retval2, image2 = cap2.read(cap2.grab())

    while retval:
        retval, image = cap.read(cap.grab())
        retval, image = cap.read(cap.grab())
        retval, image = cap.read(cap.grab())
        image_with_box, box = find_strawberry(image)
        
        minx = min(box[0][0],box[1][0], box[2][0], box[3][0]) - 10
        maxx = max(box[0][0],box[1][0], box[2][0], box[3][0]) + 10
        miny = min(box[0][1],box[1][1], box[2][1], box[3][1]) - 10
        maxy = max(box[0][1],box[1][1], box[2][1], box[3][1]) + 10
        if minx < 0:
            minx = 0
        if maxx < 0:
            maxx = 0
        if miny < 0:
            miny = 0
        if maxy < 0:
            maxy = 0
        image = image[miny:maxy, minx:maxx]
        
        cv2.imwrite("camera-output.jpg", image)
        bashCommand = "./imagenet-console camera-output.jpg output_0.jpg"
        process = subprocess.Popen(bashCommand.split(),  stdout=subprocess.PIPE)
        output, error = process.communicate()
        
        print output
        print "X:", minx + ((maxx - minx) / 2 )
        print "Y:", miny + ((maxy - miny) / 2 )
        x_1 = minx + ((maxx - minx) / 2 )
        y_1 = miny + ((maxy - miny) / 2 )

        in_front = 0
       
        if "strawberry" in output:
            in_front = 1
            print "STRAWBERRY IN FRONT CAMERA"
	else:
	    print "NO STRAWBERRY IN FRONT CAMERA"
        #in_front = 1    
        #cv2.imshow("front image", image_with_box)
        #k=cv2.waitKey(0)
        #cv2.destroyWindow("front image")
	print "Received front image.\n"        
        
        ###side camera
        retval, image = cap2.read(cap2.grab())
        retval, image = cap2.read(cap2.grab())
        retval, image = cap2.read(cap2.grab())
        image_with_box, box = find_strawberry(image)
        
        minx = min(box[0][0],box[1][0], box[2][0], box[3][0]) - 10
        maxx = max(box[0][0],box[1][0], box[2][0], box[3][0]) + 10
        miny = min(box[0][1],box[1][1], box[2][1], box[3][1]) - 10
        maxy = max(box[0][1],box[1][1], box[2][1], box[3][1]) + 10
        if minx < 0:
            minx = 0
        if maxx < 0:
            maxx = 0
        if miny < 0:
            miny = 0
        if maxy < 0:
            maxy = 0

    	#cv2.imshow("side image", image_with_box)
        #cv2.waitKey(0)
    	#cv2.destroyWindow("side image")

	y_2 = 0
	a = 0
	b = 1
        if in_front == 1 and minx + ((maxx - minx) / 2 ) > 640/2 - 300 and minx + ((maxx - minx) / 2 ) < 640/2 + 300:
	    if minx + ((maxx - minx) / 2 ) > 640/2 - 300 and minx + ((maxx - minx) / 2 ) < 640/2 - 100:
		y_2 = 150
            if minx + ((maxx - minx) / 2 ) > 640/2 - 100 and minx + ((maxx - minx) / 2 ) < 640/2 + 100:
                y_2 = 200
            if minx + ((maxx - minx) / 2 ) > 640/2 + 100 and minx + ((maxx - minx) / 2 ) < 640/2 + 300:
                y_2 = 250

	    print y_2
            print "SIDE IN MIDDLE"
            a = 1
            image = image[miny:maxy, minx:maxx]
        
            cv2.imwrite("camera-output.jpg", image)
            bashCommand = "./imagenet-console camera-output.jpg output_0.jpg"
            process = subprocess.Popen(bashCommand.split(),  stdout=subprocess.PIPE)
            output, error = process.communicate()
        
            print output
            print "X:", minx + ((maxx - minx) / 2 )
            print "Y:", miny + ((maxy - miny) / 2 )

            if "strawberry" in output:
                b = 1
                print "STRAWBERRY IN SIDE IMAGE"
            #b = 1
        else:
            print "NO STRAWBERRY ON SIDE CAMERA"
        
	print "Side Image Taken"
        #q = raw_input('Side Image Taken - Press Enter To Continue')    
        #cv2.imshow("side image", image_with_box)
        #k=cv2.waitKey(0)
        #cv2.destroyWindow("side image")
        
        if a == 1 and b == 1 and in_front==1:
            print "Found strawberry, stop moving: "
            print moveWheels(1)
            print moveArm(x_1, y_2, y_1)
            q = raw_input('Grabbing strawberry - Press Enter When Done')
        else:
            #send arduino new move code
            print "Did not find strawberry"
	print "Moving Forward - Move:"
        print moveWheels(0)
        time.sleep(6)
        print "Stop Moving"
        print moveWheels(1)
        q = raw_input('End of Loop - Press Enter To Continue')
                    
else:
        print("camera failed to open")
        
        
