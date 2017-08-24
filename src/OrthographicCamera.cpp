#include "OrthographicCamera.hpp"

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar):
	Camera(), m_left(left), m_right(right), m_bottom(bottom), m_top(top), m_zNear(zNear), m_zFar(zFar)
{
	setProjection(glm::ortho(m_left, m_right, m_bottom, m_top, zNear, zFar));
}

OrthographicCamera::OrthographicCamera(){

}

OrthographicCamera::~OrthographicCamera(){

}