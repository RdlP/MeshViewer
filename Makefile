CC = g++
CFLAGS = -g -std=c++11
INCFLAGS = -I./include/
LDFLAGS =  -lglfw -lGLEW -lGLU -lGL

RM = /bin/rm -f
BUILDDIR = build
APPNAME = MeshViewer

all: createBuild MeshViewer
createBuild:
	mkdir -p $(BUILDDIR)

MeshViewer: main.o Mesh.o Ray.o Scene.o Camera.o PerspectiveCamera.o OrthographicCamera.o
	$(CC) $(CFLAGS) -o $(APPNAME) $(BUILDDIR)/main.o $(BUILDDIR)/Mesh.o $(BUILDDIR)/Ray.o $(BUILDDIR)/Scene.o $(BUILDDIR)/PerspectiveCamera.o $(BUILDDIR)/OrthographicCamera.o $(BUILDDIR)/Camera.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -o $(BUILDDIR)/main.o -c main.cpp  
Mesh.o: src/Mesh.cpp include/Mesh.hpp
	$(CC) $(CFLAGS) $(INCFLAGS) -o $(BUILDDIR)/Mesh.o -c src/Mesh.cpp
Ray.o: src/Ray.cpp include/Ray.hpp
	$(CC) $(CFLAGS) $(INCFLAGS) -o $(BUILDDIR)/Ray.o -c src/Ray.cpp
Scene.o: src/Scene.cpp include/Scene.hpp
	$(CC) $(CFLAGS) $(INCFLAGS) -o $(BUILDDIR)/Scene.o -c src/Scene.cpp
Camera.o: src/Camera.cpp include/Camera.hpp
	$(CC) $(CFLAGS) $(INCFLAGS) -o $(BUILDDIR)/Camera.o -c src/Camera.cpp
PerspectiveCamera.o: src/PerspectiveCamera.cpp include/PerspectiveCamera.hpp
	$(CC) $(CFLAGS) $(INCFLAGS) -o $(BUILDDIR)/PerspectiveCamera.o -c src/PerspectiveCamera.cpp
OrthographicCamera.o: src/OrthographicCamera.cpp include/OrthographicCamera.hpp
	$(CC) $(CFLAGS) $(INCFLAGS) -o $(BUILDDIR)/OrthographicCamera.o -c src/OrthographicCamera.cpp

clean: 
	$(RM) $(BUILDDIR)/*.o $(APPNAME)
	rmdir $(BUILDDIR)


 
