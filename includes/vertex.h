//
// Created by 谭文波 on 2018/10/18.
//

#ifndef LEARNOPENGL_VERTEX_H
#define LEARNOPENGL_VERTEX_H

#include<iostream>
#include<vector>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include "math.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//底部平台三角形
const int IsBaseTri = 1;
//两个点在洞上的三角形
const int IsMidTri = 2;
//三个点都在洞上的三角形
const int IsTopTri = 3;
using namespace std;
typedef struct   tagVERTEX
{
    float   x;  // x坐标
    float   y;  // y坐标
    float   z;  // z坐标

    //重载
    bool operator==(const tagVERTEX b) const
    {
        return ((this->x == b.x) && (this->y == b.y) && (this->z == b.z));
    }

    void Print(){
        cout << x << "," << y << "," << z << endl;
    }

    //层数
    int index;
}VERTEX,GLvector;

class PolyLine{
public:
    vector<VERTEX> v;
    int sectionNum;


    void add(VERTEX& t){
        v.push_back(t);
    }

};


typedef struct tagAddTriangle
{
    VERTEX a;
    VERTEX b;
    VERTEX c;
}AddTriangle;



//GLfloat   fTime = 0.0;
//GLvector  sSourcePoint[3];
//GLboolean bLight = true;


//void vIdle();
//void vDrawScene();
//void vResize(GLsizei, GLsizei);
//void vKeyboard(unsigned char cKey, int iX, int iY);
//void vSpecial(int iKey, int iX, int iY);
//
//
//
//void spinDisplay();
//void yawDisplay(void);
//void myMouseCall(int button, int state, int x ,int y);

//GLvoid vPrintHelp();

//头文件只做函数声明
void readFile();
GLvoid vMarchingCubes();
void SetSample(float NewTime);
GLvoid vMarchCube(GLfloat x, GLfloat y, GLfloat z, GLfloat fScale);
//GLvoid (*vMarchCube)(GLfloat x, GLfloat y, GLfloat z, GLfloat fScale) = vMarchCube1;

//只能这么多数据？，这个应该是立方体的个数


//GLuint marchingCube=0;

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


void faultMoveFunction(vector<VERTEX>&, float moveSize, int whichDirection);

void faultScaleFunction(vector<VERTEX>& vertex, float scaleSize, int whichDirection);

bool lineIntersectSide(VERTEX A, VERTEX B, VERTEX C, VERTEX D);

//主要原理就是另一条线的一个点是否在一条直线的同一侧？如果两条线都不在同一侧那么就是不想交的。
bool sideIntersectSide(VERTEX A, VERTEX B, VERTEX C, VERTEX D);


//判断这两个断层是否相交
//
bool faultIntersect(const vector<VERTEX>& fault1, const vector<VERTEX>& fault2);

//判断点是否在集合里,存在返回其下标，不存在返回-1
int VertexInVertexs(VERTEX target, const vector<VERTEX>& source);

float DistanceOfPointLinesIn2D(VERTEX point, VERTEX a, VERTEX b);

float DistanceOfPointToPointIn3D(VERTEX point1, VERTEX point2);

float DistanceOfPointLinesIn3D(VERTEX a, VERTEX b, VERTEX s);

float DistanceOfOpposite(VERTEX point, const vector<VERTEX>& Opposite, int &index);

AddTriangle VertexToTriangle(VERTEX a, VERTEX b, VERTEX c);

//获取平面法向量
VERTEX getNormal(const VERTEX& p1, const VERTEX& p2, const VERTEX& p3);

//文本输入
void InputDataToVector(vector<vector<vector<VERTEX>>>& closeLines);
//数据划分
void VertexDivide(vector<VERTEX>& v, vector<vector<VERTEX>>& closeLineV);

#endif //LEARNOPENGL_VERTEX_H
