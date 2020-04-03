//
//  geometry.hpp
//  cse167pa3
//
//  Created by Derek Wang on 10/30/19.
//  Copyright © 2019 Derek Wang. All rights reserved.
//

#ifndef geometry_hpp
#define geometry_hpp

#include "node.hpp"
#include <stdio.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <initializer_list>




class Geometry : public Node {
    
public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textures;
    std::vector<int> indices;
    GLuint vao, vbo_vertices, ebo, vbo_normals, vbo_textures;
    struct Material {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        GLfloat shininess;
    };
    Material material;
    glm::mat4 model;
    glm::vec3 color;
    Geometry();
    Geometry(std::string objFilename);
    ~Geometry();
    void draw(glm::mat4 C,GLuint shaderProgram);
    void update(glm::mat4 updateMatrix);
    void readFile(std::string objFilename);
    void setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess);
    void setColor(glm::vec3 color);
	glm::vec3 uniformScale();
	GLfloat
		min_x, max_x,
		min_y, max_y,
		min_z, max_z;
};

#endif /* geometry_hpp */
