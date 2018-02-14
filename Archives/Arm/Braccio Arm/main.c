//
//
//  Converts X,Y,Z Coordinates in millimeters to Servo Angles M1,M2,M3,M4
//  main.c
//  ArmCodeTesting
//
//  Created by Skye VanAtta on 1/30/17.


#include <stdio.h>
#include <math.h>

//Function Prototypes
int xyEquation(int M2, int M3, int M4);
int yEquation();
int xEquation();

//ENTER COORDINATES
int y = 170;
int x = 300; //can be + or - but with z has to be shifted by 180 if -
int z = 100;

double tr = 3.14159/180; //degrees to radians
int M1;
int M2; //min is 15
int M3; //min is 0
int M4; //min is 0

int main(int argc, const char * argv[]) {
    
    int finalM1 = 0;
    int finalM2 = 0;
    int finalM3 = 0;
    int finalM4 = 0;

    int goodxyz;
  
    //testing for legal coordinates
    if((x>60)&& (abs(z)>60) && (y>= 0) && ((x*x/198025 + y*y/265225)<=1.0) && (abs(z)<=445)){
        goodxyz = 1;
    } else {
        goodxyz = 0;
    }

    if(goodxyz == 1){
        
        //calculating M1 from z and x coordinate
        int n = floor(atan(z/x)*180/3.14159)+90;
        M1 = n;
        finalM1 = M1;
        
        M2 = 15; //min is 15
        M3 = 0; //min is 0
        M4 = 0; //min is 0
        
        //iteratively calculating M2, M3, and M4 from x and y coordinates
        int complete = 1;
        
        while(complete == 1){
            if(xyEquation(M2,M3,M4) == 1){
                complete = 0;
                break;
            }
            for(int i = 0; i <= 180; i++){
                M4 = i;
                if(xyEquation(M2,M3,M4) == 1){

                    complete = 0;
                    break;
                }
                //printf("M4 is %d right now \n",M4);
                
                
                for(int j = 0; j <= 180; j = j+1){
                    M3 = j;
                    if(xyEquation(M2,M3,M4) == 1){
                        complete = 0;
                        break;
                    }
                    
                    
                    for(int k = 15; k <= 165; k = k+1){
                        M2 = k;
                        if(xyEquation(M2,M3,M4) == 1){
                            //printf("X is %d right now \n",xEquation());
                            //printf("Y is %d right now \n",yEquation());
                            printf("M2= %d, M3= %d, M4= %d\n\n",M2,M3,M4);

                            finalM2 = M2;
                            finalM3 = M3;
                            finalM4 = M4;
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




int xyEquation(int M2, int M3, int M4){
    
    int solvedX = floor( 125 * cos(M2 * tr)
                        + 125*fabs(cos((M3+abs(90-M2))*tr))*sin((M2+M3+180)*tr)/fabs(sin((M2+M3+180)*tr))
                        + 185*fabs(cos((M4+abs(90-M3)+abs(90-M2))*tr))*cos((M2+M3+M4)*tr)/fabs(cos((M2+M3+M4)*tr)));
    
    int solvedY = floor( 125*sin(M2*tr)
                        + 125*fabs((sin((M3+abs(90-M2))*tr)))*sin((M2+M3-90)*tr)/fabs(sin((M2+M3-90)*tr))
                        + 185*fabs(sin((M4+abs(90-M3)+abs(90-M2))*tr))*sin((M2+M3+M4+180)*tr)
                        /fabs(sin((M2+M3+M4+180)*tr)));
    int solved;
    if((solvedX == x) && (solvedY == y)){
        solved = 1;

    }else{
        solved = 0;
    }
    return solved; 
}


