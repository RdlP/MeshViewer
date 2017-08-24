#include "Ray.hpp"

Ray::Ray(glm::vec3 origin, glm::vec3 dir)
	: m_origin(origin), m_dir(dir)
{

}

Ray::~Ray()
{

}

void Ray::setOrigin(glm::vec3 origin)
{
	m_origin = origin;
}

glm::vec3 Ray::getOrigin()
{
	return m_origin;
}

void Ray::setDirection(glm::vec3 direction)
{
	m_dir = direction;
}

glm::vec3 Ray::getDirection()
{
	return m_dir;
}