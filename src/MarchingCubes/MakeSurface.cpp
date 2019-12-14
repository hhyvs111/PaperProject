
#include "MakeSurface.h"
#include <stdio.h>
#include <iostream>
#include "vertex.h"

//extern vector<AddTriangle> McTri;

extern vector<vector<VERTEX>> McLine;
extern double mcScale;
void MakeSurface::DataInit()
{
    m_sectionFrameArray.clear ();
    m_stratumCurveArray.clear ();
    m_faultageCurveArray.clear ();
    m_profileCurveArray.clear ();


    m_stratumSurfNorms.clear ();
    m_faultageSurfNorms.clear ();
    m_solidSurfNorms.clear ();
    m_bodyNorms.clear();
    m_surfaceNorms.clear ();
    m_crossPoints.clear ();
    m_crossPointSN.clear ();


    m_IntArray.clear();

    m_surface.clear ();
    m_Curve.clear ();



    m_GridPoint.clear();
    m_BoxArray.clear();
}


bool MakeSurface::ReadSectionData(char* strFilePath){
    FILE*    fp;
    double   x1,y1,z1;
    double   x2,y2,z2;
    double   x3,y3,z3;
    double   m_minX,m_minY,m_minZ,m_dX,m_dY,m_dZ,m_clip;
    float    rgb[3];
    char     str[100];
    unsigned uName;
    int      nLineType,nLineNum;
    int      sectionNo,nSectionNum;//sectionNo为剖面所在的层号，nSectionNum为总剖面个数
    int      nPointNum;//nPointNum为每个剖面的点的总个数
    int      nAttr1,nAttr2;//nID与sectionNo同义
    int      i,j;
    Point3D         point;
    Triangle3D      objTriangle;
    Point3DArrayEx     polyPoint;
    Triangle3DArrayEx  polyTriangle;



    DataInit();
    if((fp=fopen(strFilePath,"r"))==NULL)
        return false;

    //中心坐标
//    fscanf(fp,"%lf%lf%lf",&m_minX,&m_minY,&m_minZ);
//    fscanf(fp,"%lf%lf%lf",&m_dX,&m_dY,&m_dZ);
    //轮廓数目
    fscanf(fp,"%d",&nSectionNum);
//    cout << m_minX << endl;
    m_clip=(m_dX>m_dY)?m_dX:m_dY;
    m_clip=(m_clip>m_dZ)?m_clip:m_dZ;
        fscanf(fp,"%d",&nLineType);
        fscanf(fp,"%d",&nLineNum);
        cout << "lineNum " << nLineNum  << "lineType " << nLineType <<  endl;
        for(i=0;i<nLineNum;i++)
        {

            fscanf(fp,"%d%d%d%d",&sectionNo,&nAttr1,&nAttr2,&nPointNum);
            //afxDump<<"sectionNo="<<sectionNo<<"\n";
            //afxDump<<"nPointNum="<<nPointNum<<"\n";
            //以下四行是初始化每个剖面
            polyPoint.clear ();
            polyPoint.m_ID=sectionNo;                                        //剖面号
            polyPoint.m_attr1 =nAttr1;
            polyPoint.m_attr2 =nAttr2;


            //读坐标生成多边形
            vector<VERTEX> line;
            VERTEX v;
            for(j=0;j<nPointNum;j++)
            {
                fscanf(fp,"%lf%lf%lf",&x1,&y1,&z1);                           //点的实际坐标
                //point.Set(x1,y1,z1);//相对于区域中心点的坐标
//                v.x = (x1-m_minX-m_dX/2) / mcScale;
//                v.y = (y1-m_minY-m_dY/2) / mcScale;
//                v.z = (z1-m_minZ-m_dZ/2) / mcScale;
                v.x = x1;
                v.y = y1;
                v.z = z1;
                line.push_back(v);
                v.Print();
//                point.Set(x1-m_minX-m_dX/2,y1-m_minY-m_dY/2,z1-m_minZ-m_dZ/2);//相对于区域中心点的坐标
                point.Set(x1,y1,z1);//相对于区域中心点的坐标
                //afxDump<<m_minX+m_dX/2<<" "<<m_minY+m_dY/2<<" "<<m_minZ+m_dZ/2<<"\n";
//                point.Debug();
                polyPoint.Add(point);

            }
//            uName=nLineType*100*MaxSize;
            if(nLineType==200)//200剖面
            {
//                uName+=m_sectionFrameAttrArray.size()+1;
//                objGLAttr.Set(m_rgb[0],uName);//设置gl缺省属性
                //直接用这个成员函数的属性，不要选择了
                m_sectionFrameArray.Add (polyPoint);//添加剖面到剖面边框组
//                m_sectionFrameAttrArray.Add (objGLAttr);//添加gl属性到剖面
                //设置剖面法向量
                Vector3D vector;
                if(nPointNum<3)//如果不足三个点，则返回
                    break;
                //这个操作是干啥的？
                point=polyPoint.at (0);//????
                point.Get (x1,y1,z1);
                point=polyPoint.at (1);
                point.Get (x2,y2,z2);
                point=polyPoint.at (2);
                point.Get (x3,y3,z3);
                x2-=x1,y2-=y1,z2-=z1;
                x3-=x1,y3-=y1,z3-=z1;
                vector.CrossProduct (x2,y2,z2,x3,y3,z3);//计算叉积
                vector.Normalize ();//单位化
                vector.Get (m_n);
            }
            if(nLineType == 203){
                //实体轮廓线
//                cout << polyPoint.m_ID << endl;
                //有两个剖面，array的值为剖面个数
//                m_IntArray.push_back(polyPoint.m_ID);
                //所有的点都会加进去，后续根据这个id来区分剖面
                //将多变形放入，这里是否是可能有多个合并了？应该是在后面进行分割吧
                m_profileCurveArray.Add (polyPoint);
                McLine.push_back(line);
//                cout << line.size() << endl;

            }
        }

    cout << "read ok" << endl;
    fclose(fp);
    return true;

}
//只能求出有多少个FrameNum
void MakeSurface::PreCreateSurface(int& NumFrame, PolyPoint3DArrayEx& polyPointArray)
{
    int              i,j,num,num1,num2, frameid;
//    float            rgb[3];
//    int              attr1,attr2;
//    Triangle3D       objTriangle;
//    Point3D          point,tmpPoint,tmpPoint1;
//    Line3D           objLine;
    Point3DArrayEx      polyPoint,tempPolyPoint;
    PolyPoint3DArrayEx  tempPolyPointArray;

    //边框个数数组清空;
    m_IntArray.clear();


    //取得选择的曲线
    tempPolyPointArray = m_profileCurveArray;

    int Num = tempPolyPointArray.size();
    cout << "get temp " << Num << endl;

//    return ;
    if(Num > 0)
    {
        polyPointArray = tempPolyPointArray;
        polyPoint = polyPointArray.at(0);
        //计算已选取的曲线所在剖面的总个数NumFrame,NumFrame初始值为1;
        NumFrame = 1;
        for (i = 0; i < Num - 1; i++)
        {
            //清空上次循环的曲线;
            polyPoint.clear();
            tempPolyPoint.clear();

            //获取曲线组的下一条曲线;
            polyPoint = polyPointArray.at(i);
            tempPolyPoint = polyPointArray.at(i + 1);

            //如果曲线在同一个剖面上,则NumFrame不自加,返回,否则NumFrame自加;
            if (polyPoint.m_ID == tempPolyPoint.m_ID)
            {
                continue;
            }
            else
            {
                NumFrame ++;
            }

        }

        //将第一条曲线的编号m_ID存入到曲线编号数组当中, PreM_ID为存入前一次的曲线m_ID值;
        polyPoint = polyPointArray.at(0);
        int PreM_ID = frameid = polyPoint.m_ID;
        m_IntArray.push_back(frameid);

        //求剩余曲线的编号m_ID;
        for (i = 0; i < Num; i++)
        {
            //清空上次循环的曲线;
            polyPoint.clear();

            //获取曲线组的下一条曲线;
            polyPoint = polyPointArray.at(i);

            //当前曲线编号m_ID与前一个编号PreM_ID相同,则不存入到曲线编号数组;
            if (PreM_ID == polyPoint.m_ID)
            {
                PreM_ID = polyPoint.m_ID;
                continue;
            }
            else
            {
                frameid = polyPoint.m_ID;
                m_IntArray.push_back(frameid);
                PreM_ID = polyPoint.m_ID;

            }

        }
    }
}

void MakeSurface::CreateSurface2()
{
    int i, PointsID, frameid1, frameid2;
    Point3DArrayEx  polyLine;
    PolyPoint3DArrayEx polyPointArray, FrontPolyPointArray, BackPolyPointArray, fContours, bContours, nonfDesityContours, nonbDesityContours;
    PolyPoint3DArrayEx unfDesityContours, unbDesityContours;
    Point3D      point[4], point1[8];
    PolyPoint3DArrayEx newSectionArray;
    Point3DArrayEx  boxArray;
    bool isIncreaseDensity = false;
    Vector3D v1, v2;
    double n1[3], n2[3];
    float rgb[] = {1, 0, 0};

    int lengthNum, widthNum;
    bool isSurfaceClose = false, isPointInsert = false;


    //取得剖面个数和已选曲线组;还选个鸡儿，直接弄上去就完事了。

    PreCreateSurface(m_FrameNum, polyPointArray);
    cout << "size: " << polyPointArray.size() << endl;


//    cout << "int array" << m_IntArray.size() << endl;
    //直接将所有的传入就行了。

//    for(int i = 0 ;i < m_IntArray.size();i++)
//        cout << m_IntArray[i] << endl;
//
//    for (i=0; i<m_IntArray.size(); i++)
//    {
//        cout << "poly " <<  m_profileCurveArray.at(i).m_ID << endl;
//
//        //这里只加入了两个点？
//        newSectionArray.Add(m_profileCurveArray.at(m_IntArray[i]));
//    }


//    newSectionArray = polyPointArray;

    //因为其实是有两条组的，只是在同一个剖面上
    cout << m_IntArray.size() << " bsize " <<  m_profileCurveArray.size() << endl;
    if(m_profileCurveArray.size() != m_IntArray.size()){
        for(int i = 0;i < m_profileCurveArray.size() - 1;i++){
            if(m_profileCurveArray.at(i).m_ID == m_profileCurveArray.at(i+1).m_ID){
                Point3D tempPoint;
                for(int j = 0;j < m_profileCurveArray.at(i+1).size();j++){
                    tempPoint = m_profileCurveArray.at(i+1).at(j);
                    m_profileCurveArray.at(i).Add(tempPoint);
                }
                m_profileCurveArray.erase(i+1);
            }
        }
    }

    //这里应该是要加入所有的东西，而不是只有一个？cout << "poly " <<  m_profileCurveArray.at(i).m_ID << endl;
    //
    for (i=0; i<m_IntArray.size(); i++)
    {
        cout << m_IntArray[i] << endl;
        newSectionArray.Add(m_profileCurveArray.at(i));

    }

    cout << m_IntArray.size() << " size " <<  m_profileCurveArray.size() << endl;

    cout << "section " << newSectionArray.size() << endl;

    Triangle3DArrayEx tempSurface;
    Point3DArrayEx surfaceNormals;
    tempSurface.clear();
    surfaceNormals.clear();
    m_FrameNum = newSectionArray.size();

//    CString str;
//    str.Format("轮廓线数：%d\n", m_IntArray.size());
//    AfxOutputDebugString(str);

    float sumTime=0;
    int   sumTri=0;

    //测试计算的时间变量;
    clock_t      start, finish;

    //m_FrameNum不能为0;
    if (m_FrameNum)
    {
        //这里是对话长度，比如立方体的个数
//        lengthNum = 20;
//        widthNum = 20;
        isSurfaceClose = false;
        isPointInsert = false;
//        m_pToPDistance = dlg.m_pToPDis;

        //对每两个相邻的剖面,进行网格化,并实现表面重建;
        for(i = 1; i < m_FrameNum; i++)
        {
            MarchingCube tempGrid;
//            tempGrid.SetSizeJ(lengthNum);
//            tempGrid.SetSizeK(widthNum);

            //取曲线编号数组中的曲线编号;
            frameid1 = newSectionArray.at(i-1).m_ID;
            frameid2 = newSectionArray.at(i).m_ID;
            //两个一样的，我日尼玛还建模个几把。
            cout << "frameid " << frameid1  <<  " " << frameid2 << endl;
            //取对应曲线所在剖面的边框线;
            polyLine = newSectionArray.at(i-1);

            //取边框线的第1,2,3,4个顶点;
            for (PointsID = 0; PointsID < 4; PointsID++)
            {
                tempGrid.m_secPoint[PointsID] = polyLine.at(PointsID);
            }

            //清空边框线数据;
            polyLine.clear();

            //取下一条边框线;
            polyLine = newSectionArray.at(i);

            //取边框线的第1,2,3,4个顶点;
            for (PointsID = 0; PointsID < 4; PointsID++)
            {
                tempGrid.m_secPoint[PointsID + 4] = polyLine.at(PointsID);
            }
            //有这个分配空间啊？
            tempGrid.AllocateMerroy();

            //前后两个剖面上的曲线组清空;
            FrontPolyPointArray.clear();
            BackPolyPointArray.clear();

            //对已选的曲线,根据剖面号分别获取当前的新曲线组;
            FrontPolyPointArray = AssembleNewCurve(frameid1, newSectionArray);
            BackPolyPointArray  = AssembleNewCurve(frameid2, newSectionArray);
//            cout << "begin " << FrontPolyPointArray.size() <<  BackPolyPointArray.size() << endl;
            //保存未加密前的轮廓线;
            nonfDesityContours = FrontPolyPointArray;
            nonbDesityContours = BackPolyPointArray;
            unfDesityContours = FrontPolyPointArray;
            unbDesityContours = BackPolyPointArray;

//            //判断是否需要进行轮廓线加密，如果需要则加密;
//            if (isPointInsert)
//            {
//                InsertPoints(FrontPolyPointArray);
//                InsertPoints(BackPolyPointArray);
//            }

            tempGrid.m_fPolyPointArray = &FrontPolyPointArray;
            tempGrid.m_bPolyPointArray = &BackPolyPointArray;
            cout << " mc point " << tempGrid.m_fPolyPointArray->at(0).size() << " back " << tempGrid.m_bPolyPointArray->at(0).size() << endl;
            //如果不要求轮廓线吻合的情况，则直接重建
            if (!isSurfaceClose)
            {
                //注意这里的东西，要保存先后的轮廓线才行。
                tempGrid.CreateNewGrid(tempGrid.m_secPoint, FrontPolyPointArray, BackPolyPointArray, nonfDesityContours, nonbDesityContours, m_GridPoint);
                //将网格点连接成一个体元，即：八个网格点组成一个体元；
                tempGrid.ConnectGrids();

                //根据MC算法构造等值面；
                tempGrid.CreateSurface();


                /*sumTime += tempGrid.m_sumTime;
				sumTri += tempGrid.m_sumTri;
				str.Format("%f, %d\n", sumTime, sumTri);
				AfxOutputDebugString(str);*/
            }
//            else
//            {
//////
//                //开启时间计数;
//                start = clock();
//////
//
//                //创建两个剖面，两个剖面往原两剖面中心平移1/5的距离；
//                tempGrid.GenrateSection((double)1/15);
//
//                //计算两个方向向量，代表原剖面与新剖面的移动方向；
//                v1.Set(tempGrid.m_newSectionPoint[0].m_x-tempGrid.m_secPoint[0].m_x, tempGrid.m_newSectionPoint[0].m_y-tempGrid.m_secPoint[0].m_y, tempGrid.m_newSectionPoint[0].m_z-tempGrid.m_secPoint[0].m_z);
//                v1.Normalize();
//                v1.Get(n1);
//                v2.Set(tempGrid.m_newSectionPoint[4].m_x-tempGrid.m_secPoint[4].m_x, tempGrid.m_newSectionPoint[4].m_y-tempGrid.m_secPoint[4].m_y, tempGrid.m_newSectionPoint[4].m_z-tempGrid.m_secPoint[4].m_z);
//                v2.Normalize();
//                v2.Get(n2);
//
//                /*投影原轮廓线到新的剖面上，投影时分别将加密的轮廓线和未加密的轮廓线进行投影，
//				分别存储于fContours，bContours和unfDesityContours，unbDesityContours；*/
//                tempGrid.ProjectionContours(fContours, bContours);
//                tempGrid.m_fPolyPointArray = &nonfDesityContours;
//                tempGrid.m_bPolyPointArray = &nonbDesityContours;
//                tempGrid.ProjectionContours(unfDesityContours, unbDesityContours);
//
//                //构造网格点，并计算网格点的权值；
//                tempGrid.CreateNewGrid(tempGrid.m_newSectionPoint, fContours, bContours, unfDesityContours, unbDesityContours, m_GridPoint);
//
//                //将网格点连接成一个体元，即：八个网格点组成一个体元；
//                tempGrid.ConnectGrids();
//
//                //根据MC算法构造等值面；
//                tempGrid.CreateSurface();
///////
//                sumTri += tempGrid.m_sumTri;
////                str.Format("MP算法构造的三角形有：%d\n", sumTri);
////                AfxOutputDebugString(str);
///////
//                //确定轮廓线的对应关系，即：由MC算法构造出的边缘轮廓线与原来剖面上的轮廓线的对应关系，需对应的轮廓线相似度很高；
//                //	bool isSuccess1, isSuccess2;
//                tempGrid.ConfirmCorresponseContours(tempGrid.m_fNewContours, nonfDesityContours, n1);
//                tempGrid.ConfirmCorresponseContours(tempGrid.m_bNewContours, nonbDesityContours, n2);
//
//                //将MC算法构造出的轮廓线与原轮廓线进行三角拼接，完成整个体的构造;
//                nonfDesityContours.at(0).GetNormal(n1);
//                v1.Set(n1);
//                v1.Normalize();
//                nonbDesityContours.at(0).GetNormal(n2);
//                v2.Set(n2);
//                v2.Normalize();
//                if (tempGrid.IsSameDirection(v1, v2))
//                {
//                    for (int j=0; j<nonfDesityContours.size(); j++)
//                    {
//                        Point3DArrayEx *ptPolyPoint;
//                        ptPolyPoint = &nonfDesityContours.at(j);
//                        tempGrid.ReverseOrder(*ptPolyPoint);
//                    }
//                }
//                nonfDesityContours.at(0).GetNormal(n1);
//                v1.Set(n1);
//                v1.Normalize();
//                tempGrid.CreateSurfaceBySplice( tempGrid.m_fNewContours, nonfDesityContours,tempSurface, surfaceNormals);
//                tempGrid.CreateSurfaceBySplice(tempGrid.m_bNewContours,  nonbDesityContours,tempSurface, surfaceNormals);
//
///////
//
//                sumTri += tempSurface.size();
////                str.Format("分段拼接算法构造的三角形有：%d,共总的三角形为：%d\n", tempSurface.size(),sumTri);
////                AfxOutputDebugString(str);
//                finish = clock();
//                float duration = (double)(finish - start) / CLOCKS_PER_SEC;
//                sumTime += duration;
////                str.Format("总共用的时间：%f\n", sumTime);
////                AfxOutputDebugString(str);
///////
//                SaveSurfaceData(tempSurface, surfaceNormals/*,FrontPolyPointArray*/);
//                tempSurface.clear();
//                surfaceNormals.clear();
//
//            }

            //保存构造出的等值面;
//            tempGrid.SaveTriangleToSurface(tempSurface, surfaceNormals);
//
//            /*包围两个重建剖面之间的包围盒,可有可无*/
//            tempGrid.SaveBoxArray(boxArray);
//            m_BoxArray.Add(boxArray);
//            boxArray.clear();
//
//            //convert the surface to other expression;
//            SaveSurfaceData(tempSurface, surfaceNormals/*, FrontPolyPointArray*/);
            tempSurface.clear();
            surfaceNormals.clear();
        }
    }
    m_FrameNum = 0;
}


PolyPoint3DArrayEx MakeSurface::AssembleNewCurve(int frameid, PolyPoint3DArrayEx polyPointArray)
{
    //声明返回的新曲线组;
    PolyPoint3DArrayEx polyPointArrayNew;

    //曲线组数据清空;
    polyPointArrayNew.clear();

    //循环变量定义及中间变量polyLine线条对象声明;
    int num;
    Point3DArrayEx  polyLine;

    //线条数据清空;
    polyLine.clear();

    //对输入的曲线组循环进行查询,如果与给定的剖面号相等,则添加到新曲线组中去;
    for (num = 0; num < polyPointArray.size(); num ++)
    {
        polyLine = polyPointArray.at(num);
        if (frameid == polyLine.m_ID)
        {
            polyPointArrayNew.Add(polyLine);
        }
    }
    polyPointArrayNew.m_ID = frameid;
    return polyPointArrayNew;
}