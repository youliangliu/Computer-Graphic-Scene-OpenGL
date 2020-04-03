#include "cloud.hpp"

/*
 * Cloud constructor
 */
Cloud::Cloud() {
  
  glEnable(GL_CULL_FACE);
  
  // create vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  // create vbo
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
  //glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
}

void Cloud::draw() {
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
