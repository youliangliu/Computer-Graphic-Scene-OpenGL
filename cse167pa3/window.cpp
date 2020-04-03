//
//  window.cpp
//  cse167pa3
//
//  Created by Derek Wang on 10/30/19.
//  Copyright © 2019 Derek Wang. All rights reserved.
//

#include "window.hpp"

// Initializing Static Parameters
bool sky = false;
int Window::width;
int Window::height;
int buttonPressed = 0;
GLboolean Window::showSphere = false;
GLboolean Window::doCulling = true;
GLboolean Window::demoMode = false;
const char* Window::windowTitle = "GLFW Starter Project";
glm::mat4 Window::projection; // Projection matrix.
glm::mat4 Window::lightView;
glm::mat4 Window::lightProjection;
glm::mat4 Window::lightSpaceMatrix;
GLdouble Window::fov = 70.0;
glm::vec3 Window::eye(0, 0.7, 5); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.
glm::vec3 Window::lightPos(-2.0f, 4.0f, -1.0f);
glm::vec3 Window::lightColor(0.3f,0.3f,0.3f);
glm::vec3 Window::lastPoint(0, 0, 0);
glm::vec3 Window::front(0, 0, -1);
glm::vec3 world_up(0,1,0);
glm::vec3 Window::right;
glm::mat4 Window::view;
GLuint Window::skyBoxShader;
GLuint Window::sphereShader;
GLuint Window::trackShader;
GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::sphereViewPosLoc;
GLuint Window::sphereProjLoc;
GLuint Window::sphereViewLoc;
GLuint Window::trackProjLoc;
GLuint Window::trackViewLoc;
GLuint Window::defaultShader;
GLuint Window::depthShader;
GLuint Window::debugShader;
GLuint Window::toonShader;
GLuint Window::cloudShader;
GLuint Window::geoShader;
bool Window::Quad_render = false;
Stage* Window::stage;
SkyBox* Window::skyBox;
Box* Window::box;
Cloud* Window::cloud;
Transform* Window::sphereTransfrom;
GLdouble Window::prevTime;
GLdouble Window::frameTime;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float SPEED = 1.5f;
float Yaw = -90.0f;
float Pitch = 0.0f;
bool press_w = false;
bool press_s = false;
bool press_a = false;
bool press_d = false;
double lastX = 0.0;
double lastY = 0.0;
bool first_click = true;
bool debug_map = false;
bool fly_movement = false;
int Window::ShadowSwitch = 0;
int Window::ToonSwitch = 0;
GLdouble Window::displacement = 0.0;
glm::vec3 Window::currentPos = glm::vec3(0.0f, 0.0f, 0.0f);
const unsigned int Window::SHADOW_WIDTH = 1024, Window::SHADOW_HEIGHT = 1024;
unsigned int Window::depthMapFBO;
unsigned int Window::depthMap, Window::quadVAO, Window::quadVBO;


////////////////////////////////////////////////////////////////////Youliang's Variables
GLuint Window::bb_program;
GLuint Window::bb_projectionLoc; // Location of projection in shader.
GLuint Window::bb_viewLoc;
NewBoundingBox* Window::bb_box1;
NewBoundingBox* Window::bb_camera;
Geometry* Window::car;
NewBoundingBox* Window::bb_box2;
NewBoundingBox* Window::bb_box3;
NewBoundingBox* Window::bb_box4;
NewBoundingBox* Window::bb_box5;
std::vector<glm::vec3> box_vertices
{
		glm::vec3(-1, 1, 1),
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, -1),
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1)
};
bool bb_debug = false;
////////////////////////////////////////////////////////////////////


bool Window::initializeProgram() {
    // Initilize Shader for SkyBox
    cloudShader = LoadShaders("shaders/cloud_shader.vert", "shaders/cloud_shader.frag");
    skyBoxShader = LoadShaders("shaders/sky_box_shader.vert", "shaders/sky_box_shader.frag");
    defaultShader = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    depthShader = LoadShaders("shaders/depth.vert", "shaders/depth.frag");
    debugShader = LoadShaders("shaders/debug.vert", "shaders/debug.frag");
    toonShader = LoadShaders("shaders/toon_shadow.vert", "shaders/toon_shadow.frag");
	geoShader = LoadShaders("shaders/toon_geo.vert", "shaders/toon_geo.frag");
	////////////////////////////////////////////////////////////////////
	bb_program = LoadShaders("shaders/BoundingBox_shader.vert", "shaders/BoundingBox_shader.frag");
	///////////////////////////////////////////////////////////////////////
    if (!skyBoxShader || !defaultShader || !depthShader || !debugShader || !toonShader || !cloudShader)
    {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }
    projectionLoc = glGetUniformLocation(skyBoxShader, "projection");
    viewLoc = glGetUniformLocation(skyBoxShader, "view");
    
	////////////////////////////////////////////////////////////////////////////////
	bb_projectionLoc = glGetUniformLocation(bb_program, "projection");
	bb_viewLoc = glGetUniformLocation(bb_program, "view");
	/////////////////////////////////////////////////////////////////////////////////
    return true;
}

bool Window::initializeObjects()
{
    right = glm::normalize(glm::cross(front, up));
    view = glm::lookAt(eye, eye + front, up);
    // SkyBox
    skyBox = new SkyBox();
    stage = new Stage();
    box = new Box();
    cloud = new Cloud();
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "ERRO FRAME" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    glUseProgram(defaultShader);
    glUniform1i(glGetUniformLocation(defaultShader, "diffuseTexture"), 0);
    glUniform1i(glGetUniformLocation(defaultShader, "shadowMap"), 1);
    glUseProgram(debugShader);
    glUniform1i(glGetUniformLocation(debugShader, "depthMap"), 0);
    glUseProgram(toonShader);
    glUniform1i(glGetUniformLocation(toonShader, "diffuseTexture"), 0);
    glUniform1i(glGetUniformLocation(toonShader, "shadowMap"), 1);

	glUniform1i(glGetUniformLocation(geoShader, "diffuseTexture"), 0);
	glUniform1i(glGetUniformLocation(geoShader, "shadowMap"), 1);


	///////////////////////////////////////////////////////////////Youliang's
	car = new Geometry("objfile/car.obj");
	bb_box1 = new NewBoundingBox(car->vertices);
	bb_camera = new NewBoundingBox(eye + front, 1);
	bb_box2 = new NewBoundingBox(box_vertices);
	bb_box3 = new NewBoundingBox(box_vertices);
	bb_box4 = new NewBoundingBox(box_vertices);
	bb_box5 = new NewBoundingBox(box_vertices);
	///////////////////////////////////////////////////////////////////

    
    return true;
}

void Window::cleanUp()
{
    delete skyBox;
    //delete sphereTransfrom;
    
    
    glDeleteProgram(skyBoxShader);
    /*
     glDeleteProgram(sphereShader);
     glDeleteProgram(trackShader);
     */
}

GLFWwindow* Window::createWindow(int width, int height)
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }
    
    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);
    
#ifdef __APPLE__
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.
    
    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
    
    // Check if the window could not be created.
    if (!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }
    
    // Make the context of the window.
    glfwMakeContextCurrent(window);
    
#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.
    
    // Initialize GLEW.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif
    
    // Set swap interval to 1.
    glfwSwapInterval(0);
    
    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);
    
    prevTime = glfwGetTime();
    
    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);
    
    // Set the projection matrix.
    Window::projection = glm::perspective(glm::radians(60.0),
                                          double(width) / (double)height, 0.1,1000.0);
}

void Window::idleCallback() {
    
}

void Window::displayCallback(GLFWwindow* window)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    // Camera Control
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (press_w) {
		float velocity = SPEED * deltaTime;
		bb_camera = new NewBoundingBox(eye + glm::vec3(front.x, 0, front.z) * velocity, 1);
		bool collision1 = bb_box1->detectCollision_box(bb_box1, bb_camera);
		bool collision2 = bb_box1->detectCollision_box(bb_box2, bb_camera);
		bool collision3 = bb_box1->detectCollision_box(bb_box3, bb_camera);
		bool collision4 = bb_box1->detectCollision_box(bb_box4, bb_camera);
		bool collision5 = bb_box1->detectCollision_box(bb_box5, bb_camera);
		//std::cout << bb_box1->collision << std::endl;
		bool collision = collision1 || collision2 || collision3 || collision4 || collision5;
		if (!collision) {
			eye += glm::vec3(front.x, 0, front.z) * velocity;
			view = glm::lookAt(eye, eye + front, up);
		}
	}
	else if (press_s) {
		float velocity = SPEED * deltaTime;
		bb_camera = new NewBoundingBox(eye - glm::vec3(front.x, 0, front.z) * velocity, 1);
		bool collision1 = bb_box1->detectCollision_box(bb_box1, bb_camera);
		bool collision2 = bb_box1->detectCollision_box(bb_box2, bb_camera);
		bool collision3 = bb_box1->detectCollision_box(bb_box3, bb_camera);
		bool collision4 = bb_box1->detectCollision_box(bb_box4, bb_camera);
		bool collision5 = bb_box1->detectCollision_box(bb_box5, bb_camera);
		//std::cout << bb_box1->collision << std::endl;
		bool collision = collision1 || collision2 || collision3 || collision4 || collision5;
		if (!collision) {
			eye -= glm::vec3(front.x, 0, front.z) * velocity;
			view = glm::lookAt(eye, eye + front, up);
		}
	}
	if (press_a) {
		float velocity = SPEED * deltaTime;
		bb_camera = new NewBoundingBox(eye - glm::vec3(right.x, 0, right.z) * velocity, 1);
		bool collision1 = bb_box1->detectCollision_box(bb_box1, bb_camera);
		bool collision2 = bb_box1->detectCollision_box(bb_box2, bb_camera);
		bool collision3 = bb_box1->detectCollision_box(bb_box3, bb_camera);
		bool collision4 = bb_box1->detectCollision_box(bb_box4, bb_camera);
		bool collision5 = bb_box1->detectCollision_box(bb_box5, bb_camera);
		//std::cout << bb_box1->collision << std::endl;
		bool collision = collision1 || collision2 || collision3 || collision4 || collision5;
		if (!collision) {
			eye -= glm::vec3(right.x, 0, right.z) * velocity;
			view = glm::lookAt(eye, eye + front, up);
		}

	}
	else if (press_d) {
		float velocity = SPEED * deltaTime;
		bb_camera = new NewBoundingBox(eye + glm::vec3(right.x, 0, right.z) * velocity, 1);
		bool collision1 = bb_box1->detectCollision_box(bb_box1, bb_camera);
		bool collision2 = bb_box1->detectCollision_box(bb_box2, bb_camera);
		bool collision3 = bb_box1->detectCollision_box(bb_box3, bb_camera);
		bool collision4 = bb_box1->detectCollision_box(bb_box4, bb_camera);
		bool collision5 = bb_box1->detectCollision_box(bb_box5, bb_camera);
		//std::cout << bb_box1->collision << std::endl;
		bool collision = collision1 || collision2 || collision3 || collision4 || collision5;
		if (!collision) {
			eye += glm::vec3(right.x, 0, right.z) * velocity;
			view = glm::lookAt(eye, eye + front, up);
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    lightView = glm::lookAt(lightPos, glm::vec3(0,0,0), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    
    // First Pass of Shadow Mapping -> Render from light point of view
    glUseProgram(depthShader);
    glUniformMatrix4fv(glGetUniformLocation(depthShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // floor
    glm::mat4 flor_scale = glm::scale(glm::mat4(1), glm::vec3(5,0.1,5));
    box->draw(glm::translate(flor_scale, glm::vec3(0,-5,0)),depthShader);
    
    // boxes
	glm::mat4 scaler = glm::mat4(1);
	scaler = glm::translate(scaler, glm::vec3(0, -0.4, 0));
	scaler = glm::scale(scaler, car->uniformScale());
	scaler = glm::scale(scaler, glm::vec3(2, 2, 2));
    glm::mat4 size_mtx = glm::scale(glm::mat4(1), glm::vec3(0.5, 0.5, 0.5));
    box->draw(glm::translate(size_mtx, glm::vec3(8.0f, 0.0f, 6.0)), depthShader);
    box->draw(glm::translate(size_mtx, glm::vec3(-8.0f, 0.0f, -6.0)), depthShader);
    box->draw(glm::translate(size_mtx, glm::vec3(8.0f, 0.0f, -7.0)), depthShader);
    box->draw(glm::translate(size_mtx, glm::vec3(-7.0f, 0.0f, 8.0)), depthShader);
	car->draw(scaler, depthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Reset View Port
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Not In Debug Mode -> Second Pass: Render Scene with shadow
    //if(!debug_map){
    /*
     glUseProgram(defaultShader);
     glUniformMatrix4fv(glGetUniformLocation(defaultShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
     glUniformMatrix4fv(glGetUniformLocation(defaultShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
     glUniformMatrix4fv(glGetUniformLocation(defaultShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
     glUniform3fv(glGetUniformLocation(defaultShader, "viewPos"), 1, glm::value_ptr(eye));
     glUniform3fv(glGetUniformLocation(defaultShader, "lightPos"), 1, glm::value_ptr(lightPos));
    */
    
    glUseProgram(toonShader);
    glUniformMatrix4fv(glGetUniformLocation(toonShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    glUniformMatrix4fv(glGetUniformLocation(toonShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(toonShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(glGetUniformLocation(toonShader, "viewPos"), 1, glm::value_ptr(eye));
    glUniform3fv(glGetUniformLocation(toonShader, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform1i(glGetUniformLocation(toonShader, "ToonSwitch"), ToonSwitch);
    glUniform1i(glGetUniformLocation(toonShader, "ShadowSwitch"), ShadowSwitch);
   // glUniform3fv(glGetUniformLocation(toonShader, "lightColor"), 1, glm::value_ptr(lightColor));
    //glUniform1f(glGetUniformLocation(toonShader, "shininess"), 64.0f);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    /*
    // floor
    
    box->draw(glm::translate(flor_scale, glm::vec3(0,-5,0)),defaultShader);
    
    // boxes
    size_mtx = glm::scale(glm::mat4(1), glm::vec3(0.5, 0.5, 0.5));
    box->draw(glm::translate(size_mtx, glm::vec3(8.0f, 0.0f, 6.0)), defaultShader);
    box->draw(glm::translate(size_mtx, glm::vec3(-8.0f, 0.0f, -6.0)), defaultShader);
    box->draw(glm::translate(size_mtx, glm::vec3(8.0f, 0.0f, -7.0)), defaultShader);
    box->draw(glm::translate(size_mtx, glm::vec3(-7.0f, 0.0f, 8.0)), defaultShader);
    */
    
    box->draw(glm::translate(flor_scale, glm::vec3(0,-5,0)),toonShader);
    
    // boxes
    size_mtx = glm::scale(glm::mat4(1), glm::vec3(0.5, 0.5, 0.5));
    box->draw(glm::translate(size_mtx, glm::vec3(8.0f, 0.0f, 6.0)), toonShader);
    box->draw(glm::translate(size_mtx, glm::vec3(-8.0f, 0.0f, -6.0)), toonShader);
    box->draw(glm::translate(size_mtx, glm::vec3(8.0f, 0.0f, -7.0)), toonShader);
    box->draw(glm::translate(size_mtx, glm::vec3(-7.0f, 0.0f, 8.0)), toonShader);
    
    glUseProgram(skyBoxShader);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    skyBox->draw(glm::mat4(1), skyBoxShader);
    // cloud
    glUseProgram(cloudShader);
    glUniformMatrix4fv(glGetUniformLocation(cloudShader, "P"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(cloudShader, "V"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform1f(glGetUniformLocation(cloudShader, "time"), (float)glfwGetTime() * 0.5f);
    cloud->draw();
    //  }
    // else{
    


	/////////////////////////////////////////////////////////////////////////////////////////Youliang's code
	glUseProgram(bb_program);
	glUniformMatrix4fv(bb_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(bb_viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	
	//car->draw(scaler, bb_program);
	bb_box1->update_with_C(scaler);
	bb_box2->update_with_C(glm::translate(size_mtx, glm::vec3(8.0f, 0.0f, 6.0)));
	bb_box3->update_with_C(glm::translate(size_mtx, glm::vec3(-8.0f, 0.0f, -6.0)));
	bb_box4->update_with_C(glm::translate(size_mtx, glm::vec3(8.0f, 0.0f, -7.0)));
	bb_box5->update_with_C(glm::translate(size_mtx, glm::vec3(-7.0f, 0.0f, 8.0)));
	if (bb_debug) {
		bb_box1->draw(bb_program, scaler);
		bb_box2->draw(bb_program, glm::translate(size_mtx, glm::vec3(8.0f, 0.0f, 6.0)));
		bb_box3->draw(bb_program, glm::translate(size_mtx, glm::vec3(-8.0f, 0.0f, -6.0)));
		bb_box4->draw(bb_program, glm::translate(size_mtx, glm::vec3(8.0f, 0.0f, -7.0)));
		bb_box5->draw(bb_program, glm::translate(size_mtx, glm::vec3(-7.0f, 0.0f, 8.0)));
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glUseProgram(geoShader);
	glUniformMatrix4fv(glGetUniformLocation(geoShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glUniformMatrix4fv(glGetUniformLocation(geoShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(geoShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(glGetUniformLocation(geoShader, "viewPos"), 1, glm::value_ptr(eye));
	glUniform3fv(glGetUniformLocation(geoShader, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform1i(glGetUniformLocation(geoShader, "ToonSwitch"), ToonSwitch);
	glUniform1i(glGetUniformLocation(geoShader, "ShadowSwitch"), ShadowSwitch);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	car->draw(scaler, geoShader);


	if (Quad_render) {
		glViewport(width / 2, 0, width / 2, height / 2);
		glEnable(GL_DEPTH_TEST);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(debugShader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		// }
	}

    glfwPollEvents();
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Check for a key press.
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_W:
                if(!press_s)press_w = true;
                break;
            case GLFW_KEY_S:
                if(!press_w)press_s = true;
                break;
            case GLFW_KEY_A:
                if(!press_d)press_a = true;
                break;
            case GLFW_KEY_D:
                if(!press_a)press_d = true;
                break;
            case GLFW_KEY_M:
                debug_map = !debug_map;
                break;
            case GLFW_KEY_F:
                fly_movement = !fly_movement;
                break;
            case GLFW_KEY_T:
                ToonSwitch = 1 - ToonSwitch;
                break;
            case GLFW_KEY_U:
                ShadowSwitch = 1 - ShadowSwitch;
                break;
            case GLFW_KEY_Q:
                Quad_render = !Quad_render;
                break;
				/////////////////////////////////////////////////////////////////////////////
			case GLFW_KEY_P:
				bb_debug = !bb_debug;
				break;
				//////////////////////////////////////////////////////////////////////////////
            default:
                break;
        }
    }
    else if(action == GLFW_RELEASE){
        switch(key){
            case GLFW_KEY_W:
                press_w = false;
                break;
            case GLFW_KEY_S:
                press_s = false;
                break;
            case GLFW_KEY_A:
                press_a = false;
                break;
            case GLFW_KEY_D:
                press_d = false;
                break;
            default:
                break;
        }
    }
}

/*
 * Move the camera forward or backward
 */
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {}


void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double cursorX, cursorY;
    
    // track mouse movement
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        
        // track starting position
        if (action == GLFW_PRESS)
        {
            buttonPressed = 1;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            lastPoint = trackBallMapping(window, cursorX, cursorY);
        }
        else if (action == GLFW_RELEASE)
        {
            buttonPressed = 0;
        }
    }
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    double cursorX, cursorY;
    glm::vec3 curPoint, direction;
    
    // perform rotation is button is pressed
    if (buttonPressed) {
        glfwGetCursorPos(window, &cursorX, &cursorY);
        curPoint = trackBallMapping(window, cursorX, cursorY);
        direction = curPoint - lastPoint;
        float velocity = glm::length(direction);
        if (velocity > 0.0001)    // do nothing is little movement
        {
            glm::vec3 rotAxis(0,0,0);
            if(1){
                rotAxis = glm::cross(lastPoint, curPoint);
            }
            else{
                rotAxis = glm::cross(glm::vec3(lastPoint.x, 0, lastPoint.z), glm::vec3(curPoint.x, 0, curPoint.z));
            }
            GLfloat rotAngle = glm::acos(glm::dot(lastPoint, curPoint));
            front = glm::normalize(glm::rotate(rotAngle, rotAxis) * glm::vec4(front, 0));
            right = glm::normalize(glm::cross(front, world_up));
            up  = glm::normalize(glm::cross(right, front));
            view = glm::lookAt(eye, eye+front, up);
        }
        
        
    }
    lastPoint = curPoint;
}


glm::vec3 Window::trackBallMapping(GLFWwindow* window, double xpos, double ypos)
{
    // Treat the mouse position as the projection of a point on the hemi-sphere down to
    // the image plane (along z axis), and determine the point on the hemi-sphere.
    glm::vec3 v;
    double d;
    int windowX, windowY;
    glfwGetWindowSize(window, &windowX, &windowY);
    v.x = (2.0 * xpos - (double)windowX) / (double)windowX;
    v.y = ((double)windowY - 2.0 * ypos) / (double)windowY;
    v.z = 0.0;
    d = glm::length(v);
    d = (d < 1.0) ? d : 1.0;
    v.z = sqrt(1.001 - d * d);
    v = glm::normalize(v);
    return v;
}
