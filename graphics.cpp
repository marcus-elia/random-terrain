// Much of the code in this file is based on code provided by Lisa Dion
// in a 3D graphics tutorial.
// The code to switch to 2D drawing mode is from https://www.youtube.com/watch?v=i1mp4zflkYo,
// a video by The Pentamollis Project
// The code to enable alpha is from https://www.opengl.org/archives/resources/faq/technical/transparency.htm


#include "graphics.h"
#include "gameManager.h"

GLdouble width, height;
int wd;
GameManager manager;
// Mouse variables
int prevMouseX, prevMouseY;
bool justClicked;

void init()
{
    width = 1024;
    height = 512;
    prevMouseX = width/2;
    prevMouseY = height/2;
    justClicked = false;
}

/* Initialize OpenGL Graphics */
void initGL()
{
    // Enable alpha transparency
    // Code from https://www.opengl.org/archives/resources/faq/technical/transparency.htm
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set "clearing" or background color
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // Sky blue
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Initialize the camera where the player is, because during the intro screen,
    // if the camera gets put in a different place, it would be messed up when the
    // game starts if you initialize it to the intro location.
    Point camLoc = manager.getPlayer().getLocation();
    Point camLook = manager.getPlayer().getLookingAt();
    Point camUp = manager.getPlayer().getUp();
    gluLookAt(camLoc.x, camLoc.y, camLoc.z,  // eye position
              camLook.x, camLook.y, camLook.z,  // center position (not gaze direction)
              camUp.x, camUp.y, camUp.z); // up vector
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display()
{
    glLineWidth(3.0);

    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, width/height, 1, 4096);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glEnable(GL_DEPTH);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT, GL_FILL);

    // Update where the camera is
    Point camLoc = manager.getCameraLocation();
    Point camLook = manager.getCameraLookingAt();
    Point camUp = manager.getCameraUp();
    gluLookAt(camLoc.x, camLoc.y, camLoc.z,  // eye position
              camLook.x, camLook.y, camLook.z,  // center position (not gaze direction)
              camUp.x, camUp.y, camUp.z); // up vector

    // Draw in 3d
    manager.draw();

    // Switch to 2d mode
    // Code from https://www.youtube.com/watch?v=i1mp4zflkYo
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0,width,0.0,height);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Draw UI things
    manager.drawUI();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y)
{
    // escape
    if(key == 27)
    {
        glutDestroyWindow(wd);
        exit(0);
    }

    switch(key)
    {
        case 'w': manager.setWKey(true);
            break;
        case 'a': manager.setAKey(true);
            break;
        case 's': manager.setSKey(true);
            break;
        case 'd': manager.setDKey(true);
            break;
        case 32: manager.setSpacebar(true);
            break;
        case 'e':  manager.setHyperSpeed(true);
            break;
    }



    glutPostRedisplay();
}


void kbu(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'w': manager.setWKey(false);
            break;
        case 'a': manager.setAKey(false);
            break;
        case 's': manager.setSKey(false);
            break;
        case 'd': manager.setDKey(false);
            break;
        case 'p' : if(manager.getCurrentStatus() != Paused)
            {
                manager.togglePaused();
            }
            break;
        case 32 : manager.setSpacebar(false);
            break;
        case 'e':  manager.setHyperSpeed(false);
            break;
    }

    glutPostRedisplay();
}

void kbdS(int key, int x, int y)
{
    glutPostRedisplay();
}

void cursor(int x, int y)
{
    if(justClicked)
    {
        justClicked = false;
        return;
    }
    double theta = atan2(y - prevMouseY, x - prevMouseX);
    manager.reactToMouseMovement(x, y, theta);
    prevMouseX = x;
    prevMouseY = y;

    // If the cursor gets close to the edge during the game, put it back in the middle.
    if(manager.getCurrentStatus() == Playing && (x < 120 || x > width - 120 || y < 120 || y > height - 120))
    {
        glutWarpPointer(width/2,height/2);
    }
    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y)
{
    if(state == GLUT_UP)
    {
        manager.reactToMouseClick(x, y);
        if(manager.getCurrentStatus() == Playing)
        {
            // Make the cursor invisible
            glutSetCursor(GLUT_CURSOR_NONE);
        }
        else // Keep arrow when not playing
        {
            // Make the arrow visible
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
            // Check for if the quit button was hit
            if(manager.getCloseWindow())
            {
                glutDestroyWindow(wd);
                exit(0);
            }
        }
    }
    justClicked = true;
    glutPostRedisplay();
}

void timer(int dummy)
{
    glutPostRedisplay();
    glutTimerFunc(30, timer, dummy);
    manager.tick();
    if(manager.getShowMouse()) // If the user hasn't clicked, the cursor won't show
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv)
{

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Random Terrain" /* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // Make the cursor invisible
    //glutSetCursor(GLUT_CURSOR_NONE);

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    glutKeyboardUpFunc(kbu);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}

void drawPoint(Point p)
{
    glVertex3f(p.x, p.y, p.z);
}
void drawPoint2D(Point p)
{
    glVertex2f(p.x, p.y);
}
void setGLColor(RGBAcolor color)
{
    glColor4f(color.r, color.g, color.b, color.a);
}
void cull()
{
    glEnable(GL_CULL_FACE);
}
void unCull()
{
    glDisable(GL_CULL_FACE);
}
