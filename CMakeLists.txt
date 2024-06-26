cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(CMakeSFMLProject src/main.cpp src/Map.cpp src/Update.cpp src/Handler.cpp src/Blocks.cpp src/AnimationManager.cpp src/Perlin.cpp src/Dynamics.cpp )




configure_file(${CMAKE_CURRENT_SOURCE_DIR}/images/Arial.ttf
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/images/textures-4.png
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/images/players/char_01.png
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)  

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/images/players/char_02.png
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY) 

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/images/phonk.png
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)  

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/images/phonk2.png
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)  

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/images/button.png
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)  

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/images/button1.png
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)  

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/images/phonk.png
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)  

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/images/Entities/CatWalk.png
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)  
target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics)
target_compile_features(CMakeSFMLProject PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET CMakeSFMLProject
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
        VERBATIM)
endif()

install(TARGETS CMakeSFMLProject)
