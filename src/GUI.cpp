#include "GUI.hpp"
#include "BoundingObject.hpp"
#include "Graphics.hpp"
#include "Assets.hpp"

NAMESPACE {

  const String GUI::FONT_NAME = "Quicksand";
  Texture* GUI::FONT_TEXTURE_REGULAR = NULL;
  Texture* GUI::FONT_TEXTURE_BOLD = NULL;
  Texture* GUI::FONT_TEXTURE_ITALIC = NULL;
  Texture* GUI::FONT_TEXTURE_BOLD_ITALIC = NULL;
  const Vec2f GUI::FONT_TEXTURE_SIZE(1028,1028);
  const u8 GUI::FONT_TEXTURE_CHARS_PER_LINE = 16;
  const u8 GUI::FONT_TEXTURE_NUM_LINES = 16;
  const Vec2f GUI::FONT_TEXTURE_CHAR_SIZE
    (1/((f32)FONT_TEXTURE_CHARS_PER_LINE),
     1/((f32)FONT_TEXTURE_NUM_LINES));

  static const Asset FONT_TEXTURE_LOADER
    ([]() {
      GUI::FONT_TEXTURE_REGULAR = new Texture();
      GUI::FONT_TEXTURE_REGULAR->use();
      GUI::FONT_TEXTURE_REGULAR->load
	(GUI::FONT_NAME + "-Regular",
	 Shader::UNI_TEXTURE);

      GUI::FONT_TEXTURE_BOLD = new Texture();
      GUI::FONT_TEXTURE_BOLD->use();
      GUI::FONT_TEXTURE_BOLD->load
	(GUI::FONT_NAME + "-Bold",
	 Shader::UNI_TEXTURE);

      GUI::FONT_TEXTURE_ITALIC = new Texture();
      GUI::FONT_TEXTURE_ITALIC->use();
      GUI::FONT_TEXTURE_ITALIC->load
	(GUI::FONT_NAME + "-Italic",
	 Shader::UNI_TEXTURE);

      GUI::FONT_TEXTURE_BOLD_ITALIC = new Texture();
      GUI::FONT_TEXTURE_BOLD_ITALIC->use();
      GUI::FONT_TEXTURE_BOLD_ITALIC->load
	(GUI::FONT_NAME + "-BoldItalic",
	 Shader::UNI_TEXTURE);
    },
      []() {
	delete GUI::FONT_TEXTURE_REGULAR;
	delete GUI::FONT_TEXTURE_BOLD;
	delete GUI::FONT_TEXTURE_ITALIC;
	delete GUI::FONT_TEXTURE_BOLD_ITALIC;
      });

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
    /*Log::message("GUIElem data and elements:");
    for (GUIElemData d : data) {
      Log::message("   " + to_string(d.tex_coord) + ","
		   + to_string(d.pos));
    }
    for (u32 elem : elements) {
      Log::message("   %u", elem);
      }*/
    RenderableReg::init();
    RenderableReg::vbo.bindArray(data, false);
    RenderableReg::ebo.bindArray(elements, false);
    RenderableReg::vao.registerVars({Shader::POSITION_2D_SHORT,
	  Shader::TEX_COORD});
  }

  BoundingObject GUIElem::getBoundingObject() {
    return BoundingObject();
  }
  
  BoundingObject GUIElem::getPhysicalBoundingObject() {
    return BoundingObject();
  }
  
  BoundingObject GUIElem::getPrimaryBoundingObject() {
    return BoundingObject();
  }

  void GUIElem::render(RenderContext c) {
    Shader::setFlags(Renderable::shader_flags);
    tex->use();
    RenderableReg::render(c);
  }

  GUINode::GUINode(GUIFloatPos _float_pos, u8 _z_val)
    : float_pos(_float_pos), z_val(_z_val) {}
  
  ArrayHandle GUINode::addChild(GUINode* child) {
    return children.push_back(child);
  }
  ArrayHandle GUINode::addElem(GUIElem* elem) {
    return elems.push_back(elem);
  }
  void GUINode::removeChild(ArrayHandle h) {
    children.removeAndReplace(h);
  }
  void GUINode::removeElem(ArrayHandle h) {
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

}
