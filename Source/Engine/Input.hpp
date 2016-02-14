#pragma once

#include "GL.hpp"
#include "Containers.hpp"
#include "Function.hpp"
#include "Pointable.hpp"

NAMESPACE {
  
  PEACE_DEFINE_BITFLAGS(InputFlags, 8,
			INPUT_NO_FLAGS = 0x00,
			INPUT_CURSOR_DISABLED = 0x01);

  namespace Input {

    struct KeyCallBack : function<void(GLFWwindow*, i32, i32, i32, i32)>, Pointable {
      using function<void(GLFWwindow*, i32, i32, i32, i32)>::function;
    };
    struct CharCallBack : function<void(GLFWwindow*, u32)>, Pointable {
      using function<void(GLFWwindow*, u32)>::function;
    };
    struct CursorPosCallBack : function<void(GLFWwindow*, f64, f64)>, Pointable {
      using function<void(GLFWwindow*, f64, f64)>::function;
    };
    struct ResizeCallBack : function<void(GLFWwindow*, i32, i32)>, Pointable {
      using function<void(GLFWwindow*, i32, i32)>::function;
    };
    struct MouseButtonCallBack : function<void(GLFWwindow*, i32, i32, i32)>, Pointable {
      using function<void(GLFWwindow*, i32, i32, i32)>::function;
    };
    
    void init(GLFWwindow* win); 
    void setManager(String name);
    
    Pointer<KeyCallBack> addKeyCallback(KeyCallBack fun);
    Pointer<CharCallBack> addCharCallback(CharCallBack fun);
    Pointer<CursorPosCallBack> addCursorPosCallback(CursorPosCallBack fun);
    Pointer<ResizeCallBack> addWindowResizeCallback(ResizeCallBack fun);
    Pointer<MouseButtonCallBack> addMouseButtonCallback(MouseButtonCallBack fun);
    
    void removeKeyCallback(Pointer<KeyCallBack> handle);
    void removeCharCallback(Pointer<CharCallBack> handle);
    void removeCursorPosCallback(Pointer<CursorPosCallBack> handle);
    void removeWindowResizeCallback(Pointer<ResizeCallBack> handle);
    void removeMouseButtonCallback(Pointer<MouseButtonCallBack> handle);

    void addFlags(InputFlags flags);
    void removeFlags(InputFlags flags);

    Pointer<KeyCallBack> addKeyCallback(String name,
					KeyCallBack fun);
    Pointer<CharCallBack> addCharCallback(String name,
					  CharCallBack fun);
    Pointer<CursorPosCallBack> addCursorPosCallback(String name,
						    CursorPosCallBack fun);
    Pointer<ResizeCallBack> addWindowResizeCallback(String name,
						    ResizeCallBack fun);
    Pointer<MouseButtonCallBack> addMouseButtonCallback(String name,
							MouseButtonCallBack fun);

    void removeKeyCallback(String name,
			   Pointer<KeyCallBack> handle);
    void removeCharCallback(String name,
			    Pointer<CharCallBack> handle);
    void removeCursorPosCallback(String name,
				 Pointer<CursorPosCallBack> handle);
    void removeWindowResizeCallback(String name,
				    Pointer<ResizeCallBack> handle);
    void removeMouseButtonCallback(String name,
				   Pointer<MouseButtonCallBack> handle);

    void addFlags(String name,
		  InputFlags flags);
    void removeFlags(String name,
		     InputFlags flags);

  };

}
