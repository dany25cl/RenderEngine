/*
* creado : 8/marzo/2016
*
*
*
*/


#include "auxiliar.h"
#include <windows.h>
#include <gl/glew.h>
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h>
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cstdlib>
#include "camera.h"
#include "camara.h"
#include "BOX.h"

#define SCREEN_SIZE 500,500

//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

int wAncho = 800;						//ancho de la ventana
int wAlto = 800;						//alto de la ventana

bool centerMouse = false;								//controla si el cursor se centra en la ventana o esta libre
int camaraUsada = 1;
Camera miCamara;										//creamos un objeto tipo camara para controlar nuestra camara
Camara miCamara2;

bool keys[256] = { 0 };											//array de boolean con el estado de las teclas (true si esta presionada false si no)

int numObj = 1;													//numero de objetos en la escena
unsigned int *vaos = new unsigned int[numObj * 2];					//array con los vaos de los objetos y su numero de vertices
//unsigned int *texturas = new unsigned int[numObj*texPorObj];			//array con los ids de las texturas de los objetos, se almacenan todas seguidas por cada objeto(Color, emisiva, spec, normal, ambient)

//Matrices
glm::mat4	proj = glm::mat4(1.0f);
glm::mat4	view = glm::mat4(1.0f);
glm::mat4 *model = new glm::mat4[numObj];						//array con las matrices model de los objetos

int maxLights = 5;										//numero maximo de luces
//estructura de los parámetros de cada luz
struct myLightParam{
	float spotN;					//exponente de luz focal
	float spotAngle;				//angulo de luz focal
	glm::vec3 Id;					//Intensidad difusa
	glm::vec3 Is;					//Intensidad especular	
	glm::vec3 D;					//Direccion del foco		
	glm::vec3 colD;					//color difuso	
	glm::vec3 colS;					//color especular
	glm::vec4 pos;					//posicion de la luz, coordenada homogenea a 0 para luz direccional
};
myLightParam *myLights = new myLightParam[maxLights];				//array con los atributos de cada luz
unsigned int *ulightSource = new unsigned int[maxLights * 8];			//array para los identificadores de las variables uniform de las luces.

//////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL	//
//////////////////////////////////////////////////////
//Por definir
unsigned int vshader;
unsigned int fshader;
unsigned int program;

//Variables Uniform 
int uView;
int uModelViewMat;
int uModelViewProjMat;
int uNormalMat;

//Atributos 
int inPos;
int inColor;
int inNormal;
int inTexCoord;
int inTangent;

//Texturas Uniform 
int uColorTex;
int uEmiTex;
int uSpecTex;
int uNormalTex;
int uAmbOcluTex;

bool soportaFA = false;				//indica si la tarjeta grafica donde se ejecute soporta Filtrado Anisotropico
GLfloat maximoFA = 1.0f;				//valor maximo de filtrado anisotropico soportado

//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFuncPress(unsigned char key, int x, int y);
void keyboardFuncUp(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMove(int x, int y);
void FPS();
void keyboardOper();
void DrawGrid();

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initShader(const char *vname, const char *fname);
void initObj();
void destroy();

//VAO 
unsigned int vao;
//VBOs que forman parte del objeto 
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int tangentVBO;
unsigned int triangleIndexVBO;

//Carga el shader indicado, devuele el ID del shader
GLuint loadShader(const char *fileName, GLenum type);

//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
unsigned int loadTex(const char *fileName);

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)	
	initContext(argc, argv);
	initOGL();
	initShader("../shaders/shader.v1.vert", "../shaders/shader.v1.frag");
	//crearLuces();
	initObj();
	

	model[0] = glm::mat4(1.0f);
	/*model[1] = glm::mat4(1.0f);
	model[2] = glm::mat4(1.0f);
	model[3] = glm::mat4(1.0f);
	model[4] = glm::mat4(1.0f);*/

	vaos[0] = vao;
	vaos[1] = cubeNTriangleIndex;
	/*vaos[2] = vao;
	vaos[3] = cubeNTriangleIndex;
	vaos[4] = vaoP;
	vaos[5] = planoNTriangleIndex;
	vaos[6] = vao;
	vaos[7] = cubeNTriangleIndex;
	vaos[8] = vaoPi;
	vaos[9] = pirNTriangleIndex;

	texturas[0] = loadTex("../img/color2.png");
	texturas[1] = loadTex("../img/emissive.png");
	texturas[2] = loadTex("../img/specMap.png");
	texturas[3] = loadTex("../img/normal.png");
	texturas[4] = loadTex("../img/Ambient Oclusion.png");

	texturas[5] = texturas[0];
	texturas[6] = texturas[1];
	texturas[7] = texturas[2];
	texturas[8] = texturas[3];
	texturas[9] = texturas[4];

	texturas[10] = loadTex("../img/suelo.png");
	texturas[11] = loadTex("../img/emissive2.png");
	texturas[12] = loadTex("../img/specMap2.png");
	texturas[13] = loadTex("../img/normal2.png");
	texturas[14] = loadTex("../img/suelo AO.png");

	texturas[15] = texturas[0];
	texturas[16] = texturas[1];
	texturas[17] = texturas[2];
	texturas[18] = texturas[3];
	texturas[19] = texturas[4];

	texturas[20] = loadTex("../img/pirColor.png");
	texturas[21] = loadTex("../img/emissive2.png");
	texturas[22] = loadTex("../img/pirSpec.png");
	texturas[23] = loadTex("../img/pirNormal.png");
	texturas[24] = loadTex("../img/pirAmbO.png");*/

	glutMainLoop();   //va antes del destroy, llama a los eventos en bucle

	destroy();

	return 0;
}

//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitContextVersion(4, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);   //Si usas alguna función de OPENGL marcada deprecated da error
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(wAncho, wAlto);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Render Engine");   //creamos el contexto de opengl

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();   //se intenta inicializar glew
	if (GLEW_OK != err) {     //se comprueba si se ha inicializado bien
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;    //sino se comprueba el error
		exit(-1);
	}
	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	glutReshapeFunc(resizeFunc);					//definir las funciones que manejan el cambio de pantalla
	glutDisplayFunc(renderFunc);					//funcion de pintado
	glutIdleFunc(idleFunc);							//funcion ociosa
	glutKeyboardFunc(keyboardFuncPress);			//funcion de teclado Pressed
	glutKeyboardUpFunc(keyboardFuncUp);				//funcion de teclado UP
	glutMouseFunc(mouseFunc);						//funcion de raton
	glutPassiveMotionFunc(mouseMove);				//Funcion de movimiento del raton
	glutSetCursor(GLUT_CURSOR_NONE);				//Oculta el cursor dentro de la ventana
}

void initOGL(){
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glFrontFace(GL_CCW);							//define el modo de orden de los vertices al definir caras
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//le dice a la etapa de rasterizado que pinte triangulos y los rellene
	glEnable(GL_CULL_FACE);							//activa el culling
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //cambiar por GL_FASTEST para mejor rendimiento
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  //cambiar por GL_FASTEST para mejor rendimiento

	view = glm::mat4(1.0f);
	if (camaraUsada == 1){
		miCamara2.proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);
		miCamara2.Position_Camera(0, 1, 5, 0, 0, 0, 0, 1, 0);
		proj = miCamara2.proj;
		view[3] = glm::vec4(miCamara2.cPos, 1.0);
	}
	else{
		miCamara.setProj(glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f));
		miCamara.setAll(0, 1, 5, 0, 0, 1, 0, 1, 0);
		proj = miCamara.getProj();		
		view[3] = glm::vec4(miCamara.getPos(), 1.0);
	}
	
	GLint num = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &num);
	while (0<--num) {
		std::string FAname = "GL_EXT_texture_filter_anisotropic";
		char *extension = (char*)glGetStringi(GL_EXTENSIONS, num - 1);
		if (FAname.compare(extension) == 0)
			soportaFA = true;
		//std::cout << glGetStringi(GL_EXTENSIONS, num - 1) << std::endl;
	}
	if (soportaFA){
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximoFA);
		std::cout << "Filtro Anisotropico soportado: FAX" << maximoFA << std::endl;
	}
	else {
		std::cout << "Filtro Anisotropico NO soportado." << std::endl;
	}
}

void destroy(){
	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (inPos != -1) glDeleteBuffers(1, &posVBO);
	if (inColor != -1) glDeleteBuffers(1, &colorVBO);
	if (inNormal != -1) glDeleteBuffers(1, &normalVBO);
	if (inTexCoord != -1) glDeleteBuffers(1, &texCoordVBO);
	if (inTangent != -1) glDeleteBuffers(1, &tangentVBO);
	glDeleteBuffers(1, &triangleIndexVBO);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

	/*glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (inPos != -1) glDeleteBuffers(1, &posVBOP);
	if (inColor != -1) glDeleteBuffers(1, &colorVBOP);
	if (inNormal != -1) glDeleteBuffers(1, &normalVBOP);
	if (inTexCoord != -1) glDeleteBuffers(1, &texCoordVBOP);
	if (inTangent != -1) glDeleteBuffers(1, &tangentVBOP);
	glDeleteBuffers(1, &triangleIndexVBOP);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoP);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (inPos != -1) glDeleteBuffers(1, &posVBOPi);
	if (inColor != -1) glDeleteBuffers(1, &colorVBOPi);
	if (inNormal != -1) glDeleteBuffers(1, &normalVBOPi);
	if (inTexCoord != -1) glDeleteBuffers(1, &texCoordVBOPi);
	if (inTangent != -1) glDeleteBuffers(1, &tangentVBOPi);
	glDeleteBuffers(1, &triangleIndexVBOPi);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoPi);*/

	//glDeleteTextures(numObj*texPorObj, &(texturas[0]));

	//glDeleteBuffers(1, &buffer);
}

void initShader(const char *vname, const char *fname){
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	glBindAttribLocation(program, 0, "inPos");
	glBindAttribLocation(program, 1, "inColor");
	glBindAttribLocation(program, 2, "inNormal");
	glBindAttribLocation(program, 3, "inTexCoord");
	glBindAttribLocation(program, 4, "inTangent");

	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		//Calculamos una cadena de error 
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;
		glDeleteProgram(program);
		glDeleteShader(vshader);
		glDeleteShader(fshader);
		exit(-1);
	}
	uNormalMat = glGetUniformLocation(program, "normal");
	uModelViewMat = glGetUniformLocation(program, "modelView");
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");
	uView = glGetUniformLocation(program, "view");

	/*uColorTex = glGetUniformLocation(program, "colorTex");
	uEmiTex = glGetUniformLocation(program, "emiTex");
	uSpecTex = glGetUniformLocation(program, "specTex");
	uNormalTex = glGetUniformLocation(program, "normalTex");
	uAmbOcluTex = glGetUniformLocation(program, "ambOcluTex");*/

	//ulightSource1 = glGetUniformBlockIndex(program, "lightParam");
	//std::cout << "index light 1: " << ulightSource << " index light 2: " << ulightSource1 << " index light 3: " << ulightSource2 << " index light 4: " << ulightSource3 << std::endl;

	/*for (int i = 0; i < maxLights; i++){
		static char name[24] = { 0 };
		sprintf_s(name, "lightSource[%d].spotN", int(i));
		ulightSource[i * 8 + 0] = glGetUniformLocation(program, name);
		sprintf_s(name, "lightSource[%d].spotAngle", int(i));
		ulightSource[i * 8 + 1] = glGetUniformLocation(program, name);
		sprintf_s(name, "lightSource[%d].Id", int(i));
		ulightSource[i * 8 + 2] = glGetUniformLocation(program, name);
		sprintf_s(name, "lightSource[%d].Is", int(i));
		ulightSource[i * 8 + 3] = glGetUniformLocation(program, name);
		sprintf_s(name, "lightSource[%d].D", int(i));
		ulightSource[i * 8 + 4] = glGetUniformLocation(program, name);
		sprintf_s(name, "lightSource[%d].colD", int(i));
		ulightSource[i * 8 + 5] = glGetUniformLocation(program, name);
		sprintf_s(name, "lightSource[%d].colS", int(i));
		ulightSource[i * 8 + 6] = glGetUniformLocation(program, name);
		sprintf_s(name, "lightSource[%d].pos", int(i));
		ulightSource[i * 8 + 7] = glGetUniformLocation(program, name);
	}*/

	inPos = glGetAttribLocation(program, "inPos");      //devuelve 0
	inColor = glGetAttribLocation(program, "inColor");		//devuelve 1
	inNormal = glGetAttribLocation(program, "inNormal");		//devuelve 2
	inTexCoord = glGetAttribLocation(program, "inTexCoord");		//devuelve 3
	inTangent = glGetAttribLocation(program, "inTangent");			//devuelve 4
}

void initObj(){
	glGenVertexArrays(1, &vao);			//crea el identificador del vao
	glBindVertexArray(vao);				//activa el vao, la configuracion siguiente se guarda en este vao

	if (inPos != -1) {
		glGenBuffers(1, &posVBO);										//crea el identificador del vbo
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);							//propiedades de vertices
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float)* 3,
			cubeVertexPos, GL_STATIC_DRAW);
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos);
	}
	if (inColor != -1) {
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float)* 3,
			cubeVertexColor, GL_STATIC_DRAW);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}
	if (inNormal != -1) {
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float)* 3,
			cubeVertexNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}
	if (inTexCoord != -1) {
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float)* 2,
			cubeVertexTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}
	if (inTangent != -1) {
		glGenBuffers(1, &tangentVBO);
		glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float)* 3,
			cubeVertexTangent, GL_STATIC_DRAW);
		glVertexAttribPointer(inTangent, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTangent);
	}
	glGenBuffers(1, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		cubeNTriangleIndex*sizeof(unsigned int)* 3, cubeTriangleIndex,
		GL_STATIC_DRAW);

}


GLuint loadShader(const char *fileName, GLenum type){
	unsigned int fileLen;
	char *source = loadStringFromFile(fileName, fileLen);

	////////////////////////////////////////////// 
	//Creación y compilación del Shader 
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen);
	glCompileShader(shader);
	delete source;

	//Comprobamos que se compiló bien 
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);  //guarda el resultado de la compilacion en una variable
	if (!compiled) {			//comprueba si se compilo bien
		//Calculamos una cadena de error 
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << fileName << std::endl;			//imprime en que fichero da error
		std::cout << "Error: " << logString << std::endl;	//imprime el error
		delete logString;
		glDeleteShader(shader);
		exit(-1);
	}

	return shader;
}

unsigned int loadTex(const char *fileName){
	unsigned char *map;
	unsigned int w, h;
	map = loadTexture(fileName, w, h);
	if (!map) {
		std::cout << "Error cargando el fichero: " << fileName << std::endl;
		exit(-1);
	}

	unsigned int texId;						//sube la textura a la tarjeta grafica
	glGenTextures(1, &texId);				//identifica el tipo de textura, luego no es facil cambiarlo
	glBindTexture(GL_TEXTURE_2D, texId);		//reserva el espacio y sube los datos
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,		//hace referencia al formato interno, declaramos el tipo
		GL_UNSIGNED_BYTE, (GLvoid*)map);						//de datos que vamos a usar, rgba etc

	delete[] map;			//liberar el array, liberar espacio de la textura

	glGenerateMipmap(GL_TEXTURE_2D);  //genera los mip maps

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,				//filtrado trilineal
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//comportamiento interpolacion lineal
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		//comportamiento al exceder la textura, hace clamp al valor mas cercano
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maximoFA);		//configuramos la cantidad de filtrado anisotropico

	return texId;
}


void renderFunc(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //limpia los buffers de color y profundidad	
	FPS();
	glUseProgram(program); // pintado del objeto!!!! 

	//guardamos en view las propiedades de la camara	
	if (camaraUsada == 1)
		view = miCamara2.getView();
	else
		view = miCamara.getView();	

	for (int i = 0; i < numObj; i++){
		glm::mat4 modelView = view * model[i];
		glm::mat4 modelViewProj = proj * view * model[i];
		glm::mat4 normal = glm::transpose(glm::inverse(modelView));

		if (uView != -1)
			glUniformMatrix4fv(uView, 1, GL_FALSE, &(view[0][0]));
		if (uModelViewMat != -1)
			glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE, &(modelView[0][0]));
		if (uModelViewProjMat != -1)
			glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE, &(modelViewProj[0][0]));
		if (uNormalMat != -1)
			glUniformMatrix4fv(uNormalMat, 1, GL_FALSE, &(normal[0][0]));

		glBindVertexArray(vaos[i * 2]);
		glDrawElements(GL_TRIANGLES, vaos[i * 2 + 1] * 3,
			GL_UNSIGNED_INT, (void*)0);
	}	
	
	glUseProgram(NULL);		//desactiva el programa	
	DrawGrid();
	glutSwapBuffers();		//cambia el bufer frontal por el de pintado		
}

void resizeFunc(int width, int height){
	if (height == 0)
	{
		height = 1;
	}
	glViewport(0, 0, width, height);
	wAncho = width;
	wAlto = height;
	if (camaraUsada == 1){
		miCamara2.proj=glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 100.0f);
		proj = miCamara2.proj;
	}
	else{
		miCamara.setProj(glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 100.0f));
		proj = miCamara.getProj();
	}
	
	glutPostRedisplay();
}

void idleFunc(){
	keyboardOper();
	//Movimiento de la camara	
	if (camaraUsada == 1){
		if (centerMouse)
			miCamara2.Mouse_Move(wAncho, wAlto);
	}else
		miCamara.updateView();	

	static float angle = 0.0f;	
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.01f;

	model[0] = glm::mat4(1.0f);
	model[0] = glm::translate(model[0], glm::vec3(0.0f, 0.0f, 0.0f));
	model[0] = glm::rotate(model[0], angle, glm::vec3(1.0f, 1.0f, 0.0f));

	glutPostRedisplay();
}

void DrawGrid()
{
	for (float i = -500; i <= 500; i += 5)
	{
		glBegin(GL_LINES);
		glColor3ub(150, 190, 150);
		glVertex3f(-500, 0, i);
		glVertex3f(500, 0, i);
		glVertex3f(i, 0, -500);
		glVertex3f(i, 0, 500);
		glEnd();
	}
}

void keyboardFuncPress(unsigned char key, int x, int y){
	//std::cout << "pulsando " << key << std::endl << std::endl;
	keys[key] = true;
}

void keyboardFuncUp(unsigned char key, int x, int y){
	//std::cout << "soltando " << key << std::endl << std::endl;
	if (keys[' '] == true)
	if (centerMouse == true)
		centerMouse = false;
	else centerMouse = true;
	keys[key] = false;
}

void keyboardOper(){	

	if (keys[27]){
		exit(-1);
	}

	float speed;
	if (camaraUsada == 1)
		speed = 0.04f;
	else
		speed = 0.2;

	if (keys['w'] || keys['W']){
		miCamara.forward(speed);
		miCamara2.Move_Camera(speed);
	}

	if (keys['s'] || keys['S']){
		miCamara.forward(-speed);
		miCamara2.Move_Camera(-speed);
	}

	if (keys['a'] || keys['A']){
		miCamara.strife(-speed);
		miCamara2.Strafe_Camera(-speed);
	}	

	if (keys['d'] || keys['D']){
		miCamara.strife(speed);
		miCamara2.Strafe_Camera(speed);
	}
	
	if (keys['t'] || keys['T']){
		miCamara.toString();
	}

	if (keys['f'] || keys['F']){
		miCamara.fly(speed);
		miCamara2.Move_Up_Camera(speed);
	}

	if (keys['c'] || keys['C']){
		miCamara.fly(-speed);
		miCamara2.Move_Up_Camera(-speed);
	}
	if (keys['q'] || keys['Q']){
		miCamara2.Rotate_View(-speed);
	}
	if (keys['e'] || keys['E']){
		miCamara2.Rotate_View(speed);
	}
}

void mouseFunc(int button, int state, int x, int y){}

void mouseMove(int x, int y){
	if (centerMouse && camaraUsada !=1)
		miCamara.rotate(x, y, wAncho, wAlto);	
}

void FPS()					// This function calculates FPS
{
	static float fps = 0.0f;
	static float previousTime = 0.0f;
	static char  strFPS[30] = { 0 };
	float currentTime = (GetTickCount() * 0.001f);

	++fps;	// Increment the FPS counter

	if (currentTime - previousTime > 1.0f)
	{
		previousTime = currentTime;
		sprintf_s(strFPS, "Render Engine. FPS: %d", int(fps));
		glutSetWindowTitle(strFPS);		
		//std::cout << strFPS << std::endl;
		//SetWindowText(hWndw, strFPS);
		fps = 0.0f;
	}
}
