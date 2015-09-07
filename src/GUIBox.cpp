#include "GUIBox.hpp"

NAMESPACE {

  GUIBox::GUIBox(Vec2s center,
		 Vec2s halves,
		 Array<Vec2s> tex_coords,
		 Texture* tex,
		 u8 z_val)
    : GUIElem({
	//emacs formats this stuff REALLY strangely...
	{center - halves, tex_coords[0]},
	  {Vec2s(center.x() - halves.x(),
		 center.y() + halves.y()),
	      tex_coords[1]},
	    {center + halves, tex_coords[2]},
	      {Vec2s(center.x() + halves.x(),
		     center.y() - halves.y()),
		  tex_coords[3]}
      },
      {0,1,2,0,2,3},
      tex,
      z_val) {}
      

}
