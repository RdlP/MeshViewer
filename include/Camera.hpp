#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#define GLM_ENABLE_EXPERIMENTAL
#include <fstream>
#include <string.h>
#include <math.h>
#include <ostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <string.h>
#include <glm/ext.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>


#include "Ray.hpp"

class Camera{
public:
	Camera();
	virtual ~Camera();
	void lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up);
	void update();
	glm::vec2 project(glm::vec3 world);
	glm::vec3 unpoject(glm::vec2 screen);
	glm::mat4 getProjection();
	void setProjection(glm::mat4 projection);
	glm::mat4 getView();
	void setView(glm::mat4 view);
	Ray getRay(glm::vec2 screen);
	glm::vec3 screenToWorldCoordinates(glm::vec2 screen_pos);
	

private:
	glm::mat4 m_projection, m_view;
	glm::vec3 m_eye, m_target, m_up;
	float m_fov, m_aspect, m_zNear, m_zFar, m_width, m_height;
};

#endif