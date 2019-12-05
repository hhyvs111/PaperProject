//
// Created by 谭文波 on 2019/12/1.
//

#ifndef LEARNOPENGL_MARCHINGCUBES_H
#define LEARNOPENGL_MARCHINGCUBES_H

#include <list>
#include <vector>
#include<float.h>
#include <time.h>
#include "common.h"

//#include "TinCrossSolid.h"
//网格上的点
struct GridPoint {
    Point3D point;
    double val;
    int isInPolygon;
    GridPoint(){
        val = -DBL_MAX;
        isInPolygon = -1;
    }
};

//struct CubeEdge
//{
//	GridPoint *m_point[2];
//	int        m_Id;
//	bool       m_isProcess;
//};

//三角形结构体
struct TriangleEx {
    Point3D point[3];   /*三角形的三个点坐标*/
    Point3D normal;     /*三角形法向量*/


    void Debug(){
        point[0].Debug();
        point[1].Debug();
        point[2].Debug();
    }
};

//立方格结构
struct Cube {
    GridPoint*point[8];       /* 立方格的八个顶点*/
    bool      hasFacet;       /* 立方体是否有三角面片，有为TRUE，否则为false*/
    Cube()
    {
        hasFacet = false;
    }
};

class MarchingCube{
public:
    int m_sizeI;                                     /*剖面步长值*/
    int m_sizeJ;                                     /*长分个数值*/
    int m_sizeK;                                     /*宽分个数*/
    //int                      m_NumberDivide;
    float                    m_IsoLevel;            /*等值面值*/
    GridPoint***             m_GridPoints;          /*网格点组*/
    Cube***                  m_Cubes;               /*立方格组*/
    std::list<TriangleEx>    m_TriangleEx;	        /*三角形组*/

    Point3D                  m_secPoint[8];         /*打开sec文件一个剖面边框的三个点坐标*/
    Point3D           m_newSectionPoint[8];         /* 插入的两个剖面的八个角点*/
    Point3D                  m_box[8];              /*包围盒的立方体的八个角点*/


    PolyPoint3DArrayEx       *m_fPolyPointArray;
    PolyPoint3DArrayEx       *m_bPolyPointArray;
    PolyPoint3DArrayEx        m_fNewContours;
    PolyPoint3DArrayEx        m_bNewContours;

    float   m_sumTime;
    int     m_sumTri;

public:
    MarchingCube();

    //--------------------------------------------------------------------
    //函数名称：MarchingCube
    //函数功能：带参的构造函数
    //函数输入：
    //函数返回：无
    //--------------------------------------------------------------------
    MarchingCube(int sizeI, int sizeJ, int sizeK, float isoLevel);
    ~MarchingCube(void);


    void SetSizeI(int i){
        m_sizeI = i;
    }

    void SetSizeJ(int j)
    {
        m_sizeJ = j;
    }

    void SetSizeK(int k)
    {
        m_sizeK = k;
    }


    int CreateSurface(void);

    //--------------------------------------------------------------------
    //函数名称：SaveTriangleToSurface
    //函数功能：将三角面片保存到三角形面片组中去;
    //函数输入：tempSurface为三角形面片组，surfaceNormals为对应三角形组的法向量；
    //函数返回：无
    //--------------------------------------------------------------------
    void SaveTriangleToSurface(Triangle3DArrayEx& tempSurface, Point3DArrayEx& surfaceNormals);

    //--------------------------------------------------------------------
    //函数名称：GetTriangles
    //函数功能：获取链表中的三角形
    //函数输入：无
    //函数返回：无
    //--------------------------------------------------------------------
    std::list<TriangleEx>& GetTriangles(void);

    //--------------------------------------------------------------------
    //函数名称：ConnectGrids
    //函数功能：将每八个网格点对应链接到立方体当中去
    //函数输入：无
    //函数返回：无
    //--------------------------------------------------------------------
    void ConnectGrids(void);

    //--------------------------------------------------------------------
    //函数名称：AllocateMerroy
    //函数功能：分配存储立方体顶点的空间
    //函数输入：无
    //函数返回：无
    //--------------------------------------------------------------------
    void AllocateMerroy(void);

    //--------------------------------------------------------------------
    //函数名称：ShowGridCell
    //函数功能：显示构造好的立方体，此函数供测试时参考
    //函数输入：无
    //函数返回：无
    //--------------------------------------------------------------------
    static void ShowGridCell(PolyPoint3DArrayEx polyPoint);

    //函数名称：CreateGrid
    //函数功能：构造立方体顶点，并计算每个顶点的权值
    //函数输入：
    //函数返回：无
    //--------------------------------------------------------------------
    void CreateGrid(Point3D*, PolyPoint3DArrayEx, PolyPoint3DArrayEx, PolyPoint3DArrayEx& );

    //--------------------------------------------------------------------
    //函数名称：GenerateSurface
    //函数功能：根据当前立方体的顶点权值和等值面值，计算立方体相应边的交点
    //函数输入：指向一个立方体的指针，等值面值
    //函数返回：当前立方体中三角面片的个数
    //--------------------------------------------------------------------
    int GenerateSurface(/*const*/ Cube* cube, float isoLevel, Line3DArrayEx &fLineArray, Line3DArrayEx &bLineArray);

    /**
    *函数名称：Interpolate
    *函数功能：在网格点gp1,gp2之间进行线性插值
    *函数参数：等值点值isoLevel，网格点gp1,gp2
    *函数返回：插值点坐标
    **/
    Point3D Interpolate(float isoLevel, const GridPoint* gp1, const GridPoint* gp2);

    //--------------------------------------------------------------------
    //函数名称：DeleteMerroy
    //函数功能：释放存储立方体顶点的空间
    //函数输入：无
    //函数返回：无
    //--------------------------------------------------------------------
    void DeleteMerroy(void);

    //--------------------------------------------------------------------
    //函数名称：CalculateNormal
    //函数功能：计算每个三角形的法向量
    //函数输入：三角形
    //函数返回：无
    //--------------------------------------------------------------------
    void CalculateNormal(TriangleEx& tri);

    float Magnitude( Point3D Point1, Point3D Point2 );

    void SaveGridPoint(PolyPoint3DArrayEx& polyPointArray);


    //--------------------------------------------------------------------
    //函数名称：ComputeBox
    //函数功能：计算相邻两个剖面的包围盒
    //函数输入：polygonArray是曲线组；
    //函数返回：空；
    //--------------------------------------------------------------------
    void ComputeBox(PolyPoint3DArrayEx fPolyPointArray, PolyPoint3DArrayEx bPolyPointArray, Point3D *secPoint, double extraValue);

    //--------------------------------------------------------------------
    //函数名称：InBox
    //函数功能：计算相邻两个剖面的包围盒
    //函数输入：p是待要测试是否在包围盒的点, Polygon为多边形， n为多边形的法向量反方向
    //函数返回：空；
    //--------------------------------------------------------------------
    bool InBox(Point3D point, Polygon3D polygon, double n[]);

    //////////////////////////////////////////////////////////////
    //函数名称：PointProjectPlane
    //函数功能：默认点p1投影到法向量为U的平面上,p0在平面上,点p1沿着N方向投影
    //输入参数：待投影点p1，平面上一点p0和平面法向量U，p1沿着N投影到平面上
    //输出参数：无
    //返回结果：p1在平面上的投影点
    Point3D PointProjectPlane(Point3D p1,Point3D p0,Vector3D N,Vector3D U);

    //////////////////////////////////////////////////////////////
    //函数名称：GetNormal
    //函数功能：计算一条平面上的曲线法线向量
    //输入参数：曲线段tpp
    //输出参数：曲线段所在的平面的法向量a[]
    //返回结果：
    bool GetNormal(Point3DArrayEx& tpp,double a[]);

    /**
    *函数名称:  ShowBox
    *函数功能：
    *函数参数：
    *函数返回：
    **/
    static void ShowBox(PolyPoint3DArrayEx polyBoxArray);

    /**
    *函数名称：SaveBoxArray
    *函数功能：
    *函数参数：
    *函数返回：
    **/
    void SaveBoxArray(Point3DArrayEx &boxArray);

    /**
    *函数名称：TranlatePoint
    *函数功能：将一条点法式表示的直线沿着方向向量direction平移dis个单位
    *函数参数：待平移直线上的一点p，直线法向量lineNormal，平移的向量direction， 平移的单位距离dis
    *函数返回：平移后的点
    **/
    Point3D TranlatePoint(Point3D &p, Vector3D direction, double dis);

    /**
    *函数名称： GetProjectPointsSet
    *函数功能：获得点pointArray在一个以法向量为v，参考点为refPoint的平面上的投影点集
    *函数参数：平面法向量v， 平面上的一个参考点refPoint，待投影的点集pointArray,返回的投影点集合projectPointSet
    *函数返回：无
    **/
    void GetProjectPointsSet(Vector3D v, Point3D refPoint, Point3DArrayEx pointArray, Point3DArrayEx &projectPointSet);

    /**
    *函数名称：CreateABox
    *函数功能：计算相邻剖面之间轮廓线的立体包围盒
    *函数参数：前剖面的法向量v，两剖面距离dis，前剖面的点和后剖面点的集合Points， 返回的包围盒顶点集boxPoints， 设置的立方格边长
    *函数返回：无
    **/
    double CreateABox( Vector3D v, double dis, Point3DArrayEx Points, Point3DArrayEx &boxPoints, double extraValue);

    /**
    *函数名称：IsSameDirection
    *函数功能：判断点p1p2的方向是否与q1q2的方向一致
    *函数参数：线段1的两个端点p1, p2, 线段2的两个端点q1, q2
    *函数返回：如果方向一致返回真，否则返回假
    **/
    bool IsSameDirection(Point3D p1, Point3D p2, Point3D q1, Point3D q2);

    /**
    *函数名称：PointProjectLine
    *函数功能：计算点p投影到直线p1p2上的投影点
    *函数参数：直线上的两个端点p1p2，待投影点p
    *函数返回：投影点坐标
    **/
    Point3D PointProjectLine(Point3D p1, Point3D p2, Point3D p);

    // 计算一条直线和平面交点,此函数被PointProjectLine函数调用
    Point3D LinePlaneInt(Point3D& pointOnLine, Vector3D& vectorOfLine, Point3D& pointOnPlane, Vector3D& vectorOfPlane);

    /**
    *函数名称：DrawString
    *函数功能：绘制文本
    *函数参数：文本内容str，文本坐标pos
    *函数返回：无
    **/
//    static void MarchingCube::DrawString(const char* str, Point3D& pos);
//
//    /**
//    *函数名称：SelectFont
//    *函数功能：文本字体设置
//    *函数参数：字体大小size
//    *函数返回：无
//    **/
//    static void MarchingCube::SelectFont(int size, int charset, const char* face);

    /**
    *函数名称：	IsDirectionSameWithVector
    *函数功能：判断点p到projectPoint的方向向量是否与给定的向量n方向一致
    *函数参数：
    *函数返回：如果一致返回真，否则返回假
    **/
    bool IsDirectionSameWithVector(Point3D p, Point3D projectPoint, Vector3D n);

    bool IsSameDirection(Vector3D v1, Vector3D v2);
    /**
    *函数名称：CalcDistance
    *函数功能：计算两点间的距离(似乎比Relation3D中的PToPDistance要快)
    *函数参数：两点坐标point1,point2
    *函数返回：距离值
    **/
    double CalcDistance(Point3D point1, Point3D point2);

    /**
    *函数名称：CreateNewGrid
    *函数功能：计算网格值，连接等值点生成三角面片
    *函数参数：参数说明跟函数CreateGrid同义
    *函数返回：无
    **/
    void CreateNewGrid(Point3D *point, PolyPoint3DArrayEx FrontPolyPointArray, PolyPoint3DArrayEx BackPolyPointArray, PolyPoint3DArrayEx unfDesityContours, PolyPoint3DArrayEx unbDesityContours,PolyPoint3DArrayEx& GridPointData);

    /**
    *函数名称：CreateBox
    *函数功能：取的当前轮廓线组的包围盒，包围盒边界与剖面边框平行
    *函数参数：轮廓线组polyPointArray, 剖面边框的四个顶点secPoint，box为生成包围盒的四个顶点
    *函数返回：无
    **/
    void CreateBox(PolyPoint3DArrayEx polyPointArray, Point3D *secPoint, Point3DArrayEx &box, double extraValue);

    /**
    *函数名称：CreateALine
    *函数功能：过已知点refPoint，做一条与过点point1,point2的直线平行的直线
    *函数参数：已知点refPoint，参考直线point1point2
    *函数返回：无
    **/
    void CreateALine(Point3D refPoint, Point3D point1, Point3D point2);

    /**
    *函数名称：CreateOpenCurveGrid
    *函数功能：
    *函数参数：
    *函数返回：无
    **/
    void CreateOpenCurveGrid(Point3D *point, PolyPoint3DArrayEx FrontPolyPointArray, PolyPoint3DArrayEx BackPolyPointArray, PolyPoint3DArrayEx& GridPointData);

    /**
    *函数名称：CreateOpenCurveBox
    *函数功能：计算开曲线与辅助曲线构成的封闭轮廓线的包围盒，包围盒的一条边要求与开曲线的两个首尾端点连线平行
    *函数参数：openCurve为开曲线，auxiCurve为用户添加的辅助曲线，box为返回的包围盒
    *函数返回：无
    **/
    bool CreateOpenCurveBox(Point3DArrayEx openCurve, int index1, int index2, Point3DArrayEx &box, Vector3D n);

    /**
    *函数名称：ModifyEachBox
    *函数功能：对开曲线的包围盒进行适应修改
    *函数参数：前后曲线的包围盒顶点坐标和开曲线首尾点坐标
    *函数返回：无
    **/
    void ModifyEachBox(Point3DArrayEx &fBox, Point3DArrayEx &bBox, Point3D curve1Star, Point3D curve1End, Point3D curve2Star, Point3D curve2End);

    /**
    *函数名称：GropByLine
    *函数功能：将点集readyGroupPoints按给定的直线startP,endP分组，对于点在直线上的情况不予处理（不能区分左右关系）
    *函数参数：直线上的两点startP，endP，待分类的点集readyGroupPoints，分类后的集合oneGroupPoints,anthorGroupPoints
    *函数返回：无
    **/
    void GroupByLine(Point3D startP, Point3D endP, Point3DArrayEx readyGroupPoints, Point3DArrayEx &oneGroupPoints, Point3DArrayEx &anthorGroupPoints);
    void GenrateSection(double ratio);//在原剖面边框之间，生成新的剖面边框，ratio为平移比例系数
    void ProjectionContours(PolyPoint3DArrayEx &fContoursArray, PolyPoint3DArrayEx &bContoursArray);//投影两个相邻边框上的轮廓线到新的边框。
    void TracePointsToContours(Line3DArrayEx lineArray, PolyPoint3DArrayEx &contours);//在平面网格上进行轮廓线的跟踪
    void ConfirmCorresponseContours(PolyPoint3DArrayEx &newContours, PolyPoint3DArrayEx &refContours, double n[]);//确定轮廓线的对应关系
    void CreateSurfaceByGenrate(PolyPoint3DArrayEx newContours, PolyPoint3DArrayEx refContours, Triangle3DArrayEx &surface, Point3DArrayEx &surfaceNormal);//基于轮廓线的拼接实现表面重建，已废弃
    void CreateSurfaceBySplice(PolyPoint3DArrayEx &newContours, PolyPoint3DArrayEx &refContours, Triangle3DArrayEx &surface, Point3DArrayEx &surfaceNormal);//基于轮廓线的拼接实现表面重建.
    static void ShowInAndOut();//绘制在轮廓线里的点集，测试用。
    //下面三个函数不用
    bool LineRadial(Point3D& A, Point3D& B, Point3D& M, Point3D& N, bool bTestInPlanar);
    bool PointInPolygon(Point3D& point, Point3D* pt, int size, bool bSideIsIn);
    int PointInPolygon(Point3D& point, Point3DArrayEx& pointArray);
    void ReverseOrder(Point3DArrayEx& pointArray);
    void RadomRay(Point3D rayStartPoint, Point3D& rayEndPoint);

    //读取数据
    bool ReadSurfaceData(string strFilePath);
};
static int sizeI = 0;
static int sizeJ = 0;
static int sizek = 0;
static GridPoint***gridPoints = NULL;



#endif //LEARNOPENGL_MARCHINGCUBES_H
