#pragma once

#include "Assets.hpp"
#include "RenderableComp.hpp"
#include "Texture.hpp"
#include "Containers.hpp"

NAMESPACE {

  struct GUI {
    static const String FONT_NAME;
    static Asset<Texture> FONT_TEXTURE_REGULAR;
    static Asset<Texture> FONT_TEXTURE_BOLD;
    static Asset<Texture> FONT_TEXTURE_ITALIC;
    static Asset<Texture> FONT_TEXTURE_BOLD_ITALIC;
    static const Vec2f FONT_TEXTURE_SIZE;
    static const u8 FONT_TEXTURE_CHARS_PER_LINE;
    static const u8 FONT_TEXTURE_NUM_LINES;
    static const Vec2f FONT_TEXTURE_CHAR_SIZE;
  };

  struct GUIElemData {
    Vec2s pos;
    Vec2f tex_coord;
    GUIElemData(Vec2s _pos, Vec2f _tex_coord);
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
    GUIElem() : RenderableReg(SHADER_2D) {}
    
    void init();
    void updateBuffers();
    virtual void render(RenderContext c);

    static bool compare(GUIElem* a, GUIElem* b);
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
    GUINode** addChild(GUINode* child);
    GUIElem** addElem(GUIElem* elem);
    void removeChild(GUINode** h);
    void removeElem(GUIElem** h);
    void render(RenderContext c, Mat4f model);

    static bool compare(GUINode* a, GUINode* b);
  };

}
