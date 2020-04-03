//
//  skyBox.hpp
//  cse167pa3
//
//  Created by Derek Wang on 11/11/19.
//  Copyright © 2019 Derek Wang. All rights reserved.
//

#ifndef skyBox_hpp
#define skyBox_hpp

#include <stdio.h>
#include "geometry.hpp"

class SkyBox : public Geometry {
    
public:
    unsigned int cubemapTexture;
    
    SkyBox();
    ~SkyBox();
    void draw(glm::mat4 C, GLuint shaderProgram);
    unsigned int loadCubemap(std::vector<std::string> faces);
};

#endif /* skyBox_hpp */
