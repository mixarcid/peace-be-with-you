#include "Input.hpp"

NAMESPACE {

  Input Input::input;

  void Input::init(GLFWwindow* window) {
    Input::input.win = window;
    
    glfwSetKeyCallback(window,[](GLFWwindow* win,
			       i32 key, i32 code,
			       i32 action, i32 mods) {
	for (auto callback : Input::input.key_callbacks) {
	  callback(win, key, code, action, mods);
	}
      });
    
    glfwSetWindowSizeCallback(window,[](GLFWwindow* win,
				      i32 w, i32 h) {
	for (auto callback : Input::input.resize_callbacks) {
	  callback(win, w, h);
	}
      });

    glfwSetCursorPosCallback(window,[](GLFWwindow* win,
				     f64 x, f64 y) {
	for (auto callback : Input::input.cursor_pos_callbacks) {
	  callback(win, x, y);
	}
      });

    glfwSetCharCallback(window,[](GLFWwindow* win,
				u32 code) {
	for (auto callback : Input::input.char_callbacks) {
	  callback(win, code);
	}
      });

    glfwSetMouseButtonCallback(window,[](GLFWwindow* win,
				       i32 button,
				       i32 action,
				       i32 mods) {
	for (auto callback : Input::input.mouse_button_callbacks) {
	  callback(win, button, action, mods);
	}
      });
  }
    
  u32
    Input::addKeyCallback(function<void(GLFWwindow*, int,
				 int, int, int)> fun) {
    Input::input.key_callbacks.push_back(fun);
    return Input::input.key_callbacks.size() - 1;
  }
  u32
    Input::addCharCallback(function<void(GLFWwindow*,
				  u32)> fun) {
    Input::input.char_callbacks.push_back(fun);
    return Input::input.char_callbacks.size() - 1;
  }
  u32
    Input::addCursorPosCallback(function<void(GLFWwindow*,
				       f64, f64)> fun) {
    Input::input.cursor_pos_callbacks.push_back(fun);
    return Input::input.cursor_pos_callbacks.size() - 1;
  }
  u32
    Input::addWindowResizeCallback(function<void(GLFWwindow*,
					  int, int)> fun) {
    Input::input.resize_callbacks.push_back(fun);
    return Input::input.resize_callbacks.size() - 1;
  }
  u32
    Input::addMouseButtonCallback(function<void(GLFWwindow*,
						int, int,
						int)> fun) {
    Input::input.mouse_button_callbacks.push_back(fun);
    return Input::input.mouse_button_callbacks.size() - 1;
  }

  void Input::removeKeyCallback(u32 index) {
    removeAndReplace(Input::input.key_callbacks, index);
  }
  void Input::removeCharCallback(u32 index) {
    removeAndReplace(Input::input.char_callbacks, index);
  }
  void Input::removeCursorPosCallback(u32 index) {
    removeAndReplace(Input::input.cursor_pos_callbacks, index);
  }
  void Input::removeWindowResizeCallback(u32 index) {
    removeAndReplace(Input::input.resize_callbacks, index);
  }
  void Input::removeMouseButtonCallback(u32 index) {
    removeAndReplace(Input::input.mouse_button_callbacks,
		     index);
  }

  void Input::getWindowSize(int* width, int* height) {
    glfwGetWindowSize(Input::input.win, width, height);
  }
  
}
