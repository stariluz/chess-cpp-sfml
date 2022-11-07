#include <graphics.h>
#include <math.h>
#include <iostream>
#include <list>
#include "Chess_Graphics.h"
using namespace std;

const char* soldier="soldier.png";
void imageRender(){
    initwindow(800,500,"Chess");
    //readimagefile("soldier.png",0,0,200,200);
    readimagefile("peon.jpg",0,0,200,200);
    getch();
    closegraph();
}
int main(){
    imageRender();

    //int graphDriver=DETECT, graphMode;
    //char* pathToDriver=(char*)"";
    //initgraph(&graphDriver, &graphMode, pathToDriver);

    /*
    Coord<double> myCoord(200,200), myCoord2(45,12);
    Line<double> myLine(myCoord, 100, 180, true);
    //cout<<myLine<<"\n";
    //myLine.show();
    for(double i=0; i<=360; i+=0.5){
        myLine.setLineAtAngle(myCoord, (double) 100, i);
        myLine.show();
    }

    /*
    int option;
    while(option!=-1){
        myLine.show();
        cout<<"OPCIONES: \n";
        cout<<"1. Por puntos\n";
        cout<<"2. Por angulo\n";
        cout<<"3. Agregar linea a...\n";
        cout<<"4. Agregar linea relativa a...\n";
        cout<<"Ingrese su opcion: ";
        cin>>option;
        if(option==1){
            myLine.getConsoleInOfPoints();
            myLine.print();
        }else if(option==2){
            myLine.getConsoleInOfPointAngle();
            myLine.print();
        }else if(option==3){
            Coord<double> aux;
            aux.getConsoleInOfPoint();
            myLine.nextLineTo(aux);
        }else if(option==4){
            Coord<double> aux;
            aux.getConsoleInOfPoint();
            myLine.nextRelativeLineTo(aux);
        }
    }
    */
    return 0;
}

