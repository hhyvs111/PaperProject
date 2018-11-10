//
// Created by 谭文波 on 2018/10/18.
//




#ifndef LEARNOPENGL_VERTEX_H
#define LEARNOPENGL_VERTEX_H



using namespace std;
typedef struct   tagVERTEX
{
    float   x;  // x坐标
    float   y;  // y坐标
    float   z;  // z坐标
}VERTEX;

typedef struct tagAddTriangle
{
    VERTEX a;
    VERTEX b;
    VERTEX c;
}AddTriangle;


enum moveDirection {
    xD,
    yD,
    zD
};

typedef struct tagMoveSize{
    float size;
    moveDirection md;
}moveSize;

float * VertexToFloat(VERTEX vertex[], int num);

VERTEX * FloatToVertex(float _float[], int num);


VERTEX * faultMerge(VERTEX vertex1[], int num1, VERTEX vertex2[], int num2);


void faultMoveFunction(VERTEX *vertex, int num, float moveSize, int whichDirection);


bool lineIntersectSide(VERTEX A, VERTEX B, VERTEX C, VERTEX D);

//主要原理就是另一条线的一个点是否在一条直线的同一侧？如果两条线都不在同一侧那么就是不想交的。
bool sideIntersectSide(VERTEX A, VERTEX B, VERTEX C, VERTEX D);


//判断这两个断层是否相交
//
bool faultIntersect(VERTEX fault1[], int f1Number, VERTEX fault2[], int f2Number);


float DistanceOfPointLinesIn2D(VERTEX point, VERTEX a, VERTEX b);

float DistanceOfPointToPointIn3D(VERTEX point1, VERTEX point2);

float DistanceOfPointLinesIn3D(VERTEX a, VERTEX b, VERTEX s);

float DistanceOfOpposite(VERTEX point, VERTEX Opposite[], int num, int &index);

AddTriangle VertexToTriangle(VERTEX a, VERTEX b, VERTEX c);
#endif //LEARNOPENGL_VERTEX_H
