#include "menuBar.h"

extern GLFWwindow* glfwWindow;
extern int gWidth, gHeight;
extern string route;
extern CMenuBar * menuBar;
extern int picked;
extern vector <CObject *> objects;
extern GLfloat factorS;
extern bool ortho, persp, zb, bfcull;
extern float lightPos[3];
extern float ld[3];
extern float ls[3];
extern float ambient[3];

void TW_CALL listarObjetos(void *clientData){
	picked = (int)clientData;
	setUserInterface();
	refresh(picked);
//	FadiGluLookAt(0, 0, 0, objects.back()->xmid, objects.back()->ymid, objects.back()->zmid, 0, 1, 0);
}

void TW_CALL cargarArchivo(void *clientData){
	loadFile();
	if (route != "-1"){
		string rt, name, def;
		char * tok = strtok(&route[0u], "\\");
		while (tok != NULL){
			name = tok;
			tok = strtok(NULL, "\\");
		}

		rt = std::to_string(menuBar->indexObj) + name;
		def = "group='Objetos' label='" + std::to_string(menuBar->indexObj) + name + "'";
		TwAddButton(menuBar->menuBar, rt.c_str(), listarObjetos, (void*)(menuBar->indexObj), def.c_str());
		menuBar->indexObj++;
	}
}

void TW_CALL points(void *clientData){
	menuBar->setFMode(GL_POINT);
}

void TW_CALL lines(void *clientData){
	menuBar->setFMode(GL_LINE);
}

void TW_CALL fill(void *clientData){
	menuBar->setFMode(GL_FILL);
}

void TW_CALL trasSetX(const void *value, void *clientData){
	menuBar->setTrasX(*(const float*)value);
}

void TW_CALL trasGetX(void *value, void *clientData)
{
	*(float *)value = menuBar->getTrasX();
}

void TW_CALL trasSetY(const void *value, void *clientData){
	menuBar->setTrasY(*(const float*)value);
}

void TW_CALL trasGetY(void *value, void *clientData)
{
	*(float *)value = menuBar->getTrasY();
}

void TW_CALL trasSetZ(const void *value, void *clientData){
	menuBar->setTrasZ(*(const float*)value);
}

void TW_CALL trasGetZ(void *value, void *clientData)
{
	*(float *)value = menuBar->getTrasZ();
}

void TW_CALL factorSetX(const void *value, void *clientData){
	menuBar->setFactorX(*(const float*)value);
}

void TW_CALL factorGetX(void *value, void *clientData)
{
	*(float *)value = menuBar->getFactorX();
}

void TW_CALL factorSetY(const void *value, void *clientData){
	menuBar->setFactorY(*(const float*)value);
}

void TW_CALL factorGetY(void *value, void *clientData)
{
	*(float *)value = menuBar->getFactorY();
}

void TW_CALL factorSetZ(const void *value, void *clientData){
	menuBar->setFactorZ(*(const float*)value);
}

void TW_CALL factorGetZ(void *value, void *clientData)
{
	*(float *)value = menuBar->getFactorZ();
}

void TW_CALL viewPersp(void *clientData){
	persp = true;
	ortho = false;
	if(picked > -1)
		refresh(picked);
}

void TW_CALL viewOrtho(void *clientData){
	ortho = true;
	persp = false;
	if(picked > -1)
		refresh(picked);
}

void TW_CALL viewZB(void *clientData){
	if (zb){
		zb = false;
	}
	else{
		zb = true;
	}
}

void TW_CALL viewBFCull(void *clientData){
	if (bfcull){
		bfcull = false;
	}
	else{
		bfcull = true;
	}
}

void TW_CALL lightPosXSet(const void *value, void *clientData)
{
	lightPos[0] = (*(const float*)value);

}

void TW_CALL lightPosXGet(void *value, void *clientData)
{
	*(float *)value = lightPos[0];  // for instance

}

void TW_CALL lightPosYSet(const void *value, void *clientData)
{
	lightPos[1] = (*(const float*)value);

}

void TW_CALL lightPosYGet(void *value, void *clientData)
{
	*(float *)value = lightPos[1];  // for instance

}

void TW_CALL lightPosZSet(const void *value, void *clientData)
{
	lightPos[2] = (*(const float*)value);

}

void TW_CALL lightPosZGet(void *value, void *clientData)
{
	*(float *)value = lightPos[2];  // for instance

}

void TW_CALL ldSet(const void *value, void *clientData)
{
	ld[0] = ((float*)value)[0];
	ld[1] = ((float*)value)[1];
	ld[2] = ((float*)value)[2];
}

void TW_CALL ldGet(void *value, void *clientData)
{
	((GLfloat*)value)[0] = ld[0];
	((GLfloat*)value)[1] = ld[1];
	((GLfloat*)value)[2] = ld[2];
}

void TW_CALL lsSet(const void *value, void *clientData)
{
	ls[0] = ((float*)value)[0];
	ls[1] = ((float*)value)[1];
	ls[2] = ((float*)value)[2];
}

void TW_CALL lsGet(void *value, void *clientData)
{
	((GLfloat*)value)[0] = ls[0];
	((GLfloat*)value)[1] = ls[1];
	((GLfloat*)value)[2] = ls[2];
}

void TW_CALL ambientSet(const void *value, void *clientData)
{
	ambient[0] = ((float*)value)[0];
	ambient[1] = ((float*)value)[1];
	ambient[2] = ((float*)value)[2];
}

void TW_CALL ambientGet(void *value, void *clientData)
{
	((GLfloat*)value)[0] = ambient[0];
	((GLfloat*)value)[1] = ambient[1];
	((GLfloat*)value)[2] = ambient[2];
}

void TW_CALL setDiffuseObj(const void *value, void *clientData)
{
	menuBar->setDiffuseObject(((float*)value)[0], ((float*)value)[1], ((float*)value)[2]);
}

void TW_CALL getDiffuseObj(void *value, void *clientData)
{
	((GLfloat*)value)[0] = menuBar->getDiffuseObject()[0];
	((GLfloat*)value)[1] = menuBar->getDiffuseObject()[1];
	((GLfloat*)value)[2] = menuBar->getDiffuseObject()[2];
}

void TW_CALL setSpecularObj(const void *value, void *clientData)
{
	menuBar->setSpecularObject(((float*)value)[0], ((float*)value)[1], ((float*)value)[2]);
}

void TW_CALL getSpecularObj(void *value, void *clientData)
{
	((GLfloat*)value)[0] = menuBar->getSpecularObject()[0];
	((GLfloat*)value)[1] = menuBar->getSpecularObject()[1];
	((GLfloat*)value)[2] = menuBar->getSpecularObject()[2];
}

void TW_CALL setAmbientObj(const void *value, void *clientData)
{
	menuBar->setAmbientObject(((float*)value)[0], ((float*)value)[1], ((float*)value)[2]);
}

void TW_CALL getAmbientObj(void *value, void *clientData)
{
	((GLfloat*)value)[0] = menuBar->getAmbientObject()[0];
	((GLfloat*)value)[1] = menuBar->getAmbientObject()[1];
	((GLfloat*)value)[2] = menuBar->getAmbientObject()[2];
}

CMenuBar::CMenuBar(){
	setFigureColor(0.f, 1.f, 0.f);
	setFigureColorBB(1.f, 1.f, 1.f);
	factorX = 1; factorY = 1; factorZ = 1;
	trasX = 1; trasY = 1; trasZ = 1;
	indexObj = 0;
	quat[0] = 0; quat[1] = 0; quat[2] = 0; quat[3] = 1;
	mShow = false;
	ld[0] = 1.0f; ld[1] = 0.0f; ld[2] = 0.0f;
	ls[0] = 0.0f; ls[1] = 0.0f; ls[2] = 1.0f;
	menuBar = TwNewBar("Menu");
	TwDefine("Menu refresh = '0.0001f'");
	TwDefine("Menu resizable = false");
	TwDefine("Menu fontresizable = false");
	TwDefine("Menu movable = false");
	TwDefine("Menu visible = true");
	TwDefine("Menu position = '20 20'");
	TwDefine("Menu size = '270 500'");
	TwAddButton(menuBar, "CargarArchivo", cargarArchivo, NULL, "label='Cargar Archivo'");
	TwAddSeparator(menuBar, "Views", "group='Vista'");
	TwAddButton(menuBar, "pers", viewPersp, NULL, "label = 'Perspective' group = 'Vista'");
	TwAddButton(menuBar, "ortho", viewOrtho, NULL, "label = 'Orthogonal' group = 'Vista'");
	TwAddButton(menuBar, "bfcull", viewBFCull, NULL, "label = 'BackFace Culling' group = 'Vista'");
	TwAddButton(menuBar, "zb", viewZB, NULL, "label = 'Z-Buffer' group = 'Vista'");
	//TwAddVarRW(menuBar, "colorF", TW_TYPE_COLOR3F, &mFigureColor[0], "label = 'Color Relleno'");
	TwAddVarRW(menuBar, "colorB", TW_TYPE_COLOR3F, &bbFigureColor[0], "label = 'Color Bounding Box' group = 'Bounding Box'");
	TwAddVarRW(menuBar, "show", TW_TYPE_BOOLCPP, &mShow, "label = 'Mostrar Bounding Box' group = 'Bounding Box'");
	TwAddSeparator(menuBar, "normals", "group='Normales'");
	TwAddVarRW(menuBar, "showFN", TW_TYPE_BOOLCPP, &showFN, "label = 'Mostrar por Caras' group = 'Normales'");
	TwAddVarRW(menuBar, "colorFN", TW_TYPE_COLOR3F, &colorFN[0], "label = 'Color por Caras' group = 'Normales'");
	TwAddVarRW(menuBar, "showVN", TW_TYPE_BOOLCPP, &showVN, "label = 'Mostrar por Vertices' group = 'Normales'");
	TwAddVarRW(menuBar, "colorVN", TW_TYPE_COLOR3F, &colorVN[0], "label = 'Color por Vertices' group = 'Normales'");
	TwAddSeparator(menuBar, "StyleF", "group='Estilo'");
	TwAddButton(menuBar, "GL_POINT", points, NULL, "group='Estilo' label='Puntos'");
	TwAddButton(menuBar, "GL_LINE", lines, NULL, "group='Estilo' label='Lineas'");
	TwAddButton(menuBar, "GL_FILL", fill, NULL, "group='Estilo' label='Relleno'");
	TwAddSeparator(menuBar, "Transformations", "group='Transformaciones'");
	TwAddVarCB(menuBar, "TrasladarX", TW_TYPE_FLOAT, trasSetX, trasGetX, &trasX, "group='Transformaciones' step=0.001");
	TwAddVarCB(menuBar, "TrasladarY", TW_TYPE_FLOAT, trasSetY, trasGetY, &trasY, "group='Transformaciones' step=0.001");
	TwAddVarCB(menuBar, "TrasladarZ", TW_TYPE_FLOAT, trasSetZ, trasGetZ, &trasZ, "group='Transformaciones' step=0.001");
	TwAddVarCB(menuBar, "EscalarX", TW_TYPE_FLOAT, factorSetX, factorGetX, &factorX, "group='Transformaciones' min=0 step=0.001");
	TwAddVarCB(menuBar, "EscalarY", TW_TYPE_FLOAT, factorSetY, factorGetY, &factorY, "group='Transformaciones' min=0 step=0.001");
	TwAddVarCB(menuBar, "EscalarZ", TW_TYPE_FLOAT, factorSetZ, factorGetZ, &factorZ, "group='Transformaciones' min=0 step=0.001");
	TwAddVarRW(menuBar, "Rotation", TW_TYPE_QUAT4F, quat, "group='Transformaciones'");	
	TwAddSeparator(menuBar, "Iluminacion", "group = 'Escena'");
	TwAddVarCB(menuBar, "Light Pos X", TW_TYPE_FLOAT, lightPosXSet, lightPosXGet, &lightPos[0], "group='Luz' group='Escena' step=0.001");
	TwAddVarCB(menuBar, "Light Pos Y", TW_TYPE_FLOAT, lightPosYSet, lightPosYGet, &lightPos[1], "group='Luz' group='Escena' step=0.001");
	TwAddVarCB(menuBar, "Light Pos Z", TW_TYPE_FLOAT, lightPosZSet, lightPosZGet, &lightPos[2], "group='Luz' group='Escena' step=0.001");
	TwAddVarCB(menuBar, "Diffuse Light", TW_TYPE_COLOR3F, ldSet, ldGet, &ld[0], "group='Escena'");
	TwAddVarCB(menuBar, "Specular Light", TW_TYPE_COLOR3F, lsSet, lsGet, &ls[0], "group='Escena'");
	TwAddVarCB(menuBar, "Ambient Light", TW_TYPE_COLOR3F, ambientSet, ambientGet, &ambient[0], "group='Escena'");

	TwAddVarCB(menuBar, "Diffuse Object", TW_TYPE_COLOR3F, setDiffuseObj, getDiffuseObj, &diffuseObject[0], "group='Luz del Objeto'");
	TwAddVarCB(menuBar, "Specular Object", TW_TYPE_COLOR3F, setSpecularObj, getSpecularObj, &specularObject[0], "group='Luz del Objeto'");
	TwAddVarCB(menuBar, "Ambient Object", TW_TYPE_COLOR3F, setAmbientObj, getAmbientObj, &ambientObject[0], "group='Luz del Objeto'");
	TwAddSeparator(menuBar, "Objects", "group='Objetos'");
}

CMenuBar::~CMenuBar(){
//
}

void CMenuBar::reshape(){
	TwWindowSize(gWidth, gHeight);
}

void CMenuBar::setFigureColor(GLfloat r, GLfloat g, GLfloat b){
	mFigureColor[0] = r;
	mFigureColor[1] = g;
	mFigureColor[2] = b;
}

GLfloat* CMenuBar::getFigureColor(){
	return mFigureColor;
}

void CMenuBar::setFigureColorBB(GLfloat r, GLfloat g, GLfloat b){
	bbFigureColor[0] = r;
	bbFigureColor[1] = g;
	bbFigureColor[2] = b;
}

GLfloat* CMenuBar::getFigureColorBB(){
	return bbFigureColor;
}

void CMenuBar::setFMode(GLenum fm){
	Fmode = fm;
}

GLenum CMenuBar::getFMode(){
	return Fmode;
}

void CMenuBar::setFactorX(float fc){
	factorX = fc;
}

GLfloat CMenuBar::getFactorX(){
	return factorX;
}

void CMenuBar::setFactorY(float fc){
	factorY = fc;
}

GLfloat CMenuBar::getFactorY(){
	return factorY;
}

void CMenuBar::setFactorZ(float fc){
	factorZ = fc;
}

GLfloat CMenuBar::getFactorZ(){
	return factorZ;
}

void CMenuBar::setTrasX(float fc){
	trasX = fc;
}

GLfloat CMenuBar::getTrasX(){
	return trasX;
}

void CMenuBar::setTrasY(float fc){
	trasY = fc;
}

GLfloat CMenuBar::getTrasY(){
	return trasY;
}

void CMenuBar::setTrasZ(float fc){
	trasZ = fc;
}

GLfloat CMenuBar::getTrasZ(){
	return trasZ;
}

float* CMenuBar::getAmbientObject()
{
	return ambientObject;
}

float* CMenuBar::getDiffuseObject()
{
	return diffuseObject;
}

float* CMenuBar::getSpecularObject()
{
	return specularObject;
}

void CMenuBar::setAmbientObject(float r, float g, float b)
{
	ambientObject[0] = r;
	ambientObject[1] = g;
	ambientObject[2] = b;
}

void CMenuBar::setDiffuseObject(float r, float g, float b)
{
	diffuseObject[0] = r;
	diffuseObject[1] = g;
	diffuseObject[2] = b;
}

void CMenuBar::setSpecularObject(float r, float g, float b)
{
	specularObject[0] = r;
	specularObject[1] = g;
	specularObject[2] = b;
}