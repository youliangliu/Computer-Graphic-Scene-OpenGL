//
//  Box.hpp
//  cse167pa3
//
//  Created by Yaocong Hu on 12/5/19.
//  Copyright © 2019 Yaocong Hu. All rights reserved.
//

#ifndef Box_hpp
#define Box_hpp

#include <stdio.h>
#include "geometry.hpp"
class Box : public Geometry{
public:
    unsigned int texture;
    unsigned int VAO, VBO, EBO;
    Box();
    ~Box();
    void draw(glm::mat4 C, GLuint shaderProgram);
    unsigned int loadTexture(std::string path);
};
#endif /* Box_hpp */
