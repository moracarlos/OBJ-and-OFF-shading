#include "main.h"
#include "menuBar.h"

GLFWwindow* glfwWindow;
CMenuBar * menuBar;
vector <CObject *> objects;
CObject * object;
string route;
GLenum FigureMode;
int gWidth, gHeight, picked;
const float NCP = 0.01f, FCP = 200.0f, fAngle = 45.f; //FCP = 20.0f
bool pickObject = false, move, persp = true, ortho = false, zb = true, bfcull = true;
CGLSLProgram m_program;
glm::mat4x4 mProjMatrix, mModelViewMatrix;
float lightPos[3];
float eyePos[3];
float ld[3];
float ls[3];
float ambient[3];

//The main rendering function.
void display(){
	int i;
	cambiarCull(bfcull);
	cambiarzBuffer(zb);

	glFrontFace(GL_CCW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	//

	//glLoadIdentity();
	//glTranslatef(0.f,0.f,-10.0f);

	mModelViewMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, -10.f));

	float color[3]; color[0] = 1.f;  color[1] = 1.f; color[2] = 1.f;
	glColor3fv(color);
	glBegin(GL_TRIANGLES);
		glVertex3f(lightPos[0], lightPos[1] + 0.25, lightPos[2]);
		glVertex3f(lightPos[0] - 0.25, lightPos[1] - 0.25, lightPos[2]);
		glVertex3f(lightPos[0] + 0.25, lightPos[1] - 0.25, lightPos[2]);
	glEnd();

	for (i = 0; i < objects.size(); i++){
		//
		glPolygonMode(GL_FRONT_AND_BACK, objects[i]->mode); //GL_FILL GL_POINT
		//
		glLoadIdentity();
		glTranslatef(0.f, 0.f, -10.0f);

		//BO using glDrawElements
		glPushMatrix();
		//glTranslatef(2, -2, 0); // move to upper right corner

		glTranslatef(objects[i]->transFX, objects[i]->transFY, objects[i]->transFZ);

		glTranslatef(objects[i]->xmid, objects[i]->ymid, objects[i]->zmid);

		//rotate
		glm::quat q = glm::quat(objects[i]->quat[3], objects[i]->quat[0], objects[i]->quat[1], objects[i]->quat[2]);
		glm::normalize(q);
		glm::mat4 quatMatrix = glm::mat4_cast(q);
		glMultMatrixf(glm::value_ptr(quatMatrix));
		//
		glScalef(objects[i]->factorX, objects[i]->factorY, objects[i]->factorZ);

		glTranslatef(-objects[i]->xmid, -objects[i]->ymid, -objects[i]->zmid);

		if (objects[i]->showBB)
			objects[i]->displayBB();

		if (objects[i]->showFN)
			objects[i]->displayFacesNormals();

		if (objects[i]->showVN)
			objects[i]->displayVertexNormals();

		objects[i]->display();

		glPopMatrix();

		//glfwSwapBuffers(glfwWindow);
	}
}

bool initialize(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//Glew INIT
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "- glew Init failed :(" << std::endl;
		//return false;
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	
	//Load the shaders
	m_program.loadShader("shaders/basic.vert", CGLSLProgram::VERTEX);
	m_program.loadShader("shaders/basic.frag", CGLSLProgram::FRAGMENT);
	//Link the shaders in a program
	m_program.create_link();
	//Enable the program
	m_program.enable();
	//Link the attributes and the uniforms
	m_program.addAttribute("vVertex");
	m_program.addAttribute("vColor");
	m_program.addAttribute("vNormal");
	m_program.addUniform("mProjection");
	m_program.addUniform("mModelView");

    m_program.addUniform("centerPosition");
    m_program.addUniform("translateFactor");
    m_program.addUniform("scaleFactor");
    m_program.addUniform("quat");

	m_program.addUniform("lightPos");
	m_program.addUniform("eyePos");
	m_program.addUniform("ld");
	m_program.addUniform("ls");
	m_program.addUniform("ambient");

	m_program.addUniform("diffuseObject");
	m_program.addUniform("specularObject");
	m_program.addUniform("ambientObject");

	//Disable the program
	m_program.disable();

	//Function to initiate a triangle
//	initiateTriangle();
	return true;
}

//The resizing function
void refresh(int picked)
{
    glm::vec3 eye(objects[picked]->xmid, objects[picked]->ymid, objects[picked]->zmid+1);
	eyePos[0] = eye[0];
	eyePos[1] = eye[1];
	eyePos[2] = eye[2];
	glm::vec3 center(objects[picked]->xmid, objects[picked]->ymid, objects[picked]->zmid);
    glm::vec3 up(0,1,0);
    float ratio = gWidth / float(gHeight);
    if(gHeight == 0) gHeight = 1;
    glViewport(0, 0, gWidth, gHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (persp){
        gluperspective(fAngle, ratio, NCP, FCP);
    }else{
        glOrtho(-1.0f,1.0f,-1.0f,1.0f,-0.01f,200.0f); //revisar como se le pasa al shader
        mProjMatrix= glm::ortho(-1.0f, 1.0f, -1.f, 1.f, -0.01f, 200.f);
    }
    glm::mat4 LA= glm::lookAt(eye, center, up);
    glMultMatrixf(glm::value_ptr(LA)); //Estaba comentado
    mProjMatrix = mProjMatrix * LA;
    glMatrixMode(GL_MODELVIEW);
}

void gluperspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    fH = tan( fovY / 360 * pi ) * zNear;	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
	mProjMatrix = glm::frustum(-fW, fW, -fH, fH, zNear, zFar);
    //mProjMatrix = glm::perspective(fovY, aspect, zNear, zFar); //Aqui va con glm::frustum
}

void reshape(GLFWwindow *window, int width, int height)
{
    gWidth = width;
    gHeight = height;
    menuBar->reshape();
    float ratio = gWidth / float(gHeight);
    if(gHeight == 0) gHeight = 1;
    glViewport(0, 0, gWidth, gHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluperspective(fAngle, ratio, NCP, FCP);
    //glOrtho(-10.0f,10.0f,-10.0f,10.0f,-10.0f,10.0f);
    glMatrixMode(GL_MODELVIEW);
    //userInterface->setProyeccion(true);
    if (picked>-1)
		refresh(picked);
}

void keyInput(GLFWwindow* window, int iKey, int iScancode, int iAction, int iMods){
	if (iAction == GLFW_PRESS){
		switch (iKey){
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		case GLFW_KEY_P:
			pickObject = true;
			break;
		}
	}
}

void cambiarCull (bool c)
{
    if (c){
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
    }else{
        glDisable(GL_CULL_FACE);
    }
}

void cambiarzBuffer(bool c)
{
    if (c){
        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
    }else{
        glDisable(GL_DEPTH_TEST);
    }
}

void cambiarProyeccion(bool p)
{
    if (picked>-1){
    refresh(picked);
    }else{
        float ratio = gWidth / float(gHeight);
        if(gHeight == 0) gHeight = 1;
        glViewport(0, 0, gWidth, gHeight);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (p)
            gluperspective(fAngle, ratio, NCP, FCP);
        else
            glOrtho(-10.0f,10.0f,-10.0f,10.0f,0.01f,200.0f);
        glMatrixMode(GL_MODELVIEW);
    }
}

void mouseButton(GLFWwindow* window, int button, int action, int mods){
	if (TwEventMouseButtonGLFW(button, action))
		return;

	double x, y;
	glfwGetCursorPos(glfwWindow, &x, &y);
	float ax = x;
	float ay = gHeight - y;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		if (pickObject){
			//			pick(ax, ay);
			if (picked > -1){
				move = true;
			}
		}
		//
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		pickObject = false;
		move = false;
	}
}

void cursorPos(GLFWwindow* window, double x, double y){
	if (TwEventMousePosGLFW(x, y))
		return;

	float ax = x;
	float ay = gHeight - y;

	if (picked>-1 && move){
//		objects[picked]->factorS = ax;
	}
}

void charInput(GLFWwindow* window, static unsigned int scanChar){
	if (TwEventCharGLFW(scanChar, GLFW_PRESS))
		return;
}

//Callback function used by GLFW to capture some possible error.
void errorCB(int error, const char* description){
	std::cout << description << std::endl;
}

// Here all data must be destroyed + glfwTerminate
void destroy(){
	int i;
	for (i = 0; i < objects.size(); i++){
		glDeleteBuffers(1, &objects[i]->vbo);
		glDeleteBuffers(1, &objects[i]->vindex);
	}

	//glDeleteVertexArrays(1, &vao);
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
}

bool initGlfw(){
	glfwSetErrorCallback(errorCB);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindow = glfwCreateWindow(gWidth, gHeight, "Tarea 4", NULL, NULL);
	if (!glfwWindow){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	const GLFWvidmode * vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(glfwWindow, (vidMode->width - gWidth) >> 1, (vidMode->height - gHeight) >> 1);

	glfwMakeContextCurrent(glfwWindow);

	glfwSetKeyCallback(glfwWindow, keyInput);
	glfwSetWindowSizeCallback(glfwWindow, reshape);
	glfwSetMouseButtonCallback(glfwWindow, mouseButton);
	glfwSetCursorPosCallback(glfwWindow, cursorPos);
	glfwSetCharCallback(glfwWindow, charInput);

	return true;
}

bool initUserInterface(){
	if (!TwInit(TW_OPENGL, NULL))
		return false;

	menuBar = new CMenuBar();

	return true;
}

void setUserInterface(){
	if (picked > -1){
		//GLfloat* colorF = objects[picked]->getColor();
		GLfloat* colorB = objects[picked]->getColorBB();
		menuBar->mShow = objects[picked]->showBB;
		menuBar->setFMode(objects[picked]->getMode());
		//menuBar->setFigureColor(colorF[0], colorF[1], colorF[2]);
		menuBar->setFigureColorBB(colorB[0], colorB[1], colorB[2]);
		menuBar->setTrasX(objects[picked]->transFX);
		menuBar->setTrasY(objects[picked]->transFY);
		menuBar->setTrasZ(objects[picked]->transFZ);
		menuBar->setFactorX(objects[picked]->factorX);
		menuBar->setFactorY(objects[picked]->factorY);
		menuBar->setFactorZ(objects[picked]->factorZ);
		menuBar->quat[0] = objects[picked]->quat[0];
		menuBar->quat[1] = objects[picked]->quat[1];
		menuBar->quat[2] = objects[picked]->quat[2];
		menuBar->quat[3] = objects[picked]->quat[3];
		menuBar->showFN = objects[picked]->showFN;
		menuBar->colorFN[0] = objects[picked]->facesN[0];
		menuBar->colorFN[1] = objects[picked]->facesN[1];
		menuBar->colorFN[2] = objects[picked]->facesN[2];
		menuBar->showVN = objects[picked]->showVN;
		menuBar->colorVN[0] = objects[picked]->vertexN[0];
		menuBar->colorVN[1] = objects[picked]->vertexN[1];
		menuBar->colorVN[2] = objects[picked]->vertexN[2];
		menuBar->setDiffuseObject(objects[picked]->getDiffuseObject()[0], objects[picked]->getDiffuseObject()[1], objects[picked]->getDiffuseObject()[2]);
		menuBar->setSpecularObject(objects[picked]->getSpecularObject()[0], objects[picked]->getSpecularObject()[1], objects[picked]->getSpecularObject()[2]);
		menuBar->setAmbientObject(objects[picked]->getAmbientObject()[0], objects[picked]->getAmbientObject()[1], objects[picked]->getAmbientObject()[2]);
	}
}

void updateUserInterface(){
	if (picked > -1){
		//GLfloat* colorF = menuBar->getFigureColor();
		GLfloat* colorB = menuBar->getFigureColorBB();
		//objects[picked]->setColor(colorF[0], colorF[1], colorF[2]);
		objects[picked]->setColorBB(colorB[0], colorB[1], colorB[2]);
		objects[picked]->showBB = menuBar->mShow;
		objects[picked]->setMode(menuBar->getFMode());
		objects[picked]->transFX = menuBar->getTrasX();
		objects[picked]->transFY = menuBar->getTrasY();
		objects[picked]->transFZ = menuBar->getTrasZ();
		objects[picked]->factorX = menuBar->getFactorX();
		objects[picked]->factorY = menuBar->getFactorY();
		objects[picked]->factorZ = menuBar->getFactorZ();
		objects[picked]->quat[0] = menuBar->quat[0];
		objects[picked]->quat[1] = menuBar->quat[1];
		objects[picked]->quat[2] = menuBar->quat[2];
		objects[picked]->quat[3] = menuBar->quat[3];
		objects[picked]->showFN = menuBar->showFN;
		objects[picked]->facesN[0] = menuBar->colorFN[0];
		objects[picked]->facesN[1] = menuBar->colorFN[1];
		objects[picked]->facesN[2] = menuBar->colorFN[2];
		objects[picked]->showVN = menuBar->showVN;
		objects[picked]->vertexN[0] = menuBar->colorVN[0];
		objects[picked]->vertexN[1] = menuBar->colorVN[1];
		objects[picked]->vertexN[2] = menuBar->colorVN[2];
		objects[picked]->setDiffuseObject(menuBar->getDiffuseObject()[0], menuBar->getDiffuseObject()[1], menuBar->getDiffuseObject()[2]);
		objects[picked]->setSpecularObject(menuBar->getSpecularObject()[0], menuBar->getSpecularObject()[1], menuBar->getSpecularObject()[2]);
		objects[picked]->setAmbientObject(menuBar->getAmbientObject()[0], menuBar->getAmbientObject()[1], menuBar->getAmbientObject()[2]);
	}
}

void calculateMinMax(int i){
	//std::cout << i << std::endl;
	if (i == 0){
		object->xmin = object->vertex[0];
		object->ymin = object->vertex[1];
		object->zmin = object->vertex[2];
		//
		object->xmax = object->vertex[0];
		object->ymax = object->vertex[1];
		object->zmax = object->vertex[2];
	}
	else{
		if (object->vertex[i * 3] < object->xmin){
			object->xmin = object->vertex[i * 3];
		}
		if (object->vertex[i * 3 + 1] < object->ymin){
			object->ymin = object->vertex[i * 3 + 1];
		}
		if (object->vertex[i * 3 + 2] < object->zmin){
			object->zmin = object->vertex[i * 3 + 2];
		}
		//
		if (object->vertex[i * 3] > object->xmax){
			object->xmax = object->vertex[i * 3];
		}
		if (object->vertex[i * 3 + 1] > object->ymax){
			object->ymax = object->vertex[i * 3 + 1];
		}
		if (object->vertex[i * 3 + 2] > object->zmax){
			object->zmax = object->vertex[i * 3 + 2];
		}
	}
}

string getFile(const string & tittle){
	const int size = 1024;
	char File[size] = { 0 };
	OPENFILENAME windowBox = { 0 };
	windowBox.lStructSize = sizeof(windowBox);
	windowBox.lpstrFile = File;
	windowBox.nMaxFile = size;
	windowBox.lpstrTitle = tittle.c_str();
	if (tittle == "Abrir" && GetOpenFileName(&windowBox)){
		return File;
	}
	if (tittle == "Guardar" && GetOpenFileName(&windowBox)){
		return File;
	}
	return "-1";
}

void loadFile(){
	route = getFile("Abrir");
	if (route == "-1")
		return;

	object = new CObject();

	std::fstream file(route, std::fstream::in | std::fstream::out);
	int i, j, indexMinMax = 0, numF;
	GLfloat point;
	vector <GLuint> fc;
	GLuint pt;
	string type;
	file >> type;
	//LOAD OFF FILE
	if (type == "OFF"){
		file >> object->numVertex;
		file >> object->numFaces;
		file >> object->numEdges;
		//List of Vertex
		for (i = 0; i < object->numVertex; i++){
			for (j = 0; j < 3; j++){
				file >> point;
				object->vertex.push_back(point);
			}
			object->color.push_back(0);
			object->color.push_back(1);
			object->color.push_back(0);
			//
			calculateMinMax(indexMinMax);
			indexMinMax++;
			//
		}
		//List of Faces
		for (i = 0; i < object->numFaces; i++){
			file >> numF;
			fc.clear();
			for (j = 0; j < numF; j++){
				file >> pt;
				fc.push_back(pt);
			}
			for (j = 1; j < numF - 1; j++){
				object->index.push_back(fc[0]);
				object->index.push_back(fc[j]);
				object->index.push_back(fc[j + 1]);
			}
		}
	} //LOAD OBJ FILE
	else{
		int j = 0;
		fc.clear();
		while (!file.eof()){
			string subtype = type.substr(0, 1);
			string line;
			if (subtype == "#"){
				std::getline(file, line);
			}
			if (type == "vt"){
				std::getline(file, line);
			}
			if (type == "vn"){
				std::getline(file, line);
			} //List of Vertex
			if (type == "v"){
				for (i = 0; i < 3; i++){
					file >> point;
					object->vertex.push_back(point);
				}
				object->color.push_back(0);
				object->color.push_back(1);
				object->color.push_back(0);
				object->numVertex++;
				//
				calculateMinMax(indexMinMax);
				indexMinMax++;
				//
			}//List of Faces
			if (type == "f"){
				std::getline(file, line);
				char * tok = strtok(&line[0u], " ");
				while (tok != NULL){
					string num = tok;
					std::size_t pos = num.find("/");
					num = num.substr(0, pos);
					pt = stoi(num);
					fc.push_back(pt);
					tok = strtok(NULL, " ");
				}
				object->numFaces++;
				for (j = 1; j < fc.size() - 1; j++){
					object->index.push_back(fc[0] - 1);
					object->index.push_back(fc[j] - 1);
					object->index.push_back(fc[j + 1] - 1);
				}
				fc.clear();
			}
			file >> type;
		}
	}

	object->xmid = (object->xmin + object->xmax) / 2;
	object->ymid = (object->ymin + object->ymax) / 2;
	object->zmid = (object->zmin + object->zmax) / 2;

	objects.push_back(object);
	objects.back()->createBB();
	objects.back()->FaceNormals();
	objects.back()->VertexNormals();

	if (!objects.back()->initialize())
		exit(EXIT_FAILURE);

	file.close();
}

int main(){
	gWidth = 1024;
	gHeight = 600;
	picked = -1;

	if (!initGlfw() || !initUserInterface())
		return EXIT_FAILURE;

	if (!initialize()) 
		exit(EXIT_FAILURE);

	reshape(glfwWindow, gWidth, gHeight);

	// main loop!
	while (!glfwWindowShouldClose(glfwWindow)){
		display();

		TwDraw();

		updateUserInterface();

		glfwSwapBuffers(glfwWindow);

		glfwPollEvents();	//or glfwWaitEvents()
	}
	destroy();
	return EXIT_SUCCESS;
}