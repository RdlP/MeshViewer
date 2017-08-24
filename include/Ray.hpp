#ifndef RAY_HEADER
#define RAY_HEADER

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Ray {
	public:
		Ray(glm::vec3 origin, glm::vec3 dir);
		virtual ~Ray();
		void setOrigin(glm::vec3 origin);
		glm::vec3 getOrigin();
		void setDirection(glm::vec3 direction);
		glm::vec3 getDirection();

	private:
		glm::vec3 m_origin, m_dir;
};
#endif