cmake_minimum_required(VERSION 3.15)

project(VoxelThingEditorYeah CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

set(SRC
    main.cpp

    widgets/mainwindow.cpp
    widgets/mainwindow.hpp
    widgets/editor4pane.cpp
    widgets/editor4pane.hpp
    widgets/renderwidget.cpp
    widgets/renderwidget.hpp

    world/chunk.cpp
    world/chunk.hpp
    world/raycast.cpp
    world/raycast.hpp

    editor/tools.cpp
    editor/tools.hpp
)

set(RESOURCES resources/resources.qrc)

# Core, Widgets, Gui, and openGL
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui OpenGL)

# OpenGL itself
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} ${SRC} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::OpenGL OpenGL::GL OpenGL::GLU)
target_include_directories(${PROJECT_NAME} PRIVATE ${QT_INCLUDE}/QtWidgets ${QT_INCLUDE}/QtCore ${QT_INCLUDE}/QtGui ${QT_INCLUDE}/QtOpenGL)