//
//  shader.hpp
//  cse167pa3
//
//  Created by Derek Wang on 10/30/19.
//  Copyright © 2019 Derek Wang. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

#endif /* shader_hpp */
