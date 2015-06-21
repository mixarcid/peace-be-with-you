#include "Input.hpp"

NAMESPACE {

  Input Input::input;

  void Input::init(GLFWwindow* window) {
    Input::input.win = window;
    
    glfwSetKeyCallback(window,[](GLFWwindow* win,
			       int key, int code,
			       int action, int mods) {
	for (auto callback : Input::input.key_callbacks) {
	  callback(win, key, code, action, mods);
	}
      });
    
    glfwSetWindowSizeCallback(window,[](GLFWwindow* win,
				      int w, int h) {
	for (auto callback : Input::input.resize_callbacks) {
	  callback(win, w, h);
	}
      });

    glfwSetCursorPosCallback(window,[](GLFWwindow* win,
				     double x, double y) {
	for (auto callback : Input::input.cursor_pos_callbacks) {
	  callback(win, x, y);
	}
      });

    glfwSetCharCallback(window,[](GLFWwindow* win,
				unsigned int code) {
	for (auto callback : Input::input.char_callbacks) {
	  callback(win, code);
	}
      });

    glfwSetMouseButtonCallback(window,[](GLFWwindow* win,
				       int button,
				       int action,
				       int mods) {
	for (auto callback : Input::input.mouse_button_callbacks) {
	  callback(win, button, action, mods);
	}
      });
  }
    
  unsigned int
    Input::addKeyCallback(function<void(GLFWwindow*, int,
				 int, int, int)> fun) {
    Input::input.key_callbacks.push_back(fun);
    return Input::input.key_callbacks.size() - 1;
  }
  unsigned int
    Input::addCharCallback(function<void(GLFWwindow*,
				  unsigned int)> fun) {
    Input::input.char_callbacks.push_back(fun);
    return Input::input.char_callbacks.size() - 1;
  }
  unsigned int
    Input::addCursorPosCallback(function<void(GLFWwindow*,
				       double, double)> fun) {
    Input::input.cursor_pos_callbacks.push_back(fun);
    return Input::input.cursor_pos_callbacks.size() - 1;
  }
  unsigned int
    Input::addWindowResizeCallback(function<void(GLFWwindow*,
					  int, int)> fun) {
    Input::input.resize_callbacks.push_back(fun);
    return Input::input.resize_callbacks.size() - 1;
  }
  unsigned int
    Input::addMouseButtonCallback(function<void(GLFWwindow*,
						int, int,
						int)> fun) {
    Input::input.mouse_button_callbacks.push_back(fun);
    return Input::input.mouse_button_callbacks.size() - 1;
  }

  void Input::removeKeyCallback(unsigned int index) {
    removeAndReplace(Input::input.key_callbacks, index);
  }
  void Input::removeCharCallback(unsigned int index) {
    removeAndReplace(Input::input.char_callbacks, index);
  }
  void Input::removeCursorPosCallback(unsigned int index) {
    removeAndReplace(Input::input.cursor_pos_callbacks, index);
  }
  void Input::removeWindowResizeCallback(unsigned int index) {
    removeAndReplace(Input::input.resize_callbacks, index);
  }
  void Input::removeMouseButtonCallback(unsigned int index) {
    removeAndReplace(Input::input.mouse_button_callbacks,
		     index);
  }

  void Input::getWindowSize(int* width, int* height) {
    glfwGetWindowSize(Input::input.win, width, height);
  }
  
}
