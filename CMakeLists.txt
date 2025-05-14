cmake_minimum_required(VERSION 3.16)

project(TianJin2048 VERSION 0.1 LANGUAGES CXX)

# 启用 CMake 自动处理 Qt 文件
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)  # 自动处理 .ui 文件
set(CMAKE_AUTOMOC ON)  # 自动处理 MOC 文件
set(CMAKE_AUTORCC ON)  # 自动处理 .qrc 文件

# 查找 Qt6 或 Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 项目源代码文件
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources.qrc
)

# 创建可执行文件并链接 Qt 库
qt_add_executable(TianJin2048
    ${PROJECT_SOURCES}
)

# 使用关键字方式链接 Qt 库
target_link_libraries(TianJin2048 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

# 如果使用 Qt 6，则调用 qt_finalize_executable
if(${QT_VERSION_MAJOR} EQUAL 6)
    qt_finalize_executable(TianJin2048)
endif()
