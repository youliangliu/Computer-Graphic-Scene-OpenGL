//
//  Stage.hpp
//  cse167pa3
//
//  Created by Yaocong Hu on 12/3/19.
//  Copyright © 2019 Yaocong Hu. All rights reserved.
//

#ifndef Stage_hpp
#define Stage_hpp
#include "geometry.hpp"
#include <stdio.h>
class Stage : public Geometry{
public:
    unsigned int texture;
    unsigned int VAO, VBO, EBO;
    Stage();
    ~Stage();
    void draw(glm::mat4 C, GLuint shaderProgram);
    unsigned int loadTexture(std::string path);
};
#endif /* Stage_hpp */
