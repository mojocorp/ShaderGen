cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
find_package(OpenGL REQUIRED)

set(HEADERS
    globals.h
    SGFrame.h
    SGFixedGLState.h
    SGModels.h
    SGOglNotebook.h
    SGSurfaces.h
    SGShaderGenerator.h
    SGShaderTextWindow.h
    SGCanvasMouseHandler.h
    SGCanvas.h
    SGTextures.h
    SGOglFogNBPage.h
    SGOglMaterialNBPage.h
    SGOglLightNBPage.h
    SGOglTextureCoordNBPage.h
    SGOglTextureEnvNBPage.h
    QColorButton.h
    QVectorEdit.h
    QCodeEditor.h
)

set(SOURCES
    SGFrame.cpp
    SGFixedGLState.cpp
    SGModels.cpp
    SGOglNotebook.cpp
    SGSurfaces.cpp
    SGShaderGenerator.cpp
    SGCanvas.cpp
    SGCanvasMouseHandler.cpp
    SGShaderTextWindow.cpp
    SGOglFogNBPage.cpp
    SGOglMaterialNBPage.cpp
    SGTextures.cpp
    SGOglLightNBPage.cpp
    SGOglTextureCoordNBPage.cpp
    SGOglTextureEnvNBPage.cpp
    QColorButton.cpp
    QVectorEdit.cpp
    QCodeEditor.cpp
    main.cpp
)

set(RESOURCES
    textures1.qrc
    textures2.qrc
)

if (APPLE)
    set(RESOURCES ${RESOURCES} ShaderGen.icns)
    set_source_files_properties(ShaderGen.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif (APPLE)

if (WIN32)
    set(RESOURCES ${RESOURCES} info.rc)

    # Remove weird Debug/Release subdirectories
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR})
endif (WIN32)

add_executable (ShaderGen WIN32 MACOSX_BUNDLE ${HEADERS} ${SOURCES} ${RESOURCES})

target_link_libraries(ShaderGen
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
)

# Install
if (APPLE)
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${SHADERGEN_VERSION})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${SHADERGEN_VERSION})
    set_target_properties(ShaderGen PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
    install(TARGETS ShaderGen BUNDLE DESTINATION ./)
elseif (WIN32)
    set_target_properties(ShaderGen PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
    install(TARGETS ShaderGen RUNTIME DESTINATION bin)
else()
    # Assume linux
    install(TARGETS ShaderGen DESTINATION bin)
endif ()

# Automatically deploy qt
get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QT5_BIN_DIR ${QT5_QMAKE_EXECUTABLE} PATH)
if (APPLE)
    find_program(QT5_DEPLOYQT_EXECUTABLE macdeployqt HINTS "${QT5_BIN_DIR}")
    install(CODE "EXECUTE_PROCESS(COMMAND ${QT5_DEPLOYQT_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/ShaderGen.app)")

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ShaderGen.app/Contents/Frameworks DESTINATION ./ShaderGen.app/Contents)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ShaderGen.app/Contents/PlugIns DESTINATION ./ShaderGen.app/Contents)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ShaderGen.app/Contents/Resources/qt.conf DESTINATION ./ShaderGen.app/Contents/Resources)
elseif (WIN32)
    find_program(QT5_DEPLOYQT_EXECUTABLE windeployqt HINTS "${QT5_BIN_DIR}")
    install(CODE "EXECUTE_PROCESS(COMMAND ${QT5_DEPLOYQT_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/ShaderGen.exe)")

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Qt5Core.dll DESTINATION bin)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Qt5Gui.dll DESTINATION bin)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Qt5Widgets.dll DESTINATION bin)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/platforms DESTINATION bin)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/styles DESTINATION bin)
    include(InstallRequiredSystemLibraries)
else ()
    # Assume linux
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ShaderGen.png DESTINATION bin)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ShaderGen.desktop DESTINATION bin)
endif ()

# Package builder
set(CPACK_PACKAGE_NAME "ShaderGen")
set(CPACK_PACKAGE_VENDOR "mojocorp")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "GLSL ShaderGen")
set(CPACK_PACKAGE_VERSION ${SHADERGEN_VERSION})
set(CPACK_PACKAGE_EXECUTABLES "ShaderGen" "ShaderGen")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_PROJECT_NAME}")
set(CPACK_NSIS_MODIFY_PATH OFF)
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/License.txt")

include(CPack)
