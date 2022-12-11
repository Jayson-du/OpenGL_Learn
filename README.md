[![C++](https://img.shields.io/badge/language-c++-red.svg)](https://en.cppreference.com/)
[![platform](https://img.shields.io/badge/platform-%20%7C%20windows-lightgrey.svg)](#%E9%A1%B9%E7%9B%AE%E7%9A%84%E4%B8%80%E4%BA%9B%E8%AE%BE%E8%AE%A1%E7%89%B9%E7%82%B9)


# OpenGL练习

Windows下练习[OpenGL](https://learnopengl-cn.github.io/intro/)

* practice1为[入门练习](https://learnopengl-cn.github.io/01%20Getting%20started/01%20OpenGL/)

#### 项目结构
```
├─cmake         配置查找GLFW静态库路径
├─glad          glad静态库
│  ├─include
│  └─source
├─practice1     基础练习1
│  ├─include
│  ├─source
│  └─main.cpp
└─resource      GLFW静态资源库, 外部编译成功后放入即可
    ├─include
    └─library
```