#version 400 core
out vec4 color;
void main(){
	
	color=vec4(gl_FragDepth*0.2,gl_FragDepth*0.5,gl_FragDepth*0.6,1);

}