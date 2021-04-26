#include "gl-includes.h"
#include "input.h"
#include "world.h"
#include <iomanip>
#include <iostream>

static World g_world;
static Input g_input;

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    g_world.draw();

    unsigned err = 0;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "display: " << gluErrorString(err) << "\n";
    }

    glutSwapBuffers();
}

static void idle()
{
    static int last_time = 0;
    int time = glutGet(GLUT_ELAPSED_TIME);
    int delta = time - last_time;
    last_time = time;

    g_world.simulate(delta, g_input);

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

static void mouse_click(int, int state, int, int)
{
    g_input.handle_mouse_click(state == GLUT_DOWN);
}

static void mouse_move(int x, int y)
{
    static int last_x = x;
    int delta_x = x - last_x;
    last_x = x;

    static int last_y = y;
    int delta_y = y - last_y;
    last_y = y;

    g_input.handle_mouse_move(delta_x, delta_y);
}

int main(int argc, char** argv)
{
    std::cout << std::fixed << std::setprecision(2);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1080, 1080);
    glutInitWindowPosition(static_cast<int>((2560 - 1080) * 0.5), static_cast<int>((1440 - 1080) * 0.5));
    glutCreateWindow("Asteroid Arena 3D");
    // glutFullScreen();

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);
    glutMouseFunc(mouse_click);
    glutMotionFunc(mouse_move);
    glutPassiveMotionFunc(mouse_move);
    glutMainLoop();

    return EXIT_SUCCESS;
}
