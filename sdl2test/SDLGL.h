#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <gtx/transform.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;
using namespace glm;

#define GLM_FORCE_RADIANS
#define var auto
#define UFIND_INDEX(x,y) find(x.begin(),x.end(),y)==x.end()

class SDLGL {

public:
	SDLGL(int _w = 800, int _h = 600, Uint32 _f = SDL_INIT_VIDEO | SDL_INIT_TIMER);
	SDLGL(const SDLGL&) = delete;
	const SDLGL& operator=(const SDLGL&) = delete;
	SDLGL(SDLGL&&);
	const SDLGL& operator=(const SDLGL&&);
	~SDLGL();

	//call this before Init
	void SetWindowSize_sdl(int, int);

	//call this before Init
	void SetTitle_sdl(char*);

	//use SDL_GL_SetAttribute before call this
	int Init_sdl();

	//two way to create program
	static GLuint CreateProgram_sdl_s(const char*, const char*);
	static GLuint CreateProgram_sdl_s(const char*, const char*, const char*);
	void CreateProgram_sdl(const char*, const char*);
	static GLuint LoadShader_sdl_s(const char* _vpath, const char* _fpath);
	static GLuint LoadShader_sdl_s(const char* _vpath,const char* _gpath, const char* _fpath);
	void LoadShader_sdl(const char* _vpath, const char* _fpath);
	mat4 GetRotateMat_sdl();
	//This func is only for test
	mat4 GetRotateMat_sdl(vec3 _o);
	vec3 GetHandPos_sdl();
	//handle sdl event
	virtual int ProcessEvent_sdl();
	//sample render test,generator must be called before game loop
	virtual int RenderTestGenerator_sdl();
	virtual int RenderTest_sdl(GLuint);

	SDL_Window* GetWindow_sdl() { return sdl_window; }

protected:

	SDL_GLContext sdl_context;
	Uint32 sdl_init_flags;
	int sdl_height, sdl_width;
	SDL_Event sdl_event;
	bool sdl_ispressed, sdl_isfirstlosthand = true;
	SDL_Window *sdl_window;
	char* sdl_title;
	GLuint *sdl_program;
	vec3 sdl_trkb_center = vec3(0);
	//Square of the trackball's radius
	GLfloat sdl_trkb_radius_sqr;
	vec3 sdl_trkb_start = vec3(0), sdl_trkb_destination = vec3(0);
	//Basic rotation matrix
	mat4 sdl_trkb_mat = mat4(1);
	//Working rotation matrix
	mat4 sdl_trkb_matnow = mat4(1);

	//Map mouse to trackball point
	vec3 get_trackball_pos_sdl(float, float);

	quat get_trackball_quat_sdl(vec3 _s, vec3 _d);

	//
	virtual int MouseClickHandler_sdl(SDL_MouseButtonEvent *);

	//
	virtual int MouseMoveHandler_sdl(SDL_MouseMotionEvent *);

	static void PrintShaderLog_sdl(GLuint);
	static void PrintProgramLog_sdl(GLuint);
};

typedef unsigned int _L_U_INT;

struct _L_Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec4 v_Color;//color of single vertex
};

struct _L_Texture {
	_L_U_INT id;
	string type;
	aiString path;
};

enum FILE_l { FILE_NON, FILE_V, FILE_OBJ, FILE_VSHADER, FILE_FSHADER, FILE_PROGRAM, FILE_INDEX_MINUS_ONE, FILE_WITH_UNSORTED_NORMALS, FILE_VESSEL };

class objload {
protected:

	struct Vertex {//vertex
		GLfloat v[3];
		Vertex() {}
		Vertex(float xi, float yi, float zi) {
			v[0] = xi, v[1] = yi, v[2] = zi;
		}
	};

	struct FaceIndex {//face index
		GLuint a, b, c;
		GLuint auv, buv, cuv;
		GLuint an, bn, cn;

		FaceIndex() {}

		FaceIndex(GLuint x, GLuint y, GLuint z) :a(x), b(y), c(z) {}

		FaceIndex(GLuint x[3]) :a(x[0] - 1), b(x[1] - 1), c(x[2] - 1) {}

		FaceIndex(GLuint x[3], int) :a(x[0]), b(x[1]), c(x[2]) {}//vessel, donnot need minus

		FaceIndex(GLuint x[3], GLuint y[3], GLuint z[3]) :
			a(x[0]), b(x[1]), c(x[2]),
			auv(y[0]), buv(y[1]), cuv(y[2]),
			an(z[0]), bn(z[1]), cn(z[2]) {
		}
	};

	struct Normal {//normal
		float n[3];
		Normal() {}
		Normal(float a, float b, float c) {
			n[0] = a, n[1] = b, n[2] = c;
		}
	};

	struct Texture {//Texture
		float u, v;
		Texture() {}
		Texture(float a, float b) :u(a), v(b) {}
	};

	vector<Vertex> Vertices;
	vector<FaceIndex> Indices;
	vector<Normal> Normals;
	vector<Texture> Textures;

	GLuint* obj_indexbuffer;
	float* obj_vertexbuffer, *obj_normalbuffer;
	int obj_indexbuffer_size, obj_vertexbuffer_size, obj_normalbuffer_size;

	void load_obj(const char* path, GLuint flag = FILE_NON, GLuint method = FILE_NON);
	GLint load_obj_struct_itr(const char* path, GLuint flag = FILE_V, GLuint method = FILE_NON);

//public:

	GLuint creatvao_obj(const char* path, int flag = FILE_V);
	GLuint getElementNum() { return obj_indexbuffer_size; }

	~objload() {
		Vertices.clear();
		Indices.clear();
		Normals.clear();
		delete[] obj_vertexbuffer;
		delete[] obj_indexbuffer;
		delete[] obj_normalbuffer;
	}
};


class Mesh {
private:
	vector<_L_Vertex>	vertices;
	vector<_L_Texture>	textures;
	vector<_L_U_INT>	indices;

	_L_U_INT VAO, VBO, EBO;

	void setupMesh();

public:
	Mesh() = default;
	Mesh::Mesh(vector<_L_Vertex> _ve, vector<_L_Texture>_te, vector<_L_U_INT>_in) {
		this->vertices = _ve;
		this->textures = _te;
		this->indices = _in;

		setupMesh();
	}
	~Mesh() {
		this->indices.clear();
		this->textures.clear();
		this->vertices.clear();
	}

};

class A_model:private objload {
private:
	vector<Mesh> meshes;
	vector<_L_Texture>_loaded;
	string directory;

	void load_model(string);
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	vector<_L_Texture> loadMaterialTextures(aiMaterial*, aiTextureType, string);
	_L_U_INT TextureFromFile(const char*, string);

	//should not copy this
	A_model(A_model const&) = delete;
	A_model const& operator=(A_model const&) = delete;

public:
	A_model(char* _path) {
		load_model(_path);
	}
	using objload::creatvao_obj;
};
