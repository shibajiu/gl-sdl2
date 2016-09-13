#include "stdafx.h"
#include "SDLGL.h"


SDLGL::SDLGL() {
}


SDLGL::~SDLGL() {
}

void SDLGL::SetWindowSize_sdl(int _h, int _w) {
	sdl_height = _h;
	sdl_width = _w;
}

void SDLGL::SetTitle_sdl(char *_t) {
	sdl_title = _t;
}

int SDLGL::Init_sdl() {
	if (SDL_Init(sdl_init_flags) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n",
			SDL_GetError());
		return -1;
	}
	sdl_window = SDL_CreateWindow("s", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sdl_height, sdl_width, SDL_WINDOW_OPENGL);
	if (!sdl_window) {
		fprintf(stderr, "Create Window failed: %s\n",
			SDL_GetError());
		return -1;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	sdl_context = SDL_GL_CreateContext(sdl_window);
	if (!sdl_context) {
		fprintf(stderr, "Create Context failed: %s\n",
			SDL_GetError());
		return -1;
	}
	//disable VSync
	SDL_GL_SetSwapInterval(0);
	glewExperimental = true;
	var err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "GLEW initialization failed: %s\n",
			glewGetErrorString(err));
		return -1;
	}
}

GLuint SDLGL::CreateProgram_sdl_s(const char *_v, const char *_f) {
	GLuint _vshader, _fshader, _program;
	_vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vshader, 1, &_v, NULL);
	glCompileShader(_vshader);
	PrintShaderLog_sdl(_vshader);

	_fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fshader, 1, &_f, NULL);
	glCompileShader(_fshader);
	PrintShaderLog_sdl(_fshader);

	_program = glCreateProgram();
	glAttachShader(_program, _vshader);
	glAttachShader(_program, _fshader);
	glLinkProgram(_program);
	PrintProgramLog_sdl(_program);
	glDetachShader(_program, _vshader);
	glDetachShader(_program, _fshader);
	return GLuint(_program);
}

void SDLGL::CreateProgram_sdl(const char *_v, const char *_f) {
	CreateProgram_sdl_s(_v, _f);
}

GLuint SDLGL::LoadShader_sdl_s(const char * _vpath, const char * _fpath) {
	string vshader_f_source, fshader_f_source, temp;
	auto shaderfstream = ifstream(_vpath, ios::in);
	if (shaderfstream.is_open()) {
		while (getline(shaderfstream, temp)) {
			vshader_f_source += temp + "\n";
		}
	}
	shaderfstream.close();

	shaderfstream = ifstream(_fpath);
	if (shaderfstream.is_open()) {
		while (getline(shaderfstream, temp)) {
			fshader_f_source += temp + "\n";
		}
	}
	shaderfstream.close();
	return CreateProgram_sdl_s(vshader_f_source.c_str(), fshader_f_source.c_str());
}

void SDLGL::LoadShader_sdl(const char * _vpath, const char * _fpath) {
	
}

int SDLGL::Handle_Event_sdl(SDL_Event * _e) {
	switch (_e->type) {
	case SDL_QUIT:
		return SDL_QUIT;
	case SDL_MOUSEBUTTONDOWN:

	default:
		break;
	}
}

int SDLGL::MouseDownHandler_sdl(SDL_MouseButtonEvent *) {
	return 0;
}

int SDLGL::MouseMoveHandler_sdl(SDL_MouseMotionEvent *) {
	return 0;
}

void SDLGL::PrintShaderLog_sdl(GLuint _s) {
	//Make sure name is shader
	if (glIsShader(_s)) {
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		//Get info string length
		glGetShaderiv(_s, GL_INFO_LOG_LENGTH, &maxLength);
		//Allocate string
		char* infoLog = new char[maxLength];
		//Get info log
		glGetShaderInfoLog(_s, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0) {
			//Print Log
			printf("%s\n", infoLog);
		}
		//Deallocate string
		delete[] infoLog;
	}
	else {
		printf("Name %d is not a shader\n", _s);
	}
}

void SDLGL::PrintProgramLog_sdl(GLuint _p) {
	if (glIsProgram(_p)) {
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		//Get info string length
		glGetProgramiv(_p, GL_INFO_LOG_LENGTH, &maxLength);
		//Allocate string
		char* infoLog = new char[maxLength];
		//Get info log
		glGetProgramInfoLog(_p, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0) {
			//Print Log
			printf("%s\n", infoLog);
		}
		//Deallocate string
		delete[] infoLog;
	}
	else {
		printf("Name %d is not a program\n", _p);
	}
}

void objload::load_obj(const char* path, GLuint flag, GLuint method) {
	ifstream obj(path);
	if (!obj.is_open()) {
		fprintf_s(stderr, "cannot open file\n");
		return;
	}
	char buffer[128];
	float f1, f2, f3;
	GLuint i0[3], i1[3], i2[3];
	while (!obj.eof()) {
		obj.getline(buffer, 128, '\n');
		if (buffer[0] == 'v' && (buffer[1] == ' ' || buffer[1] == 32)) {
			if (sscanf_s(buffer, "v %f %f %f", &f1, &f2, &f3) == 3) {
				Vertices.push_back(Vertex(f1, f2, f3));
			}
			else {
				fprintf(stderr, "ERROR: vertex not in wanted format in OBJLoader\n");
			}
		}
		else if (buffer[0] == 'f' && (buffer[1] == ' ' || buffer[1] == 32)) {
			if (sscanf_s(buffer, "f %d %d %d", &i0[0], &i0[1], &i0[2]) == 3) {
				Indices.push_back((flag == FILE_VESSEL) ? FaceIndex(i0, 1) : FaceIndex(i0));
			}
			else if (sscanf_s(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&i0[0], &i1[0], &i2[0],
				&i0[1], &i1[1], &i2[1],
				&i0[2], &i1[2], &i2[2]) == 9) {
				i0[0] -= 1, i0[1] -= 1, i0[2] -= 1;
				Indices.push_back(FaceIndex(i0, i1, i2));
			}
			else {
				fprintf(stderr, "ERROR: FaceIndex not in wanted format in OBJLoader\n");
			}
		}
		else if (buffer[0] == 'n' && (buffer[1] == ' ' || buffer[1] == 32)) {
			if (sscanf_s(buffer, "n %f %f %f", &f1, &f2, &f3) == 3) {
				Normals.push_back(Normal(f1, f2, f3));
			}
			else {
				fprintf(stderr, "ERROR: Normal not in wanted format in OBJLoader\n");
			}
		}
		else if ((buffer[0] == 'v' && buffer[1] == 'n') && (buffer[2] == ' ' || buffer[2] == 32)) {
			if (sscanf_s(buffer, "vn %f %f %f", &f1, &f2, &f3) == 3) {
				Normals.push_back(Normal(f1, f2, f3));
			}
			else {
				fprintf(stderr, "ERROR: Normal not in wanted format in OBJLoader\n");
			}
		}
		else if ((buffer[0] == 'v' && buffer[1] == 't') && (buffer[2] == ' ' || buffer[2] == 32)) {
			if (sscanf_s(buffer, "vt %f %f", &f1, &f2) == 2) {
				Textures.push_back(Texture(f1, f2));
			}
			else {
				fprintf(stderr, "ERROR: Texture not in wanted format in OBJLoader\n");
			}
		}
	}
	obj.close();
}


GLint objload::load_obj_struct_itr(const char* path, GLuint flag, GLuint method) {

	load_obj(path, flag);
	int iv = 0;
	printf_s("Loading obj file...\n");
	if (!Vertices.size() || !Indices.size())
		goto jump1;


	obj_vertexbuffer_size = Vertices.size() * 3;
	obj_vertexbuffer = new float[obj_vertexbuffer_size];
	for (vector<Vertex>::const_iterator ite = Vertices.begin(); ite != Vertices.end(); ++ite) {
		obj_vertexbuffer[iv * 3] = ite->v[0];
		obj_vertexbuffer[iv * 3 + 1] = ite->v[1];
		obj_vertexbuffer[iv * 3 + 2] = ite->v[2];
		iv++;
	}
	printf_s("vertexbuffer loaded\n");

	obj_normalbuffer_size = Normals.size() * 3;
	obj_normalbuffer = new float[obj_normalbuffer_size];
	iv = 0;
	for (vector<Normal>::const_iterator ite = Normals.begin(); ite != Normals.end(); ++ite) {
		obj_normalbuffer[iv * 3] = ite->n[0];
		obj_normalbuffer[iv * 3 + 1] = ite->n[1];
		obj_normalbuffer[iv * 3 + 2] = ite->n[2];
		iv++;
	}
	printf_s("normalbuffer loaded\n");
	printf_s("obj file loaded\n");


	iv = 0;
	obj_indexbuffer_size = Indices.size() * 3;
	obj_indexbuffer = new GLuint[obj_indexbuffer_size];
	for (vector<FaceIndex>::const_iterator ite = Indices.begin(); ite != Indices.end(); ++ite) {
		obj_indexbuffer[iv * 3] = ite->a;
		obj_indexbuffer[iv * 3 + 1] = ite->b;
		obj_indexbuffer[iv * 3 + 2] = ite->c;
		iv++;
	}
jump1:
	printf_s("indexbuffer loaded: vertices:%d,normals:%d,faces:%d\n", Vertices.size(), Normals.size(), Indices.size());
	return (!Vertices.size() || !Indices.size());

}

GLuint objload::creatvao_obj(const char* path, int flag) {
	GLuint vao_l, vbo_l, ebo_l, nbo_l;
	if (load_obj_struct_itr(path, flag)) {
		fprintf_s(stderr, "obj err\n");
		return 0;
	}

	if (flag == FILE_NON) {

		glGenVertexArrays(1, &vao_l);
		glBindVertexArray(vao_l);

		glEnableVertexAttribArray(0);
		glGenBuffers(1, &vbo_l);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_l);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*obj_vertexbuffer_size, obj_vertexbuffer, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

		glGenBuffers(1, &ebo_l);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_l);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*obj_indexbuffer_size, obj_indexbuffer, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
	else if (flag == FILE_WITH_UNSORTED_NORMALS) {//something weird

		glGenVertexArrays(1, &vao_l);
		glBindVertexArray(vao_l);

		glEnableVertexAttribArray(0);
		glGenBuffers(1, &vbo_l);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_l);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, v));

		glGenBuffers(1, &ebo_l);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_l);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(obj_indexbuffer)*obj_indexbuffer_size, obj_indexbuffer, GL_STATIC_DRAW);

		vector<Normal>sequentialNormals(Vertices.size());
		vector<GLushort> temp_index;
		if (!Normals.size())
			goto no_normals;
		printf_s("sorting normals....");
		for (auto ite = Indices.begin(); ite != Indices.end(); ++ite) {
			if (UFIND_INDEX(temp_index, ite->a)) {
				sequentialNormals[ite->a] = Normals[ite->an - 1];
				temp_index.push_back(ite->a);
			}
			if (UFIND_INDEX(temp_index, ite->b)) {
				sequentialNormals[ite->b] = Normals[ite->bn - 1];
				temp_index.push_back(ite->b);
			}
			if (UFIND_INDEX(temp_index, ite->c)) {
				sequentialNormals[ite->c] = Normals[ite->cn - 1];
				temp_index.push_back(ite->c);
			}

			//cout << temp_index.size() << "	";
			if (temp_index.size() == Vertices.size()) {
				printf_s("complete\n");
				break;
			}
		}
		printf_s("sorting complete\n");
		glEnableVertexAttribArray(1);
		glGenBuffers(1, &nbo_l);
		glBindBuffer(GL_ARRAY_BUFFER, nbo_l);
		glBufferData(GL_ARRAY_BUFFER, sequentialNormals.size() * sizeof(Normal), &sequentialNormals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), (void *)offsetof(Normal, n));

	no_normals:
		temp_index.clear();
		sequentialNormals.clear();
		glBindVertexArray(0);
	}

	else if (flag == FILE_VESSEL) {
		glGenVertexArrays(1, &vao_l);
		glBindVertexArray(vao_l);

		glEnableVertexAttribArray(0);
		glGenBuffers(1, &vbo_l);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_l);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, v));

		glGenBuffers(1, &ebo_l);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_l);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(obj_indexbuffer)*obj_indexbuffer_size, obj_indexbuffer, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glGenBuffers(1, &nbo_l);
		glBindBuffer(GL_ARRAY_BUFFER, nbo_l);
		glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normal), &Normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), (void *)offsetof(Normal, n));

		glBindVertexArray(0);
	}
	return vao_l;
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(_L_Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_L_Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(_L_Vertex), (GLvoid*)offsetof(_L_Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(_L_Vertex), (GLvoid*)offsetof(_L_Vertex, TexCoords));
	// Vertex Color
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(_L_Vertex), (GLvoid*)offsetof(_L_Vertex, v_Color));

	glBindVertexArray(0);
}

void A_model::load_model(string _path) {
	Assimp::Importer importer;
	auto scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cerr << "ERR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}
	this->directory = _path.substr(0, _path.find_last_of('/'));
	this->processNode(scene->mRootNode, scene);
}

void A_model::processNode(aiNode* _node, const aiScene* _scene) {
	for (int i = 0; i < _node->mNumMeshes; ++i) {
		auto _aimesh = _scene->mMeshes[_node->mMeshes[i]];
		this->meshes.push_back(processMesh(_aimesh, _scene));
	}
	if (_node->mNumChildren == 0)
		return;
	for (int i = 0; i < _node->mNumChildren; ++i) {
		this->processNode(_node->mChildren[i], _scene);
	}
}

Mesh A_model::processMesh(aiMesh* _aimesh, const aiScene* _scene) {
	vector<_L_Vertex>	_vertices(_aimesh->mNumVertices);
	vector<_L_Texture>	_textures;
	vector<_L_U_INT>	_indices;

	for (int i = 0; i < _aimesh->mNumVertices; ++i) {
		glm::vec3 _v;
		_v.x = _aimesh->mVertices[i].x;
		_v.y = _aimesh->mVertices[i].y;
		_v.z = _aimesh->mVertices[i].z;
		_vertices[i].Position = _v;
		_v.x = _aimesh->mNormals[i].x;
		_v.y = _aimesh->mNormals[i].y;
		_v.z = _aimesh->mNormals[i].z;
		_vertices[i].Normal = _v;
		if (_aimesh->mColors[0]) {
			glm::vec4 _c;
			_c.r = _aimesh->mColors[0][i].r;
			_c.g = _aimesh->mColors[0][i].g;
			_c.b = _aimesh->mColors[0][i].b;
			_c.a = _aimesh->mColors[0][i].a;
			_vertices[i].v_Color = _c;
		}
		if (_aimesh->mTextureCoords[0]) {
			glm::vec2 _v;
			_v.x = _aimesh->mTextureCoords[0][i].x;
			_v.y = _aimesh->mTextureCoords[0][i].y;
			_vertices[i].TexCoords = _v;
		}
		else
			_vertices[i].TexCoords = vec2(0, 0);
	}
	for (int i = 0; i < _aimesh->mNumFaces; ++i) {
		auto face = _aimesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
			_indices.push_back(face.mIndices[j]);
	}

	if (_aimesh->mMaterialIndex >= 0) {
		auto _material = _scene->mMaterials[_aimesh->mMaterialIndex];
		vector<_L_Texture> _t_d = this->loadMaterialTextures(_material, aiTextureType_DIFFUSE, "TextureType_DIFFUSE");
		_textures.insert(_textures.end(), _t_d.begin(), _t_d.end());
		vector<_L_Texture> _t_s = this->loadMaterialTextures(_material, aiTextureType_SPECULAR, "TextureType_SPECULAR");
		_textures.insert(_textures.end(), _t_s.begin(), _t_s.end());
	}

	return Mesh(_vertices, _textures, _indices);
}

vector<_L_Texture> A_model::loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, string _typeName) {
	vector<_L_Texture> _textures;
	bool skip = false;
	for (int i = 0; i < _mat->GetTextureCount(_type); ++i) {
		aiString _s;
		_mat->GetTexture(_type, i, &_s);
		for (int j = 0; j < _loaded.size(); ++j) {
			if (_loaded[j].path == _s) {
				_textures.push_back(_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			_L_Texture _t;
			_t.type = _typeName;
			_t.path = _s;
			_t.id = this->TextureFromFile(_s.C_Str(), this->directory);
			_textures.push_back(_t);
			_loaded.push_back(_t);
		}
	}

	return _textures;
}

_L_U_INT A_model::TextureFromFile(const char* _s, string _p) {
	return 0;
}