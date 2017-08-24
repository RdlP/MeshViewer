#include "Camera.hpp"

Camera::Camera(){

}

Camera::~Camera(){

}

void Camera::lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up){
	m_eye = eye;
	m_target = target;
	m_up = up;
	update();
}

void Camera::update(){
	m_view = glm::lookAt(m_eye,m_target,m_up);
}

glm::vec2 Camera::project(glm::vec3 world){

}

glm::vec3 Camera::unpoject(glm::vec2 screen){

}

Ray Camera::getRay(glm::vec2 screen){
	float x = (2.0f * (screen.x / 800.0f)) - 1.0f;
	float y = 1.0f - (2.0f * (screen.y / 600.0f));
	float z = 1.0f;
	glm::vec3 ray_nds(x,y,z);

	glm::vec4 ray_clip(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

	glm::vec4 ray_eye = glm::inverse(m_projection) * ray_clip;
	ray_eye.z = -1.0;
	ray_eye.w = 0.0;

	glm::vec3 ray_wor = glm::vec3(glm::inverse(m_view) * ray_eye);
	ray_wor = glm::normalize(ray_wor);


	glm::vec3 origin = m_view[3];
	origin.x *= -1;
	origin.y *= -1;
	origin.z *= -1;
	Ray ray(origin,ray_wor);

	return ray;
}

glm::vec3 Camera::screenToWorldCoordinates(glm::vec2 screen_pos){
	glm::vec2 screen_dim(m_width, m_height);
	glm::vec2 pixel_dim(m_width, m_height);
	glm::vec2 pixel_pos=screen_pos * pixel_dim / screen_dim; // note: not necessarily integer
	pixel_pos = pixel_pos + glm::vec2(0.5f, 0.5f); // shift to GL's center convention
	glm::vec3 win=glm::vec3(pixel_pos.x, pixel_dim.y-1-pixel_pos.y, 0.0f);

	glReadPixels( (GLint)win.x, (GLint)win.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win.z);
	glm::vec4 viewport(0.0f,0.0f,(float)screen_dim.x, (float)screen_dim.y);
	glm::vec3 world = glm::unProject(win, m_view,m_projection,viewport);
	return world;
}

glm::mat4 Camera::getView(){
	return m_view;
}

void Camera::setView(glm::mat4 view){
	m_view = view;
}

glm::mat4 Camera::getProjection(){
	return m_projection;
}

void Camera::setProjection(glm::mat4 projection){
	m_projection = projection;
}