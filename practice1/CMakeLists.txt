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

# 查找指定目录下的所有vertexShader文件
file(GLOB_RECURSE vertexFile
     "${CMAKE_CURRENT_SOURCE_DIR}/source/transform/*.vs"
     "${CMAKE_CURRENT_SOURCE_DIR}/source/coordinate/*.vs"
     "${CMAKE_CURRENT_SOURCE_DIR}/source/camera/*.vs"
     )
source_group(shaderFile/vertexFile FILES ${vertexFile})

# 查找指定目录下的所有fragmentShader文件
file(GLOB_RECURSE fragmentFile
     "${CMAKE_CURRENT_SOURCE_DIR}/source/transform/*.fs"
     "${CMAKE_CURRENT_SOURCE_DIR}/source/coordinate/*.fs"
     "${CMAKE_CURRENT_SOURCE_DIR}/source/camera/*.fs"
     )
source_group(shaderFile/fragmentFile FILES ${fragmentFile})

# main.cpp不填加筛选器
source_group("" FILES main.cpp)

# 设置依赖项
link_directories("${PROJECT_BINARY_DIR}")
link_libraries(glad)

# 项目添加文件
add_executable(practice1
               ${headerfile}
               ${sourceFile}
               ${vertexFile}
               ${fragmentFile}
               main.cpp
              )
