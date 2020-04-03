#ifndef _NewBoundingBox_H_
#define _NewBoundingBox_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <list>
using namespace std;
#include "geometry.hpp"

class NewBoundingBox
{
public:
	NewBoundingBox(std::vector<glm::vec3> vertices);
	NewBoundingBox(glm::vec3 pos, GLfloat offset);
	~NewBoundingBox();
	GLfloat
		min_x, max_x,
		min_y, max_y,
		min_z, max_z;
	int collision;
	std::vector<glm::vec3> vertex;
	GLuint vao, modelLoc, collisionLoc;
	GLuint vbos[2];
	glm::mat4 model;
	glm::vec3 position;
	void draw(GLuint shaderProgram, glm::mat4 C);
	bool detectCollision_box(NewBoundingBox* box1, NewBoundingBox* box2);
	void update(glm::vec3 position);
	void update(std::vector<glm::vec3> new_vertex);
	void update_with_C(glm::mat4 C);
};

#endif


