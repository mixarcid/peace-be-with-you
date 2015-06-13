#pragma once

#include "Standard.hpp"
#include "Containers.hpp"
#include "VertexObjects.hpp"
#include "Shader.hpp"

NAMESPACE {

  struct ShaderVar;

  template <typename T>
    struct Renderable {

    Array<T> data;
    VBO vbo;
    VAO vao;
    
    void init(Array<ShaderVar> vars, bool isDynamic) {
      /*if (Renderable<T>::firstVAO) {
	Renderable<T>::vao = VAO::getVAO();
	Renderable<T>::firstVAO = false;
	}*/
      vao = VAO::getVAO();
      vao.use();
      vbo.init();
      //vbo = VBO::getVBO();
      vbo.bindArray(data, isDynamic);
      vao.registerVars(vars);
      //VAO::NONE.use();
    }
    virtual void render() {
      Renderable<T>::vao.use();
      vbo.draw(GL_TRIANGLES);
      //VAO::NONE.use();
    }
    virtual ~Renderable() {}

    //static VAO vao;
    //static bool firstVAO;
    
  };

  /*template <typename T>
    VAO Renderable<T>::vao;
  template <typename T>
  bool Renderable<T>::firstVAO = true;*/

  template <typename T>
    struct RenderableEBO : public Renderable<T> {

    EBO ebo;
    Array<GLuint> elements;
    Array<ShaderVar> shader_vars;
    bool is_dynamic;

    void init(/*Array<ShaderVar> vars, bool isDynamic*/) {
      /*if (Renderable<T>::firstVAO) {
	Renderable<T>::vao = VAO::getVAO();
	Renderable<T>::firstVAO = false;
	}*/
      /*Renderable<T>::vbo = VBO::getVBO();
	ebo = EBO::getEBO();*/
      //Renderable<T>::vao = VAO::getVAO();
      //Renderable<T>::vao.use();
      //shader_vars = vars;
      //is_dynamic = isDynamic;
      Renderable<T>::vbo.init();
      ebo.init();
      /*Renderable<T>::vbo.bindArray(Renderable<T>::data, isDynamic);
      ebo.bindArray(elements, isDynamic);
      Renderable<T>::vao.registerVars(vars);*/
      //VAO::NONE.use();
    }
    virtual void render() {
      //Renderable<T>::vao.use();
      Renderable<T>::vbo.bindArray(Renderable<T>::data,
				   is_dynamic);
      ebo.bindArray(elements, is_dynamic);
      Renderable<T>::vao.registerVars(shader_vars);
      ebo.draw(GL_TRIANGLES);
      //VAO::NONE.use();
    }
    
  };
  
}
