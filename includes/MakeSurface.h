//
// Created by 谭文波 on 2019/12/3.
//

#ifndef LEARNOPENGL_MAKESURFACE_H
#define LEARNOPENGL_MAKESURFACE_H

//
// Created by 谭文波 on 2019/12/1.
//

#ifndef LITTLEMC_MAKESURFACE_H
#define LITTLEMC_MAKESURFACE_H
#pragma once
#include "MarchingCubes.h"

class MakeSurface
{

public:

    float               m_rgb[7][3];
    double              m_n[3];
    double              m_n1[3];

public:
    string             m_strFileName;
    string             m_surfFileName;             //曲面文件名
    string             m_bodyFileName;             //实体文件名
    Point3DArrayEx         m_crossPoints;              //地层线交点
    IntArray             m_crossPointSN;             //交点剖面号
    PolyPoint3DArrayEx  m_sectionFrameArray;        //剖面边框组
    PolyPoint3DArrayEx  m_stratumCurveArray;        //地层分界线组
    PolyPoint3DArrayEx  m_faultageCurveArray;       //断层线组
    PolyPoint3DArrayEx  m_profileCurveArray;        //轮廓线组
//    PolyTriangle3DArrayEx   m_stratumSurfArray;     //地层曲面组
//    PolyTriangle3DArrayEx   m_faultageSurfArray;    //断层曲面组
//    PolyTriangle3DArrayEx   m_solidSurfArray;       //实体（部分）表面组
//    PolyTriangle3DArrayEx   m_bodies;               //体数据

    PolyPoint3DArrayEx   m_stratumSurfNorms;        //地层面法向量
    PolyPoint3DArrayEx   m_faultageSurfNorms;       //断层面法向量
    PolyPoint3DArrayEx   m_solidSurfNorms;          //实体表面法向量
    PolyPoint3DArrayEx   m_bodyNorms;
    Point3DArrayEx          m_surfaceNorms;

//    OpenGLAttrArray    m_crossPointsAttr;          //地层线交点属性
//    OpenGLAttrArray    m_sectionFrameAttrArray;    //剖面边框组属性
//    OpenGLAttrArray    m_stratumCurveAttrArray;    //地层分界线组属性
//    OpenGLAttrArray    m_faultageCurveAttrArray;   //断层线组属性
//    OpenGLAttrArray    m_profileCurveAttrArray;    //轮廓线组属性
//    OpenGLAttrArray    m_stratumSurfAttrArray;     //地层曲面组属性
//    OpenGLAttrArray    m_faultageSurfAttrArray;    //断层曲面组属性
//    OpenGLAttrArray    m_solidSurfAttrArray;       //轮廓实体（部分）表面组属性
//    OpenGLAttrArray    m_bodyAttrArray;            //体属性
//    stringArray       m_lithList;
//    CArray<COLORREF,COLORREF> m_lithColor;

//    CTime              m_fileMTime;                //当前文件的修改时间
    int m_lengthDivde;
    int m_widthDivide;
    int m_heightDivide;


    ///////////////////////////////////this following var is add by hw///////////////////////////////////////
    Point3DArrayEx        m_Curve;                    //当前曲线
    Triangle3DArrayEx     m_surface;                  //当前生成曲面
//    OpenGLAttr         m_CurveAttr;                //当前曲线属性
//    OpenGLAttr         m_surfaceAttr;              //曲面属性
//    OpenGLAttrArray    m_surfaceAttrArray;         //曲面上三角形属性
//    TDOpenGL*          m_pOpenGL;
//    GLuint              m_surList;
//    double              m_pToPDistance;
    PolyPoint3DArrayEx   m_GridPoint;              //保存生成的网格
    PolyPoint3DArrayEx   m_BoxArray;               //重建剖面曲线的包围盒序列
    int                  m_FrameNum;
    vector<int>     m_IntArray;
    PolyPoint3DArrayEx   m_originProfileCurveArray;
    PolyPoint3DArrayEx   m_originFaultageCurveArray;
    PolyPoint3DArrayEx   m_originStratumCurveArray;

public:
//    void GetBody(PolyTriangle3DArrayEx &surfs,PolyTriangle3DArrayEx &bodies);
//    int GetOptedCrossPoint(Point3D &point);
    PolyPoint3DArrayEx GetOptedStratumCurve(int unselect=0);
    PolyPoint3DArrayEx GetOptedFaultageCurve(int unselect=0);
    PolyPoint3DArrayEx GetOptedProfileCurve(int unselect=0);

//    PolyTriangle3DArrayEx GetOptedStratumSurf(int delFlag=1);
//    PolyTriangle3DArrayEx GetOptedFaultageSurf(int delFlag=1);
//    PolyTriangle3DArrayEx GetOptedSolidSurf(int delFlag=1);
//    PolyTriangle3DArrayEx GetVisibleSolidSurf(int delFlag=1);

    //////////////////////////////////////////////////////////////
    //函数名称：GetInsertedCurve
    //函数功能：取断层面与过指定点的剖面的交线
    //输入参数：无
    //输出参数：无。
    //返回结果：无
    void GetInsertedCurve(double p[3]);
    void HideSectionFrame();
    void DisplaySectionFrame();

    //////////////////////////////////////////////////////////////
    //函数名称：Hide*
    //函数功能：隐藏对象
    //输入参数：无
    //输出参数：无。
    //返回结果：无。
    void HideSurfaces();
    void HideStratumSurf();
    void HideFaultageSurf();
    void HideSolidSurf();
    void HideBody();

    //////////////////////////////////////////////////////////////
    //函数名称：HideUnSel*
    //函数功能：隐藏对象
    //输入参数：无
    //输出参数：无。
    //返回结果：无。
    void HideUnSelSurfaces();
    void HideUnSelStratumSurf();
    void HideUnSelFaultageSurf();
    void HideUnSelSolidSurf();
    void HideUnSelBody();

    //////////////////////////////////////////////////////////////
    //函数名称：Delete*
    //函数功能：隐藏对象
    //输入参数：无
    //输出参数：无。
    //返回结果：无。
    void DeleteSurfaces();
    void DeleteStratumSurf();
    void DeleteFaultageSurf();
    void DeleteSolidSurf();
    void DeleteBody();

    //////////////////////////////////////////////////////////////
    //函数名称：Display*
    //函数功能：重显被隐藏的对象
    //输入参数：无
    //输出参数：无。
    //返回结果：无。
    void DisplaySurfaces();
    void DisplayStratumSurf();
    void DisplayFaultageSurf();
    void DisplaySolidSurf();
    void DisplayBody();

    //////////////////////////////////////////////////////////////
    //函数名称：DataInit()
    //函数功能：初始化数据，主要用于清空数组
    //输入参数：无
    //输出参数：无。
    //返回结果：无。
    void DataInit();

    //////////////////////////////////////////////////////////////
    //函数名称：CreateSurface
    //函数功能：生成曲面
    //输入参数：无
    //输出参数：无。
    //返回结果：无。
    void CreateSurface1();

    //////////////////////////////////////////////////////////////
    //函数名称：CreateSurface
    //函数功能：生成曲面
    //输入参数：无
    //输出参数：无。
    //返回结果：无。
    void CreateSurface2();



    ///////////////////////////////////////////////////////////-
    //函数名称：Process*
    //函数功能：处理采样对象。
    //输入参数：被拾取对象的名字SelName，拾取点的客户坐标x和y。
    //输出参数：无。
    //返回结果：无。
    void ProcessPickup(unsigned SelName,int x,int y);

    ///////////////////////////////////////////////////////////-
    //函数名称：ProcessTriangle
    //函数功能：处理采样对象。
    //输入参数：被拾取对象的分类号nType和序号nIndex,拾取点客户坐标p[]。
    //输出参数：拾取点空间p[]
    //返回结果：无。
    void ProcessTriangles(int nType,int nIndex,double p[]);

    ///////////////////////////////////////////////////////////--
    //函数名称：Process*
    //函数功能：处理采样对象。
    //输入参数：被拾取对象的分类号nType和序号nIndex。
    //输出参数：无。
    //返回结果：无。
    void ProcessCrossPoints(int nType,int nIndex);
    void ProcessSectionFrame(int nType,int nIndex);
    void ProcessStratumCurve(int nType,int nIndex);
    void ProcessFaultageCurve(int nType,int nIndex);
    void ProcessProfileCurve(int nType,int nIndex);
    void ProcessSurfaces(int nType,int nIndex);

    ///////////////////////////////////////////////////////////
    //函数名称：ResetOptFlag
    //函数功能：清除对象的选中标志
    //输入参数：无
    //输出参数：无
    //返回结果：无
    void ResetOptFlag();

    ///////////////////////////////////////////////////////////
    //函数名称：ProcessCrossSurf
    //函数功能：处理曲面相交的情况
    //输入参数：两相交曲面
    //输出参数：处理后的两曲面
    //返回结果：无
    void ProcessCrossSurf();

    void ReverseSurface();

    ///////////////////////////////////////////////////////////
    //函数名称：GetNormal
    //函数功能：求封闭曲线所在平面的单位法向量，修改曲线的方向，使其正向相对于该法向量为逆时针方向。
    //输入参数：闭曲线polyLine，曲线polyLine所在平面外的一点p[3]。
    //输出参数：闭曲线polyLine，法向量n[3]。
    //返回值：无
    void GetNormal(Line3DArrayEx &polyLine,double p[3],double n[3]);
    void GetNormal(Line3DArrayEx &polyLine,Point3D point,double n[3]);

    //////////////////////////////////////////////////////////////
    //函数名称：ExtendSurface
    //函数功能：从一个已知曲面的边界出发，沿着另一个曲面延拓到一个给定的边界线。
    //输入参数：被延拓曲面polyTriangle，延拓过程中经过的曲面polyTriangle1，延拓终止边界线polyLine。
    //输出参数：经过延拓后生成的曲面polyTriangle。
    //返回值：无
//    void ExtendSurface(PolyTriangle3D &polyTriangle,PolyTriangle3D polyTriangle1,PolyLine3D polyLine);

    //////////////////////////////////////////////////////////////
    //函数名称：GetExternalTri
    //函数功能：计算相交于平面多边形边上的三角形对中外侧三角形的集合。
    //输入参数：曲面polyTriangle1，曲面polyTriangle2,多边形polyLine,多边形所在平面的单位法向量n[3]。
    //输出参数：曲面polyTriangle。
    //返回值：无
//    void GetExternalTri(PolyTriangle3D polyTriangle1,PolyTriangle3D polyTriangle2,PolyLine3D polyLine,double n[3],PolyTriangle3D &polyTriangle);

    //////////////////数据读取和保存///////////////////////////

    ///////////////////////////////////////////////////////////
    //函数名称：ReadSectionData
    //函数功能：读取剖面数据文件
    //输入参数：剖面文件名
    //输出参数：无
    //返回结果：无
    bool ReadSectionData(char* strFilePath);

    ///////////////////////////////////////////////////////////
    //函数名称：ReadSurfaceData
    //函数功能：读取曲面数据文件
    //输入参数：曲面文件名
    //输出参数：无
    //返回结果：无
    bool ReadSurfaceData(string strFileName);
    void ReadSolidData(string strFileName);

    void GetSurfaceAttr(int Attr1,int Attr2,int Attr3,int Attr4,int Attr[2]);
    ///////////////////////////////////////////////////////////
    //函数名称：WriteSurfaceData
    //函数功能：写曲面数据文件
    //输入参数：曲面文件名
    //输出参数：无
    //返回结果：无
    void WriteSurfaceData(string strFileName);
    void WriteAsSolids(string strFilePath);

    ///////////////////////////////////////////////////////////
    //函数名称：Assemble*Surface
    //函数功能：组合曲面
    //输入参数：无
    //输出参数：无
    //返回结果：无
    int AssembleSurface();
    int AssembleBody();
    int AssembleStratumSurface();
    int AssembleFaultageSurface();
    int AssembleSolidSurface();
    int AssembleCurve(PolyPoint3DArrayEx &polyPointArray);
private:
//    void WriteSolids(PolyTriangle3DArray &solids,string strFilePath);//提供给WriteAsSolids函数调用

    void WriteSingleSolid(string strFilePath);    //已经不用
    void WriteSolidData(string strFileName);      //已经不用
    void CreateTerminalPlane();
public:

//    //lxy add ******************************************************************
//    PolyPoint3DArrayEx  m_NewContourArray;  //新的轮廓线组,由分支等问题改变了轮廓线,存有新的轮廓线lxy add
//    PolyPoint3D         m_delcontour;       //待删除的的轮廓线,该轮廓线选中了,且做了分支等处理
//
//
//    //去掉所有轮廓线属性数组的选中信息 lxy add
//    void RemoveAllSelectAttr();
//    //删除已选的轮廓线
//    bool DelSelectContour(int p_createtype);
//
//    //去掉已选的轮廓线的选中属性
//    void RemoveSelConSelAttr();
//
//    //返回已选定的轮廓线和属性，返回引用
//    //bool GetSelectContour(int p_createtype,PolyPoint3D& p_polypoint, OpenGLAttr& p_glattr);
//
//    bool GetAllSelectContour(PolyPoint3DArrayEx& p_polypointarray, OpenGLAttrArray& p_openglattrarray);
//
//    //得到选中轮廓线的所在剖面
//    void GetOptedSectionFrame(int id,PolyPoint3D& objsecFrame,OpenGLAttr& objsecFrameGLAttr);
//    //处理分支情况, 传递数据
//    void BranchingSetData(int p_createtype);
//
//
//
//    //检查曲面是否有相交的三角形面片
//    void CheckSurfaceCrossTri();
//
//    PolyPoint3DArrayEx m_polypointarray;
//
//    //设置添加辅助线的数据,用来生成拼接表面
//    void SetHCLDate(PolyPoint3DArrayEx& p_polypointarray);
//
//    //采用模拟退火遗传混合算法，实现带辅助线的轮廓线拼接
//    void CreateAddControlLineSurface();
//
//
//    //传入单独在一个剖面上的轮廓线
//    void SetUnattachContour(PolyPoint3D& unattachcontour);
//    //
//    void SetNewContourArr(PolyPoint3DArrayEx& pnewcontour);
//
//    //
//    void GetNewContourArr(PolyPoint3DArrayEx& pnewcontour);
//
//    //删除因分支等处理过已经生成了新的轮廓线替代了的旧轮廓线
//    void DelSelectToNewCon(PolyPoint3D& pdelcontour);

    //因分支等处理,重新将轮廓线进行属性等处理
    void AnewSetContour();

    //--------------------------------------------------------------------
    //函数名称：CreateAutoBranchSurface        add 2007.4.2
    //函数功能：采用模拟退火遗传混合算法，实现自动分支的轮廓线拼接
    //函数输入：
    //函数返回：无
    //--------------------------------------------------------------------
    void CreateAutoBranchSurface();

    int GetSelContourType();

    PolyPoint3DArrayEx GetSelectContour();

    ///////////////////////////////////this following function is add by hw///////////////////////////////////////

    //--------------------------------------------------------------------
    //函数名称：ShowCubes
    //函数功能：显示立方体
    //函数输入：无
    //函数返回：无
    //--------------------------------------------------------------------
    void ShowCubes(void);

    /**
    *函数名称：ShowBoxArray
    *函数功能：显示重建实体的包围盒
    *函数参数：无
    *函数返回：无
    **/
    void ShowBoxArray(void);

    //--------------------------------------------------------------------
    //函数名称：ShowSurface
    //函数功能：显示重建的表面
    //函数输入：无
    //函数返回：无
    //--------------------------------------------------------------------
    void ShowSurface(void);

    //--------------------------------------------------------------------
    //函数名称：ShowPoints
    //函数功能：显示曲线上的点
    //函数输入：无
    //函数返回：无
    //--------------------------------------------------------------------
    void ShowPoints(void);

    //--------------------------------------------------------------------
    //函数名称：DrawContourPoint
    //函数功能：绘制曲线上的点
    //函数输入：曲线组
    //函数返回：无
    //--------------------------------------------------------------------
    void DrawContourPoint(PolyPoint3DArrayEx);

    //--------------------------------------------------------------------
    //函数名称：PreCreateSurface
    //函数功能：重建预处理
    //函数输入：已选取的曲线所在剖面的总个数，曲线组
    //函数返回：无
    //--------------------------------------------------------------------
    void PreCreateSurface(int&, PolyPoint3DArrayEx&);


    //--------------------------------------------------------------------
    //函数名称：saveSurfaceData
    //函数功能：将ＭＣ方法重建的曲面，转存到曲线组中
    //函数输入：临时存储曲面tempSurface，曲面法向量surfaceNormals，曲线组polyPointArray
    //函数返回：无
    //--------------------------------------------------------------------
    void SaveSurfaceData(Triangle3DArrayEx &tempSurface,	Point3DArrayEx &surfaceNormals/*,PolyPoint3DArrayEx polyPointArray*/);


    //--------------------------------------------------------------------
    //函数名称：AssembleNewCurve
    //函数功能：重组已选取的曲线，将一个剖面上的曲线组成一个新的曲线组。
    //函数输入：frameid为当前重组曲线所在的剖面号，曲线组polyPointArray为总曲线组
    //函数返回：剖面frameid所有的曲线重组的曲线组
    //--------------------------------------------------------------------
    PolyPoint3DArrayEx AssembleNewCurve(int frameid, PolyPoint3DArrayEx polypointarray);

    //--------------------------------------------------------------------
    //函数名称：InsertPoints
    //函数功能：对曲线进行插点
    //函数输入：待插点的曲线组
    //函数返回：无
    //--------------------------------------------------------------------
    void InsertPoints(PolyPoint3DArrayEx& polyPointArray);

    /**
    *函数名称：CheckDirection
    *函数功能：检查剖面文件里的线数组方向，统一按用户从0剖面往后看，呈逆时针方向
    *函数参数：无
    *函数返回：无
    **/
    void CheckDirection();

    /**
    *函数名称：
    *函数功能：
    *函数参数：
    *函数返回：
    **/
    void AddNewSection(PolyPoint3DArrayEx &newSectionArray);

    /**
    *函数名称：
    *函数功能：
    *函数参数：
    *函数返回：
    **/
//    void TranslateSection(PolyPoint3D& section, int d);

    /**
    *函数名称：
    *函数功能：
    *函数参数：
    *函数返回：
    **/
    void WatchFileChanged();

    void CreateSurface3();


    void IncreaseDensityOfContour();

    void CreateSurfaceByOrdinary();

    void ClearSolid();
};

#endif //LITTLEMC_MAKESURFACE_H


#endif //LEARNOPENGL_MAKESURFACE_H
