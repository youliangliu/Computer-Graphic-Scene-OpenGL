//
//  transform.hpp
//  cse167pa3
//
//  Created by Derek Wang on 10/30/19.
//  Copyright © 2019 Derek Wang. All rights reserved.
//

#ifndef transform_hpp
#define transform_hpp

#include "node.hpp"
#include <stdio.h>
#include <vector>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform : public Node {
    
public:
    std::vector<Node * > children;
    glm::mat4 transformMatrix;
    GLuint animation;
    
    Transform(glm::mat4 M, GLuint animation);
    ~Transform();
    void draw(glm::mat4 C, GLuint shaderProgram);
    void update(glm::mat4 updateMatrix);
    void addChild(Node * child);
    void setMatrix(glm::mat4 M);
};

#endif /* transform_hpp */
