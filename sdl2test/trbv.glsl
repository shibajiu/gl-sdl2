#version 400 core
layout(location=0) in vec3 vertices;
layout(location=1) in vec3 icolor;
uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;
out vec3 c;
void main(){
	gl_Position=mat_projection*mat_view*mat_model*vec4(vertices,1);
	c=icolor;
}