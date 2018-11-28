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
#include <vertex.h>

using namespace std;

//引入第三方库
#include "../poly2tri/poly2tri.h"
using namespace p2t;

//引入耳切法
#include "../includes/mapbox/earcut.hpp"

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


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 1.0f;
float lastY = SCR_HEIGHT / 1.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//灯光的位置
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
int modelNum = 3;

Delaunay *del;
unsigned int DelTraVBOs[MaxNum], DelTraVAOs[MaxNum];
unsigned int PolyVBOs[MaxNum][MaxNum], PolyVAOs[MaxNum][MaxNum];
unsigned int AddVBOs[MaxNum][MaxNum], AddVAOs[MaxNum][MaxNum];
bool isAddTra[MaxNum] = {false};

unsigned int EarVBOs[MaxNum], EarVAOs[MaxNum];

//全局变量，用来平移之类的

//这个变量用来来记录面里平移的长度，一个面有两条线。每条线有三个方位的平移值
moveSize faultMoveSize[MaxNum][2][3];

//一个值就够了
float scaleSize[MaxNum];

//搞个三维数组玩玩
float faultData[MaxNum][2][30] = {
        {
        {
                0.5f, 0.15f, -0.5f + 1.0f,
                0.41f, 0.29f, -0.5f + 1.0f,
                0.3f, 0.32f, -0.5f + 1.0f,
                0.18f, 0.26f, -0.5f + 1.0f,
                0.0f, 0.24f, -0.5f + 1.0f,
                -0.13f, 0.29f, -0.5f + 1.0f,
                -0.21f, 0.25f, -0.5f + 1.0f,
                -0.26f, 0.29f, -0.5f + 1.0f,
                -0.38f, 0.25f, -0.5f + 1.0f,
                -0.5f, 0.22f, -0.5f + 1.0f,
        }
        //这个点是尼玛外面的点吧
//            -0.5f, 0.5f, -0.5f+ 1.0f,
//            -0.5f, 0.22f,  -0.5f+ 1.0f,
        //怎么这个点重复了，有毒。
        //x下层
        ,
        {
                -0.5f, -0.24f, -0.5f + 1.0f,
                -0.41f, -0.23f, -0.5f + 1.0f,
                -0.29f, -0.23f, -0.5f + 1.0f,
                -0.23f, -0.29f, -0.5f + 1.0f,
                -0.05f, -0.32f, -0.5f + 1.0f,
                0.08f, -0.31f, -0.5f + 1.0f,
                0.19f, -0.25f, -0.5f + 1.0f,
                0.29f, -0.3f, -0.5f + 1.0f,
                0.4f, -0.26f, -0.5f + 1.0f,
                0.5f, -0.29f, -0.5f + 1.0f,
        }
        }
        ,
        {
            {
                0.5f, 0.15f,  -0.5f ,
                        0.39f, 0.25f,  -0.5f ,
                        0.28f, 0.30f,  -0.5f ,
                        0.13f, 0.28f,  -0.5f ,
                        0.0f, 0.25f,  -0.5f ,
                        -0.15f, 0.19f,  -0.5f ,
                        -0.24f, 0.22f, -0.5f,
                        -0.30f, 0.25f,  -0.5f ,
                        -0.41f, 0.20f,  -0.5f ,
                        -0.5f, 0.22f,  -0.5f }
            //这个点是尼玛外面的点吧
//            -0.5f, 0.5f, -0.5f ,
//            -0.5f, 0.22f,  -0.5f ,
            //怎么这个点重复了，有毒。
            //x下层
            ,
            {
                -0.5f, -0.24f,  -0.5f ,
                        -0.42f, -0.21f,  -0.5f ,
                        -0.35f, -0.22f,  -0.5f ,
                        -0.20f, -0.29f,  -0.5f ,
                        -0.05f, -0.32f,  -0.5f ,
                        0.08f, -0.30f,  -0.5f ,
                        0.19f, -0.25f,  -0.5f ,
                        0.31f, -0.3f,  -0.5f ,
                        0.43f, -0.27f,  -0.5f ,
                        0.5f, -0.29f, -0.5f }
        }
        ,
        {
                {
                        0.5f, 0.15f,  -0.5f - 1.0f,
                        0.39f, 0.25f,  -0.5f - 1.0f ,
                        0.28f, 0.30f,  -0.5f - 1.0f ,
                        0.13f, 0.28f,  -0.5f - 1.0f ,
                        0.0f, 0.25f,  -0.5f  - 1.0f ,
                        -0.15f, 0.19f,  -0.5f - 1.0f ,
                        -0.24f, 0.22f, -0.5f - 1.0f ,
                        -0.30f, 0.25f,  -0.5f - 1.0f ,
                        -0.41f, 0.20f,  -0.5f - 1.0f ,
                        -0.5f, 0.22f,  -0.5f - 1.0f }
                //这个点是尼玛外面的点吧
//            -0.5f, 0.5f, -0.5f ,
//            -0.5f, 0.22f,  -0.5f ,
                //怎么这个点重复了，有毒。
                //x下层
                ,
                {
                        -0.5f, -0.24f,  -0.5f - 1.0f ,
                        -0.42f, -0.21f,  -0.5f - 1.0f ,
                        -0.35f, -0.22f,  -0.5f - 1.0f ,
                        -0.20f, -0.29f,  -0.5f - 1.0f ,
                        -0.05f, -0.32f,  -0.5f - 1.0f ,
                        0.08f, -0.30f,  -0.5f - 1.0f ,
                        0.19f, -0.25f,  -0.5f - 1.0f ,
                        0.31f, -0.3f,  -0.5f - 1.0f ,
                        0.43f, -0.27f,  -0.5f - 1.0f ,
                        0.5f, -0.29f, -0.5f - 1.0f}
        }
        ,
        {
            {
                0.5f, 0.15f,  -0.5f - 2.0f,
                        0.39f, 0.25f,  -0.5f - 2.0f ,
                        0.28f, 0.30f,  -0.5f - 2.0f ,
                        0.13f, 0.28f,  -0.5f - 2.0f ,
                        0.0f, 0.25f,  -0.5f  - 2.0f ,
                        -0.15f, 0.19f,  -0.5f - 2.0f ,
                        -0.24f, 0.22f, -0.5f - 2.0f ,
                        -0.30f, 0.25f,  -0.5f - 2.0f ,
                        -0.41f, 0.20f,  -0.5f - 2.0f ,
                        -0.5f, 0.22f,  -0.5f - 2.0f }
            //这个点是尼玛外面的点吧
//            -0.5f, 0.5f, -0.5f ,
//            -0.5f, 0.22f,  -0.5f ,
            //怎么这个点重复了，有毒。
            //x下层
            ,
            {
                -0.5f, -0.24f,  -0.5f - 2.0f ,
                        -0.42f, -0.21f,  -0.5f - 2.0f ,
                        -0.35f, -0.22f,  -0.5f - 2.0f ,
                        -0.20f, -0.29f,  -0.5f - 2.0f ,
                        -0.05f, -0.32f,  -0.5f - 2.0f ,
                        0.08f, -0.30f,  -0.5f - 2.0f ,
                        0.19f, -0.25f,  -0.5f - 2.0f ,
                        0.31f, -0.3f,  -0.5f - 2.0f ,
                        0.43f, -0.27f,  -0.5f - 2.0f ,
                        0.5f, -0.29f, -0.5f - 2.0f}
        }
};

//这个就主要是框架的数据
float faceData[MaxNum][12] = {
        {
                -0.5f, 0.5f,  0.5f,
                0.5f, 0.5f,  0.5f,
                0.5f, -0.5f, 0.5f,
                -0.5f,-0.5f,  0.5f
        },
        {
                -0.5f, 0.5f,  0.5f -1.0f,
                0.5f, 0.5f,  0.5f - 1.0f,
                0.5f, -0.5f, 0.5f - 1.0f,
                -0.5f,-0.5f,  0.5f -  1.0f
        },
        {
                -0.5f, 0.5f,  0.5f - 2.0f,
                0.5f, 0.5f,  0.5f - 2.0f,
                0.5f, -0.5f, 0.5f - 2.0f,
                -0.5f,-0.5f,  0.5f - 2.0f
        }
        ,
        {
                -0.5f, 0.5f,  0.5f - 3.0f,
                0.5f, 0.5f,  0.5f - 3.0f,
                0.5f, -0.5f, 0.5f - 3.0f,
                -0.5f,-0.5f,  0.5f - 3.0f
        }
};


VERTEX *closeLine[MaxNum];

//还是要标出来每层对个数
int closeDataNum[MaxNum] = {13, 13, 13, 7, 8};

//后一个是前一个的洞，单层循环处理
float closeData[MaxNum][100] = {
        {

            -4.0f, 2.0f, -1.5f,
            -2.63f, -1.84f, -1.5f,
            -1.91f, -1.84f, -1.5f,
            -1.71f, -4.08f, -1.5f,
            0.88f, -4.47f, -1.5f,
            2.89f, -3.73f, -1.5f,
            4.2f, -2.91f, -1.5f,
            4.17f, -1.67f, -1.5f,
            3.76f, 1.24f, -1.5f,
            2.51f, 3.44f, -1.5f,
            1.09f, 4.82f, -1.5f,
            -0.26f, 3.36f, -1.5f,
            -2.18f, 4.0f, -1.5f,
        },
        {
            3.34f, -2.85f, 0,
            3.35f, -1.19f, 0,
            2.89f, 0.81f, 0,
            2.75f, 2.21f, 0,
            2.0f, 4.0f, 0,
            0.37f, 3.96f, 0,
            -1.6f, 3.35f, 0,
            -3.52f, 2.5f, 0,
            -2.79f, 1.04f, 0,
            -1.85f, -0.85f, 0,
            -1.33f, -2.88f, 0,
            0.38f, -3.7f, 0,
            1.89f, -3.1f, 0,
        },
        {
            3.26f, -3.31f, 1.5f,
            3.65f, -2.04f, 1.5f,
            3.52f, -0.33f, 1.5f,
            3.13f, 1.42f, 1.5f,
            1.92f, 2.98f, 1.5f,
            0.87f, 4.23f, 1.5f,
            -0.9f, 4.04f, 1.5f,
            -2.8f, 3.37f, 1.5f,
            -3.79f, 1.14f, 1.5f,
            -2.86f, -1.04f, 1.5f,
            -2.56f, -2.78f, 1.5f,
            -0.28f, -4.19f, 1.5f,
            1.66f, -3.99f, 1.5f
        }
        ,
        //这一组是分支数据，估计要重新设计这个数组了。得多安排一个数组，还要考虑不同高度的，可能放为一组的。
        {
            0.91f, 1.47f, 3.0f,
            0.65f, 2.28f, 3.0f,
            -0.53f, 2.4f, 3.0f,
            -1.38f, 1.72f, 3.0f,
            -1.39f, 0.79f, 3.0f,
            -0.35f, 0.54f, 3.0f,
                0.38f, 0.66f, 3.0f

        }
        ,
        {
            1.53f, -1.82, 3.0f,
                    1.72f, -1.15f, 3.0f,
                    0.8f, -1.13f, 3.0f,
                    -0.22f, -1.31f, 3.0f,
                    -0.98f, -1.6f, 3.0f,
                    -0.56f, -2.64f, 3.0f,
                    0.39f, -2.91f, 3.0f,
                    1.29f, -2.7f, 3.0f
        }

};



////函数声明
//void faultMoveFunction(VERTEX *vertex, int num, float moveSize, int whichDirection);
////将两个顶点数组合并为1个
//VERTEX * faultMerge(VERTEX vertex1[],int num1, VERTEX vertex2[], int num2);

//初始化
void drawInit(unsigned int & VAO, unsigned int & VBO, VERTEX *target, int num);

////两个转换的函数
//float * VertexToFloat(VERTEX vertex[], int num);
//
//VERTEX * FloatToVertex(float _float[], int num);

void DelaunayBind(unsigned int * DeVAOs, unsigned int * DeVBOs, int DeNum, Delaunay * del);

void Poly2TriBind(unsigned int * PolyVAOs, unsigned int * PolyVBOs, unsigned int * texture,  vector<Triangle*> _triangle);

void AddTriBind(unsigned int * AddVAOs, unsigned int * AddVBOs, unsigned int * addTexture, vector<AddTriangle> _triangle);

void EarCutBind(unsigned int * EarVAOs, unsigned int * EarVBOs, std::vector<N> indices);


void ExcessTraHandle(Triangle* _triangle, VERTEX oppositeLines[], int num,int index );

void moveFunction(VERTEX * fault1, int num1, VERTEX * fault2, int num2, int index, int which);

void poly2Tri(VERTEX * Merge, int num, int index);

void closePoly2Tri(VERTEX * closeOut, int num1, VERTEX * closeHole, int num2, int index);

void lineBack(VERTEX * _faultUp, int num1, int indexLine, int indexTra);

void closeLineBack(VERTEX * _fault, int num1, int indexTra);

void scaleFunction(VERTEX * fault1, int num1, VERTEX * fault2, int num2, int index);

vector<Point*> VertexsToPoints(VERTEX * vertex, int num);

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
//    Shader lightingShader("7.4.lighting_maps.vs", "7.4.lighting_maps.fs");
//    Shader lampShader("7.4.lamp.vs", "7.4.lamp.fs");
//
//
//    unsigned int VBO;
//    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
//    unsigned int lightVAO;
//    glGenVertexArrays(1, &lightVAO);
//    glBindVertexArray(lightVAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//
//    lightingShader.use();
//    lightingShader.setInt("material.diffuse", 0);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("7.4.camera.vs", "7.4.camera.fs");



//    cout << sizeof(closeData[0]) << endl;
//    //我尼玛，全都可以放在一个循环里了
//    for(int i = 0;i < modelNum; i++)
//    {
//        glGenVertexArrays(1, &faceVAO[i]);
//        glGenBuffers(1, &faceVBO[i]);
//
//
//        //第一个矩形
//        glBindVertexArray(faceVAO[i]);
//
//        glBindBuffer(GL_ARRAY_BUFFER, faceVBO[i]);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(faceData[i]), faceData[i], GL_STATIC_DRAW);
////        cout << "number: " << i << endl;
//        // position attribute
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//
//        //处理这个线
//        faultUp[i] = FloatToVertex(faultData[i][0], (sizeof(faultData[i][0])) / 4);
////        cout << "the z " << faultUp[i][0].z << endl;
//        faultDown[i] = FloatToVertex(faultData[i][1], (sizeof(faultData[i][1])) / 4);
//
//        drawInit(faultUpVAO[i], faultUpVBO[i], faultUp[i], sizeof(faultData[i][0]) / 12);
//
//        drawInit(faultDownVAO[i], faultDownVBO[i], faultDown[i], sizeof(faultData[i][1]) / 12);
//
//    }

    //顺序处理，先把点输进去，这里多处理了2个点。
    for(int i = 0 ;i < modelNum + 2; i++)
    {
        closeLine[i] = FloatToVertex(closeData[i], closeDataNum[i] * 3);

        cout << "closeLine raw " << closeLine[i][0].x << endl;
        drawInit(faceVAO[i], faceVBO[i], closeLine[i], closeDataNum[i]);

    }

    
    
    for(int i = 0;i < modelNum - 1; i++)
    {
        //平移
        faultMoveFunction(closeLine[i+1], closeDataNum[i+1], -1.5f, zD);
        //放缩,对x和y
        scaleFunction(closeLine[i], closeDataNum[i], closeLine[i+1], closeDataNum[i+1], i);

        //--------
        //转化
        vector<Point*> out = VertexsToPoints(closeLine[i], closeDataNum[i]);
        //三角化
        closePoly2Tri(closeLine[i], closeDataNum[i], closeLine[i+1], closeDataNum[i+1], i);
        //移回去
        cout << "closeLine befoe " << closeLine[i+1][0].x << endl;
        closeLineBack(closeLine[i+1], closeDataNum[i+1], i);

        cout << "closeLine " << closeLine[i+1][0].x << endl;
    }


    //放缩处理是一样的，主要是把两个圆放进去。
    
    for(int i = modelNum - 1; i < modelNum ; i++)
    {
        faultMoveFunction(closeLine[i+1], closeDataNum[i+1], -1.5f, zD);
        faultMoveFunction(closeLine[i+2], closeDataNum[i+2], -1.5f, zD);
        drawInit(faceVAO[i + 1], faceVBO[i + 1], closeLine[i + 1], closeDataNum[i + 1]);
        drawInit(faceVAO[i + 2], faceVBO[i + 2], closeLine[i + 2], closeDataNum[i + 2]);

        //放缩,对x和y
        //不放缩了
//        scaleFunction(closeLine[i], closeDataNum[i], closeLine[i+1], closeDataNum[i+1], i);

        //--------
        //转化 最外层
        vector<Point*> out = VertexsToPoints(closeLine[i], closeDataNum[i]);

        polylines[modelNum].push_back(out);

        cdt[modelNum] = new CDT(out);
        //加洞
        vector<Point*> hole1 = VertexsToPoints(closeLine[i+1], closeDataNum[i+1]);
        cdt[modelNum]->AddHole(hole1);
        polylines[modelNum].push_back(hole1);

        vector<Point*> hole2 = VertexsToPoints(closeLine[i+2], closeDataNum[i+2]);
        cdt[modelNum]->AddHole(hole2);
        polylines[modelNum].push_back(hole2);





        //再插入洞
//
//        //开始剖分
        cdt[modelNum]->Triangulate();

        //map是完整的剖分（包含空洞的剖分）？
        map[modelNum] = cdt[modelNum]->GetMap();
        triangles[modelNum] = cdt[modelNum]->GetTriangles();

        Poly2TriBind(PolyVAOs[modelNum], PolyVBOs[modelNum], textures[modelNum],   triangles[modelNum]);
    }
    




// Fill polygon structure with actual data. Any winding order works.
// The first polyline defines the main polygon.

//    polygon.push_back({{100, 0, 1}, {100, 100, 1}, {0, 100, 1}, {0, 0, 3}});
//// Following polylines define holes.
//    polygon.push_back({{75, 25,1}, {75, 75,2 }, {25, 75,3}, {25, 25, 4}});
//    cout << "polygon size :" << polygon.size() << endl;

//    std::vector<N> indices = mapbox::earcut<N>(polygon);
//
//    cout << "the indices size: " << indices.size() <<endl;
//    for (const auto& i : indices) {
//        Point point;
//        cout  << i<< endl;
////        cout << indices.at(i) << endl;
////        cout << endl;
//        //因为这个polygon只有一个，那么就是第一个，那么怎么访问第2个poly呢？这个是个问题了
//        //如果大于4则是访问第二个多边形
//        if(i >= 4)
//        {
//            point.x = polygon.at(1).at(i - 4).at(0);
//            point.y = polygon.at(1).at(i - 4).at(1);
//            point.z = polygon.at(1).at(i - 4).at(2);
//        }
//        else
//        {
//            point.x = polygon.at(0).at(i).at(0);
//            point.y = polygon.at(0).at(i).at(1);
//            point.z = polygon.at(0).at(i).at(2);
//        }
//
//        cout <<  point.x<< " " << point.y << " "  << point.z << endl;
//
//    }

    //开始全自动处理,减1是因为n个面只做n-1次处理
//    for(int i = 0, j = 0; i < modelNum - 1; i++)
//    {
//        //主要也是这个循环，感觉这个循环可以做很多事了。
//        moveFunction(faultUp[i], 10, faultUp[i+1], 10, i, 0);
//        moveFunction(faultDown[i], 10, faultDown[i+1], 10, i , 1);
//        //测试效果
//
//        //平移完后就开始三角化
//        //这里的i和i+1不能这样用，这里的三角
//        poly2Tri(faultMerge(faultUp[i], 10, faultUp[i+1], 10), 20, j);
//        //不能用这个j，j和i没有对应
//        //i为线的值，但是要在里面判断一下是哪一条，可以通过j来判断
//        //只处理up的线
//        lineBack(faultUp[i+1], 10, i, j++ );
//        poly2Tri(faultMerge(faultDown[i], 10, faultDown[i+1], 10), 20, j);
//        lineBack(faultDown[i+1], 10, i, j++);
////
////        //计算多余三角和撤回
////        //只算这个面的其中一半
//
//        drawInit(faultUpVAO[i+1], faultUpVBO[i+1], faultUp[i+1], sizeof(faultData[i+1][0]) / 12);
//        drawInit(faultDownVAO[i+1], faultDownVBO[i+1], faultDown[+1], sizeof(faultData[i+1][1]) / 12);
//    }


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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



//        lightingShader.use();
//        lightingShader.setVec3("light.position", lightPos);
//        lightingShader.setVec3("viewPos", camera.Position);
//
//        // light properties
//        lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
//        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
//        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//
//        // material properties
//        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
//        lightingShader.setFloat("material.shininess", 64.0f);
//
//        // view/projection transformations
//        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        glm::mat4 view = camera.GetViewMatrix();
//        lightingShader.setMat4("projection", projection);
//        lightingShader.setMat4("view", view);
//
//        // world transformation
//        glm::mat4 model;
//        lightingShader.setMat4("model", model);
//
////
//        //这里是灯光的shadel
//        lampShader.use();
//        lampShader.setMat4("projection", projection);
//        lampShader.setMat4("view", view);
//        model = glm::mat4();
//        model = glm::translate(model, lightPos);
//        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
//        lampShader.setMat4("model", model);
//
//        glBindVertexArray(lightVAO);
//        glDrawArrays(GL_TRIANGLES, 0, 36);


        // activate shader
        ourShader.use();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //这个变量很重要
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);



        //两条线，数据还是要改一下
        for(int i = 0; i < modelNum + 2; i++)
        {
//            cout << "number : " << i << endl;
            //先画框
            glBindVertexArray(faceVAO[i]);
            glm::mat4 model;
//            model = glm::translate(model, cubePositions[0]);
            float angle = 20.0f * 0;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
//            lightingShader.setMat4("model", model);

            //可以先画这个外面的点，然后在画线啊真的蠢！
            glDrawArrays(GL_LINE_LOOP, 0, closeDataNum[i]);

//            glBindVertexArray(faultUpVAO[i]);
//            //这里算不出结构体指针所指向的大小，只能用之前的数组代替了。
//            glDrawArrays(GL_LINE_STRIP,0 , (sizeof(faultData[i][0])) / 12);
//
//            glBindVertexArray(faultDownVAO[i]);
//            glDrawArrays(GL_LINE_STRIP,0 , (sizeof(faultData[i][1])) / 12);
        }


        for(int j = 0; j < modelNum + 1 ; j++) {
                cout << j << endl;
            for (int i = 0; i < triangles[j].size(); i++) {
//                cout << triangles[j].size() << endl;
                if (!triangles[j][i]->isHide) {

                    //激活一下这个纹理
//                    glActiveTexture(GL_TEXTURE0);
//                    glBindTexture(GL_TEXTURE_2D, textures[j][i]);


                    glBindVertexArray(PolyVAOs[j][i]);
                    glDrawArrays(GL_LINE_STRIP, 0, 3);
//                    glDrawArrays(GL_TRIANGLE_STRIP, 0 , 3);

                }
            }
        }

//        if(faultMove && cntMove == 0)
//        {
//
//
//
//
//            cntMove = 1;
//            //重新载入一下，加入缓冲
//            drawInit(faultUpVAO[0], faultUpVBO[0], faultUp[0], sizeof(faultData[0]) / 12);
//            drawInit(faultUpVAO[1], faultUpVBO[1], faultUp[1], sizeof(faultData[1]) / 12);
//
//            del->Init((faultMerge(faultUp[0], 10, faultUp[1], 10)), 20);
//
//        }
//        if(moveBack && cntBack == 0)
//        {
//            cntBack = 1;
//            //重新载入一下，加入缓冲
//            drawInit(faultUpVAO[0], faultUpVBO[0], faultUp[0], sizeof(faultData[0]) / 12);
//            drawInit(faultUpVAO[1], faultUpVBO[1], faultUp[1], sizeof(faultData[1]) / 12);
//
//            del->Init((faultMerge(faultUp[0], 10, faultUp[1], 10)), 20);
//
//        }
//        glBindVertexArray(fault2_upVAO);
//        //这里算不出结构体指针所指向的大小，只能用之前的数组代替了。
//        glDrawArrays(GL_LINE_STRIP, 0 , sizeof(fault2[0]) / 12);
//
//        glBindVertexArray(fault2_downVAO);
//        glDrawArrays(GL_LINE_STRIP,0 , (sizeof(fault2[1])) / 12);
//
//        glBindVertexArray(fault1VAO);
//        glDrawArrays(GL_LINE_STRIP,0 , (sizeof(fault1))/12);
//        glBindVertexArray(DelaunayVAO);
//        glDrawArrays(GL_LINE_STRIP,0,del.HowMany * 3);
//
//        if(DelaunayOpen)
//        {
//            //将剖分三角缓冲画出来
//            for (int i = 1; i < del->HowMany + 1 ; i++)
//            {
//                glColor3f(1, 0, 0);
//                glBindVertexArray(DelTraVAOs[i]);
//
//                glDrawArrays(GL_LINE_LOOP, 0, 3);
//            }
//        }
//
//        if(Poly2TriOpen)
//        {
//            for(int j = 0; j < (modelNum - 1) * 2 ; j++)
//            {
////                cout << j << endl;
//                for (int i = 0; i < triangles[j].size(); i++)
//                {
//                    if(!triangles[j][i]->isHide)
//                    {
//
//                        //激活一下这个纹理
//                        glActiveTexture(GL_TEXTURE0);
//                        glBindTexture(GL_TEXTURE_2D, textures[j][i]);
//
//
//                        glBindVertexArray(PolyVAOs[j][i]);
//                        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
//
//
//                    }
//                }
//                if( isAddTra[j] )
//                {
//                    for(int i = 0; i < extraTriangles[j].size(); i++)
//                    {
//                        glBindVertexArray(AddVAOs[j][i]);
//
//                        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
//                    }
//                }
//            }
////            将剖分三角缓冲画出来
//
//        }

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
void drawInit(unsigned int & VAO, unsigned int & VBO, VERTEX *target, int num)
{
    //还是要逐个复制啊，这感觉有点浪费空间哦。个人感觉也是没效果的
    VERTEX source[num];
    for(int i = 0; i < num; i++)
    {
        source[i] = target[i];
//        cout << "the z :" << source[i].z << endl;
//        cout<<source[i].x<<" ";
//        cout<<source[i].y<<" ";
//        cout<<source[i].z<<endl;
    }

    //这里要直接用引用还是要去了？感觉好像没报错是没毛病的。

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(source), source, GL_STATIC_DRAW);

    // position attribute
    //这里的步长为3，之前的是5因为有纹理坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}




void DelaunayBind(unsigned int * DeVAOs, unsigned int * DeVBOs, int DeNum, Delaunay * del)
{
    glGenVertexArrays(DeNum, DeVAOs);
    glGenBuffers(DeNum, DeVBOs);

    //循环读取del里的三角形顶点数据
    for(int i = 1; i < DeNum; i++)
    {
        float TraVertex[9];
        //不对啊，这里还有vvo和vv1？傻逼了
        //将顶点分配给这个float
        TraVertex[0] = del->Vertex[del->Triangle[i].vv0].x;
        TraVertex[1] = del->Vertex[del->Triangle[i].vv0].y;
        TraVertex[2] = del->Vertex[del->Triangle[i].vv0].z;
        TraVertex[3] = del->Vertex[del->Triangle[i].vv1].x;
        TraVertex[4] = del->Vertex[del->Triangle[i].vv1].y;
        TraVertex[5] = del->Vertex[del->Triangle[i].vv1].z;
        TraVertex[6] = del->Vertex[del->Triangle[i].vv2].x;
        TraVertex[7] = del->Vertex[del->Triangle[i].vv2].y;
        TraVertex[8] = del->Vertex[del->Triangle[i].vv2].z;
        //一个个三角画的，主要是找顶点，为什么会有不存在的店。



        //绑定到vbo里
        glBindVertexArray(DeVAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, DeVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TraVertex), TraVertex, GL_STATIC_DRAW);

        // position attribute
        //这里的步长为3，之前的是5因为有纹理坐标
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
}

void Poly2TriBind(unsigned int * PolyVAOs, unsigned int * PolyVBOs, unsigned int * texture,  vector<Triangle*> _triangle)
{
    glGenVertexArrays(_triangle.size(), PolyVAOs);
    glGenBuffers(_triangle.size(), PolyVBOs);

    //循环读取Polyl里的三角形顶点数据
//    float TraVertex[9];
    //这里用15，9是三个点的坐标，6是三个纹理坐标
    float TraVertex[24];
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
//            TraVertex[0] = a.x;
//            TraVertex[1] = a.y;
//            TraVertex[2] = a.z;
//            TraVertex[3] = b.x;
//            TraVertex[4] = b.y;
//            TraVertex[5] = b.z;
//            TraVertex[6] = c.x;
//            TraVertex[7] = c.y;
//            TraVertex[8] = c.z;


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


//        if(TraVertex[2] == 0 || TraVertex[5] == 0 || TraVertex[8] == 0)
//            continue;

//        cout << "TraNumber:" << i << endl;
//        for (int j = 0; j < 9; j++)
//        {
//            cout << TraVertex[j] << " ";
//            //应该是这里出错了，尼玛哦，看了我半天日了你爹
//            if ((j + 1) % 3 == 0)
//            {
//                cout << endl;
//            }
//        }
//        cout << endl;

        //绑定到vbo里
        glBindVertexArray(PolyVAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, PolyVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TraVertex), TraVertex, GL_STATIC_DRAW);

        // position attribute
        //这里的步长为3，之前的是5因为有纹理坐标
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);


        // texture 1
        // ---------
        //绑定一下纹理
        texture[i] = loadTexture(FileSystem::getPath("resources/textures/container.jpg").c_str());
//        glGenTextures(1, &texture[i]);
//        glBindTexture(GL_TEXTURE_2D, texture[i]);
//        // set the texture wrapping parameters
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        // set texture filtering parameters
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        // load image, create texture and generate mipmaps
//        int width, height, nrChannels;
//        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
//        unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
//        if (data)
//        {
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//            glGenerateMipmap(GL_TEXTURE_2D);
//        }
//        else
//        {
//            std::cout << "Failed to load texture" << std::endl;
//        }
//        stbi_image_free(data);
//
    }
}

void AddTriBind(unsigned int * AddVAOs, unsigned int * AddVBOs, unsigned  int * addTexture, vector<AddTriangle> _triangle)
{
    int size = _triangle.size();
    glGenVertexArrays(size, AddVAOs);
    glGenBuffers(size, AddVBOs);
    cout << "the add size : " << size << endl;
    //循环读取Addl里的三角形顶点数据
    for (int i = 0; i < size; i++)
    {
        //15+9，现在加入了法向量实现漫反射
        float TraVertex[24];
        AddTriangle t = _triangle[i];
        VERTEX a = t.a;
        VERTEX b = t.b;
        VERTEX c = t.c;

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




//        if(TraVertex[2] == 0 || TraVertex[5] == 0 || TraVertex[8] == 0)
//            continue;
        //一个个三角画的，主要是找顶点，为什么会有不存在的店。
        //0 0.24 1.5 比如这个点，有哦

//        cout << "TraNumber:" << i << endl;
//        for (int j = 0; j < 9; j++)
//        {
//            cout << TraVertex[j] << " ";
//            if ((j + 1) % 3 == 0)
//            {
//                cout << endl;
//            }
//        }
//        cout << endl;

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
        addTexture[i] = loadTexture(FileSystem::getPath("resources/textures/bricks2.jpg").c_str());

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
void ExcessTraHandle(Triangle* _triangle, VERTEX oppositeLines[], int num, int index)
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
    if ( Lines == ab)
    {
        d1 = DistanceOfPointLinesIn2D(c->PointToVertex(), a->PointToVertex(), b->PointToVertex());
        projectionPoint = c->PointToVertex();
        //将y的值减去d1就是投影在多余的线的点。
        projectionPoint.y -= d1;
        d2 = DistanceOfOpposite(projectionPoint, oppositeLines, num, oppositeIndex);
    }
    else if ( Lines == ac)
    {
        d1 = DistanceOfPointLinesIn2D(b->PointToVertex(), a->PointToVertex(), c->PointToVertex());
        projectionPoint = b->PointToVertex();
        //将y的值减去d1就是投影在多余的线的点。
        projectionPoint.y -= d1;
        d2 = DistanceOfOpposite(projectionPoint, oppositeLines, num, oppositeIndex);
    }
    else
    {
        d1 = DistanceOfPointLinesIn2D(a->PointToVertex(), b->PointToVertex(), c->PointToVertex());
        projectionPoint = a->PointToVertex();
        //将y的值减去d1就是投影在多余的线的点。
        projectionPoint.y -= d1;
        d2 = DistanceOfOpposite(projectionPoint, oppositeLines, num, oppositeIndex);
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
        centerPoint.y = c->y;
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
        centerPoint.y = b->y;
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
        centerPoint.y = a->y;
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
//    cout << " center "<< centerPoint.x << " " << centerPoint.y << " " << centerPoint.z << endl;
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



void moveFunction(VERTEX * fault1, int num1, VERTEX * fault2, int num2, int index, int which)
{
//    cout << " move" << endl;
    //先平移z
    int moveDirectionCnt = 0;
    faultMoveSize[index][which][moveDirectionCnt].size = fault1->z - fault2->z;
//    cout << faultMoveSize[index][which][moveDirectionCnt].size <<endl;
    faultMoveSize[index][which][moveDirectionCnt].md = zD;
    faultMoveFunction(fault2, num1,faultMoveSize[index][which][moveDirectionCnt].size, faultMoveSize[index][which][moveDirectionCnt].md);
    moveDirectionCnt++;
    //平移y轴
    float yMoveSize = 0.0f;
    while( !faultIntersect(fault1, num1, fault2, num2) )
    {
        //一般是只要平移这个y轴？这样以后处理不了转弯的情况了。
        faultMoveSize[index][which][moveDirectionCnt].size = (yMoveSize += -0.1f);
        faultMoveSize[index][which][moveDirectionCnt].md = yD;
        faultMoveFunction(fault2, num1, faultMoveSize[index][which][moveDirectionCnt].size, faultMoveSize[index][which][moveDirectionCnt].md);
    }
}

//如果只是放缩那么我就不管这个平移了，直接判断放缩了！每次放缩1点
void scaleFunction(VERTEX * fault1, int num1, VERTEX * fault2, int num2, int index)
{
    scaleSize[index] = 1.0f;

    //判断是否有相交对线
    while( !faultIntersect(fault1, num1, fault2, num2) )
    {
        //这里的改变0.1是不是有问题，如果两次之后那么就是0.9 * 0.8 = 0.72
        //还真是这样，
        //每次改变0.1

        //变化之前先把它乘回来，不知道这样是不是开销有点大，我估摸着时间复杂度是O(N)
        faultScaleFunction(fault2,num2, 1.0f / (scaleSize[index]) ,xD);
        faultScaleFunction(fault2,num2, 1.0f / (scaleSize[index]) ,yD);
        scaleSize[index] -= 0.1f;
//        cout << "index " << index << " fault[0] " << fault2[0].x << endl;
//        cout << scaleSize[index] << endl;
        faultScaleFunction(fault2,num2, scaleSize[index] ,xD);
        faultScaleFunction(fault2,num2, scaleSize[index] ,yD);
//        cout << "after index " << index << " fault[0] " << fault2[0].x << endl;

    }
}
void poly2Tri(VERTEX * Merge, int num, int index)
{
    //            VERTEX *Merge = faultMerge(faultUp[0], 10, faultUp[1], 10);
//            //将vertex的xy坐标输入到这个poly
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

//                cout << " double "<< x<< " " << y <<" "<< z <<endl;
            }

            //用完就删除
            delete [] Merge;
//        //将这个polyline输入到polylines里去，后者应该是这个集合？
            polylines[index].push_back(polyline[index]);

            cdt[index] = new CDT(polyline[index]);
//
//        //开始剖分
            cdt[index]->Triangulate();

            //map是完整的剖分（包含空洞的剖分）？
            map[index] = cdt[index]->GetMap();
            triangles[index] = cdt[index]->GetTriangles();
//            cout << "the poly2tri index " <<index<<endl;
//            cout << "the poly2tir Triangulate size :"<< triangles[index].size() << endl;



            //现在先不要绑定，这个只是剖分，等返回后再绑定
            //开始绑定poly
//            Poly2TriBind(PolyVAOs[index], PolyVBOs[index], triangles[index]);
            //这里有问题，多遍历了一边，要查询一下怎么清除
            //打开剖分
            Poly2TriOpen = true;
}

void closePoly2Tri(VERTEX * closeOut, int num1, VERTEX * closeHole, int num2, int index)
{
    //转换
    vector<Point*> out = VertexsToPoints(closeOut, num1);
    polylines[index].push_back(out);

    cdt[index] = new CDT(out);
    //加洞
    vector<Point*> hole = VertexsToPoints(closeHole, num2);
    cdt[index]->AddHole(hole);

    polylines[index].push_back(hole);


    //再插入洞
//
//        //开始剖分
    cdt[index]->Triangulate();

    //map是完整的剖分（包含空洞的剖分）？
    map[index] = cdt[index]->GetMap();
    triangles[index] = cdt[index]->GetTriangles();


}
void lineBack(VERTEX * _fault, int num1, int indexLine, int indexTra)
{
    //0 为上层， 1 为下层
    int line = 0;

    //用indexTra来判断indexLine是上层还是下层
    if((indexLine * 2) == (indexTra + 1))
        line = 1;
    //获取平移量,但是要判断一下这个是否有平移
    for(int j = 0;j < 3; j++)
    {
//        cout << " the size " << faultMoveSize[index][line][j].size<< endl;
//        cout << " the size dir " << faultMoveSize[index][line][j].md<< endl;
        if(faultMoveSize[indexLine][line][j].size == 0)
            continue;
        faultMoveFunction(_fault, num1,  0.0 - faultMoveSize[indexLine][line][j].size, faultMoveSize[indexLine][line][j].md);
//        faultMoveFunction(_faultDown, num2, 0.0 - faultMoveSize[index][1][j].size, faultMoveSize[index][1][j].md);

//        drawInit(faultUpVAO[index], faultUpVBO[index], faultUp[index], sizeof(faultData[index]) / 12);
    }

    //线平移后要绑定一下


    //获取初始三角现在要改变这个三角里的数据
//    cout << "get in indexTra:" << indexTra <<endl;
//    cout << triangles[indexTra].size() << endl;
    for (int i = 0; i < triangles[indexTra].size(); i++)
    {
        Triangle &t = *triangles[indexTra][i];
//                Point &a = *t.GetPoint(0);
//                Point &b = *t.GetPoint(1);
//                Point &c = *t.GetPoint(2);
        //看是否该三角是在同一直线的，这种三角就需要插点。
        int pointsInLineOne = 0;
        int pointsInLineTwo = 0;

        //现在是引用，应该可以改变结构体的数据吧
        for(int j = 0 ;j < 3; j++)
        {
            Point &point = *t.GetPoint(j);
//            cout << "point.indexTra" << point.indexTra<<endl;
            //好像这个不同的三角point数据也是共享的，那么猜测可能这个三角用的也是这个索引数据。
            //在point结构体里加个是否移动的属性
            //这个10是分界值
            if(point.index >= 10)
                pointsInLineTwo++;
            else
                pointsInLineOne++;
            if(point.index >= 10 && !point.isMove)
            {
                //3个方向的平移值。
                for(int j = 0;j < 3; j++)
                {
                    //判断是上面的线还是下面的线，一半双数是上面的线，单数是下面的线
                    //0为上

                    if(faultMoveSize[indexLine][line][j].size == 0)
                        continue;
//                    cout << "indexTra " << indexTra<<endl;
//                    cout << "the size : " << faultMoveSize[indexTra][line][j].size<< endl;
                    if(faultMoveSize[indexLine][line][j].md == xD)
                        point.x -= faultMoveSize[indexLine][line][j].size;
                    else if (faultMoveSize[indexLine][line][j].md == yD)
                        point.y -= faultMoveSize[indexLine][line][j].size;
                    else
                        point.z -= faultMoveSize[indexLine][line][j].size;
                }
//                point.y += 0.25f;
//                point.z += -1.0f;
                point.isMove = true;
            }
        }
//        cout<< " the point " << pointsInLineOne<<endl;
//        cout<< " the point " << pointsInLineTwo<<endl;
//        cout<<endl;
        //如果不需要平移的线段有多余三角
        if(pointsInLineOne == 3)
        {
            cout << "the index " <<indexTra << endl;
            //对这个三角搞事情
            //这个三角要隐藏起来
            t.isHide = true;

            //直接隐藏这个点吧。
            t.HidePoints();
            isAddTra[indexTra] = true;
            //这里要传入一个三角的对线。
            //如果是上层
            if(line == 0)
            {
                cout << "0" << endl;
                ExcessTraHandle(&t, faultUp[indexLine + 1], 10, indexTra);
            }
            else
            {
                ExcessTraHandle(&t, faultDown[indexLine + 1], 10, indexTra);
            }


        }
        //多余三角在第二条边，那么传入的就是第一条边的线
        if(pointsInLineTwo == 3)
        {
            t.isHide = true;
            t.HidePoints();
            isAddTra[indexTra] = true;
            //对移动到对面的三角搞事
            if(line == 0)
            {
                ExcessTraHandle(&t, faultUp[indexLine], 10, indexTra);
            }
            else
            {
                ExcessTraHandle(&t, faultDown[indexLine], 10, indexTra);
            }
        }
    }
    //重新绑定
    Poly2TriBind(PolyVAOs[indexTra], PolyVBOs[indexTra], textures[indexTra],   triangles[indexTra]);

    if( isAddTra[indexTra] )
    {
        //这里的绑定也会出问题
       AddTriBind(AddVAOs[indexTra], AddVBOs[indexTra], addTextures[indexTra],  extraTriangles[indexTra]);
    }
    moveBack = true;
}

//判断三角形里的顶点是否在闭线里，如果在则平移。
void closeLineBack(VERTEX * _fault, int num1, int indexTra)
{
    int traSize = triangles[indexTra].size();
    for (int i = 0; i < traSize; i++)
    {
        Triangle &t = *triangles[indexTra][i];
        for(int j = 0 ;j < 3; j++)
        {
            Point &point = *t.GetPoint(j);
            //如果这个点在
            if(VertexInVertexs(point.PointToVertex(), _fault, num1))
            {
                if(point.isMove)
                    continue;

                point.isMove = true;

                point.z += 1.5f;

                point.x *= (1.0f / scaleSize[indexTra]);
                point.y *= (1.0f / scaleSize[indexTra]);
            }
        }

    }
    //将线平移回去
    //放缩回去
    faultScaleFunction(_fault, num1, (1.0f / scaleSize[indexTra]), xD);
    faultScaleFunction(_fault, num1, (1.0f / scaleSize[indexTra]), yD);
    faultMoveFunction(_fault, num1, 1.5f, zD);

    drawInit(faceVAO[indexTra + 1], faceVBO[indexTra + 1], _fault, num1);

    Poly2TriBind(PolyVAOs[indexTra], PolyVBOs[indexTra], textures[indexTra],   triangles[indexTra]);

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

vector<Point*> VertexsToPoints(VERTEX * vertex, int num)
{
    vector<Point*> Points;
    for(int i = 0; i < num; i++)
    {
        double x = vertex[i].x;
        double y = vertex[i].y;
        double z = vertex[i].z;
        Point *point = new Point(x,y);
        point->z = z;
        //加入序列号，看是否有用
        point->index = i;

        Points.push_back(point);
    }
    return Points;
}