//
//  geometry.cpp
//  cse167pa3
//
//  Created by Derek Wang on 10/30/19.
//  Copyright © 2019 Derek Wang. All rights reserved.
//

#include "geometry.hpp"

/*
 * Constructor: default constructor
 */
Geometry::Geometry() {
    model = glm::mat4(1);
    color = glm::vec3(0.0f, 0.0f, 0.0f);
}

/*
 * Constructor: read file and set up shader
 */
Geometry::Geometry(std::string objFilename) {
    
    // Read in file
    readFile(objFilename);
    
    // Set material to be green rubber as default
    glm::vec3 ambient(0.25f, 0.6f, 0.2f);
    glm::vec3 diffuse(0.25f, 0.6f, 0.2f);
    glm::vec3 specular(0.25f, 0.6f, 0.2f);
    setMaterial(ambient, diffuse, specular, 0.78125);
    
    // Set model and color
    model = glm::mat4(1);
    color = glm::vec3(0.25f, 0.6f, 0.2f);
    
    // Set up vao, vbos and ebo
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glGenBuffers(1, &vbo_textures);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_textures);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textures.size(), textures.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/*
 * Destroctor: delete vao, vbos and ebo
 */
Geometry::~Geometry() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_textures);
    glDeleteBuffers(1, &ebo);
}

/*
 * Draw itself to shderProgram with transformation from C
 */
void Geometry::draw(glm::mat4 C, GLuint shaderProgram ) {
    
    // Transfrom
    glm::mat4 toDraw = C * model;
	
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(toDraw));
    //glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(color));
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    
    // Draw triangles using indices
    glDrawElements(GL_TRIANGLES, (int)indices.size() * 3, GL_UNSIGNED_INT, 0);
    
    // Unbind from the VAO.
    glBindVertexArray(0);
}

/*
 * Update
 */
void Geometry::update(glm::mat4 updateMatrix) {}

/*
 * Read object from .obj file
 */
void Geometry::readFile(std::string objFilename) {
    
    std::ifstream objFile(objFilename);
    std::vector<glm::vec3> input_vertices;
    std::vector<glm::vec3> input_normals;
    std::vector<glm::vec2> input_textures;
    std::vector<int> vertex_indices, texture_indices, normal_indices;
    
    if (objFile.is_open())
    {
        std::string line; // A line in the file.
        
        while (std::getline(objFile, line))
        {
            std::stringstream ss;
            ss << line;
            std::string label;
            ss >> label;
            
            // If the line is about vertex (starting with a "v").
            if (label == "v")
            {
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;
                input_vertices.push_back(point);
            }
            
            // If the line is about normals
            if (label == "vn")
            {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                input_normals.push_back(normal);
            }
            
            // If the line is about textures
            if (label == "vt")
            {
                glm::vec2 texture;
                ss >> texture.x >> texture.y;
                input_textures.push_back(texture);
            }
            
            // If the line is about triangles (faces)
            if (label == "f")
            {
                std::string a, b, c;
                ss >> a >> b >> c;
                std::string delimiter = "/";
                std::string x, y, z;
                std::size_t loc;
                
                // Part a
                loc = a.find(delimiter);
                x = a.substr(0, loc);
                a = a.substr(loc+1, a.length());
                loc = a.find(delimiter);
                y = a.substr(0, loc);
                z = a.substr(loc+1, a.length());
                vertex_indices.push_back(std::stoi(x));
                texture_indices.push_back(std::stoi(y));
                normal_indices.push_back(std::stoi(z));
                
                // Part b
                loc = b.find(delimiter);
                x = b.substr(0, loc);
                b = b.substr(loc+1, b.length());
                loc = b.find(delimiter);
                y = b.substr(0, loc);
                z = b.substr(loc+1, b.length());
                vertex_indices.push_back(std::stoi(x));
                texture_indices.push_back(std::stoi(y));
                normal_indices.push_back(std::stoi(z));
                
                // Part c
                loc = c.find(delimiter);
                x = c.substr(0, loc);
                c = c.substr(loc+1, c.length());
                loc = c.find(delimiter);
                y = c.substr(0, loc);
                z = c.substr(loc+1, c.length());
                vertex_indices.push_back(std::stoi(x));
                texture_indices.push_back(std::stoi(y));
                normal_indices.push_back(std::stoi(z));
            }
        }
    }
    else
    {
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }
    objFile.close();
    
    // Process the data
    for (int i = 0; i < vertex_indices.size(); i++) {
        vertices.push_back(input_vertices[(vertex_indices[i]-1)]);
        textures.push_back(input_textures[(texture_indices[i]-1)]);
        normals.push_back(input_normals[(normal_indices[i]-1)]);
        indices.push_back(i);
    }
}

/*
 * Set the material of object
 */
void Geometry::setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess){
    material.ambient = ambient;
    material.diffuse = diffuse;
    material.specular = specular;
    material.shininess = shininess;
}

/*
 * Set color of the geometry
 */
void Geometry::setColor(glm::vec3 color) {
    this->color = color;
}

glm::vec3 Geometry::uniformScale() {

	min_x = max_x = vertices.at(0).x;
	min_y = max_y = vertices.at(0).y;
	min_z = max_z = vertices.at(0).z;
	GLfloat max = 0;
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices.at(i).x < min_x) min_x = vertices.at(i).x;
		if (vertices.at(i).x > max_x) max_x = vertices.at(i).x;
		if (vertices.at(i).y < min_y) min_y = vertices.at(i).y;
		if (vertices.at(i).y > max_y) max_y = vertices.at(i).y;
		if (vertices.at(i).z < min_z) min_z = vertices.at(i).z;
		if (vertices.at(i).z > max_z) max_z = vertices.at(i).z;
	}
	max =std::max({ max_x-min_x, max_y - min_y, max_z - min_z });

	glm::vec3 size = glm::vec3(1/max, 1/max, 1/max);
	return size;
}
