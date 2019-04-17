#include "Titulo.h"
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>

Titulo::Titulo()
{
    this->setSufixo((char*) "");
    this->setPrefixo((char*) "");
}

// Constructor setting suffix and prefix
Titulo::Titulo(char *suf, char *pre, int alt, int gr)
{
    this->setSufixo(suf);
    this->setPrefixo(pre);
    altura = alt;
    grupo = gr;
}

// Setting Suffix
void Titulo::setSufixo(char *suf)
{
    strcpy(this->suf, suf);
}

// Setting Prefix
void Titulo::setPrefixo(char *pre)
{
    strcpy(this->pref, pre);
}

void Titulo::setAltura(int alt)
{
   altura = alt;
}

void Titulo::setGrupo(int gr)
{
   grupo = gr;
}

// Compute and show FPS
void Titulo::alteraTitulo()
{
    char aux[16];
    static char nome[256] = {0};

        strcpy(nome, suf);
        sprintf(aux, "%d --- %d", altura, grupo);
        strcat(nome, aux);
        strcat(nome, pref);


    glutSetWindowTitle(nome);
}
