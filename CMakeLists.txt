cmake_minimum_required(VERSION 3.15)

project(VoxelThingEditorYeah VERSION "0.1.0" LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set(SRC
    src/main.cpp

    src/ui/mainwindow.cpp
    src/ui/mainwindow.hpp

    src/ui/docks/editor4pane.cpp
    src/ui/docks/editor4pane.hpp
    src/ui/docks/currentblock.cpp
    src/ui/docks/currentblock.hpp

    src/ui/widgets/renderwidget.cpp
    src/ui/widgets/renderwidget.hpp
    src/ui/widgets/colorpicker.cpp
    src/ui/widgets/colorpicker.hpp
    src/ui/widgets/betterslider.cpp
    src/ui/widgets/betterslider.hpp
    src/ui/widgets/blocktexture.cpp
    src/ui/widgets/blocktexture.hpp

    src/ui/dialogs/blockproperties.cpp
    src/ui/dialogs/blockproperties.hpp
    src/ui/dialogs/settingsdialog.cpp
    src/ui/dialogs/settingsdialog.hpp
    src/ui/dialogs/blocklist.cpp
    src/ui/dialogs/blocklist.hpp
    src/ui/dialogs/properties.cpp
    src/ui/dialogs/properties.hpp

    src/world/chunk.cpp
    src/world/chunk.hpp
    src/world/raycast.cpp
    src/world/raycast.hpp
    src/world/world.cpp
    src/world/world.hpp

    src/world/loading/worldformat.cpp
    src/world/loading/worldformat.hpp

    src/editor/tools.cpp
    src/editor/tools.hpp
    src/editor/toml.cpp
    src/editor/toml.hpp
    src/editor/defs.cpp
    src/editor/defs.hpp
    src/editor/editorstate.cpp
    src/editor/editorstate.hpp
    src/editor/undo.cpp
    src/editor/undo.hpp
    src/editor/entity.cpp
    src/editor/entity.hpp

    src/render/model.cpp
    src/render/model.hpp
    src/render/texture.cpp
    src/render/texture.hpp
)

set(RESOURCES resources/resources.qrc)

# Core, Widgets, Gui, and openGL
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui OpenGL)

# OpenGL itself
find_package(OpenGL REQUIRED)

# Configure config.hpp.in
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/config.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/config.h"
)

add_executable(${PROJECT_NAME} ${SRC} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::OpenGL OpenGL::GL OpenGL::GLU)
target_include_directories(${PROJECT_NAME} PRIVATE ${QT_INCLUDE}/QtWidgets ${QT_INCLUDE}/QtCore ${QT_INCLUDE}/QtGui ${QT_INCLUDE}/QtOpenGL src)
target_compile_definitions(${PROJECT_NAME} PRIVATE TOML_HEADER_ONLY)