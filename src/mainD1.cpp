#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <iostream>
#include "extras.h"
#include <vector>

/// Estruturas iniciais para armazenar vertices
//  Você poderá utilizá-las adicionando novos métodos (de acesso por exemplo) ou usar suas próprias estruturas.
class vertice
{
    public:
        float x,y,z;
};

class triangle
{
    public:
        vertice v[3];
};

/// Globals
float zdist = 5.0;
float rotationX = 0.0, rotationY = 0.0, posZ=1.0, posY=1.0;
int   last_x, last_y;
int   width, height;
int numeroPontos = 0;

vertice initialVerticesV1 = {-1.0f, -1.0f,  0.0f};
vertice initialVerticesV2 = { 1.0f, -1.0f,  0.0f};
vertice initialVerticesV3 = {-1.0f,  1.0f,  0.0f};
vertice initialVerticesV4 = { 1.0f,  1.0f,  -0.5f};

vertice lastVertice1 = initialVerticesV1;
vertice lastVertice2 = initialVerticesV2;



/// Functions
void init(void)
{
    initLight(width, height); // Função extra para tratar iluminação.
//    setMaterials();
}

/* Exemplo de cálculo de vetor normal que são definidos a partir dos vértices do triângulo;
  v_2
  ^
  |\
  | \
  |  \       'vn' é o vetor normal resultante
  |   \
  +----> v_1
  v_0
*/
void CalculaNormal(triangle t, vertice *vn)
{
    vertice v_0 = t.v[0],
            v_1 = t.v[1],
            v_2 = t.v[2];
    vertice v1, v2;
    double len;

    /* Encontra vetor v1 */
    v1.x = v_1.x - v_0.x;
    v1.y = v_1.y - v_0.y;
    v1.z = v_1.z - v_0.z;

    /* Encontra vetor v2 */
    v2.x = v_2.x - v_0.x;
    v2.y = v_2.y - v_0.y;
    v2.z = v_2.z - v_0.z;

    /* Calculo do produto vetorial de v1 e v2 */
    vn->x = (v1.y * v2.z) - (v1.z * v2.y);
    vn->y = (v1.z * v2.x) - (v1.x * v2.z);
    vn->z = (v1.x * v2.y) - (v1.y * v2.x);

    /* normalizacao de n */
    len = sqrt(pow(vn->x,2) + pow(vn->y,2) + pow(vn->z,2));

    vn->x /= len;
    vn->y /= len;
    vn->z /= len;
}

void drawObject()
{
    vertice vetorNormal;
    vertice v[4] = {{-1.0f, -1.0f,  0.0f},
                    { 1.0f, -1.0f,  0.0f},
                    {-1.0f,  1.0f,  0.0f},
                    { 1.0f,  1.0f, -0.5f}};

    triangle t[2] = {{v[0], v[1], v[2]},
                     {v[1], v[3], v[2]}};

    glBegin(GL_TRIANGLES);
        for(int i = 0; i < 2; i++) // triangulos
        {
            CalculaNormal(t[i], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
            glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
            for(int j = 0; j < 3; j++) // vertices do triangulo
                glVertex3d(t[i].v[j].x, t[i].v[j].y, t[i].v[j].z);
        }
    glEnd();
}

void drawNewObject(){

    vertice vetorNormal;
    vertice v[4] = {{lastVertice1.x,  lastVertice1.y, lastVertice1.z},
                    {lastVertice2.x,  lastVertice2.y, lastVertice2.z},
                    {lastVertice1.x,  posY, posZ},
                    {lastVertice2.x,  posY, posZ}};

    triangle t[2] = {{v[0], v[1], v[2]},
                     {v[1], v[3], v[2]}};

    glBegin(GL_TRIANGLES);
        for(int i = 0; i < 2; i++) // triangulos
        {
            CalculaNormal(t[i], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
            glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
            for(int j = 0; j < 3; j++) // vertices do triangulo
                glVertex3d(t[i].v[j].x, t[i].v[j].y, t[i].v[j].z);
        }
    glEnd();
}

void display(void)
{
    //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ///Cada metade da tela tem sua própria viewport e tem o Scissor atuando nela


    ///A primeira metade é aquela na qual existe o desenho do gráfico
    ///O scissor é ativado para fazer os desenhos nessa metade
    glViewport(0 ,0, width/2, height);
    glScissor(0, 0, width/2, height);
    glEnable(GL_SCISSOR_TEST);
        glClearColor(0, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        glOrtho(-1,1,-1,1,0,0);
        glBegin(GL_LINES);
            glColor3b(1,1,0);
            glVertex2d(-2,0);
            glVertex2d(2,0);
            glVertex2d(0,-3);
            glVertex2d(0,3);
        glEnd();
        glPointSize(10);
        glEnable(GL_SMOOTH);
        glBegin(GL_POINTS);
            glColor3b(0,0,1);
            glVertex2d(posZ,posY);
            glVertex2d(initialVerticesV1.z, initialVerticesV1.y);
            glVertex2d(initialVerticesV2.z, initialVerticesV2.y);
            glVertex2d(initialVerticesV3.z, initialVerticesV3.y);
            glVertex2d(initialVerticesV4.z, initialVerticesV4.y);
        glEnd();
    glDisable(GL_SCISSOR_TEST);

    ///A segunda metade é aquela na qual existe a imagem 3D
    ///O scissor é ativado para fazer os desenhos nessa metade
    glViewport(width/2 ,0, width/2, height);
    glScissor(width/2 ,0, width/2, height);
    glEnable(GL_SCISSOR_TEST);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt (0.0, 0.0, zdist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        glPushMatrix();
            glRotatef( rotationY, 0.0, 1.0, 0.0 );
            glRotatef( rotationX, 1.0, 0.0, 0.0 );
            drawObject();
           /* glBegin(GL_TRIANGLES);
            glVertex3d(lastVertice1.x,  lastVertice1.y, lastVertice1.z);
            glVertex3d(lastVertice2.x,  lastVertice2.y, lastVertice2.z);
            glVertex3d(lastVertice2.x,  posY, posZ);
            glVertex3d(lastVertice2.x,  posY, posZ);
            glVertex3d(lastVertice1.x,  lastVertice1.y, lastVertice1.z);
            glVertex3d(lastVertice1.x,  posY, posZ);
            glEnd();*/
            drawNewObject();
        glPopMatrix();

    glDisable(GL_SCISSOR_TEST);

    glutSwapBuffers();
}

void idle ()
{
    glutPostRedisplay();
}

void reshape (int w, int h)
{
    width = w;
    height = h;

    glViewport (0, 0, (GLsizei) w/2, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w*0.5f/(GLfloat) h, 0.01, 200.0);
}

void keyboard (unsigned char key, int x, int y)
{

    switch (tolower(key))
    {
        case 27:
            exit(0);
            break;
    }
}

// Motion callback
void motion(int x, int y )
{
    rotationX += (float) (y - last_y);
    rotationY += (float) (x - last_x);

    last_x = x;
    last_y = y;
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        last_x = x;
        last_y = y;
        posZ = (float)x/100 -2;
        posY = 3 - (float)y/100;
        std::cout<<x<<" "<<y<<" "<<posZ<<" "<<posY<<std::endl;
        numeroPontos++;

    }
    if(button == 3) // Scroll up
    {
        zdist+=1.0f;
    }
    if(button == 4) // Scroll Down
    {
        zdist-=1.0f;
    }
}

/// Main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (800, 600);
    glutInitWindowPosition (0, 0);
    glutCreateWindow ("Trabalho de CG");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
