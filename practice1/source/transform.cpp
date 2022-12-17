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

    std::string vertexShader = std::string(_getcwd(NULL, 0)).append("\\source\\transform\\vertexTransformShader.vs");
    std::string fragmentShader = std::string(_getcwd(NULL, 0)).append("\\source\\transform\\fragmentTransformShader.fs");

    // Shader�����ȡ������ɫ��������Ƭ����ɫ������
    Shader ourShader(vertexShader.c_str(), fragmentShader.c_str());

    // ���ö�������&&���ö�������
    float vertices[] = {
        // λ��               // ����
       0.5f,  0.5f, 0.0f,   1.0f, 1.0f,     // 0, ���Ͻ�
       0.5f, -0.5f, 0.0f,   1.0f, 0.0f,     // 1, ���½�
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,     // 2, ���½�
      -0.5f,  0.5f, 0.0f,   0.0f, 1.0f      // 3, ���Ͻ�
    };

    // ��Ӧvertices�е�0, 1, 2, 3
    unsigned int indices[] = {
        0, 1, 3,                            // ��һ��������
        1, 2, 3                             // �ڶ���������
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

    // ����λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ������������
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture1, texture2;
    // texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // ���������������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // �������������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // ����ͼƬ&&��������
    int width, height, nrChannels;
    // ���ü��ص�ͼƬ��y����ת
    stbi_set_flip_vertically_on_load(true);
    std::string containerImg = std::string(_getcwd(NULL, 0)).append("\\..\\resource\\image\\container.jpg");
    // ����ͼƬ�е�����
    unsigned char* data = stbi_load(containerImg.c_str(), &width, &height, &nrChannels, 0);

#if defined DEBUG
    logPrint(std::cout, GREEN, "data's size is ", sizeof(data));
#endif

    if (data)
    {
        // ��������
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        exit(1);
    }
    // �ͷ���Դ
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // ���������������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // �������������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ����ͼƬ&&��������
    std::string awesomefaceImg = std::string(_getcwd(NULL, 0)).append("\\..\\resource\\image\\awesomeface.png");
    data = stbi_load(awesomefaceImg.c_str(), &width, &height, &nrChannels, 0);

#if defined DEBUG
    logPrint(std::cout, GREEN, "data's size is ", sizeof(data));
#endif

    if (data)
    {
        // ��������
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        // Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        exit(1);
    }
    // �ͷ���Դ
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Ϊ��ʹ����������, ��Ҫ����Ⱦ֮ǰ�Ȱ���Ҫʹ�õ�������
        // �������Ԫ֮ǰ��Ҫ�ȼ�������Ԫ
        glActiveTexture(GL_TEXTURE0);
        // ��texture1������ǰ���������Ԫ
        glBindTexture(GL_TEXTURE_2D, texture1);
        // �������Ԫ֮ǰ��Ҫ�ȼ�������Ԫ
        glActiveTexture(GL_TEXTURE1);
        // ��texture1������ǰ���������Ԫ
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // ������ɫ������
        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // ��VAO
        glBindVertexArray(VAO);

        /*
         * @brief : ����EBO�е�indices��VBO�е�vertices����ͼ��;
         * @param : ��������
         * @param : ���Ƶĵ���, ��Ϊ����2��������, ���Ե���Ϊ6
         * @param : ָ��indices�������������
         * @param : indices��ƫ����, ����indices�еĵڼ������ݿ�ʼ
        */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and pool IO events (key pressed/released, mouse moved etc)
        // �ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ��
        glfwSwapBuffers(window);

        // ���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
        glfwPollEvents();
    }

    // �ͷ�VAO, VBO, EBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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