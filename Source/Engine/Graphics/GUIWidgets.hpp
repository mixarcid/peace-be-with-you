#pragma once

#include "GUI.hpp"

NAMESPACE {

  struct GUIBasicBox : GUIElem {
    
    void init(Vec2s coord,
	      Vec2s size,
	      //Size of array MUST be 4
	      Array<Vec2f> tex_coords,
	      Texture* tex,
	      u8 z_val);
    void update(Vec2s coord,
		Vec2s size,
		Array<Vec2f> tex_coords,
		Texture* tex,
		u8 z_val);
  };

  struct GUIBox : GUIElem {
    GUIBox(Vec2s coord,
	   Vec2s size,
	   Array<Vec2f> tex_coords,
	   Texture* tex,
	   u8 z_val);
  };

  enum GUITextStyle {
    GUI_TEXT_STYLE_REGULAR,
    GUI_TEXT_STYLE_BOLD,
    GUI_TEXT_STYLE_ITALIC,
    GUI_TEXT_STYLE_BOLD_ITALIC
  };
  enum GUITextAlign {
    GUI_TEXT_ALIGN_CENTER,
    GUI_TEXT_ALIGN_RIGHT,
    GUI_TEXT_ALIGN_LEFT
  };
  struct GUITextProperties {
    GUITextStyle style;
    GUITextAlign align;
    GUITextProperties(GUITextStyle _style,
		      GUITextAlign _align);
  };

  struct GUIBasicTextBox : GUIElem {

    GUITextProperties properties;
    u16 font_size;
    Vec2s coord;
    Vec4f color;
    
    GUIBasicTextBox(Vec2s _coord,
		    String contents,
		    u16 _font_size,
		    u8 z_val,
		    Vec4f _color = Vec4f(0,0,0,1),
		    GUITextProperties _properties
		    = GUITextProperties
		    (GUI_TEXT_STYLE_REGULAR,
		     GUI_TEXT_ALIGN_LEFT),
		    Vec2s* size = NULL);
    void setContents(String contents, Vec2s* size = NULL);
    virtual void render(RenderContext c);
  };

  struct GUITextBox : GUINode {

    GUIBasicTextBox text_part;
    GUIBasicBox box_part;
    Array<Vec2f> tex_coords;

    GUITextBox(Vec2s coord,
	       String contents,
	       u16 font_size,
	       Array<Vec2f> _tex_coords,
	       Texture* tex,
	       GUIFloatPos float_pos,
	       u8 _z_val,
	       Vec4f color = Vec4f(0,0,0,1),
	       GUITextProperties properties
	       = GUITextProperties
	       (GUI_TEXT_STYLE_REGULAR,
		GUI_TEXT_ALIGN_LEFT));
    void setContents(String contents);
    
  };

}
