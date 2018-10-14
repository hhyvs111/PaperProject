//
// Created by 谭文波 on 2018/9/28.
//

#ifndef LEARNOPENGL_DELAUNAY_H
#define LEARNOPENGL_DELAUNAY_H
#define Type float

#define MAX_VERTEX   500
#define MAX_TRIANGLE 1000
#define MAX_CIRCLE 1000
#define MAX_NUM 1000

// 定义结构VERTEX表达一个点的坐标
typedef   struct   tagVERTEX
{
    float   x;  // x坐标
    float   y;  // y坐标
    float   z;  // z坐标
}VERTEX;

// 定义结构TRIANGLE表达一个三角形
typedef   struct   tagTRIANGLE
{
    int     vv0;
    int     vv1;
    int     vv2;
}TRIANGLE;

typedef   struct tagCIRCLE
{
    float xc;     //圆心
    float yc;
    float r;		//半径
}CIRCLE;

class Delaunay
{
//先把数据都改成这个公共可访问都吧，懒得做这个依赖性了。
public:
    int tPoints;  //定义点？
    int HowMany;		//有多少个三角形这个
    TRIANGLE Triangle[501];		//三角数组
    VERTEX Vertex[501];			//点的个数
    CIRCLE Circle[501];			//外接圆的个数


public:
    //构造函数放入一堆点，让其剖分
    Delaunay(VERTEX vertex[],int num);

    int FvsTrianglate(int nvert);
    //判断哪条边？
    int FvsTriangleWhichSide(float xp, float yp, float x1, float y1, float x2, float y2);
    //判断三角形是否在圆内

    CIRCLE FvsCenter(float x1, float y1, float x2, float y2, float x3, float y3);

    bool FvsTriangleInCircle(float xp, float yp, float x1, float y1, float x2, float y2, float x3, float y3, float *xc, float *yc, float* r);
    //Delaunay();
    virtual ~Delaunay();
};

#endif //LEARNOPENGL_DELAUNAY_H
