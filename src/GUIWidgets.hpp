#pragma once

#include "GUI.hpp"

NAMESPACE {

  struct GUIBox : GUIElem {
    GUIBox(Vec2s coord,
	   Vec2s size,
	   //Size of array MUST be 4
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

    Vec4f color;
    
    GUIBasicTextBox(Vec2s coord,
		    String contents,
		    u16 font_size,
		    u8 z_val,
		    Vec4f _color = Vec4f(0,0,0,1),
		    GUITextProperties properties
		    = GUITextProperties
		    (GUI_TEXT_STYLE_REGULAR,
		     GUI_TEXT_ALIGN_RIGHT));
    virtual void render(RenderContext c);
  };

}
