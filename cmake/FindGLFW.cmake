# 在指定目录下寻找库文件
# [LibName]_FIND_COMPONENTS为上层传递的要查找的库名称集合, [LibName]为find_package中的第一个参数
foreach (COMP ${GLFW_FIND_COMPONENTS})
    find_library(GLFW_LIBRARY_DIR  ${COMP} PATHS ${GLFW_LIB_PATH})
endforeach()

# 如果找到了, 设置
if (GLFW_LIBRARY_DIR)
    set(GLFW_PACK_LIB TRUE)
endif()