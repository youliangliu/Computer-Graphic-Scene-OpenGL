//
//  transform.cpp
//  cse167pa3
//
//  Created by Derek Wang on 10/30/19.
//  Copyright © 2019 Derek Wang. All rights reserved.
//

#include "transform.hpp"

/*
 * Constructor
 */
Transform::Transform(glm::mat4 M, GLuint animation) {
    transformMatrix = M;
    this->animation = animation;
}

/*
 * Destructor
 */
Transform::~Transform() {}

/*
 * Pass along draw command and with transfromed matrix
 */
void Transform::draw(glm::mat4 C, GLuint shaderProgram) {
    glm::mat4 newMatrix = C * transformMatrix;
    for (auto & child : children) {
        child->draw(newMatrix, shaderProgram);
    }
}

/*
 * Update the transformation
 */
void Transform::update(glm::mat4 updateMatrix) {
    transformMatrix = updateMatrix * transformMatrix;
}

/*
 * Add child (Node *) to children vector
 */
void Transform::addChild(Node *child) {
    children.push_back(child);
}

/*
 * Setter method for transform matrix
 */
void Transform::setMatrix(glm::mat4 M) {
    transformMatrix = M;
}
