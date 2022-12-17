#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "stb_image.h"
#include "shader_s.h"

/*
 * @brief : 缓冲刷新回调函数
 * @access: public static
 * @param : GLFWwindow * window
 * @param : int width
 * @param : int height
 * @return: void
 * @data  : 2022/12/12
*/
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/*
 * @brief : 处理输入
 * @access: public
 * @param : GLFWwindow * window
 * @return: void
 * @data  : 2022/12/10
*/
static void processInput(GLFWwindow* window);

/*
* @brief :  处理滚轮事件回调函数, 处理放大或者缩小, 滚动范围在 1.0 ~ 45.0之间
* @access:  public
* @param :  GLFWwindow * window
* @param :  无用
* @param :  yoffset表示竖直滚动的大小
* @return:  void
* @data  :  2022/12/15
*/
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

/*
* @brief :  处理鼠标移动回调函数
*           1、计算鼠标距上一帧的偏移量
            2、把偏移量添加到摄像机的俯仰角和偏航角中
            3、对偏航角和俯仰角进行最大和最小值的限制
            4、计算方向向量
* @access:  public
* @param :  GLFWwindow * window
* @param :  double xpos
* @param :  double ypos
* @return:  void
* @data  :  2022/12/15
*/
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

static const unsigned int SCR_WIDTH = 800;
static const unsigned int SCR_HEIGHT = 600;

// 摄像机系统
glm::vec3 cameraPos     = glm::vec3(0.0f, 0.0f,  3.0f);               // 摄像机位置
glm::vec3 cameraFront   = glm::vec3(0.0f, 0.0f, -1.0f);               // 方向向量
glm::vec3 cameraUp      = glm::vec3(0.0f, 1.0f,  0.0f);               // 上向量

bool  firstMouse        =  true;
float yaw               = -90.0f;
float pitch             =  0.0f;
float lastX             =  800.0f / 2.0;
float lastY             =  600.0f / 2.0;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void drawCamera()
{
    // glfw初始化设置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // 设置主版本版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                  // 设置次版本版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 设置为核心模式(Core-profile)

#ifdef __APPLE__
    // 使用MacOS X系统, 需要加下面这行代码到初始化代码中这些配置才能起作用
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建窗口对象 glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (NULL == window)
    {
        std::cout << "Falied to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    // 通知GLFW将当前窗口设置为主窗口
    glfwMakeContextCurrent(window);

    // 该函数设置指定窗口的framebuffer resize的大小, 当指定窗口的framebuffer调整大小时调用
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 注册鼠标在窗体移动时的响应函数
    glfwSetCursorPosCallback(window, mouse_callback);

    // 注册鼠标滚轮滑动窗体的响应函数
    glfwSetScrollCallback(window, scroll_callback);

    // 告诉GLFW，应该隐藏光标，并捕捉它; 捕捉光标表示光标应该停留在窗口中
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // GLAD是用来管理OpenGL的函数指针的，在调用任何OpenGL的函数之前需要先初始化GLAD
    // 给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数, GLFW给我们的是glfwGetProcAddress，
    // GLAD根据编译的系统定义了正确的函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // 用来开启更新深度缓冲区的功能，也就是，如果通过比较后深度值发生变化了，会进行更新深度缓冲区的操作。启动它，OpenGL就可以跟踪再Z轴上的像素，这样，它只会再那个像素前方没有东西时，才会绘画这个像素。
    // 在做绘画3D时，这个功能最好启动，视觉效果比较真实
    glEnable(GL_DEPTH_TEST);

    std::string vertexShader = std::string(_getcwd(NULL, 0)).append("\\source\\camera\\vertexCameraShader.vs");
    std::string fragmentShader = std::string(_getcwd(NULL, 0)).append("\\source\\camera\\fragmentCameraShader.fs");

    // Shader对象读取定点着色器程序与片段着色器程序
    Shader ourShader(vertexShader.c_str(), fragmentShader.c_str());

    // 初始化定点数据
    float vertices[] =
    {
        // 位置               // 材质
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // 多个立方体在世界坐标系下不同的位置
    glm::vec3 cubePositions[] = 
    {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO, VAO;
    // 创建一个VAO
    glGenVertexArrays(1, &VAO);
    // 创建一个VBO
    glGenBuffers(1, &VBO);
    // 绑定一个VAO
    glBindVertexArray(VAO);

    // 绑定一个VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 将vertices中的数据拷贝到缓冲区中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 设置如何解析顶点位置数据, 即GPU通过设置VAO, 确定如何解析vertices中的数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // 启用索引0指定的通用顶点属性数组
    glEnableVertexAttribArray(0);

    // 设置如何解析顶点材质数据, 即GPU通过设置VAO, 确定如何解析vertices中的数据
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // 启用索引1指定的通用顶点属性数组
    glEnableVertexAttribArray(1);

    unsigned int texture1, texture2;
    // texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 设置纹理轴向参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置纹理过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载图片&&创建纹理
    int width, height, nrChannels;
    // 设置加载的图片在y轴旋转
    stbi_set_flip_vertically_on_load(true);
    std::string containerImg = std::string(_getcwd(NULL, 0)).append("\\..\\resource\\image\\container.jpg");
    // 加载图片中的数据
    unsigned char* data = stbi_load(containerImg.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        // 生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // 为当前绑定的纹理自动生成所有需要的多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        exit(1);
    }
    // 释放资源
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // 设置纹理轴向参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置纹理过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载图片&&创建纹理
    std::string awesomefaceImg = std::string(_getcwd(NULL, 0)).append("\\..\\resource\\image\\awesomeface.png");
    data = stbi_load(awesomefaceImg.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        // 生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        // 为当前绑定的纹理自动生成所有需要的多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        exit(1);
    }
    // 释放资源
    stbi_image_free(data);

    // 计划shader
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // 渲染
    while (!glfwWindowShouldClose(window))
    {
        // glfwGetTime: 返回自窗体初始化到现在的秒数
        float currentFrame = static_cast<float>(glfwGetTime());
        // 计算每一帧的时间
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // 清屏的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清除颜色 && 深度缓存
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 为了使用两个纹理, 需要在渲染之前先绑定需要使用到的纹理
        // 帮点纹理单元之前需要先激活纹理单元
        glActiveTexture(GL_TEXTURE0);
        // 绑定texture1纹理到当前激活的纹理单元
        glBindTexture(GL_TEXTURE_2D, texture1);
        // 帮点纹理单元之前需要先激活纹理单元
        glActiveTexture(GL_TEXTURE1);
        // 绑定texture1纹理到当前激活的纹理单元
        glBindTexture(GL_TEXTURE_2D, texture2);
        // 激活Shader
        ourShader.use();

        // 传递投影矩阵给shader
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // LookAt矩阵需要一个位置、目标和上向量
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        glBindVertexArray(VAO);

        // 在屏幕中显示10个立方体, 每个立方体看起来都是一样的, 区别在于它们在世界的位置及旋转角度不同; 立方体的图形布局已经在cubePositions定义好了
        // 所以当渲染更多物体的时候我们不需要改变我们的缓冲数组和属性数组, 
        // **唯一需要做的只是改变每个对象的模型矩阵来将立方体变换到世界坐标系中**
        for (unsigned int i = 0; i < 10; i++)
        {
            // 计算每个对象的模型矩阵，并在绘图前将其传递给着色器
            // 确保先将矩阵初始化为单位矩阵
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);

            // 给每个模型矩阵增加一点旋转角度
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            // 给shader中的model设置模型矩阵
            ourShader.setMat4("model", model);

            // 绘制
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and pool IO events (key pressed/released, mouse moved etc)
        // 会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
        glfwSwapBuffers(window);

        // 函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
        glfwPollEvents();
    }

    // 释放VAO, VBO, EBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // 释放GLFW申请的资源
    glfwTerminate();
}


void processInput(GLFWwindow* window)
{
    // 如果输入"esc"则窗体退出
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

// 窗口大小发生变化时, OpenGL执行这个回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 重新匹配视口大小
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    // 移动鼠标后, 鼠标x, y方向位移
    float xpos = static_cast<float>(xposIn);    
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // 计算偏移量, 并记录当前鼠标x, y坐标
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;


    yaw += xoffset;                 // 计算偏航角
    pitch += yoffset;               // 计算俯仰角

    // 将俯仰角设置在[-89.0, 89.0]之间
    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    // 计算方向向量
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // yoffset表示竖直滚动的大小
    fov -= (float)yoffset;

    // 滚动范围在 1.0 ~ 45.0之间
    if (fov < 1.0f)
    {
        fov = 1.0f;
    }
    if (fov > 45.0f)
    {
        fov = 45.0f;
    }
}