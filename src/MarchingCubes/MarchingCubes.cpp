//
// Created by 谭文波 on 2019/12/1.
//


#include "MarchingCubes.h"
#include <cmath>
#include "Tables.h"
#include <iostream>
#include "vertex.h"
#include "PseudoDelaunay.h"

extern vector<AddTriangle> McTri;
extern double mcScale;
#define SQR(X) ((X)*(X))
class CProgress;
MarchingCube::MarchingCube(int sizeI, int sizeJ, int sizeK, float isoLevel) {
    m_sizeI      = sizeI;
    m_sizeJ      = sizeJ;
    m_sizeK      = sizeK;
    m_IsoLevel   = isoLevel;
    m_Cubes  = NULL;
    m_GridPoints = NULL;
}
MarchingCube::MarchingCube()
{
    m_sizeI      = 1;
    m_sizeJ      = 30;
    m_sizeK      = 30;
    //分叉的80dd
    //越大的话，三角面片越少
    m_IsoLevel   = 180;
    m_Cubes  = NULL;
    m_GridPoints = NULL;

    m_sumTime = 0;
    m_sumTri = 0;
}
MarchingCube::~MarchingCube(void) {
    DeleteMerroy();
}

int MarchingCube::CreateSurface(void) {
    m_TriangleEx.clear();
    Line3DArrayEx fLineArray, bLineArray;
    string str;
    int triCount = 0;
    for (int i = 0; i < m_sizeI; i++) {
        for (int j = 0; j < m_sizeJ; j++) {
            for (int k = 0; k < m_sizeK; k++) {
                triCount += GenerateSurface(&m_Cubes[i][j][k], m_IsoLevel, fLineArray, bLineArray);
            }
        }
    }
    //trace the edge of surface to get the new contours;
    TracePointsToContours(fLineArray, m_fNewContours);
    TracePointsToContours(bLineArray, m_bNewContours);

    m_sumTri += triCount;
    cout << "trinum: " << m_sumTri << endl;
//    str.Format("有三角形:%d个\n", triCount);
    //AfxOutputDebugString(str);
    return triCount;
}




//void MarchingCube::CreateSurfaceByGenrate(PolyPoint3DArrayEx newContours, PolyPoint3DArrayEx refContours, Triangle3DArrayEx &surface, Point3DArrayEx &surfaceNormal)
//{
//    PolyPoint3D polyPoint, tempPolyPoint,newPolyPoint;
//    PolyPoint3DArrayEx polyPointArray;
//    double n1[3], n2[3];
//    Vector3D v1, v2, n;
//    int i,j, k, num, num1, num2, index1, index2,index;
//    Point3D p1, p2, q1, q2, nearestPoint, p, q;
//    double dis, minDis;
//    Triangle3D tri;
//    int attr1, attr2;
//    int randomIndex;
//    string strID, strSubID;
//
//    attr1 = polyPoint.m_attr1 ;
//    attr2 = polyPoint.m_attr2 ;
//    strID = polyPoint.m_strID ;
//    strSubID = polyPoint.m_strSubID ;
//
//    num = refContours.size();
//    polyPoint = refContours[0];
//
//    polyPoint.GetNormal(n1);
//    n.Set(n1);
//
//    //confirm the contours is the same direction;
//    polyPointArray = refContours;
//    for (i=0; i<num; i++)
//    {
//        polyPoint = refContours.at(i);
//        polyPoint.GetNormal(n1);
//        v1.Set(n1);
//        if (v1 != n)
//        {
//            polyPoint = polyPoint.ReverseOrder();
//        }
//        polyPointArray.Add(polyPoint);
//    }
//    refContours.clear();
//    refContours = polyPointArray;
//
//    polyPointArray.clear();
//    for (i=0; i<num; i++)
//    {
//        polyPoint = newContours.at(i);
//        polyPoint.GetNormal(n2);
//        v1.Set(n2);
//        if (v1 != n)
//        {
//            polyPoint = polyPoint.ReverseOrder();
//        }
//        polyPointArray.Add(polyPoint);
//    }
//    newContours.clear();
//    newContours = polyPointArray;
//
//    for (i=0; i<num; i++)
//    {
//        newPolyPoint.clear();
//        polyPoint = refContours.at(i);
//        tempPolyPoint = newContours.at(i);
//
//        //to find the first nearest point;
//        srand(time(NULL));
//        randomIndex = rand()%polyPoint.size();
////		afxDump<<"选取的起始点是："<<randomIndex<<"\n";
//        p1 = polyPoint.at(randomIndex);
//        num2 = tempPolyPoint.size();
//        for (k=0; k<num2; k++)
//        {
//            q1 = tempPolyPoint.at(k);
//            if (k==0)
//            {
//                minDis = dis = Relation3D::PToPDistance(p1, q1);
//                nearestPoint = q1;
//                index2 = k;
//            }
//            else
//            {
//                dis = Relation3D::PToPDistance(p1, q1);
//                if (minDis > dis)
//                {
//                    minDis = dis;
//                    nearestPoint = q1;
//                    index2 = k;
//                }
//            }
//        }
//        //resort the contour of tempPolyPoint and convert it to the contour;
//        for (j=index2; j<num2; j++)
//        {
//            newPolyPoint.Add(tempPolyPoint.at(j));
//        }
//        for (j=1; j<index2; j++)
//        {
//            newPolyPoint.Add(tempPolyPoint.at(j));
//        }
//        newPolyPoint.Add(tempPolyPoint.at(index2));
//
//        tempPolyPoint.clear();
//        for (j=randomIndex; j<polyPoint.size(); j++)
//        {
//            tempPolyPoint.Add(polyPoint.at(j));
//        }
//        for (j=1; j<randomIndex; j++)
//        {
//            tempPolyPoint.Add(polyPoint.at(j));
//        }
//        tempPolyPoint.Add(polyPoint.at(randomIndex));
//        polyPoint.clear();
//        polyPoint = tempPolyPoint;
//
//        //initial the index of the contour's point;
//        index1 = 0;
//        index2 = 1;
//
//        num1 = polyPoint.size();
//        num2 = newPolyPoint.size();
//
//        for (j=0; j<num2-1; j++)
//        {
//            q1 = newPolyPoint.at(j);
//            q2 = newPolyPoint.at((j+1)%num2);
//
//            for (k=index1; k<num1; k++)
//            {
//                p2 = polyPoint.at(k);
//                if (j==num2-2)
//                {
//                    index = index1;
//                    index1 = num1-1;
//                    nearestPoint = polyPoint.at(index1);
////					afxDump<<index<<" "<<index1<<"\n";
//                    break;
//                }
//                //if this is the first execue, initial the var;
//                if (k==index1)
//                {
//                    minDis = dis = Relation3D::PToPDistance(p2, q2);
//                    p = nearestPoint = p2;
//                    index = index1 = k;
//                }
//                else
//                {
//                    dis = Relation3D::PToPDistance(p2, q2);
//                    if (minDis > dis)
//                    {
//                        minDis = dis;
//                        nearestPoint = p2;
//                        index1 = k;
//                    }
//                }
//            }
//            if (nearestPoint == p)
//            {
//                tri.Set(q1, q2, nearestPoint);
//                surface.Add(tri);
//            }
//            else
//            {
//                tri.Set(q1, q2, nearestPoint);
//                surface.Add(tri);
//                for (int h=index; h<index1; h++)
//                {
//                    p1 = polyPoint.at(h);
//                    p2 = polyPoint.at(h+1);
//                    tri.Set(p2, p1, q1);
//                    surface.Add(tri);
//                }
//            }
//        }
//    }
//    surface.m_attr1 =attr1;
//    surface.m_attr2 =attr2 ;
//    surface.m_strID =strID ;
//    surface.m_strSubID =strSubID ;
//    surface.GetSimpleNormals (surfaceNormal);
//    //	afxDump<<"\n"<<surface.size()<<"\n";
//}


//void MarchingCube::CreateSurfaceBySplice(PolyPoint3DArrayEx &newContours, PolyPoint3DArrayEx &refContours, Triangle3DArrayEx &surface, Point3DArrayEx &surfaceNormal)
//{
//    PolyPoint3D polyPoint, tempPolyPoint, newPolyPoint, *ptrPolyPoint;
//    PolyPoint3DArrayEx polyPointArray;
//    Triangle3D objTriangle;
//    int i, j, k, num, num1, num2, attr1, attr2, count;
//    string strID, strSubID;
//    GeneticA         objGA;
//    Point3D point,*pPoint, p1, p2, p3, nearestPoint;
//    double dis, minDis;
//    int index1,index2, index, lastIndex;
//    double angle, n1[3], n2[3];
//    Vector3D v1, v2, n;
//    Point3DArrayEx pointArray;
//    Line3D line;
//    array<int>indexArray;
//
//    num = refContours.size();
//    num1 = newContours.size();
//    if (num1 != num)
//    {
//        AfxOutputDebugString("轮廓线对应时出错误！");
//        return;
//    }
//    if (num > 0)
//    {
//        polyPoint = refContours.at (0);
//        attr1 = polyPoint.m_attr1 ;
//        attr2 = polyPoint.m_attr2 ;
//        strID = polyPoint.m_strID ;
//        strSubID = polyPoint.m_strSubID ;
//    }
//
//    polyPoint.GetNormal(n1);
//    n.Set(n1);
//
//    //使轮廓线方向一致;
//    polyPointArray = refContours;
//    for (i=0; i<num; i++)
//    {
//        polyPoint = refContours.at(i);
//        polyPoint.GetNormal(n1);
//        v1.Set(n1);
//        if (v1 != n)
//        {
//            polyPoint = polyPoint.ReverseOrder();
//        }
//        polyPointArray.Add(polyPoint);
//    }
//    refContours.clear();
//    refContours = polyPointArray;
//    polyPointArray.clear();
//    for (i=0; i<num; i++)
//    {
//        polyPoint = newContours.at(i);
//        polyPoint.GetNormal(n2);
//        v1.Set(n2);
//        if (v1 != n)
//        {
//            polyPoint = polyPoint.ReverseOrder();
//        }
//        polyPointArray.Add(polyPoint);
//    }
//    newContours.clear();
//    newContours = polyPointArray;
//
//
//    for(i=0; i<num; i++)
//    {
//        indexArray.clear();
//        polyPoint.clear();
//        tempPolyPoint.clear();
//        pointArray.clear();
//        newPolyPoint.clear();
//
//        polyPoint = refContours.at(i);
//        tempPolyPoint = newContours.at(i);
//        num1 = polyPoint.size();
//        num2 = tempPolyPoint.size();
//        index1 = 0;
//
//        //找到原轮廓线上的每个点到新轮廓线上的最近点;
//        for (j=0; j<num1; j++)
//        {
//            p1 = polyPoint.at(j);
//            for (k=0; k<num2; k++)
//            {
//                p2 = tempPolyPoint.at(k);
//                if (k==0)
//                {
//                    minDis = dis = Relation3D::PToPDistance(p1, p2);
//                    index1 = k;
//                }
//                else
//                {
//                    dis = Relation3D::PToPDistance(p1, p2);
//                    if (minDis > dis)
//                    {
//                        minDis = dis;
//                        nearestPoint = p2;
//                        index1 = k;
//                    }
//                }
//            }
//            if (j==0)
//            {
//                index2 = index1;
//            }
//            pointArray.Add(tempPolyPoint.at(index1));
//        }
//
//        //get a new order points;
//        for (j=index2; j<num2; j++)
//        {
//            newPolyPoint.Add(tempPolyPoint.at(j));
//        }
//        for (j=1; j<index2; j++)
//        {
//            newPolyPoint.Add(tempPolyPoint.at(j));
//        }
//        newPolyPoint.Add(tempPolyPoint.at(index2));
//
//        index1 = 0;
//        bool isMatch;
//        for (j=0; j<num1; j++)
//        {
//            isMatch  = false;
//            point = pointArray.at((j)%num1);
//            for (k=index1; k<newPolyPoint.size(); k++)
//            {
//                if (point == newPolyPoint.at(k))
//                {
//                    isMatch = true;
//                    indexArray.Add(k);
//                    index1 = k;
//                    break;
//                }
//            }
//            if (!isMatch)
//            {
//                indexArray.Add(index1);
//            }
//        }
//
//        /*afxDump<<"修改前：\n";
//        for (k=0; k<indexArray.size(); k++)
//        {
//            afxDump<<k<<" "<<indexArray.at(k)<<" \n";
//        }*/
//
//        int *ptrIndex1, *ptrIndex2;
//        for (j=0; j<indexArray.size()-2; j++)
//        {
//            ptrIndex2 = &indexArray.at(j+1);
//            ptrIndex1 = &indexArray.at(j);
//            if (*ptrIndex2 < *ptrIndex1)
//            {
//                *ptrIndex1 = *ptrIndex2;
//            }
//        }
//        for (j=0; j<num1-1; j++)
//        {
//            p1 = polyPoint.at(j);
//            index1 = indexArray.at(j);
//            index2 = indexArray.at((j+1)%num1);
//            p2 = newPolyPoint.at(index1);
//            //process the last surface;
//            if (j == num1 - 2)
//            {
//                index2 = newPolyPoint.size();
//                ptrIndex1 = &indexArray.at(indexArray.size()-1);
//                *ptrIndex1 = index2;
//            }
//            //if the value is equals to 1,then create two triangles;
//            if (index2 - index1== 1)
//            {
//                Point3D tp;
//                tp = newPolyPoint.at(index2%newPolyPoint.size());
//                objTriangle.Set(newPolyPoint.at(index2%newPolyPoint.size()),p2, p1);
//                surface.Add(objTriangle);
//                objTriangle.Set( polyPoint.at((j+1)%num1),newPolyPoint.at(index2%newPolyPoint.size()), p1);
//                surface.Add(objTriangle);
//            }
//                //if the value is equals to 0, then create one triangle;
//            else if (index2-index1 == 0)
//            {
//                objTriangle.Set(polyPoint.at((j+1)%num1), p2, p1);
//                surface.Add(objTriangle);
//            }
//            else
//            {
//                int flag = 0;
//                index = 0;
//                for (k=index1; k<index2; k++)
//                {
//                    if ((k-index1) >= ((index2 - index1)/2))
//                    {
//                        if (!flag){
//                            index = k;
//                        }
//                        flag = 1;
//                        p1 = polyPoint.at((j+1)%num1);
//                        p2 = newPolyPoint.at(k);
//                        p3 = newPolyPoint.at((k+1)%newPolyPoint.size());
//                        objTriangle.Set(p3, p2, p1);
//                        surface.Add(objTriangle);
//                    }
//                    else
//                    {
//                        p1 = polyPoint.at(j);
//                        p2 = newPolyPoint.at(k);
//                        p3 = newPolyPoint.at((k+1)%newPolyPoint.size());
//                        objTriangle.Set(p3, p2, p1);
//                        surface.Add(objTriangle);
//                    }
//                }
//                if (index != 0)
//                {
//                    p1 = polyPoint.at(j);
//                    p2 = newPolyPoint.at(index-1);
//                    p3 = newPolyPoint.at(index);
//                    objTriangle.Set(p3, p2, p1);
//                    surface.Add(objTriangle);
//
//                    p2 = polyPoint.at((j+1)%num1);
//                    objTriangle.Set(p1, p2, p3);
//                    surface.Add(objTriangle);
//                }
//            }
//        }
//
//    }
//    //用李晓勇的模拟遗传退火算法实现表面重建
//    //for (i=0; i<num; i++)
//    //{
//    //    GeneticSimA genSimA;
//    //	genSimA.SetCloseFlag(true);
//    //	//genSimA.SetContourNormals(m_n);
//    //	genSimA.SetContour(newContours.at (i), refContours.at (i));
//    //	genSimA.CreateSurface ();
//    //	num1 = genSimA.m_TriangleArray.size();
//    //	for(int j=0; j<num1; j++)
//    //	{
//    //		objTriangle = genSimA.m_TriangleArray[j];
//    //		surface.Add(objTriangle);
//    //	} //
//    //}
//    surface.m_attr1 =attr1;
//    surface.m_attr2 =attr2 ;
//    surface.m_strID =strID ;
//    surface.m_strSubID =strSubID ;
//    surface.GetSimpleNormals (surfaceNormal);
//    return;
//}

//void MarchingCube::ConfirmCorresponseContours(PolyPoint3DArrayEx &newContours, PolyPoint3DArrayEx &refContours, double n[])
//{
//    Point3D p[3], p1, p2;
//    Polygon3D polygon1, polygon2;
//    Point3DArray pointArray;
//    PolyPoint3DArrayEx tempPolyPoint;
//    int i, j, num1;
//    PolyPoint3D polyPoint;
//    Triangle3D tri;
//    Polygon3D newPolygon, refPolygon;
//    Vector3D v1, v2;
//    Line3DArrayEx lineArray;
//    Line3D line;
//    Triangle3DArrayEx delaunayTriangleArray;
//    Vector3D vx(1, 0, 0), vy(0, 1, 0), vz(0, 0, 1), v, u;
//    int flag;
//    double minValue, value;
//    int index;
//    double m_n[3];
//    Point3DArrayEx convexHullPoints;
//    Point3DArrayEx upCenterOfGravity, downCenterOfGravity;
//    Point3D gravityPoint;
//    int num = refContours.size();
//    v.Set(n);
//    //if there is only one contour to construction,it's not necessary to corresponse;
//    //the size of newContours and refContours must be the same, select one type contours arbitrary
//    if (num < 2)
//    {
//        return;
//    }
//        //else the contours must to be corresponse;
//        //the mothed: create a point in newContours,then project it to refContours,if the point is in the refContours
//        //so the contours is corresponse,otherwise they are not;
//    else
//    {
//        for (i=0; i<num; i++)
//        {
//            convexHullPoints.clear();
//            gravityPoint.Set(0, 0, 0);
//            polyPoint = refContours.at(i);
//            polyPoint.GetPlanarConvexHull(convexHullPoints, m_n);
//
//            num1 = convexHullPoints.size();
//            for (j=0; j<num1-1; j++)
//            {
//                p1 = convexHullPoints.at(j);
//                gravityPoint.m_x += p1.m_x;
//                gravityPoint.m_y += p1.m_y;
//                gravityPoint.m_z += p1.m_z;
//            }
//            gravityPoint.m_x /= (num1-1);
//            gravityPoint.m_y /= (num1-1);
//            gravityPoint.m_z /= (num1-1);
//            upCenterOfGravity.Add(gravityPoint);
//
//            convexHullPoints.clear();
//            polyPoint = newContours.at(i);
//            polyPoint.GetPlanarConvexHull(convexHullPoints, m_n);
//
//            num1 = convexHullPoints.size();
//            gravityPoint.Set(0, 0, 0);
//            for (j=0; j<num1; j++)
//            {
//                p1 = convexHullPoints.at(j);
//                gravityPoint.m_x += p1.m_x;
//                gravityPoint.m_y += p1.m_y;
//                gravityPoint.m_z += p1.m_z;
//            }
//            gravityPoint.m_x /= (num1-1);
//            gravityPoint.m_y /= (num1-1);
//            gravityPoint.m_z /= (num1-1);
//            downCenterOfGravity.Add(gravityPoint);
//        }
//
//        tempPolyPoint.clear();
//        num1 = downCenterOfGravity.size();
//        for (i=0; i<num1; i++)
//        {
//            p1 = upCenterOfGravity.at(i);
//            for (j=0; j<num1; j++)
//            {
//                p2 = downCenterOfGravity.at(j);
//                if (j==0)
//                {
//                    minValue = value = Relation3D::PToPDistance(p1, p2);
//                    index = j;
//                }
//                else
//                {
//                    value = Relation3D::PToPDistance(p1, p2);
//                    if (minValue > value)
//                    {
//                        minValue = value;
//                        index = j;
//                    }
//                }
//            }
//            tempPolyPoint.Add(newContours.at(index));
//
//        }
//        newContours.clear();
//        newContours = tempPolyPoint;
//    }
//}

void MarchingCube::TracePointsToContours(Line3DArrayEx lineArray, PolyPoint3DArrayEx &contours)
{
    int i, j, num, num1;
    Point3DArrayEx tempContourPoints;
    Point3D p1, p2;
    Line3D line;
    Point3DArrayEx curPointArray, nextPointArray, tempPointArray;
    Point3D curPoint, nextPoint, p[2];
    Line3D line1;

    num = lineArray.size();
    if (num < 1)
    {
//        AfxOutputDebugString("error: contour trace failure!!\n");
        return;
    }
    for (int j=0; j<lineArray.size(); j++)
    {
        curPointArray.clear();
        line = lineArray.at(j);
        line.Get(p1, p2);

        tempPointArray.Add(p1);
        tempPointArray.Add(p2);

        lineArray.RemoveAt(j);

        /*从正方向进行线段拼接*/
        curPoint = tempPointArray.at(1);
        for(int k=0; k<lineArray.size(); k++)
        {
            line1 = lineArray.at(k);
            line1.Get(p[0], p[1]);
            for (int n=0; n<2; n++)
            {
                nextPoint = p[n];

                /*如果候选线段组中的线段满足与当前线段连接，则添加这条连接线段到线段组中去，并删除那条线段*/
                if (curPoint == nextPoint )
                {
                    nextPoint = p[(n+1)%2];
                    curPoint = nextPoint;
                    tempPointArray.Add(nextPoint);

                    lineArray.RemoveAt(k);
                    k=-1;
                    break;
                }
            }
        }


        /*如果正方向没拼接完，则进行逆方向的拼接*/
        for(int k=0; k<lineArray.size(); k++)
        {
            line = lineArray.at(k);
            line.Get(p[0], p[1]);
            //nextPointArray = curTriInsectPointsArray.at(k);
            curPointArray.clear();
            curPointArray.Add(tempPointArray.at(1));
            curPointArray.Add(tempPointArray.at(0));

            for (int n=0; n<2; n++)
            {
                curPoint = curPointArray.at(1);
                nextPoint = p[n];

                /*如果候选线段组中的线段满足与当前线段连接，则添加这条连接线段到线段组中去，并删除那条线段*/
                if (curPoint == nextPoint)
                {
                    nextPoint = p[(n+1)%2];
                    tempPointArray.InsertAt(0,nextPoint);
                    lineArray.RemoveAt(k);
                    k=-1;
                    break;
                }
            }
        }

        /*添加拼接的线段组到线段组组中*/
        contours.Add(tempPointArray);
        tempPointArray.clear();
        j=-1;
    }

}

void MarchingCube::SaveTriangleToSurface(Triangle3DArrayEx& tempSurface, Point3DArrayEx& surfaceNormals)
{
    /*tempSurface.clear();
    surfaceNormals.clear();*/
    std::list<TriangleEx>::iterator i;
    for (i = m_TriangleEx.begin(); i != m_TriangleEx.end(); i++)
    {
        Triangle3D tempTriangle((*i).point[0], (*i).point[1], (*i).point[2]);
        Point3D tempTriangleNormals((*i).normal);

        tempSurface.Add(tempTriangle);
        surfaceNormals.Add(tempTriangleNormals);
    }

}
std::list<TriangleEx>& MarchingCube::GetTriangles(void) {
    return m_TriangleEx;
}

int MarchingCube::GenerateSurface(/*const*/ Cube* cube, float isoLevel, Line3DArrayEx &fLineArray, Line3DArrayEx &bLineArray) {
    int cubeIndex = 0;
//    cout << "surface begin" << endl;
    Line3D line;
    int index1, index2, index3;
    //cube->hasFacet = true;
    if (cube->point[0]->val <= isoLevel) cubeIndex |= 1;
    if (cube->point[1]->val <= isoLevel) cubeIndex |= 2;
    if (cube->point[2]->val <= isoLevel) cubeIndex |= 4;
    if (cube->point[3]->val <= isoLevel) cubeIndex |= 8;
    if (cube->point[4]->val <= isoLevel) cubeIndex |= 16;
    if (cube->point[5]->val <= isoLevel) cubeIndex |= 32;
    if (cube->point[6]->val <= isoLevel) cubeIndex |= 64;
    if (cube->point[7]->val <= isoLevel) cubeIndex |= 128;

    if (EdgeTable[cubeIndex] == 0) {
        return 0;
    }
    Point3D vertices[12];

    //if there is intersect point, calculate it and then add it to the array of points. the same as the follows
    if (EdgeTable[cubeIndex] & 1) {
        vertices[0] = Interpolate(isoLevel, cube->point[0], cube->point[1]);
    }
    if (EdgeTable[cubeIndex] & 2) {
        vertices[1] = Interpolate(isoLevel, cube->point[1], cube->point[2]);
    }
    if (EdgeTable[cubeIndex] & 4) {
        vertices[2] = Interpolate(isoLevel, cube->point[2], cube->point[3]);
    }
    if (EdgeTable[cubeIndex] & 8) {
        vertices[3] = Interpolate(isoLevel, cube->point[3], cube->point[0]);
    }
    if (EdgeTable[cubeIndex] & 16) {
        vertices[4] = Interpolate(isoLevel, cube->point[4], cube->point[5]);
    }
    if (EdgeTable[cubeIndex] & 32) {
        vertices[5] = Interpolate(isoLevel, cube->point[5], cube->point[6]);
    }
    if (EdgeTable[cubeIndex] & 64) {
        vertices[6] = Interpolate(isoLevel, cube->point[6], cube->point[7]);
    }
    if (EdgeTable[cubeIndex] & 128) {
        vertices[7] = Interpolate(isoLevel, cube->point[7], cube->point[4]);
    }
    if (EdgeTable[cubeIndex] & 256) {
        vertices[8] = Interpolate(isoLevel, cube->point[0], cube->point[4]);
    }
    if (EdgeTable[cubeIndex] & 512) {
        vertices[9] = Interpolate(isoLevel, cube->point[1], cube->point[5]);
    }
    if (EdgeTable[cubeIndex] & 1024) {
        vertices[10] = Interpolate(isoLevel, cube->point[2], cube->point[6]);
    }
    if (EdgeTable[cubeIndex] & 2048) {
        vertices[11] = Interpolate(isoLevel, cube->point[3], cube->point[7]);
    }

    int triCount = 0;
    for (int i = 0; TriTable[cubeIndex][i] != -1; i+=3) {
        TriangleEx tri;
        tri.point[0] = vertices[TriTable[cubeIndex][i  ]];
        tri.point[1] = vertices[TriTable[cubeIndex][i+1]];
        tri.point[2] = vertices[TriTable[cubeIndex][i+2]];
//        CalculateNormal(tri);
//        tri.Debug();

        AddTriangle add;
        add.a.x = tri.point[0].m_x/mcScale;
        add.a.y = tri.point[0].m_y/mcScale;
        add.a.z = tri.point[0].m_z/mcScale;
        add.b.x = tri.point[1].m_x/mcScale;
        add.b.y = tri.point[1].m_y/mcScale;
        add.b.z = tri.point[1].m_z/mcScale;
        add.c.x = tri.point[2].m_x/mcScale;
        add.c.y = tri.point[2].m_y/mcScale;
        add.c.z = tri.point[2].m_z/mcScale;

        McTri.push_back(add);
//        m_TriangleEx.push_back(tri);

        triCount++;

        index1 = TriTable[cubeIndex][i];
        index2 = TriTable[cubeIndex][i+1];
        index3 = TriTable[cubeIndex][i+2];

        //get the insect line on the panlne of 0123;
        if (index1 <= 3 && index2 <= 3)
        {
            line.Set(tri.point[0], tri.point[1]);
            fLineArray.Add(line);
            continue;
        }
        else if (index1 <=3 && index3 <= 3)
        {
            line.Set(tri.point[0], tri.point[2]);
            fLineArray.Add(line);
            continue;
        }
        else if(index2 <= 3 && index3 <= 3)
        {
            line.Set(tri.point[1], tri.point[2]);
            fLineArray.Add(line);
            continue;
        }

        //get the insect line on the panlne of 4567;
        if ((index1 <= 7 && index2 <= 7) && (index1 > 3 && index2 > 3) )
        {
            line.Set(tri.point[0], tri.point[1]);
            bLineArray.Add(line);
            continue;
        }
        else if ((index1 <= 7 && index3 <= 7) && (index1 > 3 && index3 > 3))
        {
            line.Set(tri.point[0], tri.point[2]);
            bLineArray.Add(line);
            continue;
        }
        else if ((index2 <= 7 && index3 <= 7) && (index2> 3 && index3 > 3))
        {
            line.Set(tri.point[1], tri.point[2]);
            bLineArray.Add(line);
            continue;
        }

    }

    cube->hasFacet = true;
    return triCount;
}

Point3D MarchingCube::Interpolate(float isoLevel, const GridPoint* gridPoint1, const GridPoint* gridPoint2) {
    float mu;
    Point3D p;
    mu = (isoLevel - gridPoint1->val) / (gridPoint2->val - gridPoint1->val);
    p.m_x = gridPoint1->point.m_x + mu * (gridPoint2->point.m_x - gridPoint1->point.m_x);
    p.m_y = gridPoint1->point.m_y + mu * (gridPoint2->point.m_y - gridPoint1->point.m_y);
    p.m_z = gridPoint1->point.m_z + mu * (gridPoint2->point.m_z - gridPoint1->point.m_z);
    return p;
}

void MarchingCube::AllocateMerroy(void) {
    if (m_GridPoints || m_Cubes) {
        DeleteMerroy();
    }
    m_GridPoints = new GridPoint**[m_sizeI + 1];
    for (int i = 0; i < m_sizeI + 1; i++) {
        m_GridPoints[i] = new GridPoint*[m_sizeJ + 1];
        for (int j = 0; j < m_sizeJ + 1; j++) {
            m_GridPoints[i][j] = new GridPoint[m_sizeK + 1];
        }
    }
    m_Cubes  = new Cube**[m_sizeI];
    for (int i = 0; i < m_sizeI; i++) {
        m_Cubes[i] = new Cube*[m_sizeJ];
        for (int j = 0; j < m_sizeJ; j++) {
            m_Cubes[i][j] = new Cube[m_sizeK];
        }
    }
}

void MarchingCube::DeleteMerroy(void) {
    if (m_GridPoints) {
        for (int i = 0; i < m_sizeI + 1; i++) {
            for (int j = 0; j < m_sizeJ + 1; j++) {
                delete [] m_GridPoints[i][j];
            }
            delete [] m_GridPoints[i];
        }
        delete [] m_GridPoints;
        m_GridPoints = NULL;
    }
    if (m_Cubes) {
        for (int i = 0; i < m_sizeI; i++) {
            for (int j = 0; j < m_sizeJ; j++) {
                delete [] m_Cubes[i][j];
            }
            delete [] m_Cubes[i];
        }
        delete [] m_Cubes;
        m_Cubes = NULL;
    }
}

void MarchingCube::ConnectGrids(void) {
    if (!m_GridPoints || !m_Cubes) {
        return;
    }
    for (int i = 0; i < m_sizeI; i++) {
        for (int j = 0; j < m_sizeJ; j++) {
            for (int k = 0; k < m_sizeK; k++) {
                Cube* gridCell = &m_Cubes[i][j][k];
                gridCell->point[0] = &m_GridPoints[i  ][j  ][k  ];
                gridCell->point[1] = &m_GridPoints[i  ][j  ][k+1];
                gridCell->point[2] = &m_GridPoints[i  ][j+1][k+1];
                gridCell->point[3] = &m_GridPoints[i  ][j+1][k  ];
                gridCell->point[4] = &m_GridPoints[i+1][j  ][k  ];
                gridCell->point[5] = &m_GridPoints[i+1][j  ][k+1];
                gridCell->point[6] = &m_GridPoints[i+1][j+1][k+1];
                gridCell->point[7] = &m_GridPoints[i+1][j+1][k  ];

            }
        }
    }
}

void MarchingCube::CalculateNormal(TriangleEx& tri) {
    float a[3], b[3];
    float length;
    a[0] = tri.point[0].m_x - tri.point[1].m_x;
    a[1] = tri.point[0].m_y - tri.point[1].m_y;
    a[2] = tri.point[0].m_z - tri.point[1].m_z;
    b[0] = tri.point[0].m_x - tri.point[2].m_x;
    b[1] = tri.point[0].m_y - tri.point[2].m_y;
    b[2] = tri.point[0].m_z - tri.point[2].m_z;
    tri.normal.m_x = a[1] * b[2] - b[1] * a[2];
    tri.normal.m_y = b[0] * a[2] - a[0] * b[2];
    tri.normal.m_z = a[0] * b[1] - b[0] * a[1];
    length = sqrt(tri.normal.m_x*tri.normal.m_x + tri.normal.m_y
                                                  *tri.normal.m_y + tri.normal.m_z*tri.normal.m_z);
    tri.normal.m_x /= length;
    tri.normal.m_y /= length;
    tri.normal.m_z /= length;
}





void MarchingCube::CreateGrid(Point3D *point, PolyPoint3DArrayEx FrontPolyPointArray, PolyPoint3DArrayEx BackPolyPointArray, PolyPoint3DArrayEx& GridPointData)
{
    //测试计算的时间变量;
    clock_t      start, finish;

    /*前曲线组的原始点*/
    PolyPoint3DArrayEx fOriginPolyPointArray;

    /*后曲线组的原始点*/
    PolyPoint3DArrayEx bOriginPolyPointArray;

    Point3DArrayEx tempPolyPoint;

    //两点间的距离值;
    double        Distance1, Distance2;

    double n1[3], n2[3];

    //耗费的时间,仅供测试;
    double       duration;
    int          IsInPolygon1, IsInPolygon2, num1, num2, MaxFrontNum, MaxBackNum, numFrontPolygonArray,numBackPolygonArray;

    //前剖面曲线FPolyPoint,和后剖面曲线BPolyPoint;
    Point3DArrayEx  FPolyPoint, BPolyPoint;
    Point3D      *TempPoint1, *TempPoint2, p;

    Polygon3D    fBox, bBox;

    //计算网格时所用到的一些中间点;
    Point3D      CurPoint0, CurPoint1, CurPoint2, CurPoint3, SecPoint0, SecPoint1;

    //前剖面多边形FrontPolygon,多边形组frontPolygonArray和后剖面多边形BackPolygon,多边形组backPolygonArray;
    Polygon3D    FrontPolygon, BackPolygon, tempPolygon;
    Polygon3DArrayEx frontPolygonArray, backPolygonArray, fOrginalPolygonArray, bOrginalPolygonArray;

    MaxFrontNum = FrontPolyPointArray.size();
    MaxBackNum  = BackPolyPointArray.size();

    frontPolygonArray.clear();
    backPolygonArray.clear();

    int num = 0;
    string str;

    //将前剖面的曲线组转化为多边形组;
    for (num1 = 0; num1 < MaxFrontNum; num1 ++)
    {
        FPolyPoint = FrontPolyPointArray.at(num1);



        /*将原来的点抽取出来*/
        for (int i=0; i<FPolyPoint.size(); i++)
        {
            CurPoint0 = FPolyPoint.at(i);


            if (CurPoint0.m_ID != -1)
            {
                tempPolyPoint.Add(CurPoint0);
            }
        }
        /*如果没有点存在，则说明没有进行插值，直接将现有的曲线进行赋值*/
        if (tempPolyPoint.size() != 0)
        {
            FrontPolygon.clear();
            Polygon3D FrontPolygon(tempPolyPoint);
            fOrginalPolygonArray.Add(FrontPolygon);

            fOriginPolyPointArray.Add(tempPolyPoint);
            tempPolyPoint.clear();
        }
        else
        {
            FrontPolygon.clear();
            Polygon3D FrontPolygon(FPolyPoint);
            fOrginalPolygonArray.Add(FrontPolygon);

            fOriginPolyPointArray.Add(FPolyPoint);
        }

        FrontPolygon.clear();
        Polygon3D FrontPolygon(FPolyPoint);
        frontPolygonArray.Add(FrontPolygon);
    }

    //将后剖面的曲线组转化为多边形组;
    for (num2 = 0; num2 < MaxBackNum; num2 ++)
    {
        BPolyPoint = BackPolyPointArray.at(num2);



        /*将原来的点抽取出来*/
        for (int i=0; i<BPolyPoint.size(); i++)
        {
            CurPoint0 = BPolyPoint.at(i);

            if (CurPoint0.m_ID != -1)
            {
                tempPolyPoint.Add(CurPoint0);
            }
        }
        /*如果没有点存在，则说明没有进行插值，直接将现有的曲线进行赋值*/
        if (tempPolyPoint.size() != 0)
        {
            BackPolygon.clear();
            Polygon3D BackPolygon(tempPolyPoint);
            bOrginalPolygonArray.Add(BackPolygon);

            bOriginPolyPointArray.Add(tempPolyPoint);
            tempPolyPoint.clear();
        }
        else
        {
            BackPolygon.clear();
            Polygon3D BackPolygon(BPolyPoint);
            bOrginalPolygonArray.Add(BackPolygon);

            bOriginPolyPointArray.Add(BPolyPoint);
        }

        Polygon3D BackPolygon(BPolyPoint);

        backPolygonArray.Add(BackPolygon);
    }

    //保存前剖面的四个角点;
    m_secPoint[0] = CurPoint0 = point[0];
    m_secPoint[1] = CurPoint1 = point[1];
    m_secPoint[2] = CurPoint2 = point[2];
    m_secPoint[3] = point[3];
    m_secPoint[4] = point[4];
    CurPoint3 = point[3];

    double extraValue=0.0;
    double wideth = Relation3D::PToPDistance(m_secPoint[0], m_secPoint[1]);
    double height = Relation3D::PToPDistance(m_secPoint[1], m_secPoint[2]);
    double maxValue = max(wideth, height);
    int minNumber = min(m_sizeJ, m_sizeK);
    extraValue = maxValue / minNumber;

    ComputeBox(fOriginPolyPointArray, bOriginPolyPointArray, point, extraValue);


    /*Point3DArrayEx f, b;
    Point3D fSecPoint[4], bSecPoint[4];
    for (int i=0; i<4; i++)
    {
        fSecPoint[i] = point[i];
        bSecPoint[i] = point[i+4];
    }

    CreateBox(fOriginPolyPointArray, fSecPoint, f, 0);
    CreateBox(bOriginPolyPointArray, bSecPoint, b, 0);

    for (int i=0; i<4; i++)
    {
        m_box[i] = f.at(i);
        m_box[i+4] = b.at(i);
    }

    for (int i=0; i<4; i++)
    {
        fBox.Add(m_box[i]);
        bBox.Add(m_box[i+4]);
    }

    fBox.GetNormal(n1);
    bBox.GetNormal(n2);

    for (int i=0; i<3; i++)
    {
        n1[i] = -n1[i];
        n2[i] = -n2[i];
    }*/

    //点计数器,供测试用;
    static int Count = 0;

    Polygon3D *ptrPolygon = NULL;

    //开启时间计数;
    start = clock();

    //计算前后两个剖面上的网格,前后对应的四个网格点组成一个立方体;
    for (int i = 0; i < m_sizeI + 1; i = i + m_sizeI)
    {
//        str.Format("%d", i);
//        AfxOutputDebugString(str);
        CurPoint0.m_x = (double)((point[4].m_x - point[0].m_x)/m_sizeI) * i + point[0].m_x;
        CurPoint0.m_y = (double)((point[4].m_y - point[0].m_y)/m_sizeI) * i + point[0].m_y;
        CurPoint0.m_z = (double)((point[4].m_z - point[0].m_z)/m_sizeI) * i + point[0].m_z;

        CurPoint1.m_x = (double)((point[5].m_x - point[1].m_x)/m_sizeI) * i + point[1].m_x;
        CurPoint1.m_y = (double)((point[5].m_y - point[1].m_y)/m_sizeI) * i + point[1].m_y;
        CurPoint1.m_z = (double)((point[5].m_z - point[1].m_z)/m_sizeI) * i + point[1].m_z;

        CurPoint2.m_x = (double)((point[6].m_x - point[2].m_x)/m_sizeI) * i + point[2].m_x;
        CurPoint2.m_y = (double)((point[6].m_y - point[2].m_y)/m_sizeI) * i + point[2].m_y;
        CurPoint2.m_z = (double)((point[6].m_z - point[2].m_z)/m_sizeI) * i + point[2].m_z;

        CurPoint3.m_x = (double)((point[7].m_x - point[3].m_x)/m_sizeI) * i + point[3].m_x;
        CurPoint3.m_y = (double)((point[7].m_y - point[3].m_y)/m_sizeI) * i + point[3].m_y;
        CurPoint3.m_z = (double)((point[7].m_z - point[3].m_z)/m_sizeI) * i + point[3].m_z;

        SecPoint0 = CurPoint0;
        SecPoint1 = CurPoint1;

        for (int j = 0; j < m_sizeJ + 1; j++)
        {
            CurPoint0.m_x = (double)((CurPoint3.m_x - SecPoint0.m_x)/m_sizeJ) * j + SecPoint0.m_x;
            CurPoint0.m_y = (double)((CurPoint3.m_y - SecPoint0.m_y)/m_sizeJ) * j + SecPoint0.m_y;
            CurPoint0.m_z = (double)((CurPoint3.m_z - SecPoint0.m_z)/m_sizeJ) * j + SecPoint0.m_z;

            CurPoint1.m_x = (double)((CurPoint2.m_x - SecPoint1.m_x)/m_sizeJ) * j + SecPoint1.m_x;
            CurPoint1.m_y = (double)((CurPoint2.m_y - SecPoint1.m_y)/m_sizeJ) * j + SecPoint1.m_y;
            CurPoint1.m_z = (double)((CurPoint2.m_z - SecPoint1.m_z)/m_sizeJ) * j + SecPoint1.m_z;

            for (int k = 0; k < m_sizeK + 1; k++)
            {
                GridPoint* gridPoint = &m_GridPoints[i][j][k];

                //获得当前立方体顶点的坐标值;
                gridPoint->point.m_x = (double)((CurPoint1.m_x - CurPoint0.m_x)/m_sizeK) * k + CurPoint0.m_x;
                gridPoint->point.m_y = (double)((CurPoint1.m_y - CurPoint0.m_y)/m_sizeK) * k + CurPoint0.m_y;
                gridPoint->point.m_z = (double)((CurPoint1.m_z - CurPoint0.m_z)/m_sizeK) * k + CurPoint0.m_z;

                TempPoint1 = &gridPoint->point;

                //当立方体顶点在前剖面时;
                if (i == 0)
                {
                    cout << "mc test" << endl;

                    if(!InBox(*TempPoint1, fBox, n1))
                    {
                        continue;
                    }
                    num ++;
                    //对当前点依次与前剖面上的多边形组进行距离计算;
                    numFrontPolygonArray = fOriginPolyPointArray.size();
                    for (int index = 0; index < numFrontPolygonArray; index++)
                    {
                        /*用未插点的曲线做未多边形，进行网格点的内外判断*/
                        ptrPolygon = &fOrginalPolygonArray.at(index);

                        //判断点顶点是否在多边形内,IsInPolygon1为1表示在多边形边上,为2表示在多边形的内部,否则在多边形的外面;
                        IsInPolygon1 = ptrPolygon->Contain(*TempPoint1);

                        /*用插点的曲线去求网格点的权值*/
                        ptrPolygon = &frontPolygonArray.at(index);

                        //在多边形上，直接顶点权值赋值为0;
                        if(IsInPolygon1 == 1)
                        {
                            //主要是给顶点赋值哦
                            gridPoint->val = 0;
                        }
                            //在多边形内部，则计算顶点到多边形上点的最短距离;
                        else if (IsInPolygon1 == 2)
                        {
                            //计算点TempPoint到多边形上点的最短距离
                            //Distance1 = Distance2 = Relation3D::PToPDistance(ptrPolygon->at(0), *TempPoint1);
                            Distance1 = Distance2 = CalcDistance(ptrPolygon->at(0), *TempPoint1);
                            for (int index = 1; index < ptrPolygon->size(); index ++)
                            {
                                TempPoint2 = &ptrPolygon->at(index);
                                //Distance1 = Relation3D::PToPDistance(*TempPoint2, *TempPoint1);
                                Distance1 = CalcDistance(*TempPoint2, *TempPoint1);
                                Distance2 = (Distance1 > Distance2)? Distance2: Distance1;
                            }
                            gridPoint->val = Distance2;

                        }
                            //在多边形外面，计算顶点多变形上点的最短距离，并取距离的负值；
                        else
                        {
                            //Distance1 = Distance2 = Relation3D::PToPDistance(ptrPolygon->at(0), *TempPoint1);
                            Distance1 = Distance2 = CalcDistance(ptrPolygon->at(0), *TempPoint1);
                            for (int index = 1; index < ptrPolygon->size(); index ++)
                            {
                                TempPoint2 = &ptrPolygon->at(index);
                                //Distance1 = Relation3D::PToPDistance(*TempPoint2, *TempPoint1);
                                Distance1 = CalcDistance(*TempPoint2, *TempPoint1);
                                Distance2 = (Distance1 > Distance2)? Distance2: Distance1;
                            }

                            //与上次计算的权值比较，若绝对值大于当前值，则更改其权值；
                            if (abs(gridPoint->val) >= Distance2) {
                                gridPoint->val = Distance2;
                                gridPoint->val = - gridPoint->val;
                            }
                                //否则，继续下一个顶点的计算;
                            else
                                continue;
                        }
                    }
                }
                    //当立方体顶点在后剖面时;
                else
                {
                    if(!InBox(*TempPoint1, bBox, n2))
                    {
                        continue;
                    }
                    num ++;
                    numBackPolygonArray = bOriginPolyPointArray.size();
                    //numBackPolygonArray = backPolygonArray.size();
                    for (int index = 0; index < numBackPolygonArray; index ++)
                    {
                        /*用未插点的曲线做未多边形，进行网格点的内外判断*/
                        ptrPolygon = &bOrginalPolygonArray.at(index);
                        IsInPolygon2 = ptrPolygon->Contain(*TempPoint1);

                        /*用插点的曲线去求网格点的权值*/
                        ptrPolygon = &backPolygonArray.at(index);

                        if (IsInPolygon2 == 1)
                        {
                            gridPoint->val = 0;
                        }
                        else if (IsInPolygon2 == 2)
                        {
                            //Distance1 = Distance2 = Relation3D::PToPDistance(ptrPolygon->at(0), *TempPoint1);
                            Distance1 = Distance2 = CalcDistance(ptrPolygon->at(0), *TempPoint1);
                            for (int index = 1; index < ptrPolygon->size(); index ++)
                            {
                                TempPoint2 = &ptrPolygon->at(index);
                                //Distance1 = Relation3D::PToPDistance(*TempPoint2, *TempPoint1);
                                Distance1 = CalcDistance(*TempPoint2, *TempPoint1);
                                Distance2 = (Distance1 > Distance2)? Distance2:Distance1;
                            }
                            gridPoint->val = Distance2;
                        }
                        else
                        {
                            Distance1 = Distance2 = CalcDistance(ptrPolygon->at(0), *TempPoint1);
                            //Distance1 = Distance2 = Relation3D::PToPDistance(ptrPolygon->at(0), *TempPoint1);
                            for (int index = 1; index < ptrPolygon->size(); index ++)
                            {
                                TempPoint2 = &ptrPolygon->at(index);
                                //Distance1 = Relation3D::PToPDistance(*TempPoint2, *TempPoint1);
                                Distance1 = CalcDistance(*TempPoint2, *TempPoint1);
                                Distance2 = (Distance1 > Distance2)? Distance2:Distance1;
                            }
                            if (abs(gridPoint->val) >= Distance2) {
                                gridPoint->val = Distance2;
                                gridPoint->val = - gridPoint->val;
                            }
                            else
                                continue;

                        }
                    }
                }

            }
        }
    }
    ConnectGrids();
    CreateSurface();
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;

//    str.Format("所用时间%f\n", duration);
//    AfxOutputDebugString(str);
    SaveGridPoint(GridPointData);
}

//建立包围盒还是有问题，算出凸包后就能求出包围盒啊？但是这里写的怎么这么复杂。
void MarchingCube::CreateBox(PolyPoint3DArrayEx polyPointArray, Point3D *secPoint, Point3DArrayEx &box,double extraValue)
{
//    cout << "begin box 1" << endl;
    Point3DArrayEx convexHullPoints, tempPointArray, newPointArray;
    int i, j, num1, num2;
    double n[3];
    Point3D point, nearestPoint, fastestPoint, projectPoint, projectPoint1;
    double dis, minDis, maxDis, maxLength;
    Vector3D v1, v2;
    Point3D boxArray[4], p[2];
    Point3DArrayEx projectPointSet;

    //将轮廓线组的坐标点都存储到一个点数组中
    num1 = polyPointArray.size();
//    cout << "numbers of poly num1 " << num1 << endl;
    for (i=0; i<num1; i++)
    {
        tempPointArray = polyPointArray.at(i);
        num2 = tempPointArray.size();
        for (j=0; j<num2; j++)
        {
            newPointArray.Add(tempPointArray.at(j));
        }
        tempPointArray.clear();
    }

//    cout << "numbers of new Point : " << newPointArray.size() << endl;
    if (newPointArray.GetPlanarConvexHull(convexHullPoints, n) == 0)
    {
        cout << "hull failed 点集凸壳计算不成功！" << endl;
//        AfxOutputDebugString("点集凸壳计算不成功！");
        return;
    }
//    cout << " convexHull" << endl;
//    for(int i = 0; i < convexHullPoints.size();i++){
//        convexHullPoints.at(i).Debug();
//    }


//    cout << "convex of 凸壳:";
//    for(int i = 0;i < convexHullPoints.size();i++){
//        convexHullPoints.at(i).Debug();
//    }

    //计算凸壳上的点到剖面边框边secPoint0,secPoint1距离最短的点和距离最远的点
    minDis = maxDis = maxLength = 0.0;

    num1 = convexHullPoints.size();


    if (num1 == 0)
    {
        return;
    }

    double maxRight = 1e-9, maxLeft = 1e-9, maxUp = 1e-9, maxDown = 1e9;
    //简单的方法求包围盒，这里x 是不变的，那么就只要变动y和z即可。
    int index = -1;
    //求最z值，这里包围盒还是有点问题，只有z和y，那么到时候如果要改成x y z的那种，那么还是要有点问题了。明天再弄了吧，数据真的是难搞。
    for(int i = 0;i < num1;i++){
        point = convexHullPoints.at(i);
        if(point.m_z > maxRight){
            maxRight = point.m_z;
            index = i;
        }
    }
//    convexHullPoints.at(index).Debug();
    box.Add(convexHullPoints.at(index));

    for(int i = 0;i < num1;i++){
        point = convexHullPoints.at(i);
        if(point.m_y < maxDown){
            maxDown = point.m_y;
            index = i;
        }

    }

//    convexHullPoints.at(index).Debug();
    box.Add(convexHullPoints.at(index));

    for(int i = 0;i < num1;i++){
        point = convexHullPoints.at(i);
        if(point.m_z < maxLeft){
            maxLeft = point.m_z;
            index = i;
        }

    }
//    convexHullPoints.at(index).Debug();
    box.Add(convexHullPoints.at(index));


    for(int i = 0;i < num1;i++){
        point = convexHullPoints.at(i);
        if(point.m_y > maxUp){
            maxUp = point.m_y;
            index = i;
        }

    }
//    convexHullPoints.at(index).Debug();
    box.Add(convexHullPoints.at(index));

    return;
    for (i=0; i<num1; i++)
    {
        point = convexHullPoints.at(i);
        if (i == 0)
        {
            maxDis = minDis = dis = Relation3D::PToLDistance(secPoint[0], secPoint[1], point);
            fastestPoint = nearestPoint = point;
        }
        else
        {
            dis = Relation3D::PToLDistance(secPoint[0], secPoint[1], point);
            if (minDis > dis)
            {
                minDis = dis;
                nearestPoint = point;
            }
            if (maxDis < dis)
            {
                maxDis = dis;
                fastestPoint = point;
            }
        }
    }

    //平移secPoint0,secPoint1，平移距离为minDis（也即使平移的两点跟nearestPoint在同一直线上）
    //先计算平移向量v1
    projectPoint = PointProjectLine(secPoint[0], secPoint[1], nearestPoint);
    v1.Set(nearestPoint.m_x-projectPoint.m_x, nearestPoint.m_y-projectPoint.m_y, nearestPoint.m_z-projectPoint.m_z);
    v1.Normalize();

//    if (!v1.Normalize())
//    {
//        cout << "向量单位化失败！" << endl;
//
//        return;
//    }

    boxArray[0] = TranlatePoint(secPoint[0], v1, minDis-extraValue);
    boxArray[1] = TranlatePoint(secPoint[1], v1, minDis-extraValue);

    //计算凸壳上其他点到线boxArray0boxArray1的投影集
    for (i=0; i<num1; i++)
    {
        projectPoint =  PointProjectLine(boxArray[0], boxArray[1], convexHullPoints.at(i));
        projectPointSet.Add(projectPoint);
    }
    //选出一对最远距离的点,存入box0,box1中
    num2 = projectPointSet.size();
    for (int k=0; k<num2; k++)
    {
        projectPoint = projectPointSet.at(k);
        for (int h=k; h<projectPointSet.size()-1; h++)
        {
            projectPoint1 = projectPointSet.at(h+1);

            dis = Relation3D::PToPDistance(projectPoint, projectPoint1);
            if (dis > maxLength)
            {
                maxLength = dis;

                //判断方向是否一致
                if (IsSameDirection(boxArray[0], boxArray[1], projectPoint, projectPoint1))
                {
                    p[0] = projectPoint;
                    p[1] = projectPoint1;
                }
                else
                {
                    p[0] = projectPoint1;
                    p[1] = projectPoint;
                }
            }
        }
    }

    boxArray[0] = p[0];//赋值新生成的点
    boxArray[1] = p[1];

    v2.Set(p[1].m_x-p[0].m_x, p[1].m_y-p[0].m_y, p[1].m_z-p[0].m_z);
    v2.Normalize();

    boxArray[0] = TranlatePoint(p[0], v2, -extraValue);
    boxArray[1] = TranlatePoint(p[1], v2, extraValue);

    boxArray[2] = TranlatePoint(boxArray[1], v1, maxDis-minDis+2*extraValue);//平移新点使其到达上边界  maxDis-minDis + extraValue
    boxArray[3] = TranlatePoint(boxArray[0], v1, maxDis-minDis+2*extraValue);

    box.Add(boxArray[0]);
    box.Add(boxArray[1]);
    box.Add(boxArray[2]);
    box.Add(boxArray[3]);
//    cout << "begin box 2" << endl;
}


void MarchingCube::ShowInAndOut()
{
    GridPoint gridPoint;
    string str;
    float rgb[]={0, 1, 0};
    return ;
    for (int i=0; i<sizeI+1; i++)
    {
        for (int j=0; j<sizeJ+1; j++)
        {
            for (int k=0; k<sizek+1; k++)
            {
                gridPoint = gridPoints[i][j][k];
                if (gridPoint.isInPolygon == 2)
                {
//                    str.Format("%d", gridPoint.isInPolygon);
//                    TDOpenGL::DrawPoint(gridPoint.point, rgb);
                }

            }
        }
    }
}



void MarchingCube::CreateNewGrid(Point3D *point, PolyPoint3DArrayEx FrontPolyPointArray, PolyPoint3DArrayEx BackPolyPointArray, PolyPoint3DArrayEx unfDesityContours, PolyPoint3DArrayEx unbDesityContours,PolyPoint3DArrayEx& GridPointData)
{
    //测试计算的时间变量;
    clock_t      start, finish;
    cout << "begin grid" << endl;
    /*前曲线组的原始点*/
    PolyPoint3DArrayEx fOriginPolyPointArray;

    /*后曲线组的原始点*/
    PolyPoint3DArrayEx bOriginPolyPointArray;

    Point3DArrayEx tempPolyPoint;

    //两点间的距离值;
    double        Distance1, Distance2;

    double n1[3], n2[3];

    //耗费的时间,仅供测试;
    double       duration;
    int          IsInPolygon1, IsInPolygon2, num1, num2, MaxFrontNum, MaxBackNum, numFrontPolygonArray,numBackPolygonArray;

    //前剖面曲线FPolyPoint,和后剖面曲线BPolyPoint;
    Point3DArrayEx  FPolyPoint, BPolyPoint;
    Point3D      *TempPoint1, *TempPoint2, p;

    Polygon3D    fBox, bBox;

    //计算网格时所用到的一些中间点;
    Point3D      CurPoint0, CurPoint1, CurPoint2, CurPoint3, SecPoint0, SecPoint1;

    //前剖面多边形FrontPolygon,多边形组frontPolygonArray和后剖面多边形BackPolygon,多边形组backPolygonArray;
    Polygon3D    FrontPolygon, BackPolygon, tempPolygon;
    Polygon3DArrayEx frontPolygonArray, backPolygonArray, fOrginalPolygonArray, bOrginalPolygonArray;

    //多边形组
    MaxFrontNum = FrontPolyPointArray.size();
    MaxBackNum  = BackPolyPointArray.size();
//    cout << "num position :" <<  MaxFrontNum << " " <<  MaxBackNum << endl;
    frontPolygonArray.clear();
    backPolygonArray.clear();

    int num = 0;
    string str;

    //将前剖面的曲线组转化为多边形组;
    for (num1 = 0; num1 < MaxFrontNum; num1 ++)
    {
        FPolyPoint = FrontPolyPointArray.at(num1);
        FrontPolygon.clear();
        Polygon3D FrontPolygon(FPolyPoint);
        frontPolygonArray.Add(FrontPolygon);

        FrontPolygon.clear();
        FPolyPoint = unfDesityContours.at(num1);
        Polygon3D FrontPolygon1(FPolyPoint);
        fOrginalPolygonArray.Add(FrontPolygon1);
        fOriginPolyPointArray.Add(FPolyPoint);
    }

    //将后剖面的曲线组转化为多边形组;
    for (num2 = 0; num2 < MaxBackNum; num2 ++)
    {
        BPolyPoint = BackPolyPointArray.at(num2);
        BackPolygon.clear();
        Polygon3D BackPolygon(BPolyPoint);
        backPolygonArray.Add(BackPolygon);

        BackPolygon.clear();
        BPolyPoint = unbDesityContours.at(num2);
        Polygon3D BackPolygon1(BPolyPoint);
        bOrginalPolygonArray.Add(BackPolygon1);
        bOriginPolyPointArray.Add(BPolyPoint);
    }
    cout << "begin grid 2" << endl;
    //保存前剖面的四个角点;
    m_secPoint[0] = CurPoint0 = point[0];
    m_secPoint[1] = CurPoint1 = point[1];
    m_secPoint[2] = CurPoint2 = point[2];
    m_secPoint[3] = point[3];
    m_secPoint[4] = point[4];
    CurPoint3 = point[3];

    Point3DArrayEx f, b;

    Point3D fSecPoint[4], bSecPoint[4];


    for (int i=0; i<4; i++)
    {
        fSecPoint[i] = point[i];
        bSecPoint[i] = point[i+4];
    }
//    cout << "fbox point size " << fOriginPolyPointArray.size() << endl;
    //这里面有问题，但是就是找不到，问题太多了老铁，根本解决不了啊。
//    cout << "numbers: " << endl;
//    for(int i = 0;i < fOriginPolyPointArray.size();i++){
//        for(int j = 0;j < fOriginPolyPointArray.at(i).size();j++){
//            fOriginPolyPointArray.at(i).at(j).Debug();
//        }
//    }

    //这里的包围盒一个是前面，一个是后面，这样就是组成了棱台？
    CreateBox(fOriginPolyPointArray, fSecPoint, f, 0);
//    cout << "bbox point size " << bOriginPolyPointArray.size() << endl;
    CreateBox(bOriginPolyPointArray, bSecPoint, b, 0);

    cout << "box point:" << endl;
    for (int i=0; i<4; i++)
    {
        m_box[i] = f.at(i);
        m_box[i+4] = b.at(i);
        b.at(i).Debug();
//        b.at(i).Debug();
    }
    for (int i=0; i<4; i++)
    {
        fBox.Add(m_box[i]);
        bBox.Add(m_box[i+4]);
    }
//    cout << "begin grid 3" << endl;
    fBox.GetNormal(n1);
    bBox.GetNormal(n2);

    for (int i=0; i<3; i++)
    {
        n1[i] = -n1[i];
        n2[i] = -n2[i];
    }

    static int Count = 0;

    Polygon3D *ptrPolygon = NULL;

    //开启时间计数;
    start = clock();
    cout << "begin grid 4" << endl;
    //计算前后两个剖面上的网格,前后对应的四个网格点组成一个立方体;
    for (int i = 0; i < 2;  i++)
    {
        //最定格的盒子
        if (i == 0)
        {
            CurPoint0 = m_box[0];
            CurPoint1 = m_box[1];
            CurPoint2 = m_box[2];
            CurPoint3 = m_box[3];

        }
        else{
            CurPoint0 = m_box[4];
            CurPoint1 = m_box[5];
            CurPoint2 = m_box[6];
            CurPoint3 = m_box[7];
        }
        CurPoint0.Debug();
        CurPoint1.Debug();
        CurPoint2.Debug();
        CurPoint3.Debug();
        SecPoint0 = CurPoint0;
        SecPoint1 = CurPoint1;
        for (int j = 0; j < m_sizeJ + 1; j++)
        {
//            cout << "mc test" << endl;
            CurPoint0.m_x = (double)((CurPoint3.m_x - SecPoint0.m_x)/m_sizeJ) * j + SecPoint0.m_x;
            CurPoint0.m_y = (double)((CurPoint3.m_y - SecPoint0.m_y)/m_sizeJ) * j + SecPoint0.m_y;
            CurPoint0.m_z = (double)((CurPoint3.m_z - SecPoint0.m_z)/m_sizeJ) * j + SecPoint0.m_z;

            CurPoint1.m_x = (double)((CurPoint2.m_x - SecPoint1.m_x)/m_sizeJ) * j + SecPoint1.m_x;
            CurPoint1.m_y = (double)((CurPoint2.m_y - SecPoint1.m_y)/m_sizeJ) * j + SecPoint1.m_y;
            CurPoint1.m_z = (double)((CurPoint2.m_z - SecPoint1.m_z)/m_sizeJ) * j + SecPoint1.m_z;
//            cout << "Debug down: " << endl;
//            CurPoint0.Debug();
//            CurPoint1.Debug();
//            CurPoint2.Debug();
//            CurPoint3.Debug();
//            break;
            for (int k = 0; k < m_sizeK + 1; k++)
            {
                GridPoint* gridPoint = &m_GridPoints[i][j][k];

                //获得当前立方体顶点的坐标值;
                gridPoint->point.m_x = (double)((CurPoint1.m_x - CurPoint0.m_x)/m_sizeK) * k + CurPoint0.m_x;
                gridPoint->point.m_y = (double)((CurPoint1.m_y - CurPoint0.m_y)/m_sizeK) * k + CurPoint0.m_y;
                gridPoint->point.m_z = (double)((CurPoint1.m_z - CurPoint0.m_z)/m_sizeK) * k + CurPoint0.m_z;

                TempPoint1 = &gridPoint->point;
//                cout << "temp point : ";
//                TempPoint1->Debug();
                //当立方体顶点在前剖面时;
                if (i == 0)
                {
                    num ++;
                    //对当前点依次与前剖面上的多边形组进行距离计算;
                    numFrontPolygonArray = fOriginPolyPointArray.size();
                    for (int index = 0; index < numFrontPolygonArray; index ++)
                    {
                        /*用未插点的曲线做未多边形，进行网格点的内外判断*/
                        ptrPolygon = &fOrginalPolygonArray.at(index);
                        //判断点顶点是否在多边形内,IsInPolygon1为1表示在多边形边上,为2表示在多边形的内部,否则在多边形的外面;
                        IsInPolygon1 = ptrPolygon->Contain(*TempPoint1, n1);
                        //IsInPolygon1 = PointInPolygon(*TempPoint1, fOriginPolyPointArray.at(index));

                        //afxDump<<i<<" "<<j<<" "<<k<<" "<<IsInPolygon1<<"\n";


                        /*用插点的曲线去求网格点的权值*/
                        ptrPolygon = &frontPolygonArray.at(index);

                        //在多边形上，直接顶点权值赋值为0;
                        if(IsInPolygon1 == 1)
                        {
//                            cout << i << " " << j << " " << k << "now" << endl;
                            gridPoint->val = 0;
                            gridPoint->isInPolygon = 1;
                        }
                            //在多边形内部，则计算顶点到多边形上点的最短距离;
                        else if (IsInPolygon1 == 2)
                        {
//                            cout << i << " " << j << " " << k << "in" << endl;
                            //计算点TempPoint到多边形上点的最短距离
                            //Distance1 = Distance2 = Relation3D::PToPDistance(ptrPolygon->at(0), *TempPoint1);
                            Distance1 = Distance2 = CalcDistance(ptrPolygon->at(0), *TempPoint1);
                            for (int index = 1; index < ptrPolygon->size(); index ++)
                            {
                                TempPoint2 = &ptrPolygon->at(index);
                                //Distance1 = Relation3D::PToPDistance(*TempPoint2, *TempPoint1);
                                Distance1 = CalcDistance(*TempPoint2, *TempPoint1);
                                Distance2 = (Distance1 > Distance2)? Distance2: Distance1;
                            }
                            gridPoint->val = Distance2;
                            gridPoint->isInPolygon = 2;

                        }
                            //在多边形外面，计算顶点多变形上点的最短距离，并取距离的负值；
                        else
                        {
//                            cout << i << " " << j << " " << k << "out" << endl;
                            gridPoint->isInPolygon = 0;
                            //Distance1 = Distance2 = Relation3D::PToPDistance(ptrPolygon->at(0), *TempPoint1);
                            Distance1 = Distance2 = CalcDistance(ptrPolygon->at(0), *TempPoint1);
                            for (int index = 1; index < ptrPolygon->size(); index ++)
                            {
                                TempPoint2 = &ptrPolygon->at(index);
                                //Distance1 = Relation3D::PToPDistance(*TempPoint2, *TempPoint1);
                                Distance1 = CalcDistance(*TempPoint2, *TempPoint1);
                                Distance2 = (Distance1 > Distance2)? Distance2: Distance1;
                            }

                            //与上次计算的权值比较，若绝对值大于当前值，则更改其权值；
                            if (abs(gridPoint->val) >= Distance2) {
                                gridPoint->val = Distance2;
                                gridPoint->val = - gridPoint->val;
                            }
                                //否则，继续下一个顶点的计算;
                            else
                                continue;
                        }
                    }
                }
                    //当立方体顶点在后剖面时;
                else
                {

                    num ++;
                    numBackPolygonArray = bOriginPolyPointArray.size();
                    //numBackPolygonArray = backPolygonArray.size();
                    for (int index = 0; index < numBackPolygonArray; index ++)
                    {
                        /*用未插点的曲线做未多边形，进行网格点的内外判断*/
                        ptrPolygon = &bOrginalPolygonArray.at(index);
                        IsInPolygon2 = ptrPolygon->Contain(*TempPoint1, n2);

                        /*用插点的曲线去求网格点的权值*/
                        ptrPolygon = &backPolygonArray.at(index);

                        if (IsInPolygon2 == 1)
                        {
                            gridPoint->val = 0;
                            gridPoint->isInPolygon = 1;
                        }
                        else if (IsInPolygon2 == 2)
                        {
                            //Distance1 = Distance2 = Relation3D::PToPDistance(ptrPolygon->at(0), *TempPoint1);
                            Distance1 = Distance2 = CalcDistance(ptrPolygon->at(0), *TempPoint1);
                            for (int index = 1; index < ptrPolygon->size(); index ++)
                            {
                                TempPoint2 = &ptrPolygon->at(index);
                                //Distance1 = Relation3D::PToPDistance(*TempPoint2, *TempPoint1);
                                Distance1 = CalcDistance(*TempPoint2, *TempPoint1);
                                Distance2 = (Distance1 > Distance2)? Distance2:Distance1;
                            }
                            gridPoint->val = Distance2;
                            gridPoint->isInPolygon = 2;
                        }
                        else
                        {
                            gridPoint->isInPolygon = 0;
                            Distance1 = Distance2 = CalcDistance(ptrPolygon->at(0), *TempPoint1);
                            //Distance1 = Distance2 = Relation3D::PToPDistance(ptrPolygon->at(0), *TempPoint1);
                            for (int index = 1; index < ptrPolygon->size(); index ++)
                            {
                                TempPoint2 = &ptrPolygon->at(index);
                                //Distance1 = Relation3D::PToPDistance(*TempPoint2, *TempPoint1);
                                Distance1 = CalcDistance(*TempPoint2, *TempPoint1);
                                Distance2 = (Distance1 > Distance2)? Distance2:Distance1;
                            }
                            if (abs(gridPoint->val) >= Distance2) {
                                gridPoint->val = Distance2;
                                gridPoint->val = - gridPoint->val;
                            }
                            else
                                continue;

                        }
                    }
                }
            }

        }
    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "mc success" << endl;
    //ConnectGrids();
    //CreateSurface();
    m_sumTime += duration;
//    str.Format("所用时间%f, 总共用的时间：%f\n", duration, m_sumTime);

    //AfxOutputDebugString(str);
    SaveGridPoint(GridPointData);

    /*sizeI = m_sizeI;
    sizeJ = m_sizeJ;
    sizek = m_sizeK;
    gridPoints = new GridPoint**[m_sizeI + 1];
    for (int i = 0; i < m_sizeI + 1; i++) {
        gridPoints[i] = new GridPoint*[m_sizeJ + 1];
        for (int j = 0; j < m_sizeJ + 1; j++) {
            gridPoints[i][j] = new GridPoint[m_sizeK + 1];
        }
    }
    for (int i=0; i<sizeI+1; i++)
    {
        for (int j=0; j<sizeJ+1; j++)
        {
            for (int k=0; k<sizek+1; k++)
            {
                gridPoints[i][j][k] = m_GridPoints[i][j][k];
            }
        }
    }*/
}

double MarchingCube::CalcDistance(Point3D point1, Point3D point2)
{
    double m_x = point1.m_x - point2.m_x;
    double m_y = point1.m_y - point2.m_y;
    double m_z = point1.m_z - point2.m_z;
    return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}



void MarchingCube::SaveGridPoint(PolyPoint3DArrayEx& polyPointArray)
{
    Point3D point;
    Point3DArrayEx polyPoint;
    Cube***  gridCell = m_Cubes;
    polyPoint.clear();
    int count = 0;
    for (int i = 0; i < m_sizeI; i++)
        for (int j = 0; j < m_sizeJ; j++)
            for (int k = 0; k < m_sizeK; k++)
            {
                if (gridCell[i][j][k].hasFacet == false)
                {
                    continue;
                }
                count ++;
                polyPoint.Add(gridCell[i][j][k].point[0]->point);
                polyPoint.Add(gridCell[i][j][k].point[1]->point);
                polyPoint.Add(gridCell[i][j][k].point[2]->point);
                polyPoint.Add(gridCell[i][j][k].point[3]->point);
                polyPoint.Add(gridCell[i][j][k].point[4]->point);
                polyPoint.Add(gridCell[i][j][k].point[5]->point);
                polyPoint.Add(gridCell[i][j][k].point[6]->point);
                polyPoint.Add(gridCell[i][j][k].point[7]->point);
                polyPointArray.Add(polyPoint);
                polyPoint.clear();
            }

}

//void MarchingCube::ShowBox(PolyPoint3DArrayEx polyBoxArray)
//{
//    float rgb[]={1.0, 0.0, 0.0};
//    Point3D PointArray1[4], PointArray2[4], PointArray3[4], PointArray4[4];
//    Point3DArrayEx polyPoint;
//    Point3DArrayEx boxArray;
//
//    Point3D point[8];
//    glLineWidth(0.2);
//
//    float rgb1[] ={0.0, 1.0, 1.0};
//
//    for (int i=0; i<polyBoxArray.size(); i++)
//    {
//        boxArray = polyBoxArray.at(i);
//        if (boxArray.size() != 8)
//        {
//            return;
//        }
//        PointArray1[0] = boxArray.at(0);
//        PointArray1[1] = boxArray.at(1);
//        PointArray1[2] = boxArray.at(2);
//        PointArray1[3] = boxArray.at(3);
//
//        PointArray2[0] = boxArray.at(4);
//        PointArray2[1] = boxArray.at(5);
//        PointArray2[2] = boxArray.at(6);
//        PointArray2[3] = boxArray.at(7);
//
//        PointArray3[0] = boxArray.at(0);
//        PointArray3[1] = boxArray.at(4);
//        PointArray3[2] = boxArray.at(7);
//        PointArray3[3] = boxArray.at(3);
//
//        PointArray4[0] = boxArray.at(1);
//        PointArray4[1] = boxArray.at(5);
//        PointArray4[2] = boxArray.at(6);
//        PointArray4[3] = boxArray.at(2);
//
//        glEnable(GL_BLEND);   //   打开混合
//
//        glColor4f(0.0, 0.0,1.0, 0.1);
//        glBegin(GL_QUADS);
//        glVertex3d(PointArray4[0].m_x,PointArray4[0].m_y,PointArray4[0].m_z);
//        glVertex3d(PointArray4[1].m_x,PointArray4[1].m_y,PointArray4[1].m_z);
//        glVertex3d(PointArray4[2].m_x,PointArray4[2].m_y,PointArray4[2].m_z);
//        glVertex3d(PointArray4[3].m_x,PointArray4[3].m_y,PointArray4[3].m_z);
//
//        glVertex3d(PointArray3[0].m_x,PointArray3[0].m_y,PointArray3[0].m_z);
//        glVertex3d(PointArray3[1].m_x,PointArray3[1].m_y,PointArray3[1].m_z);
//        glVertex3d(PointArray3[2].m_x,PointArray3[2].m_y,PointArray3[2].m_z);
//        glVertex3d(PointArray3[3].m_x,PointArray3[3].m_y,PointArray3[3].m_z);
//
//        glVertex3d(PointArray2[0].m_x,PointArray2[0].m_y,PointArray2[0].m_z);
//        glVertex3d(PointArray2[1].m_x,PointArray2[1].m_y,PointArray2[1].m_z);
//        glVertex3d(PointArray2[2].m_x,PointArray2[2].m_y,PointArray2[2].m_z);
//        glVertex3d(PointArray2[3].m_x,PointArray2[3].m_y,PointArray2[3].m_z);
//
//        glVertex3d(PointArray1[0].m_x,PointArray1[0].m_y,PointArray1[0].m_z);
//        glVertex3d(PointArray1[1].m_x,PointArray1[1].m_y,PointArray1[1].m_z);
//        glVertex3d(PointArray1[2].m_x,PointArray1[2].m_y,PointArray1[2].m_z);
//        glVertex3d(PointArray1[3].m_x,PointArray1[3].m_y,PointArray1[3].m_z);
//
//        glVertex3d(boxArray.at(0).m_x,boxArray.at(0).m_y,boxArray.at(0).m_z);
//        glVertex3d(boxArray.at(1).m_x,boxArray.at(1).m_y,boxArray.at(1).m_z);
//        glVertex3d(boxArray.at(5).m_x,boxArray.at(5).m_y,boxArray.at(5).m_z);
//        glVertex3d(boxArray.at(4).m_x,boxArray.at(4).m_y,boxArray.at(4).m_z);
//
//        glVertex3d(boxArray.at(3).m_x,boxArray.at(3).m_y,boxArray.at(3).m_z);
//        glVertex3d(boxArray.at(2).m_x,boxArray.at(2).m_y,boxArray.at(2).m_z);
//        glVertex3d(boxArray.at(6).m_x,boxArray.at(6).m_y,boxArray.at(6).m_z);
//        glVertex3d(boxArray.at(7).m_x,boxArray.at(7).m_y,boxArray.at(7).m_z);
//
//        glEnd();
//
//
//        TDOpenGL::DrawQuad(PointArray1, rgb1, UNSELECT, TRANSPARANT);
//        TDOpenGL::DrawQuad(PointArray2, rgb1, UNSELECT, TRANSPARANT);
//        TDOpenGL::DrawQuad(PointArray3, rgb1, UNSELECT, TRANSPARANT);
//        TDOpenGL::DrawQuad(PointArray4, rgb1, UNSELECT, TRANSPARANT);
//
//    }
//}

//void MarchingCube::SaveBoxArray(Point3DArray &boxArray)
//{
//    for (int i=0; i<8; i++)
//    {
//        boxArray.Add(m_box[i]);
//    }
//}

//void MarchingCube::ShowGridCell(PolyPoint3DArrayEx polyPointArray)
//{
//    float rgb[]={1.0, 0.0, 0.0};
//    Point3D PointArray1[4], PointArray2[4], PointArray3[4], PointArray4[4];
//    Point3DArrayEx polyPoint;
//    string str;
//
//    Point3D point[8];
////    glLineWidth(0.2);
//
//    float rgb1[] ={0.0, 1.0, 1.0};
//    float rgb2[] ={1.0, 1.0, 1.0};
//
//    for(int i=0; i<polyPointArray.size(); i++)
//    {
//        polyPoint.clear();
//        polyPoint = polyPointArray.at(i);
//        for(int j=0; j<polyPoint.size(); j++)
//            point[j] = polyPoint.at(j);
//
//        PointArray1[0] = point[2];
//        PointArray1[1] = point[3];
//        PointArray1[2] = point[0];
//        PointArray1[3] = point[1];
//
//        PointArray2[0] = point[4];
//        PointArray2[1] = point[5];
//        PointArray2[2] = point[6];
//        PointArray2[3] = point[7];
//
//        PointArray3[0] = point[6];
//        PointArray3[1] = point[7];
//        PointArray3[2] = point[3];
//        PointArray3[3] = point[2];
//
//        PointArray4[0] = point[1];
//        PointArray4[1] = point[0];
//        PointArray4[2] = point[4];
//        PointArray4[3] = point[5];
//
//        ////TDOpenGL::DrawPoint(point[0], rgb);
//        ////TDOpenGL::DrawPoint(point[1], rgb);
//        ////TDOpenGL::DrawPoint(point[2], rgb);
//        ////TDOpenGL::DrawPoint(point[3], rgb);
//        ////TDOpenGL::DrawPoint(point[4], rgb);
//        ////TDOpenGL::DrawPoint(point[5], rgb);
//        ////TDOpenGL::DrawPoint(point[6], rgb);
//        ////TDOpenGL::DrawPoint(point[7], rgb);
//
//        TDOpenGL::DrawQuad(PointArray1, rgb, UNSELECT, TRANSPARANT);
//        TDOpenGL::DrawQuad(PointArray2, rgb, UNSELECT, TRANSPARANT);
//        TDOpenGL::DrawQuad(PointArray3, rgb, UNSELECT, TRANSPARANT);
//        TDOpenGL::DrawQuad(PointArray4, rgb, UNSELECT, TRANSPARANT);
//    }
//}


float MarchingCube::Magnitude( Point3D Point1, Point3D Point2 )
{
    Point3D Vector;

    Vector.m_x = Point2.m_x - Point1.m_x;
    Vector.m_y = Point2.m_y - Point1.m_y;
    Vector.m_z = Point2.m_z - Point1.m_z;

    return (float)sqrt( Vector.m_x * Vector.m_x + Vector.m_y * Vector.m_y + Vector.m_z * Vector.m_z );
}

bool MarchingCube::InBox(Point3D point, Polygon3D polygon, double n[3])
{
    if (polygon.Contain(point, n))
    {
        return true;
    }
    else
        return false;
}


void MarchingCube::ComputeBox(PolyPoint3DArrayEx fPolyPointArray, PolyPoint3DArrayEx bPolyPointArray, Point3D *secPoint, double extraValue)
{
    Point3DArrayEx projectPoints;
    Point3DArrayEx newPointArray;
    Point3DArrayEx    backPoints, frontPoints;
    Vector3D v1, v2, u;
    double heigth;
    Point3DArrayEx boxPoints;

    heigth = Relation3D::PToPDistance(secPoint[0], secPoint[4]);

    v1.Set(secPoint[0].m_x - secPoint[1].m_x, secPoint[0].m_y- secPoint[1].m_y, secPoint[0].m_z- secPoint[1].m_z);
    v2.Set(secPoint[2].m_x - secPoint[1].m_x, secPoint[2].m_y- secPoint[1].m_y, secPoint[2].m_z- secPoint[1].m_z);

    v1.Normalize();
    v2.Normalize();

    Vector3D::CrossProduct(v1, v2, u);

    for (int i=0; i<bPolyPointArray.size(); i++)
    {
        backPoints = bPolyPointArray.at(i);

        for (int j=0; j<backPoints.size(); j++)
        {
            newPointArray.Add(backPoints.at(j));
        }
    }

    GetProjectPointsSet(u, secPoint[0], newPointArray, projectPoints);

    for (int i=0; i<fPolyPointArray.size(); i++)
    {
        frontPoints = fPolyPointArray.at(i);

        for (int j=0; j<frontPoints.size(); j++)
        {
            projectPoints.Add(frontPoints.at(j));
        }
    }

    CreateABox(u, heigth, projectPoints, boxPoints, extraValue);
}

void MarchingCube::GetProjectPointsSet(Vector3D v, Point3D refPoint, Point3DArrayEx pointArray, Point3DArrayEx &projectPointSet)
{
    Point3D p[3];
    Triangle3D triangle;
    Point3D point;
    //double a[3];
    Vector3D v1;
    v1.Set(-v.m_x, -v.m_y, -v.m_z);
    projectPointSet.clear();

    for (int i=0; i<pointArray.size(); i++)
    {
        point = pointArray.at(i);
        point = PointProjectPlane(point, refPoint, v, v);
        projectPointSet.Add(point);
    }
}

double MarchingCube::CreateABox(Vector3D v, double heigth, Point3DArrayEx projectPoints, Point3DArrayEx &boxPoints, double extraValue)
{
    Point3D p[4];
    Point3DArrayEx convexHullPoints, tempPointArray;
    Triangle3D triangle;
    Point3D point;
    //double a[3];
    Vector3D v1, v2;

    Point3D curBox[4], lastBox[4];
    double m_n[3];
    int num;
    double distance;
    double curMaxDistance[]={0.0, 0.0};
    double lastMaxDistance[]={FLT_MAX,FLT_MAX};
    Point3D box[8];
    double volume;
    Point3D fastestPoint, projectPoint;

    struct BoxData
    {
        Point3D p1, p2;                      //最长边的两个端点坐标
        Point3D fastestPoint, projectPoint;  //到平面最远点以及在平面上的投影
        double  length, height;              //距平面最远点距离height和凸壳一条边上最远端点的距离length
    };

    v2 = v1 = v;

    if (!projectPoints.GetPlanarConvexHull(convexHullPoints, m_n))
    {
//        AfxOutputDebugString("凸壳生成失败！！");
        return 0;
    }


    //依次对当前凸壳的每一条边进行包围盒计算，选出最小面积的包围盒
    num = convexHullPoints.size();

    if (num == 0)
    {
        return 0;
    }

    BoxData *boxData = new BoxData[num];

    for (int i=0; i<num; i++)
    {
        tempPointArray.clear();

        p[0] = convexHullPoints.at(i);
        p[1] = convexHullPoints.at((i+1)%num);

        boxData[i].p1 = p[0];
        boxData[i].p2 = p[1];

        tempPointArray.Add(p[0]);
        tempPointArray.Add(p[1]);

        for (int j=0; j<num; j++)
        {
            //取剩余的点
            point = convexHullPoints.at(j);

            if (point == p[0] || point == p[1])
            {
                continue;
            }

            //计算到当前线的最远距离
            distance = Relation3D::PToLDistance(p[0], p[1], point);

            //计算剩余点在线上的投影
            Point3D projectPoint = PointProjectLine(p[0], p[1], point);

            if (distance > curMaxDistance[0])
            {
                curMaxDistance[0] = distance;
                boxData[i].height = distance;
            }
            tempPointArray.Add(projectPoint);
        }

        curMaxDistance[0] = 0;
        distance = 0;

        //选出一对最远距离的点,存入box0,box1中
        for (int k=0; k<tempPointArray.size(); k++)
        {
            p[2] = tempPointArray.at(k);
            for (int h=k; h<tempPointArray.size()-1; h++)
            {
                p[3] = tempPointArray.at(h+1);

                distance = sqrt((double)pow((p[2].m_x - p[3].m_x), 2) + pow(p[2].m_y - p[3].m_y, 2) + pow(p[2].m_z - p[3].m_z, 2));

                if (distance > curMaxDistance[1])
                {
                    curMaxDistance[1] = distance;
                    boxData[i].length = distance;

                    //判断p2p3方向与边p0p1的方向是否一致
                    if (IsSameDirection(p[0], p[1], p[2], p[3]))
                    {
                        boxData[i].p1 = p[2];
                        boxData[i].p2 = p[3];
                    }
                    else
                    {
                        boxData[i].p1 = p[3];
                        boxData[i].p2 = p[2];
                    }
                }
            }

        }
        curMaxDistance[1] = 0;

    }

    for (int i=0; i<num; i++)
    {

        BoxData *data = &boxData[i];

        if (i==0)
        {
            box[0] = boxData[i].p1;
            box[1] = boxData[i].p2;
            distance = boxData[i].height;
            i++;
            BoxData *data = &boxData[i];
            volume = boxData[i].height * boxData[i].length * heigth;
        }

        if (boxData[i].height * boxData[i].length < Relation3D::PToPDistance(box[0], box[1]) * distance)
        {
            box[0] = boxData[i].p1;
            box[1] = boxData[i].p2;
            distance = boxData[i].height;
            volume = boxData[i].height * boxData[i].length * heigth;
        }

    }


    v1.Set(box[1].m_x - box[0].m_x, box[1].m_y - box[0].m_y, box[1].m_z - box[0].m_z);
    v1.Normalize();

    p[0] = convexHullPoints.at(0);
    p[1] = convexHullPoints.at(1);
    p[2] = convexHullPoints.at(2);

    v2.Set(p[0].m_x-p[1].m_x, p[0].m_y-p[1].m_y, p[0].m_z-p[1].m_z);
    Vector3D v3;
    v3.Set(p[2].m_x-p[1].m_x, p[2].m_y-p[1].m_y, p[2].m_z-p[1].m_z);
    Vector3D::CrossProduct(v3, v2, v2);
    v2.Normalize();

    lastBox[0] = TranlatePoint(box[0], v1, -extraValue);//起点box0负偏离extraValue长度
    lastBox[1] = TranlatePoint(box[1], v1, extraValue); //终点box1正偏离extraValue长度

    v1.CrossProduct(v2, v1);

    lastBox[2] = TranlatePoint(lastBox[1], v1, distance+extraValue); //distance加上extraValue的偏离长度
    lastBox[3] = TranlatePoint(lastBox[0], v1, distance+extraValue);
    lastBox[0] = TranlatePoint(lastBox[0], v1, -extraValue); //起点lastBox0负偏离extraValue长度
    lastBox[1] = TranlatePoint(lastBox[1], v1, -extraValue); //终点lastBox1负偏离extraValue长度

    if (!IsDirectionSameWithVector(m_secPoint[4], m_secPoint[0], v2))
    {
        heigth = -heigth;
    }

    box[4] = TranlatePoint(lastBox[0], v2, heigth);
    box[5] = TranlatePoint(lastBox[1], v2, heigth);
    box[6] = TranlatePoint(lastBox[2], v2, heigth);
    box[7] = TranlatePoint(lastBox[3], v2, heigth);

    m_box[0] = lastBox[0];
    m_box[1] = lastBox[1];
    m_box[2] = lastBox[2];
    m_box[3] = lastBox[3];

    m_box[4] = box[4];
    m_box[5] = box[5];
    m_box[6] = box[6];
    m_box[7] = box[7];

    if (boxData != NULL)
    {
        delete[] boxData;
        boxData = NULL;
    }
    return 	volume;
}
bool MarchingCube::IsDirectionSameWithVector(Point3D p, Point3D projectPoint, Vector3D n)
{
    Vector3D v, v1;
    double value;
    v.Set(p.m_x-projectPoint.m_x, p.m_y-projectPoint.m_y, p.m_z-projectPoint.m_z);
    v.Normalize();
    value = v1.DotProduct(v, n);
    if (value < THRESHOLD10)
    {
        return false;
    }
    else
        return true;
}

bool MarchingCube::IsSameDirection(Vector3D v1, Vector3D v2)
{
    double value;
    Vector3D u;
    value = u.DotProduct(v1,v2);
    if (value < THRESHOLD10)
    {
        return false;
    }
    else
        return true;
}

Point3D MarchingCube::LinePlaneInt(Point3D& pointOnLine, Vector3D& vectorOfLine, Point3D& pointOnPlane, Vector3D& vectorOfPlane)
{
    Point3D pointOfInt;
    double lineParameter, dotProduct, x, y, z;

    dotProduct = Vector3D::DotProduct(vectorOfLine, vectorOfPlane);
    lineParameter = (vectorOfPlane.m_x * (pointOnPlane.m_x - pointOnLine.m_x) +
                     vectorOfPlane.m_y * (pointOnPlane.m_y - pointOnLine.m_y) +
                     vectorOfPlane.m_z * (pointOnPlane.m_z - pointOnLine.m_z)) / dotProduct;

    x = pointOnLine.m_x + vectorOfLine.m_x * lineParameter;
    y = pointOnLine.m_y + vectorOfLine.m_y * lineParameter;
    z = pointOnLine.m_z + vectorOfLine.m_z * lineParameter;

    pointOfInt.Set(x, y, z);
    return pointOfInt;
}

Point3D MarchingCube::PointProjectLine(Point3D p1, Point3D p2, Point3D p)
{
    Vector3D v;
    v.Set(p2.m_x-p1.m_x, p2.m_y-p1.m_y, p2.m_z-p1.m_z);
    v.Normalize();
    return LinePlaneInt(p1, v, p, v);
}

bool MarchingCube::IsSameDirection(Point3D p1, Point3D p2, Point3D q1, Point3D q2)
{
    Vector3D v, v1, v2;
    double value;
    v1.Set(p2.m_x-p1.m_x, p2.m_y-p1.m_y, p2.m_z-p1.m_z);
    v1.Normalize();

    v2.Set(q2.m_x-q1.m_x, q2.m_y-q1.m_y, q2.m_z-q1.m_z);
    v2.Normalize();

    value = v.DotProduct(v1, v2);
    if (value < THRESHOLD10)
    {
        return false;
    }
    else
        return true;
}

Point3D MarchingCube::TranlatePoint(Point3D &p, Vector3D direction, double dis)
{
    Point3D resultPoint;
    resultPoint.m_x = p.m_x + direction.m_x * dis;
    resultPoint.m_y = p.m_y + direction.m_y * dis;
    resultPoint.m_z = p.m_z + direction.m_z * dis;
    return resultPoint;
}

Point3D MarchingCube::PointProjectPlane(Point3D p1,Point3D p0,Vector3D N,Vector3D U)
{
    Point3D projectpoint;
    //求直线方程
    double dt;
    dt = N.m_x*U.m_x+N.m_y*U.m_y+N.m_z*U.m_z;
    if(fabs(dt - 0.0) < THRESHOLD10) return projectpoint;

    //求平面方程
    double d; //平面方程的d
    double t;

    d  = -(U.m_x*p0.m_x + U.m_y*p0.m_y + U.m_z*p0.m_z);
    t = U.m_x*p1.m_x + U.m_y*p1.m_y + U.m_z*p1.m_z;
    t = -(t + d);
    t = t/dt;

    projectpoint.m_x = p1.m_x + t*N.m_x;
    projectpoint.m_y = p1.m_y + t*N.m_y;
    projectpoint.m_z = p1.m_z + t*N.m_z;

    return projectpoint;
}

bool MarchingCube::GetNormal(Point3DArrayEx& polyPoint,double a[])
{
    Vector3D v;
    double   p1[3],p2[3];
    int n = 0;
    n = polyPoint.size();
    if (n <=3)
        return false;
    Point3D pointa,pointb,pointc;
    pointa = polyPoint.at(0);
    pointb = polyPoint.at(1);
    pointc = polyPoint.at(2);
    p1[0]=pointa.m_x-pointb.m_x,p1[1]=pointa.m_y-pointb.m_y,p1[2]=pointa.m_z-pointb.m_z;
    p2[0]=pointc.m_x-pointb.m_x,p2[1]=pointc.m_y-pointb.m_y,p2[2]=pointc.m_z-pointb.m_z;
    v=v.CrossProduct(p1,p2);
    if(v.Normalize())
    {
        v.Get(a);
        return 1;
    }
    else
    {
        v.Set(0,0,0);
        v.Get(a);
    }
    return 0;
}


// 显示文字
//void MarchingCube::DrawString(const char* str, Point3D& pos)
//{
//    int len, i;
//    wchar_t* wstring;
//    HDC hDC = wglGetCurrentDC();
//    GLuint list = glGenLists(1);
//    static int isFirstCall = 1;
//    glColor3f(1.0, 0.0, 0.0);
//    SelectFont(20, GB2312_CHARSET, "仿宋体");
//    glRasterPos3f(pos.m_x, pos.m_y, pos.m_z);
//    len = 0;
//    for(i=0; str[i]!='\0'; ++i)
//    {
//        if( IsDBCSLeadByte(str[i]) )
//            ++i;
//        ++len;
//    }
//
//    wstring = (wchar_t*)malloc((len+1) * sizeof(wchar_t));
//    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
//    wstring[len] = L'\0';
//
//    for(i=0; i<len; ++i)
//    {
//        wglUseFontBitmapsW(hDC, wstring[i], 1, list);
//        glCallList(list);
//    }
//    free(wstring);
//    glDeleteLists(list, 1);
//}

// 选择字体
//void MarchingCube::SelectFont(int size, int charset, const char* face)
//{
//    HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
//                              charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
//                              DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
//    HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
//    DeleteObject(hOldFont);
//}

bool MarchingCube::CreateOpenCurveBox(Point3DArrayEx openCurve, int index1, int index2, Point3DArrayEx &box, Vector3D u)
{
    Point3D point, p[2], fastestPoint[2], curveStartP, curveEndP, projectPoint, projectPoint1, boxPoints[4];
    int i, j, num, num1, num2;
    Vector3D v1, v2, v3, v;
    Point3DArrayEx newPoints, projectPointSet, convexHullPoints;
    Point3DArrayEx oneGroupP, anthorGroupP;
    double n[3], maxDis[2], maxLength, dis;

    num = openCurve.size();
    curveStartP = openCurve.at(index1);
    curveEndP   = openCurve.at(index2);
    //添加开曲线到新的数组中
    for (i=0; i<num; i++)//attation
    {
        point = openCurve.at(i);
        newPoints.Add(point);
    }

    //计算凸壳
    if (newPoints.GetPlanarConvexHull(convexHullPoints, n) == 0)
    {
//        AfxOutputDebugString("点集凸壳计算不成功！");
        return false;
    }

    //求取其他点在线curveStartP,curveEndP上的投影
    num = convexHullPoints.size();
    for (i=0; i<num; i++)
    {
        projectPoint =  PointProjectLine(curveStartP, curveEndP, convexHullPoints.at(i));
        projectPointSet.Add(projectPoint);
    }

    //选出一对最远距离的点,存入p0p1中
    num = projectPointSet.size();
    maxLength = 0.0;
    for (int k=0; k<num; k++)
    {
        projectPoint = projectPointSet.at(k);
        for (int h=k; h<projectPointSet.size()-1; h++)
        {
            projectPoint1 = projectPointSet.at(h+1);

            dis = Relation3D::PToPDistance(projectPoint, projectPoint1);
            if (dis > maxLength)
            {
                maxLength = dis;

                //判断方向是否一致
                if (IsSameDirection(curveEndP, curveStartP, projectPoint, projectPoint1))
                {
                    p[0] = projectPoint1;
                    p[1] = projectPoint;
                }
                else
                {
                    p[0] = projectPoint;
                    p[1] = projectPoint1;
                }
            }
        }
    }

    v.Set(curveStartP.m_x-curveEndP.m_x, curveStartP.m_y-curveEndP.m_y, curveStartP.m_z-curveEndP.m_z);
    v.Normalize();
    //计算到线两端点的距离，选取较长的距离进行点的平移
    dis = Relation3D::PToPDistance(p[0], curveStartP);
    maxLength = Relation3D::PToPDistance(p[1], curveEndP);
    if (maxLength < dis)
    {
        maxLength = dis;
        p[1] = TranlatePoint(p[1], v, -(maxLength + 50));
    }
    else
    {
        if (maxLength > dis)
            p[0] = TranlatePoint(p[0], v, maxLength + 50);
    }

    //分别计算凸壳上点在线curveEndPcurveStartP两侧的最远点距离值和对应得坐标点
    GroupByLine(curveStartP, curveEndP, convexHullPoints, oneGroupP, anthorGroupP);

    maxDis[0] = maxDis[1] = 0.0;
    num1 = oneGroupP.size();
    for (i=0; i<num1; i++)
    {
        point = oneGroupP.at(i);
        dis = Relation3D::PToLDistance(curveStartP, curveEndP, point);
        if (i==0)
        {
            maxDis[0] = dis;
            fastestPoint[0] = point;
        }
        else
        {
            if (maxDis[0] < dis)
            {
                maxDis[0] = dis;
                fastestPoint[0] = point;
            }
        }
    }
    num2 = anthorGroupP.size();
    for (i=0; i<num2; i++)
    {
        point = anthorGroupP.at(i);
        dis = Relation3D::PToLDistance(curveStartP, curveEndP, point);
        if (i==0)
        {
            maxDis[1] = dis;
            fastestPoint[1] = point;
        }
        else
        {
            if (maxDis[1] < dis)
            {
                maxDis[1] = dis;
                fastestPoint[1] = point;
            }
        }
    }

    //平移原直线上的两点构成包围盒
    dis = max(maxDis[0], maxDis[1]);
    if (maxDis[0] != 0.0 )
    {
        projectPoint = PointProjectLine(curveStartP, curveEndP, fastestPoint[0]);
        v1.Set(fastestPoint[0].m_x-projectPoint.m_x, fastestPoint[0].m_y-projectPoint.m_y, fastestPoint[0].m_z-projectPoint.m_z);
        v1.Normalize();
        v3.CrossProduct(v1, u);
        v3.Normalize();
    }
    else
    {
        if (maxDis[1] != 0.0)
        {
            projectPoint = PointProjectLine(curveStartP, curveEndP, fastestPoint[1]);
            v1.Set(fastestPoint[1].m_x-projectPoint.m_x, fastestPoint[1].m_y-projectPoint.m_y, fastestPoint[1].m_z-projectPoint.m_z);
            v1.Normalize();
            v3.CrossProduct(v1, u);
            v3.Normalize();
        }
    }

    if (v3 == v)
    {
        boxPoints[2] = TranlatePoint(p[1], v1, dis+50);
        boxPoints[3] = TranlatePoint(p[0], v1, dis+50);
        v2.Set(-v1.m_x, -v1.m_y, -v1.m_z);
        boxPoints[0] = TranlatePoint(p[0], v2, dis+50);
        boxPoints[1] = TranlatePoint(p[1], v2, dis+50);
    }
    else
    {
        boxPoints[0] = TranlatePoint(p[0], v1, dis+50);
        boxPoints[1] = TranlatePoint(p[1], v1, dis+50);
        v2.Set(-v1.m_x, -v1.m_y, -v1.m_z);
        boxPoints[2] = TranlatePoint(p[1], v2, dis+50);
        boxPoints[3] = TranlatePoint(p[0], v2, dis+50);
    }
    box.Add(boxPoints[0]);
    box.Add(boxPoints[1]);
    box.Add(boxPoints[2]);
    box.Add(boxPoints[3]);
    box.Add(p[0]);
    box.Add(p[1]);
    return true;
}

void MarchingCube::GroupByLine(Point3D startP, Point3D endP, Point3DArrayEx readyGroupPoints, Point3DArrayEx &oneGroupPoints, Point3DArrayEx &anthorGroupPoints)
{
    Vector3D v1, v2, v3, flagVector;
    int i,j, num;
    Point3D point;
    v1.Set(startP.m_x-endP.m_x, startP.m_y-endP.m_y, startP.m_z-endP.m_z);
    v1.Normalize();

    double a[3];
    readyGroupPoints.GetNormal(a);
    flagVector.Set(a);

    num = readyGroupPoints.size();
    for (i=0; i<num; i++)
    {
        point = readyGroupPoints.at(i);

        if (point == startP || point == endP)
        {
            continue;
        }

        v2.Set(point.m_x-endP.m_x, point.m_y-endP.m_y, point.m_z-endP.m_z);
        v2.Normalize();
        v3.CrossProduct(v1, v2);
        v3.Normalize();

        if (v3 != flagVector)
        {
            oneGroupPoints.Add(point);
        }
        else
        {
            anthorGroupPoints.Add(point);
        }
    }
}

void MarchingCube::ModifyEachBox(Point3DArrayEx &fBox, Point3DArrayEx &bBox, Point3D curve1Star, Point3D curve1End, Point3D curve2Star, Point3D curve2End)
{
    int i, j;
    double dis;
    Point3D fBoxs[4], bBoxs[4];
    Point3D upMidP[2], dowMinP[2];
    Vector3D v1, v2;
    v1.Set(curve1Star.m_x-curve1End.m_x, curve1Star.m_y-curve1End.m_y, curve1Star.m_z-curve1End.m_z);
    v1.Normalize();
    v2.Set(curve2Star.m_x-curve2End.m_x, curve2Star.m_y-curve2End.m_y, curve2Star.m_z-curve2End.m_z);
    v2.Normalize();

    for(i=0; i<4; i++)
    {
        fBoxs[i] = fBox.at(i);
        bBoxs[i] = bBox.at(i);
    }
    upMidP[0] = fBox.at(4);
    dowMinP[0] = fBox.at(5);
    upMidP[1] = bBox.at(4);
    dowMinP[1] = bBox.at(5);

    if (upMidP[0] != curve1Star && upMidP[1] == curve2Star)
    {
        dis = Relation3D::PToPDistance(upMidP[0], curve1Star);
        upMidP[1] = TranlatePoint(upMidP[1], v2, dis);
        bBoxs[0] = TranlatePoint(bBoxs[0], v2, dis);
        bBoxs[3] = TranlatePoint(bBoxs[3], v2, dis);
    }
    else if (upMidP[1] != curve2Star && upMidP[0] == curve1Star)
    {
        dis = Relation3D::PToPDistance(upMidP[1], curve2Star);
        upMidP[0] = TranlatePoint(upMidP[0], v1, dis);
        fBoxs[0] = TranlatePoint(fBoxs[0], v1, dis);
        fBoxs[3] = TranlatePoint(fBoxs[3], v1, dis);
    }

    if (dowMinP[0] != curve1End && dowMinP[1] == curve2End)
    {
        dis = Relation3D::PToPDistance(dowMinP[0], curve1End);
        dowMinP[1] = TranlatePoint(dowMinP[1], v2, -dis);
        bBoxs[1] = TranlatePoint(bBoxs[1], v2, -dis);
        bBoxs[2] = TranlatePoint(bBoxs[2], v2, -dis);
    }
    else
    {
        if (dowMinP[1] != curve2End && dowMinP[0] == curve1End)
        {
            dis = Relation3D::PToPDistance(dowMinP[1], curve2End);
            dowMinP[0] = TranlatePoint(dowMinP[0], v1, -dis);
            fBoxs[1] = TranlatePoint(fBoxs[1], v1, -dis);
            fBoxs[2] = TranlatePoint(fBoxs[2], v1, -dis);
        }
    }

    fBox.clear();
    bBox.clear();
    for (i=0; i<4; i++)
    {
        fBox.Add(fBoxs[i]);
        bBox.Add(bBoxs[i]);
    }
    fBox.Add(upMidP[0]);
    fBox.Add(dowMinP[0]);

    bBox.Add(upMidP[1]);
    bBox.Add(dowMinP[1]);
}


//void MarchingCube::CreateOpenCurveGrid(Point3D *point, PolyPoint3DArrayEx FrontPolyPointArray, PolyPoint3DArrayEx BackPolyPointArray, PolyPoint3DArrayEx& GridPointData)
//{
//
//    //测试计算的时间变量;
//    clock_t      start, finish;
//    Vector3D v1, v2, v;
//    int pointIndex[4];
//    //两点间的距离值;
//    double        Distance1, Distance2;
//    //耗费的时间,仅供测试;
//    double       duration;
//    int          IsInPolygon1, IsInPolygon2, num1, num2;
//    //前剖面曲线FPolyPoint,和后剖面曲线BPolyPoint;
//    Point3DArrayEx  FPolyPoint, BPolyPoint;
//    Point3D      *TempPoint1, *TempPoint2, p, startP[2], endP[2];
//    Point3D projectPoint1, projectPoint2, projectPoint3, projectPoint4, box[4];
//    Polygon3D    fBox, bBox;
//    Point3DArrayEx f, b;
//    Point3DArrayEx openCurve1, openCurve2;
//
//    //计算网格时所用到的一些中间点;
//    Point3D      CurPoint0, CurPoint1, CurPoint2, CurPoint3, SecPoint0, SecPoint1;
//    //前剖面多边形FrontPolygon后剖面多边形BackPolygon
//    Polygon3D    FrontPolygon, BackPolygon, tempPolygon;
//    int num = 0;
//    string str;
//
//    FPolyPoint = FrontPolyPointArray.at(0);
//    /*将原来的点抽取出来*/
//    for (int i=0; i<FPolyPoint.size(); i++)
//    {
//        CurPoint0 = FPolyPoint.at(i);
//
//        if (CurPoint0.m_ID != -1)
//        {
//            openCurve1.Add(CurPoint0);
//        }
//    }
//    if (openCurve1.size() == 0)
//    {
//        openCurve1 = FPolyPoint;
//    }
//
//    BPolyPoint = BackPolyPointArray.at(0);
//    /*将原来的点抽取出来*/
//    for (int i=0; i<BPolyPoint.size(); i++)
//    {
//        CurPoint0 = BPolyPoint.at(i);
//
//        if (CurPoint0.m_ID != -1)
//        {
//            openCurve2.Add(CurPoint0);
//        }
//    }
//    if (openCurve2.size() == 0)
//    {
//        openCurve2 = BPolyPoint;
//    }
//
//    v1.Set(point[0].m_x-point[1].m_x, point[0].m_y-point[1].m_y, point[0].m_z-point[1].m_z);
//    v1.Normalize();
//    v2.Set(point[2].m_x-point[1].m_x, point[2].m_y-point[1].m_y, point[2].m_z-point[1].m_z);
//    v2.Normalize();
//    v.CrossProduct(v1, v2);
//    v.Normalize();
//
//    pointIndex[0] = 0;
//    pointIndex[1] = openCurve1.size()-1;
//    pointIndex[2] = 0;
//    pointIndex[3] = openCurve2.size()-1;
//
//    //获取开曲线首尾点，进行方向判断，首尾点的方向向量应该保持一致
//    startP[0] = openCurve1.at(pointIndex[0]);
//    endP[0]   = openCurve1.at(pointIndex[1]);
//    startP[1] = openCurve2.at(pointIndex[2]);
//    endP[1]   = openCurve2.at(pointIndex[3]);
//
//    if (!IsSameDirection(endP[0], startP[0], endP[1], startP[1]))
//    {
//        openCurve2 = openCurve2.ReverseOrder();
//    }
//
//    //建立对应的开曲线包围盒
//    if (!CreateOpenCurveBox(openCurve1, pointIndex[0], pointIndex[1], f, v))
//    {
//        return;
//    }
//    if (!CreateOpenCurveBox(openCurve2, pointIndex[2], pointIndex[3], b, v))
//    {
//        return;
//    }
//
//
//    startP[0] = openCurve1.at(pointIndex[0]);
//    endP[0]   = openCurve1.at(pointIndex[1]);
//    startP[1] = openCurve2.at(pointIndex[2]);
//    endP[1]   = openCurve2.at(pointIndex[3]);
//
//    double dis1, dis2;
//
//    ModifyEachBox(f, b, startP[0], endP[0], startP[1], endP[1]);
//
//    for (int i=0; i<4; i++)
//    {
//        m_box[i] = f.at(i);
//        m_box[i+4] = b.at(i);
//    }
//
//    //点计数器,供测试用;
//    static int Count = 0;
//
//    Polygon3D *ptrPolygon = NULL;
//
//    //开启时间计数;
//    start = clock();
//
//    int times;
//
//    bool originFlag[2] ={true, true};
//
//    projectPoint1 = PointProjectLine(m_box[0], m_box[1], openCurve1.at(0));
//    projectPoint2 = PointProjectLine(m_box[0], m_box[1], openCurve1.at(openCurve1.size() - 1));
//    projectPoint3 = PointProjectLine(m_box[2], m_box[3], openCurve1.at(0));
//    projectPoint4 = PointProjectLine(m_box[2], m_box[3], openCurve1.at(openCurve1.size() - 1));
//
//    if (projectPoint1 == m_box[0])
//    {
//        originFlag[0] = false;
//    }
//
//    if (projectPoint2 == m_box[1])
//    {
//        originFlag[1] = false;
//    }
//
//    if (originFlag[0] && originFlag[1])
//    {
//        times = m_sizeJ / 3;
//    }
//    else if (!originFlag[0] && !originFlag[1])
//    {
//        times = m_sizeJ;
//    }
//    else if ((originFlag[0] && !originFlag[1]) || (!originFlag[0] && originFlag[1]))
//    {
//        times = m_sizeJ / 2;
//    }
//
//
//    //加密点之后的轮廓线fPolygon,bPolygon
//    Polygon3D fPolygon(openCurve1);
//    fPolygon.Add(f.at(5));
//    fPolygon.Add(f.at(2));
//    fPolygon.Add(f.at(3));
//    fPolygon.Add(f.at(4));
//    fPolygon.Add(openCurve1.at(0));
//    Polygon3D bPolygon(openCurve2);
//    bPolygon.Add(b.at(5));
//    bPolygon.Add(b.at(2));
//    bPolygon.Add(b.at(3));
//    bPolygon.Add(b.at(4));
//    bPolygon.Add(openCurve2.at(0));
//
//    //未加密点的轮廓线fPolygon1,bPolygon1
//    Polygon3D fPolygon1(FPolyPoint);
//    fPolygon1.Add(f.at(5));
//    fPolygon1.Add(f.at(2));
//    fPolygon1.Add(f.at(3));
//    fPolygon1.Add(f.at(4));
//    fPolygon1.Add(openCurve1.at(0));
//    Polygon3D bPolygon1(BPolyPoint);
//    bPolygon1.Add(b.at(5));
//    bPolygon1.Add(b.at(2));
//    bPolygon1.Add(b.at(3));
//    bPolygon1.Add(b.at(4));
//    bPolygon1.Add(openCurve2.at(0));
//
//
//    double n1[3], n2[3];
//    fPolygon.GetNormal(n1);
//    bPolygon.GetNormal(n2);
//
//    /*for (int i=3; i<3; i++)
//    {
//        n1[i] = -n1[i];
//
//    }*/
//    //计算前后两个剖面上的网格,前后对应的四个网格点组成一个立方体;
//    for (int i = 0; i < 2;  i++)
//    {
//        if (i == 0)
//        {
//            box[0] = m_box[0];
//            box[1] = m_box[1];
//            box[2] = m_box[2];
//            box[3] = m_box[3];
//            projectPoint1 = PointProjectLine(box[0], box[1], openCurve1.at(0));
//            projectPoint2 = PointProjectLine(box[0], box[1], openCurve1.at(openCurve1.size() - 1));
//            projectPoint3 = PointProjectLine(box[2], box[3], openCurve1.at(0));
//            projectPoint4 = PointProjectLine(box[2], box[3], openCurve1.at(openCurve1.size() - 1));
//
//        }
//        else
//        {
//            box[0] = m_box[4];
//            box[1] = m_box[5];
//            box[2] = m_box[6];
//            box[3] = m_box[7];
//            projectPoint1 = PointProjectLine(box[0], box[1], openCurve2.at(0));
//            projectPoint2 = PointProjectLine(box[0], box[1], openCurve2.at(openCurve2.size() - 1));
//            projectPoint3 = PointProjectLine(box[2], box[3], openCurve2.at(0));
//            projectPoint4 = PointProjectLine(box[2], box[3], openCurve2.at(openCurve2.size() - 1));
//        }
//
//        num = -1;
//        for (int t=0; t<3; t++)
//        {
//            if (t==0)
//            {
//                CurPoint0 = box[0];
//                CurPoint1 = projectPoint1;
//                CurPoint2 = projectPoint3;
//                CurPoint3 = box[3];
//
//                if (!originFlag[0])
//                {
//                    continue;
//                }
//                num ++;
//            }
//            if (t==1)
//            {
//                CurPoint0 = projectPoint1;
//                CurPoint1 = projectPoint2;
//                CurPoint2 = projectPoint4;
//                CurPoint3 = projectPoint3;
//                num ++;
//            }
//            if (t==2)
//            {
//                CurPoint0 = projectPoint2;
//                CurPoint1 = box[1];
//                CurPoint2 = box[2];
//                CurPoint3 = projectPoint4;
//
//                if (!originFlag[1])
//                {
//                    continue;
//                }
//                num ++;
//            }
//
//            for (int j=0; j<times+1; j++)
//            {
//                SecPoint0.m_x = (double)(CurPoint1.m_x - CurPoint0.m_x)/times * j + CurPoint0.m_x;
//                SecPoint0.m_y = (double)(CurPoint1.m_y - CurPoint0.m_y)/times * j + CurPoint0.m_y;
//                SecPoint0.m_z = (double)(CurPoint1.m_z - CurPoint0.m_z)/times * j + CurPoint0.m_z;
//
//                SecPoint1.m_x = (double)(CurPoint2.m_x - CurPoint3.m_x)/times * j + CurPoint3.m_x;
//                SecPoint1.m_y = (double)(CurPoint2.m_y - CurPoint3.m_y)/times * j + CurPoint3.m_y;
//                SecPoint1.m_z = (double)(CurPoint2.m_z - CurPoint3.m_z)/times * j + CurPoint3.m_z;
//
//                for (int k=0; k<m_sizeK+1; k++)
//                {
//                    GridPoint* gridPoint = &m_GridPoints[i][j+times*num][k];
//
//                    //获得当前立方体顶点的坐标值;
//                    gridPoint->point.m_x = (double)(SecPoint1.m_x - SecPoint0.m_x)/m_sizeK * k + SecPoint0.m_x;
//                    gridPoint->point.m_y = (double)(SecPoint1.m_y - SecPoint0.m_y)/m_sizeK * k + SecPoint0.m_y;
//                    gridPoint->point.m_z = (double)(SecPoint1.m_z - SecPoint0.m_z)/m_sizeK * k + SecPoint0.m_z;
//
//                    TempPoint1 = &gridPoint->point;
//
//                    //当立方体顶点在前剖面时;
//                    if (i == 0)
//                    {
//                        //判断点顶点是否在多边形内,IsInPolygon1为1表示在多边形边上,为2表示在多边形的内部,否则在多边形的外面;
//                        IsInPolygon1 = fPolygon.Contain(*TempPoint1);
//
//                        /*str.Format("%d,%d,%d,%d\n", i, j+times*num, k, IsInPolygon1);
//                        AfxOutputDebugString(str);*/
//
//
//                        //在多边形上，直接顶点权值赋值为0;
//                        if(IsInPolygon1 == 1)
//                        {
//                            gridPoint->val = 0;
//                        }
//                            //在多边形内部，则计算顶点到多边形上点的最短距离;
//                        else if (IsInPolygon1 == 2)
//                        {
//                            //计算点TempPoint到多边形上点的最短距离
//                            Distance1 = Distance2 = CalcDistance(fPolygon1.at(0), *TempPoint1);
//                            for (int index = 1; index < fPolygon1.size(); index ++)
//                            {
//                                TempPoint2 = &fPolygon1.at(index);
//                                Distance1 = CalcDistance(*TempPoint2, *TempPoint1);
//                                Distance2 = (Distance1 > Distance2)? Distance2: Distance1;
//                            }
//                            gridPoint->val = Distance2;
//
//                        }
//                            //在多边形外面，计算顶点多变形上点的最短距离，并取距离的负值；
//                        else
//                        {
//                            Distance1 = Distance2 = CalcDistance(fPolygon1.at(0), *TempPoint1);
//                            for (int index = 1; index < fPolygon1.size(); index ++)
//                            {
//                                TempPoint2 = &fPolygon1.at(index);
//                                Distance1 = CalcDistance(*TempPoint2, *TempPoint1);
//                                Distance2 = (Distance1 > Distance2)? Distance2: Distance1;
//                            }
//
//                            //与上次计算的权值比较，若绝对值大于当前值，则更改其权值；
//                            if (abs(gridPoint->val) >= Distance2) {
//                                gridPoint->val = Distance2;
//                                gridPoint->val = - gridPoint->val;
//                            }
//                                //否则，继续下一个顶点的计算;
//                            else
//                                continue;
//                        }
//                    }
//                        //当立方体顶点在后剖面时;
//                    else
//                    {
//
//                        IsInPolygon2 = bPolygon.Contain(*TempPoint1);
//
//                        /*str.Format("%d,%d,%d,%d\n", i, j+times*num, k, IsInPolygon2);
//                        AfxOutputDebugString(str);*/
//
//                        if (IsInPolygon2 == 1)
//                        {
//                            gridPoint->val = 0;
//                        }
//                        else if (IsInPolygon2 == 2)
//                        {
//                            Distance1 = Distance2 = CalcDistance(bPolygon1.at(0), *TempPoint1);
//                            for (int index = 1; index < bPolygon1.size(); index ++)
//                            {
//                                TempPoint2 = &bPolygon1.at(index);
//                                Distance1 = CalcDistance(*TempPoint2, *TempPoint1);
//                                Distance2 = (Distance1 > Distance2)? Distance2:Distance1;
//                            }
//                            gridPoint->val = Distance2;
//                        }
//                        else
//                        {
//                            Distance1 = Distance2 = CalcDistance(bPolygon1.at(0), *TempPoint1);
//                            for (int index = 1; index < bPolygon1.size(); index ++)
//                            {
//                                TempPoint2 = &bPolygon1.at(index);
//                                Distance1 = CalcDistance(*TempPoint2, *TempPoint1);
//                                Distance2 = (Distance1 > Distance2)? Distance2:Distance1;
//                            }
//                            if (abs(gridPoint->val) >= Distance2) {
//                                gridPoint->val = Distance2;
//                                gridPoint->val = - gridPoint->val;
//                            }
//                            else
//                                continue;
//
//                        }
//                    }
//                }
//
//            }
//        }
//
//    }
//
//    ConnectGrids();
//    CreateSurface();
//    finish = clock();
//    duration = (double)(finish - start) / CLOCKS_PER_SEC;
////    str.Format("所用时间%f", duration);
////    AfxOutputDebugString(str);
//    SaveGridPoint(GridPointData);
//}

void MarchingCube::GenrateSection(double ratio)
{
    Vector3D normal;
    Vector3D v1, v2;
    Point3D midPoint;
    double dis;
    midPoint.Set((m_secPoint[0].m_x-m_secPoint[4].m_x)/2+m_secPoint[4].m_x, (m_secPoint[0].m_y-m_secPoint[4].m_y)/2+m_secPoint[4].m_y, (m_secPoint[0].m_z-m_secPoint[4].m_z)/2+m_secPoint[4].m_z);
    dis = Relation3D::PToPDistance(m_secPoint[0], m_secPoint[4]);

    v1.m_x = midPoint.m_x - m_secPoint[0].m_x;
    v1.m_y = midPoint.m_y - m_secPoint[0].m_y;
    v1.m_z = midPoint.m_z - m_secPoint[0].m_z;
    v1.Normalize();

    v2.m_x = midPoint.m_x - m_secPoint[4].m_x;
    v2.m_y = midPoint.m_y - m_secPoint[4].m_y;
    v2.m_z = midPoint.m_z - m_secPoint[4].m_z;
    v2.Normalize();

    m_newSectionPoint[0] = TranlatePoint(m_secPoint[0], v1, ratio*dis);
    m_newSectionPoint[1] = TranlatePoint(m_secPoint[1], v1, ratio*dis);
    m_newSectionPoint[2] = TranlatePoint(m_secPoint[2], v1, ratio*dis);
    m_newSectionPoint[3] = TranlatePoint(m_secPoint[3], v1, ratio*dis);

    m_newSectionPoint[4] = TranlatePoint(m_secPoint[4], v2, ratio*dis);
    m_newSectionPoint[5] = TranlatePoint(m_secPoint[5], v2, ratio*dis);
    m_newSectionPoint[6] = TranlatePoint(m_secPoint[6], v2, ratio*dis);
    m_newSectionPoint[7] = TranlatePoint(m_secPoint[7], v2, ratio*dis);
}

void MarchingCube::ProjectionContours(PolyPoint3DArrayEx &fContoursArray, PolyPoint3DArrayEx &bContoursArray)
{
    int i,j, numOfContours, numOfPoints;
    fContoursArray.clear();
    bContoursArray.clear();

    Point3DArrayEx tempContours;
    Point3DArrayEx projectContour;
    Vector3D v1, v2, N;
    v1.m_x = m_newSectionPoint[0].m_x - m_newSectionPoint[1].m_x;
    v1.m_y = m_newSectionPoint[0].m_y - m_newSectionPoint[1].m_y;
    v1.m_z = m_newSectionPoint[0].m_z - m_newSectionPoint[1].m_z;

    v2.m_x = m_newSectionPoint[2].m_x - m_newSectionPoint[1].m_x;
    v2.m_y = m_newSectionPoint[2].m_y - m_newSectionPoint[1].m_y;
    v2.m_z = m_newSectionPoint[2].m_z - m_newSectionPoint[1].m_z;

    N.CrossProduct(v2, v1);
    N.Normalize();

    numOfContours = m_fPolyPointArray->size();
    for (i=0; i<numOfContours; i++)
    {
        projectContour.clear();
        tempContours = m_fPolyPointArray->at(i);
        GetProjectPointsSet(N, m_newSectionPoint[0], tempContours, projectContour);
        fContoursArray.Add(projectContour);
    }

    numOfContours = m_bPolyPointArray->size();
    for (i=0; i<numOfContours; i++)
    {
        projectContour.clear();
        tempContours = m_bPolyPointArray->at(i);
        GetProjectPointsSet(N, m_newSectionPoint[4], tempContours, projectContour);
        bContoursArray.Add(projectContour);
    }
}

void MarchingCube::RadomRay(Point3D rayStartPoint, Point3D& rayEndPoint)
{

}

int MarchingCube::PointInPolygon(Point3D& point, Point3DArrayEx &pointArray)
{
    Point3D *pt;
    int  size = pointArray.size();
    pt = new Point3D[size];
    for (int i=0; i<size; i++)
    {
        pt[i] = pointArray.at(i);
    }
    if (PointInPolygon(point, pt, size, true))
        return 2;
    else
        return 0;
    delete[]pt;
}

bool MarchingCube::PointInPolygon(Point3D& point, Point3D* pt, int size, bool bSideIsIn)
{
    if(!pt || (size <= 1))
        return false;
    if(pt[0] == pt[size-1])	//去掉末尾的重复点
        size--;

    Line3D line;
    double p[3];
    point.Get(p);
    if(size == 2)
    {
        if(bSideIsIn)
        {
            line.Set(pt[0], pt[1]);
            return  Relation3D::PointInLine(line, p);
        }
        else
            return false;
    }
    int i;

    //单独处理压线的情况
    int iMiddle = size;
    if(bSideIsIn)	//压线算里
    {
        for(i = 0; i < size; i++)
        {
            line.Set(pt[i], pt[(i+1)%size]);
            if(Relation3D::PointInLine(line, p))	//并且在某条边线上
            {
                return true;
            }
            else if(iMiddle == size)
                iMiddle = i;
        }
    }

    //正常情况：找到点不在的第一条线
    if(!bSideIsIn)	//如果压线，已经在上面计算过了
    {
        for(i = 0; i < size; i++)
        {
            line.Set(pt[i], pt[(i+1)%size]);
            if(!Relation3D::PointInLine(line, p))
            {
                iMiddle = i;
                break;
            }
        }
    }
    if(iMiddle == size)	//多边形所有的点缩成一个点
        return true;
    Point3D ptMiddle;
    ptMiddle.m_x = (pt[iMiddle].m_x + pt[(iMiddle+1)%size].m_x)/2;
    ptMiddle.m_y = (pt[iMiddle].m_y + pt[(iMiddle+1)%size].m_y)/2;
    ptMiddle.m_z = (pt[iMiddle].m_z + pt[(iMiddle+1)%size].m_z)/2;
    int count = 1;
    for(i = 0; i < size; i++)
    {
        if(i != iMiddle)
        {
            if(LineRadial(pt[i], pt[(i+1)%size], point, ptMiddle, false))
                count++;
        }
    }
    return (count%2 == 1);
}



bool MarchingCube::LineRadial(Point3D& A, Point3D& B, Point3D& M, Point3D& N, bool bTestInPlanar)
{
    if((A == B) || (M == N))	//线段或者射线为空，则不认为会相交
        return false;
    if((A == M) || (A == N) || (B == M) || (B == N))	//搭在一起，相交
        return true;
    //if(bTestInPlanar)
    //{
    //	Point3D pt[4];
    //	pt[0] = A;
    //	pt[1] = B;
    //	pt[2] = M;
    //	pt[3] = N;
    //	if(!IsSamePlanar(pt, 4, NULL))	//不共面，不可能相交
    //		return false;
    //}
    Point3D CP1, CP2;
    Vector3D v1, v2, v, u;
    bool isNoEqualToZero1, isNoEqualToZero2;
    v1.Set(M.m_x-A.m_x, M.m_y-A.m_y, M.m_z-A.m_z);
    v2.Set(N.m_x-A.m_x, N.m_y-A.m_y, N.m_z-A.m_z);
    Vector3D::CrossProduct(v1, v2, v);
    isNoEqualToZero1 = Vector3D::SquareLength(v.m_x, v.m_y, v.m_z);
    v.Normalize();
    CP1 = v;

    v1.Set(M.m_x-B.m_x, M.m_y-B.m_y, M.m_z-B.m_z);
    v2.Set(N.m_x-B.m_x, N.m_y-B.m_y, N.m_z-B.m_z);
    Vector3D::CrossProduct(v1, v2, u);
    isNoEqualToZero2 = Vector3D::SquareLength(u.m_x, u.m_y, u.m_z);
    u.Normalize();
    CP2 = u;

    //CrossProduct(A, M, N, CP1);
    //CrossProduct(B, M, N, CP2);
    /*CP1.Normalize();
    CP2.Normalize();*/
    if(isNoEqualToZero1 && isNoEqualToZero2 && IsSameDirection(v, u))	//相等，则AB在MN的同侧，肯定没有交点
        return false;
    //if(!CP1.IsOrigin(false) && !CP2.IsOrigin(false) && CP1.IsSameDirect(CP2))	//相等，则AB在MN的同侧，肯定没有交点
    //	return false;
    //CrossProduct(M, A, B, CP1);
    v1.Set(A.m_x-M.m_x, A.m_y-M.m_y, A.m_z-M.m_z);
    v2.Set(B.m_x-M.m_x, B.m_y-M.m_y, B.m_z-M.m_z);
    Vector3D::CrossProduct(v1, v2, v);
    isNoEqualToZero1 = Vector3D::SquareLength(v.m_x, v.m_y, v.m_z);
    v.Normalize();
    CP1 = v;
    //CrossProduct(N, A, B, CP2);
    v1.Set(A.m_x-N.m_x, A.m_y-N.m_y, A.m_z-N.m_z);
    v2.Set(B.m_x-N.m_x, B.m_y-N.m_y, B.m_z-N.m_z);
    Vector3D::CrossProduct(v1, v2, u);
    isNoEqualToZero2 = Vector3D::SquareLength(u.m_x, u.m_y, u.m_z);
    u.Normalize();
    CP2 = u;
    /*CP1.Normalize();
    CP2.Normalize();*/
    //if(!CP1.IsOrigin(false) && !CP2.IsOrigin(false) && !CP1.IsSameDirect(CP2))	//不相等，说明MN在AB两侧。//再根据上面的判断，AB已经在MN两侧了，说明线段AB、MN相交，则线段AB与射线MN肯定相交
    //	return true;
    if(isNoEqualToZero1 && isNoEqualToZero2 && !IsSameDirection(v, u))	//不相等，说明MN在AB两侧。//再根据上面的判断，AB已经在MN两侧了，说明线段AB、MN相交，则线段AB与射线MN肯定相交
        return true;

    //  判断是否是“/--”这种情况
    double d1 = Relation3D::PToLDistance(A, B, M);
    if(fabs(d1) < THRESHOLD10)	//M在线段AB上，刚刚搭上，相交
        return true;
    double d2 = Relation3D::PToLDistance(A, B, N);
    return (d1 > d2);	//M离的距离更远，则相交
}


void MarchingCube::ReverseOrder(Point3DArrayEx& pointArray)
{
    int        i,num;
    Point3D    point;
    Point3DArrayEx tempPointArray;

    num= pointArray.size();
    for(i=num;i>0;)
    {
        i--;
        point = pointArray.at(i);
        tempPointArray.Add(point);
    }
    tempPointArray.m_attr1 =pointArray.m_attr1;
    tempPointArray.m_attr2 =pointArray.m_attr2;
    tempPointArray.m_strID =pointArray.m_strID;
    tempPointArray.m_strSubID =pointArray.m_strSubID;
    tempPointArray.m_ID =pointArray.m_ID;
    pointArray = tempPointArray;
}
