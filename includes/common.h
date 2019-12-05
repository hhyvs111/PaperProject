//
// Created by 谭文波 on 2019/12/1.
//

#ifndef LEARNOPENGL_COMMON_H
#define LEARNOPENGL_COMMON_H
#pragma once
//#include "afxtempl.h"
#include "math.h"
#include<list>
#include<vector>
#include<string>
#include<array>
#include <iostream>


typedef std::vector<int>::iterator POSITION;
using namespace std;

#define MaxSize 40000
#define THRESHOLD1 1.0e-1
#define THRESHOLD2 1.0e-2
#define THRESHOLD3 1.0e-3
#define THRESHOLD4 1.0e-4
#define THRESHOLD5 1.0e-5
#define THRESHOLD6 1.0e-6
#define THRESHOLD7 1.0e-7
#define THRESHOLD8 1.0e-8
#define THRESHOLD9 1.0e-9
#define THRESHOLD10 1.0e-10
#define THRESHOLD11 1.0e-11
#define THRESHOLD12 1.0e-12

#define CONDUCT_CURVE_MAX_NUMBER 8

struct PointPosition {
    int type;            //引导点所在曲线(1-开线，2-闭线，3-断层线)
    int curveIndex;      //引导点所在曲线序号
    int pointIndex;      //引导点在曲线上的序号
};
typedef PointPosition ConductPointPos;
//typedef array<PointPosition,PointPosition> ConductPointPosArray;
struct LinkedIndex            //三维对象索引类
{                             //用于对数组中元素建立索引表
    int index;
    LinkedIndex* pNext;
};

class Point2D      //三维坐标点类，并定义了基本的赋值及取值方法。
{
public:
    Point2D(double x=0,double y=0, int id = -1)//modify by huangwei 20009.3.27
    {
        m_x=x;
        m_y=y;
        m_ID = id;
    }
    Point2D(const double a[2])
    {
        m_x=a[0];
        m_y=a[1];
    }
    Point2D(const Point2D &point)
    {
        m_ID=point.m_ID;
        m_x=point.m_x;
        m_y=point.m_y;
    }
    Point2D(Point2D &&point)
    {
        m_ID=point.m_ID;
        m_x=point.m_x;
        m_y=point.m_y;
    }
public:
    int    m_ID;
    double m_x,m_y;
public:
    Point2D operator=(Point2D &point)
    {
        m_ID=point.m_ID;
        m_x=point.m_x;
        m_y=point.m_y;
        return *this;
    }
    bool operator==(Point2D &point)
    {
        double s;
        s=(m_x-point.m_x)*(m_x-point.m_x)+
          (m_y-point.m_y)*(m_y-point.m_y);
        if(s<THRESHOLD10)
            return true;
        return false;
    }
    bool operator!=(Point2D &point)
    {
        double s;
        s=(m_x-point.m_x)*(m_x-point.m_x)+
          (m_y-point.m_y)*(m_y-point.m_y);
        if(s<THRESHOLD10)
            return false;
        return true;
    }
    void Set(double x,double y)
    {
        m_x=x;
        m_y=y;
    }
    void Set(double a[2])
    {
        m_x=a[0];
        m_y=a[1];
    }
    void Get(double &x,double &y)
    {
        x=m_x;
        y=m_y;
    }
    void Get(double a[2])
    {
        a[0]=m_x;
        a[1]=m_y;
    }
    void translate(double x,double y)  //对点进行平移，x,y表示平移量
    {
        m_x+=x;
        m_y+=y;
    }
    void translate(double a[2])        //对点进行平移，数组a[]表示平移量
    {
        m_x+=a[0];
        m_y+=a[1];
    }
};
class Point3D:public Point2D
{
public:
    Point3D(double x=0,double y=0,double z=0, int id = -1):Point2D(x,y,id)//modify by hw 2009.3.27
    {
        m_z=z;
    }
    Point3D(double a[3]):Point2D(a)
    {
        m_z=a[2];
    }
    Point3D(const Point3D& point)
    {
        m_ID=point.m_ID;
        m_x=point.m_x;
        m_y=point.m_y;
        m_z=point.m_z;
    }
    Point3D(Point3D&& point)
    {
        m_ID=point.m_ID;
        m_x=point.m_x;
        m_y=point.m_y;
        m_z=point.m_z;
    }

    void Debug(){
        cout << m_x << " " << m_y << " " << m_z << endl;
    }

public:
    double m_z;
public:
    Point3D operator=(Point2D &point)
    {
        m_ID=point.m_ID;
        m_x=point.m_x;
        m_y=point.m_y;
        return *this;
    }
//    Point3D operator=(Point3D &point)
//    {
//        m_ID=point.m_ID;
//        m_x=point.m_x;
//        m_y=point.m_y;
//        m_z=point.m_z;
//        return *this;
//    }
    Point3D operator=(Point3D point)
    {
        m_ID=point.m_ID;
        m_x=point.m_x;
        m_y=point.m_y;
        m_z=point.m_z;
        return *this;
    }



    bool operator==(Point2D &point)
    {
        double s;
        s=  (m_x-point.m_x)*(m_x-point.m_x)+
            (m_y-point.m_y)*(m_y-point.m_y);
        if(s<THRESHOLD10)
            return true;
        return false;
    }
    bool operator==(Point3D &point)
    {
        double s;
        s=  (m_x-point.m_x)*(m_x-point.m_x)+
            (m_y-point.m_y)*(m_y-point.m_y)+
            (m_z-point.m_z)*(m_z-point.m_z);
        if(s<THRESHOLD10)
            return true;
        return false;
    }
    bool operator!=(Point2D &point)
    {
        double s;
        s=  (m_x-point.m_x)*(m_x-point.m_x)+
            (m_y-point.m_y)*(m_y-point.m_y);
        if(s<THRESHOLD10)
            return false;
        return true;
    }
    bool operator!=(Point3D &point)
    {
        double s;
        s=  (m_x-point.m_x)*(m_x-point.m_x)+
            (m_y-point.m_y)*(m_y-point.m_y)+
            (m_z-point.m_z)*(m_z-point.m_z);
        if(s<THRESHOLD10)
            return false;
        return true;
    }
    void Set(double x,double y,double z)
    {
        m_x=x;
        m_y=y;
        m_z=z;
    }
    void Set(double a[3])
    {
        m_x=a[0];
        m_y=a[1];
        m_z=a[2];
    }
    void Get(double &x,double &y,double &z)
    {
        x=m_x;
        y=m_y;
        z=m_z;
    }
    void Get(double a[3])
    {
        a[0]=m_x;
        a[1]=m_y;
        a[2]=m_z;
    }
    void translate(double x,double y,double z)   //对点进行平移，x,y表示平移量
    {
        m_x+=x;
        m_y+=y;
        m_z+=z;
    }
    void translate(double a[3])                  //对点进行平移，数组a[]表示平移量
    {
        m_x+=a[0];
        m_y+=a[1];
        m_z+=a[2];
    }
};
class Point4D:public Point3D
{
public:
    Point4D(double x=0,double y=0,double z=0,double w=0):Point3D(x,y,z)
    {
        m_w=w;
    }
    Point4D(double a[4]):Point3D(a)
    {
        m_w=a[3];
    }
    Point4D(const Point4D &point)
    {
        m_ID=point.m_ID;
        m_x=point.m_x;
        m_y=point.m_y;
        m_z=point.m_z;
        m_w=point.m_w;
    }
public:
    double m_w;
public:
    Point4D operator=(Point3D &point)
    {
        m_ID=point.m_ID;
        m_x=point.m_x;
        m_y=point.m_y;
        m_z=point.m_z;
        return *this;
    }
    Point4D operator=(Point4D &point)
    {
        m_ID=point.m_ID;
        m_x=point.m_x;
        m_y=point.m_y;
        m_z=point.m_z;
        m_w=point.m_w;
        return *this;
    }
    bool operator==(Point3D &point)
    {
        double s;
        s=  (m_x-point.m_x)*(m_x-point.m_x)+
            (m_y-point.m_y)*(m_y-point.m_y)+
            (m_z-point.m_z)*(m_z-point.m_z);
        if(s<THRESHOLD10)
            return true;
        return false;
    }
    bool operator==(Point4D &point)
    {
        double s;
        s=  (m_x-point.m_x)*(m_x-point.m_x)+
            (m_y-point.m_y)*(m_y-point.m_y)+
            (m_z-point.m_z)*(m_z-point.m_z)+
            (m_w-point.m_w)*(m_w-point.m_w);
        if(s<THRESHOLD10)
            return true;
        return false;
    }
    bool operator!=(Point3D &point)
    {
        double s;
        s=  (m_x-point.m_x)*(m_x-point.m_x)+
            (m_y-point.m_y)*(m_y-point.m_y)+
            (m_z-point.m_z)*(m_z-point.m_z);
        if(s<THRESHOLD10)
            return false;
        return true;
    }
    bool operator!=(Point4D &point)
    {
        double s;
        s=  (m_x-point.m_x)*(m_x-point.m_x)+
            (m_y-point.m_y)*(m_y-point.m_y)+
            (m_z-point.m_z)*(m_z-point.m_z)+
            (m_w-point.m_w)*(m_w-point.m_w);
        if(s<THRESHOLD10)
            return false;
        return true;
    }
    void Set(double x,double y,double z,double w)
    {
        m_x=x;
        m_y=y;
        m_z=z;
        m_w=w;
    }
    void Set(double a[4])
    {
        m_x=a[0];
        m_y=a[1];
        m_z=a[2];
        m_w=a[3];
    }
    void Get(double &x,double &y,double &z,double &w)
    {
        x=m_x;
        y=m_y;
        z=m_z;
        w=m_w;
    }
    void Get(double a[4])
    {
        a[0]=m_x;
        a[1]=m_y;
        a[2]=m_z;
        a[3]=m_w;
    }
    void translate(double x,double y,double z,double w)  //对点进行平移，x,y表示平移量
    {
        m_x+=x;
        m_y+=y;
        m_z+=z;
        m_w+=w;
    }
    void translate(double a[4])                          //对点进行平移，数组a[]表示平移量
    {
        m_x+=a[0];
        m_y+=a[1];
        m_z+=a[2];
        m_w+=a[3];
    }
};
class Vector2D:public Point2D
{
public:
    Vector2D(double x=0,double y=1):Point2D(x,y){}
    Vector2D(double a[2]):Point2D(a){}
    Vector2D(Vector2D &vector)
    {
        m_ID=vector.m_ID;
        m_x=vector.m_x ;
        m_y=vector.m_y ;
    }
public:
    Vector2D operator=(Vector2D &vector)
    {
        m_ID=vector.m_ID;
        m_x=vector.m_x ;
        m_y=vector.m_y ;
        return *this;
    }
    Vector2D operator+=(Vector2D &vector)
    {
        m_x+=vector.m_x ;
        m_y+=vector.m_y ;
        return *this;
    }
    Vector2D operator+=(double a[2])
    {
        m_x+=a[0] ;
        m_y+=a[1] ;
        return *this;
    }
    Vector2D operator-=(Vector2D &vector)
    {
        m_x-=vector.m_x ;
        m_y-=vector.m_y ;
        return *this;
    }
    Vector2D operator-=(double a[2])
    {
        m_x-=a[0] ;
        m_y-=a[1] ;
        return *this;
    }
    static double DotProduct(double x1,double y1,
                             double x2,double y2)
    {
        return (x1*x2+y1*y2);
    }
    static double DotProduct(double a[2],double b[2])
    {
        return (a[0]*b[0]+a[1]*b[1]);
    }
    static double DotProduct(Vector2D &v1,Vector2D &v2)
    {
        return (v1.m_x *v2.m_x +v1.m_y *v2.m_y );
    }
    static double CrossProduct(double x1,double y1,
                               double x2,double y2)
    {
        return (x1*y2-x2*y1);
    }
    static double CrossProduct(double a[2],double b[2])
    {
        return (a[0]*b[1]-a[1]*b[0]);
    }
    static double CrossProduct(Vector2D &v1,Vector2D &v2)
    {
        return (v1.m_x *v2.m_y -v2.m_x*v1.m_y  );
    }
    double SquareLength()
    {
        return (m_x*m_x+m_y*m_y);
    }
    static double SquareLength(double x,double y)
    {
        return (x*x+y*y);
    }
    static double SquareLength(double a[2])
    {
        return (a[0]*a[0]+a[1]*a[1]);
    }
    bool Normalize()   //可以单位化向量时单位化向量，否则将向量清零
    {
        double t=SquareLength();
        if(t<THRESHOLD10)
        {
            m_x=0.0;
            m_y=0.0;
            return false;
        }
        t=sqrt(t);
        m_x/=t;
        m_y/=t;
        return true;
    }
    bool Normalize(double &x,double &y) //单位化向量(x,y)
    {
        Set(x,y);
        bool flag=Normalize();
        x=m_x,y=m_y;
        return flag;
    }
    bool Normalize(double a[2])         //单位化向量a
    {
        Set(a);
        bool flag=Normalize();
        a[0]=m_x,a[1]=m_y;
        return flag;
    }
};
class Vector3D:public Point3D
{
public:
    Vector3D(double x=0,double y=0,double z=1):Point3D(x,y,z){}
    Vector3D(double a[3]):Point3D(a){}
    Vector3D(Point3D p):Point3D(p){}
    Vector3D(Vector3D &vector)
    {
        m_ID=vector.m_ID;
        m_x=vector.m_x ;
        m_y=vector.m_y ;
        m_z=vector.m_z;
    }
public:
//    Vector3D operator=(Vector3D &vector)
//    {
//        m_ID=vector.m_ID;
//        m_x=vector.m_x ;
//        m_y=vector.m_y ;
//        m_z=vector.m_z;
//        return *this;
//    }
    Vector3D operator=(Vector3D vector)
    {
        m_ID=vector.m_ID;
        m_x=vector.m_x ;
        m_y=vector.m_y ;
        m_z=vector.m_z;
        return *this;
    }
    Vector3D operator+=(Vector3D &vector)
    {
        m_x+=vector.m_x ;
        m_y+=vector.m_y ;
        m_z+=vector.m_z;
        return *this;
    }
    Vector3D operator+=(double a[3])
    {
        m_x+=a[0] ;
        m_y+=a[1] ;
        m_z+=a[2];
        return *this;
    }
    Vector3D operator-=(Vector3D &vector)
    {
        m_x-=vector.m_x ;
        m_y-=vector.m_y ;
        m_z-=vector.m_z;
        return *this;
    }
    Vector3D operator-=(double a[3])
    {
        m_x-=a[0] ;
        m_y-=a[1] ;
        m_z-=a[2];
        return *this;
    }

    bool operator==(Vector3D v1){
        return this->m_x == v1.m_x && this->m_y == v1.m_y && this->m_z == v1.m_z;
    }

    static double DotProduct(double x1,double y1,double z1,
                             double x2,double y2,double z2)
    {
        return (x1*x2+y1*y2+z1*z2);
    }
    static double DotProduct(double a[3],double b[3])
    {
        return (a[0]*b[0]+a[1]*b[1]+a[2]*b[2]);
    }
    static double DotProduct(Vector3D &v1,Vector3D &v2)
    {
        return (v1.m_x *v2.m_x +v1.m_y *v2.m_y +v1.m_z *v2.m_z );
    }
    Vector3D CrossProduct(double x1,double y1,double z1,
                          double x2,double y2,double z2)
    {
        m_x=y1*z2-y2*z1;
        m_y=x2*z1-x1*z2;
        m_z=x1*y2-x2*y1;
        return *this;
    }
    Vector3D CrossProduct(double a[3],double b[3])
    {
        m_x =a[1]*b[2]-b[1]*a[2];
        m_y =b[0]*a[2]-a[0]*b[2];
        m_z =a[0]*b[1]-b[0]*a[1];
        return *this;
    }
    Vector3D CrossProduct(Vector3D &v1,Vector3D &v2)
    {
        double a[3],b[3];
        v1.Get (a);
        v2.Get (b);
        m_x =a[1]*b[2]-b[1]*a[2];
        m_y =b[0]*a[2]-a[0]*b[2];
        m_z =a[0]*b[1]-b[0]*a[1];
        return *this;
    }
    static void CrossProduct(double x1,double y1,double z1,
                             double x2,double y2,double z2,Vector3D &v)
    {
        v.m_x=y1*z2-y2*z1;
        v.m_y=x2*z1-x1*z2;
        v.m_z=x1*y2-x2*y1;
    }
    static void CrossProduct(double a[3],double b[3],Vector3D &v)
    {
        v.m_x =a[1]*b[2]-b[1]*a[2];
        v.m_y =b[0]*a[2]-a[0]*b[2];
        v.m_z =a[0]*b[1]-b[0]*a[1];
    }
    static void CrossProduct(Vector3D &v1,Vector3D &v2,Vector3D &v)
    {
        double a[3],b[3];
        v1.Get (a);
        v2.Get (b);
        v.m_x =a[1]*b[2]-b[1]*a[2];
        v.m_y =b[0]*a[2]-a[0]*b[2];
        v.m_z =a[0]*b[1]-b[0]*a[1];
    }
    double SquareLength()
    {
        return (m_x*m_x+m_y*m_y+m_z*m_z);
    }
    static double SquareLength(double x,double y,double z)
    {
        return (x*x+y*y+z*z);
    }
    static double SquareLength(double a[3])
    {
        return (a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    }
    bool Normalize()                //单位化或清零
    {
        double t=SquareLength();
//        cout << "normalize t " << t << endl;
        if(t<THRESHOLD10)
        {
//            cout << "normalize failed" << endl;
            m_x=0,m_y=0,m_z=0;
            return false;
        }
        t=sqrt(t);
        m_x/=t;
        m_y/=t;
        m_z/=t;
        return true;
    }
    bool Normalize(double &x,double &y,double &z)
    {
        Set(x,y,z);
        bool flag=Normalize();
        x=m_x,y=m_y,z=m_z;
        return flag;
    }
    bool Normalize(double a[3])
    {
        Set(a);
        bool flag= Normalize();
        a[0]=m_x,a[1]=m_y,a[2]=m_z;
        return flag;
    }
};
class Line2D       //三维线段类
{
public:
    Line2D(double x1=0,double y1=0,
           double x2=0,double y2=0)
    {
        m_x1=x1,m_y1=y1;
        m_x2=x2,m_y2=y2;
    }
    Line2D(double a[],double b[])
    {
        m_x1=a[0],m_y1=a[1];
        m_x2=b[0],m_y2=b[1];
    }
    Line2D(Point2D &point1,Point2D &point2)
    {
        m_x1=point1.m_x,m_y1=point1.m_y;
        m_x2=point2.m_x,m_y2=point2.m_y ;
    }
    Line2D(const Line2D &line)
    {
        m_ID=line.m_ID;
        m_x1=line.m_x1,m_y1=line.m_y1;
        m_x2=line.m_x2,m_y2=line.m_y2;
    }
    Line2D(Line2D &&line)
    {
        m_ID=line.m_ID;
        m_x1=line.m_x1,m_y1=line.m_y1;
        m_x2=line.m_x2,m_y2=line.m_y2;
    }
public:
    int    m_ID;
    double m_x1,m_y1;
    double m_x2,m_y2;
public:
    Line2D operator=(Line2D &line)
    {
        m_ID=line.m_ID;
        m_x1=line.m_x1,m_y1=line.m_y1;
        m_x2=line.m_x2,m_y2=line.m_y2;
        return *this;
    }
    int operator==(Line2D &line);                //如果重合，方向一致时返回1，不一致时返回-1
    int operator!=(Line2D &line);                //如果不重合，返回1
    int HasVertex(double x,double y);            //判断是否为端点,返回0表示不是端点
    int HasVertex(double a[2]);	                 //判断是否为端点,返回1表示在起点
    int HasVertex(Point2D &point);
    int Contain(double x,double y);              //判断是否在线上,返回1表示在起点
    int Contain(double a[2]);                    //判断是否在线上,返回2表示在终点
    int Contain(Point2D &point);                 //判断是否在线上,返回3表示在两端点之间

    void Set(double x1,double y1,
             double x2,double y2)
    {
        m_x1=x1,m_y1=y1;
        m_x2=x2,m_y2=y2;
    }
    void Set(double a[2],double b[2])
    {
        m_x1=a[0],m_y1=a[1];
        m_x2=b[0],m_y2=b[1];
    }
    void Set(Point2D &point1,Point2D &point2)
    {
        m_x1=point1.m_x,m_y1=point1.m_y;
        m_x2=point2.m_x,m_y2=point2.m_y;
    }
    void Get(double &x1,double &y1,
             double &x2,double &y2)
    {
        x1=m_x1,y1=m_y1;
        x2=m_x2,y2=m_y2;
    }
    void Get(double a[2],double b[2])
    {
        a[0]=m_x1,a[1]=m_y1;
        b[0]=m_x2,b[1]=m_y2;
    }
    void Get(Point2D &point1,Point2D &point2)
    {
        point1.m_x=m_x1,point1.m_y=m_y1;
        point2.m_x=m_x2,point2.m_y=m_y2;
    }
    void Translate(double x,double y)
    {
        m_x1+=x,m_y1+=y;
        m_x2+=x,m_y2+=y;
    }
    void Translate(double a[])            //直线平移，数组a[]表示平移量
    {
        m_x1+=a[0],m_y1+=a[1];
        m_x2+=a[0],m_y2+=a[1];
    }
    double SquareLength()
    {
        double d=(m_x1-m_x2)*(m_x1-m_x2)+
                 (m_y1-m_y2)*(m_y1-m_y2);
        return d;
    }
    double Length()
    {
        double d=(m_x1-m_x2)*(m_x1-m_x2)+
                 (m_y1-m_y2)*(m_y1-m_y2);
        return sqrt(d);
    }
};
class Line3D:public Line2D          //三维线段类
{
public:
    Line3D(double x1=0,double y1=0,double z1=0,
           double x2=0,double y2=0,double z2=0)
    {
        m_x1=x1,m_y1=y1,m_z1=z1;
        m_x2=x2,m_y2=y2,m_z2=z2;
    }
    Line3D(double a[],double b[])
    {
        m_x1=a[0],m_y1=a[1],m_z1=a[2];
        m_x2=b[0],m_y2=b[1],m_z2=b[2];
    }
    Line3D(Point3D &point1,Point3D &point2)
    {
        m_x1=point1.m_x,m_y1=point1.m_y,m_z1=point1.m_z;
        m_x2=point2.m_x,m_y2=point2.m_y,m_z2=point2.m_z;
    }
    Line3D(const Line3D &line)
    {
        m_ID=line.m_ID;
        m_x1=line.m_x1,m_y1=line.m_y1,m_z1=line.m_z1;
        m_x2=line.m_x2,m_y2=line.m_y2,m_z2=line.m_z2;
    }
    Line3D(Line3D &&line){
        m_ID=line.m_ID;
        m_x1=line.m_x1,m_y1=line.m_y1,m_z1=line.m_z1;
        m_x2=line.m_x2,m_y2=line.m_y2,m_z2=line.m_z2;
    }
public:
    double m_z1;
    double m_z2;
public:
    Line3D operator=(Line2D &line)
    {
        m_ID=line.m_ID;
        m_x1=line.m_x1,m_y1=line.m_y1;
        m_x2=line.m_x2,m_y2=line.m_y2;
        return *this;
    }
    Line3D operator=(Line3D &&line)
    {
        m_ID=line.m_ID;
        m_x1=line.m_x1,m_y1=line.m_y1,m_z1=line.m_z1;
        m_x2=line.m_x2,m_y2=line.m_y2,m_z2=line.m_z2;
        return *this;
    }
    Line3D operator=(Line3D &line)
    {
        m_ID=line.m_ID;
        m_x1=line.m_x1,m_y1=line.m_y1,m_z1=line.m_z1;
        m_x2=line.m_x2,m_y2=line.m_y2,m_z2=line.m_z2;
        return *this;
    }
    int operator==(Line2D &line);                //如果重合，方向一致时返回1，不一致时返回-1
    int operator==(Line3D &line);
    int operator!=(Line2D &line);                //如果不重合，返回1
    int operator!=(Line3D &line);
    int HasVertex(double x,double y,double z);   //判断是否为端点,返回0表示不是端点
    int HasVertex(double a[3]);	                 //判断是否为端点,返回1表示在起点
    int HasVertex(Point3D &point);
    int Contain(double x,double y,double z);     //判断是否在线上,返回1表示在起点
    int Contain(double a[3]);                    //判断是否在线上,返回2表示在终点
    int Contain(Point3D &point);                 //判断是否在线上,返回3表示在两端点之间

    void Set(double x1,double y1,double z1,
             double x2,double y2,double z2)
    {
        m_x1=x1,m_y1=y1,m_z1=z1;
        m_x2=x2,m_y2=y2,m_z2=z2;
    }
    void Set(double a[3],double b[3])
    {
        m_x1=a[0],m_y1=a[1],m_z1=a[2];
        m_x2=b[0],m_y2=b[1],m_z2=b[2];
    }
    void Set(Point3D &point1,Point3D &point2)
    {
        m_x1=point1.m_x,m_y1=point1.m_y,m_z1=point1.m_z;
        m_x2=point2.m_x,m_y2=point2.m_y,m_z2=point2.m_z;
    }
    void Get(double &x1,double &y1,double &z1,
             double &x2,double &y2,double &z2)
    {
        x1=m_x1,y1=m_y1,z1=m_z1;
        x2=m_x2,y2=m_y2,z2=m_z2;
    }
    void Get(double a[3],double b[3])
    {
        a[0]=m_x1,a[1]=m_y1,a[2]=m_z1;
        b[0]=m_x2,b[1]=m_y2,b[2]=m_z2;
    }
    void Get(Point3D &point1,Point3D &point2)
    {
        point1.m_x=m_x1,point1.m_y=m_y1,point1.m_z=m_z1;
        point2.m_x=m_x2,point2.m_y=m_y2,point2.m_z=m_z2;
    }
    void Translate(double x,double y,double z)
    {
        m_x1+=x,m_y1+=y,m_z1+=z;
        m_x2+=x,m_y2+=y,m_z2+=z;
    }
    void Translate(double a[3])            //直线平移，数组a[]表示平移量
    {
        m_x1+=a[0],m_y1+=a[1],m_z1+=a[2];
        m_x2+=a[0],m_y2+=a[1],m_z2+=a[2];
    }
    double SquareLength()
    {
        double d=(m_x1-m_x2)*(m_x1-m_x2)+
                 (m_y1-m_y2)*(m_y1-m_y2)+
                 (m_z1-m_z2)*(m_z1-m_z2);
        return d;
    }
    double Length()
    {
        double d=(m_x1-m_x2)*(m_x1-m_x2)+
                 (m_y1-m_y2)*(m_y1-m_y2)+
                 (m_z1-m_z2)*(m_z1-m_z2);
        return sqrt(d);
    }
};
//class Line4D:public Line3D          //三维线段类
//{
//public:
//    Line4D(double x1=0,double y1=0,double z1=0,double w1=0,
//           double x2=0,double y2=0,double z2=0,double w2=0)
//    {
//        m_x1=x1,m_y1=y1,m_z1=z1,m_w1=w1;
//        m_x2=x2,m_y2=y2,m_z2=z2,m_w2=w2;
//    }
//    Line4D(double a[],double b[])
//    {
//        m_x1=a[0],m_y1=a[1],m_z1=a[2],m_w1=a[3];
//        m_x2=b[0],m_y2=b[1],m_z2=b[2],m_w2=b[3];
//    }
//    Line4D(Point4D &point1,Point4D &point2)
//    {
//        m_x1=point1.m_x,m_y1=point1.m_y,m_z1=point1.m_z,m_w1=point1.m_w;
//        m_x2=point2.m_x,m_y2=point2.m_y,m_z2=point2.m_z,m_w2=point2.m_w;
//    }
//    Line4D(Line4D &line)
//    {
//        m_ID=line.m_ID;
//        m_x1=line.m_x1,m_y1=line.m_y1,m_z1=line.m_z1,m_w1=line.m_w1;
//        m_x2=line.m_x2,m_y2=line.m_y2,m_z2=line.m_z2,m_w2=line.m_w2;
//    }
//public:
//    double m_w1;
//    double m_w2;
//public:
//    Line4D operator=(Line3D &line)
//    {
//        m_ID=line.m_ID;
//        m_x1=line.m_x1,m_y1=line.m_y1,m_z1=line.m_z1;
//        m_x2=line.m_x2,m_y2=line.m_y2,m_z2=line.m_z2;
//        return *this;
//    }
//    Line4D operator=(Line4D &line)
//    {
//        m_ID=line.m_ID;
//        m_x1=line.m_x1,m_y1=line.m_y1,m_z1=line.m_z1,m_w1=line.m_w1;
//        m_x2=line.m_x2,m_y2=line.m_y2,m_z2=line.m_z2,m_w2=line.m_w2;
//        return *this;
//    }
//    int operator==(Line3D &line);                       //如果重合，方向一致时返回1，不一致时返回-1
//    int operator==(Line4D &line);
//    int operator!=(Line3D &line);                       //如果不重合，返回1
//    int operator!=(Line4D &line);
//    int HasVertex(double x,double y,double z,double w); //判断是否为端点,返回0表示不是端点
//    int HasVertex(double a[4]);	                        //判断是否为端点,返回1表示在起点
//    int HasVertex(Point4D &point);                      //判断是否为端点,返回1表示在终点
//    int Contain(double x,double y,double z,double w);   //判断是否在线上,返回1表示在起点
//    int Contain(double a[4]);                           //判断是否在线上,返回2表示在终点
//    int Contain(Point4D &point);                        //判断是否在线上,返回3表示在两端点之间
//
//    void Set(double x1,double y1,double z1,double w1,
//             double x2,double y2,double z2,double w2)
//    {
//        m_x1=x1,m_y1=y1,m_z1=z1,m_w1=w1;
//        m_x2=x2,m_y2=y2,m_z2=z2,m_w2=w2;
//    }
//    void Set(double a[4],double b[4])
//    {
//        m_x1=a[0],m_y1=a[1],m_z1=a[2],m_w1=a[3];
//        m_x2=b[0],m_y2=b[1],m_z2=b[2],m_w2=b[3];
//    }
//    void Set(Point4D &point1,Point4D &point2)
//    {
//        m_x1=point1.m_x,m_y1=point1.m_y,m_z1=point1.m_z,m_w1=point1.m_w;
//        m_x2=point2.m_x,m_y2=point2.m_y,m_z2=point2.m_z,m_w2=point2.m_w;
//    }
//    void Get(double &x1,double &y1,double &z1,double &w1,
//             double &x2,double &y2,double &z2,double &w2)
//    {
//        x1=m_x1,y1=m_y1,z1=m_z1,w1=m_w1;
//        x2=m_x2,y2=m_y2,z2=m_z2,w2=m_w2;
//    }
//    void Get(double a[4],double b[4])
//    {
//        a[0]=m_x1,a[1]=m_y1,a[2]=m_z1,a[3]=m_w1;
//        b[0]=m_x2,b[1]=m_y2,b[2]=m_z2,b[3]=m_w2;
//    }
//    void Get(Point4D &point1,Point4D &point2)
//    {
//        point1.m_x=m_x1,point1.m_y=m_y1,point1.m_z=m_z1,point1.m_w=m_w1;
//        point2.m_x=m_x2,point2.m_y=m_y2,point2.m_z=m_z2,point2.m_w=m_w2;
//    }
//    void Translate(double x,double y,double z,double w)
//    {
//        m_x1+=x,m_y1+=y,m_z1+=z,m_w1+=w;
//        m_x2+=x,m_y2+=y,m_z2+=z,m_w2+=w;
//    }
//    void Translate(double a[4])            //直线平移，数组a[]表示平移量
//    {
//        m_x1+=a[0],m_y1+=a[1],m_z1+=a[2],m_w1+=a[3];
//        m_x2+=a[0],m_y2+=a[1],m_z2+=a[2],m_w1+=a[3];
//    }
//    double SquareLength()
//    {
//        double d=(m_x1-m_x2)*(m_x1-m_x2)+
//                 (m_y1-m_y2)*(m_y1-m_y2)+
//                 (m_z1-m_z2)*(m_z1-m_z2)+
//                 (m_w1-m_w2)*(m_w1-m_w2);
//        return d;
//    }
//    double Length()
//    {
//        double d=(m_x1-m_x2)*(m_x1-m_x2)+
//                 (m_y1-m_y2)*(m_y1-m_y2)+
//                 (m_z1-m_z2)*(m_z1-m_z2)+
//                 (m_w1-m_w2)*(m_w1-m_w2);
//        return sqrt(d);
//    }
//};
class Line
{
public:
    Line(int i=-1,int j=-1)
    {
        m_pointIndex1=i;
        m_pointIndex2=j;
    }
    Line(Line &line)
    {
        m_ID=line.m_ID;
        m_pointIndex1=line.m_pointIndex1 ;
        m_pointIndex2=line.m_pointIndex2 ;
    }
    Line(Line &&line)
    {
        m_ID=line.m_ID;
        m_pointIndex1=line.m_pointIndex1 ;
        m_pointIndex2=line.m_pointIndex2 ;
    }
public:
    int  m_ID;
    int  m_pointIndex1;   //起点序号
    int  m_pointIndex2;   //终点序号
public:
    Line operator=(Line &line)
    {
        m_ID=line.m_ID;
        m_pointIndex1=line.m_pointIndex1 ;
        m_pointIndex2=line.m_pointIndex2 ;
        return *this;
    }
};
typedef unsigned char byte;
class mcEdge:public Line
{
public:
    mcEdge(int i=-1,int j=-1,int k=-1,int s=-1,int t=-1,byte bf=0,byte ef=0):Line(i,j)
    {
        m_index=k;
        m_triangleIndex1=s;
        m_triangleIndex2=t;
        m_bFlag=bf;
        m_eFlag=ef;
    }
    mcEdge(const mcEdge &e)
    {
        m_ID=e.m_ID;
        m_pointIndex1=e.m_pointIndex1 ;
        m_pointIndex2=e.m_pointIndex2 ;
        m_triangleIndex1=e.m_triangleIndex1 ;
        m_triangleIndex2=e.m_triangleIndex2 ;
        m_index=e.m_index ;
        m_bFlag=e.m_bFlag ;
        m_eFlag=e.m_eFlag ;
    }
    mcEdge(mcEdge &&e)
    {
        m_ID=e.m_ID;
        m_pointIndex1=e.m_pointIndex1 ;
        m_pointIndex2=e.m_pointIndex2 ;
        m_triangleIndex1=e.m_triangleIndex1 ;
        m_triangleIndex2=e.m_triangleIndex2 ;
        m_index=e.m_index ;
        m_bFlag=e.m_bFlag ;
        m_eFlag=e.m_eFlag ;
    }
public:
    int  m_ID;
    int  m_triangleIndex1;
    int  m_triangleIndex2;
    int  m_index;         //保留
    byte m_bFlag;         //边界边标志
    byte m_eFlag;         //等值边标志
public:
    mcEdge operator=(mcEdge &e)
    {
        m_ID=e.m_ID;
        m_pointIndex1=e.m_pointIndex1 ;
        m_pointIndex2=e.m_pointIndex2 ;
        m_triangleIndex1=e.m_triangleIndex1 ;
        m_triangleIndex2=e.m_triangleIndex2 ;
        m_index=e.m_index ;
        m_bFlag=e.m_bFlag ;
        m_eFlag=e.m_eFlag ;
        return *this;
    }
};
class Triangle2D
{
public:
    Triangle2D(double x1=0,double y1=0,double x2=0,double y2=0,double x3=0,double y3=0)
    {
        m_x1=x1,m_y1=y1;
        m_x2=x2,m_y2=y2;
        m_x3=x3,m_y3=y3;
    }
    Triangle2D(double a[2],double b[2],double c[2])
    {
        m_x1=a[0],m_y1=a[1];
        m_x2=b[0],m_y2=b[1];
        m_x3=c[0],m_y3=c[1];
    }
    Triangle2D(Point2D &point1,Point2D &point2,Point2D &point3)
    {
        m_x1=point1.m_x,m_y1=point1.m_y;
        m_x2=point2.m_x,m_y2=point2.m_y;
        m_x3=point3.m_x,m_y3=point3.m_y;
    }
    Triangle2D(Triangle2D &triangle)
    {
        m_ID=triangle.m_ID;
        m_x1=triangle.m_x1,m_y1=triangle.m_y1;
        m_x2=triangle.m_x2,m_y2=triangle.m_y2;
        m_x3=triangle.m_x3,m_y3=triangle.m_y3;
    }
public:
    int    m_ID;
    double m_x1,m_y1;
    double m_x2,m_y2;
    double m_x3,m_y3;
public:
    Triangle2D operator=(Triangle2D &triangle)
    {
        m_ID=triangle.m_ID;
        m_x1=triangle.m_x1,m_y1=triangle.m_y1;
        m_x2=triangle.m_x2,m_y2=triangle.m_y2;
        m_x3=triangle.m_x3,m_y3=triangle.m_y3;
        return *this;
    }
    int operator==(Triangle2D &triangle);
    int operator!=(Triangle2D &triangle);
    int HasVertex(double x,double y);                  //判断坐标点是否为三角形的顶点，返回值为0，1，2，或3
    int HasVertex(double a[2]);                        //非零返回值表示坐标点为三角形的某个顶点
    int HasVertex(Point2D &point);	                   //返回值的大小表示三角形顶点的相应序号
    int HasEdge(double x1,double y1,                   //判断线段是否为三角形的一条边，返回1，2，或3表示该线段所在边的序号,线段与边同向
                double x2,double y2 );              //返回-1，-2，或-3表示所在边的序号的负值,线段与边反向
    int HasEdge(double a[2],double b[2]);              //返回0表示线段不是三角形的一条边
    int HasEdge(Point2D &point1,Point2D &point2);
    int HasEdge(Line2D &line);
    int Contain(double x,double y);                    //判断坐标点是否在三角形上，返回1，2，或3表示该点所在三角形顶点的序号
    int Contain(double a[2]);                          //返回4，5，或6表示该点所在三角形边的序号
    int Contain(Point2D &point);                       //返回7表示该点在三角形内部，返回0表示不在三角形上

    //int GetInnerCircle(double &x, double &y,double &r);
    //int GetOuterCircle(double &x, double &y, double &r);

    void Set(double a[2],double b[2],double c[2])
    {
        m_x1=a[0],m_y1=a[1];
        m_x2=b[0],m_y2=b[1];
        m_x3=c[0],m_y3=c[1];
    }
    void Set(Point2D &point1,Point2D &point2,Point2D &point3)
    {
        m_x1=point1.m_x,m_y1=point1.m_y;
        m_x2=point2.m_x,m_y2=point2.m_y;
        m_x3=point3.m_x,m_y3=point3.m_y;
    }
    void Get(double a[2],double b[2],double c[2])
    {
        a[0]=m_x1,a[1]=m_y1;
        b[0]=m_x2,b[1]=m_y2;
        c[0]=m_x3,c[1]=m_y3;
    }
    void Get(Point2D &point1,Point2D &point2,Point2D &point3)
    {
        point1.m_x=m_x1,point1.m_y=m_y1;
        point2.m_x=m_x2,point2.m_y=m_y2;
        point3.m_x=m_x3,point3.m_y=m_y3;
    }
    void Translate(double x,double y)
    {
        m_x1+=x,m_y1+=y;
        m_x2+=x,m_y2+=y;
        m_x3+=x,m_y3+=y;
    }
    void Translate(double a[2])	                       //三角形平移，数组a[]表示平移量
    {
        m_x1+=a[0],m_y1+=a[1];
        m_x2+=a[0],m_y2+=a[1];
        m_x3+=a[0],m_y3+=a[1];
    }
};

class Triangle3D:public Triangle2D
{
public:
    Triangle3D(double x1=0,double y1=0,double z1=0,
               double x2=0,double y2=0,double z2=0,
               double x3=0,double y3=0,double z3=0)
    {
        m_x1=x1,m_y1=y1,m_z1=z1;
        m_x2=x2,m_y2=y2,m_z2=z2;
        m_x3=x3,m_y3=y3,m_z3=z3;
    }
    Triangle3D(double a[3],double b[3],double c[3])
    {
        m_x1=a[0],m_y1=a[1],m_z1=a[2];
        m_x2=b[0],m_y2=b[1],m_z2=b[2];
        m_x3=c[0],m_y3=c[1],m_z3=c[2];
    }
    Triangle3D(Point3D &point1,Point3D &point2,Point3D &point3)
    {
        m_x1=point1.m_x,m_y1=point1.m_y,m_z1=point1.m_z;
        m_x2=point2.m_x,m_y2=point2.m_y,m_z2=point2.m_z;
        m_x3=point3.m_x,m_y3=point3.m_y,m_z3=point3.m_z;
    }
    Triangle3D(const Triangle3D &triangle)
    {
        m_ID=triangle.m_ID;
        m_x1=triangle.m_x1,m_y1=triangle.m_y1,m_z1=triangle.m_z1;
        m_x2=triangle.m_x2,m_y2=triangle.m_y2,m_z2=triangle.m_z2;
        m_x3=triangle.m_x3,m_y3=triangle.m_y3,m_z3=triangle.m_z3;
    }
    //移动构造函数
    Triangle3D(Triangle3D &&triangle)
    {
        m_ID=triangle.m_ID;
        m_x1=triangle.m_x1,m_y1=triangle.m_y1,m_z1=triangle.m_z1;
        m_x2=triangle.m_x2,m_y2=triangle.m_y2,m_z2=triangle.m_z2;
        m_x3=triangle.m_x3,m_y3=triangle.m_y3,m_z3=triangle.m_z3;
    }
public:
    double m_z1;
    double m_z2;
    double m_z3;
public:
    Triangle3D operator=(Triangle2D &triangle)
    {
        m_ID=triangle.m_ID;
        m_x1=triangle.m_x1,m_y1=triangle.m_y1;
        m_x2=triangle.m_x2,m_y2=triangle.m_y2;
        m_x3=triangle.m_x3,m_y3=triangle.m_y3;
        return *this;
    }
    Triangle3D operator=(Triangle3D &triangle)
    {
        m_ID=triangle.m_ID;
        m_x1=triangle.m_x1,m_y1=triangle.m_y1,m_z1=triangle.m_z1;
        m_x2=triangle.m_x2,m_y2=triangle.m_y2,m_z2=triangle.m_z2;
        m_x3=triangle.m_x3,m_y3=triangle.m_y3,m_z3=triangle.m_z3;
        return *this;
    }
    int operator==(Triangle2D &triangle);
    int operator==(Triangle3D &triangle);
    int operator!=(Triangle2D &triangle);
    int operator!=(Triangle3D &triangle);
    int HasVertex(double x,double y,double z);         //判断坐标点是否为三角形的顶点，返回值为0，1，2，或3
    int HasVertex(double a[3]);                        //非零返回值表示坐标点为三角形的某个顶点
    int HasVertex(Point3D &point);	                   //返回值的大小表示三角形顶点的相应序号

    int HasEdge(double x1,double y1,double z1,         //判断线段是否为三角形的一条边，返回1，2，或3表示该线段所在边的序号,线段与边同向
                double x2,double y2,double z2);     //返回-1，-2，或-3表示所在边的序号的负值,线段与边反向
    int HasEdge(double a[3],double b[3]);              //返回0表示线段不是三角形的一条边
    int HasEdge(Point3D &point1,Point3D &point2);
    int HasEdge(Line3D &line);

    int Contain(double x,double y,double z);           //判断坐标点是否在三角形上，返回1，2，或3表示该点所在三角形顶点的序号
    int Contain(double a[3]);                          //返回4，5，或6表示该点所在三角形边的序号
    int Contain(Point3D &point);                       //返回7表示该点在三角形内部，返回0表示不在三角形上
    int GetNormal(double a[3]);                        //取三角形的法向量，结果在数组a[]中

    //int GetInnerCircle(double &x, double &y,double &r);
    //int GetOuterCircle(double &x, double &y, double &r);

    void Set(double a[3],double b[3],double c[3])
    {
        m_x1=a[0],m_y1=a[1],m_z1=a[2];
        m_x2=b[0],m_y2=b[1],m_z2=b[2];
        m_x3=c[0],m_y3=c[1],m_z3=c[2];
    }
    void Set(Point3D &point1,Point3D &point2,Point3D &point3)
    {
        m_x1=point1.m_x,m_y1=point1.m_y,m_z1=point1.m_z;
        m_x2=point2.m_x,m_y2=point2.m_y,m_z2=point2.m_z;
        m_x3=point3.m_x,m_y3=point3.m_y,m_z3=point3.m_z;
    }
    void Get(double a[3],double b[3],double c[3])
    {
        a[0]=m_x1,a[1]=m_y1,a[2]=m_z1;
        b[0]=m_x2,b[1]=m_y2,b[2]=m_z2;
        c[0]=m_x3,c[1]=m_y3,c[2]=m_z3;
    }
    void Get(Point3D &point1,Point3D &point2,Point3D &point3)
    {
        point1.m_x=m_x1,point1.m_y=m_y1,point1.m_z=m_z1;
        point2.m_x=m_x2,point2.m_y=m_y2,point2.m_z=m_z2;
        point3.m_x=m_x3,point3.m_y=m_y3,point3.m_z=m_z3;
    }
    void Translate(double x,double y,double z)
    {
        m_x1+=x,m_y1+=y,m_z1+=z;
        m_x2+=x,m_y2+=y,m_z2+=z;
        m_x3+=x,m_y3+=y,m_z3+=z;
    }
    void Translate(double a[3])	                       //三角形平移，数组a[]表示平移量
    {
        m_x1+=a[0],m_y1+=a[1],m_z1+=a[2];
        m_x2+=a[0],m_y2+=a[1],m_z2+=a[2];
        m_x3+=a[0],m_y3+=a[1],m_z3+=a[2];
    }
};



class mcTriangle
{
public:
    mcTriangle(int i=-1,int j=-1,int k=-1,byte f=0)
    {
        m_pointIndex1=i;
        m_pointIndex2=j;
        m_pointIndex3=k;
        m_flag=f;
    }
    mcTriangle(mcTriangle &triangle)
    {
        m_ID=triangle.m_ID;
        m_pointIndex1=triangle.m_pointIndex1 ;
        m_pointIndex2=triangle.m_pointIndex2 ;
        m_pointIndex3=triangle.m_pointIndex3 ;
        m_flag=triangle.m_flag ;
        m_normal=triangle.m_normal ;
    }
    mcTriangle(mcTriangle &&triangle)
    {
        m_ID=triangle.m_ID;
        m_pointIndex1=triangle.m_pointIndex1 ;
        m_pointIndex2=triangle.m_pointIndex2 ;
        m_pointIndex3=triangle.m_pointIndex3 ;
        m_flag=triangle.m_flag ;
        m_normal=triangle.m_normal ;
    }
public:
    int  m_ID;
    int  m_pointIndex1;
    int  m_pointIndex2;
    int  m_pointIndex3;
    int  m_normal;        //三角形法向量在法向量数组中的序号
    byte m_flag;          //标志
public:
    mcTriangle operator=(mcTriangle &triangle)
    {
        m_ID=triangle.m_ID;
        m_pointIndex1=triangle.m_pointIndex1 ;
        m_pointIndex2=triangle.m_pointIndex2 ;
        m_pointIndex3=triangle.m_pointIndex3 ;
        m_flag=triangle.m_flag ;
        m_normal=triangle.m_normal ;
        return *this;
    }
};
class TriangleFacet:public mcTriangle
{
public:
    TriangleFacet(int i=-1,int j=-1,int k=-1,
                  int ei=-1,int ej=-1,int ek=-1,byte f=0):mcTriangle(i,j,k,f)
    {
        m_edgeIndex1=ei;
        m_edgeIndex2=ej;
        m_edgeIndex3=ek;
    }
    TriangleFacet(TriangleFacet &t)
    {
        m_ID=t.m_ID;
        m_pointIndex1=t.m_pointIndex1 ;
        m_pointIndex2=t.m_pointIndex2 ;
        m_pointIndex3=t.m_pointIndex3 ;
        m_edgeIndex1=t.m_edgeIndex1 ;
        m_edgeIndex2=t.m_edgeIndex2 ;
        m_edgeIndex2=t.m_edgeIndex2 ;
        m_normal=t.m_normal ;
        m_flag=t.m_flag ;
    }
public:
    int  m_ID;
    int  m_edgeIndex1;
    int  m_edgeIndex2;
    int  m_edgeIndex3;
public:

    TriangleFacet operator=(TriangleFacet &t)
    {
        m_ID=t.m_ID;
        m_pointIndex1=t.m_pointIndex1 ;
        m_pointIndex2=t.m_pointIndex2 ;
        m_pointIndex3=t.m_pointIndex3 ;
        m_edgeIndex1=t.m_edgeIndex1 ;
        m_edgeIndex2=t.m_edgeIndex2 ;
        m_edgeIndex2=t.m_edgeIndex2 ;
        m_normal=t.m_normal ;
        m_flag=t.m_flag ;
        return *this;
    }
};
class Quad3D
{
public:
    Quad3D(){}
    Quad3D(Quad3D &quad)
    {
        for(int i=0;i<3;i++)
        {
            m_a[i]=quad.m_a[i];
            m_b[i]=quad.m_b[i];
            m_c[i]=quad.m_c[i];
            m_d[i]=quad.m_d[i];
            m_value=quad.m_value ;
            m_ID=quad.m_ID ;
        }
    }
    ~Quad3D(){}
public:
    double m_a[3];
    double m_b[3];
    double m_c[3];
    double m_d[3];
    double m_value;
    int    m_ID;
public:
    Quad3D operator=(Quad3D &quad)
    {
        for(int i=0;i<3;i++)
        {
            m_a[i]=quad.m_a[i];
            m_b[i]=quad.m_b[i];
            m_c[i]=quad.m_c[i];
            m_d[i]=quad.m_d[i];
            m_value=quad.m_value ;
            m_ID=quad.m_ID ;
        }
        return *this;
    }

    //lxy v.push_back
    void Set(Point3D &pointa,Point3D &pointb,Point3D &pointc,Point3D &pointd)
    {
        m_a[0] = pointa.m_x;
        m_a[1] = pointa.m_y;
        m_a[2] = pointa.m_z;

        m_b[0] = pointb.m_x;
        m_b[1] = pointb.m_y;
        m_b[2] = pointb.m_z;

        m_c[0] = pointc.m_x;
        m_c[1] = pointc.m_y;
        m_c[2] = pointc.m_z;

        m_d[0] = pointd.m_x;
        m_d[1] = pointd.m_y;
        m_d[2] = pointd.m_z;

    }

    void Get(Point3D &pointa,Point3D &pointb,Point3D &pointc,Point3D &pointd)
    {
        pointa.m_x = m_a[0];
        pointa.m_y = m_a[1];
        pointa.m_z = m_a[2];

        pointb.m_x = m_b[0];
        pointb.m_y = m_b[1];
        pointb.m_z = m_b[2];

        pointc.m_x = m_c[0];
        pointc.m_y = m_c[1];
        pointc.m_z = m_c[2];

        pointd.m_x = m_d[0];
        pointd.m_y = m_d[1];
        pointd.m_z = m_d[2];
    }
};

class Quad
{
public:
    Quad(int i=-1,int j=-1,int k=-1,int l=-1)
    {
        m_index1=i;
        m_index2=j;
        m_index3=k;
        m_index4=l;
    }
    Quad(Quad &quad)
    {
        m_ID=quad.m_ID;
        m_index1=quad.m_index1 ;
        m_index2=quad.m_index2 ;
        m_index3=quad.m_index3 ;
        m_index4=quad.m_index4 ;
    }
public:
    int m_ID;
    int m_index1;
    int m_index2;
    int m_index3;
    int m_index4;
public:
    Quad operator=(Quad &quad)
    {
        m_ID=quad.m_ID;
        m_index1=quad.m_index1 ;
        m_index2=quad.m_index2 ;
        m_index3=quad.m_index3 ;
        m_index4=quad.m_index4 ;
        return *this;
    }
};
//class Cell3D
//{
//public:
//    int    m_id;
//    double m_value;
//    double m_minX,m_minY,m_minZ;
//    double m_dX,m_dY,m_dZ;
//public:
//    Cell3D(double minX=0,double minY=0,double minZ=0,double dX=0,double dY=0,double dZ=0)
//    {
//        m_minX=minX,m_minY=minY,m_minZ=minZ;
//        m_dX=dX,m_dY=dY,m_dZ=dZ;
//    }
//    Cell3D(double a[3],double delta[3])
//    {
//        m_minX=a[0],m_minY=a[1],m_minZ=a[2];
//        m_dX=delta[0],m_dY=delta[1],m_dZ=delta[2];
//    }
//    Cell3D(Cell3D &cell)
//    {
//        m_minX=cell.m_minX,m_minY=cell.m_minY,m_minZ=cell.m_minZ;
//        m_dX=cell.m_dX,m_dY=cell.m_dY,m_dZ=cell.m_dZ;
//        m_id=cell. m_id ;
//        m_value=cell. m_value ;
//    }
//public:
//    Cell3D operator=(Cell3D &cell)
//    {
//        m_minX=cell.m_minX,m_minY=cell.m_minY,m_minZ=cell.m_minZ;
//        m_dX=cell.m_dX,m_dY=cell.m_dY,m_dZ=cell.m_dZ;
//        m_id=cell. m_id ;
//        m_value=cell. m_value ;
//        return *this;
//    }
//    int operator==(Cell3D &cell)
//    {
//        double temp=0;
//        temp+=(m_minX-cell.m_minX)*(m_minX-cell.m_minX);
//        temp+=(m_minY-cell.m_minY)*(m_minY-cell.m_minY);
//        temp+=(m_minZ=cell.m_minZ)*(m_minZ=cell.m_minZ);
//        temp+=(m_dX-cell.m_dX)*(m_dX-cell.m_dX);
//        temp+=(m_dY-cell.m_dY)*(m_dY-cell.m_dY);
//        temp+=(m_dZ=cell.m_dZ)*(m_dZ=cell.m_dZ);
//        if(temp<THRESHOLD10)
//            return 1;
//        return 0;
//    }
//    int operator!=(Cell3D &cell)
//    {
//        double temp=0;
//        temp+=(m_minX-cell.m_minX)*(m_minX-cell.m_minX);
//        temp+=(m_minY-cell.m_minY)*(m_minY-cell.m_minY);
//        temp+=(m_minZ=cell.m_minZ)*(m_minZ=cell.m_minZ);
//        temp+=(m_dX-cell.m_dX)*(m_dX-cell.m_dX);
//        temp+=(m_dY-cell.m_dY)*(m_dY-cell.m_dY);
//        temp+=(m_dZ=cell.m_dZ)*(m_dZ=cell.m_dZ);
//        if(temp<THRESHOLD10)
//            return 0;
//        return 1;
//    }
//    void Set(double minX,double minY,double minZ,double dX,double dY,double dZ)
//    {
//        m_minX=minX,m_minY=minY,m_minZ=minZ;
//        m_dX=dX,m_dY=dY,m_dZ=dZ;
//    }
//    void Set(double a[3],double delta[3])
//    {
//        m_minX=a[0],m_minY=a[1],m_minZ=a[2];
//        m_dX=delta[0],m_dY=delta[1],m_dZ=delta[2];
//    }
//    void Get(double &minX,double &minY,double &minZ,double &dX,double &dY,double &dZ)
//    {
//        minX=m_minX,minY=m_minY,minZ=m_minZ;
//        dX=m_dX,dY=m_dY,dZ=m_dZ;
//    }
//    void Get(double a[3],double delta[3])
//    {
//        a[0]=m_minX,a[1]=m_minY,a[2]=m_minZ;
//        delta[0]=m_dX,delta[1]=m_dY,delta[2]=m_dZ;
//    }
//};
//class Circle2D     //二维圆周类
//{
//public:
//    Circle2D(double x=0,double y=0,double radius=0);
//    Circle2D(double a[2],double b[2],double c[2]);
//    Circle2D(Circle2D &circle);
//public:
//    double m_x,m_y;
//    double m_radius;
//public:
//    Circle2D operator=(Circle2D &circle);
//    int operator==(Circle2D &circle);
//    int operator!=(Circle2D &circle);
//
//    int Contain(double x,double y);                //返回0表示在圆周外
//    int Contain(double a[2]);                      //返回1表示在圆周上
//    int Contain(Point2D &point);                   //返回2表示在圆周内部
//
//    void Set(double x,double y,double radius);
//    void Set(double a[2],double b[2],double c[2]);
//    void Set(Line2D &line,Point2D &point);
//    void Set(Point2D &point1,Point2D &point2,Point2D &point3);
//    void Get(double &x,double &y,double &radius);
//    void Get(double a[2],double &radius);
//public:
//    ~Circle2D(void)
//    {
//    }
//};
//class Circle3D     //三维圆周类
//{
//public:
//    Circle3D(double x=0,double y=0,double z=0,double radius=0);
//    Circle3D(double a[3],double b[3],double c[3]);
//    Circle3D(Circle3D &circle);
//public:
//    double m_n[3];
//    double m_x,m_y,m_z;
//    double m_radius;
//public:
//    Circle3D operator=(Circle3D &circle);
//    int operator==(Circle3D &circle);
//    int operator!=(Circle3D &circle);
//
//    int Contain(double x,double y,double z);                //返回0表示在圆周外
//    int Contain(double a[3]);                               //返回1表示在圆周上
//    int Contain(Point3D &point);                            //返回2表示在圆周内部
//
//    void Set(double x,double y,double z,double radius);
//    void Set(double a[3],double b[3],double c[3]);
//    void Set(Line3D &line,Point3D &point);
//    void Set(Point3D &point1,Point3D &point2,Point3D &point3);
//    void Get(double &x,double &y,double &z,double &radius);
//    void Get(double a[3],double &radius);
//public:
//    ~Circle3D(void)
//    {
//    }
//};

class Tetrahedron3D     //四面体类
{
public:
    Tetrahedron3D(double x1=0,double y1=0,double z1=0,
                  double x2=0,double y2=0,double z2=0,
                  double x3=0,double y3=0,double z3=0,
                  double x4=0,double y4=0,double z4=0);
    Tetrahedron3D(double a[3],double b[3],double c[3],double d[3]);
    Tetrahedron3D(Point3D &point1,Point3D &point2,
                  Point3D &point3,Point3D &point4);
    Tetrahedron3D(Tetrahedron3D &tetrahedron);
public:
    int    m_ID;
    double m_x1,m_y1,m_z1;
    double m_x2,m_y2,m_z2;
    double m_x3,m_y3,m_z3;
    double m_x4,m_y4,m_z4;
public:
    Tetrahedron3D operator=(Tetrahedron3D &tetrahedron);
    int operator==(Tetrahedron3D &tetrahedron);
    int operator!=(Tetrahedron3D &tetrahedron);

    int HasVertex(double x,double y,double z);              //判断坐标点是否为四面体的顶点
    int HasVertex(double a[3]);                             //返回0表示坐标点不是四面体的顶点
    int HasVertex(Point3D &point);                          //返回1，2，3，或4表示所在顶点的序号

    int HasArris(double x1,double y1,double z1,             //判断线段是否为四面体的棱，返回1，2，或3表示所在底边的序号，同向
                 double x2,double y2,double z2);            //返回-1，-2，或-3表示所在底边的序号，反向
    int HasArris(double a[3],double b[3]);                  //返回4，5，或6表示所在侧棱的序号，自底向顶
    int HasArris(Point3D &point1,Point3D &point2);          //返回-4，-5，或-6表示所在侧棱的序号，自顶向底

    int HasFacet(double x1,double y1,double z1,             //判断三角形是否为四面体返回的一个侧面
                 double x2,double y2,double z2,             //返回0表示三角形与四面体的任何一个侧面都不相同
                 double x3,double y3,double z3);            //返回1表示三角形与四面体的某个侧面相同
    int HasFacet(double a[3],double b[3],double c[3]);
    int HasFacet(Point3D &point1,Point3D &point2,Point3D &point3);

    int Contain(double x,double y,double z);                 //未实现，返回0
    int Contain(double a[3]);                                //未实现，返回0
    int Contain(Point3D &point);                             //未实现，返回0

    void Set(double x1,double y1,double z1,
             double x2,double y2,double z2,
             double x3,double y3,double z3,
             double x4,double y4,double z4);
    void Set(double a[3],double b[3],double c[3],double d[3]);
    void Set(Triangle3D &triangle,Point3D &point);
    void Set(Point3D &point1,Point3D &point2,Point3D &point3,Point3D &point4);
    void Get(double &x1,double &y1,double &z1,
             double &x2,double &y2,double &z2,
             double &x3,double &y3,double &z3,
             double &x4,double &y4,double &z4);
    void Get(double a[3],double b[3],double c[3],double d[3]);
    void Get(Point3D &point1,Point3D &point2,Point3D &point3,Point3D &point4);
    void Translate(double a[3]);                 //四面体平移，数组a[]表示平移量
};


class Relation2D
{
public:
    Relation2D(){}
public:
    //-----------------------------------------------------------------------------
    //函数名称：PointInLine
    //函数功能：判断点是否在线段上
    //输入参数：线段端点坐标a[]和b[],点的坐标p[]。
    //输出参数：无
    //返回结果：点不在线段上返回0，在线段的起始点返回1，在线段的终止点返回2，
    //          在线段的两端点之间返回3。
    static int  PointInLine(double a[2],double b[2],double p[2]);

    //-----------------------------------------------------------------------------
    //函数名称：PointInLine
    //函数功能：判断点是否在线段上
    //输入参数：线段line,点的坐标p[]。
    //输出参数：无
    //返回结果：点不在线段上返回0，在线段的起始点返回1，在线段的终止点返回2，
    //          在线段的两端点之间返回3。
    static int  PointInLine(Line2D &line,double p[2]);

    //-----------------------------------------------------------------------------
    //函数名称：PointInTriangle
    //函数功能：判断点是否在三角形上
    //输入参数：三角形的顶点坐标a[]、b[]和c[],点的坐标p[]。
    //输出参数：无
    //返回结果：点不在三角形上返回0，在三角形顶点则返回该顶点的序号（1、2或3），
    //          在三角形的边上则按该边序号（1、2或3）返回4、5或6，在三角形的内部则返回7。
    static int  PointInTriangle(double a[2],double b[2],double c[2],double p[2]);

    //-----------------------------------------------------------------------------
    //函数名称：PointInTriangle
    //函数功能：判断点是否在三角形上
    //输入参数：三角形triangle,点的坐标p[]。
    //输出参数：无
    //返回结果：点不在三角形上返回0，在三角形顶点则返回该顶点的序号（1、2或3），
    //          在三角形的边上则按该边序号（1、2或3）返回4、5或6，在三角形的内部则返回7。
    static int  PointInTriangle(Triangle2D &triangle,double p[2]);

    //-----------------------------------------------------------------------------
    //函数名称：GetIntersectPoint
    //函数功能：判断线与线是否相交
    //输入参数：线段端点坐标a[]和b[]，线段端点坐标p1[]和p2[]。
    //输出参数：交点的坐标p[]。
    //返回结果：返回值为0时表示不相交；
    //          返回值/10表示交点在前一条线段上的位置，1表示在起始点，2表示在终止点，3表示在端点之间；
    //          返回值%10表示交点在后一条线段上的位置，1表示在起始点，2表示在终止点，3表示在端点之间。
    static int  GetIntersectPoint(double a[2],double b[2],
                                  double p1[2],double p2[2],double p[2]);

    //-----------------------------------------------------------------------------
    //函数名称：GetIntersectPoint
    //函数功能：判断线与线是否相交
    //输入参数：线段line1和line2。
    //输出参数：交点的坐标p[]。
    //返回结果：返回值为0时表示不相交；
    //          返回值/10表示交点在前一条线段上的位置，1表示在起始点，2表示在终止点，3表示在端点之间；
    //          返回值%10表示交点在后一条线段上的位置，1表示在起始点，2表示在终止点，3表示在端点之间。
    static int  GetIntersectPoint(Line2D &line1,Line2D &line2,double p[2]);

    //-----------------------------------------------------------------------------
    //函数名称：PToPDistance
    //函数功能：求点到点的距离
    //输入参数：两点的坐标a[]和b[]。
    //输出参数：无
    //返回结果：点到点的距离
    static double PToPDistance(double a[2],double b[2]);

    //-----------------------------------------------------------------------------
    //函数名称：PToPDistance
    //函数功能：求点到点的距离
    //输入参数：空间两点point1和point2。
    //输出参数：无
    //返回结果：点到点的距离
    static double PToPDistance(Point2D &point1,Point2D &point2);

    //-----------------------------------------------------------------------------
    //函数名称：PToLDistance
    //函数功能：求点到直线的距离
    //输入参数：直线上的两点a[]和b[]，点p[]。
    //输出参数：无
    //返回结果：点到直线的距离
    static double PToLDistance(double a[2],double b[2],double p[2]);

    //-----------------------------------------------------------------------------
    //函数名称：PToLDistance
    //函数功能：求点到直线的距离
    //输入参数：直线上的一线段line，点point。
    //输出参数：无
    //返回结果：点到直线的距离
    static double PToLDistance(Line2D &line,Point2D &point);

    //-----------------------------------------------------------------------------
    //函数名称：PToLDistance
    //函数功能：求点到直线的距离
    //输入参数：直线上的两点point1和point2，点point。
    //输出参数：无
    //返回结果：点到直线的距离
    static double PToLDistance(Point2D &point1,Point2D &point2,Point2D &point);

    //-----------------------------------------------------------------------------
    //函数名称：PToSegDistance
    //函数功能：求点到直线段的距离
    //输入参数：直线段两端点a[]和b[]，点p[]。
    //输出参数：无
    //返回结果：点到直线段的距离

    static double PToSegDistance(double a[2],double b[2],double p[2]);          //点到线的距离
    //-----------------------------------------------------------------------------
    //函数名称：PToSegDistance
    //函数功能：求点到直线段的距离
    //输入参数：直线段line，点point。
    //输出参数：无
    //返回结果：点到直线段的距离

    static double PToSegDistance(Line2D &line,Point2D &point);
    //-----------------------------------------------------------------------------
    //函数名称：PToSegDistance
    //函数功能：求点到直线段的距离
    //输入参数：直线段两端点point1和point2，点point。
    //输出参数：无
    //返回结果：点到直线段的距离

    static double PToSegDistance(Point2D &point1,Point2D &point2,Point2D &point);
    //-----------------------------------------------------------------------------
    //函数名称：PToFDistance
    //函数功能：求点到平面的距离
    //输入参数：平面上三角形的顶点a[]、b[]和c[]，点p[]。
    //输出参数：无
    //返回结果：非负值表示点到三角形所在平面的距离，负值表示无效。

public:
    ~Relation2D()
    {
    }
};
class Triangle3DArrayEx;
class Relation3D
{
public:
    Relation3D(){}
public:
    //-----------------------------------------------------------------------------
    //函数名称：PointInLine
    //函数功能：判断点是否在线段上
    //输入参数：线段端点坐标a[]和b[],点的坐标p[]。
    //输出参数：无
    //返回结果：点不在线段上返回0，在线段的起始点返回1，在线段的终止点返回2，
    //          在线段的两端点之间返回3。
    static int  PointInLine(double a[3],double b[3],double p[3]);

    //-----------------------------------------------------------------------------
    //函数名称：PointInLine
    //函数功能：判断点是否在线段上
    //输入参数：线段line,点的坐标p[]。
    //输出参数：无
    //返回结果：点不在线段上返回0，在线段的起始点返回1，在线段的终止点返回2，
    //          在线段的两端点之间返回3。
    static int  PointInLine(Line3D &line,double p[3]);

    //-----------------------------------------------------------------------------
    //函数名称：PointInTriangle
    //函数功能：判断点是否在三角形上
    //输入参数：三角形的顶点坐标a[]、b[]和c[],点的坐标p[]。
    //输出参数：无
    //返回结果：点不在三角形上返回0，在三角形顶点则返回该顶点的序号（1、2或3），
    //          在三角形的边上则按该边序号（1、2或3）返回4、5或6，在三角形的内部则返回7。
    static int  PointInTriangle(double a[3],double b[3],double c[3],double p[3]);

    //-----------------------------------------------------------------------------
    //函数名称：PointInTriangle
    //函数功能：判断点是否在三角形上
    //输入参数：三角形triangle,点的坐标p[]。
    //输出参数：无
    //返回结果：点不在三角形上返回0，在三角形顶点则返回该顶点的序号（1、2或3），
    //          在三角形的边上则按该边序号（1、2或3）返回4、5或6，在三角形的内部则返回7。
    static int  PointInTriangle(Triangle3D &triangle,double p[3]);

    //-----------------------------------------------------------------------------
    //函数名称：GetIntersectPoint
    //函数功能：判断线与线是否相交
    //输入参数：线段端点坐标a[]和b[]，线段端点坐标p1[]和p2[]。
    //输出参数：交点的坐标p[]。
    //返回结果：返回值为0时表示不相交；
    //          返回值/10表示交点在前一条线段上的位置，1表示在起始点，2表示在终止点，3表示在端点之间；
    //          返回值%10表示交点在后一条线段上的位置，1表示在起始点，2表示在终止点，3表示在端点之间。
    static int  GetIntersectPoint(double a[3],double b[3],
                                  double p1[3],double p2[3],double p[3]);

    //-----------------------------------------------------------------------------
    //函数名称：GetIntersectPoint
    //函数功能：判断线与线是否相交
    //输入参数：线段line1和line2。
    //输出参数：交点的坐标p[]。
    //返回结果：返回值为0时表示不相交；
    //          返回值/10表示交点在前一条线段上的位置，1表示在起始点，2表示在终止点，3表示在端点之间；
    //          返回值%10表示交点在后一条线段上的位置，1表示在起始点，2表示在终止点，3表示在端点之间。
    static int  GetIntersectPoint(Line3D &line1,Line3D &line2,double p[3]);

    //-----------------------------------------------------------------------------
    //函数名称：GetIntersectPoint
    //函数功能：判断三角形与线段是否相交
    //输入参数：三角形的顶点a[]、b[]和c[]，线段的端点p1[]和p2[]。
    //输出参数：交点的坐标p[]。
    //返回结果：返回值为0时表示不相交；
    //          返回值/10表示交点在三角形上的位置，交点在三角形顶点则返回该顶点的序号（1、2或3），
    //          在三角形的边上则按该边序号（1、2或3）返回4、5或6，在三角形的内部则返回7；
    //          返回值%10表示交点在线段上的位置，  1表示在起始点，2表示在终止点，3表示在端点之间。
    static int  GetIntersectPoint(double a[3],double b[3],double c[3],
                                  double p1[3],double p2[3],double p[3]);

    //-----------------------------------------------------------------------------
    //函数名称：GetIntersectPoint
    //函数功能：判断三角形与线段是否相交
    //输入参数：三角形triangle，线段line。
    //输出参数：交点的坐标p[]。
    //返回结果：返回值为0时表示不相交；
    //          返回值/10表示交点在三角形上的位置，交点在三角形顶点则返回该顶点的序号（1、2或3），
    //          在三角形的边上则按该边序号（1、2或3）返回4、5或6，在三角形的内部则返回7；
    //          返回值%10表示交点在线段上的位置，  1表示在起始点，2表示在终止点，3表示在端点之间。
    static int  GetIntersectPoint(Triangle3D &triangle, Line3D &line,double p[3]);//线与面相交

    //-----------------------------------------------------------------------------
    //函数名称：PToPDistance
    //函数功能：求点到点的距离
    //输入参数：两点的坐标a[]和b[]。
    //输出参数：无
    //返回结果：点到点的距离
    static double PToPDistance(double a[3],double b[3]);

    //-----------------------------------------------------------------------------
    //函数名称：PToPDistance
    //函数功能：求点到点的距离
    //输入参数：空间两点point1和point2。
    //输出参数：无
    //返回结果：点到点的距离
    static double PToPDistance(Point3D &point1,Point3D &point2);

    //-----------------------------------------------------------------------------
    //函数名称：PToLDistance
    //函数功能：求点到直线的距离
    //输入参数：直线上的两点a[]和b[]，点p[]。
    //输出参数：无
    //返回结果：点到直线的距离
    static double PToLDistance(double a[3],double b[3],double p[3]);

    //-----------------------------------------------------------------------------
    //函数名称：PToLDistance
    //函数功能：求点到直线的距离
    //输入参数：直线上的一线段line，点point。
    //输出参数：无
    //返回结果：点到直线的距离
    static double PToLDistance(Line3D &line,Point3D &point);

    //-----------------------------------------------------------------------------
    //函数名称：PToLDistance
    //函数功能：求点到直线的距离
    //输入参数：直线上的两点point1和point2，点point。
    //输出参数：无
    //返回结果：点到直线的距离
    static double PToLDistance(Point3D &point1,Point3D &point2,Point3D &point);

    //-----------------------------------------------------------------------------
    //函数名称：PToSegDistance
    //函数功能：求点到直线段的距离
    //输入参数：直线段两端点a[]和b[]，点p[]。
    //输出参数：无
    //返回结果：点到直线段的距离

    static double PToSegDistance(double a[3],double b[3],double p[3]);          //点到线的距离
    //-----------------------------------------------------------------------------
    //函数名称：PToSegDistance
    //函数功能：求点到直线段的距离
    //输入参数：直线段line，点point。
    //输出参数：无
    //返回结果：点到直线段的距离

    static double PToSegDistance(Line3D &line,Point3D &point);
    //-----------------------------------------------------------------------------
    //函数名称：PToSegDistance
    //函数功能：求点到直线段的距离
    //输入参数：直线段两端点point1和point2，点point。
    //输出参数：无
    //返回结果：点到直线段的距离

    static double PToSegDistance(Point3D &point1,Point3D &point2,Point3D &point);
    //-----------------------------------------------------------------------------
    //函数名称：PToFDistance
    //函数功能：求点到平面的距离
    //输入参数：平面上三角形的顶点a[]、b[]和c[]，点p[]。
    //输出参数：无
    //返回结果：非负值表示点到三角形所在平面的距离，负值表示无效。

    static double PToFDistance(double a[3],double b[3],double c[3],double p[3]);
    //-----------------------------------------------------------------------------
    //函数名称：PToFDistance
    //函数功能：求点到平面的距离
    //输入参数：平面上三角形triangle，点point。
    //输出参数：无
    //返回结果：非负值表示点到三角形所在平面的距离，负值表示无效。

    static double PToFDistance(Triangle3D &triangle,Point3D &point);
    //-----------------------------------------------------------------------------
    //函数名称：PToFDistance
    //函数功能：求点到平面的距离
    //输入参数：平面上三角形的顶点point1、point2和point3，点point。
    //输出参数：无
    //返回结果：非负值表示点到三角形所在平面的距离，负值表示无效。
    static double PToFDistance(Point3D &point1,Point3D &point2,
                               Point3D &point3,Point3D &point);

    //-----------------------------------------------------------------------------
    //函数名称：GetCrossCut
    //函数功能：已知三角形与平面相交，且三角形的顶点不在平面上，求三角形与平面的交线。
    //输入参数：平面上三角形的顶点、平面上一点和平面法向量。
    //输出参数：无
    //返回结果：交线。
    static int GetCrossCut(Line3D& line,Triangle3D triangle,double p[3],double n[3]);
    static int GetCrossCut(Line3D& line, double a[3],double b[3],double c[3],double p[3],double n[3]);
    static int GetCrossCut(Line3D& line,Triangle3D triangle1,Triangle3D triangle2);

    ////////////////////////////////////////////////////----------
    //函数名称：
    //函数功能：
    //输入参数：
    //输出参数：
    //返回结果：
    int DivideTriangles(Triangle3DArrayEx& polyTriangle1,Triangle3DArrayEx& polyTriangle2);
    int DivideTriangles(Triangle3DArrayEx& polyTriangle1,Triangle3DArrayEx& polyTriangle2,
                        Triangle3D triangle1,Triangle3D triangle2);
public:
    ~Relation3D()
    {
    }
};
typedef list<byte> ByteList;
typedef list<int> IntList;
//typedef list<long> LongList;
//typedef list<float> FloatList;
//typedef list<double> DoubleList;
//typedef list<Point2D> Point2DList;
//typedef list<Point3D> Point3DList;
//typedef list<Point4D> Point4DList;
//typedef list<Line2D> Line2DList;
//typedef list<Line3D> Line3DList;
//typedef list<Line4D> Line4DList;
//typedef list<Line> LineList;
//typedef list<Edge> EdgeList;
//typedef list<Triangle2D> Triangle2DList;
//typedef list<Triangle3D> Triangle3DList;
////typedef list<Triangle4D> Triangle4DList;
//typedef list<Triangle> TriangleList;
//typedef list<TriangleFacet> TriangleFacetList;
//typedef list<Tetrahedron3D> Tetrahedron3DList;
//typedef list<Quad3D> Quad3DList;
//typedef list<Quad> QuadList;
////typedef list<Cell3D> Cell3DList;
//

typedef vector<int> IntArray;

//typedef vector<Point2D> Point2DArray;
//typedef vector<Point3D> Point3DArray;
//
//typedef vector<Vector3D> NormalArray;
//typedef vector<Line2D> Line2DArray;
//typedef vector<Line3D> Line3DArray;
//
//typedef vector<Line> LineArray;
//typedef vector<Edge> EdgeArray;
//typedef vector<Triangle2D> Triangle2DArray;
//typedef vector<Triangle3D> Triangle3DArray;
////typedef vector<Triangle4D,Triangle4D> Triangle4DArray;
//typedef vector<Triangle,Triangle> TriangleArray;
//typedef vector<TriangleFacet,TriangleFacet> TriangleFacetArray;
//typedef vector<Tetrahedron3D,Tetrahedron3D> Tetrahedron3DArray;
//typedef vector<Quad3D,Quad3D> Quad3DArray;
//typedef vector<Quad,Quad> QuadArray;
////typedef vector<Cell3D,Cell3D> Cell3DArray;


class Point2DArrayEx
{
public:
    vector<Point2D> v;
    int size(){
        return v.size();
    }
    Point2D& at(int i){
        return v[i];
    }
    void clear(){
        v.clear();
    }
    Point2DArrayEx()
    {
        m_strID="";
        m_strSubID="";
        m_attr1=m_attr2=m_ID=-1;
        m_minX=m_maxX=m_minY=m_maxY=0.0;
    }
//    Point2DArrayEx(Point2DArray &pointArray);
    Point2DArrayEx(Point2DArrayEx &pointArray);
public:
    int      m_attr1;
    int      m_attr2;
    int      m_ID;
    string  m_strID;
    string  m_strSubID;
    double   m_minX,m_maxX,m_minY,m_maxY;
public:
//    Point2DArrayEx operator =(Point2DArray &pointArray);
    Point2DArrayEx operator =(Point2DArrayEx &pointArray);
//    Point2DArrayEx operator+=(Point2DArray &pointArray);  //后接一数组到当前数组
    Point2DArrayEx operator+=(Point2DArrayEx &pointArray);//后接一数组到当前数组
    Point2DArrayEx operator|=(Point2DArrayEx &pointArray);//后接一数组到当前数组，去掉重复元素
    double GetDiameter(void);                             //取点集的直径
    Point2DArrayEx ReverseOrder(void);                    //反向排列数组中的元素
    bool GetNormal(double n[1]);

    //---------------------------------------------------------------------------
    //函数名称：GetConvexHull
    //函数功能：求平面点集的凸壳和点集所在平面的一单位法向量，
    //          该凸壳上顶点的顺序与该法向量成右手螺旋关系,凸壳闭合点不重复存储。
    //输入参数：无
    //输出参数：用坐标点数组pointArray表示的凸壳,单位法向量n[]
    //返回结果：成功时返回1，否则返回0
    bool GetConvexHull(Point2DArrayEx &pointArray,double n[1]); //算法效率不高，需改进

    //---------------------------------------------------------------------------
    //函数名称：GetConvexHull
    //函数功能：求平面点集的凸壳,该凸壳的方向与给定的法向量成右手系,凸壳闭合点不重复存储。
    //输入参数：平面法向量n。　
    //输出参数：凸壳顶点序号数组indices。
    //返回结果：成功时返回1，否则返回0
    bool GetConvexHull(IntArray &hullIndices,double n[1]);      //高效算法,O(nlogn)

    //-----------------------------------------------------------------------------
    //函数名称：ExistSharedPoint
    //函数功能：判断两个坐标点数组有无公共元素。
    //输入参数：用pointArray表示的坐标点数组。
    //输出参数：无
    //返回结果：无公共点时返回0，否则设第一个公共点在当前数组中的序号为i,在当前数组中的序号为j,
    //          返回(i+1)*MaxSize+(j+1).
    int     ExistSharedPoint(Point2DArrayEx pointArray);

    int     HasElement(double x,double y);        //判断数组是否包含某个坐标点
    int     HasElement(double a[2]);
    int     HasElement(Point2D point);
    void    Add(double x,double y);
    void    Add(double a[2]);
    void    Add(Point2D &point);
    void    InsertAt(int index,Point2D& point);
private:
    bool GetLeftPoints(Line2D &line,IntArray& subsetIndices,IntArray& resultIndices);          //取有向线段左侧点，输出点的序号
    bool GetRightPoints(Line2D &line,IntArray& subsetIndices,IntArray& resultIndices);         //取有向线段右侧点，输出点的序号
    bool GetFarMost(Line2D &line,IntArray& subsetIndices,int& index);                          //取最远点，输出点的序号
    bool GetLeftMost(int& index);
    bool GetRightMost(int& index);
//    bool GetLeftHull(vector<int>::iterator start,vector<int>::iterator end,               //in 线段序号在vertexIndices中的位置
//                     IntArray &subsetIndices,                    //in 线段左侧点
//                     IntList& vertexIndices);                    //in,out凸壳顶点
    bool GetRightHull(vector<int>::iterator& start,vector<int>::iterator& end,IntArray &subsetIndices,IntArray& vertexIndices);

public:
    virtual ~Point2DArrayEx()
    {
    }
};

class Line3DArrayEx;
class Point3DArrayEx
{
public:
    vector<Point3D> v;
    int size(){
        return v.size();
    }
    Point3D& at(int i){
        return v[i];
    }


    void clear(){
        v.clear();
    }

    Point3DArrayEx()
    {
        m_minX=m_maxX=m_minY=m_maxY=m_minZ=m_maxZ=0.0;
        m_strID="";
        m_strSubID="";
        m_attr1=m_attr2=m_ID=-1;
    }
//    Point3DArrayEx(Point3DArrayEx &pointArray);
//    Point3DArrayEx(Point3DArrayEx &&pointArray);
public:
    int      m_attr1;
    int      m_attr2;
    int      m_ID;
    string  m_strID;
    string  m_strSubID;
    double   m_minX,m_maxX,m_minY,m_maxY,m_minZ,m_maxZ;
public:
//    Point3DArrayEx operator =(Point3DArray &pointArray);
//    Point3DArrayEx operator =(Point3DArrayEx &pointArray);
    Point3DArrayEx operator =(Point3DArrayEx pointArray);
//    Point3DArrayEx operator+=(Point3DArray &pointArray);  //后接一数组到当前数组
    Point3DArrayEx operator+=(Point3DArrayEx &pointArray);//后接一数组到当前数组
    Point3DArrayEx operator|=(Point3DArrayEx &pointArray);//后接一数组到当前数组，去掉重复元素
    int operator==(Point3DArrayEx &pointArray);           //同向，点相同返回1,异向，点相同，返回-1
    double GetDiameter(void);                             //取点集的直径
    Point3DArrayEx ReverseOrder(void);                    //反向排列数组中的元素

    //----------------------------------------------------------------------------------
    //函数名称：GetNormal
    //函数功能：求点集所在平面的单位法向量，该点集的凸壳上顶点顺序与该法向量成右手螺旋关系。
    //输入参数：无
    //输出参数：平面点集的法向量
    //返回结果：法向量存在时返回1，否则返回0
    bool GetNormal(double n[3]);

    //-----------------------------------------------------------------------------
    //函数名称：GetPseudoConvexHull
    //函数功能：求空间点集形的伪凸壳。伪凸壳在平面法向量n的垂直平面上的投影为已知空间点集在该平面上的投影的凸壳。
    //输入参数：平面法向量n[3]。
    //输出参数：伪凸壳顶点序号hullIndices。
    //返回结果：成功时返回true，否则返回false。
    bool GetPseudoConvexHull(IntArray &hullIndices,double n[3]);    //高效算法，O(nlogn)

    //---------------------------------------------------------------------------
    //函数名称：GetPanarConvexHull
    //函数功能：求平面点集的凸壳和点集所在平面的一单位法向量，
    //          该凸壳上顶点的顺序与该法向量成右手螺旋关系,凸壳闭合点不重复存储。
    //输入参数：无
    //输出参数：用坐标点数组pointArray表示的凸壳,单位法向量n[]
    //返回结果：成功时返回1，否则返回0
    bool GetPlanarConvexHull(Point3DArrayEx &pointArray,double n[3]);

    //----------------------------------------------------------------------------
    //函数名称：GetPlanarConvexHull
    //函数功能：求平面点集的凸壳和点集所在平面的一单位法向量，该凸壳上顶点的顺序与该法向量成右手螺旋关系。
    //输入参数：无
    //输出参数：用有向线段数组objLineArray表示的凸壳,单位法向量n[]
    //返回结果：成功时返回1，否则返回0。
    bool GetPlanarConvexHull(Line3DArrayEx  &lineArray, double n[]);

    //-----------------------------------------------------------------------------
    //函数名称：GetPseudoConvexHull
    //函数功能：求空间点集的伪凸壳，该伪凸壳为一空间多边形，该空间多边形的闭合点不重复存储。
    //          伪凸壳在向量n[]的垂直平面上的投影为已知空间点集在该平面上的投影的凸壳。
    //输入参数：某平面法向量n[]
    //输出参数：用有坐标数组pointArray表示的伪凸壳
    //返回结果：成功时返回1，否则返回0
    Point3DArrayEx GetPseudoConvexHull(double n[3]);

    ///----------------------------------------------------------------------------
    //函数名称：GetSpatialConvexHull
    //函数功能：求空间点集的凸壳
    //输入参数：无
    //输出参数：用三角形数组triangleList表示的凸壳,其中三角形正向与凸壳外侧法向量成右手系。
    //返回结果：成功时返回1，否则返回0。
//    int GetSpatialConvexHull(Triangle3DArray &triangleArray);       //算法需要改进

    //-----------------------------------------------------------------------------
    //函数名称：GetSharedPoint
    //函数功能：判断两个坐标点数组有无公共元素。
    //输入参数：用pointArray表示的坐标点数组。
    //输出参数：无
    //返回结果：无公共点时返回0，否则设第一个公共点在当前数组中的序号为i,在当前数组中的序号为j,
    //          返回(i+1)*MaxSize+(j+1).
    int     ExistSharedPoint(Point3DArrayEx pointArray);

    int     HasElement(double x,double y,double z);    //判断数组是否包含某个坐标点
    int     HasElement(double a[3]);
    int     HasElement(Point3D point);
    void    Add(double x,double y,double z);           //向数组中增加坐标点
    void    Add(double a[3]);                          //向数组中增加坐标点
    void    Add(Point3D &point);
    void    InsertAt(int index,Point3D& point);
public:
    virtual ~Point3DArrayEx()
    {
    }
};

class Line2DArrayEx
{
public:
    vector<Line2D> v;
    int size(){
        return v.size();
    }
    Line2D& at(int i){
        return v[i];
    }
    void clear(){
        v.clear();
    }
    Line2DArrayEx()
    {
        m_strID="";
        m_strSubID="";
        m_attr1=m_attr2=m_ID=-1;
    }
//    Line2DArrayEx(Line2DArray &lineArray);
    Line2DArrayEx(Line2DArrayEx &lineArray);
public:
    int      m_attr1;
    int      m_attr2;
    int      m_ID;
    string  m_strID;
    string  m_strSubID;
public:
//    Line2DArrayEx operator =(Line2DArray &lineArray);
    Line2DArrayEx operator =(Line2DArrayEx &lineArray);
    Line2DArrayEx operator =(Line2DArrayEx lineArray);
    Line2DArrayEx operator+=(Line2DArrayEx &lineArray); //在当前数组上后接一数组
    Line2DArrayEx operator-=(Line2DArrayEx &lineArray); //在当前数组与参数数组求差
    Line2DArrayEx operator|=(Line2DArrayEx &lineArray); //在当前数组上后接一数组，去掉重复元素

    void  Add(double x1,double y1 ,                     //向数组中增加一线段
              double x2,double y2 );
    void  Add(double a[],double b[]);                   //向数组中增加一线段
    void  Add(Line2D &line);
    void  SetUniDirection(void);                        //使线段方向保持一致
//    Arc2DArrayEx GetArcs();                             //从有向线段搜索弧段
public:
    virtual ~Line2DArrayEx()
    {
    }
};


class Line3DArrayEx
{
    vector<Line3D> v;
public:
    int size(){
        return v.size();
    }
    Line3D& at(int i){
        return v[i];
    }
    void clear(){
        v.clear();
    }

    void RemoveAt(int i){
        v.erase(v.begin() + i);
    }
    Line3DArrayEx()
    {
        m_strID="";
        m_strSubID="";
        m_attr1=m_attr2=m_ID=-1;
    }

//    Line3DArrayEx(Line3DArray &lineArray);
    Line3DArrayEx(Line3DArrayEx &lineArray);

public:
    int      m_attr1;
    int      m_attr2;
    int      m_ID;
    string  m_strID;
    string  m_strSubID;
public:
//    Line3DArrayEx operator =(Line3DArray &lineArray);
    Line3DArrayEx operator =(Line3DArrayEx lineArray);
    Line3DArrayEx operator =(Line3DArrayEx &lineArray);
    Line3DArrayEx operator+=(Line3DArrayEx &lineArray); //在当前数组上后接一数组
    Line3DArrayEx operator-=(Line3DArrayEx &lineArray); //在当前数组与参数数组求差
    Line3DArrayEx operator|=(Line3DArrayEx &lineArray); //在当前数组上后接一数组，去掉重复元素

    void  Add(double x1,double y1,double z1,          //向数组中增加一线段
              double x2,double y2,double z2);
    void  Add(double a[],double b[]);                 //向数组中增加一线段
    void  Add(Line3D &line);
    void  SetUniDirection(void);                      //使线段方向保持一致
//    Arc3DArrayEx GetArcs();                           //从有向线段搜索弧段

public:
    virtual ~Line3DArrayEx()
    {
    }
};

//class Triangle2DArrayEx        //三角形数组类
//{
//    vector<Triangle2D> v;
//public:
//    int size(){
//        return v.size();
//
//    }
//    Triangle2D& at(int i){
//        return v[i];
//    }
//    void clear(){
//        v.clear();
//    }
//    Triangle2DArrayEx()
//    {
//        m_strID="";
//        m_strSubID="";
//        m_attr1=m_attr2=m_ID=-1;
//    }
//    Triangle2DArrayEx(Triangle2DArray &triangleList);
//    Triangle2DArrayEx(Triangle2DArrayEx &triangleList);
//public:
//    int      m_attr1;
//    int      m_attr2;
//    int      m_ID;
//    string  m_strID;
//    string  m_strSubID;
//public:
//    Triangle2DArrayEx operator =(Triangle2DArray &triangleList);
//    Triangle2DArrayEx operator =(Triangle2DArrayEx &triangleList);
//    Triangle2DArrayEx operator+=(Triangle2DArrayEx &triangleList);//在当前数组上后接一数组
//    Triangle2DArrayEx operator-=(Triangle2DArrayEx &triangleList);//在当前数组上后接一数组
//    Triangle2DArrayEx operator|=(Triangle2DArrayEx &triangleList);//在当前数组上后接一数组，去年重复元素
//
//    void Add(double x1,double y1,                     //在数组中增加一元素
//             double x2,double y2,
//             double x3,double y3 );
//    void Add(double a[],double b[],double c[]);       //在数组中增加一元素
//    void Add(Triangle2D &triangle);
//    void ReverseDirection();                          //各三角形的顶点反向存储
//    Line2DArrayEx  GetBoundaryLines(void);
//public:
//    virtual ~Triangle2DArrayEx()
//    {
//    }
//};
class Triangle3DArrayEx     //三角形数组类
{
public:
    vector<Triangle3D> v;
    int size(){
        return v.size();
    }
    Triangle3D& at(int i){
        return v[i];
    }
    void clear(){
        v.clear();
    }

    void SetAt(int i, Triangle3D d){
        v[i] = d;
    }
    Triangle3DArrayEx()
    {
        m_strID="";
        m_strSubID="";
        m_attr1=m_attr2=m_ID=-1;
    }
    Triangle3DArrayEx(Triangle3DArrayEx &&triangles);
    Triangle3DArrayEx(Triangle3DArrayEx &triangles);
public:
    int      m_attr1;
    int      m_attr2;
    int      m_ID;
    string  m_strID;
    string  m_strSubID;
public:
//    Triangle3DArrayEx operator =(Triangle3DArray &triangleList);
    Triangle3DArrayEx operator =(Triangle3DArrayEx &triangleList);
    Triangle3DArrayEx operator =(Triangle3DArrayEx triangleList);
    Triangle3DArrayEx operator+=(Triangle3DArrayEx &triangleList);//在当前数组上后接一数组
    Triangle3DArrayEx operator-=(Triangle3DArrayEx &triangleList);//在当前数组上后接一数组
    Triangle3DArrayEx operator|=(Triangle3DArrayEx &triangleList);//在当前数组上后接一数组，去年重复元素

    void Add(double x1,double y1,double z1,           //在数组中增加一元素
             double x2,double y2,double z2,
             double x3,double y3,double z3);
    void Add(double a[],double b[],double c[]);       //在数组中增加一元素
    void Add(Triangle3D &triangle);                   //在数组中增加一元素
    bool SetUniDirection();                           //把组成曲面的三角形的法向量置为指向曲面的同一侧
    void SetUpDirection();
    void ReverseDirection();                          //各三角形的顶点反向存储
    int  IsBody();

    //-----------------------------------------------------------------------------
    //函数名称：GetBoundaryLines
    //函数功能：取由三角形组成的曲面的边界线段。
    //输入参数：无
    //输出参数：无
    //返回结果：曲面的边界线段
    Line3DArrayEx  GetBoundaryLines(void);

    Triangle3DArrayEx GetAverageNormals();
    Triangle3DArrayEx GetSimpleNormals();
    void GetSimpleNormals(Point3DArrayEx &normalArray);
//    void GetSimpleNormals(NormalArray &normalArray);
public:
    virtual ~Triangle3DArrayEx()
    {
    }
};

//class IntArrayEx : public IntArray
//{
//public:
//
//    IntArrayEx(){}
//    IntArrayEx(IntArray &intArray)
//    {
//        clear();
//        int num=intArray.size ();
//        for(int i=0;i<num;i++)
//            v.push_back(intArray.at (i));
//    }
//public:
//    //都是深拷贝
//    IntArrayEx operator=(IntArray &intArray)
//    {
//        v.clear();
//        int num=intArray.size ();
//        for(int i=0;i<num;i++)
//            v.push_back(intArray.at (i));
//        return *this;
//    }
//    IntArrayEx operator+=(IntArray &intArray)
//    {
//        v.clear();
//        int num=intArray.size ();
//        for(int i=0;i<num;i++)
//            v.push_back(intArray.at (i));
//        return *this;
//    }
//};


class LineArrayEx
{

public:

    vector<Line> v;
    LineArrayEx(){}

    LineArrayEx(LineArrayEx &lineArray)
    {
        m_ID=lineArray.m_ID;
        v.clear();
        int num=lineArray.size();
        for(int i=0;i<num;i++)
            v.push_back(lineArray.at(i));
    }
public:
    int m_ID;
public:

    int size(){
        return v.size();
    }

    Line at(int i){
        return v.at(i);
    }


    LineArrayEx operator+=(LineArrayEx &lineArray)
    {
        int num=lineArray.size();
        for(int i=0;i<num;i++)
            v.push_back(lineArray.at(i));
        return *this;
    }
};
class EdgeArrayEx
{
public:
    vector<mcEdge> v;
    EdgeArrayEx(){}

    EdgeArrayEx(EdgeArrayEx &edgeArray)
    {
        m_ID=edgeArray.m_ID;
        v.clear();
        int num=edgeArray.size();
        for(int i=0;i<num;i++)
            v.push_back(edgeArray.at(i));
    }
public:
    int m_ID;
public:

    int size(){
        return v.size();
    }
    mcEdge& at(int i){
        return v[i];
    }

    EdgeArrayEx operator=(EdgeArrayEx &edgeArray)
    {
        m_ID=edgeArray.m_ID;
        v.clear();
        int num=edgeArray.size();
        for(int i=0;i<num;i++)
            v.push_back(edgeArray.at(i));
        return *this;
    }

};
class TriangleArrayEx
{
public:
    vector<mcTriangle> v;
    TriangleArrayEx(){}
//    TriangleArrayEx(TriangleArray &triangleArray)
//    {
//        v.clear();
//        int num=triangleArray.size();
//        for(int i=0;i<num;i++)
//            v.push_back(triangleArray.at(i));
//    }
    TriangleArrayEx(TriangleArrayEx &triangleArray)
    {
        m_ID=triangleArray.m_ID ;
        v.clear();
        int num=triangleArray.size();
        for(int i=0;i<num;i++)
            v.push_back(triangleArray.at(i));
    }
public:
    int m_ID;
public:
    int size(){
        return v.size();
    }
    mcTriangle at(int i){
        return v[i];
    }
//    TriangleArrayEx operator=(TriangleArray &triangleArray)
//    {
//        v.clear();
//        int num=triangleArray.size();
//        for(int i=0;i<num;i++)
//            v.push_back(triangleArray.at(i));
//        return *this;
//    }
    TriangleArrayEx operator=(TriangleArrayEx &triangleArray)
    {
        m_ID=triangleArray.m_ID ;
        v.clear();
        int num=triangleArray.size();
        for(int i=0;i<num;i++)
            v.push_back(triangleArray.at(i));
        return *this;
    }
//    TriangleArrayEx operator+=(TriangleArray &triangleArray)
//    {
//        int num=triangleArray.size();
//        for(int i=0;i<num;i++)
//            v.push_back(triangleArray.at(i));
//        return *this;
//    }
};

class Polygon2D:public Point2DArrayEx         //多边形类，闭合点不重复存储
{
public:
    Polygon2D(void){}
//    Polygon2D(Point2DArray &pointArray);
    Polygon2D(Point2DArrayEx &pointArray);
    Polygon2D(Polygon2D  &polygon);
public:
//    Polygon2D operator=(Point3DArray &pointArray);
    Polygon2D operator=(Point3DArrayEx &pointArray);
    Polygon2D operator=(Polygon2D &polygon);
    int       operator==(Polygon2D &polygon);     //返回非零表示顶点相同，值的正负表方向是否一致
    int       operator!=(Polygon2D &polygon);     //判断无向多边形是否不相等
    int  HasVertex(double x,double y);            //判断坐标点是否为多边形的顶点,返回顶点从1开始计数的序号
    int  HasVertex(double a[]);                   //判断坐标点是否为多边形的顶点
    int  HasVertex(Point2D point);                //判断坐标点是否为多边形的顶点
    bool IsConvexPolygon(void);	                  //判断是否为凸多边形
    bool IsDelaunayPolygon(void);	              //判断是否为空圆凸多边形
    bool IsCounterclockwise(double n[1]);         //判断是否为逆时针
    bool IsCounterclockwise()
    {
        double n[1]={1};
        return IsCounterclockwise(n);
    }
    bool IsAnticlockwise()
    {
        return IsCounterclockwise();
    }
//    Triangle2DArrayEx GetInnerTriangles();        //最大空圆凸多边形的内部三角形剖分
    Line2DArrayEx  GetLines();                    //提取多边形的所有边界线段

    //----------------------------------------------------------------
    //函数名称：Contain
    //函数功能：判断点是否在多边形内部
    //输入参数：坐标点point
    //输出参数：无
    //返回结果：点在多边形上返回1，点在多边形内返回2，否则返回0。
    int Contain(Point2D point);

    double GetArea();
private:
    double m_n[1];
public:
    virtual ~Polygon2D(void)
    {
    }
};

class Polygon3D:public Point3DArrayEx            //多边形类，闭合点不重复存储
{
public:
    Polygon3D(){};
    Polygon3D(Point3DArrayEx& pointArray);
//    Polygon3D(Polygon3D  &polygon);
//    Polygon3D(Polygon3D  &&polygon);
public:
    Polygon3D operator=(Polygon3D &polygon);
    Polygon3D operator=(Polygon3D polygon);
    int       operator==(Polygon3D &polygon);     //返回非零表示顶点相同，值的正负表方向是否一致
    int       operator!=(Polygon3D &polygon);     //判断无向多边形是否不相等
    int  HasVertex(double x,double y,double z);   //判断坐标点是否为多边形的顶点
    int  HasVertex(double a[]);                   //判断坐标点是否为多边形的顶点
    int  HasVertex(Point3D point);                //判断坐标点是否为多边形的顶点
    bool IsConvexPolygon(void);	                  //判断是否为凸多边形
    bool IsDelaunayPolygon(void);	              //判断是否为空圆凸多边形
    bool IsCounterclockwise(double n[]);          //相对于法向量n,判断是否为逆时针
    int  IsAnticlockwise(double n[])
    {
        return IsCounterclockwise(n);
    }
    Triangle3DArrayEx GetInnerTriangles();          //最大空圆凸多边形的内部三角形剖分
    Line3DArrayEx&  GetLines();                      //提取多边形的所有边界线段
    //----------------------------------------------------------------
    //函数名称：Contain
    //函数功能：判断空间一坐标点向平面的投影点是否在空间多边形向平面投影形成的平面多边形内部
    //输入参数：坐标点point和投影方向矢量n[]
    //输出参数：无
    //返回结果：在多边形上返回1，在多边形内部返回2，否则返回0
    int Contain(Point3D point,double n[]);
    int Contain(Point3D point)
    {
        return Contain(point,m_n);
    }
    double GetArea();
private:
    double m_n[3];                                //多边形面片的正侧法向量
public:
    virtual ~Polygon3D(void)
    {
    }
};

//多边形组，其点是多个多边形
class PolyPoint3DArrayEx 
{
public:
    vector<Point3DArrayEx> v;
    int size(){
        return v.size();
    }
    void clear(){
        v.clear();
    }
    Point3DArrayEx& at(int i){
        return v[i];
    }

    void Add(Point3DArrayEx p){
        v.push_back(p);
    }

    void erase(int i){
        v.erase(v.begin() + i);
    }
    PolyPoint3DArrayEx(){}
    PolyPoint3DArrayEx(PolyPoint3DArrayEx &polyPointArray)
    {
        
        int num=polyPointArray.size ();
        for(int i=0;i<num;i++)
            v.push_back(polyPointArray.at (i));
    }
    PolyPoint3DArrayEx(PolyPoint3DArrayEx &&polyPointArray)
    {

        int num=polyPointArray.size ();
        for(int i=0;i<num;i++)
            v.push_back(polyPointArray.at (i));
    }

public:
    int m_ID;
public:

//    PolyPoint3DArrayEx operator=(PolyPoint3DArrayEx &polyPointArray)
//    {
//        clear();
//        if (polyPointArray.size()<=0)
//            return * this;
//
//        m_ID=polyPointArray.m_ID;
//        int num=polyPointArray.size ();
//        for(int i=0;i<num;i++)
//            v.push_back(polyPointArray.at(i));
//        return *this;
//    }
//    PolyPoint3DArrayEx operator=(PolyPoint3DArrayEx &&polyPointArray)
//    {
//        clear();
//        if (polyPointArray.size()<=0)
//            return * this;
//
//        m_ID=polyPointArray.m_ID;
//        int num=polyPointArray.size ();
//        for(int i=0;i<num;i++)
//            v.push_back(polyPointArray.at(i));
//        return *this;
//    }

    PolyPoint3DArrayEx operator=(PolyPoint3DArrayEx polyPointArray)
    {
        clear();
        if (polyPointArray.size()<=0)
            return * this;

        m_ID=polyPointArray.m_ID;
        int num=polyPointArray.size ();
        for(int i=0;i<num;i++)
            v.push_back(polyPointArray.at(i));
        return *this;
    }

    PolyPoint3DArrayEx operator+=(PolyPoint3DArrayEx &polyPointArray)
    {
        m_ID=polyPointArray.m_ID;
        int num=polyPointArray.size ();
        for(int i=0;i<num;i++)
            v.push_back(polyPointArray.at (i));
        return *this;
    }
//    void OrderByID()
//    {
//        int i,j,num;
//        Point3DArrayEx objPolyPoint,temPolyPoint;
//        num=size ();
//        for(i=0;i<num-1;i++)
//        {
//            for(j=num-1;j>i;j--)
//            {
//                objPolyPoint= at (j-1);
//                temPolyPoin =at (j);
//                if(objPolyPoint.m_ID >temPolyPoint.m_ID )
//                {
//                    SetAt (j-1,temPolyPoint);
//                    SetAt (j,objPolyPoint);
//                }
//            }
//        }
//    }
};

class Polygon3DArrayEx
{
public:
    vector<Polygon3D> v;
    int size(){
        return v.size();
    }
    Polygon3D& at(int i){
        return v[i];
    }
    void clear(){
        v.clear();
    }

    void Add(Polygon3D p){
        v.push_back(p);
    }
    Polygon3DArrayEx(){}

    Polygon3DArrayEx(Polygon3DArrayEx &polygons)
    {
        m_ID=polygons.m_ID;
        clear();
        int num=polygons.size();
        for(int i=0;i<num;i++)
            v.push_back(polygons.at(i));
    }
public:
    int m_ID;
public:

    Polygon3DArrayEx operator=(Polygon3DArrayEx &polygons)
    {
        m_ID=polygons.m_ID;
        clear();
        int num=polygons.size();
        for(int i=0;i<num;i++)
            v.push_back(polygons.at(i));
        return *this;
    }
    Polygon3DArrayEx operator+=(Polygon3DArrayEx &polygons)
    {
        int num=polygons.size();
        for(int i=0;i<num;i++)
            v.push_back(polygons.at(i));
        return *this;
    }

    //----------------------------------------------------------------------------------
    //函数名称：GetExtremeMaxPolygon
    //函数功能：对于彼此具有包含或相离关系的简单多边形构成的数组,计算极大多边形
    //输入参数：无
    //输出参数：与返回极大多边形数组相对应的极大多边形序号数组
    //返回结果：返回极大多边形数组
//    Polygon3DArrayEx GetExtremeMaxPolygon(IntArray &indices,double n[3]);
//    Polygon3DArrayEx GetExtremeMaxPolygon(double n[3]);
//    Polygon3DArrayEx GetExtremeMaxPolygon();
//
//    //----------------------------------------------------------------------------------
//    //函数名称：GetComplexPolygon
//    //函数功能：对于彼此具有包含或相离关系的简单多边形构成的数组,计算复杂多边形
//    //输入参数：极大多边形序号数组
//    //输出参数：构成复杂多边形的简单多边形序号集构成的数组
//    //返回结果：返回与极大多边形相关的复杂多边形数组
//    ComplexPolygon3DArrayEx GetComplexPolygon(IntArray &indices,PolyIntArray& polyIntegerArray,double n[3]);
//    ComplexPolygon3DArrayEx GetComplexPolygon(IntArray &indices,double n[3]);
//    ComplexPolygon3DArrayEx GetComplexPolygon(IntArray &indices);
};



#endif //LEARNOPENGL_COMMON_H
