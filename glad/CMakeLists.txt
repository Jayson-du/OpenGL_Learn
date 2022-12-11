# 设置头文件目录
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

# 通过CMake内置的file函数将.h文件与.cpp文件分离,
# ${CMAKE_CURRENT_SOURCE_DIR}为当前目录
# 递归查找当前项目include下的所有.h文件
file(GLOB_RECURSE HeaderFile
     "${CMAKE_CURRENT_SOURCE_DIR}/include/glad/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/*.h"
     )
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HeaderFile})

# 递归查找当前项目source目录下所有.cpp文件
file(GLOB_RECURSE SourceFile
     "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c"
     )
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SourceFile})

# 设置静态库输出路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# 库项目添加文件
add_library(glad
            ${HeaderFile}
            ${SourceFile}
            )
