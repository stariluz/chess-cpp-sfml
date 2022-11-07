#ifndef CHESS_GRAPHICS_H_INCLUDED
#define CHESS_GRAPHICS_H_INCLUDED

using namespace std;

template <typename T>
struct Coord{
    T x=0, y=0;
    Coord(){
        x=0;
        y=0;
    }

    Coord(T x, T y){
        this->x=x;
        this->y=y;
    }
    Coord operator + (const Coord& obj){
        Coord result;
        result.x=x+obj.x;
        result.y=y+obj.y;
        return result;
    }
    Coord operator - (const Coord& obj){
        Coord result;
        result.x=x-obj.x;
        result.y=y-obj.y;
        return result;
    }
    friend ostream& operator <<(ostream &os, Coord &coord){
        os<<"("<<coord.x<<", "<<coord.y<<")";
        return os;
    }
    friend istream& operator >>(istream &iStream, Coord &coord){
        iStream>>coord.x>>coord.y;
        return iStream;
    }
    void print(){
        cout<<*this;
    }

    void getConsoleInOfPoint(){
        cout<<" New line to point (x y): ";
        cin>>(*this);
        cout<<"\n";
    }
};

template <typename T>
struct Line{
    Coord<T> point1, point2;
    list<Coord<T>> points;

    Line(){
        point1(0,0);
        point2(0,0);
    }
    Line(T x1, T y1, T x2, T y2){
        setLineAtPoints(Coord<T>(x1,y1), Coord<T>(x2,y2));
    }
    Line(Coord<T> point1, Coord<T> point2){
        setLineAtPoints(point1, point2);
    }
    Line(Coord<T> point, T length, T angle, bool isAngle=true){
        /**
            CONSTRUCTOR WITH A POINT AND AN ANGLE
        */
        cout<<point<<" "<<length<<" "<<angle;
        setLineAtAngle(point,length,angle);
    }
    Line(T x1, T y1, T length, T angle, bool isAngle=true){
        setLineAtAngle(Coord<T>(x1,y1),length,angle);
    }


    void setLineAtPoints(Coord<T>& point1, Coord<T>& point2){
        this->point1=point1;
        this->point2=point2;
    }

    void nextLineTo(Coord<T>& point){
        points.push_back(point);
        lineto(point.x, point.y);
    }

    void nextRelativeLineTo(Coord<T>& offset){
        Coord<T> aux(getx(), gety());

        points.push_back(aux+offset);
        linerel(offset.x, offset.y);
    }
    void setLineAtAngle(Coord<T> point, T length, T angle){
        double rad=(double) angle*M_PI/180.0;
        double x=point.x+length*cos(rad);
        double y=point.y-length*sin(rad);
        cout<<"POINT: "<<point<<"\n";
        cout<<"ANGLE: "<<angle<<"\n";
        cout<<"RAD: "<<rad<<"\n";
        cout<<"Length: "<<length<<"\n";
        cout<<"X Y: "<<x<<" "<<y<<"\n";
        this->point1=point;
        this->point2=Coord<T>(x, y);
    }

    void getConsoleInOfPoints(){
        cout<<" Line point1 (x y): ";
        cin>>point1;
        cout<<" Line point2 (x y): ";
        cin>>point2;
        cout<<"\n";
    }
    void getConsoleInOfPointAngle(){
        Coord<T> point;
        T length, angle;
        cout<<" Line point (x y): ";
        cin>>point;
        cout<<" Length: ";
        cin>>length;
        cout<<" Angle: ";
        cin>>angle;
        cout<<"\n";
        setLineAtAngle(point, length, angle);
    }

    void print(){
        cout<<*this<<"\n";
    }
    void show(){
        line(
             point1.x, point1.y,
             point2.x, point2.y
        );
    }

    friend ostream& operator <<(ostream &os, Line &line){
        os<<"[ "<<line.point1 <<" - "<<line.point2<<" ]";
        return os;
    }
    friend istream& operator >>(istream &iStream, Line &line){
        iStream>>line.point1>>line.point2;
        return iStream;
    }
};

template <typename T>
struct Square {
    Coord<T> point;
    private: Coord<T> TLPoint;
    T length;
    int referencePoint;
    static const int LEFT_TOP=0;
    static const int CENTER=1;
    int color;

    Square(){
        point=Coord<T>(0,0);
        referencePoint=LEFT_TOP;
    }
    Square(Coord<T> point, T length, const int& referencePoint){
        this->point=point;
        this->referencePoint=referencePoint;
        if(referencePoint==CENTER){
            TLPoint=Coord<T>(point.x-(length/2),point.y-(length/2));
        }else{
            TLPoint=point;
        }
    }

    void draw(){
        rectangle(TLPoint.x, TLPoint.y, TLPoint.x+length, TLPoint.y+length);
        setfillstyle(0, 1);
    }
};

#endif // CHESS_GRAPHICS_H_INCLUDED
