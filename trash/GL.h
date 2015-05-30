#pragma once

#include "GL_OpenGL.h"

NAMESPACE {  
  namespace gl {

    void enable(GLenum mode);

    void viewPort(int width, int height);
    void init2dMat(int width, int height);
    void init3dMat(int width, int height);

    void rasterPos(Vec2f vec);
    void rasterPos(Vec2d vec);

    void renderString(char* str);
    void renderString(String str);
    
    void translate(Vec3f vec);
    void translate(Vec3d vec);
    void scale(Vec3f vec);
    void scale(Vec3d vec);
    void rotate(Vec3f vec, float angle);
    void rotate(Vec3d vec, double angle);

    void useColor(Colorf col);
    void useColor(Colord col);

    void pushMat();
    void popMat();

    void drawArray(Array<float> arr, GLenum mode);
  }
}
