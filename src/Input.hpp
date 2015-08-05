#pragma once

#include "GL.hpp"
#include "Containers.hpp"
#include "Function.hpp"

NAMESPACE {

  struct Input {

    GLFWwindow* win;
    Array<function<void(GLFWwindow*, int, int, int, int)>>
    key_callbacks;
    Array<function<void(GLFWwindow*, u32)>>
    char_callbacks;
    Array<function<void(GLFWwindow*, f64, f64)>>
    cursor_pos_callbacks;
    Array<function<void(GLFWwindow*, int, int)>>
    resize_callbacks;
    Array<function<void(GLFWwindow*, int, int, int)>>
    mouse_button_callbacks;

    static void init(GLFWwindow* window);
    
    static u32
    addKeyCallback(function<void(GLFWwindow*, int,
				 int, int, int)> fun);
    static u32
    addCharCallback(function<void(GLFWwindow*,
				  u32)> fun);
    static u32
    addCursorPosCallback(function<void(GLFWwindow*,
				       f64, f64)> fun);
    static u32
    addWindowResizeCallback(function<void(GLFWwindow*,
					  int, int)> fun);
    static u32
    addMouseButtonCallback(function<void(GLFWwindow*, int,
					 int, int)> fun);

    static void removeKeyCallback(u32 index);
    static void removeCharCallback(u32 index);
    static void removeCursorPosCallback(u32 index);
    static void removeWindowResizeCallback(u32 index);
    static void removeMouseButtonCallback(u32 index);

    static void getWindowSize(int* width, int* height);
    
    static Input input;
  };

}
