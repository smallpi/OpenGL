#include "mat.h"

mat::mat(void)
{
	//pass;
}
mat::~mat(void)
{
	//pass;
}

// 平移
void mat::translate(mat4& _mat4, float dx, float dy, float dz)
{
	float *rData = _mat4.data();
	rData[0] += dx * rData[12];
	rData[1] += dx * rData[13];
	rData[2] += dx * rData[14];
	rData[3] += dx * rData[15];
	rData[4] += dy * rData[12];
	rData[5] += dy * rData[13];
	rData[6] += dy * rData[14];
	rData[7] += dy * rData[15];
	rData[8] += dz * rData[12];
	rData[9] += dz * rData[13];
	rData[10] += dz * rData[14];
	rData[11] += dz * rData[15];
}

void mat::translate(dmat4& _dmat4, double dx, double dy, double dz)
{
	double *rData = _dmat4.data();
	rData[0] += dx * rData[12];
	rData[1] += dx * rData[13];
	rData[2] += dx * rData[14];
	rData[3] += dx * rData[15];
	rData[4] += dy * rData[12];
	rData[5] += dy * rData[13];
	rData[6] += dy * rData[14];
	rData[7] += dy * rData[15];
	rData[8] += dz * rData[12];
	rData[9] += dz * rData[13];
	rData[10] += dz * rData[14];
	rData[11] += dz * rData[15];
}

//旋转
void mat::rotate(mat4& _mat4, float radian, float rx, float ry, float rz)
{
	float *rData = _mat4.data();
	float sinValue = sin(radian);
	float cosValue = cos(radian);
	float left00 = cosValue + rx * rx * (1 - cosValue);
	float left01 = rx * ry * (1 - cosValue) - rz * sinValue;
	float left02 = rx * rz * (1 - cosValue) + ry * sinValue;
	float left10 = ry * rx * (1 - cosValue) + rz * sinValue;
	float left11 = cosValue + ry * ry * (1 - cosValue);
	float left12 = ry * rz * (1 - cosValue) - rx * sinValue;
	float left20 = rz * rx * (1 - cosValue) - ry * sinValue;
	float left21 = rz * ry * (1 - cosValue) + rx * sinValue;
	float left22 = cosValue + rz * rz * (1 - cosValue);

	float tempData[16];
	tempData[0] = left00 * rData[0] + left01 * rData[4] + left02 * rData[8];
	tempData[1] = left00 * rData[1] + left01 * rData[5] + left02 * rData[9];
	tempData[2] = left00 * rData[2] + left01 * rData[6] + left02 * rData[10];
	tempData[3] = left00 * rData[3] + left01 * rData[7] + left02 * rData[11];
	tempData[4] = left10 * rData[0] + left11 * rData[4] + left12 * rData[8];
	tempData[5] = left10 * rData[1] + left11 * rData[5] + left12 * rData[9];
	tempData[6] = left10 * rData[2] + left11 * rData[6] + left12 * rData[10];
	tempData[7] = left10 * rData[3] + left11 * rData[7] + left12 * rData[11];
	tempData[8] = left20 * rData[0] + left21 * rData[4] + left22 * rData[8];
	tempData[9] = left20 * rData[1] + left21 * rData[5] + left22 * rData[9];
	tempData[10] = left20 * rData[2] + left21 * rData[6] + left22 * rData[10];
	tempData[11] = left20 * rData[3] + left21 * rData[7] + left22 * rData[11];
	tempData[12] = rData[12];
	tempData[13] = rData[13];
	tempData[14] = rData[14];
	tempData[15] = rData[15];

	memcpy(rData,tempData,16*sizeof(float));
}
void mat::rotate(dmat4& _dmat4, double radian, double rx, double ry, double rz)
{
	double *rData = _dmat4.data();
	double sinValue = sin(radian);
	double cosValue = cos(radian);
	double left00 = cosValue + rx * rx * (1 - cosValue);
	double left01 = rx * ry * (1 - cosValue) - rz * sinValue;
	double left02 = rx * rz * (1 - cosValue) + ry * sinValue;
	double left10 = ry * rx * (1 - cosValue) + rz * sinValue;
	double left11 = cosValue + ry * ry * (1 - cosValue);
	double left12 = ry * rz * (1 - cosValue) - rx * sinValue;
	double left20 = rz * rx * (1 - cosValue) - ry * sinValue;
	double left21 = rz * ry * (1 - cosValue) + rx * sinValue;
	double left22 = cosValue + rz * rz * (1 - cosValue);

	double tempData[16];
	tempData[0] = left00 * rData[0] + left01 * rData[4] + left02 * rData[8];
	tempData[1] = left00 * rData[1] + left01 * rData[5] + left02 * rData[9];
	tempData[2] = left00 * rData[2] + left01 * rData[6] + left02 * rData[10];
	tempData[3] = left00 * rData[3] + left01 * rData[7] + left02 * rData[11];
	tempData[4] = left10 * rData[0] + left11 * rData[4] + left12 * rData[8];
	tempData[5] = left10 * rData[1] + left11 * rData[5] + left12 * rData[9];
	tempData[6] = left10 * rData[2] + left11 * rData[6] + left12 * rData[10];
	tempData[7] = left10 * rData[3] + left11 * rData[7] + left12 * rData[11];
	tempData[8] = left20 * rData[0] + left21 * rData[4] + left22 * rData[8];
	tempData[9] = left20 * rData[1] + left21 * rData[5] + left22 * rData[9];
	tempData[10] = left20 * rData[2] + left21 * rData[6] + left22 * rData[10];
	tempData[11] = left20 * rData[3] + left21 * rData[7] + left22 * rData[11];
	tempData[12] = rData[12];
	tempData[13] = rData[13];
	tempData[14] = rData[14];
	tempData[15] = rData[15];

	memcpy(rData,tempData,16*sizeof(double));
}

// 缩放
void mat::scale(mat4& _mat4, float sx, float sy, float sz)
{
	float *rData = _mat4.data();
	rData[0] = sx * rData[0];
	rData[1] = sx * rData[1];
	rData[2] = sx * rData[2];
	rData[3] = sx * rData[3];
	rData[4] = sy * rData[4];
	rData[5] = sy * rData[5];
	rData[6] = sy * rData[6];
	rData[7] = sy * rData[7];
	rData[8] = sz * rData[8];
	rData[9] = sz * rData[9];
	rData[10] = sz * rData[10];
	rData[11] = sz * rData[11];
}
void mat::scale(dmat4& _dmat4, double sx, double sy, double sz)
{
	double *rData = _dmat4.data();
	rData[0] = sx * rData[0];
	rData[1] = sx * rData[1];
	rData[2] = sx * rData[2];
	rData[3] = sx * rData[3];
	rData[4] = sy * rData[4];
	rData[5] = sy * rData[5];
	rData[6] = sy * rData[6];
	rData[7] = sy * rData[7];
	rData[8] = sz * rData[8];
	rData[9] = sz * rData[9];
	rData[10] = sz * rData[10];
	rData[11] = sz * rData[11];
}

// 生成 world坐标系 转换成 view坐标系 的转换矩阵
void mat::lookAt(mat4& _mat4, 
		float eyex, float eyey, float eyez,
		float aimx, float aimy, float aimz,
		float upx, float upy, float upz)
{
	float dirVector[3];
	dirVector[0] = aimx - eyex;
	dirVector[1] = aimy - eyey;
	dirVector[2] = aimz - eyez;
	float length = sqrt(dirVector[0]*dirVector[0]+dirVector[1]*dirVector[1]+dirVector[2]*dirVector[2]);
	dirVector[0] = dirVector[0] / length;
	dirVector[1] = dirVector[1] / length;
	dirVector[2] = dirVector[2] / length;

	float rightVector[3] = 
	{
		dirVector[1] * upz - dirVector[2] * upy,
		dirVector[2] * upx - dirVector[0] * upz,
		dirVector[0] * upy - dirVector[1] * upx,
	};
	length = sqrt(rightVector[0]*rightVector[0]+rightVector[1]*rightVector[1]+rightVector[2]*rightVector[2]);
	rightVector[0] = rightVector[0] / length;
	rightVector[1] = rightVector[1] / length;
	rightVector[2] = rightVector[2] / length;

	float upVector[3] = 
	{
		rightVector[1] * dirVector[2] - rightVector[2] * dirVector[1],
		rightVector[2] * dirVector[0] - rightVector[0] * dirVector[2],
		rightVector[0] * dirVector[1] - rightVector[1] * dirVector[0],
	};
	length = sqrt(upVector[0]*upVector[0]+upVector[1]*upVector[1]+upVector[2]*upVector[2]);

	float *data = _mat4.data();
	data[0] = rightVector[0];
	data[1] = rightVector[1];
	data[2] = rightVector[2];
	data[3] = -rightVector[0] * eyex - rightVector[1] * eyey - rightVector[2] * eyez;
	data[4] = upVector[0];
	data[5] = upVector[1];
	data[6] = upVector[2];
	data[7] = -upVector[0] * eyex - upVector[1] * eyey - upVector[2] * eyez;
	data[8] = -dirVector[0];
	data[9] = -dirVector[1];
	data[10] = -dirVector[2];
	data[11] = dirVector[0] * eyex + dirVector[1] * eyey + dirVector[2] * eyez;
	data[12] = 0.0f;
	data[13] = 0.0f;
	data[14] = 0.0f;
	data[15] = 1.0f;
}
void mat::lookAt(dmat4& _dmat4, 
	double eyex, double eyey, double eyez,
	double aimx, double aimy, double aimz,
	double upx, double upy, double upz)
{
	double dirVector[3];
	dirVector[0] = aimx - eyex;
	dirVector[1] = aimy - eyey;
	dirVector[2] = aimz - eyez;
	double length = sqrt(dirVector[0]*dirVector[0]+dirVector[1]*dirVector[1]+dirVector[2]*dirVector[2]);
	dirVector[0] = dirVector[0] / length;
	dirVector[1] = dirVector[1] / length;
	dirVector[2] = dirVector[2] / length;

	double rightVector[3] = 
	{
		dirVector[1] * upz - dirVector[2] * upy,
		dirVector[2] * upx - dirVector[0] * upz,
		dirVector[0] * upy - dirVector[1] * upx,
	};
	length = sqrt(rightVector[0]*rightVector[0]+rightVector[1]*rightVector[1]+rightVector[2]*rightVector[2]);
	rightVector[0] = rightVector[0] / length;
	rightVector[1] = rightVector[1] / length;
	rightVector[2] = rightVector[2] / length;

	double upVector[3] = 
	{
		rightVector[1] * dirVector[2] - rightVector[2] * dirVector[1],
		rightVector[2] * dirVector[0] - rightVector[0] * dirVector[2],
		rightVector[0] * dirVector[1] - rightVector[1] * dirVector[0],
	};
	length = sqrt(upVector[0]*upVector[0]+upVector[1]*upVector[1]+upVector[2]*upVector[2]);
	upVector[0] = upVector[0] / length;
	upVector[1] = upVector[1] / length;
	upVector[2] = upVector[2] / length;

	double *data = _dmat4.data();
	data[0] = rightVector[0];
	data[1] = rightVector[1];
	data[2] = rightVector[2];
	data[3] = -rightVector[0] * eyex - rightVector[1] * eyey - rightVector[2] * eyez;
	data[4] = upVector[0];
	data[5] = upVector[1];
	data[6] = upVector[2];
	data[7] = -upVector[0] * eyex - upVector[1] * eyey - upVector[2] * eyez;
	data[8] = -dirVector[0];
	data[9] = -dirVector[1];
	data[10] = -dirVector[2];
	data[11] = dirVector[0] * eyex + dirVector[1] * eyey + dirVector[2] * eyez;
	data[12] = 0.0;
	data[13] = 0.0;
	data[14] = 0.0;
	data[15] = 1.0;
}

void mat::perspective(mat4& _mat4,
		float fovy, float aspect, float near, float far)
{
	float *data = _mat4.data();
	data[0] = 1.0f / (aspect * tan(0.5f * fovy));
	data[1] = 0.0f;
	data[2] = 0.0f;
	data[3] = 0.0f;
	data[4] = 0.0f;
	data[5] = 1.0f / tan(0.5f * fovy);
	data[6] = 0.0f;
	data[7] = 0.0f;
	data[8] = 0.0f;
	data[9] = 0.0f;
	data[10] = - (far + near) / (far - near);
	data[11] = - 2.0f * (near * far) / (far - near);
	data[12] = 0.0f;
	data[13] = 0.0f;
	data[14] = -1.0f;
	data[15] = 0.0f;
}
void mat::perspective(dmat4& _dmat4,
		double fovy, double aspect, double near, double far)
{
	double *data = _dmat4.data();
	data[0] = 1.0 / (aspect * tan(0.5f * fovy));
	data[1] = 0.0;
	data[2] = 0.0;
	data[3] = 0.0;
	data[4] = 0.0;
	data[5] = 1.0 / tan(0.5 * fovy);
	data[6] = 0.0;
	data[7] = 0.0;
	data[8] = 0.0;
	data[9] = 0.0;
	data[10] = - (far + near) / (far - near);
	data[11] = - 2.0 * (near * far) / (far - near);
	data[12] = 0.0;
	data[13] = 0.0;
	data[14] = - 1.0;
	data[15] = 0.0;
}

// 正交投影矩阵
void mat::ortho(mat4& _mat4,
	float left, float right, float bottom, float top, float near, float far)
{
	float *data = _mat4.data();
	data[0] = 2.0f / (right - left);
	data[1] = 0.0f;
	data[2] = 0.0f;
	data[3] = - (right + left) / (right - left);
	data[4] = 0.0f;
	data[5] = 2.0f / (top - bottom);
	data[6] = 0.0f;
	data[7] = - (top + bottom) / (top - bottom);
	data[8] = 0.0f;
	data[9] = 0.0f;
	data[10] = - 2.0f / (far - near);
	data[11] = - (far + near) / (far - near);
	data[12] = 0.0f;
	data[13] = 0.0f;
	data[14] = 0.0f;
	data[15] = 1.0f;
}
void mat::ortho(dmat4& _dmat4,
	double left, double right, double bottom, double top, double near, double far)
{
	double *data = _dmat4.data();
	data[0] = 2.0 / (right - left);
	data[1] = 0.0;
	data[2] = 0.0;
	data[3] = - (right + left) / (right - left);
	data[4] = 0.0;
	data[5] = 2.0 / (top - bottom);
	data[6] = 0.0;
	data[7] = - (top + bottom) / (top - bottom);
	data[8] = 0.0;
	data[9] = 0.0;
	data[10] = - 2.0 / (far - near);
	data[11] = - (far + near) / (far - near);
	data[12] = 0.0;
	data[13] = 0.0;
	data[14] = 0.0;
	data[15] = 1.0;
}



mat2::mat2(void)
{
	memset(m_data,0,sizeof(m_data));
}
mat2::mat2(float i)
{
	memset(m_data,0,sizeof(m_data));
	m_data[0] = i;
	m_data[3] = i;
}
mat2::~mat2(void)
{
	//pass
}


float* mat2::data(void)
{
	return m_data;
}

// 矩阵乘法
void mat2::dot(mat2& _mat2)
{
	float *rData = _mat2.data();
	float tempData[4];
	tempData[0] = m_data[0] * rData[0] + m_data[1] * rData[2];
	tempData[1] = m_data[0] * rData[1] + m_data[1] * rData[3];
	tempData[2] = m_data[2] * rData[0] + m_data[3] * rData[2];
	tempData[3] = m_data[2] * rData[1] + m_data[3] * rData[3];
	memcpy(m_data,tempData,sizeof(m_data));
}


mat3::mat3(void)
{
	memset(m_data,0,sizeof(m_data));
}
mat3::mat3(float i)
{
	memset(m_data,0,sizeof(m_data));
	m_data[0] = i;
	m_data[4] = i;
	m_data[8] = i;
}
mat3::~mat3(void)
{
	//pass
}

float* mat3::data(void)
{
	return m_data;
}

// 矩阵乘法
void mat3::dot(mat3& _mat3)
{
	float* rData = _mat3.data();
	float tempData[9];
	tempData[0] = m_data[0] * rData[0] + m_data[1] * rData[3] + m_data[2] * rData[6];
	tempData[1] = m_data[0] * rData[1] + m_data[1] * rData[4] + m_data[2] * rData[7];
	tempData[2] = m_data[0] * rData[2] + m_data[1] * rData[5] + m_data[2] * rData[8];
	tempData[3] = m_data[3] * rData[0] + m_data[4] * rData[3] + m_data[5] * rData[6];
	tempData[4] = m_data[3] * rData[1] + m_data[4] * rData[4] + m_data[5] * rData[7];
	tempData[5] = m_data[3] * rData[2] + m_data[4] * rData[5] + m_data[5] * rData[8];
	tempData[6] = m_data[6] * rData[0] + m_data[7] * rData[3] + m_data[8] * rData[6];
	tempData[7] = m_data[6] * rData[1] + m_data[7] * rData[4] + m_data[8] * rData[7];
	tempData[8] = m_data[6] * rData[2] + m_data[7] * rData[5] + m_data[8] * rData[8];
	memcpy(m_data,tempData,sizeof(m_data));
}


mat4::mat4(void)
{
	memset(m_data,0,sizeof(m_data));
}
mat4::mat4(float i)
{
	memset(m_data,0,sizeof(m_data));
	m_data[0] = i;
	m_data[5] = i;
	m_data[10] = i;
	m_data[15] = i;
}
mat4::~mat4(void)
{
	//pass
}

float* mat4::data(void)
{
	return m_data;
}

// 矩阵乘法
void mat4::dot(mat4& _mat4)
{
	float *rData = _mat4.data();
	float tempData[16];
	tempData[0] = m_data[0] * rData[0] + m_data[1] * rData[4] + m_data[2] * rData[8] + m_data[3] * rData[12];
	tempData[1] = m_data[0] * rData[1] + m_data[1] * rData[5] + m_data[2] * rData[9] + m_data[3] * rData[13];
	tempData[2] = m_data[0] * rData[2] + m_data[1] * rData[6] + m_data[2] * rData[10] + m_data[3] * rData[14];
	tempData[3] = m_data[0] * rData[3] + m_data[1] * rData[7] + m_data[2] * rData[11] + m_data[3] * rData[15];
	tempData[4] = m_data[4] * rData[0] + m_data[5] * rData[4] + m_data[6] * rData[8] + m_data[7] * rData[12];
	tempData[5] = m_data[4] * rData[1] + m_data[5] * rData[5] + m_data[6] * rData[9] + m_data[7] * rData[13];
	tempData[6] = m_data[4] * rData[2] + m_data[5] * rData[6] + m_data[6] * rData[10] + m_data[7] * rData[14];
	tempData[7] = m_data[4] * rData[3] + m_data[5] * rData[7] + m_data[6] * rData[11] + m_data[7] * rData[15];
	tempData[8] = m_data[8] * rData[0] + m_data[9] * rData[4] + m_data[10] * rData[8] + m_data[11] * rData[12];
	tempData[9] = m_data[8] * rData[1] + m_data[9] * rData[5] + m_data[10] * rData[9] + m_data[11] * rData[13];
	tempData[10] = m_data[8] * rData[2] + m_data[9] * rData[6] + m_data[10] * rData[10] + m_data[11] * rData[14];
	tempData[11] = m_data[8] * rData[3] + m_data[9] * rData[7] + m_data[10] * rData[11] + m_data[11] * rData[15];
	tempData[12] = m_data[12] * rData[0] + m_data[13] * rData[4] + m_data[14] * rData[8] + m_data[15] * rData[12];
	tempData[13] = m_data[12] * rData[1] + m_data[13] * rData[5] + m_data[14] * rData[9] + m_data[15] * rData[13];
	tempData[14] = m_data[12] * rData[2] + m_data[13] * rData[6] + m_data[14] * rData[10] + m_data[15] * rData[14];
	tempData[15] = m_data[12] * rData[3] + m_data[13] * rData[7] + m_data[14] * rData[11] + m_data[15] * rData[15];
	memcpy(m_data,tempData,sizeof(m_data));
}

// 矩阵转置
void mat4::transpose(void){
	float temp = m_data[1];
	m_data[1] = m_data[4];
	m_data[4] = temp;

	temp = m_data[2];
	m_data[2] = m_data[8];
	m_data[8] = temp;

	temp = m_data[3];
	m_data[3] = m_data[12];
	m_data[12] = temp;

	temp = m_data[6];
	m_data[6] = m_data[9];
	m_data[9] = temp;

	temp = m_data[7];
	m_data[7] = m_data[13];
	m_data[13] = temp;

	temp = m_data[11];
	m_data[11] = m_data[14];
	m_data[14] = temp;
}


dmat2::dmat2(void)
{
	memset(m_data,0,sizeof(m_data));
}
dmat2::dmat2(double i)
{
	memset(m_data,0,sizeof(m_data));
	m_data[0] = i;
	m_data[3] = i;
}
dmat2::~dmat2(void)
{
	//pass
}

double* dmat2::data(void)
{
	return m_data;
}

// 矩阵乘法
void dmat2::dot(dmat2& _dmat2)
{
	double *rData = _dmat2.data();
	double tempData[4];
	tempData[0] = m_data[0] * rData[0] + m_data[1] * rData[2];
	tempData[1] = m_data[0] * rData[1] + m_data[1] * rData[3];
	tempData[2] = m_data[2] * rData[0] + m_data[3] * rData[2];
	tempData[3] = m_data[2] * rData[1] + m_data[3] * rData[3];
	memcpy(m_data,tempData,sizeof(m_data));
}

dmat3::dmat3(void)
{
	memset(m_data,0,sizeof(m_data));
}
dmat3::dmat3(double i)
{
	memset(m_data,0,sizeof(m_data));
	m_data[0] = i;
	m_data[4] = i;
	m_data[8] = i;
}
dmat3::~dmat3(void)
{
	//pass
}

double* dmat3::data(void)
{
	return m_data;
}

void dmat3::dot(dmat3& _dmat3)
{
	double* rData = _dmat3.data();
	double tempData[9];
	tempData[0] = m_data[0] * rData[0] + m_data[1] * rData[3] + m_data[2] * rData[6];
	tempData[1] = m_data[0] * rData[1] + m_data[1] * rData[4] + m_data[2] * rData[7];
	tempData[2] = m_data[0] * rData[2] + m_data[1] * rData[5] + m_data[2] * rData[8];
	tempData[3] = m_data[3] * rData[0] + m_data[4] * rData[3] + m_data[5] * rData[6];
	tempData[4] = m_data[3] * rData[1] + m_data[4] * rData[4] + m_data[5] * rData[7];
	tempData[5] = m_data[3] * rData[2] + m_data[4] * rData[5] + m_data[5] * rData[8];
	tempData[6] = m_data[6] * rData[0] + m_data[7] * rData[3] + m_data[8] * rData[6];
	tempData[7] = m_data[6] * rData[1] + m_data[7] * rData[4] + m_data[8] * rData[7];
	tempData[8] = m_data[6] * rData[2] + m_data[7] * rData[5] + m_data[8] * rData[8];
	memcpy(m_data,tempData,sizeof(m_data));
}

dmat4::dmat4(void)
{
	memset(m_data,0,sizeof(m_data));
}
dmat4::dmat4(double i)
{
	memset(m_data,0,sizeof(m_data));
	m_data[0] = i;
	m_data[5] = i;
	m_data[10] = i;
	m_data[15] = i;
}
dmat4::~dmat4(void)
{
	//pass;
}

double* dmat4::data(void)
{
	return m_data;
}

// 矩阵乘法
void dmat4::dot(dmat4& _dmat4)
{
	double *rData = _dmat4.data();
	double tempData[16];
	tempData[0] = m_data[0] * rData[0] + m_data[1] * rData[4] + m_data[2] * rData[8] + m_data[3] * rData[12];
	tempData[1] = m_data[0] * rData[1] + m_data[1] * rData[5] + m_data[2] * rData[9] + m_data[3] * rData[13];
	tempData[2] = m_data[0] * rData[2] + m_data[1] * rData[6] + m_data[2] * rData[10] + m_data[3] * rData[14];
	tempData[3] = m_data[0] * rData[3] + m_data[1] * rData[7] + m_data[2] * rData[11] + m_data[3] * rData[15];
	tempData[4] = m_data[4] * rData[0] + m_data[5] * rData[4] + m_data[6] * rData[8] + m_data[7] * rData[12];
	tempData[5] = m_data[4] * rData[1] + m_data[5] * rData[5] + m_data[6] * rData[9] + m_data[7] * rData[13];
	tempData[6] = m_data[4] * rData[2] + m_data[5] * rData[6] + m_data[6] * rData[10] + m_data[7] * rData[14];
	tempData[7] = m_data[4] * rData[3] + m_data[5] * rData[7] + m_data[6] * rData[11] + m_data[7] * rData[15];
	tempData[8] = m_data[8] * rData[0] + m_data[9] * rData[4] + m_data[10] * rData[8] + m_data[11] * rData[12];
	tempData[9] = m_data[8] * rData[1] + m_data[9] * rData[5] + m_data[10] * rData[9] + m_data[11] * rData[13];
	tempData[10] = m_data[8] * rData[2] + m_data[9] * rData[6] + m_data[10] * rData[10] + m_data[11] * rData[14];
	tempData[11] = m_data[8] * rData[3] + m_data[9] * rData[7] + m_data[10] * rData[11] + m_data[11] * rData[15];
	tempData[12] = m_data[12] * rData[0] + m_data[13] * rData[4] + m_data[14] * rData[8] + m_data[15] * rData[12];
	tempData[13] = m_data[12] * rData[1] + m_data[13] * rData[5] + m_data[14] * rData[9] + m_data[15] * rData[13];
	tempData[14] = m_data[12] * rData[2] + m_data[13] * rData[6] + m_data[14] * rData[10] + m_data[15] * rData[14];
	tempData[15] = m_data[12] * rData[3] + m_data[13] * rData[7] + m_data[14] * rData[11] + m_data[15] * rData[15];
	memcpy(m_data,tempData,sizeof(m_data));
}