#include "TerrainRenderable.hpp"
#include "Terrain.hpp"

NAMESPACE {

  const ShaderFlags TerrainRenderable::SHADER_FLAGS;

  TerrainRenderable::TerrainRenderable()
    : RenderableComp(SHADER_FLAGS),
    tex(*Terrain::texture.get()) {}

  void TerrainRenderable::init() {
    Shader::setFlags(RenderableComp::shader_flags);
    vao.init();
    vao.use();
    vbo.init();
    vbo.bindArray(data, false);
    vao.registerVars({Shader::POSITION,
	  Shader::NORMAL,
	  Shader::TEX_COORD});
  }

  static const f32 DIST_MULTIPLIER_LARGE = 0.1;
  static const f32 DIST_MULTIPLIER_SMALL = 1.5;
  void TerrainRenderable::render(RenderContext c) {
    Shader::setFlags(RenderableComp::shader_flags);
    Shader::UNI_TEXTURE.registerTexture(tex);
    vao.use();
    if (c.dist > DIST_MULTIPLIER_LARGE
	*sqr(Terrain::CHUNK_SIZE)) {
      Terrain::elem_buffer_small.draw();
    } else if (c.dist > DIST_MULTIPLIER_SMALL
	       *Terrain::CHUNK_SIZE) {
      Terrain::elem_buffer_mid.draw();
    } else {
      Terrain::elem_buffer_large.draw();
    }
  }

}
