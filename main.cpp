#include <windows.h>
#include <glut.h>
#include <stdio.h>
#include <time.h>

#define UP      1
#define DOWN    2
#define LEFT    3
#define RIGHT   4

GLint level = 1;
GLint points = 0;
GLint size = 0;
GLbyte game_over = true;
GLbyte enable_light = true;

GLint body_position[2][100] = { {} };
GLint x_coordinate = 5;
GLint z_coordinate = 10;
GLint old_x_coordinate[2] = {};
GLint old_z_coordinate[2] = {};
GLbyte direction = 0;

GLint x_coordinate_of_food = 0;
GLint z_coordinate_of_food = 0;

GLint width = 800;
GLint height = 550;
GLint gi_width = 0;
GLint gi_height = 0;
GLint gf_width = 150;
GLint gf_height = 150;

static GLfloat x_rotation = 45.0F;
static GLfloat y_rotation = 0.0F;
static GLfloat z_rotation = 0.0F;

static GLfloat head_rotation = 90.0F;
static GLfloat zoom = -300.0F;

DWORD last_fps = 0;
int new_fps = 0, fps_frame = 0;


void InitLight(void)
{
    GLfloat ambient_color[] = {0.3f, 0.4f, 0.8f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_color);

    GLfloat light_color_0[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat light_position_0[] = {4.0f, 0.0f, 8.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color_0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position_0);

    GLfloat light_color_1[] = {0.5f, 0.2f, 0.2f, 1.0f};
    GLfloat light_position_1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color_1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position_1);
}

void Initialize(void)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 1.0, 0.0, 0.0);

    if (enable_light) {
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_NORMALIZE);
    }
}

void Resize(int weight, int height)
{
    glViewport(0, 0, weight, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)weight / (double)height, 1, 800.0);
}

void Write(const char* str) 
{
    while (*str)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *str++);
}

void ManipulateViewAngle(void)
{
    glRotatef(x_rotation, 1.0, 0.0, 0.0);
    glRotatef(y_rotation, 0.0, 1.0, 0.0);
    glRotatef(z_rotation, 0.0, 0.0, 1.0);
}

void Reset(void)
{
    x_coordinate = 5;
    z_coordinate = 10;
    direction = 0;
    level = 1;
    points = 0;
    size = 0;
    game_over = false;
    x_rotation = 45.0F;
    y_rotation = 0.0F;
    z_rotation = 0.0F;
    head_rotation = 90.0F;
    zoom = -300.0F;
}

void WelcomeScreen(void) 
{
    glColor3f(1, 0, 0);
    glRasterPos2f(0, 20);
    Write("Snake Game");

    glColor3f(0, 0, 1);
    glRasterPos2f(0, 10);
    Write("Esen EYRIBAS");

    glColor3f(0, 0, 1);
    glRasterPos2f(0, 0);
    Write("Press the letter 'e' to start the game....");
}

void DrawSnake(void)
{
    glPushMatrix();
    ManipulateViewAngle();

    glPushMatrix();
    glColor3f(0.6f, 0.7f, 0.8f);
    glTranslatef(75.0, -16.0, 75.0);
    glScalef(155, 5.0, 155);
    glutSolidCube(1);
    glPopMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(x_coordinate, -10.0, z_coordinate);
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(10, 20, 20);
    glRotatef(head_rotation, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(0, 0.0, 6.0);
    glScalef(0.8, 1.0, 1.0);
    glutSolidCone(10, 10, 20, 20);
    glColor3f(0, 0, 0);
    glTranslatef(-4.0, 10.0, 0.0);
    glScalef(0.3, 0.3, 0.3);
    glutSolidSphere(10, 20, 20);
    glTranslatef(26.0, 0.0, 0.0);
    glutSolidSphere(10, 20, 20);
    glPopMatrix();

    for (int i = 0; i < size; i++) {
        glPushMatrix();
        ManipulateViewAngle();
        glTranslatef(body_position[0][i], -10.0, body_position[1][i]);
        glColor3f(1.0, 1.0, 0.6);
        glScalef(0.5, 0.5, 0.5);
        glutSolidSphere(7, 20, 20);
        glPopMatrix();
    }
}

void DrawFood(void)
{
    glPushMatrix();
    ManipulateViewAngle();
    glTranslatef(x_coordinate_of_food, -10.0, z_coordinate_of_food);
    glColor3f(0.8, 0.4, 0.4);
    glScalef(0.5, 0.5, 0.5);
    glutSolidSphere(7, 20, 20);
    glPopMatrix();
}

void GameStatus(void) 
{
    char tmp[40];
    glColor3f(0, 0, 0);
    glRasterPos2f(5, 50);
    sprintf_s(tmp, "Level:%d  Score:%d      ", level, points);
    Write(tmp);
}

int RandomNumber(int high, int low)
{
    return (rand() % (high - low)) + low;
}

void NewFood(void) 
{
    time_t second;
    time(&second);
    srand((unsigned int)second);
    x_coordinate_of_food = RandomNumber(gf_width - gi_width, gi_width + 10);
    z_coordinate_of_food = RandomNumber(gf_height - gi_height, gi_height + 10);
}

void FPS(void) 
{
    char tmp[40];

    if (GetTickCount() - last_fps >= 1000) {
        last_fps = GetTickCount();
        new_fps = fps_frame;
        fps_frame = 0;
    }
    fps_frame++;

    glRasterPos2f(75, 50);
    sprintf_s(tmp, "FPS degeri:%d", new_fps);
    Write(tmp);
}

bool Collision() 
{
    for (int i = 0; i < size; i++) {
        if ((body_position[0][i] == x_coordinate && body_position[1][i] == z_coordinate) ||
            ((body_position[0][i] >= x_coordinate) && (body_position[0][i] <= x_coordinate + 5) &&
            (body_position[1][i] >= z_coordinate) && (body_position[1][i] <= z_coordinate + 5)) ||
            ((body_position[0][i] <= x_coordinate) && (body_position[0][i] >= x_coordinate - 5) &&
            (body_position[1][i] <= z_coordinate) && (body_position[1][i] >= z_coordinate - 5)) ||
            ((body_position[0][i] <= x_coordinate) && (body_position[0][i] >= x_coordinate - 5) &&
            (body_position[1][i] >= z_coordinate) && (body_position[1][i] <= z_coordinate + 5)) ||
            ((body_position[0][i] >= x_coordinate) && (body_position[0][i] <= x_coordinate + 5) &&
            (body_position[1][i] <= z_coordinate) && (body_position[1][i] >= z_coordinate - 5))) {
            return true;
        }
    }

    return false;
}

void Run(int value) 
{
    old_x_coordinate[1] = x_coordinate;
    old_z_coordinate[1] = z_coordinate;

    switch (direction) {
        case RIGHT:
            head_rotation = 90;
            x_coordinate += 6;
            if (x_coordinate > gf_width - 2) 
                x_coordinate = gi_width - 1;
            break;
        case LEFT:
            head_rotation = -90;
            x_coordinate -= 6;
            if (x_coordinate < 0) 
                x_coordinate = gf_width - 2;
            break;
        case UP:
            head_rotation = 0;
            z_coordinate += 6;
            if (z_coordinate > gf_height - 2) 
                z_coordinate = gi_height - 1;
            break;
        case DOWN:
            head_rotation = 180;
            z_coordinate -= 6;
            if (z_coordinate < 2)
                z_coordinate = gf_height - 2;
            break;
    }

    if (Collision()) 
        game_over = true;

    if ((x_coordinate == x_coordinate_of_food && z_coordinate == z_coordinate_of_food) ||
       ((x_coordinate >= x_coordinate_of_food) && (x_coordinate <= x_coordinate_of_food + 4) && 
       (z_coordinate >= z_coordinate_of_food) && (z_coordinate <= z_coordinate_of_food + 4)) ||
       ((x_coordinate <= x_coordinate_of_food) && (x_coordinate >= x_coordinate_of_food - 4) && 
       (z_coordinate <= z_coordinate_of_food) && (z_coordinate >= z_coordinate_of_food - 4)) ||
       ((x_coordinate <= x_coordinate_of_food) && (x_coordinate >= x_coordinate_of_food - 4) && 
       (z_coordinate >= z_coordinate_of_food) && (z_coordinate <= z_coordinate_of_food + 4)) ||
       ((x_coordinate >= x_coordinate_of_food) && (x_coordinate <= x_coordinate_of_food + 4) && 
       (z_coordinate <= z_coordinate_of_food) && (z_coordinate >= z_coordinate_of_food - 4))) {
        points++;

        if (points < 100) 
            size++;

        if (points % 5 == 0 && level < 15) 
            level++;

        NewFood();
    }

    for (int i = 0; i < size; i++) {
        old_x_coordinate[0] = old_x_coordinate[1];
        old_z_coordinate[0] = old_z_coordinate[1];
        old_x_coordinate[1] = body_position[0][i];
        old_z_coordinate[1] = body_position[1][i];
        body_position[0][i] = old_x_coordinate[0];
        body_position[1][i] = old_z_coordinate[0];
    }

    glutTimerFunc(130 - level * 4, Run, 0);
}

void Display(void) 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (enable_light) 
        InitLight();

    glTranslatef(-60.0, 40.0, zoom);
    if (!game_over) {
        GameStatus();
        DrawFood();
        DrawSnake();
    } else {
        WelcomeScreen();
    }

    FPS();
    glutPostRedisplay();
    glutSwapBuffers();
}

void Special(int key, int x, int y) 
{
    switch (key) {
        case GLUT_KEY_RIGHT:
            if (direction != LEFT)
                direction = RIGHT;
            break;
        case GLUT_KEY_LEFT:
            if (direction != RIGHT)
                direction = LEFT;
            break;
        case GLUT_KEY_UP:
            if (direction != UP)
                direction = DOWN;
            break;
        case GLUT_KEY_DOWN:
            if (direction != DOWN)
                direction = UP;
            break;
    }
}

void Keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'a': 
            x_rotation += 2;
            glutPostRedisplay();
            break;
        case 'A': 
            x_rotation -= 2;
            glutPostRedisplay();
            break;
        case 's': 
            y_rotation += 2;
            glutPostRedisplay();
            break;
        case 'S': 
            y_rotation -= 2;
            glutPostRedisplay();
            break;
        case 'd': 
            z_rotation += 2;
            glutPostRedisplay();
            break;
        case 'D': 
            z_rotation -= 2;
            glutPostRedisplay();
            break;
        case 'f': 
            size++;
            glutPostRedisplay();
            break;
        case 'F': 
            size--;
            glutPostRedisplay();
            break;
        case 'g': 
            zoom++;
            glutPostRedisplay();
            break;
        case 'G': 
            zoom--;
            glutPostRedisplay();
            break;
        case 'e': 
            Reset();
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

int main(void) 
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(80, 80);
    glutCreateWindow("Snake Game");

    glutSpecialFunc(Special);
    glutKeyboardFunc(Keyboard);
    glutDisplayFunc(Display);
    glutReshapeFunc(Resize);

    NewFood();
    Run(0);
    Initialize();

    glutMainLoop();

    return 0;
}