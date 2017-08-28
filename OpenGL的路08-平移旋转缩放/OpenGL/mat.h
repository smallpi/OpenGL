#pragma once 

#ifndef _MAT_H_
#define _MAT_H_

#ifndef PI
#define PI (3.1415926535898)
#endif

#ifndef TO_RADIAN(X)      
#define TO_RADIAN(X)	((X) * PI / 180.0)
#endif

#ifndef TO_DEGREE(X)     
#define TO_DEGREE(X)	((X) * 180.0 / PI);
#endif


#include <memory.h>
#include <math.h>

class mat2;
class mat3;
class mat4;
class dmat2;
class dmat3;
class dmat4;

class mat
{
public:
	~mat(void);

	// ∆Ω“∆
	static void translate(mat4& _mat4, float dx, float dy, float dz);
	static void translate(dmat4& _dmat4, double dx, double dy, double dz);

	//–˝◊™
	static void rotate(mat4& _mat4, float radian, float rx, float ry, float rz);
	static void rotate(dmat4& _dmat4, double radian, double rx, double ry, double rz);

	// Àı∑≈
	static void scale(mat4& _mat4, float sx, float sy, float sz);
	static void scale(dmat4& _dmat4, double sx, double sy, double sz);


protected:
	mat(void);
private:

};


class mat2
{
public:
	mat2(void);

	mat2(float i);

	~mat2(void);

	float* data(void);

	// æÿ’Û≥À∑®
	void dot(mat2& _mat2);

private:
	float m_data[4];
};

class mat3
{
public:
	mat3(void);
	mat3(float i);
	~mat3(void);

	float* data(void);

	// æÿ’Û≥À∑®
	void dot(mat3& _mat3);

private:
	float m_data[9];
};

class mat4
{
public:
	mat4(void);
	mat4(float i);
	~mat4(void);

	float* data(void);

	// æÿ’Û≥À∑®
	void dot(mat4& _mat4);
private:
	float m_data[16];
};

class dmat2
{
public:
	dmat2(void);
	dmat2(double i);
	~dmat2(void);

	double* data(void);

	// æÿ’Û≥À∑®
	void dot(dmat2& _dmat2);

private:
	double m_data[4];
};

class dmat3
{
public:
	dmat3(void);
	dmat3(double i);
	~dmat3(void);

	double* data(void);

	void dot(dmat3& _dmat3);

private:
	double m_data[9];
};

class dmat4
{
public:
	dmat4(void);
	dmat4(double i);
	~dmat4(void);

	double* data(void);

	// æÿ’Û≥À∑®
	void dot(dmat4& _dmat4);

private:
	double m_data[16];
};


#endif