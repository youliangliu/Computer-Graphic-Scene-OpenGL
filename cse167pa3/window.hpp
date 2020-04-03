//
//  window.hpp
//  cse167pa3
//
//  Created by Derek Wang on 10/30/19.
//  Copyright © 2019 Derek Wang. All rights reserved.
//

#ifndef window_hpp
#define window_hpp

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <math.h>

#include "shader.hpp"
#include "geometry.hpp"
#include "transform.hpp"
#include "node.hpp"
#include "skyBox.hpp"
#include "Stage.hpp"
#include "Box.hpp"
#include "cloud.hpp"
#include "NewBoundingBox.h"
class Window
{
public:
    static int width;
    static int height;
    static int ShadowSwitch, ToonSwitch;
    static bool Quad_render;
    static GLboolean showSphere, doCulling, demoMode;
    static const char* windowTitle;
    static glm::mat4 projection;
    static glm::mat4 view;
    static glm::mat4 lightProjection, lightView, lightSpaceMatrix;
    static glm::vec3 eye, center, up, lightPos, front, right, lightColor;
    static glm::vec3 lastPoint;    // for model track ball
    static glm::vec3 currentPos;    // for sphere translation
    static GLuint skyBoxShader, sphereShader, projectionLoc, viewLoc, geoShader;
    static GLuint sphereViewPosLoc, sphereProjLoc, sphereViewLoc;
    static GLuint trackShader, trackProjLoc, trackViewLoc;
    static GLuint defaultShader, depthShader, debugShader, toonShader, cloudShader;
    static SkyBox* skyBox;
    static Stage* stage;
    static Box* box;
    static Cloud* cloud;
    static Transform* sphereTransfrom;
    static GLdouble prevTime, frameTime;
    static GLdouble displacement;
    static GLdouble fov;
    static bool initializeProgram();
    static bool initializeObjects();
    static void cleanUp();
    static GLFWwindow* createWindow(int width, int height);
    static void resizeCallback(GLFWwindow* window, int width, int height);
    static void idleCallback();
    static void displayCallback(GLFWwindow*);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static glm::vec3 trackBallMapping(GLFWwindow* window, double xpos, double ypos);
    static const unsigned int SHADOW_WIDTH, SHADOW_HEIGHT;
    static unsigned int depthMapFBO;
    static unsigned int depthMap,quadVAO,quadVBO;



	////////////////////////
	static GLuint bb_program, bb_projectionLoc, bb_viewLoc;
	static NewBoundingBox* bb_box1;
	static NewBoundingBox* bb_camera;
	static NewBoundingBox* bb_box2;
	static NewBoundingBox* bb_box3;
	static NewBoundingBox* bb_box4;
	static NewBoundingBox* bb_box5;
	static Geometry* car;


};

#endif /* window_hpp */
