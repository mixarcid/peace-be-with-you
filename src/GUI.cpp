#include "GUI.hpp"
#include "BoundingObject.hpp"
#include "Graphics.hpp"

NAMESPACE {

  GUIElem::GUIElem(Array<GUIElemData> _data,
		   Array<u32> _elems,
		   Texture* _tex,
		   u8 _z_val)
    : RenderableReg(SHADER_2D),
    data(_data),
    elements(_elems),
    tex(_tex),
    z_val(_z_val) {
      /*for (GUIElemData d : data) {
	Log::message(to_string(d.tex_coord) + ","
		     + to_string(d.pos));
      }
      for (u32 elem : elements) {
	Log::message("%u", elem);
	}*/
      RenderableReg::init();
      RenderableReg::vbo.bindArray(data, false);
      RenderableReg::ebo.bindArray(elements, false);
      RenderableReg::vao.registerVars({Shader::POSITION_2D_SHORT,
	    Shader::TEX_COORD_SHORT});
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
  
  void GUINode::addChild(GUINode* child) {
    children.push_back(child);
  }
  
  void GUINode::addElem(GUIElem* elem) {
    elems.push_back(elem);
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
	Mat4f::scaleTrans(Vec3f(1,-1,1),
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
      transform *= Mat4f::scaleTrans(Vec3f(-1,1,1),
				    Vec3f(1,0,0));
      break;
    case GUI_FLOAT_TOP_LEFT:
      transform *= Mat4f::scaleTrans(Vec3f(1,-1,1),
				     Vec3f(-1,1,0));
      break;
    case GUI_FLOAT_TOP_RIGHT:
      transform *= Mat4f::scaleTrans(Vec3f(-1,-1,1),
				     Vec3f(1,1,0));
      break;
    case GUI_FLOAT_BOTTOM_LEFT:
      transform *= Mat4f::scaleTrans(Vec3f(1,1,1),
				     Vec3f(-1,-1, 0));
      break;
    case GUI_FLOAT_BOTTOM_RIGHT:
      transform *= Mat4f::scaleTrans(Vec3f(-1,1,1),
				     Vec3f(1,-1,0));
      break;
    case GUI_FLOAT_CENTER:
      break;
    }
    for (GUINode* child : children) {
      child->render(c, transform);
    }
    //Log::message(to_string(transform));
    Shader::UNI_MODEL.registerVal(transform);
    for (GUIElem* elem : elems) {
      elem->render(c);
    }
  }

}
