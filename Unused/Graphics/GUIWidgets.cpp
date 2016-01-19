#include "GUIWidgets.hpp"

NAMESPACE {

  void GUIBasicBox::init(Vec2s center,
			 Vec2s halves,
			 Array<Vec2f> tex_coords,
			 Texture* tex,
			 u8 z_val) {
    RenderableReg::init();
    update(center, halves, tex_coords, tex, z_val);
  }

  void GUIBasicBox::update(Vec2s center,
			   Vec2s halves,
			   Array<Vec2f> tex_coords,
			   Texture* tex,
			   u8 z_val) {

    GUIElem::data  = { GUIElemData(center - halves, tex_coords[0]),
		       GUIElemData(Vec2s(center.x() - halves.x(),
					 center.y() + halves.y()),
				   tex_coords[1]),
		       GUIElemData(center + halves, tex_coords[2]),
		       GUIElemData(Vec2s(center.x() + halves.x(),
					 center.y() - halves.y()),
				   tex_coords[3])
    };
    GUIElem::elements = {0,1,2,0,2,3};
    GUIElem::tex = tex;
    GUIElem::z_val = z_val;
    GUIElem::updateBuffers();
  }

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

  GUIBasicTextBox::GUIBasicTextBox(Vec2s _coord,
				   String contents,
				   u16 _font_size,
				   u8 z_val,
				   Vec4f _color,
				   GUITextProperties _properties,
				   Vec2s* size)
    : GUIElem({}, {}, NULL, z_val),
    properties(_properties),
    font_size(_font_size),
    coord(_coord),
    color(_color) {
      
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

      RenderableReg::init();
      setContents(contents, size);
    }
  
  void GUIBasicTextBox::setContents(String contents,
				    Vec2s* size) {

    static const f32 FONT_SIZE_MULTIPLIER = 0.5;
    u16 text_width
      = contents.length()*font_size*FONT_SIZE_MULTIPLIER;
    
    i16 upper_y = coord.y() + font_size/2;
    i16 lower_y = coord.y() - font_size/2;
    i16 cur_x, start_x;
    u16 cur_index = 0;
    
    switch (properties.align)  {
    case GUI_TEXT_ALIGN_CENTER:
      start_x = coord.x() - text_width/2;
      break;
    case GUI_TEXT_ALIGN_RIGHT:
      start_x = coord.x() - text_width;
      break;
    case GUI_TEXT_ALIGN_LEFT:
      start_x = coord.x();
      break;
    }
    cur_x = start_x;

    GUIElem::data.clear();
    GUIElem::elements.clear();
      
    for (char ch : contents) {
      char c = ch-32;
      Vec2f tex_coord
	((f32)(c%GUI::FONT_TEXTURE_CHARS_PER_LINE)
	 /(f32)GUI::FONT_TEXTURE_CHARS_PER_LINE,
	 GUI::FONT_TEXTURE_CHAR_SIZE.cy() + 
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
		-GUI::FONT_TEXTURE_CHAR_SIZE.cy())));
      GUIElem::data.push_back
	(GUIElemData
	 (Vec2s(cur_x+font_size, upper_y),
	  Vec2f(tex_coord.x()
		+GUI::FONT_TEXTURE_CHAR_SIZE.cx(),
		tex_coord.y()
		-GUI::FONT_TEXTURE_CHAR_SIZE.cy())));
      GUIElem::data.push_back
	(GUIElemData
	 (Vec2s(cur_x+font_size, lower_y),
	  Vec2f(tex_coord.x()
		+GUI::FONT_TEXTURE_CHAR_SIZE.cx(),
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

    if (size) {
      *size = Vec2s(cur_x - start_x, font_size);
    }

    updateBuffers();
  }
  
  void GUIBasicTextBox::render(RenderContext c) {
    Shader::setFlags(Renderable::shader_flags);
    Shader::UNI_COLOR.registerVal(color);
    tex->use();
    RenderableReg::render(c);
    Shader::UNI_COLOR.registerVal(Shader::DEFAULT_COLOR);
  }

  GUITextBox::GUITextBox(Vec2s coord,
			 String contents,
			 u16 font_size,
			 Array<Vec2f> _tex_coords,
			 Texture* tex,
			 GUIFloatPos float_pos,
			 u8 z_val,
			 Vec4f color,
			 GUITextProperties properties)
    : GUINode(float_pos, z_val),
    text_part(coord, "", font_size, 1, color, properties),
    tex_coords(_tex_coords) {

    Vec2s size;
    text_part.setContents(contents, &size);
    
    switch (properties.align) {
    case GUI_TEXT_ALIGN_CENTER:
      box_part.init(coord, size, tex_coords, tex, 0);
      break;
    case GUI_TEXT_ALIGN_RIGHT:
      box_part.init(Vec2s(coord.x() - size.x()/2, coord.y()),
		    size, tex_coords, tex, 0);
      break;
    case GUI_TEXT_ALIGN_LEFT:
      box_part.init(Vec2s(coord.x() + size.x()/2, coord.y()),
		    size, tex_coords, tex, 0);
      break;
    }

    addElem(&box_part);
    addElem(&text_part);
  }
  
  void GUITextBox::setContents(String contents) {
    Vec2s size;
    text_part.setContents(contents, &size);
    switch (text_part.properties.align) {
    case GUI_TEXT_ALIGN_CENTER:
      box_part.update(text_part.coord, size, tex_coords, text_part.tex, 0);
      break;
    case GUI_TEXT_ALIGN_RIGHT:
      box_part.update(Vec2s(text_part.coord.x() - size.x()/2,
			    text_part.coord.y()),
		      size, tex_coords, text_part.tex, 0);
      break;
    case GUI_TEXT_ALIGN_LEFT:
      box_part.update(Vec2s(text_part.coord.x() + size.x()/2,
			    text_part.coord.y()),
		      size, tex_coords, text_part.tex, 0);
      break;
    }
  }
  
}
