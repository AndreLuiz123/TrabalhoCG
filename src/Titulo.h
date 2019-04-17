#ifndef TITULO_H_INCLUDED
#define TITULO_H_INCLUDED

class Titulo{

    private:
      char suf[64];
      char pref[64];
      int altura;
      int grupo;

    public:
      Titulo();
      Titulo(char *suf, char *pref, int alt, int gr);
      void setSufixo(char *suf);
      void setPrefixo(char *pref);
      void setAltura(int alt);
      void setGrupo(int gr);
      void alteraTitulo();
};

#endif // TITULO_H_INCLUDED
