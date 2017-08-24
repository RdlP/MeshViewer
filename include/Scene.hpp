#ifndef SCENE_HEADER
#define SCENE_HEADER

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
#include "Mesh.hpp"
#include "Camera.hpp"

class Scene{
public:
	Scene();
	virtual ~Scene();
	void add(Mesh *m);
	std::vector<Mesh*> getChilds();
	Mesh* getChild(int i);
	void setChilds(std::vector<Mesh*>);
	void render(Camera camera);
	Mesh* pick(Camera camera, glm::vec2 screen);
	std::vector<Mesh*> intersectAll(Camera camera, glm::vec2 screen);


private:
	std::vector<Mesh*> m_childs; 
};

#endif