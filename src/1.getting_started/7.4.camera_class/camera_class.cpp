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




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


//一些和poly2tri相关的变量定义
/// Dude hole examples
vector<Point*> CreateHeadHole();
vector<Point*> CreateChestHole();

/// Constrained triangles
vector<Triangle*> triangles;
/// Triangle map
list<Triangle*> map;
/// Polylines
//这个好像还是个二维数组？有点牛批
vector< vector<Point*> > polylines;

vector<p2t::Point*> polyline;
//这个是用开画图的？
CDT* cdt;

//多出的三角形
vector<AddTriangle> extraTriangles;


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

//顶点
VERTEX *fault1_up, *fault1_down;
VERTEX *fault2_up, *fault2_down;


Delaunay *del;
unsigned int DelTraVBOs[1024], DelTraVAOs[1024];
unsigned int PolyVBOs[1024], PolyVAOs[1024];
unsigned int AddVBOs[1024], AddVAOs[1024];
bool isAddTra = false;

unsigned int EarVBOs[1024], EarVAOs[1024];

//全局变量，用来平移之类的
float fault1[2][30] = {
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
//            0.5f, 0.4f,   -0.5f+ 1.0f,
};

//第二个平面的断层
//这种断层数据不要一样，主要是这个z坐标的区别。
float fault2[2][30] = {
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
                -0.5f, 0.22f,  -0.5f}
        //这个点是尼玛外面的点吧
//            -0.5f, 0.5f, -0.5f ,
//            -0.5f, 0.22f,  -0.5f ,
        //怎么这个点重复了，有毒。
        //x下层
        ,
        {
            -0.5f, -0.24f,  -0.5f ,
                -0.41f, -0.23f,  -0.5f ,
                -0.29f, -0.23f,  -0.5f ,
                -0.23f, -0.29f,  -0.5f ,
                -0.05f, -0.32f,  -0.5f ,
                0.08f, -0.31f,  -0.5f ,
                0.19f, -0.25f,  -0.5f ,
                0.29f, -0.3f,  -0.5f ,
                0.4f, -0.26f,  -0.5f ,
                0.5f, -0.29f, -0.5f }
//            0.5f, 0.4f,   -0.5f ,
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

void Poly2TriBind(unsigned int * PolyVAOs, unsigned int * PolyVBOs, vector<Triangle*> _triangle);

void AddTriBind(unsigned int * AddVAOs, unsigned int * AddVBOs, vector<AddTriangle> _triangle);

void EarCutBind(unsigned int * EarVAOs, unsigned int * EarVBOs, std::vector<N> indices);


void ExcessTraHandle(Triangle* _triangle, VERTEX oppositeLines[], int num);

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

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("7.4.camera.vs", "7.4.camera.fs");
    //看下怎么单独应用这个shader
//    Shader ourShader_other("7.4.camera1.vs", "7.4.camera1.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    //六个面的坐标值
    

    float cube1[] = {
            -0.5f, 0.5f,  -0.5f,  0.0f, 0.0f,
            0.5f, 0.5f,  -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            -0.5f,-0.5f,  -0.5f,  0.0f, 0.0f,
    };

    float cube22[] = {
            -0.5f, 0.5f,  -0.5f + 1.0f,  0.0f, 0.0f,
            0.5f, 0.5f,  -0.5f+ 1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f+ 1.0f,  0.0f, 0.0f,
            -0.5f,-0.5f,  -0.5f+ 1.0f,  0.0f, 0.0f,
    };
    // world space positions of our cubes
    //正方体的位置
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 0.0f,  0.0f, -1.0f),
//        glm::vec3(-1.5f, -2.2f, -2.5f),
//        glm::vec3(-3.8f, -1.0f, -12.3f),
//        glm::vec3( 2.4f, -0.4f, -3.5f),
//        glm::vec3(-1.7f,  3.0f, -7.5f),
//        glm::vec3( 1.3f, -1.0f, -2.5f),
//        glm::vec3( 1.5f,  1.0f, -2.5f),
//        glm::vec3( 1.5f,  0.2f, -1.5f),
//        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
//    vector <Point> point;
//    Point pointa(0.5f, 0.5f, 0.5f);
//    point.push_back(pointa);
//
//    Point **outside = new Point* [4];
//    outside[0] = new Point(-0.5f,0.5f,-0.5f);
//    outside[1] = new Point(0.5f, 0.5f, -0.5f);
//    outside[2] = new Point(0.5f, -0.5f, -0.5f);
//    outside[3] = new Point(-0.5f, -0.5f, -0.5f);
//
////    Point point(0.5f,0.5f,0.5f);
////    point[1].x = 0.5f;
//    //指针，然后大小变为8了
////    std::cout<<sizeof(*(point)<<std::endl;
//    cout<<sizeof(*(outside[0])) * 4<<endl;
//    cout<<sizeof(cube22)<<endl;
    //两个面
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    //第一个矩形
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube1), cube1, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //第二个矩形
    glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube22),cube22, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


//    VERTEX vertex[20];

   //分配空间,sizeof是120，那么要除以4啊。
   fault1_up = FloatToVertex(fault1[0], (sizeof(fault1[0])) / 4);
   fault1_down = FloatToVertex(fault1[1], (sizeof(fault1[1])) / 4);

    
   //把这个原来的线也要放进缓冲器
    unsigned int fault1_upVBO, fault1_upVAO;
    drawInit(fault1_upVAO, fault1_upVBO, fault1_up, sizeof(fault1[0]) / 12);
    
    unsigned int fault1_downVBO, fault1_downVAO;
    drawInit(fault1_downVAO, fault1_downVBO, fault1_down, sizeof(fault1[1]) / 12);


    fault2_up = FloatToVertex(fault2[0], (sizeof(fault2[0])) / 4);
    fault2_down = FloatToVertex(fault2[1], (sizeof(fault2[1])) / 4);


    //把这个原来的线也要放进缓冲器
    unsigned int fault2_upVBO, fault2_upVAO;
    drawInit(fault2_upVAO, fault2_upVBO, fault2_up, sizeof(fault2[0]) / 12);

    unsigned int fault2_downVBO, fault2_downVAO;
    drawInit(fault2_downVAO, fault2_downVBO, fault2_down, sizeof(fault2[1]) / 12);
    

    

   //falut为断层，将其传入三角剖分的构造函数里去。
   //现在改为这个传入顶点了，那么还是要做一个顶点集合，就是两个断层合入为一个断层。


//    VERTEX *MergeFault = NULL;
//    MergeFault = faultMerge(fault1_up, 10, fault1_up, 10);

//    Delaunay del((faultMerge(fault1_up, 10, fault2_up, 10)), 20);
      //初始化
      del = new Delaunay();





////    Delaunay del(fault,60);
//    cout <<"the size of :" << (sizeof(fault1)/4)<< endl;
//
//    //先声明一组vbo
//    int DelTraNumber = del->HowMany + 1;
//    //浪费了一个0这个顶点数据，三角剖分代码里都数据都是从1开始的
//    unsigned int DelTraVBOs[DelTraNumber], DelTraVAOs[DelTraNumber];
//    glGenVertexArrays(DelTraNumber,DelTraVAOs);
//    glGenBuffers(DelTraNumber,DelTraVBOs);
//
//    //循环读取del里的三角形顶点数据
//    for(int i = 1; i < DelTraNumber; i++)
//    {
//        float TraVertex[9];
//        //不对啊，这里还有vvo和vv1？傻逼了
//        //将顶点分配给这个float
//        TraVertex[0] = del->Vertex[del->Triangle[i].vv0].x;
//        TraVertex[1] = del->Vertex[del->Triangle[i].vv0].y;
//        TraVertex[2] = del->Vertex[del->Triangle[i].vv0].z;
//        TraVertex[3] = del->Vertex[del->Triangle[i].vv1].x;
//        TraVertex[4] = del->Vertex[del->Triangle[i].vv1].y;
//        TraVertex[5] = del->Vertex[del->Triangle[i].vv1].z;
//        TraVertex[6] = del->Vertex[del->Triangle[i].vv2].x;
//        TraVertex[7] = del->Vertex[del->Triangle[i].vv2].y;
//        TraVertex[8] = del->Vertex[del->Triangle[i].vv2].z;
//        //一个个三角画的，主要是找顶点，为什么会有不存在的店。
//        //0 0.24 1.5 比如这个点，有哦
//
//        cout<<"TraNumber:"<<i<<endl;
//        for(int j = 0 ;j < 9; j++)
//        {
//            cout<<TraVertex[j]<<" ";
//            //应该是这里出错了，尼玛哦，看了我半天日了你爹
//            if((j+1)%3 == 0)
//            {
//                cout<<endl;
//            }
//        }
//        cout<<endl;
//
//        //绑定到vbo里
//        glBindVertexArray(DelTraVAOs[i]);
//
//        glBindBuffer(GL_ARRAY_BUFFER, DelTraVBOs[i]);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(TraVertex), TraVertex, GL_STATIC_DRAW);
//
//        // position attribute
//        //这里的步长为3，之前的是5因为有纹理坐标
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//    }





//    //一个三角形有三点点，howmany为Delaunay三角的个数。
//    float dataFinish[del.HowMany * 3 ];
//    //但是循环不需要乘以3，是以三角形的结构体存在vertex里的。
//    for(int i = 1, j = 0; i <= del.HowMany && j < del.HowMany * 3; i++,j+=3)
//    {
//        dataFinish[j] = del.Vertex[del.Triangle[i].vv0].x;
//        dataFinish[j+1] = del.Vertex[del.Triangle[i].vv0].y;
//        dataFinish[j+2] = del.Vertex[del.Triangle[i].vv0].z;
//        cout << "the "<<i << "vertex: x:"<<dataFinish[j]<<" y:"<<dataFinish[j+1] << " z:"<< dataFinish[j+2]<<endl;
//    }
//    //放个局部变量
//    //构造完float数组后将其传入顶点缓存器
//    unsigned int DelaunayVBO, DelaunayVAO;
//    glGenVertexArrays(1, &DelaunayVAO);
//    glGenBuffers(1, &DelaunayVBO);
//
//    //第一个矩形
//    glBindVertexArray(DelaunayVAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, DelaunayVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(dataFinish), dataFinish, GL_STATIC_DRAW);
//
//    // position attribute
//    //这里的步长为3，之前的是5因为有纹理坐标
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//   float fault2[] = {
//           -0.5f, -0.24f,  -0.5f+ 1.0f,
//            -0.41f, -0.23f,  -0.5f+ 1.0f,
//            -0.29f, -0.23f,  -0.5f+ 1.0f,
//            -0.23f, -0.29f,  -0.5f+ 1.0f,
//            -0.05f, -0.32f,  -0.5f+ 1.0f,
//            0.08f, -0.31f,  -0.5f+ 1.0f,
//            0.19f, -0.25f,  -0.5f+ 1.0f,
//            0.29f, -0.3f,  -0.5f+ 1.0f,
//            0.4f, -0.26f,  -0.5f+ 1.0f,
//            0.5f, -0.29f, -0.5f+ 1.0f,
//            0.5f, 0.4f,   -0.5f+ 1.0f,
//            0.5f, -0.29f,  -0.5f+ 1.0f
//   };



    // load and create a texture 
    // -------------------------
//    unsigned int texture1, texture2;
//    // texture 1
//    // ---------
//    glGenTextures(1, &texture1);
//    glBindTexture(GL_TEXTURE_2D, texture1);
//    // set the texture wrapping parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    // set texture filtering parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // load image, create texture and generate mipmaps
//    int width, height, nrChannels;
//    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//    unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
//    if (data)
//    {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else
//    {
//        std::cout << "Failed to load texture" << std::endl;
//    }
//    stbi_image_free(data);
//    // texture 2
//    // ---------
//    glGenTextures(1, &texture2);
//    glBindTexture(GL_TEXTURE_2D, texture2);
//    // set the texture wrapping parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    // set texture filtering parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // load image, create texture and generate mipmaps
//    data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
//    if (data)
//    {
//        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else
//    {
//        std::cout << "Failed to load texture" << std::endl;
//    }
//    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
//    ourShader.setInt("texture1", 0);
//    ourShader.setInt("texture2", 1);

//这个三角剖分的地方不应该放在画图的位置，画图只管画顶点就好了。
    // render loop
    // -----------

//    int cnt = 0;
//    for(int i = 0;i < 186; i++)
//    {
//        double x = test[i++];
//        double y = test[i];
//        polyline.push_back(new Point(x,y));
//        cnt ++;
//        cout << " double "<< x<< " " << y<< endl;
//    }
//
//    cout << "Number of constrained edges = " << polyline.size() << endl;
//
//
//    cout << cnt <<endl;





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

    while (!glfwWindowShouldClose(window))
    {
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

        // bind textures on corresponding texture units
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, texture1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // render boxes
        //为什么只画了一个点？
        glBindVertexArray(VAOs[0]);
        for (unsigned int i = 0; i < 1; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model;
//            model = glm::translate(model, cubePositions[0]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            //可以先画这个外面的点，然后在画线啊真的蠢！
            glDrawArrays(GL_LINE_LOOP, 0, 4);
        }
        //绑定顶点数组
        glBindVertexArray(VAOs[1]);
        for (unsigned int i = 0; i < 1; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model;
//            model = glm::translate(model, cubePositions[1]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
//            std::cout<< (sizeof(cube22))/20 <<std::endl;
            //float是4个字节，然后一个坐标有5个值，那么就要除以20了。
//            glDrawArrays(GL_LINE_STRIP, 0, (sizeof(cube22))/20);
            //先画一个Loop这个是外面的框
            glDrawArrays(GL_LINE_LOOP, 0, 4);
            //先固定个数
        }
//可以直接画，那么就传入那个顶点数组好了。

        //两条线，数据还是要改一下
        glBindVertexArray(fault1_upVAO);
        //这里算不出结构体指针所指向的大小，只能用之前的数组代替了。
        glDrawArrays(GL_LINE_STRIP,0 , sizeof(fault1[0]) / 12);

        glBindVertexArray(fault1_downVAO);
        glDrawArrays(GL_LINE_STRIP,0 , (sizeof(fault1[1])) / 12);



        if(faultMove && cntMove == 0)
        {
            cntMove = 1;
            //重新载入一下，加入缓冲
            drawInit(fault2_upVAO, fault2_upVBO, fault2_up, sizeof(fault2[0]) / 12);

            del->Init((faultMerge(fault1_up, 10, fault2_up, 10)), 20);

        }
        if(moveBack && cntBack == 0)
        {
            cntBack = 1;
            //重新载入一下，加入缓冲
            drawInit(fault2_upVAO, fault2_upVBO, fault2_up, sizeof(fault2[0]) / 12);

            del->Init((faultMerge(fault1_up, 10, fault2_up, 10)), 20);

        }
        glBindVertexArray(fault2_upVAO);
        //这里算不出结构体指针所指向的大小，只能用之前的数组代替了。
        glDrawArrays(GL_LINE_STRIP, 0 , sizeof(fault2[0]) / 12);

        glBindVertexArray(fault2_downVAO);
        glDrawArrays(GL_LINE_STRIP,0 , (sizeof(fault2[1])) / 12);
//
//        glBindVertexArray(fault1VAO);
//        glDrawArrays(GL_LINE_STRIP,0 , (sizeof(fault1))/12);
//        glBindVertexArray(DelaunayVAO);
//        glDrawArrays(GL_LINE_STRIP,0,del.HowMany * 3);
//
        if(DelaunayOpen)
        {
            //将剖分三角缓冲画出来
            for (int i = 1; i < del->HowMany + 1 ; i++)
            {
                glColor3f(1, 0, 0);
                glBindVertexArray(DelTraVAOs[i]);

                glDrawArrays(GL_LINE_LOOP, 0, 3);
            }
        }

        if(Poly2TriOpen)
        {
            //将剖分三角缓冲画出来
            for (int i = 0; i < triangles.size(); i++)
            {
                if(triangles[i]->isHide)
                {

                    continue;
                }
                glColor3f(1, 0, 0);
                glBindVertexArray(PolyVAOs[i]);

                glDrawArrays(GL_LINE_LOOP, 0, 3);
            }

            if( isAddTra )
            {
                for(int i = 0; i < extraTriangles.size(); i++)
                {
//                    cout<<"add "<<i<<endl;
                    glBindVertexArray(AddVAOs[i]);

                    glDrawArrays(GL_LINE_LOOP, 0, 3);
                }
            }
        }

        if(EarCutOpen)
        {
            //将剖分三角缓冲画出来
            for (int i = 0; i < EarTraSize; i++)
            {
                glColor3f(1, 0, 0);
                glBindVertexArray(EarVAOs[i]);

                glDrawArrays(GL_LINE_LOOP, 0, 3);
            }
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteVertexArrays(del->HowMany + 1 , DelTraVAOs);
    glDeleteBuffers(del->HowMany + 1, DelTraVBOs);

    delete []fault1_up;
    delete []fault1_down;
    delete []fault2_up;
    delete []fault2_down;

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
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        //还是要设置一下绑定，不然总是触发多次。
        //那还得加一个键啊，不然不能取消。
        if(!DelaunayOpen)
        {
            cout<<"delaunay"<<endl;
            //在这里计算del
            //按了键才进行三角剖分，那么我改的话键是按了键后将输入存入进去，然后开始剖分。获取三角，
            del->Init(faultMerge(fault1_up, 10, fault2_up, 10), 20);
            //绑定缓冲
            DelaunayBind(DelTraVAOs, DelTraVBOs, del->HowMany, del);
            //打开剖分
            DelaunayOpen = true;
        }
    }
    //p 显示poly2tri
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        //还是要设置一下绑定，不然总是触发多次。
        //那还得加一个键啊，不然不能取消。
        if(!Poly2TriOpen)
        {
            cout<<"poly2tri"<<endl;

            VERTEX *Merge = faultMerge(fault1_up, 10, fault2_up, 10);
            //将vertex的xy坐标输入到这个poly
            for(int i = 0;i < 20; i++)
            {
                double x = Merge[i].x;
                double y = Merge[i].y;
                double z = Merge[i].z;
                Point *point = new Point(x,y);
                point->z = z;
                //加入序列号，看是否有用
                point->index = i;
                polyline.push_back(point);

                cout << " double "<< x<< " " << y <<" "<< z <<endl;
            }

            //用完就删除
            delete [] Merge;
//        //将这个polyline输入到polylines里去，后者应该是这个集合？
            polylines.push_back(polyline);

            cdt = new CDT(polyline);
//
//        //开始剖分
            cdt->Triangulate();

            //map是完整的剖分（包含空洞的剖分）？
            map = cdt->GetMap();
            triangles = cdt->GetTriangles();
            cout << "the poly2tir Triangulate size :"<< triangles.size() << endl;
            //开始绑定poly
            Poly2TriBind(PolyVAOs, PolyVBOs, triangles);
            //这里有问题，多遍历了一边，要查询一下怎么清除！
            //打开剖分
            Poly2TriOpen = true;
        }
    }
    //开始耳切法
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        if(!EarCutOpen)
        {
            cout<<"poly2tri"<<endl;

            VERTEX *Merge = faultMerge(fault1_up, 10, fault2_up, 10);
            //这里只能这样定义？
            //将Merge输入到arrays里，能直接用这个VERTEX输入进去
            vector<Points> lines;
            // 不用数组，就单个就行了。
            //个人感觉应该不行，
            for(int i = 0; i < 20;i++)
            {
                Points array;
                array.at(0) = Merge[i].x;
                array.at(1) = Merge[i].y;
                array.at(2) = Merge[i].z;
                lines.push_back(array);
            }
            
            //输入后再push到polygon
            polygon.push_back(lines);

            indices = mapbox::earcut<N>(polygon);
            
            
            //这里写一个绑定函数，输入到缓冲里
            EarCutBind(EarVAOs, EarVBOs, indices);

            cout << "the indices size: " << indices.size() <<endl;
            
            EarCutOpen = true;
        }
    }
    //f1取消显示del
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        if(DelaunayOpen)
            DelaunayOpen = false;
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    {
        if(Poly2TriOpen)
            Poly2TriOpen = false;
    }

    //平移操作，如果按下则进行平移
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        cout<<"move"<<endl;
       //主要是移动一下z轴，先试试。
       if(!faultMove)
       {
           faultMoveFunction(fault2_up, 10, 1.0f, zD);
           faultMoveFunction(fault2_up, 10, -0.25f, yD);
//           for(int i = 0 ;i < 10;i++)
//           {
//               cout<<fault2_up[i].x<<" test ";
//               cout<<fault2_up[i].y<<" ";
//               cout<<fault2_up[i].z<<endl;
//           }
           faultMove = true;
       }

    }
    //剖分后平移回去，就直接改变坐标的点试试。
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        cout<<"back"<<endl;
        //主要是移动一下z轴，先试试。
        if(!moveBack && DelaunayOpen)
        {
            //把三角形里的平移回去
            del->MoveVertex(11, 10, zD, -1.0f);
            del->MoveVertex(11, 10, yD, 0.25f);
            //更改了后要重新缓冲一下
            DelaunayBind(DelTraVAOs, DelTraVBOs, del->HowMany, del);

            //移动过来的线要移动回去啊
            moveBack = true;

            faultMoveFunction(fault2_up, 10, -1.0f, zD);
            faultMoveFunction(fault2_up, 10, 0.25f, yD);
        }
        //这个back可以分多种情况
        else if(!moveBack && EarCutOpen)
        {
            if(polygon.size() == 1)
            {
                //数据写的太死了！
                for(int i = 10;i < 20; i++)
                {
                    polygon.at(0).at(i).at(1) += 0.25f;
                    polygon.at(0).at(i).at(2) += -1.0f;

                }
                EarCutBind(EarVAOs, EarVBOs, indices);
            }
            moveBack = true;
            faultMoveFunction(fault2_up, 10, -1.0f, zD);
            faultMoveFunction(fault2_up, 10, 0.25f, yD);
        }
        else if(!moveBack && Poly2TriOpen)
        {
            //先移动
            faultMoveFunction(fault2_up, 10, -1.0f, zD);
            faultMoveFunction(fault2_up, 10, 0.25f, yD);
            //获取初始三角现在要改变这个三角里的数据
            for (int i = 0; i < triangles.size(); i++)
            {
                Triangle &t = *triangles[i];
//                Point &a = *t.GetPoint(0);
//                Point &b = *t.GetPoint(1);
//                Point &c = *t.GetPoint(2);
                //看是否该三角是在同一直线的，这种三角就需要插点。
                int pointsInLineOne = 0;
                int pointsInLineTwo = 0;

                //基本点
                int pointBase;
                //现在是引用，应该可以改变结构体的数据吧
                for(int j = 0 ;j < 3; j++)
                {
                    Point &point = *t.GetPoint(j);
                    //好像这个不同的三角point数据也是共享的，那么猜测可能这个三角用的也是这个索引数据。
                    //在point结构体里加个是否移动的属性
                    if(point.index >= 10)
                        pointsInLineTwo++;
                    else
                        pointsInLineOne++;
                    if(point.index >= 10 && !point.isMove)
                    {
                        point.y += 0.25f;
                        point.z += -1.0f;
                        point.isMove = true;
                    }
                }
                cout<< " the point " << pointsInLineOne<<endl;
                cout<< " the point " << pointsInLineTwo<<endl;
                cout<<endl;
                //如果不需要平移的线段有多余三角
                if(pointsInLineOne == 3)
                {
                    //对这个三角搞事情
                    //这个三角要隐藏起来
                    t.isHide = true;
                    isAddTra = true;
                    //处理后得到一个d，这个d可以用来干嘛呢？
                    ExcessTraHandle(&t, fault2_up, 10);
                }
                if(pointsInLineTwo == 3)
                {
                    t.isHide = true;
                    isAddTra = true;
                    //对移动到对面的三角搞事
                    ExcessTraHandle(&t, fault1_up, 10);
                }
            }
            //重新绑定
            Poly2TriBind(PolyVAOs, PolyVBOs, triangles);

            if( isAddTra )
            {
               AddTriBind(AddVAOs, AddVBOs, extraTriangles);
            }
            moveBack = true;

        }
    }
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
        //0 0.24 1.5 比如这个点，有哦

//        cout<<"TraNumber:"<<i<<endl;
//        for(int j = 0 ;j < 9; j++)
//        {
//            cout<<TraVertex[j]<<" ";
//            //应该是这里出错了，尼玛哦，看了我半天日了你爹
//            if((j+1)%3 == 0)
//            {
//                cout<<endl;
//            }
//        }
//        cout<<endl;

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

void Poly2TriBind(unsigned int * PolyVAOs, unsigned int * PolyVBOs, vector<Triangle*> _triangle)
{
    glGenVertexArrays(_triangle.size(), PolyVAOs);
    glGenBuffers(_triangle.size(), PolyVBOs);

    //循环读取Polyl里的三角形顶点数据
    for (int i = 0; i < _triangle.size(); i++)
    {
        float TraVertex[9];
        Triangle &t = *_triangle[i];
        if(t.isHide)
            continue;
        Point &a = *t.GetPoint(0);
        Point &b = *t.GetPoint(1);
        Point &c = *t.GetPoint(2);
        //不对啊，这里还有vvo和vv1？傻逼了
        //将顶点分配给这个float

        TraVertex[0] = a.x;
        TraVertex[1] = a.y;
        TraVertex[2] = a.z;
        TraVertex[3] = b.x;
        TraVertex[4] = b.y;
        TraVertex[5] = b.z;
        TraVertex[6] = c.x;
        TraVertex[7] = c.y;
        TraVertex[8] = c.z;

//        if(TraVertex[2] == 0 || TraVertex[5] == 0 || TraVertex[8] == 0)
//            continue;
        //一个个三角画的，主要是找顶点，为什么会有不存在的店。
        //0 0.24 1.5 比如这个点，有哦

        cout << "TraNumber:" << i << endl;
        for (int j = 0; j < 9; j++)
        {
            cout << TraVertex[j] << " ";
            //应该是这里出错了，尼玛哦，看了我半天日了你爹
            if ((j + 1) % 3 == 0)
            {
                cout << endl;
            }
        }
        cout << endl;

        //绑定到vbo里
        glBindVertexArray(PolyVAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, PolyVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TraVertex), TraVertex, GL_STATIC_DRAW);

        // position attribute
        //这里的步长为3，之前的是5因为有纹理坐标
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
    }
}

void AddTriBind(unsigned int * AddVAOs, unsigned int * AddVBOs, vector<AddTriangle> _triangle)
{
    glGenVertexArrays(_triangle.size(), AddVAOs);
    glGenBuffers(_triangle.size(), AddVBOs);

    //循环读取Addl里的三角形顶点数据
    for (int i = 0; i < _triangle.size(); i++)
    {
        float TraVertex[9];
        AddTriangle t = _triangle[i];
        VERTEX a = t.a;
        VERTEX b = t.b;
        VERTEX c = t.c;
        //不对啊，这里还有vvo和vv1？傻逼了
        //将顶点分配给这个float

        TraVertex[0] = a.x;
        TraVertex[1] = a.y;
        TraVertex[2] = a.z;
        TraVertex[3] = b.x;
        TraVertex[4] = b.y;
        TraVertex[5] = b.z;
        TraVertex[6] = c.x;
        TraVertex[7] = c.y;
        TraVertex[8] = c.z;

//        if(TraVertex[2] == 0 || TraVertex[5] == 0 || TraVertex[8] == 0)
//            continue;
        //一个个三角画的，主要是找顶点，为什么会有不存在的店。
        //0 0.24 1.5 比如这个点，有哦

        cout << "TraNumber:" << i << endl;
        for (int j = 0; j < 9; j++)
        {
            cout << TraVertex[j] << " ";
            //应该是这里出错了，尼玛哦，看了我半天日了你爹
            if ((j + 1) % 3 == 0)
            {
                cout << endl;
            }
        }
        cout << endl;

        //绑定到vbo里
        glBindVertexArray(AddVAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, AddVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TraVertex), TraVertex, GL_STATIC_DRAW);

        // position attribute
        //这里的步长为3，之前的是5因为有纹理坐标
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
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
void ExcessTraHandle(Triangle* _triangle, VERTEX oppositeLines[], int num)
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
    cout << "oppositeIndex" << oppositeIndex << endl;
    cout << "Lines "<< Lines << endl;
    cout << "projection point" << projectionPoint.x <<" " << projectionPoint.y<< " " << projectionPoint.z<<endl;
    //这个d就是总偏移量了，也就是point，z的偏移量
    float d = fabs(((a->z / pow(d1, 2) ) + (oppositeLines[0].z / pow(d2, 2))) / ((1 / pow(d1, 2)) + (1 / pow(d2, 2))));

    cout << "the d1 d2 d " << d1<<" "<<d2<<" "<<d<<endl;

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
//        //统一一下方向，都是减去该值。
////        Triangle *triangle = new Triangle(*c, *b, centerPoint);
//
//        //接下来要构造几个三角了
//        extraTriangles->push_back(new Triangle(*c, *b, centerPoint));
//        extraTriangles->push_back(new Triangle(*c, *a, centerPoint));
//
//        //要判断一下这个点是否在同一侧，那么就判断他们的序号值好了
//        if(a->index < b->index)
//        {
//            extraTriangles->push_back(new Triangle(*a, centerPoint, oppositePointOne));
//            extraTriangles->push_back(new Triangle(*b, centerPoint, oppositePointTwo));
//        }
//        else
//        {
//            extraTriangles->push_back(new Triangle(*b, centerPoint, oppositePointOne));
//            extraTriangles->push_back(new Triangle(*a, centerPoint, oppositePointTwo));
//        }
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
    cout << " center "<< centerPoint.x << " " << centerPoint.y << " " << centerPoint.z << endl;

    cout << " left "<< left.x << " " << left.y << " " << left.z << endl;
    extraTriangles.push_back(VertexToTriangle(left, mid, centerPoint));
    extraTriangles.push_back(VertexToTriangle(right, mid, centerPoint));
    extraTriangles.push_back(VertexToTriangle(left, centerPoint, oppositePointOne));
    extraTriangles.push_back(VertexToTriangle(right, centerPoint, oppositePointTwo));
    extraTriangles.push_back(VertexToTriangle(centerPoint, oppositePointTwo, oppositePointOne));

//    AddTriBind(AddVAOs, AddVBOs, extraTriangles);
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