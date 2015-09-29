#include "GUIWidgets.hpp"

NAMESPACE {

  GUIBox::GUIBox(Vec2s center,
		 Vec2s halves,
		 Array<Vec2f> tex_coords,
		 Texture* tex,
		 u8 z_val)
    : GUIElem({
	//Emacs formats this stuff rather strangely...
	GUIElemData(center - halves, tex_coords[0]),
	  GUIElemData(Vec2s(center.x() - halves.x(),
			    center.y() + halves.y()),
		      tex_coords[1]),
	  GUIElemData(center + halves, tex_coords[2]),
	  GUIElemData(Vec2s(center.x() + halves.x(),
			    center.y() - halves.y()),
		      tex_coords[3])
	  },
      {0,1,2,0,2,3},
      tex,
      z_val) { GUIElem::init(); }

  GUITextProperties::GUITextProperties
    (GUITextStyle _style,
     GUITextAlign _align)
    : style(_style), align(_align) {}

  GUIBasicTextBox::GUIBasicTextBox(Vec2s coord,
				   String contents,
				   u16 font_size,
				   u8 z_val,
				   Vec4f _color,
				   GUITextProperties properties)
    : GUIElem({},
	      {},
	      NULL,
	      z_val),
    color(_color) {

      static const f32 FONT_SIZE_MULTIPLIER = 0.5;
      u16 text_width
	= contents.length()*font_size*FONT_SIZE_MULTIPLIER;
      
      switch (properties.style) {
      case GUI_TEXT_STYLE_REGULAR:
	GUIElem::tex = GUI::FONT_TEXTURE_REGULAR;
	break;
      case GUI_TEXT_STYLE_BOLD:
	GUIElem::tex = GUI::FONT_TEXTURE_BOLD;
	break;
      case GUI_TEXT_STYLE_ITALIC:
	GUIElem::tex = GUI::FONT_TEXTURE_ITALIC;
	break;
      case GUI_TEXT_STYLE_BOLD_ITALIC:
	GUIElem::tex = GUI::FONT_TEXTURE_BOLD_ITALIC;
	break;
      }
      
      i16 upper_y = coord.y() + font_size/2;
      i16 lower_y = coord.y() - font_size/2;
      i16 cur_x;
      u16 cur_index = 0;
    
      switch (properties.align)  {
      case GUI_TEXT_ALIGN_CENTER:
	cur_x = coord.x() - text_width/2;
	break;
      case GUI_TEXT_ALIGN_RIGHT:
	cur_x = coord.x() - text_width;
	break;
      case GUI_TEXT_ALIGN_LEFT:
	cur_x = coord.x();
	break;
      }
      for (char ch : contents) {
	char c = ch-32;
	Vec2f tex_coord
	  ((f32)(c%GUI::FONT_TEXTURE_CHARS_PER_LINE)
	   /(f32)GUI::FONT_TEXTURE_CHARS_PER_LINE,
	   GUI::FONT_TEXTURE_CHAR_SIZE.y() + 
	   (f32)(c/GUI::FONT_TEXTURE_CHARS_PER_LINE)
	   /(f32)GUI::FONT_TEXTURE_CHARS_PER_LINE);
	GUIElem::data.push_back
	  (GUIElemData
	   (Vec2s(cur_x, lower_y),
	    Vec2f(tex_coord.x(),
		  tex_coord.y())));
	GUIElem::data.push_back
	  (GUIElemData
	   (Vec2s(cur_x, upper_y),
	    Vec2f(tex_coord.x(),
		  tex_coord.y()
		  -GUI::FONT_TEXTURE_CHAR_SIZE.y())));
	GUIElem::data.push_back
	  (GUIElemData
	   (Vec2s(cur_x+font_size, upper_y),
	    Vec2f(tex_coord.x()
		  +GUI::FONT_TEXTURE_CHAR_SIZE.x(),
		  tex_coord.y()
		  -GUI::FONT_TEXTURE_CHAR_SIZE.y())));
	GUIElem::data.push_back
	  (GUIElemData
	   (Vec2s(cur_x+font_size, lower_y),
	    Vec2f(tex_coord.x()
		  +GUI::FONT_TEXTURE_CHAR_SIZE.x(),
		  tex_coord.y())));
	GUIElem::elements.push_back(cur_index);
	GUIElem::elements.push_back(cur_index+1);
	GUIElem::elements.push_back(cur_index+2);
	GUIElem::elements.push_back(cur_index);
	GUIElem::elements.push_back(cur_index+2);
	GUIElem::elements.push_back(cur_index+3);
	cur_index += 4;
	cur_x += font_size*FONT_SIZE_MULTIPLIER;
      }
      GUIElem::init();
  }

  void GUIBasicTextBox::render(RenderContext c) {
    Shader::setFlags(Renderable::shader_flags);
    Shader::UNI_COLOR.registerVal(color);
    tex->use();
    RenderableReg::render(c);
    Shader::UNI_COLOR.registerVal(Vec4f(1,1,1,1));
  }

}
