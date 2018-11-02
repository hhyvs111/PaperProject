//
// Created by 谭文波 on 2018/10/18.
//
#include <vertex.h>
#include <iostream>
#include <cstring>
#include <cmath>
float * VertexToFloat(VERTEX vertex[], int num)
{
    if(vertex == NULL)
        return NULL;
    //new分配空间
    float * _float = new float[num * 3];
    for(int i = 0,j = 0;i < num && j < num * 3; i++)
    {
        _float[j++] = vertex[i].x;
        _float[j++] = vertex[i].y;
        _float[j++] = vertex[i].z;
    }
    return _float;
}

VERTEX * FloatToVertex(float _float[], int num)
{
    if(_float == NULL)
        return NULL;
    VERTEX * vertex = new VERTEX[num/3];
    for(int i = 0,j = 0; i < num/3 && j < num; i++)
    {
        vertex[i].x = _float[j++];
        vertex[i].y = _float[j++];
        vertex[i].z = _float[j++];

    }
    return vertex;
}

VERTEX * faultMerge(VERTEX vertex1[], int num1, VERTEX vertex2[], int num2)
{
    VERTEX *merge = new VERTEX[num1 + num2];
    //不用考虑排序就这样插入吧。
    for(int i = 0;i < num1;i++)
    {
        merge[i] = vertex1[i];
    }

    for(int i = num1,j = num2 -1 ;j >= 0;j--)
    {
        merge[i++] = vertex2[j];
    }
    return merge;
}

void faultMoveFunction(VERTEX *vertex, int num, float moveSize, int whichDirection)
{
    cout<<"movefunction"<<endl;
    //先写一下这个yd得了
    if(whichDirection == xD)
    {
        for(int i = 0;i < num; i++)
        {
            vertex[i].x += moveSize;
        }
    }
    if(whichDirection == yD)
    {
        //从1开头，然后加等于2
        for(int i = 0;i < num; i++)
        {
            vertex[i].y += moveSize;
        }
    }
    if(whichDirection == zD)
    {
        for(int i = 0;i < num;i++)
        {
            vertex[i].z += moveSize;
        }
    }
}

bool lineIntersectSide(VERTEX A, VERTEX B, VERTEX C, VERTEX D)
{
    float fC = (C.y - A.y) * (A.x - B.x) - (C.x - A.x) * (A.y - B.y);
    float fD = (D.y - A.y) * (A.x - B.x) - (D.x - A.x) * (A.y - B.y);
    if(fC * fD > 0)
        return false;
    return true;

}
//主要原理就是另一条线的一个点是否在一条直线的同一侧？如果两条线都不在同一侧那么就是不想交的。
bool sideIntersectSide(VERTEX A, VERTEX B, VERTEX C, VERTEX D)
{
    if(!lineIntersectSide(A, B, C, D))
        return false;
    if(!lineIntersectSide(C, D, A, B))
        return false;

    return true;
}

//判断这两个断层是否相交
//
bool faultIntersect(VERTEX fault1[], int f1Number, VERTEX fault2[], int f2Number)
{
    //断层里的线是依次的，基本上第一个点的ip是基本在同一起点，那么还是要判断多重啊。不能单一的判断，那么就是一个线段是否与另一条的所有线段相交
    //如果相交则要平移一下，但是这样感觉效率好低啊，不管了，先这样做吧。应该计算量不大，这个东西的话是一个基本的算法，时间复杂度应该不高。
    for(int i = 0; i < f1Number - 1; i++)
    {
        for(int j = 0; j < f2Number - 1; j++)
        {
            //如果有任意一条相交，那么该线就需要平移。
            if(sideIntersectSide(fault1[i],fault1[i+1], fault2[j], fault2[j+1]))
                return false;
        }
    }
    return true;
}
//point点到直线ab的距离
float DistanceOfPointLinesIn2D(VERTEX point, VERTEX a, VERTEX b)
{

    return (fabs((b.y - a.y) * point.x +(a.x - b.x) * point.y + ((b.x * a.y) -(a.x * b.y)))) / (sqrt(pow(b.y - a.y, 2) + pow(a.x - b.x, 2)));
}

//计算三维空间两点的距离。
float DistanceOfPointToPointIn3D(VERTEX point1, VERTEX point2)
{
    return (fabs(sqrt(pow((point1.x - point2.x), 2) + pow((point1.y - point2.y), 2) + pow((point1.z - point2.z), 2))));
}


float DistanceOfPointLinesIn3D(VERTEX a, VERTEX b, VERTEX s)
{
    float ab = sqrt(pow((a.x - b.x), 2.0) + pow((a.y - b.y), 2.0) + pow((a.z - b.z), 2.0));
    float as = sqrt(pow((a.x - s.x), 2.0) + pow((a.y - s.y), 2.0) + pow((a.z - s.z), 2.0));
    float bs = sqrt(pow((s.x - b.x), 2.0) + pow((s.y - b.y), 2.0) + pow((s.z - b.z), 2.0));
    float cos_A = (pow(as, 2.0) + pow(ab, 2.0) - pow(bs, 2.0)) / (2 * ab*as);
    float sin_A = sqrt(1 - pow(cos_A, 2.0));
    return as*sin_A;
}

//求点到对线的最短距离。
float DistanceOfOpposite(VERTEX point, VERTEX Opposite[], int num, int &index)
{
//    float minDistance = DistanceOfPointLinesIn3D(point, Opposite[0], Opposite[1]);
    float minDistance = DistanceOfPointToPointIn3D(point, Opposite[0]);
    for(int i = 1;i < num - 1; i++)
    {
        float distance = DistanceOfPointToPointIn3D(point, Opposite[i]);
//        float distance = DistanceOfPointLinesIn3D(point, Opposite[i], Opposite[i+1]);
        cout<< "the d2 distance : " << distance << endl;
        if( distance < minDistance )
        {
            minDistance = distance;
            index = i;
        }
    }
    return minDistance;
}

AddTriangle VertexToTriangle(VERTEX a, VERTEX b, VERTEX c)
{
    AddTriangle _triangle;
    _triangle.a = a;
    _triangle.b = b;
    _triangle.c = c;
    return  _triangle;
}