#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include "extras.h"
#include <vector>

/// Estruturas iniciais para armazenar vertices
//  Você poderá utilizá-las adicionando novos métodos (de acesso por exemplo) ou usar suas próprias estruturas.
class vertice
{
public:
    double x,y,z;
};

class face
{
public:
    int nVertices;
    std::vector<vertice> verticesFace;
    vertice vetorNormal;
};



class triangle
{
public:
    vertice v[3];
};

/// Globals
float zdist = 5.0;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;
int   width, height;
std::ifstream arquivoPly;
std::string str;
int numeroVertices;
int numeroFaces;
std::vector<vertice> verticesImg;
std::vector<face> facesImg;
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
void CalculaNormal(face t, vertice *vn)
{
    vertice v_0 = t.verticesFace[0],
            v_1 = t.verticesFace[1],
            v_2 = t.verticesFace[2];
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

void leArquivoPly()
{

    bool leArquivo=true;

    while(leArquivo)
    {
        arquivoPly >> str;
        if(str=="vertex")
            arquivoPly >> numeroVertices;
        else
        {
            if(str=="face")
                arquivoPly >> numeroFaces;
            else
            {
                if(str=="end_header")
                    leArquivo = false;
            }
        }

    }

    for(int  i = 0; i<numeroVertices; i ++)
    {
        vertice v;
        arquivoPly >> v.x;
        arquivoPly >> v.y;
        arquivoPly >> v.z;

        verticesImg.push_back(v);
    }

    for(int  i = 0; i<numeroFaces; i ++)
    {
        face f;
        arquivoPly >> f.nVertices;
        for(int j=0; j<f.nVertices; j++)
        {
            int indice;
            arquivoPly >> indice;
            f.verticesFace.push_back(verticesImg[indice]);
        }
        CalculaNormal(f, &f.vetorNormal);

        facesImg.push_back(f);
    }


}
/// Functions
void init(void)
{

    initLight(width, height); // Função extra para tratar iluminação.
    glEnable(GL_DEPTH_TEST);
//    setMaterials();
    arquivoPly.open("bunny.ply");
    leArquivoPly();

}


void drawObject()
{

    vertice vetorNormal;

    for(int i = 0; i < numeroFaces; i++)
    {
        //CalculaNormal(facesImg[i], &vetorNormal);
        glNormal3f(facesImg[i].vetorNormal.x, facesImg[i].vetorNormal.y,facesImg[i].vetorNormal.z);
        glBegin(GL_POLYGON);
        for(int j=0; j < facesImg[i].nVertices; j++)
        {

            glVertex3d(facesImg[i].verticesFace[j].x,facesImg[i].verticesFace[j].y,facesImg[i].verticesFace[j].z);
        }
        glEnd();

    }
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt (0.0, 0.0, zdist,0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    glRotatef( rotationY, 0.0, 1.0, 0.0 );
    glRotatef( rotationX, 1.0, 0.0, 0.0 );
    drawObject();
    glPopMatrix();

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

    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 0.01, 200.0);
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
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
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
