#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <Delaunay.h>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "vertex.h"

using namespace std;

//引入第三方库
#include "../poly2tri/poly2tri.h"
using namespace p2t;

//引入耳切法
#include "../includes/mapbox/earcut.hpp"

//引入移动立方体
//#include "MarchingCubes.h"

#include "MakeSurface.h"



//ear cutting
using Coord = float;

// The index type. Defaults to uint32_t, but you can also pass uint16_t if you know that your
// data won't have more than 65536 vertices.
using N = uint32_t;

// Create array
//使用PO
using Points = array<Coord, 3>;
vector<vector<Points>> polygon;
std::vector<N> indices;

int EarTraSize = 0;

#include "Sum.h"

#define  MaxNum 2048*2
#define  MaxLevel 20


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

//一些和poly2tri相关的变量定义
/// Dude hole examples
vector<Point*> CreateHeadHole();
vector<Point*> CreateChestHole();

/// Constrained triangles
vector<Triangle*> triangles[MaxLevel];
/// Triangle map
list<Triangle*> map[MaxLevel];
/// Polylines
//这个好像还是个二维数组？有点牛批
vector< vector<Point*> > polylines[MaxLevel];

vector<p2t::Point*> polyline[MaxLevel];
//这个是用开画图的？
CDT* cdt[MaxLevel];

//多出的三角形
//vector<AddTriangle> extraTriangles[MaxNum];

//
vector<AddTriangle> McTri;

vector<vector<VERTEX>> McLine;

double mcScale = 1.0;

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 1.0f;
float lastY = SCR_HEIGHT / 1.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//灯光的位置,论文效果图里的
glm::vec3 lightPos(1.0f, -2.0f, 3.0f);

//calculate the box
double maxZ = -1e9, minZ = 1e9, maxX = -1e9, minX = 1e9, maxY = -1e9, minY = 1e9;

////function
////设置一下变量，用按键实现一下功能
//
////是否开启Delaunay
//bool DelaunayOpen = false;
//bool Poly2TriOpen = false;
//
////是否开启耳切法
//bool EarCutOpen = false;
////平移操作
//bool faultMove = false;
//bool moveBack1 = false;
//int cntMove = 0;
//int cntBack = 0;
//试一下这个是否有用啊。

//直接写一个指针数组来存数据，up为上层，down为下层
//VERTEX *faultUp[MaxNum], *faultDown[MaxNum];

//断层VAO
//unsigned int faultUpVBO[MaxNum], faultUpVAO[MaxNum], faultDownVBO[MaxNum], faultDownVAO[MaxNum];

//框架数据
unsigned int faceVBO[MaxLevel], faceVAO[MaxLevel];  //直接用你这个face

//纹理数据
//unsigned int textures[MaxLevel][MaxNum];
//unsigned int addTextures[MaxLevel][MaxNum];


//VERTEX *fault2_up, *fault2_down;

//这个值为断层个数，现在暂时定为1个
int modelNum;

int insertTime = 0;


//unsigned int DelTraVBOs[MaxNum], DelTraVAOs[MaxNum];
unsigned int PolyVBOs[MaxLevel][MaxNum], PolyVAOs[MaxLevel][MaxNum];
//unsigned int AddVBOs[MaxNum][MaxNum], AddVAOs[MaxNum][MaxNum];

//移动立方体的数据
//unsigned int McVBOs[1][MaxNum * 10], McVAOs[1][MaxNum * 10];

//unsigned int VAOs[2], VBOs[2];


//bool isAddTra[MaxNum] = {false};

//unsigned int EarVBOs[MaxNum], EarVAOs[MaxNum];

//轮廓线数据
vector<vector<vector<VERTEX>>> closeLineV;

//轮廓线合并数据
//vector<VERTEX> closeLineMerge;

//全局变量，用来平移之类的

//记录每一层的原质心
vector<VERTEX> center;
//用来记录质心的平移量
vector<double> MoveSize;
//这个变量用来来记录面里平移的长度，一个面有两条线。每条线有三个方位的平移值
//moveSize faultMoveSize[MaxNum][2][3];

//一个值就够了，用来记录每一层的平移量
double scaleSize[MaxLevel];


//每一层的高度值
float diff[MaxLevel];

//用一组变量来记录是否进行了质心对齐
bool CenterToCenter[MaxLevel];
bool IsR[MaxLevel];


clock_t startTime, windowTime, calculateTime, renderTime;

void PrintTime(const char * str, clock_t start, clock_t end);

////函数声明
//void faultMoveFunction(VERTEX *vertex, int num, float moveSize, int whichDirection);
////将两个顶点数组合并为1个
//VERTEX * faultMerge(VERTEX vertex1[],int num1, VERTEX vertex2[], int num2);

//初始化
void drawInit(unsigned int & VAO, unsigned int & VBO, vector<VERTEX>&target);

////两个转换的函数
//float * VertexToFloat(VERTEX vertex[], int num);
//
//VERTEX * FloatToVertex(float _float[], int num);

void DelaunayBind(unsigned int * DeVAOs, unsigned int * DeVBOs, int DeNum, Delaunay * del);




void Poly2TriBind(unsigned int * PolyVAOs, unsigned int * PolyVBOs, vector<Triangle*> _triangle);


//未优化的光照
void Poly2TriBindGabi(unsigned int * PolyVAOs, unsigned int * PolyVBOs, vector<Triangle*> _triangle, bool isR);

//void McTriBind(unsigned int *, unsigned int *, unsigned int *, vector<McTriangle>);

//各种三角绑定
void AddTriBind(unsigned int * AddVAOs, unsigned int * AddVBOs, unsigned int * addTexture, vector<AddTriangle> _triangle);

void EarCutBind(unsigned int * EarVAOs, unsigned int * EarVBOs, std::vector<N> indices);


void ExcessTraHandle(Triangle* _triangle, vector<VERTEX>& op, int index);

void moveFunction(VERTEX * fault1, int num1, VERTEX * fault2, int num2, int index, int which);

void poly2Tri(vector<VERTEX>& Merge, int index);

void closePoly2Tri(vector<VERTEX>& closeOut, vector<vector<VERTEX>>& closeHole, int index);

//检查三角形的状态
void checkTri(int index);



//人工处理平台三角形
void checkTriShort(int index);

void lineBack(VERTEX * _faultUp, int num1, int indexLine, int indexTra);

//void BeginMc(vector<VERTEX>);

//判断三角形里的顶点是否在闭线里，如果在则平移。并且如果有放缩的情况，则进行放大
void closeLineBack(vector<VERTEX>& _fault, int indexTra);

//放缩如果有交线则缩小
//如果只是放缩那么我就不管这个平移了，直接判断放缩了！每次放缩1点

//获取插值点的高度，距离反比
void SetPointZ(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in, Point* p);

//反距离加权方法
void SetPointZofIDW(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in, Point* p);

//求每一层的质心
void GetSecCenter(vector<vector<VERTEX>>& sec, int index);


//合并轮廓线
void VertexMerge(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in);

//质心对齐
void SecAligned(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in, int index, bool isR);

//质心对齐0
void SecAlignedToCenter(vector<vector<VERTEX>>& in, int index);

//基于质心放缩
void SecScale(int index, bool isR);



//基于质心放缩
void SecScale(vector<VERTEX>& out, vector<vector<VERTEX>>& in, int index);

//基于质心原点缩放
void SecScaleToCenter(vector<vector<VERTEX>>& in, int index);

//放缩函数
void SecScaleFunction(vector<vector<VERTEX>>& in, double scaleSize, int index);

//还原剖面
void SecBack(int index, bool isR);

//还原三角面片
void TriBack(int index, bool isR);

//对三角形进行插值，times为插值次数，次数越多，模型越精确
void TriInsert(int times, int i);


//美化度检测

void MeshNormalize(int index, bool isR);

bool PolygonInPolygon(int index, bool isR);


p2t::Normal Normalize(p2t::Normal no)
{
    double len = sqrt(no.x * no.x + no.y * no.y + no.z * no.z);
    return p2t::Normal(no.x / len, no.y / len, no.z / len);
}

p2t::Normal Reverse(p2t::Normal no)
{
    return p2t::Normal( - no.x, - no.y, - no.z);
}


void MeshNormalize(int index, bool isR){
    vector<Triangle*> tris = cdt[index]->GetTriangles();

    vector<Point*> verts = cdt[index]->GetPoints();

    for(int i = 0;i < verts.size();i++){
        //默认为空
        verts[i]->normal = Normal();
    }

    for(int i = 0;i < tris.size();i++){
        Triangle &t = *tris[i];

        Point &a = *t.GetPoint(0);
        Point &b = *t.GetPoint(1);
        Point &c = *t.GetPoint(2);

        a.print();

        Point e1 = a - b;
        Point e2 = c - b;
//        e1.print();
//        e2.print();
        //叉乘
        Point no = e1*e2;

//        no.print();
//        cout << "---" << endl;
        //这里改变了别人的点？
        tris[i]->points_[0]->normal += no;
        tris[i]->points_[1]->normal += no;
        tris[i]->points_[2]->normal += no;

    }

    for(int i = 0;i < verts.size();i++){
        verts[i]->normal = Normalize(verts[i]->normal);
    }
}

//void scaleFunction(vector<VERTEX>& fault1, vector<VERTEX>& fault2, int index)
//{
////    int num1 = fault1.size(), num2 = fault2.size();
//    scaleSize[index] = 1.0f;
//
//    //判断是否有相交的线，这里效率也太低了，如果是那种奇怪的图形，估计也是弄不出来什么效果的。
//    while(!faultIntersect(fault1, fault2))
//    {
//        //这里的改变0.1是不是有问题，如果两次之后那么就是0.9 * 0.8 = 0.72
//        //还真是这样，
//        //每次改变0.1
//        //变化之前先把它乘回来，不知道这样是不是开销有点大，我估摸着时间复杂度是O(N)
//        faultScaleFunction(fault2, 1.0f / (scaleSize[index]) ,xD);
//        faultScaleFunction(fault2, 1.0f / (scaleSize[index]) ,yD);
//        scaleSize[index] -= 0.3f;
////        cout << "index " << index << " fault[0] " << fault2[0].x << endl;
//        cout << scaleSize[index] << endl;
//        faultScaleFunction(fault2, scaleSize[index] ,xD);
//        faultScaleFunction(fault2, scaleSize[index] ,yD);
////        cout << "after index " << index << " fault[0] " << fault2[0].x << endl;
//
//    }
//}


//将顶点转换成三角剖分里的点，带洞的另外标注
vector<Point*> VertexsToPoints(vector<VERTEX>& vertex, int holeNum)
{
    vector<Point*> Points;
    for(int i = 0; i < vertex.size(); i++)
    {
        double x = vertex[i].x;
        double y = vertex[i].y;
        double z = vertex[i].z;
        Point *point = new Point(x,y);
        point->z = z;
        //加入序列号，看是否有用
        point->index = i;
        point->isHole = holeNum;
        Points.push_back(point);
    }
    return Points;
}


void LineProcess(){

    modelNum = closeLineV.size();
    center.resize(modelNum);
    //自己定义的就是从下到上，这样是没问题吧，反正是自己的数据，那么第一层就是最低，第n层就是最高。没毛病啊老铁！
    maxZ = closeLineV[modelNum-1][0][0].z;
    minZ = closeLineV[0][0][0].z;


    if(closeLineV.size() < 2){
        cout << "error, closeLine num is less than 2" << endl;
        exit(0);
    }
//    float dif = closeLineV[0][0][0].z - closeLineV[1][0][0].z;

    //数据预处理，并绑定轮廓
//    int faceNum = 0;
//    for(int i = 0;i < modelNum;i++){
//        //获取当前质心
//        GetSecCenter(closeLineV[i], i);
//
////        //质心对齐坐标原点
////        SecAlignedToCenter(closeLineV[i], i);
////        GetSecCenter(closeLineV[i], i);
//        //质心单纯放缩变小
////        SecScaleFunction(closeLineV[i], 0.01, i);
//        for(int j = 0;j < closeLineV[i].size();j++) {
//            drawInit(faceVAO[faceNum], faceVBO[faceNum], closeLineV[i][j]);
//            faceNum++;
//        }
//    }

    //自动处理
//    modelNum = 5;

    for(int i = 0;i < modelNum - 1; i++)
    {
        //单层时间计算
//        cout << "center: ";
//        center[i].Print();
//        center[i+1].Print();
        //如果第二层比第一层轮廓线多，否则反过来？其实还是没用，需要做多层才行。
        //求上下的质心
        GetSecCenter(closeLineV[i], i);
        GetSecCenter(closeLineV[i + 1], i + 1);
        if(closeLineV[i].size() <= closeLineV[i+1].size()){
            IsR[i] = false;
            //放缩也得放进来
            //判断多边形是否在多边形内
            if(!PolygonInPolygon(i, IsR[i])){
                //如果不在，那么需要进行质心对齐和放缩检测了
                SecAligned(closeLineV[i], closeLineV[i+1], i, IsR[i]);
//            轮廓线进行放缩
                SecScale(i, IsR[i]);
//                SecScale(closeLineV[i][0], closeLineV[i+1], i);
                CenterToCenter[i] = true;
            }else{
                //没有进行质心对齐
                CenterToCenter[i] = false;
            }
            closePoly2Tri(closeLineV[i][0], closeLineV[i+1], i);
        }else{
            IsR[i] = true;
            if(!PolygonInPolygon(i, IsR[i])) {
                //传入的时候将上层当做是外边，下层当做是内边
                SecAligned(closeLineV[i + 1], closeLineV[i], i, IsR[i]);
                //轮廓线进行放缩
                SecScale(i, IsR[i]);
//                SecScale(closeLineV[i + 1][0], closeLineV[i], i);
                //传入数据逆置
                CenterToCenter[i] = true;
            }else{
                CenterToCenter[i] = false;
            }
            closePoly2Tri(closeLineV[i+1][0], closeLineV[i], i);
        }

        //很多操作可以写在一起
        checkTri(i);
        TriInsert(insertTime, i);

        if(CenterToCenter[i]){
            SecBack(i, IsR[i]);
            TriBack(i, IsR[i]);
        }
        //MeihuaCheck(i);
        //法向量计算


        MeshNormalize(i, IsR[i]);
        Poly2TriBind(PolyVAOs[i], PolyVBOs[i], triangles[i]);
//        Poly2TriBindGabi(PolyVAOs[i], PolyVBOs[i], triangles[i], IsR[i]);
    }


    //还原后再检查一下三角形的情况，如果是有平台三角形则进行插值。

    //做两次插值

    int faceNum = 0;

    for(int i = 0;i < modelNum;i++){
        //获取当前质心
//        GetSecCenter(closeLineV[i], i);
//        //质心对齐坐标原点
//        SecAlignedToCenter(closeLineV[i], i);
//        GetSecCenter(closeLineV[i], i);
        //质心单纯放缩变小
//        SecScaleFunction(closeLineV[i], 0.01, i);
        for(int j = 0;j < closeLineV[i].size();j++) {
            drawInit(faceVAO[faceNum], faceVBO[faceNum], closeLineV[i][j]);
            faceNum++;
        }
    }
    //三角化本身
//    for(int i = 0;i < modelNum;i++){
//        poly2Tri(closeLineV[i], i+modelNum);
//    }
}

//void DrawLine(){
//
//    //画等值面
//    for(int j = 0; j < modelNum - 1; j++) {
//        for(int i = 0; i < triangles[j].size(); i++) {
//            //激活一下这个纹理
////            glActiveTexture(GL_TEXTURE0);
////            glBindTexture(GL_TEXTURE_2D, textures[j][i]);
//            glBindVertexArray(PolyVAOs[j][i]);
//
//            if(ShowModel){
//                if(ShowTexture)
//                    glDrawArrays(GL_TRIANGLE_STRIP, 0 , 3);
//                else
//                    glDrawArrays(GL_LINE_LOOP, 0, 3);
//            }
//        }
//    }
//    //画线
//    int faceNum = 0;
//    for(int i = 0;i < modelNum;i++){
//        for(int j = 0;j < closeLineV[i].size();j++){
//            glBindVertexArray(faceVAO[faceNum]);
//            glDrawArrays(GL_LINE_LOOP, 0, closeLineV[i][j].size());
//            faceNum++;
//        }
//    }
//}


//mc算法的问题
//void MarchingCubesProcess(){
//     MakeSurface m;
////     string file = "/Users/tanwenbo/CLionProjects/PaperProject/src/Circle.sec";
//    string file = "/Users/tanwenbo/CLionProjects/PaperProject/src/1215.txt";
//     m.ReadSectionData((char*)file.data());
//     //创建好表面后，可以进行渲染了。
//     m.CreateSurface2();
//
//    //获取剖面个数
//    cout << McTri.size() << endl;
//    AddTriBind(McVAOs[0], McVBOs[0], textures[0], McTri);
//
//    for(int i = 0;i < McLine.size();i++){
//        drawInit(faceVAO[i], faceVBO[i], McLine[i]);
//    }
//}

//void McProcess(){
////    readFile();
//
//    SetSample(0.0);
//    vMarchingCubes();
////    vector<VERTEX> v;
////
////    InputDataToVector(v);
////
////    for(int i = 0;i < v.size();i++) {
////        vMarchCube(v[i].x, v[i].y, v[i].z, 1.0/ 16.0);
////    }
//    cout << McTri.size() << endl;
//    AddTriBind(McVAOs[0], McVBOs[0], textures[0], McTri);
//
//}

//void DrawMc(){
//    for(int i = 0; i < McTri.size();i++){
//
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, textures[0][i]);
//        glBindVertexArray(McVAOs[0][i]);
//
//        if(ShowTexture)
//            glDrawArrays(GL_TRIANGLE_STRIP, 0 , 3);
//        else
//            glDrawArrays(GL_LINE_STRIP, 0, 3);
//    }
//
//    for(int i = 0;i < McLine.size();i++){
////        glBindVertexArray(VAOs[i]);
//        glBindVertexArray(faceVAO[i]);
////            cout << McLine[i].size() << endl;
//        glDrawArrays(GL_LINE_LOOP, 0, McLine[i].size());
//    }
//
//}

void SecScale(vector<VERTEX>& out, vector<vector<VERTEX>>& in, int index){

    //这里已经是质心对齐了的，所以直接取上和下都是一样。
    scaleSize[index] = 1.0;
    vector<VERTEX> mergeSec;
    for(int i = 0;i < in.size();i++){
        mergeSec.insert(mergeSec.end(), in[i].begin(), in[i].end());
    }
    cout << "begin scale judge" << endl;
    //这里判断的是合并的，但是我变换的是in？
    while(!faultIntersect(out, mergeSec)){
        //因为这里是循环去调用，所以每次都先还原，然后再加大这个分量进行放缩
        SecScaleFunction(in, 1.0 / scaleSize[index], index);
        mergeSec.clear();
        //每次减少0.1
        scaleSize[index] -= 0.1;
        SecScaleFunction(in, scaleSize[index], index);
        cout << "scale: " << scaleSize[index] << endl;
        //这了为什么又合并一次？
        for(int i = 0;i < in.size();i++){
            mergeSec.insert(mergeSec.end(), in[i].begin(), in[i].end());
        }
    }
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------


    startTime = clock();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
//    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    //光照顶点缓冲
    Shader lightingShader("7.4.lighting_maps.vs", "7.4.lighting_maps.fs");
    Shader lampShader("7.4.lamp.vs", "7.4.lamp.fs");

    //shader 设置
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);


    //处理MC
//    McProcess();


    windowTime = clock();

    //读入数据
    InputDataToVector(closeLineV);



    LineProcess();
    calculateTime = clock();
    PrintTime("calculate time: ", windowTime, calculateTime);

    PrintTime("window time: ", startTime, windowTime);
    PrintTime("total time: ", startTime, calculateTime);

//    MarchingCubesProcess();

//第一个插值模型的灯光参数
//    glm::vec3 lightPos(minX - (maxX - minX)/2, minY + (maxY - minY)/2, (maxZ - minZ) / 2);

//第二个插值模型的灯光参数
//    glm::vec3 lightPos(4*maxX + (maxX - minX)/2, maxY - (maxY - minY)/2, minZ + (maxZ - minZ) / 2);


    //纹理导入一次就ok
    unsigned int texture = loadTexture(FileSystem::getPath("resources/textures/soild.png").c_str());

    //显示代码
    while (!glfwWindowShouldClose(window))
    {
        //更新mojave后的bug，加入以下代码才能正常显示
        bool needDraw = true;
        if (needDraw) {
            glfwShowWindow(window);
            glfwHideWindow(window);
            glfwShowWindow(window);
            needDraw = false;
        }

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

    // render
        // ------
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //光照漫反射实现
        lightingShader.use();
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // light properties
        //环境光照
        lightingShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
        //漫反射光照
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        //镜面光照
        lightingShader.setVec3("light.specular", 0.5f, 0.5f, 0.5f);

        // material properties
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        lightingShader.setFloat("material.shininess", 64.0f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model;
        lightingShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        //画等值面
        for(int j = 0; j < modelNum - 1 ; j++) {
            for(int i = 0; i < triangles[j].size(); i++) {
                glBindVertexArray(PolyVAOs[j][i]);
                if(ShowModel){
                    if(ShowTexture)
                        glDrawArrays(GL_TRIANGLE_STRIP, 0 , 3);
                    else
                        glDrawArrays(GL_LINE_LOOP, 0, 3);
                }
            }
        }
        //画线
        int faceNum = 0;
        for(int i = 0;i < modelNum;i++){
            for(int j = 0;j < closeLineV[i].size();j++){
                glBindVertexArray(faceVAO[faceNum]);
                glDrawArrays(GL_LINE_LOOP, 0, closeLineV[i][j].size());
                faceNum++;
            }
        }
        //渲染MC
//        DrawMc();
//        DrawLine();
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(MaxLevel, faceVAO);
    glDeleteBuffers(MaxLevel, faceVBO);
    glDeleteVertexArrays(MaxLevel, PolyVAOs[MaxNum]);
    glDeleteBuffers(MaxLevel, PolyVBOs[MaxNum]);


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //绑定了wsad键，如果要绘制的话那么需要加一个开关啊。
    //绘制的代码是while循环，看来要加一个全局变量，在while里在加一个判断，是否绘制
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    //是否打开纹理
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
        if(ShowTexture){
            ShowTexture = false;
        } else
            ShowTexture = true;
    }

    //是否显示模型
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
        if(ShowModel){
            ShowModel = false;
        } else
            ShowModel = true;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        insertTime++;
        cout << "insertTime:" << insertTime << endl;

        LineProcess();
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){

        if(insertTime > 1) {
            insertTime--;
            LineProcess();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
        if(IDWOpen){

            IDWOpen = false;
            LineProcess();
        } else{
            IDWOpen = true;
            LineProcess();
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}



//写一个顶点缓冲绑定的函数
//参数要做地址引用，不然都是形参
void drawInit(unsigned int & VAO, unsigned int & VBO, vector<VERTEX>&target)
{
    //还是要逐个复制啊，这感觉有点浪费空间哦。个人感觉也是没效果的
    int num = target.size();

    float source[3 * num];
    for(int i = 0, j = 0;i < num;i++, j+=3){
        source[j] = target[i].x;
        source[j+1] = target[i].y;
        source[j+2] = target[i].z;
//        cout << source[j] << " " << source[j+1] << " " << source[j+2] <<endl;
    }


    //这里要直接用引用还是要去了？感觉好像没报错是没毛病的。

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(source), source, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}





void Poly2TriBind(unsigned int * PolyVAOs, unsigned int * PolyVBOs, vector<Triangle*> _triangle)
{
    glGenVertexArrays(_triangle.size(), PolyVAOs);
    glGenBuffers(_triangle.size(), PolyVBOs);

    //循环读取Polyl里的三角形顶点数据
    //这里用15，9是三个点的坐标，6是三个纹理坐标
    float TraVertex[24];
    cout << "tri size: " << _triangle.size() << endl;

    for (int i = 0; i < _triangle.size(); i++)
    {
        Triangle &t = *_triangle[i];
            Point &a = *t.GetPoint(0);
            Point &b = *t.GetPoint(1);
            Point &c = *t.GetPoint(2);
        //这里计算出来的是这个三角形的法向量，还要计算一下顶点的法向量，顶点法向量由两个平均一下？
//        VERTEX traNormol = getNormal(a.PointToVertex(), b.PointToVertex(), c.PointToVertex());
        //不对啊，这里还有vvo和vv1？傻逼了
        //将顶点分配给这个float
        //三角坐标
        TraVertex[0] = a.x;
        TraVertex[1] = a.y;
        TraVertex[2] = a.z;
        //平面法向量实现光照
        //这里改成用计算出来的点

        TraVertex[3] = a.normal.x;
        TraVertex[4] = a.normal.y;
        TraVertex[5] = a.normal.z;
        //纹理坐标
        TraVertex[6] = 0.0f;
        TraVertex[7] = 0.0f;

        TraVertex[8] = b.x;
        TraVertex[9] = b.y;
        TraVertex[10] = b.z;
        TraVertex[11] = b.normal.x;
        TraVertex[12] = b.normal.y;
        TraVertex[13] = b.normal.z;
        TraVertex[14] = 1.0f;
        TraVertex[15] = 0.0f;


        TraVertex[16] = c.x;
        TraVertex[17] = c.y;
        TraVertex[18] = c.z;
        TraVertex[19] = c.normal.x;
        TraVertex[20] = c.normal.y;
        TraVertex[21] = c.normal.z;
        TraVertex[22] = 0.5f;
        TraVertex[23] = 1.0f;

        //绑定到vbo里
        glBindVertexArray(PolyVAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, PolyVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TraVertex), TraVertex, GL_STATIC_DRAW);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

}


void Poly2TriBindGabi(unsigned int * PolyVAOs, unsigned int * PolyVBOs, vector<Triangle*> _triangle, bool isR)
{
    glGenVertexArrays(_triangle.size(), PolyVAOs);
    glGenBuffers(_triangle.size(), PolyVBOs);

    //循环读取Polyl里的三角形顶点数据
    //这里用15，9是三个点的坐标，6是三个纹理坐标
    float TraVertex[24];
    cout << "tri size: " << _triangle.size() << endl;

    for (int i = 0; i < _triangle.size(); i++)
    {
        Triangle &t = *_triangle[i];
        Point &a = *t.GetPoint(0);
        Point &b = *t.GetPoint(1);
        Point &c = *t.GetPoint(2);
        //这里计算出来的是这个三角形的法向量，还要计算一下顶点的法向量，顶点法向量由两个平均一下？
        VERTEX traNormol = getNormal(a.PointToVertex(), b.PointToVertex(), c.PointToVertex());

        if(!isR){
            traNormol.x = - traNormol.x;
            traNormol.y = - traNormol.y;
            traNormol.z = - traNormol.z;
        }
        //不对啊，这里还有vvo和vv1？傻逼了
        //将顶点分配给这个float
        //三角坐标
        TraVertex[0] = a.x;
        TraVertex[1] = a.y;
        TraVertex[2] = a.z;
        //平面法向量实现光照
        //这里改成用计算出来的点

        TraVertex[3] = traNormol.x;
        TraVertex[4] = traNormol.y;
        TraVertex[5] = traNormol.z;
        //纹理坐标
        TraVertex[6] = 0.0f;
        TraVertex[7] = 0.0f;
        TraVertex[8] = b.x;
        TraVertex[9] = b.y;
        TraVertex[10] = b.z;
        TraVertex[11] = traNormol.x;
        TraVertex[12] = traNormol.y;
        TraVertex[13] = traNormol.z;
        TraVertex[14] = 1.0f;
        TraVertex[15] = 0.0f;


        TraVertex[16] = c.x;
        TraVertex[17] = c.y;
        TraVertex[18] = c.z;
        TraVertex[19] = traNormol.x;
        TraVertex[20] = traNormol.y;
        TraVertex[21] = traNormol.z;
        TraVertex[22] = 0.5f;
        TraVertex[23] = 1.0f;

        //绑定到vbo里
        glBindVertexArray(PolyVAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, PolyVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TraVertex), TraVertex, GL_STATIC_DRAW);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
}



//闭线带洞的三角剖分
void closePoly2Tri(vector<VERTEX>& closeOut, vector<vector<VERTEX>>& closeHoles, int index)
{
    //转换
    vector<Point*> out = VertexsToPoints(closeOut, 0);
    polylines[index].push_back(out);
    cdt[index] = new CDT(out);
    //每一层的洞不一样，但是尼玛这个还是轮廓线拼接啊，说好的这个三棱柱呢？
    //加洞
    for(int i = 0;i < closeHoles.size();i++){
        vector<Point*> hole = VertexsToPoints(closeHoles[i], i+1);
        cdt[index]->AddHole(hole);
    }

    //最大三角形
//    Point* p1 = new Point(-1.5, 1, 1.0);
//    Point* p2 = new Point(2.1, -1, 1.0);
//    cdt[index]->AddPoint(p1);
//    cdt[index]->AddPoint(p2);

//    //手动添加分支效果
//    Point* p1 = new Point(0.51, 1.142, 1.0);
//    Point* p2 = new Point(0.487, 0.773, 1.0);
//    Point* p3 = new Point(0.448, 0.412, 1.0);
//    Point* p4 = new Point(0.387, 0.12, 1.0);
//    Point* p5 = new Point(0.31, -0.164, 1.0);
//    Point* p6 = new Point(0.279, -0.494, 1.0);
//    Point* p7 = new Point(0.241, -0.747, 1.0);
//
//    p2->isHole = 2;
//    p3->isHole = 2;
//    p4->isHole = 2;
//    p5->isHole = 2;
//    p6->isHole = 2;
//    p7->isHole = 2;
////    cdt[index]->AddPoint(p1);
//    cdt[index]->AddPoint(p2);
//    cdt[index]->AddPoint(p3);
//    cdt[index]->AddPoint(p4);
//    cdt[index]->AddPoint(p5);
//    cdt[index]->AddPoint(p6);
////    cdt[index]->AddPoint(p7);


    //手动添加分支
//    vector<Point*> hand {new Point(0.51, 1.142, 1.0),
//                         new Point(0.487, 0.773, 1.0),
//                         new Point(0.448, 0.412, 1.0),
//                         new Point(0.387, 0.12, 1.0),
//                         new Point(0.31, -0.164, 1.0),
//                         new Point(0.279, -0.494, 1.0),
//                         new Point(0.241, -0.747, 1.0)};
//
//    polylines[index].push_back(hand);




//    polylines[index].push_back(hole);
    //开始剖分
    cdt[index]->Triangulate();

    //map是完整的剖分（包含空洞的剖分）？
    map[index] = cdt[index]->GetMap();
    triangles[index] = cdt[index]->GetTriangles();

}


unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

//判断直线是否相交
//bool lineIntersectSide(VERTEX A, VERTEX B, VERTEX C, VERTEX D)
//{
//    float fC = (C.y - A.y) * (A.x - B.x) - (C.x - A.x) * (A.y - B.y);
//    float fD = (D.y - A.y) * (A.x - B.x) - (D.x - A.x) * (A.y - B.y);
//    if(fC * fD > 0)
//        return false;
//    return true;
//
//}
////主要原理就是另一条线的一个点是否在一条直线的同一侧？如果两条线都不在同一侧那么就是不想交的。
//bool sideIntersectSide(VERTEX A, VERTEX B, VERTEX C, VERTEX D)
//{
//    if(!lineIntersectSide(A, B, C, D))
//        return false;
//    if(!lineIntersectSide(C, D, A, B))
//        return false;
//
//    return true;
//}
//
////判断这两个断层是否相交
////
//bool faultIntersect(VERTEX fault1[], int f1Number, VERTEX fault2[], int f2Number)
//{
//    //断层里的线是依次的，基本上第一个点的ip是基本在同一起点，那么还是要判断多重啊。不能单一的判断，那么就是一个线段是否与另一条的所有线段相交
//    //如果相交则要平移一下，但是这样感觉效率好低啊，不管了，先这样做吧。应该计算量不大，这个东西的话是一个基本的算法，时间复杂度应该不高。
//    for(int i = 0; i < f1Number - 1; i++)
//    {
//        for(int j = 0; j < f2Number - 1; j++)
//        {
//            //如果有任意一条相交，那么该线就需要平移。
//            if(sideIntersectSide(fault1[i],fault1[i+1], fault2[j], fault2[j+1]))
//                return false;
//        }
//    }
//    return true;
//}
//
////平移，哪个断层，平移多少,方向是什么？
////到底是传入vertex还是数组啊？感觉好像
//void faultMoveFunction(VERTEX *vertex, int num, float moveSize, int whichDirection)
//{
//    cout<<"movefunction"<<endl;
//    //先写一下这个yd得了
//    if(whichDirection == xD)
//    {
//        for(int i = 0;i < num; i++)
//        {
//            vertex[i].x += moveSize;
//        }
//    }
//    if(whichDirection == yD)
//    {
//        //从1开头，然后加等于2
//        for(int i = 0;i < num; i++)
//        {
//            vertex[i].y += moveSize;
//        }
//    }
//    if(whichDirection == zD)
//    {
//        for(int i = 0;i < num;i++)
//        {
//            vertex[i].z += moveSize;
//        }
//    }
//}
//
//VERTEX * faultMerge(VERTEX vertex1[], int num1, VERTEX vertex2[], int num2)
//{
//    VERTEX *merge = new VERTEX[num1 + num2];
//    //不用考虑排序就这样插入吧。
//    for(int i = 0;i < num1;i++)
//    {
//        merge[i] = vertex1[i];
//    }
//
//    for(int i = num1,j = 0;j < num2;j++)
//    {
//        merge[i++] = vertex2[j];
//    }
//    return merge;
//}


//基于三棱柱的检查方式，看是否有相关的不合法三角形
void checkTri(int index){
    if(index < 0)
        return;
    int triSize = triangles[index].size();

    cout << "begin check " << triSize << endl;
    //检查所有的三角，是否有在同一平面的？我感觉压根判断不了啊，十分的尴尬了老铁。
    for(int i = 0;i < triSize;i++){
        int triStatus = triangles[index][i]->IsFalseTri();



        //如果是顶部三角，那么直接进行插点操作
        if(triStatus == IsTopTri || triStatus == IsMidTri || triStatus == IsBaseTri){
//            triangles[index][i]->DebugPrint();
            //这里都不用hide了
//            triangles[index][i]->isHide = true;
            //设置这个z的大小，有一定的弧度
            Point* center = new Point;
            triangles[index][i]->GetCircleCenter(center);
            SetPointZ(closeLineV[index], closeLineV[index+1], center);
            //这个三角的z值怎么算？直接减去一半的dif？
//            center->z = 1.5;
            //这里指定插值的hole，到时候跟着进行移动。
            center->isHole = -1;
//            center->print();
            cdt[index]->AddPoint(center);
        }
    }
    //重新剖分后就有问题啊老铁，这样都分不出来哪些是插值的，哪些是旧的。
    cdt[index]->TriangleClear();
    cdt[index]->Triangulate();
    triangles[index] = cdt[index]->GetTriangles();
    //map是完整的剖分（包含空洞的剖分）？
//    map[index] = cdt[index]->GetMap();

//    //侧视图效果代码
//    triangles[index] = cdt[index]->GetTriangles();
//    triSize = triangles[index].size();
//
//    for(int i = 0;i < triSize;i++){
//        int triStatus = triangles[index][i]->IsFalseTri();
//        if(triStatus == 0){
//            triangles[index][i]->isHide = true;
//        }
//    }
//    cout << "after check " << triSize << endl;


}


//最短对角线的方法，解决这个？
void checkTriShort(int index){
    if(index < 0)
        return;
    int triSize = triangles[index].size();

    Point *center = new Point(-0.323, 0.402, 2.234);
//    for(int i = 0;i < closeLineV[index+1].size();i++){
//        for(int j = 0;j < closeLineV[index+1][i].size();j++){
//            if(closeLineV[index+1][i][j].x == -0.323){
//                cout << "niubi " << endl;
//                closeLineV[index+1][i][j].Print();
//                center->x = closeLineV[index+1][i][j].x;
//                center->y = closeLineV[index+1][i][j].y;
//                center->z = closeLineV[index+1][i][j].z;
//            }
//        }
//    }
    center->print();

    cout << "begin check " << triSize << endl;
    //检查所有的三角，是否有在同一平面的？我感觉压根判断不了啊，十分的尴尬了老铁。
    for(int i = 0;i < triSize;i++){
        int triStatus = triangles[index][i]->IsFalseTri();
        //如果是底部三角
        Triangle &t = *triangles[index][i];
        if(triStatus == IsBaseTri || triStatus == IsTopTri){
            t.isHide = true;
//            cout << "t before: " ;
//            t.DebugPrint();
        }else{
            for(int j = 0;j < 3;j++){
                if(t.GetPoint(j)->x - (-0.913) < 0.001){
                    for(int k = 0;k < 3;k++){
                        if(t.GetPoint(k)->x - (-1.075) < 0.001){
                            t.isHide = true;
                        }
                    }
                }
            }
            for(int j = 0;j < 3;j++) {
                if ( abs(t.GetPoint(j)->x  - (1.681))  <  0.001) {
                    t.DebugPrint();
                    for (int k = 0; k < 3; k++) {
                        if (abs(t.GetPoint(k)->x - 1.895) < 0.001) {
                            t.isHide = true;
                        }
                    }
                }
            }
        }
    }

    Point *c = new Point(-0.876,0.726,1.234);
    Point *d = new Point(-1.075,0.594,1.234);
    Point *e = new Point(-1.5,0.5,1.234);
    Point *f = new Point(-1.768,0.358,1.234);
    Point *g = new Point(-1.76,0.122,1.234);
    Point *h = new Point(-1.576,-0.099,1.234);
    Point *i = new Point(-1.355,-0.173,1.234);
    Point *j = new Point(-0.913,-0.158,1.234);
    triangles[index].push_back(new Triangle(*c,*d,*center));
    triangles[index].push_back(new Triangle(*d,*e,*center));
    triangles[index].push_back(new Triangle(*e,*f,*center));
    triangles[index].push_back(new Triangle(*f,*g,*center));
    triangles[index].push_back(new Triangle(*g,*h,*center));
    triangles[index].push_back(new Triangle(*h,*i,*center));
    triangles[index].push_back(new Triangle(*i,*j,*center));


    Point *u = new Point(2.418, 0.969,1.234);
    Point *e2 = new Point(1.681,1.22,2.234);
    Point *d2 = new Point(1.438,1.124,2.234);
    Point *c2 = new Point(1.224,1.08,2.234);
    Point *b2 = new Point(1,1,2.234);
    Point *a2 = new Point(1.084,0.8,2.234);
    Point *z1 = new Point(1.305,0.748,2.234);
    Point *w1 = new Point(1.622,0.756,2.234);
    Point *v1 = new Point(1.895,0.763,2.234);

    triangles[index].push_back(new Triangle(*e2,*d2,*u));
    triangles[index].push_back(new Triangle(*d2,*c2,*u));
    triangles[index].push_back(new Triangle(*c2,*b2,*u));
    triangles[index].push_back(new Triangle(*b2,*a2,*u));
    triangles[index].push_back(new Triangle(*a2,*z1,*u));
    triangles[index].push_back(new Triangle(*z1,*w1,*u));
    triangles[index].push_back(new Triangle(*w1,*v1,*u));

}

//获取插值点的高度，基于老师给的公式。
//这里不好传入带洞的分支，因为都是已经做了一次区分了
void SetPointZ(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in, Point* p){
    double disOut = 1e9, disIn = 1e9;
    VERTEX point;
    point.x = p->x;
    point.y = p->y;
    for(int i = 0;i < out.size();i++){
        //环线计算
        for(int j = 0;j < out[i].size() - 1;j++) {
            disOut = min(disOut, (double)DistanceOfPointAndLine(point, out[i][j], out[i][j+1]));
//            disOut = min(disOut, sqrt(pow((out[i][j].x - p->x), 2) + pow((out[i][j].y - p->y), 2)));
        }
        //这里再算一下初始的距离
        disOut = min(disOut, (double)DistanceOfPointAndLine(point, out[i][out[i].size() - 1], out[i][0]));
    }
    for(int i = 0;i < in.size();i++){
        for(int j = 0;j < in[i].size() - 1;j++){
            //这里求的是点到点的距离啊，有点不太对，应该要算点到直线的距离，也就是两个相邻的点所形成的直线。

            disIn = min(disIn, (double)DistanceOfPointAndLine(point, in[i][j], in[i][j+1]));
//            disIn = min(disIn, sqrt(pow((in[i][j].x - p->x),2) + pow((in[i][j].y - p->y), 2)));
//            in[i][j].Print();
//            in[i][j+1].Print();
//            cout << "disIn: " << disIn << endl;
        }
        disIn = min(disIn, (double)DistanceOfPointAndLine(point, in[i][in[i].size() - 1], in[i][0]));
//        cout << "disIn1: " << disIn << endl;
    }
//    cout << "disOut: " << disOut << endl;
//    cout << "disIn: " << disIn << endl;

    p->inDistance = disIn;
    p->outDistance = disOut;
    double a = disIn / (disIn + disOut);
    double zOut = out[0][0].z, zIn = in[0][0].z;
    p->z = a * zOut + (1 - a) * zIn;
//    cout << a << " | " << p->z << endl;
}





//void VertexMerge(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in){
//    closeLineMerge.clear();
//
//    for(int i = 0;i < out.size();i++){
//        for(int j = 0; j < out[i].size();j++){
//            closeLineMerge.push_back(out[i][j]);
//        }
//    }
//
//    //内轮廓的距离与总权重计算
//    for(int i = 0;i < in.size();i++){
//        for(int j = 0; j < in[i].size();j++){
//            closeLineMerge.push_back(in[i][j]);
//        }
//    }
//
//    cout << "merge success"<< endl;
//}


//这个插值方法好像没有用到z值？
void SetPointZofIDW(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in, Point* p){
    VERTEX point;
    point.x = p->x;
    point.y = p->y;

    //从0开始到底对不对呢？还是感觉不是很对的样子了
    point.z = 0;

//    point.Print();

    float f = 0;

    //外轮廓的距离与总权重计算
    for(int i = 0;i < out.size();i++){
        for(int j = 0; j < out[i].size();j++){
            out[i][j].distance = DistanceOfPointAndPoint(out[i][j], point);
//            cout << out[i][j].distance << endl;
            f += pow(1.0 / out[i][j].distance, r);
//            cout << f << endl;
        }
    }

    //内轮廓的距离与总权重计算
    for(int i = 0;i < in.size();i++){
        for(int j = 0; j < in[i].size();j++){
            in[i][j].distance = DistanceOfPointAndPoint(in[i][j], point);
            f += pow(1.0 / in[i][j].distance, r);
        }
    }

    //外轮廓的单个点权重计算
    for(int i = 0;i < out.size();i++){
        for(int j = 0; j < out[i].size();j++){
            //距离越远，权重值越小，因为这里是倒数放进去的。
            out[i][j].weight = pow(1.0/out[i][j].distance, r) / f;
        }
    }

    //内轮廓的单个点权重计算
    for(int i = 0;i < in.size();i++){
        for(int j = 0; j < in[i].size();j++){
            in[i][j].weight = pow(1.0/in[i][j].distance, r) / f;
        }
    }


    //得到插值点的权重
    point.weight = 0;

    //计算外轮廓
    for(int i = 0;i < out.size();i++){
        for(int j = 0; j < out[i].size();j++){
            //这里是用了z值，那么如果我用这个初始值为0应该是没什么毛病的，如果原轮廓的值是负值，那么加起来也是负的。怎么去修改这个函数呢？
            point.z += out[i][j].weight * out[i][j].z;
        }
    }

    //计算内轮廓
    for(int i = 0;i < in.size();i++){
        for(int j = 0; j < in[i].size();j++){
            point.z += in[i][j].weight * in[i][j].z;
        }
    }
    point.Print();
    p->z = point.z;
}


//用边来代替这个权重试一下看看，完全就是行不通的了老铁。
void SetPointZofIDWOfLine(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in, Point* p){
    VERTEX point;
    point.x = p->x;
    point.y = p->y;

    //从0开始到底对不对呢？还是感觉不是很对的样子了
    point.z = 0;

    point.Print();

    float f = 0;

    //外轮廓的距离与总权重计算
    for(int i = 0;i < out.size();i++){
        for(int j = 0; j < out[i].size();j++){
            out[i][j].distance = DistanceOfPointAndPoint(out[i][j], point);
            cout << out[i][j].distance << endl;
            f += pow(1.0 / out[i][j].distance, r);
//            cout << f << endl;
        }
    }

    //内轮廓的距离与总权重计算
    for(int i = 0;i < in.size();i++){
        for(int j = 0; j < in[i].size();j++){
            in[i][j].distance = DistanceOfPointAndPoint(in[i][j], point);
            f += pow(1.0 / in[i][j].distance, r);
        }
    }

    //外轮廓的单个点权重计算
    for(int i = 0;i < out.size();i++){
        for(int j = 0; j < out[i].size();j++){
            //距离越远，权重值越小，因为这里是倒数放进去的。
            out[i][j].weight = pow(1.0/out[i][j].distance, r) / f;
        }
    }

    //内轮廓的单个点权重计算
    for(int i = 0;i < in.size();i++){
        for(int j = 0; j < in[i].size();j++){
            in[i][j].weight = pow(1.0/in[i][j].distance, r) / f;
        }
    }


    //得到插值点的权重
    point.weight = 0;

    //计算外轮廓
    for(int i = 0;i < out.size();i++){
        for(int j = 0; j < out[i].size();j++){
            //这里是用了z值，那么如果我用这个初始值为0应该是没什么毛病的，如果原轮廓的值是负值，那么加起来也是负的。怎么去修改这个函数呢？
            point.z += out[i][j].weight * out[i][j].z;
        }
    }

    //计算内轮廓
    for(int i = 0;i < in.size();i++){
        for(int j = 0; j < in[i].size();j++){
            point.z += in[i][j].weight * in[i][j].z;
        }
    }
    point.Print();
    p->z = point.z;
}



//求每一层的质心
void GetSecCenter(vector<vector<VERTEX>>& sec, int index){
    int cnt = 0;
    double sumX = 0.0, sumY = 0.0;
    for(int i = 0;i < sec.size();i++){
        for(int j = 0;j < sec[i].size();j++){
//            cout << cnt << endl;
            cnt++;
            sumX += sec[i][j].x;
            sumY += sec[i][j].y;
        }
    }
    VERTEX c;
    c.x = sumX / cnt;
    c.y = sumY / cnt;

    //这里把高度也放进去，都默认是统一平行吧。
    c.z = sec[0][0].z;
    center[index] = c;

}

void SecAligned(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in, int index, bool isR){

    double difX, difY;
    if(isR){
        difX = center[index].x - center[index+1].x;
        difY = center[index].y - center[index+1].y;
    }else{
        //非翻转的情况是上减下
        difX = center[index+1].x - center[index].x;
        difY = center[index+1].y - center[index].y;
    }

    cout << "dif: " << difX << " " <<  difY<< endl;
//简单的质心对齐？
    for(int i = 0;i < in.size();i++){
        for(int j = 0;j < in[i].size();j++){
            in[i][j].x -= difX ;
            in[i][j].y -= difY ;
        }
    }
}

void SecAlignedToCenter(vector<vector<VERTEX>>& in, int index){
    Point *zero = new Point(0.0, 0.0, 0.0);
    double difX, difY;
    difX = center[index].x - zero->x;
    difY = center[index].y - zero->y;
    cout << "dif: " << difX << " " <<  difY<< endl;
//简单的质心对齐？
    for(int i = 0;i < in.size();i++){
        for(int j = 0;j < in[i].size();j++){
            in[i][j].x -= difX;
            in[i][j].y -= difY;
        }
    }

}

void SecScale(int index, bool isR){

    //这里已经是质心对齐了的，所以直接取上和下都是一样。
    scaleSize[index] = 1.0;
//    vector<VERTEX> mergeSec;
//    for(int i = 0;i < in.size();i++){
//        mergeSec.insert(mergeSec.end(), in[i].begin(), in[i].end());
//    }
    cout << "begin scale judge" << endl;

    while(!PolygonInPolygon(index, isR)){
        //反面，对底边进行放缩
        if(isR){
            SecScaleFunction(closeLineV[index], 1.0 / scaleSize[index], index);
            scaleSize[index] -= 0.1;
            SecScaleFunction(closeLineV[index], scaleSize[index], index);
            cout << "scale: " << scaleSize[index]  << endl;
        }else{
            SecScaleFunction(closeLineV[index+1], 1.0 / scaleSize[index], index);
            scaleSize[index] -= 0.1;
            SecScaleFunction(closeLineV[index+1], scaleSize[index], index);
            cout << "scale: " << scaleSize[index]  << endl;
        }
        //因为这里是循环去调用，所以每次都先还原，然后再加大这个分量进行放缩
    }
}


void SecScaleFunction(vector<vector<VERTEX>>& in, double scaleSize, int index){
    for(int i = 0;i < in.size();i++){
        for(int j = 0;j < in[i].size();j++){
            //基于底层重心进行变换是没什么毛病的。
            in[i][j].x = (1 - scaleSize) * center[index].x + scaleSize * in[i][j].x;
            in[i][j].y = (1 - scaleSize) * center[index].y + scaleSize * in[i][j].y;
        }
    }
}


//剖面还原，还原应该是要以自己的重心为准。以下层的为准也行，后面加上相应的dif即可
void SecBack(int index, bool isR){

    //这里如果index是0的话，那么就变成-1了？

    double difX, difY;
    if(isR){
        difX = center[index].x - center[index+1].x;
        difY = center[index].y - center[index+1].y;
    }else{
        //非翻转的情况是上减下
        difX = center[index+1].x - center[index].x;
        difY = center[index+1].y - center[index].y;
    }
    int lineNum;

    //翻转了就是对上层进行还原？之前自己写的不是很对吗？
    if(isR){
        lineNum = index;
    }else{
        lineNum = index+1;
    }

    //TODO:线的还原暂时没有做，因为本来就是面里有线，所以这个东西如果后期要展示的话，那么需要单独处理一下了。像mc里的方法，直接给抽出来。
    for(int i = 0;i < closeLineV[lineNum].size();i++){
        for(int j = 0;j < closeLineV[lineNum][i].size();j++){
//            cout << closeLineV[lineNum][i][j].x<< endl;
            closeLineV[lineNum][i][j].x = (1 - 1.0/scaleSize[index]) * center[index].x + 1.0/scaleSize[index] * closeLineV[lineNum][i][j].x + difX;
            closeLineV[lineNum][i][j].y = (1 - 1.0/scaleSize[index]) * center[index].y + 1.0/scaleSize[index] * closeLineV[lineNum][i][j].y + difY;
//            cout << "- " << closeLineV[lineNum][i][j].x << " " <<closeLineV[lineNum][i][j].z <<  endl;
        }
    }
}

//将三角面片进行还原

//index当前层次，isR是否是翻转
void TriBack(int index, bool isR){

    if(index < 0)
        return;
    int traSize = triangles[index].size();
//    cout << traSize << endl;

    //求得两个质心的差值
    double difX, difY;

    //翻转情况，dif的差别是下减上
    if(isR){
        difX = center[index].x - center[index+1].x;
        difY = center[index].y - center[index+1].y;
    }else{
        //非翻转的情况是上减下
        difX = center[index+1].x - center[index].x;
        difY = center[index+1].y - center[index].y;
    }
    cout <<"index: " << index << " " << "dif " << difX << " " << difY << endl;
    center[index].Print();
    center[index+1].Print();
    for (int i = 0; i < traSize; i++) {
        Triangle &t = *triangles[index][i];
        for (int j = 0; j < 3; j++) {
            Point &point = *t.GetPoint(j);
            //如果已经移动了，或者是底层剖面，则不需要进行处理
            if(point.isMove || point.isHole == 0)
                continue;

            //先进行放缩，这里可以基于之前的质心进行放缩。后面可以直接加上当前的分量就好了，没毛病。

            //正常轮廓线上的点
            if(point.isHole >= 0){
                //是以自己的质心为基准在平移？
                //还原还是基于这个重心的值进行还原，应该是没什么问题。
                point.x = (1 - 1.0/scaleSize[index]) * center[index].x + 1.0/scaleSize[index] * point.x + difX;
                point.y =  (1 - 1.0/scaleSize[index]) * center[index].y + 1.0/scaleSize[index] * point.y + difY;
            }else{
                //插值点
                //计算比例
                double proportion, proportionZ;
                //翻转情况
                if(isR){
                    proportionZ = 1 - (center[index+1].z - point.z) / (center[index+1].z - center[index].z);
//                    point.print();
                    proportion = point.outDistance / ( point.outDistance + point.inDistance);
//                    cout << "proportion: " << proportion << " " << proportionZ << endl;
//                    point.x = ((1 - 1.0/(scaleSize[index] + (scaleSize[index] /(1/proportion)))) * center[index].x + (1.0/(scaleSize[index] + (scaleSize[index] /(1/proportion)))) * point.x  +  proportion * difX);
//                    point.y = ((1 - 1.0/(scaleSize[index] + (scaleSize[index] /(1/proportion)))) * center[index].y + (1.0/(scaleSize[index] + (scaleSize[index] /(1/proportion)))) * point.y  +  proportion * difY);

                    if(scaleSize[index] - 1.0  == 0){
                        point.x = point.x + proportionZ * difX;
                        point.y = point.y + proportionZ * difY;
                    }else{
//                        point.x = (1 - 1.0/(scaleSize[index] + scaleSize[index] / proportionZ)) * center[index].x
//                                  + 1.0/(scaleSize[index] + scaleSize[index] / proportionZ) * point.x  +  proportionZ * difX;
//                        point.y = (1 - 1.0/(scaleSize[index] + scaleSize[index] / proportionZ)) * center[index].y
//                                  + 1.0/(scaleSize[index] + scaleSize[index] / proportionZ) * point.y  +  proportionZ * difY;

                        point.x = (1 - 1.0/(scaleSize[index] + (1-scaleSize[index]) * proportionZ)) * center[index].x
                                  + 1.0/(scaleSize[index] +  (1-scaleSize[index])  * proportionZ) * point.x  +  proportionZ * difX;
                        point.y = (1 - 1.0/(scaleSize[index] +  (1-scaleSize[index])  * proportionZ)) * center[index].y
                                  + 1.0/(scaleSize[index] +  (1-scaleSize[index]) * proportionZ) * point.y  +  proportionZ * difY;
                    }
//                    point.x = (1 - 1.0/proportion) * center[index].x + 1.0/(proportion) * point.x  +  proportion * difX;
//                    point.y = (1 - 1.0/proportion) * center[index].y + 1.0/(proportion) * point.y  +  proportion * difY;

                    //若插值点在轮廓线内
//                    VERTEX *v = new VERTEX(point.x, point.y);
//                    if(InPolygon(closeLineV[index], *v)){
//
//                    }else{
//                        //不在轮廓线内就不需要计算高度
//                        SetPointZ(closeLineV[index], closeLineV[index+1], &point);
//                    }
//                    SetPointZ(closeLineV[index], closeLineV[index+1], &point);
                    //先把这个比例给弄上去
//                    point.x =   point.x + proportion * difX * (point.inDistance / (point.outDistance));
//                    point.y =   point.y + proportion * difY * (point.inDistance / (point.outDistance));
//                    point.print();
//                    //重新计算高度
//                    point.print();
                }else{
                    proportionZ = 1 - (point.z - center[index].z) / (center[index+1].z - center[index].z);

                    proportion = point.inDistance / ( point.outDistance + point.inDistance);
//                    cout << "proportion: " << proportion << " " << proportionZ << endl;

//                    point.print();
//                    cout << proportion << " " << proportionZ << endl;

//                    point.x = ((1 - 1.0/scaleSize[index] * proportion) * center[index].x + (1.0/scaleSize[index]) * proportion * point.x  +  proportion * difX);
//                    point.y = ((1 - 1.0/scaleSize[index] * proportion) * center[index].y + (1.0/scaleSize[index]) * proportion * point.y  +  proportion * difY);

//                    point.x = (1 - 1.0/proportion) * center[index].x + 1.0/proportion * point.x + difX * proportion;
//                    point.y =  (1 - 1.0/proportion) * center[index].y + 1.0/proportion * point.y + difY * proportion;


//                    cout << "scale " << scaleSize[index] << endl;

                    if(scaleSize[index] - 1.0  == 0){
                        point.x = point.x + proportionZ * difX;
                        point.y = point.y + proportionZ * difY;
                    }else{
//                        point.x = (1 - 1.0/(scaleSize[index] + scaleSize[index] * proportionZ)) * center[index].x
//                                + 1.0/(scaleSize[index] + scaleSize[index] * proportionZ) * point.x  +  proportionZ * difX;
//                        point.y = (1 - 1.0/(scaleSize[index] + scaleSize[index] * proportionZ)) * center[index].y
//                                + 1.0/(scaleSize[index] + scaleSize[index] * proportionZ) * point.y  +  proportionZ * difY;


                        point.x = (1 - 1.0/(scaleSize[index] + (1-scaleSize[index]) * proportionZ)) * center[index].x
                                  + 1.0/(scaleSize[index] +  (1-scaleSize[index])  * proportionZ) * point.x  +  proportionZ * difX;
                        point.y = (1 - 1.0/(scaleSize[index] +  (1-scaleSize[index])  * proportionZ)) * center[index].y
                                  + 1.0/(scaleSize[index] +  (1-scaleSize[index]) * proportionZ) * point.y  +  proportionZ * difY;
                    }
//                    point.x = point.x + difX * (1-proportion);
//                    point.y = point.y + difY * (1-proportion);
//                    SetPointZ(closeLineV[index], closeLineV[index+1], &point);
//                    point.x = (1 - proportion) * center[index].x + proportion * point.x;
//                    point.y = (1 - proportion) * center[index].y + proportion * point.y;

                    //先把这个比例给弄上去
//                    point.x =   point.x + proportion * difX * (point.outDistance / point.inDistance);
//                    point.y =   point.y + proportion * difY * (point.outDistance / point.inDistance);
//                    point.print();
                    //重新计算高度
//                    SetPointZ(closeLineV[index], closeLineV[index+1], &point);
//                    point.print();
                }



            }

            point.isMove = true;
        }
    }
}


//对所有的三角形进行插值，times为插值次数
void TriInsert(int times, int i){
    if(times < 1)
        return;

    cout << "begin tri insert" << endl;
    unsigned long triSize;
    //对每一层进行插值
    while(times--){
        //这样插值是不是效率有点慢啊
        //这里层次要减少1，因为插值的时候
            triSize = triangles[i].size();
            for(int j = 0;j < triSize;j++){
                //对所有的三角形进行插值
                Point* center = new Point;
                //用外接圆心可能容易出事，如果有狭长的三角形那点都可能跑到外面去了。
                //此处是求三角形的重心，考虑了z轴的值？
                triangles[i][j]->GetCenter(center);

                //这里应该是可以取消插值，再最后一次再进行插值计算？应该是可以的，这样可以大大减少时间。
                //不行啊，完全就是取消不了这个的，也是难顶。
                if(IDWOpen){
                    SetPointZofIDW(closeLineV[i], closeLineV[i+1], center);
                }else{
                    SetPointZ(closeLineV[i], closeLineV[i+1], center);
                }


                center->isHole = -1;
                cdt[i]->AddPoint(center);
            }
            cdt[i]->TriangleClear();
            //这个函数操作是应该会改变triangles这个函数指针的
            cdt[i]->Triangulate();
            triangles[i] = cdt[i]->GetTriangles();
    }
    cout <<"the " << i << " nums of tri is " <<  triangles[i].size() << endl;
}

//这里来检测所有的三角形，计算它们的插值点和高度值等来算这个美化度
void MeihuaCheck(int index){
    if(index < 0)
        return;
    int triSize = triangles[index].size();
    //美化度总值
    double bt = 0.0;

    //内切圆半径和外接圆半径
    double innerR, outerR;
    for(int i = 0;i < triSize;i++) {


        //计算最大包围盒
        for(int j = 0;j < 3;j++){
            maxX = max(triangles[index][i]->points_[j]->x, maxX);
            minX = min(triangles[index][i]->points_[j]->x, minX);
            maxY = max(triangles[index][i]->points_[j]->y, maxY);
            minY = min(triangles[index][i]->points_[j]->y, minY);
        }

        //对每个三角计算美化度试试
        Point *outerCenter = new Point;
        Point *innerCenter = new Point;

        triangles[index][i]->GetCircleCenter(outerCenter);
        triangles[index][i]->GetInnerCenter(innerCenter);



//        cout << "outCenter1: ";
//        outerCenter->print();
//        cout << "outCenter2: ";

        //不应该在这里求高度值啊
//        SetPointZ(closeLineV[index], closeLineV[index+1], outerCenter);
//        outerCenter->print();


        //外接圆心到三角形任意一点的距离就是外接圆心的半径，这个是没什么问题的。
        outerR = sqrt(pow(triangles[index][i]->points_[0]->x - outerCenter->x, 2) +
                      pow(triangles[index][i]->points_[0]->y - outerCenter->y, 2) +
                      pow(triangles[index][i]->points_[0]->z - outerCenter->z, 2));
        //内切圆圆心是点到线段的距离，那么这里就随意设定一个线段
        //问题应该是出在这里了，计算的是2d的。那么要找一个3d的了。
        innerR = shortDistance(*triangles[index][i]->points_[0], *triangles[index][i]->points_[1], *innerCenter);
//        triangles[index][i]->DebugPrint();
//        cout << "meihua " << innerR << " " << outerR << " " << innerR / outerR << endl;

        bt += innerR / outerR;
        //这里没有这个高度值啊，十分的难顶了。
    }
    cout << "final bt value: " << bt << "avg: " << bt/triSize <<  endl;
}


//判断点是否都在多边形内，如果都在的话那么就是可以说明不用平移了

//有一个点不在多边形内，返回false
//点在多边形内，返回true
bool PolygonInPolygon(int index, bool isR){
    if(index < 0)
        return false;
//    vector<VERTEX> mergeSec;
//    if(isR) {
//        //相反的话就把自己底层给拼起来计算
//        for (int i = 0; i < closeLineV[index].size(); i++) {
//            mergeSec.insert(mergeSec.end(), closeLineV[index][i].begin(), closeLineV[index][i].end());
//        }
////        cout << "begin scale judge" << endl;
//        //这里判断的是合并的，但是我变换的是in？
//
//        //反正这个0肯定是要计算的，或者都算进去？
//        if (faultIntersect(closeLineV[index + 1][0], mergeSec)) {
//            return true;
//        } else {
//            return false;
//        }
//    }else {
//        //相反的话就把自己底层给拼起来计算
//        for (int i = 0; i < closeLineV[index+1].size(); i++) {
//            mergeSec.insert(mergeSec.end(), closeLineV[index+1][i].begin(), closeLineV[index+1][i].end());
//        }
////        cout << "begin scale judge" << endl;
//        //这里判断的是合并的，但是我变换的是in？
//
//        //反正这个0肯定是要计算的，或者都算进去？
//        if (faultIntersect(closeLineV[index][0], mergeSec)) {
//            return true;
//        } else {
//            return false;
//        }
//    }

    if(isR) {
        //相反的话就把自己底层给拼起来计算
        for (int i = 0; i < closeLineV[index].size(); i++) {
            if (!faultIntersect(closeLineV[index + 1][0], closeLineV[index][i])) {
                return false;
            }
        }
    }else {
        //相反的话就把自己底层给拼起来计算
        for (int i = 0; i < closeLineV[index+1].size(); i++) {
                //单层轮廓判断
            if (!faultIntersect(closeLineV[index][0], closeLineV[index+1][i])) {
                return false;
            }
        }
    }
    return true;

}


void PrintTime(const char * str, clock_t start, clock_t end){
    cout << str << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
}