//
//  cloud.hpp
//  CS_Offline
//
//  Created by Derek Wang on 12/11/19.
//  Copyright © 2019 Derek Wang. All rights reserved.
//

#ifndef cloud_hpp
#define cloud_hpp

#include <stdio.h>
#include "node.hpp"

class Cloud {

public:
  
  GLuint vao, vbo;

  Cloud();
  ~Cloud();
  void draw();
};

#endif /* cloud_hpp */
