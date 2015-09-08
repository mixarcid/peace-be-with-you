CXX=g++-5
CXXFLAGS= -Wall -std=c++1y -fno-rtti
INCLUDE = -Isrc/Standard -IThirdParty/include
SOURCES=$(wildcard src/*.cpp) $(wildcard src/Standard/*.cpp)
#OBJDIR = obj/
OBJECTS=$(SOURCES:.cpp=.o)
OUTDIR = bin/
EXECUTABLE= $(OUTDIR)peace

UNAME = $(shell uname)

ifeq ($(UNAME), Linux)
LIBS = -lX11 -lXxf86vm -lXcursor -lXinerama -lXrandr -lXext -lXi -lGL
LIBDIR = -LThirdParty/lib/Linux
endif
ifeq ($(UNAME), Darwin)
LIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation
LIBDIR = -LThirdParty/lib/OSX
endif

LIBS += -lGLEW -lglfw3 -lSOIL -lpthread

debug: CXXFLAGS += -rdynamic
debug: $(SOURCES) $(EXECUTABLE)	

all: debug

depend:
	@makedepend $(SOURCES)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LIBDIR) $(LIBS) $(LIBS) $(OBJECTS) -o $@ $(LIBS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm $(OBJECTS) $(EXECUTABLE)
# DO NOT DELETE

src/Assets.o: /usr/include/unistd.h /usr/include/features.h
src/Assets.o: /usr/include/stdc-predef.h /usr/include/getopt.h src/Assets.hpp
src/BonedMesh.o: src/BonedMesh.hpp src/StaticMesh.hpp src/Renderable.hpp
src/BonedMesh.o: src/VertexObjects.hpp src/GL.hpp src/Shader.hpp
src/BonedMesh.o: src/Vector.hpp src/Math.hpp /usr/include/string.h
src/BonedMesh.o: /usr/include/features.h /usr/include/stdc-predef.h
src/BonedMesh.o: /usr/include/xlocale.h /usr/include/math.h src/Matrix.hpp
src/BonedMesh.o: src/Texture.hpp src/BasicMesh.hpp src/BoundingObject.hpp
src/BonedMesh.o: src/Quaternion.hpp src/Transform.hpp
src/BoundingObject.o: src/BoundingObject.hpp src/Vector.hpp src/Math.hpp
src/BoundingObject.o: /usr/include/string.h /usr/include/features.h
src/BoundingObject.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/BoundingObject.o: /usr/include/math.h src/Matrix.hpp src/BasicMesh.hpp
src/BoundingObject.o: src/Node.hpp src/Transform.hpp src/Quaternion.hpp
src/BoundingObject.o: src/Renderable.hpp src/VertexObjects.hpp src/GL.hpp
src/BoundingObject.o: src/Shader.hpp
src/Camera.o: src/Camera.hpp src/Vector.hpp src/Math.hpp
src/Camera.o: /usr/include/string.h /usr/include/features.h
src/Camera.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/Camera.o: /usr/include/math.h src/Matrix.hpp src/Quaternion.hpp
src/Camera.o: src/Transform.hpp src/GL.hpp
src/Collision.o: src/Collision.hpp src/PhysicalObject.hpp src/Node.hpp
src/Collision.o: src/Transform.hpp src/Vector.hpp src/Math.hpp
src/Collision.o: /usr/include/string.h /usr/include/features.h
src/Collision.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/Collision.o: /usr/include/math.h src/Matrix.hpp src/Quaternion.hpp
src/Collision.o: src/Renderable.hpp src/VertexObjects.hpp src/GL.hpp
src/Collision.o: src/Shader.hpp src/Messageable.hpp src/Contact.hpp
src/Collision.o: src/BoundingObject.hpp src/BasicMesh.hpp
src/Contact.o: src/Contact.hpp src/BoundingObject.hpp src/Vector.hpp
src/Contact.o: src/Math.hpp /usr/include/string.h /usr/include/features.h
src/Contact.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/Contact.o: /usr/include/math.h src/Matrix.hpp src/BasicMesh.hpp
src/GL.o: src/GL.hpp
src/Graphics.o: src/Graphics.hpp src/Light.hpp src/Vector.hpp src/Math.hpp
src/Graphics.o: /usr/include/string.h /usr/include/features.h
src/Graphics.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/Graphics.o: /usr/include/math.h src/Shader.hpp src/GL.hpp src/Matrix.hpp
src/Graphics.o: src/Camera.hpp src/Quaternion.hpp src/Transform.hpp
src/Graphics.o: src/Node.hpp src/Renderable.hpp src/VertexObjects.hpp
src/Graphics.o: src/GUI.hpp src/Texture.hpp src/Input.hpp
src/GUI.o: src/GUI.hpp src/Renderable.hpp src/VertexObjects.hpp src/GL.hpp
src/GUI.o: src/Shader.hpp src/Vector.hpp src/Math.hpp /usr/include/string.h
src/GUI.o: /usr/include/features.h /usr/include/stdc-predef.h
src/GUI.o: /usr/include/xlocale.h /usr/include/math.h src/Matrix.hpp
src/GUI.o: src/Texture.hpp src/BoundingObject.hpp src/BasicMesh.hpp
src/GUI.o: src/Graphics.hpp src/Light.hpp
src/GUIWidgets.o: src/GUIWidgets.hpp src/GUI.hpp src/Renderable.hpp
src/GUIWidgets.o: src/VertexObjects.hpp src/GL.hpp src/Shader.hpp
src/GUIWidgets.o: src/Vector.hpp src/Math.hpp /usr/include/string.h
src/GUIWidgets.o: /usr/include/features.h /usr/include/stdc-predef.h
src/GUIWidgets.o: /usr/include/xlocale.h /usr/include/math.h src/Matrix.hpp
src/GUIWidgets.o: src/Texture.hpp
src/Input.o: src/Input.hpp src/GL.hpp
src/Light.o: src/Light.hpp src/Vector.hpp src/Math.hpp /usr/include/string.h
src/Light.o: /usr/include/features.h /usr/include/stdc-predef.h
src/Light.o: /usr/include/xlocale.h /usr/include/math.h
src/main.o: src/Graphics.hpp src/Light.hpp src/Vector.hpp src/Math.hpp
src/main.o: /usr/include/string.h /usr/include/features.h
src/main.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/main.o: /usr/include/math.h src/Shader.hpp src/GL.hpp src/Matrix.hpp
src/main.o: src/GUIWidgets.hpp src/GUI.hpp src/Renderable.hpp
src/main.o: src/VertexObjects.hpp src/Texture.hpp src/Input.hpp
src/main.o: src/StaticMesh.hpp src/BasicMesh.hpp src/BoundingObject.hpp
src/main.o: src/MeshLoader.hpp src/BonedMesh.hpp src/Quaternion.hpp
src/main.o: src/Transform.hpp src/Node.hpp src/Camera.hpp src/Physics.hpp
src/main.o: src/PhysicalObject.hpp src/Messageable.hpp
src/MeshLoader.o: src/MeshLoader.hpp src/BonedMesh.hpp src/StaticMesh.hpp
src/MeshLoader.o: src/Renderable.hpp src/VertexObjects.hpp src/GL.hpp
src/MeshLoader.o: src/Shader.hpp src/Vector.hpp src/Math.hpp
src/MeshLoader.o: /usr/include/string.h /usr/include/features.h
src/MeshLoader.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/MeshLoader.o: /usr/include/math.h src/Matrix.hpp src/Texture.hpp
src/MeshLoader.o: src/BasicMesh.hpp src/BoundingObject.hpp src/Quaternion.hpp
src/MeshLoader.o: src/Transform.hpp src/Assets.hpp
src/Node.o: src/Node.hpp src/Transform.hpp src/Vector.hpp src/Math.hpp
src/Node.o: /usr/include/string.h /usr/include/features.h
src/Node.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/Node.o: /usr/include/math.h src/Matrix.hpp src/Quaternion.hpp
src/Node.o: src/Renderable.hpp src/VertexObjects.hpp src/GL.hpp
src/Node.o: src/Shader.hpp src/BoundingObject.hpp src/BasicMesh.hpp
src/PhysicalObject.o: src/PhysicalObject.hpp src/Node.hpp src/Transform.hpp
src/PhysicalObject.o: src/Vector.hpp src/Math.hpp /usr/include/string.h
src/PhysicalObject.o: /usr/include/features.h /usr/include/stdc-predef.h
src/PhysicalObject.o: /usr/include/xlocale.h /usr/include/math.h
src/PhysicalObject.o: src/Matrix.hpp src/Quaternion.hpp src/Renderable.hpp
src/PhysicalObject.o: src/VertexObjects.hpp src/GL.hpp src/Shader.hpp
src/PhysicalObject.o: src/Messageable.hpp src/BoundingObject.hpp
src/PhysicalObject.o: src/BasicMesh.hpp
src/Physics.o: src/Physics.hpp src/PhysicalObject.hpp src/Node.hpp
src/Physics.o: src/Transform.hpp src/Vector.hpp src/Math.hpp
src/Physics.o: /usr/include/string.h /usr/include/features.h
src/Physics.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/Physics.o: /usr/include/math.h src/Matrix.hpp src/Quaternion.hpp
src/Physics.o: src/Renderable.hpp src/VertexObjects.hpp src/GL.hpp
src/Physics.o: src/Shader.hpp src/Messageable.hpp src/Collision.hpp
src/Renderable.o: src/Renderable.hpp src/VertexObjects.hpp src/GL.hpp
src/Renderable.o: src/Shader.hpp src/Vector.hpp src/Math.hpp
src/Renderable.o: /usr/include/string.h /usr/include/features.h
src/Renderable.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/Renderable.o: /usr/include/math.h src/Matrix.hpp
src/Shader.o: src/Shader.hpp src/GL.hpp src/Vector.hpp src/Math.hpp
src/Shader.o: /usr/include/string.h /usr/include/features.h
src/Shader.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/Shader.o: /usr/include/math.h src/Matrix.hpp src/Color.hpp src/Assets.hpp
src/StaticMesh.o: src/StaticMesh.hpp src/Renderable.hpp src/VertexObjects.hpp
src/StaticMesh.o: src/GL.hpp src/Shader.hpp src/Vector.hpp src/Math.hpp
src/StaticMesh.o: /usr/include/string.h /usr/include/features.h
src/StaticMesh.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/StaticMesh.o: /usr/include/math.h src/Matrix.hpp src/Texture.hpp
src/StaticMesh.o: src/BasicMesh.hpp src/BoundingObject.hpp
src/Terrain.o: src/Terrain.hpp src/StaticMesh.hpp src/Renderable.hpp
src/Terrain.o: src/VertexObjects.hpp src/GL.hpp src/Shader.hpp src/Vector.hpp
src/Terrain.o: src/Math.hpp /usr/include/string.h /usr/include/features.h
src/Terrain.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/Terrain.o: /usr/include/math.h src/Matrix.hpp src/Texture.hpp
src/Terrain.o: src/BasicMesh.hpp src/BoundingObject.hpp
src/Texture.o: src/Texture.hpp src/GL.hpp src/Shader.hpp src/Vector.hpp
src/Texture.o: src/Math.hpp /usr/include/string.h /usr/include/features.h
src/Texture.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/Texture.o: /usr/include/math.h src/Matrix.hpp src/Assets.hpp
src/Transform.o: src/Transform.hpp src/Vector.hpp src/Math.hpp
src/Transform.o: /usr/include/string.h /usr/include/features.h
src/Transform.o: /usr/include/stdc-predef.h /usr/include/xlocale.h
src/Transform.o: /usr/include/math.h src/Matrix.hpp src/Quaternion.hpp
src/Transform.o: src/Shader.hpp src/GL.hpp
src/VertexObjects.o: src/VertexObjects.hpp src/GL.hpp src/Shader.hpp
src/VertexObjects.o: src/Vector.hpp src/Math.hpp /usr/include/string.h
src/VertexObjects.o: /usr/include/features.h /usr/include/stdc-predef.h
src/VertexObjects.o: /usr/include/xlocale.h /usr/include/math.h
src/VertexObjects.o: src/Matrix.hpp
src/Standard/Exception.o: src/Standard/Exception.hpp /usr/include/stdlib.h
src/Standard/Exception.o: /usr/include/features.h /usr/include/stdc-predef.h
src/Standard/Exception.o: /usr/include/alloca.h /usr/include/execinfo.h
src/Standard/Exception.o: src/Standard/Macros.hpp /usr/include/stdio.h
src/Standard/Exception.o: /usr/include/libio.h /usr/include/_G_config.h
src/Standard/Exception.o: /usr/include/wchar.h /usr/include/xlocale.h
src/Standard/Exception.o: src/Standard/Types.hpp /usr/include/stdint.h
src/Standard/Exception.o: src/Standard/String.hpp src/Standard/Log.hpp
src/Standard/Exception.o: src/Standard/EventManager.hpp
src/Standard/Exception.o: src/Standard/Containers.hpp src/Standard/System.hpp
src/Standard/Exception.o: src/Standard/Thread.hpp
src/Standard/FileIO.o: src/Standard/FileIO.hpp /usr/include/stdio.h
src/Standard/FileIO.o: /usr/include/features.h /usr/include/stdc-predef.h
src/Standard/FileIO.o: /usr/include/libio.h /usr/include/_G_config.h
src/Standard/FileIO.o: /usr/include/wchar.h /usr/include/xlocale.h
src/Standard/FileIO.o: /usr/include/stdint.h src/Standard/Macros.hpp
src/Standard/FileIO.o: /usr/include/stdlib.h /usr/include/alloca.h
src/Standard/FileIO.o: src/Standard/Log.hpp src/Standard/String.hpp
src/Standard/FileIO.o: src/Standard/EventManager.hpp
src/Standard/FileIO.o: src/Standard/Containers.hpp src/Standard/Types.hpp
src/Standard/FileIO.o: src/Standard/System.hpp src/Standard/Thread.hpp
src/Standard/FileIO.o: src/Standard/Exception.hpp /usr/include/execinfo.h
src/Standard/Log.o: src/Standard/Log.hpp src/Standard/Macros.hpp
src/Standard/Log.o: /usr/include/stdio.h /usr/include/features.h
src/Standard/Log.o: /usr/include/stdc-predef.h /usr/include/libio.h
src/Standard/Log.o: /usr/include/_G_config.h /usr/include/wchar.h
src/Standard/Log.o: /usr/include/xlocale.h /usr/include/stdlib.h
src/Standard/Log.o: /usr/include/alloca.h src/Standard/String.hpp
src/Standard/Log.o: src/Standard/EventManager.hpp src/Standard/Containers.hpp
src/Standard/Log.o: src/Standard/Types.hpp /usr/include/stdint.h
src/Standard/Log.o: src/Standard/System.hpp src/Standard/Thread.hpp
src/Standard/Log.o: src/Standard/Exception.hpp /usr/include/execinfo.h
src/Standard/String.o: src/Standard/String.hpp src/Standard/Macros.hpp
src/Standard/String.o: /usr/include/stdio.h /usr/include/features.h
src/Standard/String.o: /usr/include/stdc-predef.h /usr/include/libio.h
src/Standard/String.o: /usr/include/_G_config.h /usr/include/wchar.h
src/Standard/String.o: /usr/include/xlocale.h /usr/include/stdlib.h
src/Standard/String.o: /usr/include/alloca.h src/Standard/Containers.hpp
src/Standard/String.o: src/Standard/Types.hpp /usr/include/stdint.h
src/Standard/System.o: src/Standard/System.hpp src/Standard/Macros.hpp
src/Standard/System.o: /usr/include/stdio.h /usr/include/features.h
src/Standard/System.o: /usr/include/stdc-predef.h /usr/include/libio.h
src/Standard/System.o: /usr/include/_G_config.h /usr/include/wchar.h
src/Standard/System.o: /usr/include/xlocale.h /usr/include/stdlib.h
src/Standard/System.o: /usr/include/alloca.h src/Standard/Containers.hpp
src/Standard/System.o: src/Standard/Types.hpp /usr/include/stdint.h
src/Standard/System.o: src/Standard/Thread.hpp
