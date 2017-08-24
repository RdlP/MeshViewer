#version 410 core

uniform vec3 color_bb;
out vec3 color;

void main(){
  color = color_bb;
}