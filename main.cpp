#define GLM_ENABLE_EXPERIMENTAL

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <string.h>
#include <glm/ext.hpp>
#include "Mesh.hpp"
#include "Ray.hpp"
#include "Camera.hpp"
#include "PerspectiveCamera.hpp"
#include "Scene.hpp"

bool lbutton_down = false;
double old_x, old_y, dX, dY;
double theta, phi;
float width=800.0f, height=600.0f;
bool translate = false;
Camera camera;
Scene scene;
Mesh* selectedMesh = NULL, *prevSelected = NULL;;

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if(GLFW_PRESS == action){
            lbutton_down = true;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
			old_x = xpos;
			old_y = ypos;
			float d = 0;
			
			selectedMesh = scene.pick(camera, glm::vec2(xpos,ypos));
			if (selectedMesh != NULL){
				selectedMesh->showAABB(true);
				selectedMesh->showBoundingBox(true);
			}
				for (int i = 0; i < scene.getChilds().size(); i++){
					Mesh * m = scene.getChild(i);
					if (selectedMesh == m){
						continue;
					}
					if (m->isShowingAABB()){
						m->showAABB(false);
					}
					if (m->isShowingBoundingBox()){
						m->showBoundingBox(false);
					}
				}

		}
        else if(GLFW_RELEASE == action)
            lbutton_down = false;
    }
}

void mousedrag(GLFWwindow *window, Mesh *mesh){
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	dX=(xpos-old_x)*2*M_PI/width,
    dY=(ypos-old_y)*2*M_PI/height;
    theta+=dX;
    phi+=dY;
    old_x=xpos, old_y=ypos;

    if (translate){
    	if (selectedMesh){
		    glm::vec3 world = camera.screenToWorldCoordinates(glm::vec2(xpos,ypos));
		    world.z = 0;
		    selectedMesh->setPosition(world);
		}
	}else{
		if (selectedMesh){
			selectedMesh->rotateWorldCoordinates(glm::vec3(1,0,0), phi);
	    	selectedMesh->rotateWorldCoordinates(glm::vec3(0,1,0), theta);
	    }
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_T && action == GLFW_PRESS){
        translate = true;
    }else if (key == GLFW_KEY_R && action == GLFW_PRESS){
        translate = false;
    }
}

int main(int argc, char** argv){
	if (!glfwInit()){
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Queremos OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window;

	window = glfwCreateWindow(width,height, "MeshViewer", NULL, NULL);
	if (window == NULL){
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE; 
	if (glewInit() != GLEW_OK){
		return -1;
	}
	glViewport(0,0,width, height);

	camera = PerspectiveCamera(45.0f,width, height, 0.1f, 1000.0f);
	camera.lookAt(glm::vec3(0,0,3*3),glm::vec3(0,0,0),glm::vec3(0,1,0));
	Mesh mesh;
	mesh.load(argv[1]);
	scene.add(&mesh);
	mesh.setBoundingBoxColor(glm::vec3(1,0,0));

	std::stringstream ss;
	ss << "MeshViewer - " << mesh;
	glfwSetWindowTitle(window, ss.str().c_str());

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);

	do{
		theta = phi = 0;
		if(lbutton_down) {
        	mousedrag(window, &mesh);
    	}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3,0.3,0.5,1);
		scene.render(camera);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& glfwWindowShouldClose(window)==0);

	return 0;
}

