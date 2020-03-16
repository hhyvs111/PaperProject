//
// Created by 谭文波 on 2018/10/18.
//
#include <vertex.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <fstream>



//射线法

//三态函数，判断两个double在eps精度下的大小关系?
int dcmp(double x)
{
    if(fabs(x)<eps) return 0;
    else
        return x<0?-1:1;
}
//判断点Q是否在P1和P2的线段上
bool OnSegment(VERTEX& P1,VERTEX& P2,VERTEX& Q)
{
    //前一个判断点Q在P1P2直线上 后一个判断在P1P2范围上
    return dcmp((P1-Q)^(P2-Q))==0&&dcmp((P1-Q)*(P2-Q))<=0;
}
//判断点P在多边形内-射线法

//true是在多边形内？
//fasle是在多边形外？
bool InPolygon(vector<vector<VERTEX>>& vec, VERTEX &P)
{
    bool flag = false; //相当于计数
    VERTEX P1,P2; //多边形一条边的两个顶点

    for(int i = 0;i < vec.size();i++){
        //多边形里多个多边形
        for(int j = 0; j < vec[i].size() - 1;j++){
            P1 = vec[i][j+1];
            P2 = vec[i][j];
            cout << "test of inPolygon" << endl;
            P1.Print();
            P2.Print();
            //点在线上
            if(OnSegment(P1,P2,P)) return true;

            //前一个判断min(P1.y,P2.y)<P.y<=max(P1.y,P2.y)
            //这个判断代码我觉得写的很精妙 我网上看的 应该是大神模版
            //后一个判断被测点 在 射线与边交点 的左边
            if( (dcmp(P1.y-P.y)>0 != dcmp(P2.y-P.y)>0) && dcmp(P.x - (P.y-P1.y)*(P1.x-P2.x)/(P1.y-P2.y)-P1.x)<0)
                flag = !flag;
            cout << flag << endl;
        }
        //首位计算
        P1 = vec[i][0];
        P2 = vec[i][vec.size()-1];

        if(OnSegment(P1,P2,P)) return true;

        //前一个判断min(P1.y,P2.y)<P.y<=max(P1.y,P2.y)
        //这个判断代码我觉得写的很精妙 我网上看的 应该是大神模版
        //后一个判断被测点 在 射线与边交点 的左边
        if( (dcmp(P1.y-P.y)>0 != dcmp(P2.y-P.y)>0) && dcmp(P.x - (P.y-P1.y)*(P1.x-P2.x)/(P1.y-P2.y)-P1.x)<0)
            flag = !flag;
    }
    return flag;
}

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

//        cout << " the z " << vertex[i].z << endl;

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

//断层平移函数

void faultMoveFunction(vector<VERTEX>& vertex, float moveSize, int whichDirection)
{
    int num = vertex.size();
    if(num == 0)
        return;
//    cout<<"movefunction"<<endl;
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

//断层放缩函数
void faultScaleFunction(vector<VERTEX>& vertex, float scaleSize, int whichDirection)
{
    int num = vertex.size();
    if(num == 0)
        return;

    if(whichDirection == xD)
    {
        for(int i = 0;i < num; i++)
        {
            vertex[i].x *= scaleSize;
        }
    }
    if(whichDirection == yD)
    {
        //从1开头，然后加等于2
        for(int i = 0;i < num; i++)
        {
            vertex[i].y *= scaleSize;
        }
    }
    if(whichDirection == zD)
    {
        for(int i = 0;i < num;i++)
        {
            vertex[i].z *= scaleSize;
        }
    }
}

//判断两条线是否相交,返回true是
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
//如果返回false说明相交，true说明不相交
bool faultIntersect(vector<VERTEX>& fault1, vector<VERTEX>& fault2)
{

    int f1Number = fault1.size(), f2Number = fault2.size();
    //断层里的线是依次的，基本上第一个点的ip是基本在同一起点，那么还是要判断多重啊。不能单一的判断，那么就是一个线段是否与另一条的所有线段相交
    //如果相交则要平移一下，但是这样感觉效率好低啊，不管了，先这样做吧。应该计算量不大，这个东西的话是一个基本的算法，时间复杂度应该不高。
    for(int i = 0; i < f1Number - 1; i++)
    {
        for(int j = 0; j < f2Number - 1; j++)
        {
            //如果有任意一条相交，那么该线就需要平移。
            if(sideIntersectSide(fault1[i],fault1[i+1], fault2[j], fault2[j+1])){

                cout << "this line is cross: " << endl;
                fault1[i].Print();
                fault1[i+1].Print();
                fault2[j].Print();
                fault2[j+1].Print();
                return false;
            }

        }
    }
    return true;
}



//point点到直线ab的距离
float DistanceOfPointLinesIn2D(VERTEX point, VERTEX a, VERTEX b)
{

    return (fabs((b.y - a.y) * point.x +(a.x - b.x) * point.y + ((b.x * a.y) -(a.x * b.y)))) / (sqrt(pow(b.y - a.y, 2) + pow(a.x - b.x, 2)));
//    return (fabs((b.y - a.y) * point.x + (a.x - b.x) * point.y + (b.x -a.x)*a.y + (a.y-b.y)*a.x)) / (sqrt(pow(b.y - a.y, 2) + pow(a.x - b.x, 2)));
}


// 点到点的距离

float DistanceOfPointAndPoint(VERTEX p1, VERTEX p2){
    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x, y2 = p2.y;
    float dis = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

    return dis;
}



float DistanceOfPointAndLine(VERTEX p, VERTEX p1, VERTEX p2){
    float ans = 0;
    float a, b, c;
    a = DistanceOfPointAndPoint(p1, p2);
    b = DistanceOfPointAndPoint(p1, p);
    c = DistanceOfPointAndPoint(p2, p);
    if (c+b==a) {//点在线段上
        ans = 0;
        return ans;
    }
    if (a<=0.00001) {//不是线段，是一个点
        ans = b;
        return ans;
    }
    if (c*c >= a*a + b*b) { //组成直角三角形或钝角三角形，p1为直角或钝角
        ans = b;
        return ans;
    }
    if (b * b >= a * a + c * c) {// 组成直角三角形或钝角三角形，p2为直角或钝角
        ans = c;
        return ans;
    }
    // 组成锐角三角形，则求三角形的高
    double p0 = (a + b + c) / 2;// 半周长
    double s = sqrt(p0 * (p0 - a) * (p0 - b) * (p0 - c));// 海伦公式求面积
    ans = 2*s / a;// 返回点到线的距离（利用三角形面积公式求高）
    return ans;
}


//float DistanceOfPointAndLine(VERTEX p0, VERTEX p1, VERTEX p2){
//    float dis12 = DistanceOfPointAndPoint(p1, p2);//线段长度
//    float dis01 = DistanceOfPointAndPoint(p0, p1);//p1与p0的距离
//    float dis02 = DistanceOfPointAndPoint(p0, p2);//p2与p0的距离
//    float HalfC = (dis12 + dis01 + dis02) / 2;// 半周长
//    float s = sqrt(HalfC * (HalfC - dis12) * (HalfC - dis01) * (HalfC - dis02));//海伦公式求面积
//    float xj2DisPL = 2 * s / dis12;// 返回点到线的距离（利用三角形面积公式求高）
//
//    return xj2DisPL;
//
//}


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
float DistanceOfOpposite(VERTEX point, const vector<VERTEX>& Opposite, int &index)
{
    int num = Opposite.size();
//    float minDistance = DistanceOfPointLinesIn3D(point, Opposite[0], Opposite[1]);
    float minDistance = DistanceOfPointToPointIn3D(point, Opposite[0]);
    for(int i = 1;i < num - 1; i++)
    {
        float distance = DistanceOfPointToPointIn3D(point, Opposite[i]);
//        float distance = DistanceOfPointLinesIn3D(point, Opposite[i], Opposite[i+1]);
//        cout<< "the d2 distance : " << distance << endl;
        if( distance < minDistance )
        {
            minDistance = distance;
            index = i;
        }
    }
    return minDistance;
}

//判断顶点是否在该数组里
int VertexInVertexs(VERTEX target, const vector<VERTEX>& source)
{

    for(int i = 0;i < source.size(); i++)
    {
        if(target == source[i])
            return i;

    }
    return -1;
}

AddTriangle VertexToTriangle(VERTEX a, VERTEX b, VERTEX c)
{
    AddTriangle _triangle;
    _triangle.a = a;
    _triangle.b = b;
    _triangle.c = c;
    return  _triangle;
}

VERTEX getNormal(const VERTEX& p1, const VERTEX& p2, const VERTEX& p3)
{
    VERTEX normal;

    normal.x = ((p2.y - p1.y) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.y - p1.y));

    normal.y = ((p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z));

    normal.z = ((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x));

    return normal;
}


//从文本读入数据，感觉要弄成三维的才行了
void InputDataToVector(vector<vector<vector<VERTEX>>>& closeLines){
    ifstream infile;
    infile.open("/Users/tanwenbo/CLionProjects/PaperProject/src/data/secondOkdata.txt", ios::in);
    if(!infile){
        cout << "fail to open the file " << endl;
        exit(1);
    }


    //读入剖面数，和轮廓线数
    int sectionNum, lineNum;


    infile >> sectionNum >> lineNum;
    closeLines.resize(sectionNum);
    int sectionNo, pointNum;

    float x, y ,z;
    int index = 0;
    for(int i = 0;i < lineNum;i++){
        //读入轮廓点
        infile >> sectionNo >> pointNum;
        vector<VERTEX> closeLine;

        for(int j = 0;j < pointNum;j++){
            infile >> x >> y >> z;
//            cout << x  << " " << y << " " << z  << endl;
            VERTEX v;
            v.x = x;
            v.y = y;
            v.z = z;
            v.index = index++;
            closeLine.push_back(v);
        }
        //剖面问题
        closeLines[sectionNo].push_back(closeLine);
    }
//    cout << "end" << endl;
    infile.close();
}


//void VertexDivide(vector<VERTEX>& v, vector<vector<VERTEX>>& closeLineV){
//    int index = 0;
//    for(int i = 0;i < v.size();i++){
//        auto zIndex = v[i].z;
//        vector<VERTEX> mV;
//        int j = i;
//        for(; v[j].z == zIndex;j++){
//            v[j].index = index;
//            mV.push_back(v[j]);
//        }
//        i = j - 1;
//        index++;
//        closeLineV.push_back(mV);
//    }
//}
