#include "shader_s.h"

#include <direct.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

void drawDynamicTriangle()
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

    // glad: load all OpenGL function pointers
    // GLAD是用来管理OpenGL的函数指针的，在调用任何OpenGL的函数之前需要先初始化GLAD
    // 给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数, GLFW给我们的是glfwGetProcAddress，
    // GLAD根据编译的系统定义了正确的函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    std::string vertexShader = std::string(_getcwd(NULL, 0)).append("\\source\\vertexShader.vs");
    std::string fragmentShader = std::string(_getcwd(NULL, 0)).append("\\source\\fragmentShader.fs");

    // Shader对象读取定点着色器程序与片段着色器程序
    Shader ourShader(vertexShader.c_str(), fragmentShader.c_str());

    // 数据
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 1、创建VAO;
    glBindVertexArray(VAO);

    // 2、创建VBO;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 3、用VBO绑定一块缓冲区, 并将vertices中的数据写入缓冲区中
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 4、设置GPU解析位置数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // 使能位置解析
    glEnableVertexAttribArray(0);

    // 5、设置GPU解析颜色数据
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // 使能颜色解析
    glEnableVertexAttribArray(1);

    // 渲染
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // 渲染
        // 设置清空屏幕所用的颜色。当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清空屏幕的颜色缓冲，它接受一个缓冲位(Buffer Bit)来指定要清空的缓冲，可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);


        // 激活Shader程序对象
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

