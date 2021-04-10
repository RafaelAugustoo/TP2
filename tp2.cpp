#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <cmath>


using namespace std;

static bool SolLigado = 1, LigOrbita=0, nomeLigado=0, Pausa=1;   // Luz branca/ orbita ligada ?!
static float d = 1.0;           // Intensidade da cor difusa da luz branca
static float e = 1.0;           // Intensidade da cor especular da luz branca
static float m = 0.0;           // Intensidade da luz ambiente global
static float p = 1.0;           // A luz branca é posicional?
static float s = 50.0;          // Expoente especular do material (shininess)
float matShine[] = { s };                       // expoente especular (shininess)
static float xAngle = 0.0, yAngle = 0.0, angle,fAspect;        // Rotação da luz branca
static long font = (long)GLUT_BITMAP_8_BY_13;   // Fonte usada para imprimir na tela
static bool LuzLigada = true;               // O sistema de iluminação está ligado?
static float translacaoMercurio =0, translacaoVenus=0, translacaoTerra=0, translacaoLua=0, translacaoMarte=0,
              translacaoJupiter=0, translacaoSaturno=0, translacaoUrano=0, translacaoNetuno=0; // Rotação da esfera em torno do sol
static float rotaMercurio =0, rotaVenus=0, rotaTerra=0, rotaLua=0, rotaMarte=0, rotaJupiter=0, rotaSaturno=0, rotaUrano=0, rotaNetuno=0;
static int esferaLados = 200;                   // Quantas subdivisões latitudinais/longitudinais da esfera
static bool localViewer = false;
Mix_Music *Musica;

enum CAMERAS {PRIMEIRA_PESSOA = 1, ESTATICAL, ESTATICAC };
int modoCAM = PRIMEIRA_PESSOA;            //guardar o modo de câmera atual

static int xMouseCamera = 30, yMouseCamera = 0;     //variáveis globais que serão usadas na função posicionaCamera
static int xCursor, yCursor, zCursor;  //guarda o centro do cursor
static float phi = 90, teta = 0;       //ângulos das coordenadas esféricas

struct Textura {
    GLuint id;
    char arquivo[50];
};
GLubyte qtdeTexturas = 12;

static struct Textura texturas[] = { {0, "Recursos/Planetas/Sun.jpg"}, {0, "Recursos/Planetas/mercury.jpg"}, {0, "Recursos/Planetas/venus.jpg"},
                                     {0, "Recursos/Planetas/terra.jpg"}, {0, "Recursos/Planetas/marte.jpg"}, {0, "Recursos/Planetas/jupiter.jpg"},
                                     {0, "Recursos/Planetas/saturno.jpg"}, {0, "Recursos/Planetas/urano.jpg"}, {0, "Recursos/Planetas/netuno.jpg"},
                                     {0, "Recursos/Planetas/lua.jpg"}, {0, "Recursos/Planetas/io.jpg"},{0, "Recursos/Planetas/tita.jpg"}};



// estrutura de dados que representará as coordenadas da câmera
struct {
  float x, y, z;
  float targetX, targetY, targetZ;
} camera;

// Escreve uma cadeia de caracteres
void escreveNaTela(void *font, char *string)
{
    char *c;
    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Converte um número em string
void floatParaString(char * destStr, int precision, float val)
{
    sprintf(destStr,"%f",val);
    destStr[precision] = '\0';
}

void informacoesTela(void)
{
    glClearColor(0,0,0,0);
    glDisable(GL_LIGHTING); // Desabilita iluminação
    glColor3f(1.0f, 1.0f, 1.0f);

    switch (modoCAM) {
        case PRIMEIRA_PESSOA:
        {
            glRasterPos3f(70, -10.0,-20);
            escreveNaTela((void*)font, (char*)"Modo (1) Primeira Pessoa use W A S D para se mover");
            glRasterPos3f(70, -15.0,-20);
            escreveNaTela((void*)font, (char*)"Alternar as Cameras (Pressione 1, 2 ou 3)");
            glRasterPos3f(70, -20, -20);
            escreveNaTela((void*)font, (char*)"Luz do Sol(L/l): ");
            escreveNaTela((void*)font, (char*)(SolLigado ? "Ligada" : "Desligada"));
            glRasterPos3f(70, -25,-20);
            escreveNaTela((void*)font, (char*)"Nomes(N/n): ");
            escreveNaTela((void*)font, (char*)(nomeLigado ? "Ligados" : "Desligados"));
            glRasterPos3f(70, -30, -20);
            escreveNaTela((void*)font, (char*)"Orbita(O/o) ");
            escreveNaTela((void*)font, (char*)(LigOrbita ? "Ligada" : "Desligada"));
            glRasterPos3f(70, -35, -20);
            escreveNaTela((void*)font, (char*)"Il Global(G/g): ");
            escreveNaTela((void*)font, (char*)(LuzLigada ? "Desligada" : "Ligada"));
            glRasterPos3f(70.0, -40,-20);
            escreveNaTela((void*)font, (char*)"Musica(Z/z): ");
            escreveNaTela((void*)font, (char*)(Pausa ? "Ligada" : "Desligada"));
            glRasterPos3f(70, -45.0,-20);
            escreveNaTela((void*)font, (char*)"Pressione Esc pra Sair");

          }
          break;

        case ESTATICAL: //lateral
        {
            glRasterPos3f(0.0, 290.0,450);
            escreveNaTela((void*)font, (char*)"Modo (2): visualização Lateral");
            glRasterPos3f(0.0, 270.0,450.0);
            escreveNaTela((void*)font, (char*)"Alternar as Cameras (Pressione 1, 2 ou 3)");
            glRasterPos3f(0.0, 250.0, 450.0);
            escreveNaTela((void*)font, (char*)"Luz do Sol(L/l): ");
            escreveNaTela((void*)font, (char*)(SolLigado ? "Ligada" : "Desligada"));
            glRasterPos3f(0.0, 230.0,450.0);
            escreveNaTela((void*)font, (char*)"Nomes(N/n): ");
            escreveNaTela((void*)font, (char*)(nomeLigado ? "Ligados" : "Desligados"));
            glRasterPos3f(0.0, 210.0, 450.0);
            escreveNaTela((void*)font, (char*)"Orbita(O/o) ");
            escreveNaTela((void*)font, (char*)(LigOrbita ? "Ligada" : "Desligada"));
            glRasterPos3f(0.0, 190.0, 450.0);
            escreveNaTela((void*)font, (char*)"Il Global(G/g): ");
            escreveNaTela((void*)font, (char*)(LuzLigada ? "Desligada" : "Ligada"));
            glRasterPos3f(0.0, 170.0, 450.0);
            escreveNaTela((void*)font, (char*)"Musica(Z/z): ");
            escreveNaTela((void*)font, (char*)(Pausa ? "Ligada" : "Desligada"));
            glRasterPos3f(0.0, 150.0, 450.0);
            escreveNaTela((void*)font, (char*)"Pressione Esc pra Sair");



          }
          break;
        case ESTATICAC://cima
        {
            glRasterPos3f(900.0, 0.0,-350.0);
            escreveNaTela((void*)font, (char*)"Modo (3): Visualização de Cima");
            glRasterPos3f(880.0, 0.0,-370.0);
            escreveNaTela((void*)font, (char*)"Alternar as Cameras (Pressione 1, 2 ou 3)");
            glRasterPos3f(860.0,0.0, -390.0);
            escreveNaTela((void*)font, (char*)"Luz do Sol(L/l): ");
            escreveNaTela((void*)font, (char*)(SolLigado ? "Ligada" : "Desligada"));
            glRasterPos3f(840.0, 0.0,-410.0);
            escreveNaTela((void*)font, (char*)"Nomes(N/n): ");
            escreveNaTela((void*)font, (char*)(nomeLigado ? "Ligados" : "Desligados"));
            glRasterPos3f(820.0, 0.0,-430.0);
            escreveNaTela((void*)font, (char*)"Orbita(O/o) ");
            escreveNaTela((void*)font, (char*)(LigOrbita ? "Ligada" : "Desligada"));
            glRasterPos3f(800.0, 0.0,-450.0);
            escreveNaTela((void*)font, (char*)"Il Global(G/g): ");
            escreveNaTela((void*)font, (char*)(LuzLigada ? "Desligada" : "Ligada"));
            glRasterPos3f(780.0, 0.0,-470.0);
            escreveNaTela((void*)font, (char*)"Musica(Z/z): ");
            escreveNaTela((void*)font, (char*)(Pausa ? "Ligada" : "Desligada"));
            glRasterPos3f(760.0, 0.0,-490.0);
            escreveNaTela((void*)font, (char*)"Pressione Esc pra Sair");
          }
          break;
        default:
              break;
          }

}

//carrega texturas
GLuint carregaTextura(const char* caminho) {
    GLuint idTextura = SOIL_load_OGL_texture(
        caminho,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (idTextura == 0) {
        printf("Erro do SOIL ao carregar %s: '%s'\n", caminho, SOIL_last_result());
    }

    return idTextura;
}

void inicializa(void){

    glClearColor(0,0,0, 0.0);
    glEnable(GL_DEPTH_TEST);

    // Propriedades do material da esfera
    float matAmbAndDif[] = {1.0, 1.0, 1.0, 1.0};    // cor ambiente e difusa: branca
    float matSpec[] = { 1.0, 1.0, 1,0, 1.0 };       // cor especular: branca

    //ropriedades do material
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Musica = Mix_LoadMUS("Recursos/Som/musica.mp3");
    //angle =45 ;
    xCursor = 0;       //a câmera começa olhando para o ponto 0
    yCursor = 0;
    zCursor = 0;

  // carrega as texturas
  for (int c = 0; c < qtdeTexturas; c++) {
      texturas[c].id = carregaTextura(texturas[c].arquivo);
  }

    // Não mostra faces do lado de dentro
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    Mix_PlayMusic(Musica, -1);
}

// Desenha uma esfera na origem
void solidSphere(int radius, int stacks, int columns)
{
    // cria uma quádrica
    GLUquadric* quadObj = gluNewQuadric();
    // estilo preenchido... poderia ser GLU_LINE, GLU_SILHOUETTE
    // ou GLU_POINT
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    // chama 01 glNormal para cada vértice.. poderia ser
    // GLU_FLAT (01 por face) ou GLU_NONE
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    // chama 01 glTexCoord por vértice
    gluQuadricTexture(quadObj, GL_TRUE);
    // cria os vértices de uma esfera
    gluSphere(quadObj, radius, stacks, columns);
    // limpa as variáveis que a GLU usou para criar
    // a esfera
    gluDeleteQuadric(quadObj);
}

void mercurio(){
  // Desenha mercurio

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturas[1].id);
  glPushMatrix();
      glRotatef(translacaoMercurio, 0, 1, 0);
      glRotatef(90, 1, 0, 0);
      glTranslatef(60, 0, 0); //60 é a distancia em x em relação ao sol, cada planeta tem uam distancia maior como segue nas funções abaixo
      if(nomeLigado){
        glDisable(GL_LIGHTING);
        glRasterPos3i(0, 0, -20);
        glColor3ub(255, 255, 255);
        escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_10, "Mercurio");
        glEnable(GL_LIGHTING);
      }
      glRotatef(rotaMercurio, 0, 0, 1);
      glRotatef(90, 1, 0, 0);
      solidSphere(2.0, esferaLados, esferaLados);
  glPopMatrix();
}

void venus(){
  // Desenha a venus
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturas[2].id);
  glPushMatrix();
      glRotatef(translacaoVenus, 0, 1, 0);
      glRotatef(90, 1, 0, 0);
      glTranslatef(120, 0, 0);
      if(nomeLigado){
        glDisable(GL_LIGHTING);
        glRasterPos3i(0, 0, -20);
        glColor3ub(255, 255, 255);
        escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_10, "Venus");
        glEnable(GL_LIGHTING);
      }
      glRotatef(rotaVenus, 0, 0, 1);
      solidSphere(6.5, esferaLados, esferaLados);
  glPopMatrix();
}

void lua(){
  // Desenha a lua
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturas[9].id);
  glPushMatrix();
      glRotatef(translacaoLua, 0, 0, 1);
      glRotatef(90, 1, 0, 0);
      glTranslatef(14, 0, 0);
      if(nomeLigado){
        glDisable(GL_LIGHTING);
        glRasterPos3i(0, 0, -20);
        glColor3ub(255, 255, 255);
        escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_10, "Lua");
        glEnable(GL_LIGHTING);
      }
      glRotatef(rotaLua, 1, 0, 0);
      solidSphere(3.0, esferaLados, esferaLados);
  glPopMatrix();

      if(LigOrbita){ //orbita da lua
        glPushMatrix();
          glDisable(GL_TEXTURE_2D);
          glDisable(GL_LIGHTING);
          glColor3ub(255, 255, 255);
          glRotatef(90.0, 0, 0.0, 0.0);
          glutWireTorus(0.001, 14, 100.0, 100.0);
          glPopMatrix();
          glEnable(GL_LIGHTING);
        }

}

void terra(){
  // Desenha a terra
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturas[3].id);
  glPushMatrix();
      glRotatef(translacaoTerra, 0, 1, 0);
      glRotatef(90, 1, 0, 0);
      glTranslatef(160, 0, 0);
      if(nomeLigado){
        glDisable(GL_LIGHTING);
        glRasterPos3i(0, 0, -20);
        glColor3ub(255, 255, 255);
        escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_10, "Terra");
        glEnable(GL_LIGHTING);
      }
      glRotatef(rotaTerra, 0, 0, 1);
      solidSphere(7.0, esferaLados, esferaLados);
      lua();
  glPopMatrix();
}


void marte(){
  // Desenha a marte
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturas[4].id);
  glPushMatrix();
      glRotatef(translacaoMarte, 0, 1, 0);
      glRotatef(90, 1, 0, 0);
      glTranslatef(240, 0, 0);
      if(nomeLigado){
        glDisable(GL_LIGHTING);
        glRasterPos3i(0, 0, -20);
        glColor3ub(255, 255, 255);
        escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_10, "Marte");
        glEnable(GL_LIGHTING);
      }
      glRotatef(rotaMarte, 0, 0, 1);
      glRotatef(90, 1, 0, 0);
      solidSphere(5.0, esferaLados, esferaLados);
  glPopMatrix();

}
void io(){
  // Desenha a lua
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturas[10].id);
  glPushMatrix();
      glRotatef(translacaoLua, 0, 0, 1);
      glRotatef(90, 1, 0, 0);
      glTranslatef(14, 0, 0);
      if(nomeLigado){
        glDisable(GL_LIGHTING);
        glRasterPos3i(0, 0, -20);
        glColor3ub(255, 255, 255);
        escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_10, "io");
        glEnable(GL_LIGHTING);
      }
      glRotatef(rotaLua, 1, 0, 0);
      solidSphere(3.8, esferaLados, esferaLados);
  glPopMatrix();

      if(LigOrbita){ //orbita da lua
        glPushMatrix();
          glDisable(GL_TEXTURE_2D);
          glDisable(GL_LIGHTING);
          glColor3ub(255, 255, 255);
          glRotatef(90.0, 0, 0.0, 0.0);
          glutWireTorus(0.001, 14, 100.0, 100.0);
          glPopMatrix();
          glEnable(GL_LIGHTING);
        }

}
void jupiter(){
  // Desenha jupiter

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturas[5].id);
  glPushMatrix();
      glRotatef(translacaoJupiter, 0, 1, 0);
      glRotatef(90, 1, 0, 0);
      glTranslatef(300, 0, 0);
      if(nomeLigado){
        glDisable(GL_LIGHTING);
        glRasterPos3i(0, 0, -20);
        glColor3ub(255, 255, 255);
        escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_10, "Jupiter");
        glEnable(GL_LIGHTING);
      }
      glRotatef(rotaJupiter, 0, 0, 1);
      solidSphere(10.0, esferaLados, esferaLados);
      io();
  glPopMatrix();
}
void aneis(){
  glPushMatrix();
    glRotatef(50, 1, 0, 0);
    glutWireTorus(0.003, 14, 100.0, 100.0);
    glutWireTorus(0.003, 14.5, 100.0, 100.0);
    glutWireTorus(0.003, 15, 100.0, 100.0);
    glutWireTorus(0.003, 15.5, 100.0, 100.0);
    glutWireTorus(0.003, 16, 100.0, 100.0);
    glutWireTorus(0.003, 16.5, 100.0, 10.0);
    glutWireTorus(0.003, 17, 100.0, 100.0);
    glutWireTorus(0.001, 17.5, 100.0, 100.0);
    glutWireTorus(0.001, 18, 100.0, 100.0);
    glutWireTorus(0.001, 18.5, 100.0, 100.0);
  glPopMatrix();
}
void tita(){
  // Desenha a lua
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturas[11].id);
  glPushMatrix();
      glRotatef(translacaoLua, 0, 0, 1);
      glRotatef(90, 1, 0, 0);
      glTranslatef(19, 0, 0);
      if(nomeLigado){
        glDisable(GL_LIGHTING);
        glRasterPos3i(0, 0, -20);
        glColor3ub(255, 255, 255);
        escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_10, "Titan");
        glEnable(GL_LIGHTING);
      }
      glRotatef(rotaLua, 1, 0, 0);
      solidSphere(3.8, esferaLados, esferaLados);
  glPopMatrix();

      if(LigOrbita){ //orbita da lua
        glPushMatrix();
          glDisable(GL_TEXTURE_2D);
          glDisable(GL_LIGHTING);
          glColor3ub(255, 255, 255);
          glRotatef(90.0, 0, 0.0, 0.0);
          glutWireTorus(0.001, 14, 100.0, 100.0);
          glPopMatrix();
          glEnable(GL_LIGHTING);
        }
}
void saturno(){
  // Desenha saturno
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturas[6].id);
  glPushMatrix();
      glRotatef(translacaoSaturno, 0, 1, 0);
      glRotatef(90, 1, 0, 0);
      glTranslatef(360, 0, 0);
      if(nomeLigado){
        glDisable(GL_LIGHTING);
        glRasterPos3i(0, 0, -20);
        glColor3ub(255, 255, 255);
        escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_10, "Saturno");
        glEnable(GL_LIGHTING);
      }
      glRotatef(rotaSaturno, 0, 0, 1);
      solidSphere(9.3, esferaLados, esferaLados);
      aneis();
      tita();

  glPopMatrix();
}

void urano(){
  // Desenha urano
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturas[7].id);
  glPushMatrix();
      glRotatef(translacaoUrano, 0, 1, 0);
      glRotatef(90, 1, 0, 0);
      glTranslatef(420, 0, 0);
      if(nomeLigado){
        glDisable(GL_LIGHTING);
        glRasterPos3i(0, 0, -20);
        glColor3ub(255, 255, 255);
        escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_10, "Urano");
        glEnable(GL_LIGHTING);
      }
      glRotatef(rotaUrano, 0, 0, 1);
      solidSphere(9.0, esferaLados, esferaLados);

  glPopMatrix();
}

void netuno(){
  // Desenha netuno
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturas[8].id);
  glPushMatrix();
      glRotatef(translacaoNetuno, 0, 1, 0);
      glRotatef(90, 1, 0, 0);
      glTranslatef(480, 0, 0);
      if(nomeLigado){
        glDisable(GL_LIGHTING);
        glRasterPos3i(0, 0, -20);
        glColor3ub(255, 255, 255);
        escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_10, "Netuno");
        glEnable(GL_LIGHTING);
      }
      glRotatef(rotaNetuno, 0, 0, 1);
      solidSphere(8.8, esferaLados, esferaLados);

  glPopMatrix();
}


void orbitas(){
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.001, 60, 100.0, 100.0);
	glutWireTorus(0.001, 120, 100.0, 100.0);
	glutWireTorus(0.001, 160, 100.0, 100.0);
	glutWireTorus(0.001, 240, 100.0, 100.0);
	glutWireTorus(0.001, 300, 100.0, 100.0);
  glutWireTorus(0.001, 360, 100.0, 100.0);
  glutWireTorus(0.001, 420, 100.0, 100.0);
  glutWireTorus(0.001, 480, 100.0, 100.0);
	glPopMatrix();
}

void desenhaCena()
{
    // Propriedades das fontes de luz
    float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDif0[] = { d, d, d, 1.0 };
    float lightSpec0[] = { e, e, e, 1.0 };
    float lightPos0[] = { 0.0, 0.0, 1.0, p };
    float globAmb[] = { m, m, m, 1.0 };

    //Propriedades da fonte de luz LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec0);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);        // Luz ambiente global
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer);// Enable local viewpoint

    // Ativa a fonte de luz branca do sol
    if (SolLigado) {
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHT0);
    }

    // Limpa a tela e o z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


    //esfera de raio 100
    camera.x = 100 * sin(phi) * cos(teta);  //coordenada x denotada em coordenadas esféricas
    camera.z = 100 * sin(phi) * sin(teta); //coordenada z denotada em coordenadas esféricas
    camera.y = 100 * cos(phi);          //coordenada y denotada em coordenadas esféricas

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //define um LookAt diferente para cada modo da câmera
    switch (modoCAM) {
        case PRIMEIRA_PESSOA:

            gluLookAt( xCursor+10, 0, zCursor+10, xCursor+camera.x, camera.y, zCursor+camera.z,0, 1, 0);
            break;

        case ESTATICAL: //lateral
          //600x de dist, 0y, 100z, (0, 0, 0) origem do mundo
            gluLookAt(600, 0, 100, 0, 0, 0, 0, 10, 0);
            break;

        case ESTATICAC://cima
        default:
      //1x de dist, 600y de altura, 1z, (0, 0, 0) origem do mundo
              gluLookAt(1, 600, 1, 0, 0, 0,1, 1, 1);
              break;
          }


    // Desabilita iluminação
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    informacoesTela();
    if (LigOrbita) {
            orbitas();
    }

  //DESENHA SOL

    if(nomeLigado){
      glDisable(GL_LIGHTING);
      glRasterPos3i(0, 50, 0);
      glColor3ub(255, 255, 255);
      escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_24, "Sol");
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturas[0].id);
    glPushMatrix();
          glColor3f(d, d, d);
          glRotatef(50, 0, 1, 0);
          glRotatef(90, 1, 0, 0);
          glTranslatef(0, 0, 0);
          if(SolLigado){
            solidSphere(40.0, esferaLados, esferaLados);
            glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
          }
    glPopMatrix();

    //luz global
    if (LuzLigada) {
        glEnable(GL_LIGHTING);
    }

    // Define (atualiza) o valor do expoente de especularidade
    matShine[0] = s;
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
    glColor3f(1, 1, 1);

    mercurio();
    venus();
    marte();
    urano();
    netuno();
    jupiter();
    saturno();
    terra();
    glutSwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

//capturar posicionamento do mouse
void posicionaCamera(int x, int y){
    // variáveis que guardam o vetor 2D de movimento do mouse na tela
    // xMouse e yMouse são os valores de x e y no frame anterior
    float xChange = x - xMouseCamera;
    float yChange = y - yMouseCamera;

    // Coordenadas esféricas para controlar a câmera.
    // teta e phi guardam a conversão do vetor 2D para um espaço 3D
    // com coordenada esférica
    teta = (teta + xChange/150);
    phi = (phi - yChange/150);

    if(phi >= 180){
      //limite de 180 para o phi
      phi = 180;
    }

    // guarda o x e y do mouse para usar na comparação do próximo frame
    xMouseCamera = x;
    yMouseCamera = y;
}


// callback de atualização
void atualiza(int time) {
    glutPostRedisplay();
    glutTimerFunc(time, atualiza, time);
}

void rotacionaEsfera() {
    //rotação ao redor do sol
    translacaoMercurio += 0.8f;
    translacaoVenus += 0.7f;
    translacaoTerra += 0.5f;
    translacaoMarte += 0.09f;
    translacaoJupiter += 0.08f;
    translacaoSaturno += 0.07;
    translacaoUrano += 0.06;
    translacaoNetuno += 0.05;

    //Luas
    translacaoLua += 0.8f;

    //rotação no eixo
    rotaMercurio += 1.2f;
    rotaVenus += 1.0f;
    rotaTerra += 0.9f;
    rotaLua += 2.0f;
    rotaMarte += .3f;
    rotaJupiter += .2f;
    rotaSaturno += .2f;
    rotaUrano += .2f;
    rotaNetuno += .2f;

    glutPostRedisplay();
}



void redimensiona(int w, int h){
    glEnable(GL_DEPTH_TEST);
    glViewport (0, 0, w, h);                //define a proporção da janela de visualização
    glMatrixMode (GL_PROJECTION);           //define o tipo de matriz de transformação que será utilizada
    glLoadIdentity();
    gluPerspective(80.0, (float)w/(float)h, 0.5, 1000.0);    //funciona como se fosse o glOrtho, mas para o espaço 3D
    glMatrixMode(GL_MODELVIEW);                             //ativa o modo de matriz de visualização para utilizar o LookAt
}

void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 27:    //aperte ESC para fechar
            exit(0);
            break;
        case 'n':
        case 'N':
             nomeLigado = !nomeLigado;
             break;
        case 's':   //andar pelo plano X-Z utilizando W A S D
            xCursor++;
            break;
        case 'w':
            xCursor--;
            break;
        case 'a':
            zCursor++;
            break;
        case 'd':
            zCursor--;
            break;
        case 'l':
        case 'L':
           if (SolLigado) SolLigado = false;
           else SolLigado = true;
           break;
        case 'G':
        case 'g':
            LuzLigada = !LuzLigada;
            break;
         case 'o':
         case 'O':
             LigOrbita = !LigOrbita;
             break;
        case '1':
            modoCAM = PRIMEIRA_PESSOA;
            break;
        case '2':
            modoCAM = ESTATICAL;
            break;
        case '3':
            modoCAM = ESTATICAC;
            break;
        case 'Z':
        case 'z':
            Pausa = !Pausa;
            if (Pausa){
            Mix_ResumeMusic();
            }
            else Mix_PauseMusic();
            break;
        default:
            break;
    }
}

void Console(void)
{
    cout << "Ajuda:" << endl;
    cout << "  Aperte 'l/L' para ligar/desligar a iluminacao do Sol." << endl
         << "  Aperte 'W A S D' para se movimentar na camera em primeira pessoa" << endl
         << "  Aperte 'n/N' para ligar/desligar o nome dos Planetas" << endl
         << "  Aperte 'o/O' para ligar/desligar a Orbita." << endl
         << "  Aperte 'g/G' para ligar/desligar a luz global" << endl
         << "  Aperte '1' para camera em 1a pessoa" << endl
         << "  Aperte '2' para camera lateral" << endl
         << "  Aperte '3' para camera de cima" << endl;
}


int main(int argc, char *argv[]) {
    Console();
    glutInit(&argc, argv);
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition (0, 0);
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);

    glutCreateWindow("TP2");

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutTimerFunc(16, atualiza, 16);

    glutKeyboardFunc(teclado);
    // usada para capturar o posicionamento do mouse
    glutPassiveMotionFunc(posicionaCamera);
    glutIdleFunc(rotacionaEsfera);

    inicializa();
    glutMainLoop();

    Mix_CloseAudio();
    return 0;
}
