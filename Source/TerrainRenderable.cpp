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

    Terrain::pos_buffer.use();
    vao.registerVars({Shader::POSITION_TERRAIN});

    height_vbo.init();
    height_vbo.bindArray(height_data, false);
    vao.registerVars({Shader::HEIGHT});

    normal_vbo.init();
    normal_vbo.bindArray(normal_data, false);
    vao.registerVars({Shader::NORMAL});

    biome_vbo.init();
    biome_vbo.bindArray(biome_data, false);
    vao.registerVars({Shader::BIOME_DATA});
    
  }

  static const f32 DIST_MULTIPLIER_LARGE = 0.01;
  static const f32 DIST_MULTIPLIER_SMALL = 1.6;
  void TerrainRenderable::render(RenderContext c) {
    Shader::UNI_TEXTURE.registerTexture(tex);
    vao.use();
    f32 dist = c.dist.norm();
    if (dist > DIST_MULTIPLIER_LARGE
	*sqr(Terrain::CHUNK_SIZE)) {
      Terrain::elem_buffer_small.draw(c.instances);
    } else if (dist > DIST_MULTIPLIER_SMALL
	       *Terrain::CHUNK_SIZE) {
      Terrain::elem_buffer_mid.draw(c.instances);
    } else {
      Terrain::elem_buffer_large.draw(c.instances);
    }
  }

}
