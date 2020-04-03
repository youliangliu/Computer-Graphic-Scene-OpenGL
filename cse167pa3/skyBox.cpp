//
//  skyBox.cpp
//  cse167pa3
//
//  Created by Derek Wang on 11/11/19.
//  Copyright © 2019 Derek Wang. All rights reserved.
//

#include "skyBox.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SkyBox::SkyBox() {
    
    // Points for skybox
    float points[] = {
        -500.0f,  500.0f, -500.0f,
        -500.0f, -500.0f, -500.0f,
        500.0f, -500.0f, -500.0f,
        500.0f, -500.0f, -500.0f,
        500.0f,  500.0f, -500.0f,
        -500.0f,  500.0f, -500.0f,
        
        -500.0f, -500.0f,  500.0f,
        -500.0f, -500.0f, -500.0f,
        -500.0f,  500.0f, -500.0f,
        -500.0f,  500.0f, -500.0f,
        -500.0f,  500.0f,  500.0f,
        -500.0f, -500.0f,  500.0f,
        
        500.0f, -500.0f, -500.0f,
        500.0f, -500.0f,  500.0f,
        500.0f,  500.0f,  500.0f,
        500.0f,  500.0f,  500.0f,
        500.0f,  500.0f, -500.0f,
        500.0f, -500.0f, -500.0f,
        
        -500.0f, -500.0f,  500.0f,
        -500.0f,  500.0f,  500.0f,
        500.0f,  500.0f,  500.0f,
        500.0f,  500.0f,  500.0f,
        500.0f, -500.0f,  500.0f,
        -500.0f, -500.0f,  500.0f,
        
        -500.0f,  500.0f, -500.0f,
        500.0f,  500.0f, -500.0f,
        500.0f,  500.0f,  500.0f,
        500.0f,  500.0f,  500.0f,
        -500.0f,  500.0f,  500.0f,
        -500.0f,  500.0f, -500.0f,
        
        -500.0f, -500.0f, -500.0f,
        -500.0f, -500.0f,  500.0f,
        500.0f, -500.0f, -500.0f,
        500.0f, -500.0f, -500.0f,
        -500.0f, -500.0f,  500.0f,
        500.0f, -500.0f,  500.0f
    };
    
    // Set up vao, vbo, ebo
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, 3*36*sizeof(float), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    std::vector<std::string> faces {
		 "./sky_box/right.jpg",
		"./sky_box/left.jpg",
		"./sky_box/top.jpg",
		"./sky_box/base.jpg",
		"./sky_box/front.jpg",
		"./sky_box/back.jpg",
    };
    
    cubemapTexture = loadCubemap(faces);
}

SkyBox::~SkyBox() {
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteVertexArrays(1, &vao);
}

void SkyBox::draw(glm::mat4 C, GLuint shaderProgram) {
    
    glDepthFunc(GL_LEQUAL);
    
    glm::mat4 toDraw = C * model;
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(toDraw));
    glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(color));
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

unsigned int SkyBox::loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                         );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}
