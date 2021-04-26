#ifndef GL_INCLUDES_H
#define GL_INCLUDES_H

#if _WIN32
#include <windows.h>
#endif
#if __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#endif
