#pragma once

#include <GLFW/glfw3.h>
#include <GL/glc.h>
#include <GL/glu.h>
#include "Standard.h"
#include "Decorations.h"
#include "VectorMath.h"


NAMESPACE {
  namespace gl {

    inline void enable(GLenum mode) {
      glEnable(mode);
    }

    inline void viewPort(int width, int height) {
      glViewport(0, 0, width, height);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void init2dMat(int width, int height) {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0, width, 0, height, 1.f, -1.f);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void init3dMat(int width, int height) {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(90, width/(float)height, 0.1, 100);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
    }

    inline void rasterPos(Vec2f vec) {
      glRasterPos2f(vec.x, vec.y);
    }
    inline void rasterPos(Vec2d vec) {
      glRasterPos2d(vec.x, vec.y);
    }

    inline void renderString(char* str) {
      glcRenderString(str);
    }
    inline void renderString(String str) {
      glcRenderString(str.c_str());
    }
    
    inline void translate(Vec3f vec) {
      glTranslatef(vec.x, vec.y, vec.z);
    }
    inline void translate(Vec3d vec) {
      glTranslated(vec.x, vec.y, vec.z);
    }
    inline void scale(Vec3f vec) {
      glScalef(vec.x, vec.y, vec.z);
    }
    inline void scale(Vec3d vec) {
      glScaled(vec.x, vec.y, vec.z);
    }
    inline void rotate(Vec3f vec, float angle) {
      glRotatef(angle, vec.x, vec.y, vec.z);
    }
    inline void rotate(Vec3d vec, double angle) {
      glRotated(angle, vec.x, vec.y, vec.z);
    }

    inline void useColor(Colorf col) {
      glColor4f(col.r, col.g, col.b, col.a);
    }
    inline void useColor(Colord col) {
      glColor4d(col.r, col.g, col.b, col.a);
    }

    inline void pushMat() {
      glPushMatrix();
    }
    inline void popMat() {
      glPopMatrix();
    }

    void drawArray(Array<float> arr, GLenum mode) {

    }
    
  }
}
