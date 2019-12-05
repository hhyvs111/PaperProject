//
// Created by 谭文波 on 2019/12/1.
//

#ifndef LEARNOPENGL_PSEUDODELAUNAY_H
#define LEARNOPENGL_PSEUDODELAUNAY_H
//PseudoDelaunay.h
#ifndef  H_PseudoDelaunay
#define  H_PseudoDelaunay

#include "common.h"

class PseudoDelaunay            //约束三角剖分类
{
public:
    PseudoDelaunay(void){}
    PseudoDelaunay(Line3DArrayEx   &lineArray);
    PseudoDelaunay(Point3DArrayEx  &pointArray,
                   Line3DArrayEx   &lineArray);
    PseudoDelaunay(PseudoDelaunay &pseudoDelaunay);
public:
    double m_n[3];               //法向量
    Point3DArrayEx m_pointArray; //离散点数组,不包含约束边上的点
    Line3DArrayEx  m_lineArray;  //约束曲线，由一个逆时针外环多边形和零到多个顺时针内环多边形组成
public:
    PseudoDelaunay operator=(PseudoDelaunay &pseudoDelaunay);
    //------------------------------------------------------------------
    //函数名称：HasNormalVector
    //函数功能：计算点列中的点所能构成的最大三角形所在平面的法向量，
    //          三角形的顶点按它们在点列中的顺序排列，
    //          三角形的方向与法向量成右手关系。
    //输入参数：点列pointArray。
    //输出参数：平面法向量n[]。
    //返回结果：返回0表示点列中的所有点共线，返回1表示法向量存在。
    //---------------------------------------------------------
    int HasNormalVector(Point3DArrayEx &pointArray ,double n[]);
    //----------------------------------------------------------------------
    //函数名称：IsAInnerTriangle
    //函数功能：判断约束曲线的一条有向边与一个离散点构成的三角形是否在曲线所围区域内。
    //输入参数：约束曲线上一有向边的两端点a[]和b[]，离散点c[]。
    //输出参数：无。
    //返回结果：返回0表示否，返回1表示是。
    //---------------------------------------------------------
    int IsAInnerTriangle(double a[],double b[],double c[]);
    //----------------------------------------------------------------------
    //函数名称：IsAInnerTriangle
    //函数功能：判断约束曲线的一条有向边与一个离散点构成的三角形是否在曲线所围区域内。
    //输入参数：约束曲线上一有向边的两端点point1和point2，离散点point3。
    //输出参数：无。
    //返回结果：返回0表示否，返回1表示是。
    //---------------------------------------------------------
    int IsAInnerTriangle(Point3D &poin1,Point3D &point2,Point3D &point3);
    //----------------------------------------------------------------------
    //函数名称：IsAInnerTriangle
    //函数功能：判断约束曲线的一条有向边与一个离散点构成的三角形是否在曲线所围区域内。
    //输入参数：约束曲线上一有向边line，离散点point。
    //输出参数：无。
    //返回结果：返回0表示否，返回1表示是。
    //---------------------------------------------------------
    int IsAInnerTriangle(Line3D &line,Point3D &point);
    //----------------------------------------------------------------------
    //函数名称：IsAPDTriangle
    //函数功能：判断三角形是否为约束Delaunay三角形。
    //输入参数：三角形的三顶点a[]、b[]和c[]。
    //输出参数：无。
    //返回结果：返回0表示否，返回1表示是。
    //---------------------------------------------------------
    int IsAPDTriangle(double a[],double b[],double c[]);
    //----------------------------------------------------------------------
    //函数名称：IsAPDTriangle
    //函数功能：判断三角形是否为约束Delaunay三角形。
    //输入参数：三角形的三顶点point1、point2和point3。
    //输出参数：无。
    //返回结果：返回0表示否，返回1表示是。
    //-----------------------------------------------------
    int IsAPDTriangle(Point3D &point1,Point3D &point2,Point3D &point3);
    //----------------------------------------------------------------------
    //函数名称：IsAPDTriangle
    //函数功能：判断三角形是否为约束Delaunay三角形。
    //输入参数：三角形的一条边line和一个顶点point。
    //输出参数：无。
    //返回结果：返回0表示否，返回1表示是。
    //-----------------------------------------------------
    int IsAPDTriangle(Line3D &line,Point3D &point);
    //----------------------------------------------------------------------
    //函数名称：GetConstrainPolygon
    //函数功能：计算约束最大空圆凸多边形，该多边形的方向与已知三角形的方向相反。
    //输入参数：三角形的三顶点a[]、b[]和c[]。
    //输出参数：无。
    //返回结果：返回约束最大空圆凸多边形。
    //-----------------------------------------------------
    Polygon3D GetConstrainPolygon(double a[],double b[],double c[]);
    //----------------------------------------------------------------------
    //函数名称：GetConstrainPolygon
    //函数功能：计算约束最大空圆凸多边形，该多边形的方向与已知三角形的方向相反。
    //输入参数：三角形的三顶点point1、point2和point3。
    //输出参数：无。
    //返回结果：返回约束最大空圆凸多边形。
    //-----------------------------------------------------
    Polygon3D GetConstrainPolygon(Point3D &point1,Point3D &point2,Point3D &point3);
    //----------------------------------------------------------------------
    //函数名称：GetConstrainPolygon
    //函数功能：计算约束最大空圆凸多边形，该多边形的方向与已知三角形的方向相反。
    //输入参数：三角形triangle。
    //输出参数：无。
    //返回结果：返回约束最大空圆凸多边形。
    //-----------------------------------------------------
    Polygon3D GetConstrainPolygon(Triangle3D &triangle);
    //----------------------------------------------------------------------
    //函数名称：CreatePolygonArray
    //函数功能：计算平面散点集约束最大空圆凸多边形剖分。
    //输入参数：无。
    //输出参数：无。
    //返回结果：返回约束最大空圆凸多边形数组。
    //--------------------------------------------------
    Polygon3DArrayEx  CreatePolygonArray(void);
    //----------------------------------------------------------------------
    //函数名称：CreateTriangleArray
    //函数功能：计算平面散点集约束三角形剖分。
    //输入参数：无。
    //输出参数：无。
    //返回结果：返回约束Delaunay三角形数组。
    Triangle3DArrayEx CreateTriangleArray(void);

    //----------------------------------------------------------------------
    //函数名称：CreateTriangleArray
    //函数功能：计算空间曲面三角网（设该空间曲面到已知平面的投影映射为一一映射）。
    //输入参数：平面法向量n[]。
    //输出参数：无。
    //返回结果：返回三角形数组。
    Triangle3DArrayEx CreateTriangleArray(double n[]);
public:
    ~PseudoDelaunay(void){}
};

#endif
#endif //LEARNOPENGL_PSEUDODELAUNAY_H
