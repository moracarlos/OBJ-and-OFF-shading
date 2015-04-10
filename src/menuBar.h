#pragma once

#include "AntTweakBar\AntTweakBar.h"
#include "object.h"

using std::string;

class CMenuBar{
	public:
		GLfloat mFigureColor[3], bbFigureColor[3], colorFN[3], colorVN[3], factorX, factorY, factorZ, quat[4], trasX, trasY, trasZ;
		int indexObj;
		TwBar *menuBar;
		GLenum Fmode;
		bool mShow, showFN, showVN;
		float scaleX;
		float scaleY;
		float scaleZ;
		float ambientObject[3];
		float specularObject[3];
		float diffuseObject[3];

	public:
		CMenuBar();
		~CMenuBar();
		void reshape();
		void setFigureColor(GLfloat r, GLfloat g, GLfloat b);
		GLfloat* getFigureColor();
		void setFigureColorBB(GLfloat r, GLfloat g, GLfloat b);
		GLfloat* getFigureColorBB();
		void setFMode(GLenum fm);
		GLenum getFMode();
		void setFactorX(float fc);
		GLfloat getFactorX();
		void setFactorY(float fc);
		GLfloat getFactorY();
		void setFactorZ(float fc);
		GLfloat getFactorZ();
		void setTrasX(float fc);
		GLfloat getTrasX();
		void setTrasY(float fc);
		GLfloat getTrasY();
		void setTrasZ(float fc);
		GLfloat getTrasZ();
		float* getAmbientObject();
		float* getDiffuseObject();
		float* getSpecularObject();
		void setAmbientObject(float r, float g, float b);
		void setDiffuseObject(float r, float g, float b);
		void setSpecularObject(float r, float g, float b);
};

