#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

#define  MaxNum 1024

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
vector<Triangle*> triangles[MaxNum];
/// Triangle map
list<Triangle*> map[MaxNum];
/// Polylines
//这个好像还是个二维数组？有点牛批
vector< vector<Point*> > polylines[MaxNum];

vector<p2t::Point*> polyline[MaxNum];
//这个是用开画图的？
CDT* cdt[MaxNum];

//多出的三角形
vector<AddTriangle> extraTriangles[MaxNum];

//
vector<AddTriangle> McTri;

vector<vector<VERTEX>> McLine;

double mcScale = 1.0;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 1.0f;
float lastY = SCR_HEIGHT / 1.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//灯光的位置
glm::vec3 lightPos(1.0f, -2.0f, 3.0f);

//function
//设置一下变量，用按键实现一下功能

//是否开启Delaunay
bool DelaunayOpen = false;
bool Poly2TriOpen = false;

//是否开启耳切法
bool EarCutOpen = false;
//平移操作
bool faultMove = false;

bool moveBack = false;
bool moveBack1 = false;
int cntMove = 0;
int cntBack = 0;
//试一下这个是否有用啊。

//直接写一个指针数组来存数据，up为上层，down为下层
VERTEX *faultUp[MaxNum], *faultDown[MaxNum];

//断层VAO
unsigned int faultUpVBO[MaxNum], faultUpVAO[MaxNum], faultDownVBO[MaxNum], faultDownVAO[MaxNum];

//框架数据
unsigned int faceVBO[MaxNum], faceVAO[MaxNum];  //直接用你这个face

unsigned int textures[MaxNum][MaxNum];
unsigned int addTextures[MaxNum][MaxNum];


//VERTEX *fault2_up, *fault2_down;

//这个值为断层个数，现在暂时定为1个
int modelNum;


unsigned int DelTraVBOs[MaxNum], DelTraVAOs[MaxNum];
unsigned int PolyVBOs[MaxNum][MaxNum], PolyVAOs[MaxNum][MaxNum];
unsigned int AddVBOs[MaxNum][MaxNum], AddVAOs[MaxNum][MaxNum];

//移动立方体的数据
unsigned int McVBOs[1][MaxNum * 10], McVAOs[1][MaxNum * 10];

unsigned int VAOs[2], VBOs[2];


bool isAddTra[MaxNum] = {false};

//unsigned int EarVBOs[MaxNum], EarVAOs[MaxNum];

vector<vector<vector<VERTEX>>> closeLineV;


vector<VERTEX> closeLineMerge;

//全局变量，用来平移之类的

//记录每一层的原质心
vector<VERTEX> center;
//用来记录质心的平移量
vector<double> MoveSize;
//这个变量用来来记录面里平移的长度，一个面有两条线。每条线有三个方位的平移值
moveSize faultMoveSize[MaxNum][2][3];

//一个值就够了，用来记录每一层的平移量
double scaleSize[MaxNum];


//每一层的高度值
float diff[MaxNum];


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

void Poly2TriBind(unsigned int * PolyVAOs, unsigned int * PolyVBOs, unsigned int * texture,  vector<Triangle*> _triangle);

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
void SecAligned(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in, int index);

//质心对齐0
void SecAlignedToCenter(vector<vector<VERTEX>>& in, int index);

//基于质心放缩
void SecScale(vector<VERTEX>& out, vector<vector<VERTEX>>& in, int index);


//基于质心原点缩放
void SecScaleToCenter(vector<vector<VERTEX>>& in, int index);

//放缩函数
void SecScaleFunction(vector<vector<VERTEX>>& in, double scaleSize, int index);

//还原剖面
void SecBack(int index);

//还原三角面片
void TriBack(int index);

//对三角形进行插值，times为插值次数，次数越多，模型越精确
void TriInsert(int times);




void scaleFunction(vector<VERTEX>& fault1, vector<VERTEX>& fault2, int index)
{
//    int num1 = fault1.size(), num2 = fault2.size();
    scaleSize[index] = 1.0f;

    //判断是否有相交的线，这里效率也太低了，如果是那种奇怪的图形，估计也是弄不出来什么效果的。
    while(!faultIntersect(fault1, fault2))
    {
        //这里的改变0.1是不是有问题，如果两次之后那么就是0.9 * 0.8 = 0.72
        //还真是这样，
        //每次改变0.1
        //变化之前先把它乘回来，不知道这样是不是开销有点大，我估摸着时间复杂度是O(N)
        faultScaleFunction(fault2, 1.0f / (scaleSize[index]) ,xD);
        faultScaleFunction(fault2, 1.0f / (scaleSize[index]) ,yD);
        scaleSize[index] -= 0.3f;
//        cout << "index " << index << " fault[0] " << fault2[0].x << endl;
        cout << scaleSize[index] << endl;
        faultScaleFunction(fault2, scaleSize[index] ,xD);
        faultScaleFunction(fault2, scaleSize[index] ,yD);
//        cout << "after index " << index << " fault[0] " << fault2[0].x << endl;

    }
}
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
    vector<VERTEX> v;

    InputDataToVector(closeLineV);

    modelNum = closeLineV.size();
    center.resize(modelNum);


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
    for(int i = 0;i < modelNum - 1; i++)
    {
        //获取质心，这里质心的获取还是有点问题。
        cout << "center: ";
        center[i].Print();
        center[i+1].Print();
        //如果第二层比第一层轮廓线多，否则反过来？其实还是没用，需要做多层才行。
        if(closeLineV[i].size() <= closeLineV[i+1].size()){
            //放缩也得放进来
            cout << "scale before: " << closeLineV[i+1][0][0].x << endl;
            //质心对齐
            GetSecCenter(closeLineV[i], i);
            GetSecCenter(closeLineV[i+1], i+1);
            SecAligned(closeLineV[i], closeLineV[i+1], i);

//            轮廓线进行放缩
            SecScale(closeLineV[i][0], closeLineV[i+1], i);
            cout << "scale after: " << closeLineV[i+1][0][0].x << endl;
//            //这里的传入有点问题！不能传入这个0
            closePoly2Tri(closeLineV[i][0], closeLineV[i+1], i);
            //对三角形进行检查
            checkTri(i);
//
//            checkTriShort(i);
//            三角形还原
            TriBack(i);
//            轮廓线还原，这里是对up轮廓线进行还原
            SecBack(i);
        }else{
            //逆置一下
            //质心对齐
            SecAligned(closeLineV[i+1], closeLineV[i], i);
            //轮廓线进行放缩
            SecScale(closeLineV[i+1][0], closeLineV[i], i);
//            cout << "scale after: " << closeLineV[i+1][0][0].x << endl;
            //传入数据逆置
            closePoly2Tri(closeLineV[i+1][0], closeLineV[i], i);
            //剖分的时候还是真的吧
            checkTri(i);
            //三角形还原
            TriBack(i);
            //轮廓线还原，这里是对down轮廓线进行还原
            SecBack(i-1);

        }

        //画一下线

    }

    //做两次插值
    TriInsert(1);

    int faceNum = 0;
    for(int i = 0;i < modelNum;i++){
        //获取当前质心
        GetSecCenter(closeLineV[i], i);

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

void DrawLine(){

    //画等值面
    for(int j = 0; j < modelNum - 1 ; j++) {
        for(int i = 0; i < triangles[j].size(); i++) {
            if (!triangles[j][i]->isHide) {
                //激活一下这个纹理
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textures[j][i]);
                glBindVertexArray(PolyVAOs[j][i]);

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
}


//mc算法的问题
void MarchingCubesProcess(){
     MakeSurface m;
//     string file = "/Users/tanwenbo/CLionProjects/PaperProject/src/Circle.sec";
    string file = "/Users/tanwenbo/CLionProjects/PaperProject/src/1215.txt";
     m.ReadSectionData((char*)file.data());
     //创建好表面后，可以进行渲染了。
     m.CreateSurface2();

    //获取剖面个数
    cout << McTri.size() << endl;
    AddTriBind(McVAOs[0], McVBOs[0], textures[0], McTri);

    for(int i = 0;i < McLine.size();i++){
        drawInit(faceVAO[i], faceVBO[i], McLine[i]);
    }
}

void McProcess(){
//    readFile();

    SetSample(0.0);
    vMarchingCubes();
//
//    vector<VERTEX> v;
//
//    InputDataToVector(v);
//
//    for(int i = 0;i < v.size();i++) {
//        vMarchCube(v[i].x, v[i].y, v[i].z, 1.0/ 16.0);
//    }
    cout << McTri.size() << endl;
    AddTriBind(McVAOs[0], McVBOs[0], textures[0], McTri);

}

void DrawMc(){
    for(int i = 0; i < McTri.size();i++){

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0][i]);
        glBindVertexArray(McVAOs[0][i]);

        if(ShowTexture)
            glDrawArrays(GL_TRIANGLE_STRIP, 0 , 3);
        else
            glDrawArrays(GL_LINE_STRIP, 0, 3);
    }

    for(int i = 0;i < McLine.size();i++){
//        glBindVertexArray(VAOs[i]);
        glBindVertexArray(faceVAO[i]);
//            cout << McLine[i].size() << endl;
        glDrawArrays(GL_LINE_LOOP, 0, McLine[i].size());
    }

}



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
//    Shader lightingShader("7.4.camera.vs", "7.4.camera.fs");
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);


    //处理MC
//    McProcess();



    LineProcess();

//    MarchingCubesProcess();

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
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

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

        //渲染MC
//        DrawMc();
        DrawLine();



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1024, faceVAO);
    glDeleteBuffers(1024, faceVBO);
//    glDeleteVertexArrays(del->HowMany + 1 , DelTraVAOs);
//    glDeleteBuffers(del->HowMany + 1, DelTraVBOs);
    glDeleteVertexArrays(1024, AddVAOs[1023]);
    glDeleteBuffers(1024, AddVBOs[1023]);
    glDeleteVertexArrays(1024, PolyVAOs[1023]);
    glDeleteBuffers(1024, PolyVBOs[1023]);


    //这里不能直接释放1024，有很多都没有分配
    delete []faultUp[modelNum];
    delete []faultDown[modelNum];

//    glDeleteVertexArrays(1,&DelaunayVAO);
//    glDeleteBuffers(1, &DelaunayVBO);
//
//    glDeleteVertexArrays(1,&faultVAO);
//    glDeleteBuffers(1, &faultVBO);
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

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
        if(AddTrangle){
            AddTrangle = false;
        } else
            AddTrangle = true;
    }


//    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
//    {
//        //还是要设置一下绑定，不然总是触发多次。
//        //那还得加一个键啊，不然不能取消。
//        if(!DelaunayOpen)
//        {
//            cout<<"delaunay"<<endl;
//            //在这里计算del
//            //按了键才进行三角剖分，那么我改的话键是按了键后将输入存入进去，然后开始剖分。获取三角，
//            del->Init(faultMerge(faultUp[0], 10, faultUp[1], 10), 20);
//            //绑定缓冲
//            DelaunayBind(DelTraVAOs, DelTraVBOs, del->HowMany, del);
//            //打开剖分
//            DelaunayOpen = true;
//        }
//    }
//    //p 显示poly2tri
//    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
//    {
//        //还是要设置一下绑定，不然总是触发多次。
//        //那还得加一个键啊，不然不能取消。
//        if(!Poly2TriOpen)
//        {
//            cout<<"poly2tri"<<endl;
//
//            VERTEX *Merge = faultMerge(faultUp[0], 10, faultUp[1], 10);
//            //将vertex的xy坐标输入到这个poly
//            for(int i = 0;i < 20; i++)
//            {
//                double x = Merge[i].x;
//                double y = Merge[i].y;
//                double z = Merge[i].z;
//                Point *point = new Point(x,y);
//                point->z = z;
//                //加入序列号，看是否有用
//                point->index = i;
//                polyline.push_back(point);
//
//                cout << " double "<< x<< " " << y <<" "<< z <<endl;
//            }
//
//            //用完就删除
//            delete [] Merge;
////        //将这个polyline输入到polylines里去，后者应该是这个集合？
//            polylines.push_back(polyline);
//
//            cdt = new CDT(polyline);
////
////        //开始剖分
//            cdt->Triangulate();
//
//            //map是完整的剖分（包含空洞的剖分）？
//            map = cdt->GetMap();
//            triangles = cdt->GetTriangles();
//            cout << "the poly2tir Triangulate size :"<< triangles.size() << endl;
//
//
//
//
//            //开始绑定poly
//            Poly2TriBind(PolyVAOs, PolyVBOs, triangles);
//            //这里有问题，多遍历了一边，要查询一下怎么清除
//            //打开剖分
//            Poly2TriOpen = true;
//        }
//    }
//    //开始耳切法
//    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//    {
//        if(!EarCutOpen)
//        {
//            cout<<"poly2tri"<<endl;
//
//            VERTEX *Merge = faultMerge(faultUp[0], 10, faultUp[1], 10);
//            //这里只能这样定义？
//            //将Merge输入到arrays里，能直接用这个VERTEX输入进去
//            vector<Points> lines;
//            // 不用数组，就单个就行了。
//            //个人感觉应该不行，
//            for(int i = 0; i < 20;i++)
//            {
//                Points array;
//                array.at(0) = Merge[i].x;
//                array.at(1) = Merge[i].y;
//                array.at(2) = Merge[i].z;
//                lines.push_back(array);
//            }
//
//            //输入后再push到polygon
//            polygon.push_back(lines);
//
//            indices = mapbox::earcut<N>(polygon);
//
//
//            //这里写一个绑定函数，输入到缓冲里
//            EarCutBind(EarVAOs, EarVBOs, indices);
//
//            cout << "the indices size: " << indices.size() <<endl;
//
//            EarCutOpen = true;
//        }
//    }
//    //f1取消显示del
//    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
//    {
//        if(DelaunayOpen)
//            DelaunayOpen = false;
//    }
//    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
//    {
//        if(Poly2TriOpen)
//            Poly2TriOpen = false;
//    }
//
//    //平移操作，如果按下则进行平移
//    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
//    {
//        cout<<"move"<<endl;
//       //主要是移动一下z轴，先试试。
//       if(!faultMove)
//       {
//           faultMoveFunction(faultUp[1], 10, 1.0f, zD);
//           faultMoveFunction(faultUp[1], 10, -0.25f, yD);
////           for(int i = 0 ;i < 10;i++)
////           {
////               cout<<fault2_up[i].x<<" test ";
////               cout<<fault2_up[i].y<<" ";
////               cout<<fault2_up[i].z<<endl;
////           }
//           faultMove = true;
//       }
//
//    }
//    //剖分后平移回去，就直接改变坐标的点试试。
//    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
//    {
//        cout<<"back"<<endl;
//        //主要是移动一下z轴，先试试。
//        if(!moveBack && DelaunayOpen)
//        {
//            //把三角形里的平移回去
//            del->MoveVertex(11, 10, zD, -1.0f);
//            del->MoveVertex(11, 10, yD, 0.25f);
//            //更改了后要重新缓冲一下
//            DelaunayBind(DelTraVAOs, DelTraVBOs, del->HowMany, del);
//
//            //移动过来的线要移动回去啊
//            moveBack = true;
//
//            faultMoveFunction(faultUp[1], 10, -1.0f, zD);
//            faultMoveFunction(faultUp[1], 10, 0.25f, yD);
//        }
//        //这个back可以分多种情况
//        else if(!moveBack && EarCutOpen)
//        {
//            if(polygon.size() == 1)
//            {
//                //数据写的太死了！
//                for(int i = 10;i < 20; i++)
//                {
//                    polygon.at(0).at(i).at(1) += 0.25f;
//                    polygon.at(0).at(i).at(2) += -1.0f;
//
//                }
//                EarCutBind(EarVAOs, EarVBOs, indices);
//            }
//            moveBack = true;
//            faultMoveFunction(faultUp[1], 10, -1.0f, zD);
//            faultMoveFunction(faultUp[1], 10, 0.25f, yD);
//        }
//        else if(!moveBack && Poly2TriOpen)
//        {
//            //先移动
//            faultMoveFunction(faultUp[1], 10, -1.0f, zD);
//            faultMoveFunction(faultUp[1], 10, 0.25f, yD);
//            //获取初始三角现在要改变这个三角里的数据
//            for (int i = 0; i < triangles.size(); i++)
//            {
//                Triangle &t = *triangles[i];
////                Point &a = *t.GetPoint(0);
////                Point &b = *t.GetPoint(1);
////                Point &c = *t.GetPoint(2);
//                //看是否该三角是在同一直线的，这种三角就需要插点。
//                int pointsInLineOne = 0;
//                int pointsInLineTwo = 0;
//
//                //现在是引用，应该可以改变结构体的数据吧
//                for(int j = 0 ;j < 3; j++)
//                {
//                    Point &point = *t.GetPoint(j);
//                    //好像这个不同的三角point数据也是共享的，那么猜测可能这个三角用的也是这个索引数据。
//                    //在point结构体里加个是否移动的属性
//                    if(point.index >= 10)
//                        pointsInLineTwo++;
//                    else
//                        pointsInLineOne++;
//                    if(point.index >= 10 && !point.isMove)
//                    {
//                        point.y += 0.25f;
//                        point.z += -1.0f;
//                        point.isMove = true;
//                    }
//                }
//                cout<< " the point " << pointsInLineOne<<endl;
//                cout<< " the point " << pointsInLineTwo<<endl;
//                cout<<endl;
//                //如果不需要平移的线段有多余三角
//                if(pointsInLineOne == 3)
//                {
//                    //对这个三角搞事情
//                    //这个三角要隐藏起来
//                    t.isHide = true;
//
//                    //直接隐藏这个点吧。
//                    t.HidePoints();
//                    isAddTra = true;
//                    //处理后得到一个d，这个d可以用来干嘛呢？
//                    ExcessTraHandle(&t, faultUp[1], 10);
//                }
//                if(pointsInLineTwo == 3)
//                {
//                    t.isHide = true;
//                    t.HidePoints();
//                    isAddTra = true;
//                    //对移动到对面的三角搞事
//                    ExcessTraHandle(&t, faultUp[0], 10);
//                }
//            }
//            //重新绑定
//            Poly2TriBind(PolyVAOs, PolyVBOs, triangles);
//
//            if( isAddTra )
//            {
//               AddTriBind(AddVAOs, AddVBOs, extraTriangles);
//            }
//            moveBack = true;
//
//        }
//    }
    //剖分后平移回去，就直接改变坐标的点试试。
//    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
//    {
//        cout<<"back"<<endl;
//        //主要是移动一下z轴，先试试。
//        if(!moveBack1)
//        {
//            del->MoveVertex(11, 10, zD, 1.0f);
//            del->MoveVertex(11, 10, yD, -0.35f);
//            //更改了后要重新缓冲一下
//            DelaunayBind(DelTraVAOs, DelTraVBOs, del->HowMany, del);
//
//            moveBack1 = true;
//        }
//    }
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

//        cout << source[i] << " " << source[i+1] << " " << source[i+2] <<endl;
    }


    //这里要直接用引用还是要去了？感觉好像没报错是没毛病的。

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    cout << "sizeof " << sizeof(source) << endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(source), source, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}




void Poly2TriBind(unsigned int * PolyVAOs, unsigned int * PolyVBOs, unsigned int * texture,  vector<Triangle*> _triangle)
{
    glGenVertexArrays(_triangle.size(), PolyVAOs);
    glGenBuffers(_triangle.size(), PolyVBOs);

    //循环读取Polyl里的三角形顶点数据
//    float TraVertex[9];
    //这里用15，9是三个点的坐标，6是三个纹理坐标
    float TraVertex[24];
//    cout << "tri size " << _triangle.size() << endl;
    for (int i = 0; i < _triangle.size(); i++)
    {

        Triangle &t = *_triangle[i];

            Point &a = *t.GetPoint(0);
            Point &b = *t.GetPoint(1);
            Point &c = *t.GetPoint(2);
            //不对啊，这里还有vvo和vv1？傻逼了
            //将顶点分配给这个float

            int hideNumber = 0;
            if(a.isHide)
                hideNumber++;
            if(b.isHide)
                hideNumber++;
            if(c.isHide)
                hideNumber++;

            if(hideNumber >= 2)
                continue;
        //将point传入
        VERTEX traNormol = getNormal(a.PointToVertex(), b.PointToVertex(), c.PointToVertex());
        //不对啊，这里还有vvo和vv1？傻逼了
        //将顶点分配给这个float
        //三角坐标
        TraVertex[0] = a.x;
        TraVertex[1] = a.y;
        TraVertex[2] = a.z;
        //平面法向量实现光照
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
        // texture 1
        // ---------
        //绑定一下纹理
        texture[i] = loadTexture(FileSystem::getPath("resources/textures/rock.png").c_str());
    }
}

//这个函数可以用来做很多东西，不管是什么绑定，反正也是通用的那种。
void AddTriBind(unsigned int * AddVAOs, unsigned int * AddVBOs, unsigned  int * addTexture, vector<AddTriangle> _triangle)
{
    int size = _triangle.size();
    glGenVertexArrays(size, AddVAOs);
    glGenBuffers(size, AddVBOs);
//    cout << "the add size : " << size << endl;
    //循环读取Addl里的三角形顶点数据
    VERTEX a, b, c;
    for (int i = 0; i < size; i++)
    {
        //15+9，现在加入了法向量实现漫反射
        float TraVertex[24];
        AddTriangle &t = _triangle[i];
        a = t.a;
        b = t.b;
        c = t.c;
        VERTEX traNormol = getNormal(a, b, c);
        //不对啊，这里还有vvo和vv1？傻逼了
        //将顶点分配给这个float
        //三角坐标
        TraVertex[0] = a.x;
        TraVertex[1] = a.y;
        TraVertex[2] = a.z;
        //平面法向量实现光照
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

        //求三角平面法向量

        //绑定到vbo里
        glBindVertexArray(AddVAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, AddVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TraVertex), TraVertex, GL_STATIC_DRAW);
        // position attribute
        //这里的步长为8，之前的是5因为有纹理坐标，现在加入了光照漫反射
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // texture 1
        // ---------
        //绑定一下纹理
//        addTexture[i] = loadTexture(FileSystem::getPath("resources/textures/bricks2.jpg").c_str());

    }
}

void EarCutBind(unsigned int * EarVAOs, unsigned int * EarVBOs, std::vector<N> indices)
{
    int triangleSize = (indices.size()) / 3;
    EarTraSize = triangleSize;
    glGenVertexArrays(triangleSize, EarVAOs);
    glGenBuffers(triangleSize, EarVBOs);

    //循环读取Earl里的三角形顶点数据
    //这是没有空洞的情况
    int cnt = 0;
    for (int i : indices)
    {
        cout<<i<<endl;
    }


    if(polygon.size() == 1)
    {
        for(int i = 0,j = 0; i < indices.size(); i++,j++)
        {
            float EarVertex[9];
            EarVertex[0] = polygon.at(0).at(indices[i]).at(0);
            EarVertex[1] = polygon.at(0).at(indices[i]).at(1);
            EarVertex[2] = polygon.at(0).at(indices[i++]).at(2);
            EarVertex[3] = polygon.at(0).at(indices[i]).at(0);
            EarVertex[4] = polygon.at(0).at(indices[i]).at(1);
            EarVertex[5] = polygon.at(0).at(indices[i++]).at(2);
            EarVertex[6] = polygon.at(0).at(indices[i]).at(0);
            EarVertex[7] = polygon.at(0).at(indices[i]).at(1);
            EarVertex[8] = polygon.at(0).at(indices[i]).at(2);
            //最后一次不用++了
            cnt++;
            cout << "Cnt:" << cnt << endl;
            for (int k = 0; k < 9; k++)
            {
                cout << EarVertex[k] << " ";
                //应该是这里出错了，尼玛哦，看了我半天日了你爹
                if ((k + 1) % 3 == 0)
                {
                    cout << endl;
                }
            }
            cout << endl;


            glBindVertexArray(EarVAOs[j]);

            glBindBuffer(GL_ARRAY_BUFFER, EarVBOs[j]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(EarVertex), EarVertex, GL_STATIC_DRAW);

            // position attribute
            //这里的步长为3，之前的是5因为有纹理坐标
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(0);
        }
    }

}


//多余的三角形，需要处理掉
void ExcessTraHandle(Triangle* _triangle, vector<VERTEX>& oppositeLines, int index)
{
    //从三角里找到需要处理的那条线,如果序列号不是相邻的那么就是相邻的线
    Point *a = _triangle->GetPoint(0);
    Point *b = _triangle->GetPoint(1);
    Point *c = _triangle->GetPoint(2);
    //Lines 1 是ab 2是ac 3是bc
    enum {ab = 1, ac, bc};
    int Lines;

    if( abs((a->index) - (b->index)) == 2 )
        Lines = ab;
    else if( abs((a->index) - (c->index)) == 2 )
        Lines = ac;
    else
        Lines = bc;

    //找出这条线后，然后计算另外一个点到该线的距离。
    //d1为三角里点到线段的距离，d2为多余线段到对面线的最短距离
    float d1,d2;

    //这个值将是即将要连线的点
    VERTEX projectionPoint;
    int oppositeIndex = -1;
    if(Lines == ab)
    {
        d1 = DistanceOfPointLinesIn2D(c->PointToVertex(), a->PointToVertex(), b->PointToVertex());
        projectionPoint = c->PointToVertex();
        //将y的值减去d1就是投影在多余的线的点。
        projectionPoint.y -= d1;
        d2 = DistanceOfOpposite(projectionPoint, oppositeLines, oppositeIndex);
    }
    else if(Lines == ac)
    {
        d1 = DistanceOfPointLinesIn2D(b->PointToVertex(), a->PointToVertex(), c->PointToVertex());
        projectionPoint = b->PointToVertex();
        //将y的值减去d1就是投影在多余的线的点。
        projectionPoint.y -= d1;
        d2 = DistanceOfOpposite(projectionPoint, oppositeLines, oppositeIndex);
    }
    else
    {
        d1 = DistanceOfPointLinesIn2D(a->PointToVertex(), b->PointToVertex(), c->PointToVertex());
        projectionPoint = a->PointToVertex();
        //将y的值减去d1就是投影在多余的线的点。
        projectionPoint.y -= d1;
        d2 = DistanceOfOpposite(projectionPoint, oppositeLines, oppositeIndex);
    }
//    cout << "oppositeIndex" << oppositeIndex << endl;
//    cout << "Lines "<< Lines << endl;
//    cout << "projection point" << projectionPoint.x <<" " << projectionPoint.y<< " " << projectionPoint.z<<endl;
    //这个d就是总偏移量了，也就是point，z的偏移量
    float d = fabs(((a->z / pow(d1, 2) ) + (oppositeLines[0].z / pow(d2, 2))) / ((1 / pow(d1, 2)) + (1 / pow(d2, 2))));

//    cout << "the d1 d2 d " << d1<<" "<<d2<<" "<<d<<endl;

    VERTEX centerPoint, left, mid, right;

    VERTEX oppositePointOne, oppositePointTwo;

    oppositePointOne.x = oppositeLines[oppositeIndex].x;
    oppositePointOne.y = oppositeLines[oppositeIndex].y;
    oppositePointOne.z = oppositeLines[oppositeIndex].z;

    oppositePointTwo.x = oppositeLines[oppositeIndex + 1].x;
    oppositePointTwo.y = oppositeLines[oppositeIndex + 1].y;
    oppositePointTwo.z = oppositeLines[oppositeIndex + 1].z;

    if(Lines == ab)
    {
        centerPoint.x = c->x;


        centerPoint.y = c->y - (c->y - oppositePointOne.y) / 2;
        //
        centerPoint.z = c->z - d;

        mid = c->PointToVertex();
        if(a->index < b->index)
        {
            left = a->PointToVertex();
            right = b->PointToVertex();
        }
        else
        {
            left = b->PointToVertex();
            right = a->PointToVertex();
        }

    }
    else if( Lines == ac)
    {
        centerPoint.x = b->x;
        centerPoint.y = b->y - (b->y - oppositePointOne.y) / 2;
        centerPoint.z = b->z - d;
        mid = b->PointToVertex();
        if(a->index < c->index)
        {
            left = a->PointToVertex();
            right = c->PointToVertex();
        }
        else
        {
            left = c->PointToVertex();
            right = a->PointToVertex();
        }
    }
    else
    {
        centerPoint.x = a->x;
        //平滑计算
        centerPoint.y = a->y - (a->y - oppositePointOne.y) / 2;
        centerPoint.z = a->z - d;
        mid = a->PointToVertex();
        if(c->index < b->index)
        {
            left = c->PointToVertex();
            right = b->PointToVertex();
        }
        else
        {
            left = b->PointToVertex();
            right = c->PointToVertex();
        }
    }
    cout << " center "<< centerPoint.x << " " << centerPoint.y << " " << centerPoint.z << endl;
//
    cout << " left "<< left.x << " " << left.y << " " << left.z << endl;
    //把这5个三角形放入数组里
    extraTriangles[index].push_back(VertexToTriangle(left, mid, centerPoint));
    extraTriangles[index].push_back(VertexToTriangle(right, mid, centerPoint));
    extraTriangles[index].push_back(VertexToTriangle(left, centerPoint, oppositePointOne));
    extraTriangles[index].push_back(VertexToTriangle(right, centerPoint, oppositePointTwo));
    extraTriangles[index].push_back(VertexToTriangle(centerPoint, oppositePointTwo, oppositePointOne));


//    AddTriBind(AddVAOs, AddVBOs, extraTriangles);
}



//void moveFunction(VERTEX * fault1, int num1, VERTEX * fault2, int num2, int index, int which)
//{
////    cout << " move" << endl;
//    //先平移z
//    int moveDirectionCnt = 0;
//    faultMoveSize[index][which][moveDirectionCnt].size = fault1->z - fault2->z;
////    cout << faultMoveSize[index][which][moveDirectionCnt].size <<endl;
//    faultMoveSize[index][which][moveDirectionCnt].md = zD;
//    faultMoveFunction(fault2, num1,faultMoveSize[index][which][moveDirectionCnt].size, faultMoveSize[index][which][moveDirectionCnt].md);
//    moveDirectionCnt++;
//    //平移y轴
//    float yMoveSize = 0.0f;
//    while( !faultIntersect(fault1, num1, fault2, num2) )
//    {
//        //一般是只要平移这个y轴？这样以后处理不了转弯的情况了。
//        faultMoveSize[index][which][moveDirectionCnt].size = (yMoveSize += -0.1f);
//        faultMoveSize[index][which][moveDirectionCnt].md = yD;
//        faultMoveFunction(fault2, num1, faultMoveSize[index][which][moveDirectionCnt].size, faultMoveSize[index][which][moveDirectionCnt].md);
//    }
//}

void poly2Tri(vector<VERTEX>& Merge, int index)
{
//            //将vertex的xy坐标输入到这个poly
            int num = Merge.size();
            for(int i = 0;i < num; i++)
            {
                double x = Merge[i].x;
                double y = Merge[i].y;
                double z = Merge[i].z;
                Point *point = new Point(x,y);
                point->z = z;
                //加入序列号，看是否有用
                point->index = i;
                polyline[index].push_back(point);
            }

            //用完就删除
//        //将这个polyline输入到polylines里去，后者应该是这个集合？
            polylines[index].push_back(polyline[index]);

            cdt[index] = new CDT(polyline[index]);
//
//        //开始剖分
            cdt[index]->Triangulate();

            //map是完整的剖分（包含空洞的剖分）？
            map[index] = cdt[index]->GetMap();
            triangles[index] = cdt[index]->GetTriangles();

    Poly2TriBind(PolyVAOs[index], PolyVBOs[index], textures[index],triangles[index]);
            //这里有问题，多遍历了一边，要查询一下怎么清除
            //打开剖分
//            Poly2TriOpen = true;
}

void closeLineBack(vector<VERTEX>& _fault, int indexTra)
{
    int traSize = triangles[indexTra].size();
    cout << traSize << endl;
    for (int i = 0; i < traSize; i++)
    {
        Triangle &t = *triangles[indexTra][i];
        int isAdd = 0;
        for(int j = 0 ;j < 3; j++)
        {
            Point &point = *t.GetPoint(j);

//            如果这个点在线段内
            int index = VertexInVertexs(point.PointToVertex(), _fault);
            if(index != -1)
            {
                //如果已经移动了就不需要判断了
                if(point.isMove)
                    continue;

                point.isMove = true;
                //z值的差别
                point.z -= 1.0f;
                point.x *= (1.0f / scaleSize[indexTra]);
                point.y *= (1.0f / scaleSize[indexTra]);
                _fault[index].z -= 1.0f;
                _fault[index].x *= (1.0f / scaleSize[indexTra]);
                _fault[index].y *= (1.0f / scaleSize[indexTra]);

            }else{
                //不在这个对面的线上，那么就在自己这个线上？
                isAdd++;
            }
        }
        //如果这个三角形是平台三角形那么需要插值处理
        if(isAdd == 3){
            cout << "is add " << endl;
//            t.isHide = true;
            ExcessTraHandle(&t, _fault, indexTra);
            isAddTra[indexTra] = true;
        }

    }
    //将线平移回去
    //放缩回去
//    faultScaleFunction(_fault, (1.0f / scaleSize[indexTra]), xD);
//    faultScaleFunction(_fault, (1.0f / scaleSize[indexTra]), yD);
//    faultMoveFunction(_fault, 1.0f, zD);

    //画线
    drawInit(faceVAO[indexTra + 1], faceVBO[indexTra + 1], _fault);

    Poly2TriBind(PolyVAOs[indexTra], PolyVBOs[indexTra], textures[indexTra],  triangles[indexTra]);
    //绑定额外三角
    if( isAddTra[indexTra] )
    {
        //这里的绑定也会出问题
        AddTriBind(AddVAOs[indexTra], AddVBOs[indexTra], addTextures[indexTra],  extraTriangles[indexTra]);
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
//    map[index] = cdt[index]->GetMap();
    triangles[index] = cdt[index]->GetTriangles();

    //纹理绑定
    Poly2TriBind(PolyVAOs[index], PolyVBOs[index], textures[index],triangles[index]);
}


//void lineBack(VERTEX * _fault, int num1, int indexLine, int indexTra)
//{
//    //0 为上层， 1 为下层
//    int line = 0;
//
//    //用indexTra来判断indexLine是上层还是下层
//    if((indexLine * 2) == (indexTra + 1))
//        line = 1;
//    //获取平移量,但是要判断一下这个是否有平移
//    for(int j = 0;j < 3; j++)
//    {
////        cout << " the size " << faultMoveSize[index][line][j].size<< endl;
////        cout << " the size dir " << faultMoveSize[index][line][j].md<< endl;
//        if(faultMoveSize[indexLine][line][j].size == 0)
//            continue;
//        faultMoveFunction(_fault, num1,  0.0 - faultMoveSize[indexLine][line][j].size, faultMoveSize[indexLine][line][j].md);
////        faultMoveFunction(_faultDown, num2, 0.0 - faultMoveSize[index][1][j].size, faultMoveSize[index][1][j].md);
//
////        drawInit(faultUpVAO[index], faultUpVBO[index], faultUp[index], sizeof(faultData[index]) / 12);
//    }
//
//    //线平移后要绑定一下
//    //获取初始三角现在要改变这个三角里的数据
////    cout << "get in indexTra:" << indexTra <<endl;
////    cout << triangles[indexTra].size() << endl;
//    for (int i = 0; i < triangles[indexTra].size(); i++)
//    {
//        Triangle &t = *triangles[indexTra][i];
////                Point &a = *t.GetPoint(0);
////                Point &b = *t.GetPoint(1);
////                Point &c = *t.GetPoint(2);
//        //看是否该三角是在同一直线的，这种三角就需要插点。
//        int pointsInLineOne = 0;
//        int pointsInLineTwo = 0;
//
//        //现在是引用，应该可以改变结构体的数据吧
//        for(int j = 0 ;j < 3; j++)
//        {
//            Point &point = *t.GetPoint(j);
////            cout << "point.indexTra" << point.indexTra<<endl;
//            //好像这个不同的三角point数据也是共享的，那么猜测可能这个三角用的也是这个索引数据。
//            //在point结构体里加个是否移动的属性
//            //这个10是分界值
//            if(point.index >= 10)
//                pointsInLineTwo++;
//            else
//                pointsInLineOne++;
//            if(point.index >= 10 && !point.isMove)
//            {
//                //3个方向的平移值。
//                for(int j = 0;j < 3; j++)
//                {
//                    //判断是上面的线还是下面的线，一半双数是上面的线，单数是下面的线
//                    //0为上
//                    if(faultMoveSize[indexLine][line][j].size == 0)
//                        continue;
////                    cout << "indexTra " << indexTra<<endl;
////                    cout << "the size : " << faultMoveSize[indexTra][line][j].size<< endl;
//                    if(faultMoveSize[indexLine][line][j].md == xD)
//                        point.x -= faultMoveSize[indexLine][line][j].size;
//                    else if (faultMoveSize[indexLine][line][j].md == yD)
//                        point.y -= faultMoveSize[indexLine][line][j].size;
//                    else
//                        point.z -= faultMoveSize[indexLine][line][j].size;
//                }
////                point.y += 0.25f;
////                point.z += -1.0f;
//                point.isMove = true;
//            }
//        }
////        cout<< " the point " << pointsInLineOne<<endl;
////        cout<< " the point " << pointsInLineTwo<<endl;
////        cout<<endl;
//        //如果不需要平移的线段有多余三角
//        if(pointsInLineOne == 3)
//        {
//            cout << "the index " <<indexTra << endl;
//            //对这个三角搞事情
//            //这个三角要隐藏起来
//            t.isHide = true;
//
//            //直接隐藏这个点吧。
//            t.HidePoints();
//            isAddTra[indexTra] = true;
//            //这里要传入一个三角的对线。
//            //如果是上层
//            if(line == 0)
//            {
//                cout << "0" << endl;
//                ExcessTraHandle(&t, faultUp[indexLine + 1], 10, indexTra);
//            }
//            else
//            {
//                ExcessTraHandle(&t, faultDown[indexLine + 1], 10, indexTra);
//            }
//        }
//        //多余三角在第二条边，那么传入的就是第一条边的线
//        if(pointsInLineTwo == 3)
//        {
//            t.isHide = true;
//            t.HidePoints();
//            isAddTra[indexTra] = true;
//            //对移动到对面的三角搞事
//            if(line == 0)
//            {
//                ExcessTraHandle(&t, faultUp[indexLine], 10, indexTra);
//            }
//            else
//            {
//                ExcessTraHandle(&t, faultDown[indexLine], 10, indexTra);
//            }
//        }
//    }
//    //重新绑定
//    Poly2TriBind(PolyVAOs[indexTra], PolyVBOs[indexTra], textures[indexTra],   triangles[indexTra]);
//
//    if( isAddTra[indexTra] )
//    {
//        //这里的绑定也会出问题
//       AddTriBind(AddVAOs[indexTra], AddVBOs[indexTra], addTextures[indexTra],  extraTriangles[indexTra]);
//    }
//    moveBack = true;
//}




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
        if(triStatus == IsTopTri || triStatus == IsMidTri){
//            triangles[index][i]->DebugPrint();
            //这里都不用hide了
//            triangles[index][i]->isHide = true;
            Point* center = new Point;
            triangles[index][i]->GetCircleCenter(center);
            //设置这个z的大小，有一定的弧度
            SetPointZ(closeLineV[index], closeLineV[index+1], center);
            //这个三角的z值怎么算？直接减去一半的dif？
//            center->z = 1.5;
            //这里指定插值的hole，到时候跟着进行移动。
            center->isHole = 2;
//            center->print();
            cdt[index]->AddPoint(center);
        }
    }
    //重新剖分
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

    //纹理绑定
    Poly2TriBind(PolyVAOs[index], PolyVBOs[index], textures[index],triangles[index]);
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


    //纹理绑定
    Poly2TriBind(PolyVAOs[index], PolyVBOs[index], textures[index],triangles[index]);
}

//获取插值点的高度，基于老师给的公式。
//这里不好传入带洞的分支，因为都是已经做了一次区分了
void SetPointZ(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in, Point* p){
    double disOut = 1e9, disIn = 1e9;
    VERTEX point;
    point.x = p->x;
    point.y = p->y;

    point.Print();
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
    double a = disIn / (disIn + disOut);
    double zOut = out[0][0].z, zIn = in[0][0].z;
    p->z = a * zOut + (1 - a) * zIn;
//    cout << a << " | " << p->z << endl;
}





void VertexMerge(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in){
    closeLineMerge.clear();

    for(int i = 0;i < out.size();i++){
        for(int j = 0; j < out[i].size();j++){
            closeLineMerge.push_back(out[i][j]);
        }
    }

    //内轮廓的距离与总权重计算
    for(int i = 0;i < in.size();i++){
        for(int j = 0; j < in[i].size();j++){
            closeLineMerge.push_back(in[i][j]);
        }
    }

    cout << "merge success"<< endl;
}


//这个插值方法好像没有用到z值？
void SetPointZofIDW(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in, Point* p){
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
    center[index] = c;

}

void SecAligned(vector<vector<VERTEX>>& out, vector<vector<VERTEX>>& in, int index){

    double difX, difY;
    difX = center[index+1].x - center[index].x;
    difY = center[index+1].y - center[index].y;

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
        for(int i = 0;i < in.size();i++){
            mergeSec.insert(mergeSec.end(), in[i].begin(), in[i].end());
        }
    }
}


void SecScaleFunction(vector<vector<VERTEX>>& in, double scaleSize, int index){
    for(int i = 0;i < in.size();i++){
        for(int j = 0;j < in[i].size();j++){
            in[i][j].x = (1 - scaleSize) * center[index].x + scaleSize * in[i][j].x;
            in[i][j].y = (1 - scaleSize) * center[index].y + scaleSize * in[i][j].y;
        }
    }
}


//剖面还原，设计到三角形还原和轮廓线还原
void SecBack(int index){
    double difX = center[index+1].x - center[index].x;
    double difY = center[index+1].y - center[index].y;
    //TODO:线的还原暂时没有做，因为本来就是面里有线，所以这个东西如果后期要展示的话，那么需要单独处理一下了。像mc里的方法，直接给抽出来。
    for(int i = 0;i < closeLineV[index+1].size();i++){
        for(int j = 0;j < closeLineV[index+1][i].size();j++){
            closeLineV[index+1][i][j].x = (1 - 1.0/scaleSize[index]) * center[index].x + 1.0/scaleSize[index] * closeLineV[index+1][i][j].x + difX;
            closeLineV[index+1][i][j].y = (1 - 1.0/scaleSize[index]) * center[index].y + 1.0/scaleSize[index] * closeLineV[index+1][i][j].y + difY;
        }
    }
}

//将三角面片进行还原
void TriBack(int index){

    if(index < 0)
        return;
    int traSize = triangles[index].size();
//    cout << traSize << endl;

    //求得两个质心的差值
    double difX = center[index+1].x - center[index].x;
    double difY = center[index+1].y - center[index].y;
    for (int i = 0; i < traSize; i++) {
        Triangle &t = *triangles[index][i];
        for (int j = 0; j < 3; j++) {
            Point &point = *t.GetPoint(j);
            //如果已经移动了，或者是底层剖面，则不需要进行处理
            if(point.isMove || point.isHole == 0)
                continue;

            //先进行放缩，这里可以基于之前的质心进行放缩。后面可以直接加上当前的分量就好了，没毛病。
            point.x = (1 - 1.0/scaleSize[index]) * center[index].x + 1.0/scaleSize[index] * point.x + difX;
            point.y =  (1 - 1.0/scaleSize[index]) * center[index].y + 1.0/scaleSize[index] * point.y + difY;
            point.isMove = true;
        }
    }
    //重新绑定
    Poly2TriBind(PolyVAOs[index], PolyVBOs[index], textures[index],triangles[index]);
}


//对所有的三角形进行插值，times为插值次数
void TriInsert(int times){
    if(times < 1)
        return;

    cout << "begin tri insert" << endl;
    unsigned long triSize;
    //对每一层进行插值
    while(times--){
        //这样插值是不是效率有点慢啊
        //这里层次要减少1，因为插值的时候
        for(int i = 0;i < modelNum - 1;i++){
            triSize = triangles[i].size();
            for(int j = 0;j < triSize;j++){
                //对所有的三角形进行插值
                Point* center = new Point;
                //用外接圆心可能容易出事，如果有狭长的三角形那点都可能跑到外面去了。
                //此处是求三角形的重心，考虑了z轴的值？
                triangles[i][j]->GetCenter(center);

                //这里应该是可以取消插值，再最后一次再进行插值计算？应该是可以的，这样可以大大减少时间。
                //不行啊，完全就是取消不了这个的，也是难顶。
                SetPointZ(closeLineV[i], closeLineV[i+1], center);

                cdt[i]->AddPoint(center);
            }
            cdt[i]->TriangleClear();
            //这个函数操作是应该会改变triangles这个函数指针的
            cdt[i]->Triangulate();
            triangles[i] = cdt[i]->GetTriangles();
            Poly2TriBind(PolyVAOs[i], PolyVBOs[i], textures[i], triangles[i]);
        }
    }

}