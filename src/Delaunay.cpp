//
// Created by 谭文波 on 2018/9/28.
//

#include "Delaunay.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
using namespace std;

#define max(a, b)  (((a) > (b)) ? (a) : (b))

#define random(a,b) rand() / (float)30

Delaunay::Delaunay()
{
    //传入的数组是连续的，所以赋值的时候要注意，num应该是要/3



}

Delaunay::~Delaunay()
{
    //析构函数
}


////void Delaunay::paintEvent(QPaintEvent *)
//{
//    QPainter painter(this);
//    int n = 100;
//    for (int i = 1;i <= n;i++)
//    {
//        Vertex[i].x = random(100, 500);
//        Vertex[i].y = random(100, 500);
//        cout << "X :" << Vertex[i].x << endl;
//        cout << "Y :" << Vertex[i].y << endl;
//
//    }
//    HowMany = FvsTrianglate(n);
//
//    for(int i = 1;i <= HowMany;i++)
//    {
//        painter.drawLine(QPointF(Vertex[Triangle[i].vv0].x, Vertex[Triangle[i].vv0].y),
//                         QPointF(Vertex[Triangle[i].vv1].x, Vertex[Triangle[i].vv1].y));
//        painter.drawLine(QPointF(Vertex[Triangle[i].vv1].x, Vertex[Triangle[i].vv1].y),
//                         QPointF(Vertex[Triangle[i].vv2].x, Vertex[Triangle[i].vv2].y));
//        painter.drawLine(QPointF(Vertex[Triangle[i].vv2].x, Vertex[Triangle[i].vv2].y),
//                         QPointF(Vertex[Triangle[i].vv0].x, Vertex[Triangle[i].vv0].y));
//    }


//}

void Delaunay::Init(VERTEX *vertex, int num)
{
    for(int i = 1; i <= num; i++)
    {
        Vertex[i] = vertex[i];
        cout<<"the vertex "<<i;
        cout<<" x:"<<Vertex[i].x;
        cout<<" y:"<<Vertex[i].y;
        cout<<" z:"<<Vertex[i].z<<endl;
    }
    //多少个三角形
    HowMany = FvsTrianglate(num);
    cout<<"howmany: "<<HowMany<<endl;
}

CIRCLE Delaunay::FvsCenter(float x1, float y1, float x2, float y2, float x3, float y3)
{
    CIRCLE Circle;
    float eps;   //一个极小值，epsilon，计算机所能识别的最小非零浮点数，易步西农
    float m1;
    float m2;
    float mx1;
    float mx2;
    float my1;
    float my2;
    float dx;
    float dy;
    float rsqr;
    float drsqr;
    float xc, yc, r;
    eps = 0.000001;

    if (fabs(y2 - y1) < eps)   //如果y2和y1在同一直线上
    {
        m2 = -(x3 - x2) / (y3 - y2);	//m2是p2和p3连线的斜率
        mx2 = (x2 + x3) / 2;			//mx2和my2是求p2和p3两点连线的中点坐标。
        my2 = (y2 + y3) / 2;

        xc = (x2 + x1) / 2;			//圆心的x坐标，因为y2和y1在同一直线上，则圆心的x坐标必在p1和p2的中点上。
        yc = m2 * (xc - mx2) + my2;   //圆心的y坐标，是斜率公式的变形，因为垂线的斜率是原来的-1/m2
    }
    else if (fabs(y3 - y2) < eps)		//同上
    {
        m1 = -(x2 - x1) / (y2 - y1);
        mx1 = (x1 + x2) / 2;
        my1 = (y1 + y2) / 2;
        xc = (x3 + x2) / 2;
        yc = m1 * ((xc)-mx1) + my1;
    }
    else                                   //这个的话就不是上面那种特殊情况。
    {
        m1 = -(x2 - x1) / (y2 - y1);		//分别求两直线的斜率
        m2 = -(x3 - x2) / (y3 - y2);
        mx1 = (x1 + x2) / 2;				//求两直线的中点坐标
        mx2 = (x2 + x3) / 2;
        my1 = (y1 + y2) / 2;
        my2 = (y2 + y3) / 2;
        xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);		//
        yc = m1 * ((xc)-mx1) + my1;
    }

    dx = x2 - (xc);			//任意点到圆心的距离都是一样的
    dy = y2 - (yc);
    rsqr = dx * dx + dy * dy;	//圆的半径的平方
    r = sqrt(rsqr);
    Circle.r = r;
    Circle.xc = xc;
    Circle.yc = yc;
    return Circle;
}
//判断一个点是否在圆内，三点确定一个圆，x1,x2,x3是三角形的三个点，xp,yp是其他点。
bool Delaunay::FvsTriangleInCircle(float xp, float yp, float x1, float y1, float x2, float y2, float x3, float y3, float *xc, float *yc, float *r)
{
    bool  Ret;

    float eps;   //一个极小值，epsilon，计算机所能识别的最小非零浮点数，易步西农
    float m1;
    float m2;
    float mx1;
    float mx2;
    float my1;
    float my2;
    float dx;
    float dy;
    float rsqr;
    float drsqr;

    eps = 0.000001;
    Ret = false;
    //fabs求浮点数的绝对值
    if (fabs(y1 - y2) < eps && fabs(y2 - y3) < eps)	//如果y1、y2，y3在同一直线上，返回false
    {
        printf("INCIRCUM - F - Points are coincident !\n");
        return Ret;
    }

    if (fabs(y2 - y1) < eps)   //如果y2和y1在同一直线上
    {
        m2 = -(x3 - x2) / (y3 - y2);	//m2是p2和p3连线的斜率
        mx2 = (x2 + x3) / 2;			//mx2和my2是求p2和p3两点连线的中点坐标。
        my2 = (y2 + y3) / 2;

        *xc = (x2 + x1) / 2;			//圆心的x坐标，因为y2和y1在同一直线上，则圆心的x坐标必在p1和p2的中点上。
        *yc = m2 * (*xc - mx2) + my2;   //圆心的y坐标，是斜率公式的变形，因为垂线的斜率是原来的-1/m2
    }
    else if (fabs(y3 - y2) < eps)		//同上
    {
        m1 = -(x2 - x1) / (y2 - y1);
        mx1 = (x1 + x2) / 2;
        my1 = (y1 + y2) / 2;
        *xc = (x3 + x2) / 2;
        *yc = m1 * ((*xc) - mx1) + my1;
    }
    else                                   //这个的话就不是上面那种特殊情况。
    {
        m1 = -(x2 - x1) / (y2 - y1);		//分别求两直线的斜率
        m2 = -(x3 - x2) / (y3 - y2);
        mx1 = (x1 + x2) / 2;				//求两直线的中点坐标
        mx2 = (x2 + x3) / 2;
        my1 = (y1 + y2) / 2;
        my2 = (y2 + y3) / 2;
        *xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);		//某个高级公式求X的坐标，
        *yc = m1 * ((*xc) - mx1) + my1;
    }

    dx = x2 - (*xc);			//任意点到圆心的距离都是一样的
    dy = y2 - (*yc);
    rsqr = dx * dx + dy * dy;	//圆的半径的平方
    *r = sqrt(rsqr);
    dx = xp - (*xc);			//p点到圆心的距离
    dy = yp - (*yc);
    drsqr = dx * dx + dy * dy;

    /*    if ( drsqr <= rsqr )
    TRACE("true\n");
    else
    TRACE("false\n");
    */
    if (drsqr <= rsqr)	//如果点到圆心的距离小于半径则返回真，说明点在圆内。
        return true;
    else
        return false;
}


//判断三点是否共线
int Delaunay::FvsTriangleWhichSide(float xp, float yp, float x1, float y1, float x2, float y2)
{
    //equation 相等。
    float equation;
    /*
    (y2 - y1)   (y3 - y2)
    --------- = --------- ,
    (x2 - x1)   (x3 - x2)*/

    //这两个每个的乘积都是0则共线。
    equation = ((yp - y1) * (x2 - x1)) - ((y2 - y1) * (xp - x1));

    if (equation > 0)
        return -1;
    else if (equation == 0)
        return 0;
    else
        return 1;

}


int Delaunay::FvsTrianglate(int nvert)  //nvert顶点数
{
    //	bool         Complete[MAX_TRIANGLE]; 完整的三角形？
    bool         Complete[MAX_TRIANGLE + 1];
    //    float          Edges[2][3000]; /// ?
    long          Edges[3][3001]; //   边的个数，Edges[1]代表边的起点，Edges[2]代表边的终点
    long          Nedge; //   边的个数

    // For Super Triangle，创建一个超级三角形。
    float   xmin;	//x的最小坐标值，下同
    float   xmax;
    float   ymin;
    float   ymax;

    float   xmid;	//这个暂时不知
    float   ymid;

    float dx;		//等于xmax-xmin，下同
    float dy;
    float dmax;	//math.max(dx,dy)
    // General Variables
    int   i;
    int   j;
    int   k;
    int   ntri;
    float   xc;	//圆心和半径
    float   yc;
    float   r;
    bool  inc;

    /// ? 添加布尔变量的初始化过程，将三角形数组全部初始化为false
    for (i = 1; i <= MAX_TRIANGLE; i++)
        Complete[i] = false;

    // Find the maximum and minimum vertex bounds.  找到最大最小的边界
    // This is to allow calculation of the bounding triangle
    xmin = Vertex[1].x;

    ymin = Vertex[1].y;
    xmax = xmin;
    ymax = ymin;
    for (i = 2; i <= nvert; i++)     //找到当前点里，最大最小的XY坐标
    {
        cout << "X :" << Vertex[i].x << endl;
        if (Vertex[i].x < xmin)      xmin = Vertex[i].x;
        if (Vertex[i].x > xmax)      xmax = Vertex[i].x;
        if (Vertex[i].y < ymin)      ymin = Vertex[i].y;
        if (Vertex[i].y > ymax)      ymax = Vertex[i].y;
    }

    dx = xmax - xmin;
    dy = ymax - ymin;
    if (dx > dy)
        dmax = dx;
    else
        dmax = dy;
    xmid = (xmax + xmin) / 2;      //super三角形的中点。
    ymid = (ymax + ymin) / 2;

    // Set up the supertriangle
    // This is a triangle which encompasses all the sample points.
    // The supertriangle coordinates are added to the end of the
    // vertex list. The supertriangle is the first triangle in
    // the triangle list.
    //设置一个超级三角形，里面包含了所有的点。超级三角形的点添加到顶点列
    //表的后面，超级三角形是三角列表里的第一个三角形


    //这些就是超级三角的顶点，注意是nvert再+1，所以超级三角的顶点放在顶点列表的后面
    Vertex[nvert + 1].x = xmid - 2 * (float)dmax;		//这几个点构成一个倒正三角形，一条边平行于X轴。
    Vertex[nvert + 1].y = ymid - (float)dmax;
    Vertex[nvert + 2].x = xmid;
    Vertex[nvert + 2].y = ymid + 2 * (float)dmax;
    Vertex[nvert + 3].x = xmid + 2 * (float)dmax;
    Vertex[nvert + 3].y = ymid - (float)dmax;
    Triangle[1].vv0 = nvert + 1;			//这里为什么是顶点数+1？
    Triangle[1].vv1 = nvert + 2;			//因为三角就是一些顶点组成，而Vertex[nvert]就能找到相应顶点的坐标值，所以直接用顶点数组的下标值来表示
    Triangle[1].vv2 = nvert + 3;
    Complete[1] = false;
    ntri = 1;			//三角的数量

    // Include each point one at a time into the existing mesh
    for (i = 1; i <= nvert; i++)        //循环N顶点次
    {
        Nedge = 0;			//边的数量
        /* Set up the edge buffer.    边缓存区，用来放不是三角形的边
        'If the point (Vertex(i).x,Vertex(i).y) lies inside the circumcircle then the  如果这个条线在这个圆里那么这三角的三边都放到边缓存区里去。
        'three edges of that triangle are added to the edge buffer.
        */
        j = 0;
        do
        {
            j = j + 1;
            if (Complete[j] != true)					//如果j三角形不是Delaunay三角则进行下述判断
            {
                inc = FvsTriangleInCircle(Vertex[i].x, Vertex[i].y,		//i点和j三角的匹配情况
                                          Vertex[Triangle[j].vv0].x,
                                          Vertex[Triangle[j].vv0].y,
                                          Vertex[Triangle[j].vv1].x,
                                          Vertex[Triangle[j].vv1].y,
                                          Vertex[Triangle[j].vv2].x,
                                          Vertex[Triangle[j].vv2].y,
                                          &xc, &yc, &r);				//圆的属性为地址引用，所以数值等于该函数返回的指针值

                //'Include this if points are sorted by X
                //'If (xc + r) < Vertex(i).x Then
                //    'complete(j) = True
                //'Else


                if (inc == true)	//如果点在圆内，则三角形减少一个，将被测三角形的三边加入边集（buffer edges）。
                {
                    Edges[1][Nedge + 1] = Triangle[j].vv0;				//Edges的1和2分别代表线段的起点和终点，自己想象
                    Edges[2][Nedge + 1] = Triangle[j].vv1;
                    Edges[1][Nedge + 2] = Triangle[j].vv1;
                    Edges[2][Nedge + 2] = Triangle[j].vv2;
                    Edges[1][Nedge + 3] = Triangle[j].vv2;
                    Edges[2][Nedge + 3] = Triangle[j].vv0;
                    Nedge = Nedge + 3;									//因为三角被拆分，则边数加3
                    Triangle[j].vv0 = Triangle[ntri].vv0;
                    Triangle[j].vv1 = Triangle[ntri].vv1;
                    Triangle[j].vv2 = Triangle[ntri].vv2;

                    Complete[j] = Complete[ntri];    //更新j的三角状态
                    //TRACE("j = %d, ntri = %d \n", j, ntri);
                    j = j - 1;
                    ntri = ntri - 1;			//拆了一个三角-1
                }
                //}
            }
        } while (j < ntri);	//测试数量小于三角

        //'Tag multiple edges			标记多个边缘
        //'Note: if all triangles are specified anticlockwise then all
        //'interior edges are opposite pointing in direction.
        //如果所有三角是逆时针剖分，那么所有内部边都是相反的
        //此循环的作用是消除重复边
        for (j = 1; j <= Nedge - 1; j++)
        {
            // 当表达式有多种运算符时，先处理算术运算符，接着处理比较运算符，然后再处理逻辑运算符。 /// ?
            // If Not Edges(1, j) = 0 And Not Edges(2, j) = 0 Then
            if (!(Edges[1][j] == 0) && !(Edges[2][j] == 0))			//如果该边存在，那么
            {
                //TRACE("enter then. ij=%ld-%ld\n", i, j);

                for (k = j + 1; k <= Nedge; k++)				//j的邻边
                {
                    if (!(Edges[1][k] == 0) && !(Edges[2][k] == 0))		//如果j的邻边K也存在
                    {
                        if (Edges[1][j] == Edges[2][k])				//如果J边的起点和K边的终点相等
                        {
                            if (Edges[2][j] == Edges[1][k])			//如果J边的终点和K边的起点相等
                            {
                                Edges[1][j] = 0;					//消除这两个边？
                                Edges[2][j] = 0;
                                Edges[1][k] = 0;
                                Edges[2][k] = 0;
                            }
                        }
                    }
                }
            }
        }

        //'Form new triangles for the current point  对于来自新三角形的点
        //'Skipping over any tagged edges.		跳过所有被标记过的边
        //'All edges are arranged in clockwise order.		所有边都按顺时针排列


        //讲所有的点与超级三角所生成的点连成三角形
        for (j = 1; j <= Nedge; j++)
        {
            if (!(Edges[1][j] == 0) && !(Edges[2][j] == 0))			//如果是没有被标记的边
            {
                ntri = ntri + 1;						//三角+1？
                Triangle[ntri].vv0 = Edges[1][j];		//将没标记的边连成三角
                Triangle[ntri].vv1 = Edges[2][j];
                Triangle[ntri].vv2 = i;					//这个i是什么？  循环的一开始
                Complete[ntri] = false;

            }
        }
    }

    //'Remove triangles with supertriangle vertices   移除超级三角顶点
    //'These are triangles which have a vertex number greater than NVERT		  这些三角的顶点数大于NVERT

    i = 0;
    do
    {
        //TRACE("nvert:%d ", nvert);
        i = i + 1;
        if (Triangle[i].vv0 > nvert ||					//大于nvert的点要
            Triangle[i].vv1 > nvert ||
            Triangle[i].vv2 > nvert)
        {

            Triangle[i].vv0 = Triangle[ntri].vv0;				//删除
            Triangle[i].vv1 = Triangle[ntri].vv1;
            Triangle[i].vv2 = Triangle[ntri].vv2;
            i = i - 1;
            ntri = ntri - 1;

        }
    } while (i < ntri);

    return ntri;						//返回三角的数量

}



