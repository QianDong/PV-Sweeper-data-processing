#ifndef READ_H
#define READ_H

#include <iostream>
#include "stdio.h"
#include <fstream>
#include <vector>


using namespace std;

#endif // READ_H

/* !!!!!!!!!!
 * I didnt save value for temperature!!!!
 * !!!!!!!!!!
 */
//this head file defined a function -- read, which read data so IV curve and other curves could be displayed

const int LINE_LENGTH = 30;
char mystring [LINE_LENGTH];  //every sign in each line from original .txt file is saved as char

//define 2 vectors x() and y() that save cordinates data, has to be double type otherwise can not use graph() to plot
QVector<double> qvoltage;   //qvoltage,qcurrent is queue to store the read data
QVector<double> qcurrent;
QVector<double> mppcurve;       // double 64 bit
QVector<double> irradiance;
QVector<double> airtemp;
QVector<double> paneltemp;

QVector<double> straightIx;
QVector<double> straightIy;
QVector<double> straightVx;
QVector<double> straightVy;

QVector<double> thix(100);
QVector<double> thiy(100);
QVector<double> thimpp(100);

float xsize = 0;
float ysize = 0;
float val1 = 0;         //do I need to change float into double? float is 32 bit
float val2 = 0;
float val3 = 0;
float mpp = 0;
float Isc = 0;
float Voc = 0;
float ImPP = 0;
float VmPP = 0;
float PmPP = 0;
float airT = 0;
float panelT = 0;
float irr = 0;

int yy,mm,dd,hh,mi,ss;
char filename[8],filepath[35];
FILE * file;



// --------------------------- Least Squares Method --------------------------
// seems this is most used for round curve, not precise when predicting straight line
double leastSquare(const QVector <double> &x, const QVector<double> &y, int type){ //have to add const otherwise type will not match
    double a=0;
    double b=0; //y=a*x+b
    double sumXX=0;
    double sumX = 0;
    double sumXY= 0;
    double sumY = 0;
    int n = 0;
    if (type == 1){
        n = 70;
        for (int i=100-n;i<100;i++){ //from x[89] to x[99], last 10 digits
            sumXX += x[i]*x[i];
            sumX  += x[i];
            sumXY += x[i]*y[i];
            sumY  += y[i];
        }
    }
    if (type == 2){
        n = 10;
        for (int i=0;i<n;i++){ //from x[89] to x[99], last 10 digits
            sumXX += x[i]*x[i];
            sumX  += x[i];
            sumXY += x[i]*y[i];
            sumY  += y[i];
        }
    }
    a = (sumXY * n - sumX * sumY) / (sumXX * n - sumX*sumX);
    b = (sumXX * sumY - sumX * sumXY) / (sumXX * n - sumX*sumX);
    // y=a*x+b, when x=0, y=b; when y=0, x=-b/a

    if (type == 1){return b;}
    if (type == 2){return -b/a;}

}


// ---------------------------------- draw y = ImPP straight line------------------------------------------
void drawStraightI(){
    straightIx.clear();
    straightIy.clear();
    for (float i = 0;i<VmPP;i+=0.1){
        straightIx.push_back(i);
        straightIy.push_back(ImPP);
    }
}


// ---------------------------------- draw x = VmPP straight line------------------------------------------
void drawStraightV(){
    straightVx.clear();
    straightVy.clear();
    for (float i = 0;i<ImPP;i+=0.1){
        straightVx.push_back(VmPP);
        straightVy.push_back(i);
    }
}



// --------------------------- Convert double to keep 2 digits after dot --------------------------
//accurate to 2 decimal digits
float accurate2decimal(double x){
    int x100 = x*100;
    float result = x100/100.00;
    return result;
}


// ---------------------------- Initialize thix,thiy,thimpp,xsize,ysize,mpp to be 0 ----------------
void initialIV(){
    thix.clear();
    thiy.clear();
    thimpp.clear();
    xsize = 0;
    ysize = 0;
    mpp = 0;
    ImPP = 0;
    VmPP = 0;
}


// -------------------------- Read ALL data from an entire file ---------------------------------
void read(){

    qcurrent.clear();
    qvoltage.clear();
    mppcurve.clear();

    filename[0]= char((yy/10)+48); filename[1]=char((yy%10)+48);
    if (mm<10){filename[2]='0';filename[3]=char(mm+48);}
    else {filename[2]= char((mm/10)+48); filename[3]=char((mm%10)+48);}
    if (dd<10){filename[4]='0';filename[5]=char(dd+48);}
    else {filename[4]= char((dd/10)+48); filename[5]=char((dd%10)+48);}
    if (hh<10){filename[6]='0';filename[7]=char(hh+48);}
    else {filename[6]= char((hh/10)+48); filename[7]=char((hh%10)+48);}


    strcpy(filepath,"/home/pi/Desktop/data/");
    for (int i=0; i<8; i++){filepath[i+22]=filename[i];}
    filepath[30]='.';
    filepath[31]='t';
    filepath[32]='x';
    filepath[33]='t';
    filepath[34]='\0';

    file = fopen(filepath,"r");
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

            if (val3 == -1){ irradiance.push_back(val1/4096*5/(0.06*49)*1000); }

            if (val3 >= 0){
                qcurrent.push_back(val1);        //x is value in 1st column, current
                qvoltage.push_back(val2);       // y is value in 1st column, voltage
                double power=val1*val2;
                mppcurve.push_back(power);
            }                  
        }
        fclose(file);
    }
}


// --------------------------- get THIS 100 data and save in thix and thiy --------------------------
void get100IV(int count){          //loop is sweep
    initialIV();
    for (int i=count*100; i<count*100+100; i++){          //I am demonstrate only the 100 numbers
        thix.push_back( qvoltage[i]);
        thiy.push_back( qcurrent[i]);
        thimpp.push_back( mppcurve[i]/25);//NB!!!!! mpp curve is NOT in scale!!!!!
        if (xsize < qvoltage[i]) {xsize = qvoltage[i];}
        if (ysize < qcurrent[i]) {ysize = qcurrent[i];}
        if (mpp < mppcurve[i]) {
            mpp = mppcurve[i];
            ImPP = qcurrent[i];
            VmPP = qvoltage[i];}
    }
    Isc = leastSquare(thix,thiy, 1);
    Voc = leastSquare(thix,thiy, 2);
    if (ysize < Isc) ysize = Isc;
    if (xsize < Voc) xsize = Voc;
    Isc = accurate2decimal(Isc);
    Voc = accurate2decimal(Voc);

    ImPP = accurate2decimal(ImPP);
    VmPP = accurate2decimal(VmPP);
    PmPP = accurate2decimal(mpp);

    airT = accurate2decimal(airtemp[count]);
    panelT = accurate2decimal(paneltemp[count]);
    irr = accurate2decimal(irradiance[count]);

    drawStraightI();
    drawStraightV();
}


// --------------------------- get PREVIOUS 100 data and save in thix and thiy --------------------------
int previousIV(int count){        //count previous 100 data and save data in thix/thiy
    initialIV();
    if (count >= 0){ count--;}
    if (count >=0){
        for (int i=count*100; i<count*100+100; i++){          //I am demonstrate only the 100 numbers
            thix.push_back( qvoltage[i]);
            thiy.push_back( qcurrent[i]);
            thimpp.push_back( mppcurve[i]/25);
            if (xsize < qvoltage[i]) {xsize = qvoltage[i];}         if (ysize < qcurrent[i]) {ysize = qcurrent[i];}
            if (ysize < qcurrent[i]) {ysize = qcurrent[i];}
            if (mpp < mppcurve[i]) {mpp = mppcurve[i]; ImPP = qcurrent[i];VmPP = qvoltage[i];}
        }
    Isc = leastSquare(thix,thiy, 1);
    Voc = leastSquare(thix,thiy, 2);
    if (ysize < Isc) ysize = Isc;
    if (xsize < Voc) xsize = Voc;
    Isc = accurate2decimal(Isc);
    Voc = accurate2decimal(Voc);
    ImPP = accurate2decimal(ImPP);
    VmPP = accurate2decimal(VmPP);
    PmPP = accurate2decimal(mpp);

    airT = accurate2decimal(airtemp[count]);
    panelT = accurate2decimal(paneltemp[count]);
    irr = accurate2decimal(irradiance[count]);
    drawStraightI();
    drawStraightV();

    return count;
    }

}



// --------------------------- get NEXT 100 data and save in thix and thiy --------------------------
int nextIV(int count){        //count previous 100 data and save data in thix/thiy
    initialIV();
    count++;
    if (count >=0){
        for (int i=count*100; i<count*100+100; i++){          //I am demonstrate only the 100 numbers
            thix.push_back( qvoltage[i]);
            thiy.push_back( qcurrent[i]);
            thimpp.push_back( mppcurve[i]/25);
            if (xsize < qvoltage[i]) {xsize = qvoltage[i];}
            if (ysize < qcurrent[i]) {ysize = qcurrent[i];}
            if (mpp < mppcurve[i]) {mpp = mppcurve[i]; ImPP = qcurrent[i];VmPP = qvoltage[i];}
        }
    Isc = leastSquare(thix,thiy, 1);
    Voc = leastSquare(thix,thiy, 2);
    if (ysize < Isc) ysize = Isc;
    if (xsize < Voc) xsize = Voc;
    Isc = accurate2decimal(Isc);
    Voc = accurate2decimal(Voc);
    ImPP = accurate2decimal(ImPP);
    VmPP = accurate2decimal(VmPP);
    PmPP = accurate2decimal(mpp);
    airT = accurate2decimal(airtemp[count]);
    panelT = accurate2decimal(paneltemp[count]);
    irr = accurate2decimal(irradiance[count]);
    drawStraightI();
    drawStraightV();

    return count;
    }
}







