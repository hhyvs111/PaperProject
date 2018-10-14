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
#include <cstring>
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
bool faultMove = false;
//试一下这个是否有用啊。

//顶点
VERTEX *fault1_up, *fault1_down;
VERTEX *fault2_up, *fault2_down;

//全局变量，用来平移之类的
float fault1[2][30] = {
        {
                0.5f, 0.15f, -0.5f + 2.0f,
                0.41f, 0.29f, -0.5f + 2.0f,
                0.3f, 0.32f, -0.5f + 2.0f,
                0.18f, 0.26f, -0.5f + 2.0f,
                0.0f, 0.24f, -0.5f + 2.0f,
                -0.13f, 0.29f, -0.5f + 2.0f,
                -0.21f, 0.25f, -0.5f + 2.0f,
                -0.26f, 0.29f, -0.5f + 2.0f,
                -0.38f, 0.25f, -0.5f + 2.0f,
                -0.5f, 0.22f, -0.5f + 2.0f,
        }
        //这个点是尼玛外面的点吧
//            -0.5f, 0.5f, -0.5f+ 2.0f,
//            -0.5f, 0.22f,  -0.5f+ 2.0f,
        //怎么这个点重复了，有毒。
        //x下层
        ,
        {
                -0.5f, -0.24f, -0.5f + 2.0f,
                -0.41f, -0.23f, -0.5f + 2.0f,
                -0.29f, -0.23f, -0.5f + 2.0f,
                -0.23f, -0.29f, -0.5f + 2.0f,
                -0.05f, -0.32f, -0.5f + 2.0f,
                0.08f, -0.31f, -0.5f + 2.0f,
                0.19f, -0.25f, -0.5f + 2.0f,
                0.29f, -0.3f, -0.5f + 2.0f,
                0.4f, -0.26f, -0.5f + 2.0f,
                0.5f, -0.29f, -0.5f + 2.0f,
        }
//            0.5f, 0.4f,   -0.5f+ 2.0f,
};




//第二个平面的断层
//这种断层数据不要一样，主要是这个z坐标的区别。
float fault2[2][30] = {
        {0.5f, 0.15f,  -0.5f ,
                0.41f, 0.29f,  -0.5f ,
                0.3f, 0.32f,  -0.5f ,
                0.18f, 0.26f,  -0.5f ,
                0.0f, 0.24f,  -0.5f ,
                -0.13f, 0.29f,  -0.5f ,
                -0.20f, 0.20f, -0.5f,
                -0.26f, 0.29f,  -0.5f ,
                -0.38f, 0.25f,  -0.5f ,
                -0.5f, 0.22f,  -0.5f}
        //这个点是尼玛外面的点吧
//            -0.5f, 0.5f, -0.5f ,
//            -0.5f, 0.22f,  -0.5f ,
        //怎么这个点重复了，有毒。
        //x下层
        ,
        {-0.5f, -0.24f,  -0.5f ,
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


enum moveDirection {
        xD,
        yD,
        zD,
        xyD,
        xzD,
        yzD,
        xyzD
};

//函数声明
void faultMoveFunction(VERTEX *vertex, int num, float moveSize, int whichDirection);
//将两个顶点数组合并为1个
VERTEX * faultMerge(VERTEX vertex1[],int num1, VERTEX vertex2[], int num2);

//初始化
void drawInit(unsigned int & VAO, unsigned int & VBO, VERTEX *target, int num);

//两个转换的函数
float * VertexToFloat(VERTEX vertex[], int num);

VERTEX * FloatToVertex(float _float[], int num);

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
    
//    for (int i = 0, j1 = 0, j2 = 0;i < 20; i++)
//    {
//        if(i < 10)
//        {
//            fault_up[i].x = fault[0][j1++];
//            fault_up[i].y = fault[0][j1++];
//            fault_up[i].z = fault[0][j1++];
//            //这里的j++重复了
//            fault1_up[i].x = fault1[0][j2++];
//            fault1_up[i].y = fault1[0][j2++];
//            fault1_up[i].z = fault1[0][j2++];
//            if(i == 9)
//                j1 = j2 = 0;
//        }
//        else
//            //这个i都越界了尼玛
//        {
//            fault_down[i].x = fault[1][j1++];
//            cout<<"the tran"<<fault_down[i].x;
//            fault_down[i].y = fault[1][j1++];
//            fault_down[i].z = fault[1][j1++];
//            fault1_down[i].x = fault1[1][j2++];
//            fault1_down[i].y = fault1[1][j2++];
//            fault1_down[i].z = fault1[1][j2++];
//
//        }
//
//    }



    float cube1[] = {
            -0.5f, 0.5f,  -0.5f,  0.0f, 0.0f,
            0.5f, 0.5f,  -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            -0.5f,-0.5f,  -0.5f,  0.0f, 0.0f,
    };

    float cube22[] = {
            -0.5f, 0.5f,  -0.5f + 2.0f,  0.0f, 0.0f,
            0.5f, 0.5f,  -0.5f+ 2.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f+ 2.0f,  0.0f, 0.0f,
            -0.5f,-0.5f,  -0.5f+ 2.0f,  0.0f, 0.0f,
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
    
//    glGenVertexArrays(1, &faultVAO);
//    glGenBuffers(1, &faultVBO);
//
//    glBindVertexArray(faultVAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, faultVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(fault_up), fault_up, GL_STATIC_DRAW);
//
//    // position attribute
//    //这里的步长为3，之前的是5因为有纹理坐标
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);

//    unsigned int fault1VBO, fault1VAO;
//    glGenVertexArrays(1, &fault1VAO);
//    glGenBuffers(1, &fault1VBO);
//
//    glBindVertexArray(fault1VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, fault1VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(fault1), fault1, GL_STATIC_DRAW);
//
//    // position attribute
//    //这里的步长为3，之前的是5因为有纹理坐标
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
    

   //falut为断层，将其传入三角剖分的构造函数里去。
   //现在改为这个传入顶点了，那么还是要做一个顶点集合，就是两个断层合入为一个断层。



    Delaunay del((faultMerge(fault1_up, 10, fault1_up, 10)), 20);
//    Delaunay del(fault,60);
    cout <<"the size of :" << (sizeof(fault1)/4)<< endl;

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

        //两条线，数据还是要改一下
        glBindVertexArray(fault1_upVAO);
        //这里算不出结构体指针所指向的大小，只能用之前的数组代替了。
        glDrawArrays(GL_LINE_STRIP,0 , sizeof(fault1[0]) / 12);

        glBindVertexArray(fault1_downVAO);
        glDrawArrays(GL_LINE_STRIP,0 , (sizeof(fault1[1])) / 12);



        if(faultMove)
        {
            //重新载入一下，加入缓冲
            drawInit(fault2_upVAO, fault2_upVBO, fault2_up, sizeof(fault2[0]) / 12);
        }
        glBindVertexArray(fault2_upVAO);
        //这里算不出结构体指针所指向的大小，只能用之前的数组代替了。
        glDrawArrays(GL_LINE_STRIP,0 , sizeof(fault2[0]) / 12);

        glBindVertexArray(fault2_downVAO);
        glDrawArrays(GL_LINE_STRIP,0 , (sizeof(fault2[1])) / 12);
//
//        glBindVertexArray(fault1VAO);
//        glDrawArrays(GL_LINE_STRIP,0 , (sizeof(fault1))/12);
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
        cout<<"delaunay"<<endl;
        DelaunayOpen = !DelaunayOpen;
    }
    //平移操作，如果按下则进行平移
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        cout<<"move"<<endl;
       //主要是移动一下z轴，先试试。
       if(!faultMove)
       {
           faultMoveFunction(fault2_up, 10, 2.0f, zD);
           faultMoveFunction(fault2_up, 10, -0.25f, yD);
       }
        faultMove = true;
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

//判断直线是否相交
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
//
bool faultIntersect(VERTEX fault1[], int f1Number, VERTEX fault2[], int f2Number)
{
    //断层里的线是依次的，基本上第一个点的ip是基本在同一起点，那么还是要判断多重啊。不能单一的判断，那么就是一个线段是否与另一条的所有线段相交
    //如果相交则要平移一下，但是这样感觉效率好低啊，不管了，先这样做吧。应该计算量不大，这个东西的话是一个基本的算法，时间复杂度应该不高。
    for(int i = 0; i < f1Number - 1; i++)
    {
        for(int j = 0; j < f2Number - 1; j++)
        {
            //如果有任意一条相交，那么该线就需要平移。
            if(sideIntersectSide(fault1[i],fault1[i+1], fault2[j], fault2[j+1]))
                return false;
        }
    }
    return true;
}

//平移，哪个断层，平移多少,方向是什么？
//到底是传入vertex还是数组啊？感觉好像
void faultMoveFunction(VERTEX *vertex, int num, float moveSize, int whichDirection)
{
    cout<<"movefunction"<<endl;
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
        for(int i = 1;i < num; i++)
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

VERTEX * faultMerge(VERTEX vertex1[],int num1, VERTEX vertex2[], int num2)
{
    VERTEX *merge = new VERTEX[num1 + num2];
    //不用考虑排序就这样插入吧。
    for(int i = 0;i < num1;i++)
    {
        merge[i] = vertex1[i];
    }

    for(int i = num1,j = 0;j < num2;j++)
    {
        merge[i++] = vertex2[j];
    }
    return merge;
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
        cout<<source[i].x<<" ";
        cout<<source[i].y<<" ";
        cout<<source[i].z<<endl;
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

    }
    return vertex;
}