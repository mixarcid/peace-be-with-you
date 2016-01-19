#include "Input.hpp"

NAMESPACE {

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
    
    InputManager() {
      flags = INPUT_NO_FLAGS;
    }
    
  };
  
  namespace Input {

    static GLFWwindow* window;
    static HashMap<String, InputManager> managers;
    static InputManager main_manager;
    static InputManager* cur_manager;

    static InputManager* defineManager(String name) {
      InputManager man;
      return &((*(managers.insert(Pair<String,
				InputManager>(name,man))
		  .first)).second);
    }

    static void implimentFlags(InputFlags flags) {
      if (flags & INPUT_CURSOR_DISABLED) {
	glfwSetInputMode(window,
			 GLFW_CURSOR,
			 GLFW_CURSOR_DISABLED);
      } else {
	glfwSetInputMode(window,
			 GLFW_CURSOR,
			 GLFW_CURSOR_NORMAL);
      }
    }

    void init(GLFWwindow* win) {
      window = win;
    
      glfwSetKeyCallback
	(window,[](GLFWwindow* win,
		   i32 key,
		   i32 code,
		   i32 action,
		   i32 mods) {
	  for (auto callback : cur_manager
		 ->key_callbacks) {
	    callback(win, key, code, action, mods);
	  }
	  for (auto callback : main_manager
		 .key_callbacks) {
	    callback(win, key, code, action, mods);
	  }
	});
    
      glfwSetWindowSizeCallback
	(window,[](GLFWwindow* win, i32 w, i32 h) {
	  for (auto callback : cur_manager
		 ->resize_callbacks) {
	    callback(win, w, h);
	  }
	  for (auto callback : main_manager
		 .resize_callbacks) {
	    callback(win, w, h);
	  }
	});

      glfwSetCursorPosCallback
	(window,[](GLFWwindow* win, f64 x, f64 y) {
	  for (auto callback : cur_manager
		 ->cursor_pos_callbacks) {
	    callback(win, x, y);
	  }
	  for (auto callback : main_manager
		 .cursor_pos_callbacks) {
	    callback(win, x, y);
	  }
	});

      glfwSetCharCallback
	(window,[](GLFWwindow* win, u32 code) {
	  for (auto callback : cur_manager
		 ->char_callbacks) {
	    callback(win, code);
	  }
	  for (auto callback : main_manager
		 .char_callbacks) {
	    callback(win, code);
	  }
	});

      glfwSetMouseButtonCallback
	(window,[](GLFWwindow* win,
		   i32 button,
		   i32 action,
		   i32 mods) {
	  for (auto callback : cur_manager
		 ->mouse_button_callbacks) {
	    callback(win, button, action, mods);
	  }
	  for (auto callback : main_manager
		 .mouse_button_callbacks) {
	    callback(win, button, action, mods);
	  }
	});
    }

    void setManager(String name) {
      cur_manager = defineManager(name);
      implimentFlags(cur_manager->flags);
    }
    
    ArrayHandle
    addKeyCallback(function<void(GLFWwindow*, i32,
				 i32, i32, i32)> fun) {
      return main_manager.key_callbacks.push_back(fun);
    }
    ArrayHandle
    addCharCallback(function<void(GLFWwindow*,
				  u32)> fun) {
      return main_manager.char_callbacks.push_back(fun);
    }
    ArrayHandle
    addCursorPosCallback(function<void(GLFWwindow*,
				       f64, f64)> fun) {
      return main_manager.cursor_pos_callbacks.push_back(fun);
    }
    ArrayHandle
    addWindowResizeCallback(function<void(GLFWwindow*,
					  i32, i32)> fun) {
      return main_manager.resize_callbacks.push_back(fun);
    }
    ArrayHandle
    addMouseButtonCallback(function<void(GLFWwindow*,
					 i32, i32,
					 i32)> fun) {
      return main_manager.mouse_button_callbacks.push_back(fun);
    }

    void removeKeyCallback(ArrayHandle handle) {
      return main_manager.key_callbacks.removeAndReplace(handle);
    }
    void removeCharCallback(ArrayHandle handle) {
      return main_manager.char_callbacks.removeAndReplace(handle);
    }
    void removeCursorPosCallback(ArrayHandle handle) {
      return main_manager.cursor_pos_callbacks
	.removeAndReplace(handle);
    }
    void removeWindowResizeCallback(ArrayHandle handle) {
      return main_manager.resize_callbacks.removeAndReplace(handle);
    }
    void removeMouseButtonCallback(ArrayHandle handle) {
      return main_manager.mouse_button_callbacks
	.removeAndReplace(handle);
    }

    void addFlags(InputFlags flags) {
      main_manager.flags |= flags;
      implimentFlags(main_manager.flags);
    }
    void removeFlags(InputFlags flags) {
      main_manager.flags &= ~flags;
      implimentFlags(main_manager.flags);
    }

    ArrayHandle
    addKeyCallback(String name,
		   function<void(GLFWwindow*, i32,
				 i32, i32, i32)> fun) {
      return defineManager(name)->key_callbacks
	.push_back(fun);
    }
    ArrayHandle
    addCharCallback(String name,
		    function<void(GLFWwindow*,
				  u32)> fun) {
      return defineManager(name)->char_callbacks
	.push_back(fun);
    }
    ArrayHandle
    addCursorPosCallback(String name,
			 function<void(GLFWwindow*,
				       f64, f64)> fun) {
      return defineManager(name)->cursor_pos_callbacks
	.push_back(fun);
    }
    ArrayHandle
    addWindowResizeCallback(String name,
			    function<void(GLFWwindow*,
					  i32, i32)> fun) {
      return defineManager(name)->resize_callbacks
	.push_back(fun);
    }
    ArrayHandle
    addMouseButtonCallback(String name,
			   function<void(GLFWwindow*,
					 i32, i32,
					 i32)> fun) {
      return defineManager(name)->mouse_button_callbacks
	.push_back(fun);
    }

    void removeKeyCallback(String name,
			   ArrayHandle handle) {
      defineManager(name)->key_callbacks
	.removeAndReplace(handle);
    }
    void removeCharCallback(String name,
			    ArrayHandle handle) {
      defineManager(name)->char_callbacks
	.removeAndReplace(handle);
    }
    void removeCursorPosCallback(String name,
				 ArrayHandle handle) {
      defineManager(name)->cursor_pos_callbacks
	.removeAndReplace(handle);
    }
    void removeWindowResizeCallback(String name,
				    ArrayHandle handle) {
      defineManager(name)->resize_callbacks
	.removeAndReplace(handle);
    }
    void removeMouseButtonCallback(String name,
				   ArrayHandle handle) {
      defineManager(name)->mouse_button_callbacks
	.removeAndReplace(handle);
    }

    void addFlags(String name,
		  InputFlags flags) {
      InputManager* man = defineManager(name);
      man->flags |= flags;
      if (man == cur_manager) {
	implimentFlags(man->flags);
      }
    }
    void removeFlags(String name,
		     InputFlags flags) {
      InputManager* man = defineManager(name);
      man->flags &= ~flags;
      if (man == cur_manager) {
	implimentFlags(man->flags);
      }
    }

    void getWindowSize(i32* width, i32* height) {
      glfwGetWindowSize(window, width, height);
    }

  }
  
}
