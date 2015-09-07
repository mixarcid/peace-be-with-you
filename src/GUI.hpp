#pragma once

#include "Renderable.hpp"
#include "Texture.hpp"

NAMESPACE {

  struct GUIElemData {
    Vec2s pos;
    Vec2s tex_coord;
  };

  struct GUIElem : RenderableReg {
    
    Array<GUIElemData> data;
    Array<u32> elements;
    Texture* tex;
    u8 z_val;

    GUIElem(Array<GUIElemData> _data,
	    Array<u32> _elems,
	    Texture* _tex,
	    u8 _z_val);
    virtual BoundingObject getBoundingObject();
    virtual BoundingObject getPhysicalBoundingObject();
    virtual BoundingObject getPrimaryBoundingObject();
    virtual void render(RenderContext c);
  };

  enum GUIFloatPos {
    GUI_FLOAT_TOP,
    GUI_FLOAT_BOTTOM,
    GUI_FLOAT_LEFT,
    GUI_FLOAT_RIGHT,
    GUI_FLOAT_TOP_LEFT,
    GUI_FLOAT_TOP_RIGHT,
    GUI_FLOAT_BOTTOM_LEFT,
    GUI_FLOAT_BOTTOM_RIGHT,
    GUI_FLOAT_CENTER
  };

  struct GUINode {
    
    Array<GUINode*> children;
    Array<GUIElem*> elems;
    GUIFloatPos float_pos;
    u8 z_val;

    GUINode(GUIFloatPos _float_pos, u8 _z_val);
    void addChild(GUINode* child);
    void addElem(GUIElem* elem);
    void render(RenderContext c, Mat4f model);
  };

}
