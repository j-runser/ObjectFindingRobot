//
//  main.c
//  Coordinates2ServoAngles
//
//  Redone by Skye VanAtta on 3/22/17.
//  Copyright © 2017 capstone. All rights reserved.
//
//  

#define epsilon 0.05 //this is the error allowance

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
    
    //Function Prototypes
    int yzEquation(double M2, double M3, double M4);

    
    //ENTER COORDINATES IN MILLIMETERS
    double z = 150; //must be > 0
    double y = 150; //must be > 0
    double x = 150; //can be + or -



    double tr = 3.14159/180; //degrees to radians
    double M1;
    double M2; //min is 15
    double M3; //min is 0
    double M4; //min is 0
    
    int main(int argc, const char * argv[]) {
        
        int finalM1 = 0;
        int finalM2 = 0;
        int finalM3 = 0;
        int finalM4 = 0;
        
        int goodxyz = 0;
        
        //testing for legal coordinates
        double XY = sqrt(x*x+y*y);
        if((z>= 0.0) && (XY*XY/198025.00 + z*z/265225.00)<=1 && (XY*XY/15625 + z*z/52900)>=1 ){
            
            goodxyz = 1;
            printf("Coordinates are legal\n");
        } else {
            goodxyz = 0;
            printf("Coordinates are NOT legal\n");
        }
        
        if(goodxyz == 1){
            
            //calculating M1 from x and y coordinate
            double xDivByy = x/y;
            int n = floor(atan(xDivByy)*180/3.14159 + 90 + 0.5);
            if(n > -1 && n < 181){
            M1 = n;
            finalM1 = M1;
            printf("M1: %d\n",finalM1);
            } else {
                finalM1 = 90;
                printf("M1 set to default: %d\n",finalM1);
            }
            
            x = fabs(x);
            
            M2 = 15.00; //min is 15
            M3 = 0.00; //min is 0
            M4 = 0.00; //min is 0
            
            //iteratively calculating M2, M3, and M4 from y and z coordinates
            int complete = 1;
            
            while(complete == 1){
                if(yzEquation(M2,M3,M4) == 1){
                    complete = 0;
                    break;
                }
                for(double i = 180.0; i >= 0.0; i=i-0.2){ //every 22.5 degrees for M4 is a minute or so
                    M4 = i;
                    if(M4 == 0){complete = 0; break;}
                    if(complete == 0){break;}
                    if(yzEquation(M2,M3,M4) == 1){
                        
                        complete = 0;
                        break;
                    }
                    //printf("M4 is %d right now \n",M4);
                    
                    
                    for(double j = 180.0; j >= 0.0; j=j-0.2){
                        M3 = j;
                        if(complete == 0){break;}
                        if(yzEquation(M2,M3,M4) == 1){
                            complete = 0;
                            break;
                        }
                        
                        
                        for(double k = 15.0; k <= 165.0; k=k+0.2){
                            M2 = k;
                            if(complete == 0){break;}
                            if(yzEquation(M2,M3,M4) == 1){

                                printf("M2= %f, M3= %f, M4= %f\n\n",M2,M3,M4);
                                
                                finalM2 = (int)floor(M2+0.5);
                                finalM3 = (int)floor(M3+0.5);
                                finalM4 = (int)floor(M4+0.5);
                                complete = 0;
                                break;
                                
                            }
                        }
                    }
                }
            }
        }
        
        
        printf("Final values to be passed to the arduino: \n M1 = %d, M2 = %d, M3 = %d, M4 = %d \n",finalM1,finalM2,finalM3,finalM4);
        
        
        return 0;
    }
    
    
    
    
    int yzEquation(double M2, double M3, double M4){
        
        double XY = sqrt(x*x+y*y);
        
        double solvedXY =  -125.00 * cos(M2 * tr)
                            + 125.00 * fabs(cos((M3+fabs(90.0-M2))*tr))*sin((M2+M3+180.0)*tr)/fabs(sin((M2+M3+180.0)*tr))
                            + 195.00 *fabs(cos((M4+fabs(90.0-M3)+fabs(90.0-M2))*tr))*cos((M2+M3+M4)*tr)
                            /fabs(cos((M2+M3+M4)*tr));
        
        
        double solvedZ =  125.00 * sin(M2*tr)
                            + 125.00 * fabs((sin((M3+fabs(90.0-M2))*tr)))*sin((M2+M3-90.0)*tr)/fabs(sin((M2+M3-90.0)*tr))
                            + 195.00 * fabs(sin((M4+fabs(90.0-M3)+fabs(90.0-M2))*tr))*sin((M2+M3+M4+180.0)*tr)
                            /fabs(sin((M2+M3+M4+180.0)*tr)) + 70.0;
  


        int solved;
        if(((fabs(solvedXY - XY)) < epsilon) && ((fabs(solvedZ - z)) < epsilon)){
            solved = 1;
        }else{
            solved = 0;
        }
        return solved; 
    }


