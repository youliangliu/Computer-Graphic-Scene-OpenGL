//
//  main.cpp
//  cse167pa3
//
//  Created by Derek Wang on 10/28/19.
//  Copyright © 2019 Derek Wang. All rights reserved.
//

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "window.hpp"
#include <iostream>

void error_callback(int error, const char* description)
{
    // Print error.
    std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window)
{
    // Set the error callback.
    glfwSetErrorCallback(error_callback);
    // Set the key callback.
    glfwSetKeyCallback(window, Window::keyCallback);
    // Set the window resize callback.
    glfwSetWindowSizeCallback(window, Window::resizeCallback);
    // Set the scroll callback
    glfwSetScrollCallback(window, Window::scroll_callback);
    // Set the mouse key callback
    glfwSetMouseButtonCallback(window, Window::mouse_button_callback);
    // Set cursor position callback
    glfwSetCursorPosCallback(window, Window::cursor_position_callback);
}

void setup_opengl_settings()
{
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions()
{
    // Get info of GPU and supported OpenGL version.
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
    << std::endl;
    
    //If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << "Supported GLSL version is: " <<
    glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int main(void)
{
    // Create the GLFW window.
    GLFWwindow* window = Window::createWindow(1080, 980);
    if (!window) exit(EXIT_FAILURE);
    
    // Print OpenGL and GLSL versions.
    print_versions();
    // Setup callbacks.
    setup_callbacks(window);
    // Setup OpenGL settings.
    setup_opengl_settings();
    // Initialize the shader program; exit if initialization fails.
    if (!Window::initializeProgram()) exit(EXIT_FAILURE);
    // Initialize objects/pointers for rendering; exit if initialization fails.
    if (!Window::initializeObjects()) exit(EXIT_FAILURE);
    
    // Loop while GLFW window should stay open.
    while (!glfwWindowShouldClose(window))
    {
        // Main render display callback. Rendering of objects is done here.
        Window::displayCallback(window);
        
        // Idle callback. Updating objects, etc. can be done here.
        Window::idleCallback();
    }
    
    Window::cleanUp();
    // Destroy the window.
    glfwDestroyWindow(window);
    // Terminate GLFW.
    glfwTerminate();
    
    exit(EXIT_SUCCESS);
}

