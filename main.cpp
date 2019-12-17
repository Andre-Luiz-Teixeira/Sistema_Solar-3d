/// trabalho feito por Andre Teixeira, Eduardo Senra, Matheus Toledo

#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <GL/glut.h>
#include "imageloader.h"

// Variáveis para controles de navegação
GLfloat angle, fAspect;
GLfloat rotX, rotY, rotX_ini, rotY_ini;
GLfloat obsX, obsY, obsZ, obsX_ini, obsY_ini, obsZ_ini;
int x_ini,y_ini,bot;
GLint tempo =40;
GLint zoom=0;
GLint panX = 0;
GLint pany = 0;

GLUquadric *quad0, *quad1, *quad2, *quad3;
GLuint _texturasol, _texturaterra, _texturalua, _texturauniverso;

//para criar uma animação
GLboolean animacao = false; //T = continua animação   F = pausa a animação

//variaveis para translação da terra
GLfloat transTerra = 50.0;
GLfloat transLua = 10.0;

GLfloat angulo_sol = 0;
GLfloat angulo_lua = 0;
GLfloat angulo_terra = 0;

void var(){
     system("cls");
        std::cout<<"terra: "<<transTerra<<std::endl;
        std::cout<<"lua: "<< transLua<<std::endl;

        std::cout<<"angle"<< angle<<std::endl;

        std::cout<<"rotX: "<<rotX<<std::endl;
        std::cout<<"roty: "<<rotY<<std::endl;

        std::cout<<"rotX_ini: "<<rotX_ini<<std::endl;
        std::cout<<"roty_ini: "<<rotY_ini<<std::endl;

        std::cout<<"obsX: "<<obsX<<std::endl;
std::cout<<"obsY: "<<obsY<<std::endl;
std::cout<<"obsZ: "<<obsZ<<std::endl;
std::cout<<"obsX_ini: "<<obsX_ini<<std::endl;
std::cout<<"obsY_ini: "<<obsY_ini<<std::endl;
std::cout<<"obsZ_ini: "<<obsZ_ini<<std::endl;
}


// Define um vértice
// Define um vértice
struct VERT{
	float x,y,z;	// posição no espaço
};

// Define uma face
struct FACE{
	int total;	// total de vértices
	int ind[4];	// índices para o vetor de vértices
};

// Define um objeto 3D
struct OBJ{
	VERT *vertices;		// aponta para os vértices
	FACE *faces;		// aponta para as faces
	int total_faces;	// total de faces no objeto
};

// Definição dos vértices
VERT vertices[] = {

	//frente

	{ -2, 3, 0 },       // v0
	{  -2, -3, 0 },     // v1
	{  2, -3,  0 },     // v2
	{ 2, 3,  0 },       // v3

    {  0, 5,  -1 },     // v4           topo sup


	//trás

    { -2, 3, -2 },       // v5
	{  -2, -3, -2 },     // v6
	{  2, -3,  -2 },     // v7
	{ 2, 3,  -2 },       // v8

    {  0, -5,  -1 }     // v9           topo inf

};

// Definição das faces
FACE faces[] = {
	{ 4, { 0,1,2,3 }},	// frente
	{ 4, { 8,7,6,5 }},	// tras
	{ 4, { 1,6,7,2 }},	// base inf
	{ 4, { 5,0,3,8 }},	// base sup
	{ 4, { 3,2,7,8 }},	// lado dianteiro
    { 4, {5,6,1,0  }},  // lado esquerdo
    { 3, {4,0,3,-1 }},  // topo sup frente
    { 3, {4,8,5,-1 }},  //  topo sup tras
    { 3, {2,1,9,-1 }},  // topo inf frente
    { 3, {9,6,7,-1 }}  //  topo inf tras

};


// Finalmente, define o objeto pirâmide
OBJ piramide = {
	vertices, faces, 10
};

// Desenha um objeto em wireframe
void DesenhaObjetoWireframe(OBJ *objeto)
{
    // Percorre todas as faces
    for(int f=0; f < objeto->total_faces; f++)
    {
        glBegin(GL_POLYGON);
        // Percorre todos os vértices da face
        for(int v=0; v < objeto->faces[f].total; v++)
            glVertex3f(objeto->vertices[objeto->faces[f].ind[v]].x,
                       objeto->vertices[objeto->faces[f].ind[v]].y,
                       objeto->vertices[objeto->faces[f].ind[v]].z);
            glColor3f(1.0f, 1.0f, 1.0f);
        glEnd();
    }
}

void DefineIluminacao (void)
{
    GLfloat luzAmbiente[4]= {0.5, 0.5, 0.5, 1.0};
    GLfloat luzDifusa[4]= {1.0, 1.0, 1.0, 0.0};	  // "cor"
    GLfloat luzEspecular[4]= {1.0, 1.0, 1.0, 1.0}; // "brilho"
    GLfloat posicaoLuz[4]= { 0.0, 1.0, 0.0, 1.0 };

    // Capacidade de brilho do material
    GLfloat especularidade[4]= {1.0,1.0,1.0,0.0};
    GLint especMaterial=90;


    // Habilita a luz de número 0
    glEnable(GL_LIGHT0);
    // Define a refletância do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    // Define a concentração do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);



		glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
		glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
		glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
		//glLightfv(GL_LIGHT0+i,GL_SPOT_DIRECTION,dirLuz[i]);
		//glLightf(GL_LIGHT0+i,GL_SPOT_CUTOFF,91.0);
		//glLightf(GL_LIGHT0+i,GL_SPOT_EXPONENT,0.0);

}

// Função callback de redesenho da janela de visualização
void Desenha(void)
{
    // Limpa a janela de visualização com a cor
    // de fundo definida previamente
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DefineIluminacao();
    glEnable(GL_TEXTURE_2D);

    glDisable(GL_LIGHTING);

    //desenha o universo
        glBindTexture(GL_TEXTURE_2D, _texturauniverso);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gluQuadricTexture(quad0,1);
        gluSphere(quad0,1000,100,100);

    // Desenha a sol
    glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, _texturasol);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glRotatef(angulo_terra, 0,1,0);
        gluQuadricTexture(quad1,1);
        gluSphere(quad1,20,100,100);
        glEnable(GL_LIGHTING);
        glPushMatrix();
        // Desenha a terra
            glBindTexture(GL_TEXTURE_2D, _texturaterra);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
           // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTranslated(0, 0, transTerra);
            glRotated(angulo_terra, 0, 1, 0);
            gluQuadricTexture(quad2,1);
            gluSphere(quad2,5,90,90);

            glPushMatrix();

            //desenha lua
                glBindTexture(GL_TEXTURE_2D, _texturalua);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
               // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTranslated(10, 0, 0.9);
                glRotated(angulo_lua, 0, 1, 0);
                gluQuadricTexture(quad3,1);
                gluSphere(quad3,1,100,100);

            glPopMatrix();

                glTranslated(-7.7, 0.5 , 1.0);
                glScaled(0.1, 0.1, 0.1);
                DesenhaObjetoWireframe(&piramide);

        glPopMatrix();
    glPopMatrix();




    var();
    //

    // Executa os comandos OpenGL
    glutSwapBuffers();
}

// Função usada para especificar a posição do observador virtual
void PosicionaObservador(void)
{
    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();
    // Posiciona e orienta o observador
    glTranslatef(-obsX+panX,-obsY+pany,-obsZ);
    glRotatef(rotX,1,0,0);
    glRotatef(rotY,0,1,0);
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
    // Especifica sistema de coordenadas de projeção
    glMatrixMode(GL_PROJECTION);
    // Inicializa sistema de coordenadas de projeção
    glLoadIdentity();

    // Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
    gluPerspective(angle,fAspect,0.1,1200);

    PosicionaObservador();
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Para previnir uma divisão por zero
    if ( h == 0 )
        h = 1;

    // Especifica as dimensões da viewport
    glViewport(0, 0, w, h);

    // Calcula a correção de aspecto
    fAspect = (GLfloat)w/(GLfloat)h;

    EspecificaParametrosVisualizacao();
}

void Anima(int value)
{
   /* if(rotY <= 360)
        rotY = rotY +1;
    else
        rotY = 0;

        */
    if (angulo_terra<=360)
        angulo_terra = angulo_terra + 1;
    else
        angulo_terra=0;

    if (angulo_lua<=360)
        angulo_lua = angulo_lua + 1;
    else
        angulo_lua=0;


    PosicionaObservador();
     Desenha();

     if(animacao)
       glutTimerFunc(5,Anima,600);
}

// Função callback chamada para gerenciar eventos de teclas normais (ESC)
void Teclado (unsigned char tecla, int x, int y)
{
    if(tecla==27) // ESC ?
    {
        // Libera memória e finaliza programa
        exit(0);
    }

    if(tecla == '=')
    {
        tempo = tempo+2;
    }

    if(tecla == '-')
    {
        tempo = tempo - 2;
    }

    if (tecla == '1')
        zoom = zoom +10;
    if (tecla == '2')
        zoom = zoom -10;

    if(tecla == '.')
    {
        animacao = !animacao;

        if(animacao)
            glutTimerFunc(tempo,Anima,1);
    }


    EspecificaParametrosVisualizacao();
    glutPostRedisplay();

}


// Função callback para tratar eventos de teclas especiais
void TeclasEspeciais (int tecla, int x, int y)
{
    switch (tecla)
    {
    case GLUT_KEY_HOME:
        if(angle>=10)
            angle -=5;
        break;
    case GLUT_KEY_END:
        if(angle<=150)
            angle +=5;
        break;
    case GLUT_KEY_LEFT:
        panX = panX - 1;
        break;
    case GLUT_KEY_RIGHT:
        panX = panX + 1;
        break;
    case GLUT_KEY_UP:
        pany = pany + 1;
        break;
    case GLUT_KEY_DOWN:
        pany = pany - 1;
        break;
    }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}


// Função callback para eventos de botões do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
    if(state==GLUT_DOWN)
    {
        // Salva os parâmetros atuais
        x_ini = x;
        y_ini = y;
        obsX_ini = obsX;
        obsY_ini = obsY;
        obsZ_ini = obsZ;
        rotX_ini = rotX;
        rotY_ini = rotY;
        bot = button;
    }
    else
        bot = -1;
}

// Função callback para eventos de movimento do mouse
#define SENS_ROT	10.0
#define SENS_OBS	15.0
#define SENS_TRANSL	30.0
void GerenciaMovim(int x, int y)
{
    // Botão esquerdo ?
    if(bot==GLUT_LEFT_BUTTON)
    {
        // Calcula diferenças
        int deltax = x_ini - x;
        int deltay = y_ini - y;
        // E modifica ângulos
        rotY = rotY_ini - deltax/SENS_ROT;
        rotX = rotX_ini - deltay/SENS_ROT;
    }
    // Botão direito ?
    else if(bot==GLUT_RIGHT_BUTTON)
    {
        // Calcula diferença
        int deltaz = y_ini - y;
        // E modifica distância do observador
        obsZ = obsZ_ini + deltaz/SENS_OBS;
    }
    // Botão do meio ?
    else if(bot==GLUT_MIDDLE_BUTTON)
    {
        // Calcula diferenças
        int deltax = x_ini - x;
        int deltay = y_ini - y;
        // E modifica posições
        obsX = obsX_ini + deltax/SENS_TRANSL;
        obsY = obsY_ini - deltay/SENS_TRANSL;
    }
    PosicionaObservador();
    glutPostRedisplay();
}

GLuint loadTexture(Image* image){
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height,0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    return textureId;
}
// Função responsável por inicializar parâmetros e variáveis
void Inicializa (void)
{
    glEnable(GL_TEXTURE);
    // Define a cor de fundo da janela de visualização como branca
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

    // Inicializa a variável que especifica o ângulo da projeção
    // perspectiva
    angle=100;

    // Inicializa as variáveis usadas para alterar a posição do
    // observador virtual
    rotX = 0;
    rotY = 0;
    obsX = obsY = 0;
    obsZ = 80;

    quad0 = gluNewQuadric();
    Image* image = loadBMP("C:/Users/eduar/Desktop/sistemasolar/texturas/universo.bmp");
    _texturauniverso = loadTexture (image);


    quad1 = gluNewQuadric();
    image = loadBMP("C:/Users/eduar/Desktop/sistemasolar/texturas/sol.bmp");
    _texturasol = loadTexture (image);

    quad2 = gluNewQuadric();
    image = loadBMP("C:/Users/eduar/Desktop/sistemasolar/texturas/terra.bmp");
   _texturaterra = loadTexture (image);

    quad3 = gluNewQuadric();
    image = loadBMP("C:/Users/eduar/Desktop/sistemasolar/texturas/lua.bmp");
    _texturalua = loadTexture (image);
}

// Programa Principal
int main(void)
{
    // Define do modo de operação da GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Especifica a posição inicial da janela GLUT
    glutInitWindowPosition(0,0);

    // Especifica o tamanho inicial em pixels da janela GLUT
    glutInitWindowSize(1600,900);

    // Cria a janela passando como argumento o título da mesma
    glutCreateWindow("Visão terra");

    // Registra a função callback de redesenho da janela de visualização
    glutDisplayFunc(Desenha);

    // Registra a função callback de redimensionamento da janela de visualização
    glutReshapeFunc(AlteraTamanhoJanela);

    // Registra a função callback para eventos de botões do mouse
    glutMouseFunc(GerenciaMouse);

    // Registra a função callback para eventos de movimento do mouse
    glutMotionFunc(GerenciaMovim);

    // Registra a função callback para tratamento das teclas normais
    glutKeyboardFunc (Teclado);

    // Registra a função callback para tratamento das teclas especiais
    glutSpecialFunc (TeclasEspeciais);

    // Chama a função responsável por fazer as inicializações
    Inicializa();

    // Inicia o processamento e aguarda interações do usuário
    glutMainLoop();

    return 0;
}

