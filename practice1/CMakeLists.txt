# 设置工程名
set (CURRENT_PROJECT_NAME practice1)

# 设置包含目录
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

# 查找指定目录下的所有头文件
file(GLOB_RECURSE headerfile
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
     )
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${headerfile})

# 查找指定目录下的所有源文件
file(GLOB_RECURSE sourceFile
     "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
     )
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sourceFile})

# 查找指定目录下的所有shader文件
file(GLOB_RECURSE shaderFile
     "${CMAKE_CURRENT_SOURCE_DIR}/source/transform/*.vs"
     "${CMAKE_CURRENT_SOURCE_DIR}/source/transform/*.fs"
     )
source_group(shaderFile FILES ${shaderFile})

# main.cpp不填加筛选器
source_group("" FILES main.cpp)

# 设置依赖项
link_directories("${PROJECT_BINARY_DIR}")
link_libraries(glad)

# 项目添加文件
add_executable(practice1
               ${headerfile}
               ${sourceFile}
               ${shaderFile}
               main.cpp
              )
