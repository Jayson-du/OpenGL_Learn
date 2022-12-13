#include "direct.h"

#include "shader_s.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void drawTransform()
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

    std::string vertexShader = std::string(_getcwd(NULL, 0)).append("\\source\\transform\\vertexTransformShader.vs");
    std::string fragmentShader = std::string(_getcwd(NULL, 0)).append("\\source\\transform\\fragmentTransformShader.fs");

    // Shader对象读取定点着色器程序与片段着色器程序
    Shader ourShader(vertexShader.c_str(), fragmentShader.c_str());

    // 设置顶点数据&&配置顶点属性
    float vertices[] = {
        // 位置               // 纹理
       0.5f,  0.5f, 0.0f,   1.0f, 1.0f,     // 0, 右上角
       0.5f, -0.5f, 0.0f,   1.0f, 0.0f,     // 1, 右下角
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,     // 2, 左下角
      -0.5f,  0.5f, 0.0f,   0.0f, 1.0f      // 3, 左上角
    };

    // 对应vertices中的0, 1, 2, 3
    unsigned int indices[] = {
        0, 1, 3,                            // 第一个三角形
        1, 2, 3                             // 第二个三角形
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 设置纹理属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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

#if defined DEBUG
    logPrint(std::cout, GREEN, "data's size is ", sizeof(data));
#endif

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

#if defined DEBUG
    logPrint(std::cout, GREEN, "data's size is ", sizeof(data));
#endif

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

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 为了使用两个纹理, 需要在渲染之前先绑定需要使用到的纹理
        // 帮点纹理单元之前需要先激活纹理单元
        glActiveTexture(GL_TEXTURE0);
        // 绑定texture1纹理到当前激活的纹理单元
        glBindTexture(GL_TEXTURE_2D, texture1);
        // 帮点纹理单元之前需要先激活纹理单元
        glActiveTexture(GL_TEXTURE1);
        // 绑定texture1纹理到当前激活的纹理单元
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // 激活着色器程序
        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // 绑定VAO
        glBindVertexArray(VAO);

        /*
         * @brief : 根据EBO中的indices与VBO中的vertices绘制图像;
         * @param : 绘制类型
         * @param : 绘制的点数, 因为绘制2个三角形, 所以点数为6
         * @param : 指定indices数组的数据类型
         * @param : indices的偏移量, 即从indices中的第几个数据开始
        */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and pool IO events (key pressed/released, mouse moved etc)
        // 会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
        glfwSwapBuffers(window);

        // 函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
        glfwPollEvents();
    }

    // 释放VAO, VBO, EBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // 释放GLFW申请的资源
    glfwTerminate();
}

// 窗口大小发生变化时, OpenGL执行这个回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 重新匹配视口大小
    glViewport(0, 0, width, height);
}

// 比较输入字符, 如果是"esc"则窗体退出
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}