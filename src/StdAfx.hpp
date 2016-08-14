#pragma once

#ifdef _WIN32
#include <Windows.h>
// Needed because Windows is stupid and defines macros for these things
#undef min
#undef max
#undef near
#undef far
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/CEGUIOpenGLRenderer.h>
#include <SILLY/SILLY.h>
#include <Cg/cgGL.h>
