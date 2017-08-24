#include "Scene.hpp"

Scene::Scene(){
	
}

Scene::~Scene(){

}

void Scene::add(Mesh *m){
	m_childs.push_back(m);
}

std::vector<Mesh*> Scene::getChilds(){
	return m_childs;
}

Mesh* Scene::getChild(int i){
	return m_childs.at(i);
}

void Scene::setChilds(std::vector<Mesh*> childs){
	m_childs = childs;
}

Mesh* Scene::pick(Camera camera, glm::vec2 screen){
	Ray ray = camera.getRay(screen);
	float d, minDistance = std::numeric_limits<float>::max();
	Mesh* pickedMesh = NULL;
	for (int i = 0; i < m_childs.size(); i++){
		bool intersect = m_childs.at(i)->testRay(ray, &d);
		if (intersect && d < minDistance){
			minDistance = d;
			pickedMesh = m_childs.at(i);
		}
	}
	return pickedMesh;
}

std::vector<Mesh*> Scene::intersectAll(Camera camera, glm::vec2 screen){
	Ray ray = camera.getRay(screen);
	std::vector<Mesh*> ret;
	float d;
	for (int i = 0; i < m_childs.size(); i++){
		bool intersect = m_childs.at(i)->testRay(ray, &d);	
		if (intersect){
			ret.push_back(m_childs.at(i));
		}
	}
	return ret;
}

void Scene::render(Camera camera){
	for (int i = 0; i < m_childs.size(); i++){
		m_childs.at(i)->render(camera);
	}
}