// Much of the code in this file is based on code provided by Lisa Dion
// in a 3D graphics tutorial.

#ifndef graphics_h
#define graphics_h

#include "structs.h"

#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Program initialization NOT OpenGL/GLUT dependent,
// as we haven't created a GLUT window yet
void init();

// Initialize OpenGL Graphics
void InitGL();

// Callback functions for GLUT

// Draw the window - this is where all the GL actions are
void display();

// Trap and process alphanumeric keyboard events
void kbd(unsigned char key, int x, int y);

// Trap and process special keyboard events
void kbdS(int key, int x, int y);

// Handle "mouse cursor moved" events
void cursor(int x, int y);

// Calls itself after a specified time
void timer(int dummy);

// Handle mouse button pressed and released events
void mouse(int button, int state, int x, int y);

// Shortcut functions
void drawPoint(Point p);            // Calls glVertex3f
void drawPoint2D(Point p);          // Calls glVertex2f on x and y
void setGLColor(RGBAcolor color);   // Calls glColor4f
void cull();                        // glEnable(GL_CULL_FACE)
void unCull();                      // glDisable(GL_CULL_FACE)

#endif /* graphics_h */
