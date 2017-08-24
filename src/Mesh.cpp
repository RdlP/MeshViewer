#include "Mesh.hpp"

Mesh::Mesh():m_showBoundingBox(false), m_showAABB(false), m_renderAsWireframe(false){
	m_color_bb = glm::vec3(1,1,1);
	m_color_aabb = glm::vec3(1,1,1);
}

Mesh::Mesh(char* path):m_path(path), m_showBoundingBox(false), m_showAABB(false), m_renderAsWireframe(false){
	load(m_path);
	m_color_bb = glm::vec3(1,1,1);
	m_color_aabb = glm::vec3(1,1,1);
}

Mesh::~Mesh(){
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_vbo_bb);
	glDeleteBuffers(1, &m_ibo_bb);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteVertexArrays(1, &m_vao_bb);
}

void Mesh::readFace(std::string face, GLuint *vertex, GLuint *tex, GLuint *normal){
	*vertex = *tex = *normal = 0;
	int pos1=0,pos2=0;
	for (int i = 0; i < face.size(); i++){
		char c = face.at(i);
		if (c == '/'){
			if (pos1 == 0){
				pos1 = i;
			}else{
				pos2 = i;
			}
		}
	}
	if (pos1 == 0){
		//only vertex
		std::stringstream ss(face);
		ss >> *vertex;
	}else{
		if (pos2 == pos1+1){
			//vertex and normal
			std::stringstream ss(face.substr(0,pos1));
			ss >> *vertex;
			ss.str(face.substr(pos2+1,face.size()));
			std::cout << ss.str() << std::endl;
			ss.clear();
			ss >> *normal;
		}else{
			//vertex texture and normal
			std::stringstream ss(face.substr(0,pos1));
			ss >> *vertex;
			ss.str(face.substr(pos1+1,pos2));
			ss.clear();
			ss >> *tex;
			ss.str(face.substr(pos2+1,face.size()));
			ss.clear();
			ss >> *normal;
		}
	}
}


void Mesh::readFace(std::string face, std::string *format){
	int pos1=0,pos2=0;
	for (int i = 0; i < face.size(); i++){
		char c = face.at(i);
		if (c == '/'){
			if (pos1 == 0){
				pos1 = i;
			}else{
				pos2 = i;
			}
		}
	}
	if (pos1 == 0){
		//only vertex
		*format = "x";
	}else{
		if (pos2 == pos1+1){
			//vertex and normal
			*format = "x//x";
		}else{
			//vertex texture and normal
			*format = "x/x/x";
		}
	}
}


void Mesh::load(char* path){
	std::string line;
	std::ifstream obj_file(path);
	std::vector<GLfloat> vs, ns, ts;
	float xmin,xmax, ymin,ymax, zmin,zmax;
	xmin = ymin = zmin = std::numeric_limits<float>::max();
	xmax = ymax = zmax = -std::numeric_limits<float>::max();
	std::string formatFace("");
	if (obj_file.is_open()){
		m_path = path;
		std::vector<GLuint> normalIndices;
		std::vector<GLuint> uvIndices;
		while(getline(obj_file, line)){
			std::stringstream ss(line);
			std::string dummy;
			
			ss >> dummy;
			if (line.substr(0,2) == "v "){
				GLfloat v1,v2,v3;
				ss >> v1;
				ss >> v2;
				ss >> v3;
				vs.push_back(v1);
				vs.push_back(v2);
				vs.push_back(v3);
				if (v1 < xmin){
					xmin = v1;
				}
				if (v2 < ymin){
					ymin = v2;
				}
				if (v3 < zmin){
					zmin = v3;
				}

				if (v1 > xmax){
					xmax = v1;
				}
				if (v2 > ymax){
					ymax = v2;
				}
				if (v3 > zmax){
					zmax = v3;
				}
			}else if(line.substr(0,2) == "f "){
				GLuint fi1,fi2,fi3,fi4;
				GLuint fn1,fn2,fn3,fn4;
				GLuint fuv1, fuv2, fuv3;
				std::stringstream ss1(line);
				std::string dummy;
				ss1 >> dummy;
				ss1 >> dummy;
				if (formatFace == ""){
					readFace(dummy, &formatFace);
				}
				if (formatFace == "x"){
					// only vertex
					sscanf(line.c_str(),"f %d %d %d",&fi1,&fi2,&fi3);
					m_indices.push_back(fi1-1);
					m_indices.push_back(fi2-1);
					m_indices.push_back(fi3-1);
				}else if (formatFace == "x//x"){
					sscanf(line.c_str(),"f %d//%d %d//%d %d//%d",&fi1,&fn1,&fi2,&fn2,&fi3,&fn3);
					m_indices.push_back(fi1-1);
					m_indices.push_back(fi2-1);
					m_indices.push_back(fi3-1);

					normalIndices.push_back(fn1-1);
					normalIndices.push_back(fn2-1);
					normalIndices.push_back(fn3-1);
				}else if (formatFace == "x/x/x"){
					m_indices.push_back(fi1-1);
					m_indices.push_back(fi2-1);
					m_indices.push_back(fi3-1);

					uvIndices.push_back(fuv1-1);
					uvIndices.push_back(fuv2-1);
					uvIndices.push_back(fuv3-1);

					normalIndices.push_back(fn1-1);
					normalIndices.push_back(fn2-1);
					normalIndices.push_back(fn3-1);
				}				
			}else if (line.substr(0,2) == "vn"){
				GLfloat n1,n2,n3;
				ss >> n1;
				ss >> n2;
				ss >> n3;
				ns.push_back(n1);
				ns.push_back(n2);
				ns.push_back(n3);
			}else if (line.substr(0,2) == "vt"){
				GLfloat uv1, uv2;
				ss >> uv1;
				ss >> uv2;
				ts.push_back(uv1);
				ts.push_back(uv2);
			}
			
		}
		obj_file.close();
		for (int i = 0; i < m_indices.size(); i++){
			if (vs.size() != 0){
				GLfloat v1 = vs.at(3*m_indices.at(i));
				GLfloat v2 = vs.at(3*m_indices.at(i)+1);
				GLfloat v3 = vs.at(3*m_indices.at(i)+2);
				m_vertices.push_back(v1);
				m_vertices.push_back(v2);
				m_vertices.push_back(v3);
				m_vertexData.push_back(v1);
				m_vertexData.push_back(v2);
				m_vertexData.push_back(v3);
			}

			if (ts.size() != 0){
				GLfloat uv1 = ts.at(3*uvIndices.at(i));
				GLfloat uv2 = ts.at(3*uvIndices.at(i)+1);
				m_uvs.push_back(uv1);
				m_uvs.push_back(uv2);
			}

			if (ns.size() != 0){
				GLfloat n1 = ns.at(3*normalIndices.at(i));
				GLfloat n2 = ns.at(3*normalIndices.at(i)+1);
				GLfloat n3 = ns.at(3*normalIndices.at(i)+2);
				m_normals.push_back(n1);
				m_normals.push_back(n2);
				m_normals.push_back(n3);
				m_vertexData.push_back(n1);
				m_vertexData.push_back(n2);
				m_vertexData.push_back(n3);
			}
		}
	}
	m_size_bb = glm::vec3(xmax-xmin, ymax-ymin, zmax-zmin);
	m_center_bb = glm::vec3((xmax+xmin)/2,(ymax+ymin)/2,(zmax+zmin)/2);
	m_transform_bb = glm::translate(glm::mat4(1),m_center_bb)*glm::scale(glm::mat4(1), m_size_bb);
	prepareData();
}

GLuint Mesh::loadShader(const char * vertex_file_path,const char * fragment_file_path){
	// Crear los shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Leer el Vertex Shader desde archivo
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		return 0;
	}

	// Leer el Fragment Shader desde archivo
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compilar Vertex Shader
	//printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Revisar Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		//printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compilar Fragment Shader
	//printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Revisar Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		//printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Vincular el programa por medio del ID
	//printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Revisar el programa
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		//printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void Mesh::rotateWorldCoordinates(glm::vec3 axis, float angle){
	glm::mat4 rotation;
	glm::mat3 inv = glm::mat3(glm::inverse(m_transform));
	axis = inv * axis;
	rotation = glm::rotate(rotation, angle, axis);
	m_transform *= rotation;
}

void Mesh::rotateXWorldCoordinates(float angle){
	rotateWorldCoordinates(glm::vec3(1,0,0), angle);
}

void Mesh::rotateYWorldCoordinates(float angle){
	rotateWorldCoordinates(glm::vec3(0,1,0), angle);
}

void Mesh::rotateZWorldCoordinates(float angle){
	rotateWorldCoordinates(glm::vec3(0,0,1), angle);
}

void Mesh::rotateLocalCoordinates(glm::vec3 axis, float angle){
	glm::mat4 rotation;
	rotation = glm::rotate(rotation, angle, axis);
	m_transform *= rotation;
}

void Mesh::rotateXLocalCoordinates(float angle){
	rotateLocalCoordinates(glm::vec3(1,0,0), angle);
}

void Mesh::rotateYLocalCoordinates(float angle){
	rotateLocalCoordinates(glm::vec3(0,1,0), angle);
}

void Mesh::rotateZLocalCoordinates(float angle){
	rotateLocalCoordinates(glm::vec3(0,0,1), angle);
}



void Mesh::translateWorldCoordinates(glm::vec3 amount){
	glm::mat4 translate;
	glm::mat3 inv = glm::mat3(glm::inverse(m_transform));
	amount = inv * amount;
	translate = glm::translate(translate, amount);
	m_transform *= translate;
}

void Mesh::translateXWorldCoordinates(float amount){
	translateWorldCoordinates(glm::vec3(amount,0,0));
}

void Mesh::translateYWorldCoordinates(float amount){
	translateWorldCoordinates(glm::vec3(0,amount,0));
}

void Mesh::translateZWorldCoordinates(float amount){
	translateWorldCoordinates(glm::vec3(0,0,amount));
}

void Mesh::translateLocalCoordinates(glm::vec3 amount){
	glm::mat4 translate;
	translate = glm::translate(translate, amount);
	m_transform *= translate;
}

void Mesh::translateXLocalCoordinates(float amount){
	translateLocalCoordinates(glm::vec3(amount,0,0));
}

void Mesh::translateYLocalCoordinates(float amount){
	translateLocalCoordinates(glm::vec3(0,amount,0));
}

void Mesh::translateZLocalCoordinates(float amount){
	translateLocalCoordinates(glm::vec3(0,0,amount));
}

int Mesh::getVerticesNumber(){
	return m_vertices.size()/3;
}

int Mesh::getTrianglesNumber(){
	return m_vertices.size()/3/3;
}

float Mesh::getSizeInMemory(char unit){
	float bytes = m_vertexData.size() * sizeof(GLfloat);
	if (unit == 'B'){
		return bytes;
	}else if (unit == 'K'){
		return bytes/1024;
	}else if (unit == 'M'){
		return bytes/1024/1024;
	}else if (unit == 'G'){
		return bytes/1024/1024/1024;
	}
	return bytes/1024/1024;
}

void Mesh::showBoundingBox(bool show){
	m_showBoundingBox = show;
	if (m_showBoundingBox){
		prepareBoundingBoxData();
	}
}

void Mesh::showAABB(bool show){
	m_showAABB = show;
	if (m_showAABB){
		prepareBoundingBoxData();
	}
}

bool Mesh::isShowingBoundingBox(){
	return m_showBoundingBox;
}

bool Mesh::isShowingAABB(){
	return m_showAABB;
}

void Mesh::prepareBoundingBoxData(){
	m_programID_bb = loadShader("shaders/bb_vertex.vs", "shaders/bb_fragment.fs");
	GLfloat vertices[] = {
	    -0.5, -0.5, -0.5,
	     0.5, -0.5, -0.5,
	     0.5,  0.5, -0.5,
	    -0.5,  0.5, -0.5,
	    -0.5, -0.5,  0.5,
	     0.5, -0.5,  0.5,
	     0.5,  0.5,  0.5,
	    -0.5,  0.5,  0.5,
	};
	glGenVertexArrays(1,&m_vao_bb);
	glBindVertexArray(m_vao_bb);
	glGenBuffers(1, &m_vbo_bb);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_bb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	GLushort elements[] = {
	  0, 1, 2, 3,
	  4, 5, 6, 7,
	  0, 4, 1, 5, 2, 6, 3, 7
	};
	GLuint m_ibo_bb;
	glGenBuffers(1, &m_ibo_bb);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_bb);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::prepareData(){
	m_programID = loadShader( "shaders/light_vertex.vs", "shaders/light_fragment.fs" );
	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size()*sizeof(GLfloat), &m_vertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
	glBindVertexArray(0);
	if (m_showBoundingBox || m_showAABB){
		prepareBoundingBoxData();
	}
}

glm::mat4 Mesh::getTransform(){
	return m_transform;
}

glm::vec3 Mesh::getPosition(){
	return glm::vec3(m_transform[3].x,m_transform[3].y,m_transform[3].z);
}

void Mesh::setPosition(glm::vec3 position){
	m_transform[3].x = position.x;
	m_transform[3].y = position.y;
	m_transform[3].z = position.z;
}

glm::vec3 Mesh::getBoundingBoxColor(){
	return m_color_bb;
}

glm::vec3 Mesh::getAABBColor(){
	return m_color_aabb;
}

void Mesh::setBoundingBoxColor(glm::vec3 color){
	m_color_bb = color;
}

void Mesh::setAABBColor(glm::vec3 color){
	m_color_aabb = color;
}

char * Mesh::getPath(){
	return m_path;
}

void Mesh::renderAsWireframe(bool mode){
	m_renderAsWireframe = mode;
}

void Mesh::render_bb(Camera camera){
	glUseProgram(m_programID_bb);
	glBindVertexArray(m_vao_bb);
	glm::mat4 mvp = camera.getProjection()*camera.getView()*m_transform*m_transform_bb;
	
	GLuint mvpID = glGetUniformLocation(m_programID_bb, "MVP");
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

	GLuint colorID = glGetUniformLocation(m_programID_bb, "color_bb");
	glUniform3fv(colorID, 1, &m_color_bb[0]);
	
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
	glBindVertexArray(0);
}

bool Mesh::testRay(Ray ray, float *distance){
	float tmax = std::numeric_limits<float>::max();
	float tmin = -std::numeric_limits<float>::max();
	float t1,t2;
	glm::mat4 aux = m_transform * m_transform_bb;
	glm::vec3 axisX(aux[0].x,aux[0].y,aux[0].z), axisY(aux[1].x,aux[1].y,aux[1].z), axisZ(aux[2].x,aux[2].y,aux[2].z);
	axisX = glm::normalize(axisX);
	axisY = glm::normalize(axisY);
	axisZ = glm::normalize(axisZ);
	glm::vec3 axis[3] = {axisX, axisY, axisZ};
	
	glm::vec3 center(aux[3].x,aux[3].y,aux[3].z);

	glm::vec3 p = center - ray.getOrigin();
	
	for (int i = 0; i < 3; i++){
		float e = glm::dot(axis[i], p);
		float f = glm::dot(axis[i], ray.getDirection());
		if (f != 0){
			t1 = (e + m_size_bb[i]/2)/f;
			t2 = (e - m_size_bb[i]/2)/f;
			if (t1 > t2){
				std::swap(t1,t2);
			}
			if (t1 > tmin){
				tmin = t1;
			}
			if (t2 < tmax){
				tmax = t2;
			}
			if (tmin > tmax){
				// No intersection
				return false;
			}
			
			// if (tmax < 0){
			// 	// No intersection
			// 	return false;
			// }
		}
	}

	if (tmin > 0){
		// Intersection
		*distance = tmin;
		return true;
	}
}

void Mesh::render_aabb(Camera camera){
	glm::vec4 v[8];
	v[0] = m_transform * glm::vec4(m_center_bb.x - m_size_bb.x/2, m_center_bb.y - m_size_bb.y/2, m_center_bb.z - m_size_bb.z/2,1);
	v[1] = m_transform * glm::vec4(m_center_bb.x + m_size_bb.x/2, m_center_bb.y - m_size_bb.y/2, m_center_bb.z - m_size_bb.z/2,1);
	v[2] = m_transform * glm::vec4(m_center_bb.x - m_size_bb.x/2, m_center_bb.y + m_size_bb.y/2, m_center_bb.z - m_size_bb.z/2,1);
	v[3] = m_transform * glm::vec4(m_center_bb.x - m_size_bb.x/2, m_center_bb.y - m_size_bb.y/2, m_center_bb.z + m_size_bb.z/2,1);
	v[4] = m_transform * glm::vec4(m_center_bb.x - m_size_bb.x/2, m_center_bb.y + m_size_bb.y/2, m_center_bb.z + m_size_bb.z/2,1);
	v[5] = m_transform * glm::vec4(m_center_bb.x + m_size_bb.x/2, m_center_bb.y + m_size_bb.y/2, m_center_bb.z - m_size_bb.z/2,1);
	v[6] = m_transform * glm::vec4(m_center_bb.x + m_size_bb.x/2, m_center_bb.y - m_size_bb.y/2, m_center_bb.z + m_size_bb.z/2,1);
	v[7] = m_transform * glm::vec4(m_center_bb.x + m_size_bb.x/2, m_center_bb.y + m_size_bb.y/2, m_center_bb.z + m_size_bb.z/2,1);

	float xmin,xmax, ymin,ymax, zmin,zmax;

	xmin = ymin = zmin = std::numeric_limits<float>::max();
	xmax = ymax = zmax = -std::numeric_limits<float>::max();

	for (int i = 0; i < 8; i++){
		if (v[i].x < xmin){
			xmin = v[i].x;
		}
		if (v[i].y < ymin){
			ymin = v[i].y;
		}
		if (v[i].z < zmin){
			zmin = v[i].z;
		}

		if (v[i].x > xmax){
			xmax = v[i].x;
		}
		if (v[i].y > ymax){
			ymax = v[i].y;
		}
		if (v[i].z > zmax){
			zmax = v[i].z;
		}
	}
	m_size_aabb = glm::vec3((xmax-xmin), (ymax-ymin), (zmax-zmin));
	m_center_aabb = glm::vec3((xmax+xmin)/2,(ymax+ymin)/2,(zmax+zmin)/2);
	m_transform_aabb = glm::translate(glm::mat4(1),m_center_aabb)*glm::scale(glm::mat4(1), m_size_aabb);

	glUseProgram(m_programID_bb);
	glBindVertexArray(m_vao_bb);
	//glm::mat4 mvp = m_projection*m_view*m_transform_aabb;
	glm::mat4 mvp = camera.getProjection()*camera.getView()*m_transform_aabb;

	GLuint mvpID = glGetUniformLocation(m_programID_bb, "MVP");
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

	GLuint colorID = glGetUniformLocation(m_programID_bb, "color_bb");
	glUniform3fv(colorID, 1, &m_color_aabb[0]);

	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
	glBindVertexArray(0);
}

void Mesh::render(Camera camera){
	glUseProgram(m_programID);
	glBindVertexArray(m_vao);
	if (m_renderAsWireframe){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//glm::mat4 mvp =m_projection*camera.getView()*m_transform;
	glm::mat4 mvp =camera.getProjection()*camera.getView()*m_transform;
	//glm::mat4 mv = m_view*m_transform;
	glm::mat4 mv = camera.getView()*m_transform;
	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(mvp));

	GLuint mvpID = glGetUniformLocation(m_programID, "MVP");
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

	GLuint mvID = glGetUniformLocation(m_programID, "viewModelMatrix");
	glUniformMatrix4fv(mvID, 1, GL_FALSE, &mv[0][0]);

	GLuint normalMatrixID = glGetUniformLocation(m_programID, "normalMatrix");
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, &normalMatrix[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	
	glBindVertexArray(0);

	if (m_renderAsWireframe){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (m_showBoundingBox){
		render_bb(camera);
	}
	if (m_showAABB){
		render_aabb(camera);
	}
	
}

std::ostream& operator<<(std::ostream &os, Mesh& mesh){
	os << mesh.getPath() << " [ Vértices: " << mesh.getVerticesNumber() << " ]" << " [ Triangulos:" << mesh.getTrianglesNumber() << " ] [ Memoria: " << mesh.getSizeInMemory() << " MB ]";
	return os;
}