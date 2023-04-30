#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

#include "ShaderClass.h"

class Camera {
public:
    Camera();
    Camera(int width, int height, glm::vec3 position);

    void matrix(Shader shader, const char *uniform);
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void inputs(GLFWwindow *window);

    void reset();


    glm::vec3 position;
    glm::vec3 orientation = glm::vec3(0.0f, -1.0f, -0.001f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    int width, height;

    float speed = 0.1f;
    float sensitivity = 100.0f;
    bool firstClick = true;;
    bool freeCam = false;
};

#endif