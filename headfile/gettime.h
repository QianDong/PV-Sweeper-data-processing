#ifndef GETTIME_H
#define GETTIME_H

#endif // GETTIME_H



#include "time.h"
#include "stdio.h"
#include <iostream>
#include <ctime>
using namespace std;
int year, month, day, hour ,minute ,second, sweep = 0;

// --------------------Declearion of "getsweep" function--------------------------
int getsweep(){
    int s = minute*60 + second;//minute*60 + second;
    return s;
}


// --------------------Declearion of "gettime" function--------------------------
int gettime(void) {               //get Linux system ti0me
    time_t now = time(0);       //get current time, save in "now"
    struct tm * p=localtime(&now);
    year = 1900+p->tm_year-2000;
    month = 1 + p->tm_mon;
    day = p->tm_mday;
    hour = p->tm_hour;
    minute = p->tm_min;
    second = p->tm_sec;
    int swep = getsweep();

    return swep;
}


// -------------------------- Read current ALL data from an entire file ---------------------------------
void readcurrent(){

    qcurrent.clear();
    qvoltage.clear();
    mppcurve.clear();

    filename[0]= char((year/10)+48); filename[1]=char((year%10)+48);
    if (month<10){filename[2]='0';filename[3]=char(month+48);}
    else {filename[2]= char((month/10)+48); filename[3]=char((month%10)+48);}
    if (day<10){filename[4]='0';filename[5]=char(day+48);}
    else {filename[4]= char((day/10)+48); filename[5]=char((day%10)+48);}
    if (hour<10){filename[6]='0';filename[7]=char(hour+48);}
    else {filename[6]= char((hour/10)+48); filename[7]=char((hour%10)+48);}


    strcpy(filepath,"/home/pi/Desktop/data/");
    for (int i=0; i<8; i++){filepath[i+22]=filename[i];}
    filepath[30]='.';
    filepath[31]='t';
    filepath[32]='x';
    filepath[33]='t';
    filepath[34]='\0';

    file = fopen(filepath,"r");
    //if (file == NULL) {return 0;}
    //file = fopen("/home/pi/Desktop/data/F5test.txt","r");     // for testing purpose

    if (file != NULL) {
        while(! feof(file) ) {      //if file is not reaching its end
            //for(int i = 0; i<3600; i++){
            fgets(mystring, LINE_LENGTH,file);  //read file by each line
            sscanf(mystring,"%f,%f,%f", &val1,&val2,&val3);  //save data according to the needs
            if (val3 == -2){
                airtemp.push_back(val1*0.0458-62.786);
                paneltemp.push_back(val2*0.0458-62.632);
            }
            if (val3 == -1){ irradiance.push_back(val1*0.06-49); }
            if (val3 >= 0){
                qcurrent.push_back(val1);        //x is value in 1st column, current
                qvoltage.push_back(val2);       // y is value in 1st column, voltage
                double power=val1*val2;
                mppcurve.push_back(power);
            }
        }
        fclose(file);
        //return 1;
    }
}
