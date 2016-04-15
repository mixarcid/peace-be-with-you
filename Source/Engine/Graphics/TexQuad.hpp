#pragma once

#include "RenderableComp.hpp"
#include "Texture.hpp"
#include "BoundingSphere.hpp"

NAMESPACE {

  struct TexQuadData {
    Vec3f pos;
    Vec2f tex_coord;
  };

  struct TexQuad : RenderableReg {

    Texture tex;
    BoundingSphere b_sphere;

    TexQuad(Texture texture, Vec2f halves);
    virtual void render(RenderContext c);

    const static ShaderFlags SHADER_FLAGS
    = (SHADER_3D | SHADER_USE_TEXTURE);

  };

}
