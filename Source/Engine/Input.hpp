#pragma once

#include "GL.hpp"
#include "Containers.hpp"
#include "Function.hpp"

NAMESPACE {

  PEACE_DEFINE_BITFLAGS(InputFlags, 8,
			INPUT_NO_FLAGS = 0x00,
			INPUT_CURSOR_DISABLED = 0x01);

  namespace Input {

    typedef function<void(GLFWwindow*, i32, i32, i32, i32)> KeyCallBack;
    typedef function<void(GLFWwindow*, u32)> CharCallBack;
    typedef function<void(GLFWwindow*, f64, f64)> CursorPosCallBack;
    typedef function<void(GLFWwindow*, i32, i32)> ResizeCallBack;
    typedef function<void(GLFWwindow*, i32, i32, i32)> MouseButtonCallBack;
    
    void init(GLFWwindow* win);   
    void setManager(String name);
    
    KeyCallBack* addKeyCallback(KeyCallBack fun);
    CharCallBack* addCharCallback(CharCallBack fun);
    CursorPosCallBack* addCursorPosCallback(CursorPosCallBack fun);
    ResizeCallBack* addWindowResizeCallback(ResizeCallBack fun);
    MouseButtonCallBack* addMouseButtonCallback(MouseButtonCallBack fun);

    void removeKeyCallback(KeyCallBack* handle);
    void removeCharCallback(CharCallBack* handle);
    void removeCursorPosCallback(CursorPosCallBack* handle);
    void removeWindowResizeCallback(ResizeCallBack* handle);
    void removeMouseButtonCallback(MouseButtonCallBack* handle);

    void addFlags(InputFlags flags);
    void removeFlags(InputFlags flags);

    KeyCallBack* addKeyCallback(String name,
				KeyCallBack fun);
    CharCallBack* addCharCallback(String name,
				  CharCallBack fun);
    CursorPosCallBack* addCursorPosCallback(String name,
					    CursorPosCallBack fun);
    ResizeCallBack* addWindowResizeCallback(String name,
					    ResizeCallBack fun);
    MouseButtonCallBack* addMouseButtonCallback(String name,
						MouseButtonCallBack fun);

    void removeKeyCallback(String name,
			   KeyCallBack* handle);
    void removeCharCallback(String name,
			    CharCallBack* handle);
    void removeCursorPosCallback(String name,
				 CursorPosCallBack* handle);
    void removeWindowResizeCallback(String name,
				    ResizeCallBack* handle);
    void removeMouseButtonCallback(String name,
				   MouseButtonCallBack* handle);

    void addFlags(String name,
		  InputFlags flags);
    void removeFlags(String name,
		     InputFlags flags);

    void getWindowSize(i32* width, i32* height);

  };

}
