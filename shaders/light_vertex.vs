#version 410 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

uniform mat3 normalMatrix;
uniform mat4 viewModelMatrix;

out vec3 E, N, P;

//out vec2 UV;

uniform mat4 MVP;

void main(){
  gl_Position = MVP * vec4(vPosition,1);
  vec4 vPos = vec4(vPosition,1);
  P = normalize((viewModelMatrix * vPos).xyz);
  E = normalize(-P);
  N = normalize(normalMatrix * vNormal);
  //UV = vertexUV;
}