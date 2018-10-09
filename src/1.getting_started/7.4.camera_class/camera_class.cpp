#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/point.h>
#include <Delaunay.h>
#include <iostream>
#include <vector>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//function
//设置一下变量，用按键实现一下功能

//是否开启Delaunay
bool DelaunayOpen = false;

//平移操作

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
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, 0.5f,  -0.5f,  0.0f, 0.0f,
            0.5f, 0.5f,  -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            -0.5f,-0.5f,  -0.5f,  0.0f, 0.0f,


//        -0.5f, 0.5f,  -0.5f,  0.0f, 0.0f,
//        0.5f, 0.5f,  -0.5f,  0.0f, 0.0f,


//        0.5f, 0.25f,  -0.5f,  0.0f, 0.0f,
//        0.41f, 0.29f,  -0.5f,  0.0f, 0.0f,
//        0.3f, 0.32f,  -0.5f,  0.0f, 0.0f,
//        0.18f, 0.26f,  -0.5f,  0.0f, 0.0f,
//        0.0f, 0.24f,  -0.5f,  0.0f, 0.0f,
//        -0.13f, 0.29f,  -0.5f,  0.0f, 0.0f,
//        -0.26f, 0.29f,  -0.5f,  0.0f, 0.0f,
//        -0.38f, 0.25f,  -0.5f,  0.0f, 0.0f,
//        -0.5f, 0.22f,  -0.5f,  0.0f, 0.0f,
////        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
//        -0.5f, 0.22f,  -0.5f,  0.0f, 0.0f,
//
//
//        -0.5f, -0.24f,  -0.5f,  0.0f, 0.0f,
//        -0.41f, -0.23f,  -0.5f,  0.0f, 0.0f,
//        -0.29f, -0.23f,  -0.5f,  0.0f, 0.0f,
//        -0.23f, -0.29f,  -0.5f,  0.0f, 0.0f,
//        -0.05f, -0.32f,  -0.5f,  0.0f, 0.0f,
//        0.08f, -0.31f,  -0.5f,  0.0f, 0.0f,
//        0.19f, -0.25f,  -0.5f,  0.0f, 0.0f,
//        0.29f, -0.23f,  -0.5f,  0.0f, 0.0f,
//        0.4f, -0.26f,  -0.5f,  0.0f, 0.0f,
//        0.5f, -0.29f, -0.5f,  0.0f, 0.0f,
//        0.5f, 0.25f,   -0.5f,  0.0f, 0.0f,
//        0.5f, -0.29f,  -0.5f,  0.0f, 0.0f,
//
//
//        0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//        -0.5f,-0.5f,  -0.5f,  0.0f, 0.0f,
//        -0.5f, -0.24f,  -0.5f,  0.0f, 0.0f,

//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float cube22[] = {
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, 0.5f,  -0.5f + 2.0f,  0.0f, 0.0f,
            0.5f, 0.5f,  -0.5f+ 2.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f+ 2.0f,  0.0f, 0.0f,
            -0.5f,-0.5f,  -0.5f+ 2.0f,  0.0f, 0.0f,
            //断层曲线的位置上半部分
//            0.5f, 0.15f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            0.41f, 0.29f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            0.3f, 0.32f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            0.18f, 0.26f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            0.0f, 0.24f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            -0.13f, 0.29f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            -0.26f, 0.29f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            -0.38f, 0.25f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            -0.5f, 0.22f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            -0.5f, 0.5f, -0.5f+ 2.0f, 0.0f, 0.0f,
//            -0.5f, 0.22f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//
//            //断层曲线下半部分
//            -0.5f, -0.24f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            -0.41f, -0.23f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            -0.29f, -0.23f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            -0.23f, -0.29f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            -0.05f, -0.32f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            0.08f, -0.31f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            0.19f, -0.25f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            0.29f, -0.3f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            0.4f, -0.26f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//            0.5f, -0.29f, -0.5f+ 2.0f,  0.0f, 0.0f,
//            0.5f, 0.4f,   -0.5f+ 2.0f,  0.0f, 0.0f,
//            0.5f, -0.29f,  -0.5f+ 2.0f,  0.0f, 0.0f,
//
//
//
//            -0.5f, -0.24f,  -0.5f+ 2.0f,  0.0f, 0.0f,



//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // world space positions of our cubes
    //正方体的位置
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 0.0f,  0.0f, -1.0f),
//        glm::vec3(-1.5f, -2.2f, -2.5f),
//        glm::vec3(-3.8f, -2.0f, -12.3f),
//        glm::vec3( 2.4f, -0.4f, -3.5f),
//        glm::vec3(-1.7f,  3.0f, -7.5f),
//        glm::vec3( 1.3f, -2.0f, -2.5f),
//        glm::vec3( 1.5f,  2.0f, -2.5f),
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

    //定义一下断层数据
    //感觉还是不要定义这种数组，实在是太麻烦了。没有虽然还是有规律性
    float fault[] = {
            0.5f, 0.15f,  -0.5f+ 2.0f,  
            0.41f, 0.29f,  -0.5f+ 2.0f,
            0.3f, 0.32f,  -0.5f+ 2.0f,
            0.18f, 0.26f,  -0.5f+ 2.0f,  
            0.0f, 0.24f,  -0.5f+ 2.0f,  
            -0.13f, 0.29f,  -0.5f+ 2.0f, 
            -0.26f, 0.29f,  -0.5f+ 2.0f,
            -0.38f, 0.25f,  -0.5f+ 2.0f, 
            -0.5f, 0.22f,  -0.5f+ 2.0f,
            //这个点是尼玛外面的点吧
//            -0.5f, 0.5f, -0.5f+ 2.0f,
            -0.5f, 0.22f,  -0.5f+ 2.0f,

          //x下层
            -0.5f, -0.24f,  -0.5f+ 2.0f,
            -0.41f, -0.23f,  -0.5f+ 2.0f,
            -0.29f, -0.23f,  -0.5f+ 2.0f,
            -0.23f, -0.29f,  -0.5f+ 2.0f,
            -0.05f, -0.32f,  -0.5f+ 2.0f,
            0.08f, -0.31f,  -0.5f+ 2.0f,
            0.19f, -0.25f,  -0.5f+ 2.0f,
            0.29f, -0.3f,  -0.5f+ 2.0f,
            0.4f, -0.26f,  -0.5f+ 2.0f,
            0.5f, -0.29f, -0.5f+ 2.0f,
//            0.5f, 0.4f,   -0.5f+ 2.0f,
            0.5f, -0.29f,  -0.5f+ 2.0f
    };
   //把这个原来的线也要放进缓冲器
    unsigned int faultVBO, faultVAO;
    glGenVertexArrays(1, &faultVAO);
    glGenBuffers(1, &faultVBO);

    glBindVertexArray(faultVAO);

    glBindBuffer(GL_ARRAY_BUFFER, faultVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fault), fault, GL_STATIC_DRAW);

    // position attribute
    //这里的步长为3，之前的是5因为有纹理坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

   //falut为断层，将其传入三角剖分的构造函数里去。
   //除以4好像没毛病，float大小为4，但是好像多出了一个点，要确认一下是否是这个画的线有问题
    Delaunay del(fault,(sizeof(fault)) / 4);
//    Delaunay del(fault,60);
    cout <<"the size of :" << (sizeof(fault)/4)<< endl;

    //先声明一组vbo
    int DelTraNumber = del.HowMany + 1;
    //浪费了一个0这个顶点数据，三角剖分代码里都数据都是从1开始的
    unsigned int DelTraVBOs[DelTraNumber], DelTraVAOs[DelTraNumber];
    glGenVertexArrays(DelTraNumber,DelTraVAOs);
    glGenBuffers(DelTraNumber,DelTraVBOs);

    //循环读取del里的三角形顶点数据
    for(int i = 1; i < DelTraNumber; i++)
    {
        float TraVertex[9];
        //不对啊，这里还有vvo和vv1？傻逼了
        //将顶点分配给这个float
        TraVertex[0] = del.Vertex[del.Triangle[i].vv0].x;
        TraVertex[1] = del.Vertex[del.Triangle[i].vv0].y;
        TraVertex[2] = del.Vertex[del.Triangle[i].vv0].z;
        TraVertex[3] = del.Vertex[del.Triangle[i].vv1].x;
        TraVertex[4] = del.Vertex[del.Triangle[i].vv1].y;
        TraVertex[5] = del.Vertex[del.Triangle[i].vv1].z;
        TraVertex[6] = del.Vertex[del.Triangle[i].vv2].x;
        TraVertex[7] = del.Vertex[del.Triangle[i].vv2].y;
        TraVertex[8] = del.Vertex[del.Triangle[i].vv2].z;
        //一个个三角画的，主要是找顶点，为什么会有不存在的店。
        //0 0.24 1.5 比如这个点，有哦

        cout<<"TraNumber:"<<i<<endl;
        for(int j = 0 ;j < 9; j++)
        {
            cout<<TraVertex[j]<<" ";
            //应该是这里出错了，尼玛哦，看了我半天日了你爹
            if((j+1)%3 == 0)
            {
                cout<<endl;
            }
        }
        cout<<endl;

        //绑定到vbo里
        glBindVertexArray(DelTraVAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, DelTraVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TraVertex), TraVertex, GL_STATIC_DRAW);

        // position attribute
        //这里的步长为3，之前的是5因为有纹理坐标
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }





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
//           -0.5f, -0.24f,  -0.5f+ 2.0f,
//            -0.41f, -0.23f,  -0.5f+ 2.0f,
//            -0.29f, -0.23f,  -0.5f+ 2.0f,
//            -0.23f, -0.29f,  -0.5f+ 2.0f,
//            -0.05f, -0.32f,  -0.5f+ 2.0f,
//            0.08f, -0.31f,  -0.5f+ 2.0f,
//            0.19f, -0.25f,  -0.5f+ 2.0f,
//            0.29f, -0.3f,  -0.5f+ 2.0f,
//            0.4f, -0.26f,  -0.5f+ 2.0f,
//            0.5f, -0.29f, -0.5f+ 2.0f,
//            0.5f, 0.4f,   -0.5f+ 2.0f,
//            0.5f, -0.29f,  -0.5f+ 2.0f
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

        glBindVertexArray(faultVAO);
        glDrawArrays(GL_LINE_STRIP,0 , (sizeof(fault))/12);
//        glBindVertexArray(DelaunayVAO);
//        glDrawArrays(GL_LINE_STRIP,0,del.HowMany * 3);
//
        if(DelaunayOpen) {
            //将剖分三角缓冲画出来
            for (int i = 1; i < DelTraNumber; i++) {
                glBindVertexArray(DelTraVAOs[i]);
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
    glDeleteVertexArrays(DelTraNumber, DelTraVAOs);
    glDeleteBuffers(DelTraNumber, DelTraVBOs);

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
        DelaunayOpen = !DelaunayOpen;
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

