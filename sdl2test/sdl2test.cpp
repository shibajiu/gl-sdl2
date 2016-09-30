// sdl2test.cpp : Defines the entry point for the console application.
//
//gllinewidth
#include "stdafx.h"
void s(ovrrs_tc *_r) {
	_r->Start();
}

void s_fh(ovrrs_fh* _f) {
	_f->Start();
}

int main(int argc, char* argv[]) {
	//ovrrs_tc *rs=new ovrrs_tc();
	ovrrs_fh *rsf = new ovrrs_fh();
	thread rsthrd = thread(s_fh,rsf);
	var kal = new KalmanFilter<vec3>(vec3(1.f,1.f,1.f),vec3(1.f,1.f,1.f),vec3(1.f,1.f,1.f));
	//Use this to wait for init a glm*, DANGEROUS!!!!@
	Sleep(100);
	SDLGL s(1200, 800);
	SDL_Event *e = new SDL_Event();
	var a = s.RenderTestGenerator_sdl();
	var window = s.GetWindow_sdl();
	var program = s.LoadShader_sdl_s("trbv.glsl", "trbf.glsl");
	var handprogram = s.LoadShader_sdl_s("hv.glsl","hg.glsl", "hf.glsl");
	mat4 mat_model, mat_view, mat_projection;
	vec3 handpos;
	vec3 eyepos=vec3(0,0,2), eyefront=vec3(0,0,-1), eyeup=vec3(0,1,0);

	var _ve = rsf->GetJointPoints();
	GLuint _v,_a;
	//var p = s.GetHandPos_sdl();
	glGenVertexArrays(1, &_a);
	glBindVertexArray(_a);
	glGenBuffers(1, &_v);
	glBindBuffer(GL_ARRAY_BUFFER, _v);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*MAX_NUMBER_OF_JOINTS, &_ve[0],GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vec3), 0);
	glBindVertexArray(0);

	glClearColor(0.1, 0.2, 0.3, 1);
	glEnable(GL_DEPTH_TEST);

	while (1) {
		if (s.ProcessEvent_sdl() == 0)
			break;
		mat_view = lookAtRH(eyepos, eyepos + eyefront, eyeup);
		mat_projection = perspectiveRH(radians(60.f), 4.f / 3, 0.1f, 50.f);
		var _o =kal->Process(rsf->GetWristOrientation());
		mat_model = s.GetRotateMat_sdl(_o);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "mat_model"), 1, false, value_ptr(mat_model));
		glUniformMatrix4fv(glGetUniformLocation(program, "mat_view"), 1, false, value_ptr(mat_view));
		glUniformMatrix4fv(glGetUniformLocation(program, "mat_projection"), 1, false, value_ptr(mat_projection));
		s.RenderTest_sdl(a);
		glUseProgram(0);

		var mat_model1 = mat4(1);
		var mat_view1 = lookAt(vec3(0, 0, 1), vec3(0, 0, 0), vec3(0, 1, 0));
		mat4x4 mat_projection1 = mat_projection;
		glDisable(GL_DEPTH_TEST);
		glUseProgram(handprogram);
		glUniformMatrix4fv(glGetUniformLocation(program, "mat_model"), 1, false, value_ptr(mat_model1));
		glUniformMatrix4fv(glGetUniformLocation(program, "mat_view"), 1, false, value_ptr(mat_view1));
		glUniformMatrix4fv(glGetUniformLocation(program, "mat_projection"), 1, false, value_ptr(mat_projection1));

		glBindVertexArray(_a);
		/*p = s.GetHandPos_sdl();
		vec3 _gh = rs->getHandPos();
		_gh.y = 1 - _gh.y;
		var _h = vec3(2) *_gh - vec3(1);
		_gh -= vec3(0.5, 0.5, 0.5);
		var _hn = normalize(_gh);*/
		_ve = rsf->GetJointPoints();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*MAX_NUMBER_OF_JOINTS*3, &_ve[0]);
		glDrawArrays(GL_POINTS, 0, MAX_NUMBER_OF_JOINTS);
		/*glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3), value_ptr(_h));
		glDrawArrays(GL_POINTS, 0, 1);
		var _t = rs->GetAlert();*/
		//cout << _t << endl;
		glBindVertexArray(0);
		glUseProgram(0);
		glEnable(GL_DEPTH_TEST);

		SDL_GL_SwapWindow(window);
	}
	delete e;
	//system("pause");
	//rs->Stop();
	rsf->Stop();
	rsthrd.join();
	SDL_Quit();
	return 0;
}