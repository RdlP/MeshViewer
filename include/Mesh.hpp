#ifndef MESH_HEADER
#define MESH_HEADER

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
#include "Camera.hpp"

class Mesh{
public:
	Mesh();
	Mesh(char *path);
	virtual ~Mesh();
	void load(char* path);
	void rotateLocalCoordinates(glm::vec3 axis, float angle);
	void rotateXLocalCoordinates(float angle);
	void rotateYLocalCoordinates(float angle);
	void rotateZLocalCoordinates(float angle);
	void rotateWorldCoordinates(glm::vec3 axis, float angle);
	void rotateXWorldCoordinates(float angle);
	void rotateYWorldCoordinates(float angle);
	void rotateZWorldCoordinates(float angle);

	void translateLocalCoordinates(glm::vec3 amount);
	void translateXLocalCoordinates(float amount);
	void translateYLocalCoordinates(float amount);
	void translateZLocalCoordinates(float amount);
	void translateWorldCoordinates(glm::vec3 amount);
	void translateXWorldCoordinates(float amount);
	void translateYWorldCoordinates(float amount);
	void translateZWorldCoordinates(float amount);

	int getVerticesNumber();
	int getTrianglesNumber();
	float getSizeInMemory(char unit='M');
	void showBoundingBox(bool show);
	void showAABB(bool show);
	bool isShowingBoundingBox();
	bool isShowingAABB();
	glm::vec3 getBoundingBoxColor();
	glm::vec3 getAABBColor();
	void setBoundingBoxColor(glm::vec3 color);
	void setAABBColor(glm::vec3 color);
	void render(Camera camera);
	char *getPath();
	glm::mat4 getTransform();
	glm::vec3 getPosition();
	void setPosition(glm::vec3 position);
	bool testRay(Ray ray, float *distance);
	void renderAsWireframe(bool);

	friend std::ostream&  operator << (std::ostream &os, Mesh& m);

private:
	void prepareData();
	void prepareBoundingBoxData();
	void prepareAABBData();
	void render_bb(Camera camera);
	void render_aabb(Camera camera);
	void readFace(std::string face, GLuint *vertex, GLuint *tex, GLuint *normal);
	void readFace(std::string face, std::string *format);
	char *m_path;
	GLuint loadShader(const char * vertex_file_path,const char * fragment_file_path);
	std::vector<GLfloat> m_vertexData, m_vertices, m_normals, m_uvs;
	GLuint m_vbo, m_vbo_bb, m_programID, m_programID_bb, m_vao, m_vao_bb, m_ibo_bb;
	glm::mat4 m_transform, m_transform_bb, m_transform_aabb;/* m_projection, m_view;*/
	std::vector<GLuint> m_indices;
	bool m_showBoundingBox, m_showAABB, m_renderAsWireframe;
	glm::vec3 m_size_bb, m_center_bb, m_color_bb, m_color_aabb, m_size_aabb, m_center_aabb;

};

#endif