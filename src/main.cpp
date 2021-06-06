#include "input.h"
#include "obj.h"
#include "world.h"
#include <CoreGraphics/CoreGraphics.h>
#include <GLUT/glut.h>

static std::unique_ptr<World> g_world;
static Input g_input;

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_world->draw();

    glutSwapBuffers();
}

static void idle()
{
    static int last_time = 0;
    int time = glutGet(GLUT_ELAPSED_TIME);
    int delta = time - last_time;
    last_time = time;

    g_world->simulate(delta, g_input.take());

    glutPostRedisplay();
}

static void keyboard(unsigned char key, int, int)
{
    g_input.handle_key(key, true);
}

static void keyboard_up(unsigned char key, int, int)
{
    g_input.handle_key(key, false);
}

static void mouse_move(int, int)
{
    int delta_x = 0;
    int delta_y = 0;
    CGGetLastMouseDelta(&delta_x, &delta_y);

    g_input.handle_mouse_move(delta_x, delta_y);
}

int main(int argc, char** argv)
{
    CGAssociateMouseAndMouseCursorPosition(false);

    glutInit(&argc, argv);
    glutInitWindowPosition(static_cast<int>((2560 - 1080) * 0.5), static_cast<int>((1440 - 1080) * 0.5));
    glutCreateWindow("Asteroid Arena 3D");
    glutFullScreen();

    glClearColor(0, 0, 0, 1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_RESCALE_NORMAL);
    glutSetCursor(GLUT_CURSOR_NONE);

    g_world = std::make_unique<World>();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);
    glutMotionFunc(mouse_move);
    glutPassiveMotionFunc(mouse_move);
    glutMainLoop();

    return EXIT_SUCCESS;
}
