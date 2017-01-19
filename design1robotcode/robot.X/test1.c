//***preprocessor***
#include<htc.h>
#include<PICinit.h>
#include<PICinit.c>
#define N 5
//***main***
void motorsoff();
void forward();
void backward();
void pivotleft();
void pivotright();
void target();
void navigation();
void test(int x);

unsigned int sensor0[N],sensor1=0,sensor2[N],sensor3=0,sensor5=0,sensor=0,floor,color,i,sum=0,average,a,b=0;
main()
{

    configurePIC();        //Setup hardware configuration of the PIC
    INTE = 1; // Enable the External Interrupt Line
GIE = 1; // Enable Global Interrupts
motorspeed(1,93);      //right from above
motorspeed(2,100);
output_high('a',4);    // Initialize LED#1(port A bit 4)-turn it off(high)
output_high('e',2);    //Initialize LED#2(port E bit 2)-turn it off(high)

/*test the color of the field*/
output_high('d',0);
sensor1=read_adc(1);
pause(100);// sensor1 now holds the voltage read at light sensor 0
output_low('d',0);
if(sensor1<20000)   //need test
    floor=0;        //white
else
    floor=1;        //black

while(1)
{
output_high('d',0);
sensor1=read_adc(1); // sensor1 now holds the voltage read at light sensor 0
pause(100);
output_low('d',0);
if(sensor1<20000)   //need test
    color=0;        //white
else
    color=1;       //black

if(color==floor)   //robot is on home side
navigation();
else              //robot is on enemy side
 target();
   forward();
   pause(2800);   //robot goes forward for 2.8 seconds
   motorsoff();
/*Testing Part*/  
//sensor0[0]= read_adc (0),putdata (sensor0[0]),pause (400); // pause 4 tenths of a second to slow down the output
//test(23);
//target();forward();pause(2800);motorsoff();
//output_high('d',0);sensor1=read_adc(1);pause(100);output_low('d',0); putdata(sensor1);pause(1000);
//navigation();motorsoff();pause(5000);
//sensor2[0]=read_adc(2);putdata(sensor2[0]);pause(500);//TEST
//sensor0[0]=read_adc(0);putdata(sensor0[0]);pause(500);//TEST
//sensor3=read_adc(3);putdata(sensor3);pause(500);
//sensor5=read_adc(5);putdata(sensor5);pause(1000);
}
}

void test(int x)// testing function, to test the counting variable for robot rotating a full circle in target and navigation functions
{
    int y=0;
    sensor3 = read_adc (3);
sensor5 = read_adc (5);

sensor0[0]=read_adc(0);
sensor=sensor0[0];

while(y<x)   //need test no average for speed
{
   sensor=sensor0[0];
    if(sensor3<sensor5)
       pivotleft();
   else
       pivotright();
   pause(90);

sensor0[0]=read_adc(0);
y++;
}

motorsoff();
   return;
}
void motorsoff()
{
 output_low('C',0);
 output_low('C',5);
 output_low('C',3);
 output_low('C',4);
 return;
}

void forward()
{
output_low('C',5);
output_low('C',3);
 output_high('C',4);
 output_high('C',0);
 return;
}

void backward()
{
 output_low('C',0);
 output_high('C',5);
 output_low('C',4);
 output_high('C',3);
 return;
}

void pivotleft()
{
 output_low('C',5);
output_low('C',4);
output_high('C',3);
output_high('C',0);

return;
}

void pivotright()
{
 output_low('C',0);
output_low('C',3);
output_high('C',4);
output_high('C',5);

 return;
}

void target()
{
a=0;                    //counting variable
sensor3 = read_adc (3);
sensor5 = read_adc (5);
 sensor0[0]=read_adc(0);
sensor2[0]=read_adc(2);
sensor=sensor2[0];

while(sensor>12000||sensor0[0]<12000)   //number need test, quickly finding the lighting area
{if(a>23)              //if robot rotates a full circle and can't find target, break out first loop
{forward();break;}
   sensor=sensor2[0];
    if(sensor3<sensor5) //left light sensor get stonger light intensity
       pivotleft();
   else                 //right light sensor get stonger light intensity
       pivotright();
   pause(90);
   
sensor2[0]=read_adc(2);
a++;
sensor0[0]=read_adc(0);
}
motorsoff();
motorspeed(1,50);      //low down the speed
motorspeed(2,70);
 for(i=0,sum=0;i<N;i++)    //get an average data in small scale
{
sensor2[i]=read_adc(2);
sum+=sensor2[i];
}
average=sum/N;
sensor2[0]=average;
sensor=sensor2[0];
//need test
sensor3 = read_adc (3);         //get light intensity data from left and right sensors
sensor5 = read_adc (5);

while(sensor2[0]<=sensor)       //use average sum for precisely locating the exact position
{
       sensor=sensor2[0];
    if(sensor3<sensor5)        //left light sensor get stonger light intensity

    pivotleft();


    else                       //right light sensor get stonger light intensity
        pivotright();
       pause(90);
       for(i=0,sum=0;i<N;i++) //get an average data in small scale
{
sensor2[i]=read_adc(2);
sum+=sensor2[i];
}
average=sum/N;
sensor2[0]=average;          //average
}
motorsoff();
pause(100);
motorspeed(1,93);      //speed up and return
motorspeed(2,100);
forward();
   return;
    //test}



}

void navigation()
{
   b=0;               //counting variable
sensor3 = read_adc (3);
sensor5 = read_adc (5);

sensor0[0]=read_adc(0);
sensor=sensor0[0];

while(sensor>10000)   //number need test quickly find the lighting area
{if(b>23)             //if robot rotates a full circle and can't find target, break out first loop
{forward();break;}
   sensor=sensor0[0];
    if(sensor3<sensor5) //left light sensor get stonger light intensity
       pivotleft();
   else                //right light sensor get stonger light intensity
       pivotright();
   pause(90);

sensor0[0]=read_adc(0);
b++;
}
motorsoff();
pause(500);
motorspeed(1,50);      //right from above
motorspeed(2,70);
 for(i=0,sum=0;i<N;i++)  //get an average data in small scale
{
sensor0[i]=read_adc(0);
sum+=sensor0[i];
}
average=sum/N;
sensor0[0]=average;
sensor=sensor0[0];
//need test the light
sensor3 = read_adc (3);
sensor5 = read_adc (5);

while(sensor0[0]<=sensor)       //average for precise
{
       sensor=sensor0[0];
    if(sensor3<sensor5)        //left light sensor get stonger light intensity

    pivotleft();


    else                     //right light sensor get stonger light intensity
        pivotright();
       pause(90);
       for(i=0,sum=0;i<N;i++) //get an average data in small scale
{
sensor0[i]=read_adc(0);
sum+=sensor0[i];
}
average=sum/N;
sensor0[0]=average;

}
motorsoff();
pause(500);
motorspeed(1,93);      //right from above
motorspeed(2,100);
forward();
   return;
    //test}


}

void interrupt isr (void) // This function will be automatically called upon an interrupt
{
if (save_data ( ) ==0) // If interrupt was caused by real time clock, return. If
{return;} // interrupt was caused by bumper, save function variables
// being used by main() so they can be restored upon returning
// from interrupt.
pause (5); // Pause 5 msec to allow for bumper switch contact bounce
if ((PORTB & 0x01) == 1) // Confirm valid bumper interrupt
{
INTF = 0;
restore_data ( ); //Restore function variables used in main()
return;
}
output_low ('c', 0); //STOP the Motors! I?ve bumped into something.
output_low ('c', 5);
output_low ('c', 3);
output_low ('c', 4);
pause (50); //Always allow some time (50ms) for the motors to stop
/*YOUR CODE GOES HERE - I.e., Check which bumper(s) hit and take evasive motor action. SEE IMPORTANT NOTES BELOW */
if ((read_input ('E', 0)) == 0&&read_input('E',1)!=0)//right bumper hit
{
output_high ('E', 2);
    output_low ('A', 4);
    /*DO SOMETHING*/
    backward();
    pause(425);
    motorsoff();
    pivotleft();      //rotate left a little degree
    pause(125);
    motorsoff();
    forward();
}
else if ((read_input ('E', 1)) == 0&&read_input('E',0)!=0)//left bumper hit
{
output_high ('A', 4);
output_low ('E', 2);
    /*DO SOMETHING*/
    backward();
    pause(425);
    motorsoff();
    pivotright();    //rotate right a little degree
    pause(125);
    motorsoff();
    forward();
}                                                          //both bumpers hit
else if ((read_input ('E', 1)) == 0&&read_input('E',0)==0)//use two light sensors on two sides
{                                                         //to determine which side to turn
output_low ('A', 4);
output_low ('E', 2);
    /*DO SOMETHING*/

    sensor3=read_adc(3);
    sensor5=read_adc(5);
    backward();
    pause(425);
    motorsoff();
    if(sensor3<sensor5) //left light sensor get stonger light intensity
    {
    pivotleft();
    pause(125);
    motorsoff();

    }
    else              //left light sensor get stonger light intensity
    {
    pivotright();
    pause(125);
        motorsoff();
    }
    forward();
 }


if ((PORTB & 0x01) == 1) // If all bumpers have cleared their obstacles
{
INTF = 0; // Clear interrupt flag bit
output_high ('A', 4);
output_high ('E', 2);
}
restore_data ( ); //Restore function variables used in main()
return;
}
