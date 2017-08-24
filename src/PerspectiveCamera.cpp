#include "PerspectiveCamera.hpp"

PerspectiveCamera::PerspectiveCamera(float fov, float width, float height, float zNear, float zFar):
	Camera(), m_fov(fov), m_zNear(zNear), m_zFar(zFar), m_width(width), m_height(height)
{
	setProjection(glm::perspective(glm::radians(fov), width/height, zNear, zFar));
}

PerspectiveCamera::PerspectiveCamera(){

}

PerspectiveCamera::~PerspectiveCamera(){

}