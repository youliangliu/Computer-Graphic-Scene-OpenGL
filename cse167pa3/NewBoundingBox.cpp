#include "NewBoundingBox.h"
#include <fstream>
#include <sstream> 
#include <iostream>
#include <math.h>
using namespace std;


NewBoundingBox::NewBoundingBox(glm::vec3 pos, GLfloat offset) {
	collision = 0;
	max_x = pos.x + offset;
	min_x = pos.x - offset;
	max_y = pos.y + offset;
	min_y = pos.y - offset;
	max_z = pos.z + offset;
	min_z = pos.z - offset;
}



NewBoundingBox::NewBoundingBox(std::vector<glm::vec3> vertices) {
	collision = 0;
	model = glm::mat4(1);
	vertex = vertices;

	min_x = max_x = vertices.at(0).x;
	min_y = max_y = vertices.at(0).y;
	min_z = max_z = vertices.at(0).z;
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices.at(i).x < min_x) min_x = vertices.at(i).x;
		if (vertices.at(i).x > max_x) max_x = vertices.at(i).x;
		if (vertices.at(i).y < min_y) min_y = vertices.at(i).y;
		if (vertices.at(i).y > max_y) max_y = vertices.at(i).y;
		if (vertices.at(i).z < min_z) min_z = vertices.at(i).z;
		if (vertices.at(i).z > max_z) max_z = vertices.at(i).z;
	}
	std::vector<glm::vec3> box_vertices;
	box_vertices.push_back(glm::vec3(min_x, max_y, max_z));
	box_vertices.push_back(glm::vec3(min_x, min_y, max_z));
	box_vertices.push_back(glm::vec3(max_x, min_y, max_z));
	box_vertices.push_back(glm::vec3(max_x, max_y, max_z));
	box_vertices.push_back(glm::vec3(min_x, max_y, min_z));
	box_vertices.push_back(glm::vec3(min_x, min_y, min_z));
	box_vertices.push_back(glm::vec3(max_x, min_y, min_z));
	box_vertices.push_back(glm::vec3(max_x, max_y, min_z));

	std::vector<glm::ivec3> box_indices
	{
		// Front face.
		glm::ivec3(0, 1, 2),
		glm::ivec3(2, 3, 0),
		// Back face.
		glm::ivec3(7, 6, 5),
		glm::ivec3(5, 4, 7),
		// Right face.
		glm::ivec3(3, 2, 6),
		glm::ivec3(6, 7, 3),
		// Left face.
		glm::ivec3(4, 5, 1),
		glm::ivec3(1, 0, 4),
		// Top face.
		glm::ivec3(4, 0, 3),
		glm::ivec3(3, 7, 4),
		// Bottom face.
		glm::ivec3(1, 5, 6),
		glm::ivec3(6, 2, 1),
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * box_vertices.size(),
		box_vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * box_indices.size(),
		box_indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);


}

void NewBoundingBox::draw(GLuint shaderProgram, glm::mat4 C)
{
	/*
	std::vector<glm::vec3> temp = vertex;
	for (int i = 0; i < temp.size(); i++) {
		temp.at(i).x = (C * glm::vec4(temp.at(1), 1)).x;
		temp.at(i).y = (C * glm::vec4(temp.at(1), 1)).y;
		temp.at(i).z = (C * glm::vec4(temp.at(1), 1)).z;
	}
	min_x = max_x = temp.at(0).x;
	min_y = max_y = temp.at(0).y;
	min_z = max_z = temp.at(0).z;
	for (int i = 0; i < temp.size(); i++) {
		if (temp.at(i).x < min_x) min_x = temp.at(i).x;
		if (temp.at(i).x > max_x) max_x = temp.at(i).x;
		if (temp.at(i).y < min_y) min_y = temp.at(i).y;
		if (temp.at(i).y > max_y) max_y = temp.at(i).y;
		if (temp.at(i).z < min_z) min_z = temp.at(i).z;
		if (temp.at(i).z > max_z) max_z = temp.at(i).z;
	}
	*/
	

	glUseProgram(shaderProgram);
	
	glm::mat4 temp1 = C * model;
	

	modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(temp1));

	collisionLoc = glGetUniformLocation(shaderProgram, "collision");
	glUniform1i(collisionLoc, collision);

	
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}


bool NewBoundingBox::detectCollision_box(NewBoundingBox* box1, NewBoundingBox* box2) {
	bool collision_x = box1->max_x >= box2->min_x && box2->max_x >= box1->min_x;
	
	
	bool collision_y = box1->max_y >= box2->min_y && box2->max_y >= box1->min_y;
	//std::cout <<"y: " <<  collision_y << std::endl;
	bool collision_z = box1->max_z >= box2->min_z && box1->min_z <= box2->max_z;
	//std::cout <<"box1 max_z :  " << box1->max_x << "box2 min_z : " << box2->min_z <<  std::endl;
	//std::cout << "box1 x max: " << box1->max_z << std::endl;
	//std::cout << "box2 x min: " << box2->min_z << std::endl;
	bool result = collision_x && collision_y && collision_z;
	box1->collision = result;
	box2->collision = result;
	return result;
}


void NewBoundingBox::update(std::vector<glm::vec3> vertices) {
	vertex = vertices;
	min_x = max_x = vertex.at(0).x;
	min_y = max_y = vertex.at(0).y;
	min_z = max_z = vertex.at(0).z;
	for (int i = 0; i < vertex.size(); i++) {
		if (vertex.at(i).x < min_x) min_x = vertex.at(i).x;
		if (vertex.at(i).x > max_x) max_x = vertex.at(i).x;
		if (vertex.at(i).y < min_y) min_y = vertex.at(i).y;
		if (vertex.at(i).y > max_y) max_y = vertex.at(i).y;
		if (vertex.at(i).z < min_z) min_z = vertex.at(i).z;
		if (vertex.at(i).z > max_z) max_z = vertex.at(i).z;
	}

}

void NewBoundingBox::update_with_C(glm::mat4 C) {
	std::vector<glm::vec3> temp = vertex;
	for (int i = 0; i < temp.size(); i++) {
		temp.at(i).x = (C * glm::vec4(temp.at(1), 1)).x;
		temp.at(i).y = (C * glm::vec4(temp.at(1), 1)).y;
		temp.at(i).z = (C * glm::vec4(temp.at(1), 1)).z;
	}
	min_x = max_x = temp.at(0).x;
	min_y = max_y = temp.at(0).y;
	min_z = max_z = temp.at(0).z;
	for (int i = 0; i < temp.size(); i++) {
		if (temp.at(i).x < min_x) min_x = temp.at(i).x;
		if (temp.at(i).x > max_x) max_x = temp.at(i).x;
		if (temp.at(i).y < min_y) min_y = temp.at(i).y;
		if (temp.at(i).y > max_y) max_y = temp.at(i).y;
		if (temp.at(i).z < min_z) min_z = temp.at(i).z;
		if (temp.at(i).z > max_z) max_z = temp.at(i).z;
	}
}