# 设置工程名
set (CURRENT_PROJECT_NAME practice1)

# 设置包含目录
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../glfw/include/"
)

# 设置链接目录
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/${CMAKE_BUILD_TYPE}/")

# 添加依赖库名称
link_libraries(glfw3)

# 查找指定目录下的所有头文件
file(GLOB_RECURSE Headerfile
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
     )
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Headerfile})

# 查找指定目录下的所有源文件
file(GLOB_RECURSE SourceFile
     "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
     )
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SourceFile})

add_executable(practice1
               ${Headerfile}
               ${SourceFile}
               main.cpp
              )