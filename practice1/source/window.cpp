#include "practice1.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/*
 * @brief :
 * @access:   public
 * @param :   GLFWwindow * window
 * @return:   void
 * @data  :   2022/12/10
*/
void processInput(GLFWwindow* window);

// setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void drawWindow()
{
    // glfw��ʼ������
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // �������汾�汾��
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                  // ���ôΰ汾�汾��
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // ����Ϊ����ģʽ(Core-profile)

#ifdef __APPLE__
    // ʹ��MacOS Xϵͳ, ��Ҫ���������д��뵽��ʼ����������Щ���ò���������
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // �������ڶ��� glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (NULL == window)
    {
        std::cout << "Falied to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    // ֪ͨGLFW����ǰ��������Ϊ������
    glfwMakeContextCurrent(window);

    // �ú�������ָ�����ڵ�framebuffer resize�Ĵ�С, ��ָ�����ڵ�framebuffer������Сʱ����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // GLAD����������OpenGL�ĺ���ָ��ģ��ڵ����κ�OpenGL�ĺ���֮ǰ��Ҫ�ȳ�ʼ��GLAD
    // ��GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ���, GLFW�����ǵ���glfwGetProcAddress��
    // GLAD���ݱ����ϵͳ��������ȷ�ĺ���
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // ��Ⱦѭ��(render loop), �жϵ�ǰ�����Ƿ��˳�
    while (!glfwWindowShouldClose(window))
    {
        // ��������
        processInput(window);

        // ��Ⱦ
        // ���������Ļ���õ���ɫ��������glClear�����������ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // �����Ļ����ɫ���壬������һ������λ(Buffer Bit)��ָ��Ҫ��յĻ��壬���ܵĻ���λ��GL_COLOR_BUFFER_BIT��GL_DEPTH_BUFFER_BIT��GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and pool IO events (key pressed/released, mouse moved etc)
        // �ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ��
        glfwSwapBuffers(window);

        // ���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
        glfwPollEvents();
    }

    // �ͷ�GLFW�������Դ
    glfwTerminate();
}

// ���ڴ�С�����仯ʱ, OpenGLִ������ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ����ƥ���ӿڴ�С
    glViewport(0, 0, width, height);
}

// �Ƚ������ַ�, �����"esc"�����˳�
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}