// sdl2test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char* argv[]) {
	SDLGL s(1200, 800);
	SDL_Event *e = new SDL_Event();
	var a = s.RenderTestGenerator_sdl();
	var window = s.GetWindow_sdl();
	var program = s.LoadShader_sdl_s("trbv.glsl", "trbf.glsl");
	var handprogram = s.LoadShader_sdl_s("hv.glsl","hg.glsl", "hf.glsl");
	mat4 mat_model, mat_view, mat_projection;
	vec3 handpos;
	vec3 eyepos=vec3(0,0,2), eyefront=vec3(0,0,-1), eyeup=vec3(0,1,0);

	GLuint _v,_a;
	var p = s.GetHandPos_sdl();
	glGenVertexArrays(1, &_a);
	glBindVertexArray(_a);
	glGenBuffers(1, &_v);
	glBindBuffer(GL_ARRAY_BUFFER, _v);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vec3),value_ptr(p),GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vec3), 0);
	glBindVertexArray(0);

	glClearColor(0.1, 0.2, 0.3, 1);
	glEnable(GL_DEPTH_TEST);
	while (1) {
		if (s.ProcessEvent_sdl() == 0)
			break;
		mat_view = lookAtRH(eyepos, eyepos + eyefront, eyeup);
		mat_projection = perspectiveRH(radians(60.f), 4.f / 3, 0.1f, 100.f);
		mat_model = s.GetRotateMat_sdl();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "mat_model"), 1, false, value_ptr(mat_model));
		glUniformMatrix4fv(glGetUniformLocation(program, "mat_view"), 1, false, value_ptr(mat_view));
		glUniformMatrix4fv(glGetUniformLocation(program, "mat_projection"), 1, false, value_ptr(mat_projection));
		s.RenderTest_sdl(a);
		glUseProgram(0);

		glDisable(GL_DEPTH_TEST);
		glUseProgram(handprogram);
		glBindVertexArray(_a);
		p = s.GetHandPos_sdl();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3), value_ptr(p));
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
		glUseProgram(0);
		glEnable(GL_DEPTH_TEST);

		SDL_GL_SwapWindow(window);
	}
	delete e;
	//system("pause");
	SDL_Quit();
	return 0;
}