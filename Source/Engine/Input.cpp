#include "Input.hpp"

NAMESPACE {

  struct InputManager {
    
    InputFlags flags;
    Array<Input::KeyCallBack> key_callbacks;
    Array<Input::CharCallBack> char_callbacks;
    Array<Input::CursorPosCallBack> cursor_pos_callbacks;
    Array<Input::ResizeCallBack> resize_callbacks;
    Array<Input::MouseButtonCallBack> mouse_button_callbacks;
    
    InputManager() {
      flags = INPUT_NO_FLAGS;
    }

    InputManager(const InputManager& man) = default;
    
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

    Pointer<KeyCallBack> addKeyCallback(KeyCallBack fun) {
      return main_manager.key_callbacks.push_back(fun);
    }
    Pointer<CharCallBack> addCharCallback(CharCallBack fun) {
      return main_manager.char_callbacks.push_back(fun);
    }
    Pointer<CursorPosCallBack> addCursorPosCallback(CursorPosCallBack fun) {
      return main_manager.cursor_pos_callbacks.push_back(fun);
    }
    Pointer<ResizeCallBack> addWindowResizeCallback(ResizeCallBack fun) {
      return main_manager.resize_callbacks.push_back(fun);
    }
    Pointer<MouseButtonCallBack> addMouseButtonCallback(MouseButtonCallBack fun) {
      return main_manager.mouse_button_callbacks.push_back(fun);
    }

    void removeKeyCallback(Pointer<KeyCallBack> handle) {
      return main_manager.key_callbacks.removeAndReplace(handle);
    }
    void removeCharCallback(Pointer<CharCallBack> handle) {
      return main_manager.char_callbacks.removeAndReplace(handle);
    }
    void removeCursorPosCallback(Pointer<CursorPosCallBack> handle) {
      return main_manager.cursor_pos_callbacks
	.removeAndReplace(handle);
    }
    void removeWindowResizeCallback(Pointer<ResizeCallBack> handle) {
      return main_manager.resize_callbacks.removeAndReplace(handle);
    }
    void removeMouseButtonCallback(Pointer<MouseButtonCallBack> handle) {
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

    Pointer<KeyCallBack> addKeyCallback(String name,
					KeyCallBack fun) {
      return defineManager(name)->key_callbacks
	.push_back(fun);
    }
    Pointer<CharCallBack> addCharCallback(String name,
					  CharCallBack fun) {
      return defineManager(name)->char_callbacks
	.push_back(fun);
    }
    Pointer<CursorPosCallBack> addCursorPosCallback(String name,
						    CursorPosCallBack fun) {
      return defineManager(name)->cursor_pos_callbacks
	.push_back(fun);
    }
    Pointer<ResizeCallBack> addWindowResizeCallback(String name,
						    ResizeCallBack fun) {
      return defineManager(name)->resize_callbacks
	.push_back(fun);
    }
    Pointer<MouseButtonCallBack> addMouseButtonCallback(String name,
							MouseButtonCallBack fun) {
      return defineManager(name)->mouse_button_callbacks
	.push_back(fun);
    }

    void removeKeyCallback(String name,
			   Pointer<KeyCallBack> handle) {
      defineManager(name)->key_callbacks
	.removeAndReplace(handle);
    }
    void removeCharCallback(String name,
			    Pointer<CharCallBack> handle) {
      defineManager(name)->char_callbacks
	.removeAndReplace(handle);
    }
    void removeCursorPosCallback(String name,
				 Pointer<CursorPosCallBack> handle) {
      defineManager(name)->cursor_pos_callbacks
	.removeAndReplace(handle);
    }
    void removeWindowResizeCallback(String name,
				    Pointer<ResizeCallBack> handle) {
      defineManager(name)->resize_callbacks
	.removeAndReplace(handle);
    }
    void removeMouseButtonCallback(String name,
				   Pointer<MouseButtonCallBack> handle) {
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

  }
  
}
