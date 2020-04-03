//
//  node.hpp
//  cse167pa3
//
//  Created by Derek Wang on 10/28/19.
//  Copyright © 2019 Derek Wang. All rights reserved.
//

#ifndef node_hpp
#define node_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <stdio.h>
#include <glm/glm.hpp>

class Node {
public:
    virtual void draw(glm::mat4 C, GLuint shaderProgram)=0;
    virtual void update(glm::mat4 updateMatrix)=0;
};

#endif /* node_hpp */
