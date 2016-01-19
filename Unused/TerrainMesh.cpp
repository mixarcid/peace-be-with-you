#include "TerrainMesh.hpp"

NAMESPACE {

  const ShaderFlags TerrainMesh::FLAGS = SHADER_NO_FLAGS;

  TerrainMesh::TerrainMesh(EBO* _ebo,
			   Texture* texture)
    : Renderable(TerrainMesh::FLAGS),
    ebo(_ebo),
    tex(texture) {}
  
  void TerrainMesh::init() {
    Shader::setFlags(this->shader_flags);
    vao.init();
    vao.use();
    vbo.init();
    vbo.bindArray(data, false);
    vao.registerVars({Shader::POSITION,
	  Shader::NORMAL,
	  Shader::TEX_COORD});
  }

  void TerrainMesh::render(RenderContext c) {
    Shader::setFlags(this->shader_flags);
    tex->use();
    vao.use();
    ebo->draw();
  }

}
