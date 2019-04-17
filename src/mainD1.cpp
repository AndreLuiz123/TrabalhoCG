#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <iostream>
#include "extras.h"
#include <vector>
#include "Titulo.h"



using namespace std;

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

class grupo
{
public:
    std::vector<vertice> verticesGrupo1;
    std::vector<vertice> verticesGrupo2;
};

/// Globals
float zdist = 5.0;
float rotationX = 0.0, rotationY = 0.0, posZ, posY;
int   last_x, last_y;
int   width, height;
int numeroPontos = 0;
int grupoAtual=0;
int altura=1;
int ultimaAltura;
bool apagarTela;
int indice=0;

vertice initialVerticesV1 = {-1.0f, -1.0f,  0.0f};
vertice initialVerticesV2 = { 1.0f, -1.0f,  0.0f};
vertice initialVerticesV3 = {-1.0f,  1.0f,  0.0f};
vertice initialVerticesV4 = { 1.0f,  1.0f,  -0.5f};

vertice lastVertice1 = initialVerticesV1;
vertice lastVertice2 = initialVerticesV2;

std::vector<vertice> vert;
std::vector<triangle> triang;
std::vector<grupo> grupos;

grupo gr;


/// Functions
Titulo *barraT = new Titulo((char*)"ALTURA: ", (char*)" GRUPO", altura, grupoAtual);

void init(void)
{
    initLight(width, height); // Função extra para tratar iluminação.
//   setMaterials();

    grupos.push_back(gr);


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
    for(int j=0; j<grupos.size(); j++)
    {
        if(!grupos[j].verticesGrupo1.empty())
            for(int i=0; i<grupos[j].verticesGrupo1.size()-1; i++)
            {

                vertice v[4] = {/*{-1.0f, -1.0f,  0.0f},
                { 1.0f, -1.0f,  0.0f},
                {-1.0f,  1.0f,  0.0f},
                { 1.0f,  1.0f, -0.5f}*/
                {0.0f,  grupos[j].verticesGrupo1[i].y, grupos[j].verticesGrupo1[i].z},
                {grupos[j].verticesGrupo1[i].x,  grupos[j].verticesGrupo1[i].y, grupos[j].verticesGrupo1[i].z},
                {0.0f,  grupos[j].verticesGrupo1[i+1].y, grupos[j].verticesGrupo1[i+1].z},
                {grupos[j].verticesGrupo1[i+1].x,  grupos[j].verticesGrupo1[i+1].y, grupos[j].verticesGrupo1[i+1].z}
                               };

                triangle t[2] = {{v[0], v[1], v[2]},
                    {v[1], v[3], v[2]}
                };

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
    }


}


void desenhaEixos()
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex2d(-1,0);
    glVertex2d(1,0);
    glColor3f(0,1,0);
    glVertex2d(0,-1);
    glVertex2d(0,1);
    glEnd();
    glEnable(GL_LIGHTING);
}

void desenho2D()
{
    glDisable(GL_LIGHTING);
    glPointSize(9.0f);
    glEnable(GL_POINT_SMOOTH);
    glColor3b(0.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for(int j = 0; j<grupos.size(); j++)
    {
        if(!grupos[j].verticesGrupo1.empty())
        {
            for(int i = 0; i<grupos[j].verticesGrupo1.size(); i++)
                glVertex2d(grupos[j].verticesGrupo1[i].z, grupos[j].verticesGrupo1[i].y);
        }
    }


    glEnd();
    glEnable(GL_LIGHTING);
}

void display(void)
{


    ///A primeira metade é aquela na qual existe o desenho do gráfico
    ///O scissor é ativado para fazer os desenhos nessa metade
    glViewport(0,0, width/2, height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, width/2, height);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    desenhaEixos();
    desenho2D();

    ///A segunda metade é aquela na qual existe a imagem 3D
    ///O scissor é ativado para fazer os desenhos nessa metade



    glViewport(width/2,0, width/2, height);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0, (GLfloat) width*0.5f/(GLfloat) height, 0.01, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    gluLookAt (0.0, 0.0, zdist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    glRotatef( rotationY, 0.0, 1.0, 0.0 );
    glRotatef( rotationX, 1.0, 0.0, 0.0 );


    //if(grupos[grupoAtual].verticesGrupo1.size()>1)
    drawObject();


    glPopMatrix();

    apagarTela = false;

    barraT->setAltura(altura);
    barraT->setGrupo(grupoAtual);
    barraT->alteraTitulo();


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

void specialKeysPress (int key, int x, int y)
{


    switch (tolower(key))
    {
    case GLUT_KEY_UP:
        altura++;
        break;
    case GLUT_KEY_DOWN:
        altura--;
        break;
    case GLUT_KEY_RIGHT:

        indice++;

        if(indice>grupos.size()-1)
        {
            grupo gr;
            grupos.push_back(gr);
            grupoAtual=indice;
        }
        else
        {
            grupoAtual=indice;
        }

        break;
    case GLUT_KEY_LEFT:
        indice--;
        if(indice<0)
        {
            indice = 0;
            grupoAtual=0;
        }
        else
        {
            grupoAtual = indice;
        }

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
        posZ = ((float)x*4)/width -1;
        posY = -(((float)y*2)/height -1);
        if(posZ<1)
        {
            vertice newVertice;

            newVertice.x = altura;
            newVertice.y = posY;
            newVertice.z = posZ;
            grupos[grupoAtual].verticesGrupo1.push_back(newVertice);
            //grupos[grupoAtual].verticesGrupo2.push_back(newVertice);

        }
        // drawNewObject();

    }

    if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
    {
        if(grupos[grupoAtual].verticesGrupo1.size()>0)
        {
            grupos[grupoAtual].verticesGrupo1.pop_back();
            cout<<"apagou"<<endl;
        }

    }

    if(button == 3) // Scroll up
    {
        zdist+=1.0;
    }
    if(button == 4) // Scroll Down
    {
        zdist-=1.0;
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
    glutSpecialFunc(specialKeysPress);
    glutIdleFunc(idle);
    desenhaEixos();
    desenho2D();
    glutMainLoop();
    return 0;
}
