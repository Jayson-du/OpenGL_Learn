#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// 定义相机移动的几个可能选项。用作抽象，以远离特定于窗口系统的输入方法
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.Of;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
    /*
    * @brief :  摄像机向量构造函数
    * @access:  public 
    * @param :  摄像机位置向量
    * @param :  摄像机上向量
    * @param :  摄像机偏航角
    * @param :  摄像机俯仰角
    * @data  :  2022/12/17
    */
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
           float yaw = YAW, float pitch = PITCH) 
        : Front(glm::vec3(0.0f, 0.0f, -1.0f))
        , MovementSpeed(SPEED)
        , MouseSensitivity(SENSITIVITY)
        , Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // constructor with scalar values
    /*
    * @brief :  摄像机位置构造函数
    * @access:  public 
    * @param :  摄像机世界空间中X坐标
    * @param :  摄像机世界空间中Y坐标
    * @param :  摄像机世界空间中Z坐标
    * @param :  摄像机世界空间上向量中X坐标
    * @param :  摄像机世界空间上向量中Y坐标
    * @param :  摄像机世界空间上向量中Z坐标
    * @param :  摄像机偏航角
    * @param :  摄像机俯仰角
    * @data  :  2022/12/17
    */
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
        : Front(glm::vec3(0.0f, 0.0f, -1.0f))
        , MovementSpeed(SPEED)
        , MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    /*
    * @brief :  获取观察矩阵
    * @access:  public 
    * @return:  glm::mat4
    * @data  :  2022/12/17
    */
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    /*
    * @brief :  处理键盘输入事件
    * @access:  public 
    * @param :  摄像机方向
    * @param :  float deltaTime
    * @return:  void
    * @data  :  2022/12/17
    */
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
        {
            Position += Front * velocity;
        }
        if (direction == BACKWARD)
        {
            Position -= Front * velocity;
        }
        if (direction == LEFT)
        {
            Position -= Right * velocity;
        }
        if (direction == RIGHT)
        {
            Position += Right * velocity;
        }
    }

    /*
    * @brief :  处理鼠标移动事件
    * @access:  public 
    * @param :  float xoffset
    * @param :  float yoffset
    * @param :  GLboolean constrainPitch
    * @return:  void
    * @data  :  2022/12/17
    */
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        // 计算偏移量, 并记录当前鼠标x, y坐标
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        if (constrainPitch)
        {
            // 将俯仰角设置在[-89.0, 89.0]之间
            if (Pitch > 89.0f)
            {
                Pitch = 89.0f;
            }
            if (Pitch < -89.0f)
            {
                Pitch = -89.0f;
            }
        }

        // 计算方向向量
        updateCameraVectors();
    }

    /*
    * @brief :  处理滚轮放大缩小事件
    * @access:  public 
    * @param :  float yoffset
    * @return:  void
    * @data  :  2022/12/17
    */
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
        {
            Zoom = 1.0f;
        }
        if (Zoom > 45.0f)
        {
            Zoom = 45.0f;
        }
    }

private:
    /*
    * @brief :  更新前向、右向、向上方向向量
    * @access:  private 
    * @return:  void
    * @data  :  2022/12/17
    */
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }

public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // 欧拉角
    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
};

#endif
