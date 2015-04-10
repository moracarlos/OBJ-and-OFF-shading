#pragma once

#include "main.h"

class CObject{
	public:
		int numVertex, numFaces, numEdges;
		GLuint vbo, vindex, vbbo, vbbindex;
		string route;
		vector <GLuint> index, bbindex;
		vector <GLfloat> vertex, color, normal, bbvertex, bbcolor, faceNormals, vertexNormals;
		GLenum mode;
		GLfloat quat[4];
		GLfloat xmin, ymin, zmin, xmax, ymax, zmax, xmid, ymid, zmid;
		GLfloat transFX, transFY, transFZ, factorX, factorY, factorZ;
		bool showBB, showFN, showVN;
		float vertexN[3], facesN[3];
		GLuint m_idVAO;
		float ambientObject[3];
		float diffuseObject[3];
		float specularObject[3];

	public:
		CObject();
		~CObject();
		void display();
		int initialize();
		void setColor(GLfloat r, GLfloat g, GLfloat b);
		GLfloat* getColor();
		void setColorBB(GLfloat r, GLfloat g, GLfloat b);
		GLfloat* getColorBB();
		void setMode(GLenum fm);
		GLenum getMode();
		void displayBB();
		void createBB();
		void FaceNormals();
		void VertexNormals();
		void displayFacesNormals();
		void displayVertexNormals();
		float* getAmbientObject();
		float* getDiffuseObject();
		float* getSpecularObject();
		void setAmbientObject(float r, float g, float b);
		void setDiffuseObject(float r, float g, float b);
		void setSpecularObject(float r, float g, float b);
};