#pragma once

#include "GL.hpp"
#include "Containers.hpp"
#include "Function.hpp"

NAMESPACE {

  struct Input {

    GLFWwindow* win;
    Array<function<void(GLFWwindow*, int, int, int, int)>>
    key_callbacks;
    Array<function<void(GLFWwindow*, unsigned int)>>
    char_callbacks;
    Array<function<void(GLFWwindow*, double, double)>>
    cursor_pos_callbacks;
    Array<function<void(GLFWwindow*, int, int)>>
    resize_callbacks;
    Array<function<void(GLFWwindow*, int, int, int)>>
    mouse_button_callbacks;

    static void init(GLFWwindow* window);
    
    static unsigned int
    addKeyCallback(function<void(GLFWwindow*, int,
				 int, int, int)> fun);
    static unsigned int
    addCharCallback(function<void(GLFWwindow*,
				  unsigned int)> fun);
    static unsigned int
    addCursorPosCallback(function<void(GLFWwindow*,
				       double, double)> fun);
    static unsigned int
    addWindowResizeCallback(function<void(GLFWwindow*,
					  int, int)> fun);
    static unsigned int
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
