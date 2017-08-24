#ifndef PERSPECTIVECAMERA_HEADER
#define PERSPECTIVECAMERA_HEADER

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

class PerspectiveCamera : public Camera{
public:
	PerspectiveCamera(float fov, float width, float height, float zNear, float zFar);
	PerspectiveCamera();
	virtual ~PerspectiveCamera();

private:
	float m_fov, m_zNear, m_zFar, m_width, m_height;
};

#endif