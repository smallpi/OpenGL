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

	// 平移
	static void translate(mat4& _mat4, float dx, float dy, float dz);
	static void translate(dmat4& _dmat4, double dx, double dy, double dz);

	//旋转
	static void rotate(mat4& _mat4, float radian, float rx, float ry, float rz);
	static void rotate(dmat4& _dmat4, double radian, double rx, double ry, double rz);

	// 缩放
	static void scale(mat4& _mat4, float sx, float sy, float sz);
	static void scale(dmat4& _dmat4, double sx, double sy, double sz);

	// 生成 world坐标系 转换成 view坐标系 的转换矩阵
	static void lookAt(mat4& _mat4, 
		float eyex, float eyey, float eyez,
		float aimx, float aimy, float aimz,
		float upx, float upy, float upz);
	static void lookAt(dmat4& _dmat4, 
		double eyex, double eyey, double eyez,
		double aimx, double aimy, double aimz,
		double upx, double upy, double upz);
	static void toSkyboxView(mat4& _mat4);
	// 投影矩阵
	static void perspective(mat4& _mat4,
		float fovy, float aspect, float near, float far);
	static void perspective(dmat4& _dmat4,
		double fovy, double aspect, double near, double far);
	// 正交投影矩阵
	static void ortho(mat4& _mat4,
		float left, float right, float bottom, float top, float near, float far); 
	static void ortho(dmat4& _dmat4,
		double left, double right, double bottom, double top, double near, double far);

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

	// 矩阵乘法
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

	// 矩阵乘法
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

	// 矩阵乘法
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

	// 矩阵乘法
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

	// 矩阵乘法
	void dot(dmat4& _dmat4);

private:
	double m_data[16];
};


#endif