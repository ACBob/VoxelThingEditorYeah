cmake_minimum_required(VERSION 3.15)

project(VoxelThingEditorYeah CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set(SRC
    src/main.cpp

    src/widgets/mainwindow.cpp
    src/widgets/mainwindow.hpp
    src/widgets/editor4pane.cpp
    src/widgets/editor4pane.hpp
    src/widgets/renderwidget.cpp
    src/widgets/renderwidget.hpp
    src/widgets/settingsdialog.cpp
    src/widgets/settingsdialog.hpp
    src/widgets/colorpicker.cpp
    src/widgets/colorpicker.hpp
    src/widgets/blockproperties.cpp
    src/widgets/blockproperties.hpp

    src/world/chunk.cpp
    src/world/chunk.hpp
    src/world/raycast.cpp
    src/world/raycast.hpp

    src/editor/tools.cpp
    src/editor/tools.hpp
    src/editor/toml.cpp
    src/editor/toml.hpp
    src/editor/blockdefs.cpp
    src/editor/blockdefs.hpp

    src/render/model.cpp
    src/render/model.hpp
)

set(RESOURCES resources/resources.qrc)

# Core, Widgets, Gui, and openGL
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui OpenGL)

# OpenGL itself
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} ${SRC} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::OpenGL OpenGL::GL OpenGL::GLU)
target_include_directories(${PROJECT_NAME} PRIVATE ${QT_INCLUDE}/QtWidgets ${QT_INCLUDE}/QtCore ${QT_INCLUDE}/QtGui ${QT_INCLUDE}/QtOpenGL)
target_compile_definitions(${PROJECT_NAME} PRIVATE TOML_HEADER_ONLY)