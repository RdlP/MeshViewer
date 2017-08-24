#version 410 core
in vec2 UV;
out vec3 color;
//uniform sampler2D sampler;
void main(){
  //color = vec3(UV,1);
  color = vec3(1,0,0);
  //color = texture(sampler,UV).rgb;
}