#ifndef PRACTICE1_H
#define PRACTICE1_H

#include <iostream>

#ifdef DIRECT
#include <direct.h>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <initializer_list>

#define LOGBUFFER      512

#define NONE                        "\033[m"
#define RED                         "\033[0;32;31m"
#define LIGHT_RED                   "\033[1;31m"
#define GREEN                       "\033[0;32;32m"
#define LIGHT_GREEN                 "\033[1;32m"
#define BLUE                        "\033[0;32;34m"
#define LIGHT_BLUE                  "\033[1;34m"
#define DARY_GRAY                   "\033[1;30m"
#define CYAN                        "\033[0;36m"
#define LIGHT_CYAN                  "\033[1;36m"
#define PURPLE                      "\033[0;35m"
#define LIGHT_PURPLE                "\033[1;35m"
#define BROWN                       "\033[0;33m"
#define YELLOW                      "\033[1;33m"
#define LIGHT_GRAY                  "\033[0;37m"
#define WHITE                       "\033[1;37m"

/*
 * @brief : 日志信息输出函数
 * @access: public 
 * @param : const char * pColor                     选择颜色
 * @param : std::initializer_list<std::string> var  期望输出的日志信息
 * @return: void
 * @data  : 2022/12/12
*/
template <typename T>
void print(std::ostream& os, const T& t) {
    os << t << " ";
}
template <typename... Args>
void logPrint(std::ostream& os, Args&&... args)
{
    std::cout << LIGHT_GREEN << "**";
    std::initializer_list<int>{(print(os, std::forward< Args>(args)), 0)...};
    std::cout << LIGHT_GREEN << "**";
    std::cout << NONE;
    std::cout << std::endl;
}

/*
* @brief :  画窗体
* @access:  public 
* @return:  void
* @data  :  2022/12/11
*/
void drawWindow();

/*
* @brief :  画三角形
* @access:  public 
* @return:  void
* @data  :  2022/12/11
*/
void drawTriangle();

/*
 * @brief : 动态三角行
 * @access: public 
 * @return: void
 * @data  : 2022/12/12
*/
void drawDynamicTriangle();

/*
 * @brief : 绘制纹理
 * @access: public 
 * @return: void
 * @data  : 2022/12/12
*/
void drawTexture();

/*
* @brief :  绘制坐标变换
* @access:  public 
* @return:  void
* @data  :  2022/12/13
*/
void drawCoordinate();

/*
* @brief :  摄像机
* @access:  public 
* @return:  void
* @data  :  2022/12/15
*/
void drawCamera();

#endif