#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "stb_image.h"
#include "shader_s.h"

/*
 * @brief : ����ˢ�»ص�����
 * @access: public static
 * @param : GLFWwindow * window
 * @param : int width
 * @param : int height
 * @return: void
 * @data  : 2022/12/12
*/
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/*
 * @brief : ��������
 * @access: public
 * @param : GLFWwindow * window
 * @return: void
 * @data  : 2022/12/10
*/
static void processInput(GLFWwindow* window);

/*
* @brief :  ��������¼��ص�����, ����Ŵ������С, ������Χ�� 1.0 ~ 45.0֮��
* @access:  public
* @param :  GLFWwindow * window
* @param :  ����
* @param :  yoffset��ʾ��ֱ�����Ĵ�С
* @return:  void
* @data  :  2022/12/15
*/
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

/*
* @brief :  ��������ƶ��ص�����
*           1������������һ֡��ƫ����
            2����ƫ������ӵ�������ĸ����Ǻ�ƫ������
            3����ƫ���Ǻ͸����ǽ���������Сֵ������
            4�����㷽������
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

// �����ϵͳ
glm::vec3 cameraPos     = glm::vec3(0.0f, 0.0f,  3.0f);               // �����λ��
glm::vec3 cameraFront   = glm::vec3(0.0f, 0.0f, -1.0f);               // ��������
glm::vec3 cameraUp      = glm::vec3(0.0f, 1.0f,  0.0f);               // ������

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

    // ע������ڴ����ƶ�ʱ����Ӧ����
    glfwSetCursorPosCallback(window, mouse_callback);

    // ע�������ֻ����������Ӧ����
    glfwSetScrollCallback(window, scroll_callback);

    // ����GLFW��Ӧ�����ع�꣬����׽��; ��׽����ʾ���Ӧ��ͣ���ڴ�����
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // GLAD����������OpenGL�ĺ���ָ��ģ��ڵ����κ�OpenGL�ĺ���֮ǰ��Ҫ�ȳ�ʼ��GLAD
    // ��GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ���, GLFW�����ǵ���glfwGetProcAddress��
    // GLAD���ݱ����ϵͳ��������ȷ�ĺ���
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // ��������������Ȼ������Ĺ��ܣ�Ҳ���ǣ����ͨ���ȽϺ����ֵ�����仯�ˣ�����и�����Ȼ������Ĳ�������������OpenGL�Ϳ��Ը�����Z���ϵ����أ���������ֻ�����Ǹ�����ǰ��û�ж���ʱ���Ż�滭������ء�
    // �����滭3Dʱ�������������������Ӿ�Ч���Ƚ���ʵ
    glEnable(GL_DEPTH_TEST);

    std::string vertexShader = std::string(_getcwd(NULL, 0)).append("\\source\\camera\\vertexCameraShader.vs");
    std::string fragmentShader = std::string(_getcwd(NULL, 0)).append("\\source\\camera\\fragmentCameraShader.fs");

    // Shader�����ȡ������ɫ��������Ƭ����ɫ������
    Shader ourShader(vertexShader.c_str(), fragmentShader.c_str());

    // ��ʼ����������
    float vertices[] =
    {
        // λ��               // ����
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

    // �������������������ϵ�²�ͬ��λ��
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
    // ����һ��VAO
    glGenVertexArrays(1, &VAO);
    // ����һ��VBO
    glGenBuffers(1, &VBO);
    // ��һ��VAO
    glBindVertexArray(VAO);

    // ��һ��VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // ��vertices�е����ݿ�������������
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ������ν�������λ������, ��GPUͨ������VAO, ȷ����ν���vertices�е�����
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // ��������0ָ����ͨ�ö�����������
    glEnableVertexAttribArray(0);

    // ������ν��������������, ��GPUͨ������VAO, ȷ����ν���vertices�е�����
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // ��������1ָ����ͨ�ö�����������
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

    // �ƻ�shader
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // ��Ⱦ
    while (!glfwWindowShouldClose(window))
    {
        // glfwGetTime: �����Դ����ʼ�������ڵ�����
        float currentFrame = static_cast<float>(glfwGetTime());
        // ����ÿһ֡��ʱ��
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // ��������ɫ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // �����ɫ && ��Ȼ���
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Ϊ��ʹ����������, ��Ҫ����Ⱦ֮ǰ�Ȱ���Ҫʹ�õ�������
        // �������Ԫ֮ǰ��Ҫ�ȼ�������Ԫ
        glActiveTexture(GL_TEXTURE0);
        // ��texture1������ǰ���������Ԫ
        glBindTexture(GL_TEXTURE_2D, texture1);
        // �������Ԫ֮ǰ��Ҫ�ȼ�������Ԫ
        glActiveTexture(GL_TEXTURE1);
        // ��texture1������ǰ���������Ԫ
        glBindTexture(GL_TEXTURE_2D, texture2);
        // ����Shader
        ourShader.use();

        // ����ͶӰ�����shader
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // LookAt������Ҫһ��λ�á�Ŀ���������
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        glBindVertexArray(VAO);

        // ����Ļ����ʾ10��������, ÿ�������忴��������һ����, �������������������λ�ü���ת�ǶȲ�ͬ; �������ͼ�β����Ѿ���cubePositions�������
        // ���Ե���Ⱦ���������ʱ�����ǲ���Ҫ�ı����ǵĻ����������������, 
        // **Ψһ��Ҫ����ֻ�Ǹı�ÿ�������ģ�;�������������任����������ϵ��**
        for (unsigned int i = 0; i < 10; i++)
        {
            // ����ÿ�������ģ�;��󣬲��ڻ�ͼǰ���䴫�ݸ���ɫ��
            // ȷ���Ƚ������ʼ��Ϊ��λ����
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);

            // ��ÿ��ģ�;�������һ����ת�Ƕ�
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            // ��shader�е�model����ģ�;���
            ourShader.setMat4("model", model);

            // ����
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and pool IO events (key pressed/released, mouse moved etc)
        // �ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ��
        glfwSwapBuffers(window);

        // ���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
        glfwPollEvents();
    }

    // �ͷ�VAO, VBO, EBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // �ͷ�GLFW�������Դ
    glfwTerminate();
}


void processInput(GLFWwindow* window)
{
    // �������"esc"�����˳�
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

// ���ڴ�С�����仯ʱ, OpenGLִ������ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ����ƥ���ӿڴ�С
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    // �ƶ�����, ���x, y����λ��
    float xpos = static_cast<float>(xposIn);    
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // ����ƫ����, ����¼��ǰ���x, y����
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;


    yaw += xoffset;                 // ����ƫ����
    pitch += yoffset;               // ���㸩����

    // ��������������[-89.0, 89.0]֮��
    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    // ���㷽������
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // yoffset��ʾ��ֱ�����Ĵ�С
    fov -= (float)yoffset;

    // ������Χ�� 1.0 ~ 45.0֮��
    if (fov < 1.0f)
    {
        fov = 1.0f;
    }
    if (fov > 45.0f)
    {
        fov = 45.0f;
    }
}