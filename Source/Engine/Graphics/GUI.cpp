#include "GUI.hpp"
#include "BoundingObject.hpp"
#include "Graphics.hpp"

NAMESPACE {

  const String GUI::FONT_NAME = "Quicksand";
  
  Asset<Texture> GUI::FONT_TEXTURE_REGULAR(GUI::FONT_NAME + "-Regular");
  Asset<Texture> GUI::FONT_TEXTURE_BOLD(GUI::FONT_NAME + "-Bold");
  Asset<Texture> GUI::FONT_TEXTURE_ITALIC(GUI::FONT_NAME + "-Italic");
  Asset<Texture> GUI::FONT_TEXTURE_BOLD_ITALIC(GUI::FONT_NAME + "-BoldItalic");
  
  const Vec2f GUI::FONT_TEXTURE_SIZE(1028,1028);
  const u8 GUI::FONT_TEXTURE_CHARS_PER_LINE = 16;
  const u8 GUI::FONT_TEXTURE_NUM_LINES = 16;
  const Vec2f GUI::FONT_TEXTURE_CHAR_SIZE
    (1/((f32)FONT_TEXTURE_CHARS_PER_LINE),
     1/((f32)FONT_TEXTURE_NUM_LINES));

  
  GUIElemData::GUIElemData(Vec2s _pos, Vec2f _tex_coord)
    : pos(_pos), tex_coord(_tex_coord) {}
    
  GUIElem::GUIElem(Array<GUIElemData> _data,
		   Array<u32> _elems,
		   Texture* _tex,
		   u8 _z_val)
    : RenderableReg(SHADER_2D),
    data(_data),
    elements(_elems),
    tex(_tex),
    z_val(_z_val) {}

  void GUIElem::init() {
    RenderableReg::init();
    updateBuffers();
  }

  void GUIElem::updateBuffers() {
    Shader::setFlags(RenderableComp::shader_flags);
    RenderableReg::vao.use();
    RenderableReg::vbo.bindArray(data, false);
    RenderableReg::ebo.bindArray(elements, false);
    RenderableReg::vao.registerVars({Shader::POSITION_2D_SHORT,
	  Shader::TEX_COORD});
  }

  void GUIElem::render(RenderContext c) {
    Shader::setFlags(RenderableComp::shader_flags);
    tex->use();
    RenderableReg::render(c);
  }

  bool GUIElem::compare(GUIElem* a, GUIElem* b) {
    return a->z_val > b->z_val;
  }

  GUINode::GUINode(GUIFloatPos _float_pos, u8 _z_val)
    : float_pos(_float_pos), z_val(_z_val) {}
  
  GUINode** GUINode::addChild(GUINode* child) {
    return children.insertSorted(child,
				 GUINode::compare);
  }
  GUIElem** GUINode::addElem(GUIElem* elem) {
    return elems.insertSorted(elem,
			      GUIElem::compare);
  }
  void GUINode::removeChild(GUINode** h) {
    children.removeAndReplace(h);
  }
  void GUINode::removeElem(GUIElem** h) {
    elems.removeAndReplace(h);
  }
  /* |  0  1  2  3 |
     |  4  5  6  7 |
     |  8  9 10 11 |
     | 12 13 14 15 | */
  
  void GUINode::render(RenderContext c, Mat4f model) {
    Mat4f transform = model;
    switch (float_pos) {
    case GUI_FLOAT_TOP:
      transform *=
	Mat4f::scaleTrans(Vec3f(1,1,1),
			  Vec3f(0,1,0));
      break;
    case GUI_FLOAT_BOTTOM:
      transform *= Mat4f::scaleTrans(Vec3f(1,1,1),
				    Vec3f(0,-1,0));
      break;
    case GUI_FLOAT_LEFT:
      transform *= Mat4f::scaleTrans(Vec3f(1,1,1),
				    Vec3f(-1,0,0));
      break;
    case GUI_FLOAT_RIGHT:
      transform *= Mat4f::scaleTrans(Vec3f(1,1,1),
				    Vec3f(1,0,0));
      break;
    case GUI_FLOAT_TOP_LEFT:
      transform *= Mat4f::scaleTrans(Vec3f(1,1,1),
				     Vec3f(-1,1,0));
      break;
    case GUI_FLOAT_TOP_RIGHT:
      transform *= Mat4f::scaleTrans(Vec3f(1,1,1),
				     Vec3f(1,1,0));
      break;
    case GUI_FLOAT_BOTTOM_LEFT:
      transform *= Mat4f::scaleTrans(Vec3f(1,1,1),
				     Vec3f(-1,-1, 0));
      break;
    case GUI_FLOAT_BOTTOM_RIGHT:
      transform *= Mat4f::scaleTrans(Vec3f(1,1,1),
				     Vec3f(1,-1,0));
      break;
    case GUI_FLOAT_CENTER:
      break;
    }
    for (GUINode* child : children) {
      child->render(c, transform);
    }
    Shader::UNI_MODEL.registerVal(transform);
    for (GUIElem* elem : elems) {
      elem->render(c);
    }
  }

  bool GUINode::compare(GUINode* a, GUINode* b) {
    return a->z_val < b->z_val;
  }

}
