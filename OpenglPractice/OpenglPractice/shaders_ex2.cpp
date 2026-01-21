#include "shaders.h"
#include "2dshapes_definitions.h"
#include "filesystem"

int shaders_ex2() {
	std::filesystem::path vShaderPath = std::filesystem::current_path() / "vertex_shader.vert";
	std::filesystem::path fShaderPath = std::filesystem::current_path() / "fragment_shader.frag";
	
	Shader s(vShaderPath, fShaderPath);

	return 1;
}

