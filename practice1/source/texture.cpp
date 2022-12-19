#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define DIRECT
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

// 设置窗体宽与高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void drawTexture()
{
    // glfw 初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                          // 设置主版本版本号 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                          // 设置次版本版本号
    // GLFW_OPENGL_PROFILE 选择要设置的模式, GLFW_OPENGL_CORE_PROFILE 设置核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);          // 设置为核心模式(Core-profile)

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建一个窗体对象
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (NULL == window)
    {
        std::cout << "Failed to craeate GLFW window" << std::endl;
        return;
    }
    // 通知GLFW将当前窗口设置为主窗口
    glfwMakeContextCurrent(window);
    // 绑定窗体发生变化时, 处理窗体变化的回调函数
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

    // ----构建顶点着色器----
    std::string textureVertextShader  = std::string(_getcwd(NULL, 0)).append("\\source\\textureVertextShader.vs");
    std::string textureFragmentShader = std::string(_getcwd(NULL, 0)).append("\\source\\textureFragmentShader.fs");

    // 构建并编译Shader程序
    Shader ourShader(textureVertextShader.c_str(), textureFragmentShader.c_str());

    // 初始化顶点属性
    float vertices[] = {
        // 位置               // 颜色             // 材质
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,    // 右上角
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,    // 右下角
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    // 左下角
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f     // 左上角
    };

    unsigned int indices[] = {
        0, 1, 3,                                                // 第一个三角形
        1, 2, 3                                                 // 第二个三角形
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

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 纹路属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 加载&&创建纹理
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    /*
     * @brief : 设置纹理的环绕方式
     * @param : 纹理目标
     * @param : 指定设置的选项与应用的纹理轴
     * @param : 纹理环绕方式
     */
    // 设置S(x)轴纹理, repeat模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // 设置T(y)轴纹理, repeat模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*
     * @brief : 设置纹理的过滤方式, 当进行放大(Magnify)和缩小(Minify)操作的时候设置纹理过滤的选项
     * @param : 纹理目标
     * @param : 方式
     * @param : 纹理过滤方式(4种)
     */
    // 设置纹理过滤器参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载图片, 创建纹理
    int width, height, nrChannels;
    std::string containerImg = std::string(_getcwd(NULL, 0)).append("\\..\\resource\\image\\container.jpg");
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
    }
    
    // 释放资源
    stbi_image_free(data);

    // 渲染
    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);

        // 渲染
        // 设置清空屏幕所用的颜色。当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清空屏幕的颜色缓冲，它接受一个缓冲位(Buffer Bit)来指定要清空的缓冲，可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);

        // 绑定纹理,
        // 本次在textureFragmentShader.fs中使用了全局的"uniform sampler2D texture1;"即纹理采样器
        // 因此需要在glDrawElements之前绑定纹理, 它会自动把纹理赋值给片段着色器的采样器
        glBindTexture(GL_TEXTURE_2D, texture);

        // 激活Shader
        ourShader.use();
        glBindVertexArray(VAO);
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
