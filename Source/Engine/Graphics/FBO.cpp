#include "FBO.hpp"

NAMESPACE {
  
  FBO::FBO() : depth_buffer(0), num_targets(0) {}

  FBO::~FBO() {
    if (PEACE_GL_SHOULD_DELETE) {
      glDeleteFramebuffers(1, &this->id);
    }
    if (depth_buffer) {
      glDeleteRenderbuffers(1, &depth_buffer);
    }
  }

  void FBO::init() {
    GLObject::init();
    glGenFramebuffers(1, &this->id);
  }

  void FBO::addDepthBuffer(Vec2i size) {
    debugAssert(this->id != 0,
		"You must init the FBO before adding a depth buffer");
    if (depth_buffer) {
      glDeleteRenderbuffers(1, &depth_buffer);
    }
    use();
    glGenRenderbuffers(1, &depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x(), size.y());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			      GL_RENDERBUFFER, depth_buffer);
  }

  void FBO::bindTargets(Array<Texture> textures) {
    use();
    num_targets = 0;
    Array<GLenum> buffers(textures.size());
    for (Texture tex : textures) {
      GLenum attach = GL_COLOR_ATTACHMENT0 + (num_targets++);
      buffers.push_back(attach);
      glFramebufferTexture(GL_FRAMEBUFFER, attach, tex.id, 0);
    }
    glDrawBuffers(num_targets, buffers.begin());
  }

  void FBO::clearTargets(Array<Vec4f> colors) {
    debugAssert(colors.size() == num_targets,
		"You must call FBO::clearTargets with "
		"an array that has size equal to the "
		"number of targets");
    use();
    Array<GLenum> buffers(colors.size());
    u8 count = 0;
    for (Vec4f color : colors) {
      GLenum attach = GL_COLOR_ATTACHMENT0 + (count++);
      buffers.push_back(attach);
      glDrawBuffer(attach);
      glClearColor(color.x(), color.y(),
		   color.z(), color.w());
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    glDrawBuffers(count, buffers.begin());
  }

  bool FBO::isComplete() {
    use();
    return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
  }
  
  void FBO::use() {
    debugAssert(!(this->flags & PEACE_GL_UNINITIALIZED),
		"You must initialize an FBO before using it");
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
  }

  void FBO::useScreen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

}
