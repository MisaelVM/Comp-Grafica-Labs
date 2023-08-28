@ECHO OFF
SetLocal EnableDelayedExpansion

SET glfwPath=../Dependencies/glfw-3.3.8
SET gladPath=../Dependencies/glad
SET glmPath=../Dependencies/glm
SET stbPath=../Dependencies/stb

SET cxxFiles=main.cpp GL/Shader.cpp GL/VertexBuffer.cpp GL/VertexArray.cpp GL/ElementBuffer.cpp Rubik/RubiksCube.cpp
SET compilerFlags=-g -Wall
SET includeFlags=-I%glfwPath%/include -I%gladPath%/include -I%glmPath% -I%stbPath% -I.
SET linkerFlags=-L%glfwPath%/lib-mingw-w64 -lglfw3 -lopengl32 -lUser32 -lGdi32 -lShell32

ECHO "Building glad..."
gcc %gladPath%/src/glad.c %compilerFlags% -o glad.o %includeFlags% %linkerFlags% -c
ECHO "Building program..."
g++ %cxxFiles% glad.o -o prog.exe %includeFlags% %linkerFlags%
