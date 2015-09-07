#pragma once

#include "GL.hpp"
#include "Containers.hpp"
#include "Function.hpp"

NAMESPACE {

  PEACE_DEFINE_BITFLAGS(InputFlags, 8,
			INPUT_NO_FLAGS = 0x00,
			INPUT_CURSOR_DISABLED = 0x01);

  struct InputManager {
    InputFlags flags;
    HandledArray<function<void(GLFWwindow*, i32, i32, i32, i32)>>
    key_callbacks;
    HandledArray<function<void(GLFWwindow*, u32)>>
    char_callbacks;
    HandledArray<function<void(GLFWwindow*, f64, f64)>>
    cursor_pos_callbacks;
    HandledArray<function<void(GLFWwindow*, i32, i32)>>
    resize_callbacks;
    HandledArray<function<void(GLFWwindow*, i32, i32, i32)>>
    mouse_button_callbacks;
    InputManager();
  };

  namespace Input {
    
    void init(GLFWwindow* win);   
    void setManager(String name);
    
    ArrayHandle
    addKeyCallback(function<void(GLFWwindow*,
				 i32,
				 i32,
				 i32,
				 i32)> fun);
    ArrayHandle
    addCharCallback(function<void(GLFWwindow*,
				  u32)> fun);
    ArrayHandle
    addCursorPosCallback(function<void(GLFWwindow*,
				       f64,
				       f64)> fun);
    ArrayHandle
    addWindowResizeCallback(function<void(GLFWwindow*,
					  i32,
					  i32)> fun);
    ArrayHandle
    addMouseButtonCallback(function<void(GLFWwindow*,
					 i32,
					 i32,
					 i32)> fun);

    void removeKeyCallback(ArrayHandle handle);
    void removeCharCallback(ArrayHandle handle);
    void removeCursorPosCallback(ArrayHandle handle);
    void removeWindowResizeCallback(ArrayHandle handle);
    void removeMouseButtonCallback(ArrayHandle handle);

    void addFlags(InputFlags flags);
    void removeFlags(InputFlags flags);

    ArrayHandle
    addKeyCallback(String name,
		   function<void(GLFWwindow*,
				 i32,
				 i32,
				 i32,
				 i32)> fun);
    ArrayHandle
    addCharCallback(String name,
		   function<void(GLFWwindow*,
				  u32)> fun);
    ArrayHandle
    addCursorPosCallback(String name,
			 function<void(GLFWwindow*,
				       f64,
				       f64)> fun);
    ArrayHandle
    addWindowResizeCallback(String name,
			    function<void(GLFWwindow*,
					  i32,
					  i32)> fun);
    ArrayHandle
    addMouseButtonCallback(String name,
			   function<void(GLFWwindow*,
					 i32,
					 i32,
					 i32)> fun);

    void removeKeyCallback(String name,
			   ArrayHandle handle);
    void removeCharCallback(String name,
			    ArrayHandle handle);
    void removeCursorPosCallback(String name,
				 ArrayHandle handle);
    void removeWindowResizeCallback(String name,
				    ArrayHandle handle);
    void removeMouseButtonCallback(String name,
				   ArrayHandle handle);

    void addFlags(String name,
		  InputFlags flags);
    void removeFlags(String name,
		     InputFlags flags);

    void getWindowSize(i32* width, i32* height);

  };

}
