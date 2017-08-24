#ifndef ORTHOGRAPHICCAMERA_HEADER
#define ORTHOGRAPHICCAMERA_HEADER

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


#include "Camera.hpp"

class OrthographicCamera : public Camera{
public:
	OrthographicCamera(float m_left, float m_right, float m_bottom, float m_top, float zNear, float zFar);
	OrthographicCamera();
	virtual ~OrthographicCamera();

private:
	float m_left, m_right, m_top, m_bottom, m_zFar, m_zNear;
};

#endif