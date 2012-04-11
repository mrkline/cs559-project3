#pragma once

#include <Windows.h>
// Needed because Windows is stupid and defines macros for these things
#undef min
#undef max
#undef near
#undef far
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <CEGUI.h>
#include <RendererModules/OpenGL/CEGUIOpenGLRenderer.h>