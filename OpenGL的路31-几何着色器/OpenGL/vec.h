
#ifndef _VEC_H_
#define _VEC_H_

#define F_TOLERANCE		(1.0e-6F)
#define D_TOLERANCE		(1.0e-6)

class vec2;
class vec3;
class vec4;
class dvec2;
class dvec3;
class dvec4;
class uvec2;
class uvec3;
class uvec4;

vec2 operator - (const vec2& _vec2);
vec3 operator - (const vec3& _vec3);
vec4 operator - (const vec4& _vec4);
dvec2 operator - (const dvec2& _dvec2);
dvec3 operator - (const dvec3& _dvec3);
dvec4 operator - (const dvec4& _dvec4);

vec2 operator * (float _xy, const vec2& _vec2);
vec3 operator * (float _xyz, const vec3& _vec3);
vec3 operator * (float _xyzw, const vec4& _vec4);
dvec2 operator * (double _xy, const dvec2& _dvec2);
dvec3 operator * (double _xyz, const dvec3& _dvec3);
dvec3 operator * (double _xyzw, const dvec4& _dvec4);

class vec{
public:
	static const float* pointer(vec2& _vec2); 
	static const float* pointer(vec3& _vec3);
	static const float* pointer(vec4& _vec4);
	static const double* pointer(dvec2& _dvec2); 
	static const double* pointer(dvec3& _dvec3);
	static const double* pointer(dvec4& _dvec4);

	static vec2 normalize(vec2& _vec2);
	static vec3 normalize(vec3& _vec3);
	static dvec2 normalize(dvec2& _dvec2);
	static dvec3 normalize(dvec3& _dvec3);
};

// two dimension float vector
// 二维float向量
class vec2
{
public:
	vec2(void);
	vec2(const float _xy);
	vec2(const float _x, const float _y);
	vec2(const vec2& _vec2);
	vec2(const vec3& _vec3);
	vec2(const vec4& _vec4);

	~vec2(void);

	float x;
	float y;

	bool equals(const vec2& _vec2);
	float length(void);
	void normalize(void);
	float dot(const vec2& _vec2);
	vec3 cross(const vec2& _vec2);
	//操作符重载
	vec2 operator + (const float _xy) const;
	vec2 operator + (const vec2& _vec2) const;
	vec2 operator - (const float _xy) const;
	vec2 operator - (const vec2& _vec2) const;
	vec2 operator * (const float _xy) const;
	vec2 operator * (const vec2& _vec2) const;
	vec2 operator / (const float _xy) const;
	vec2 operator / (const vec2& _vec2) const;
	void operator += (const float _xy);
	void operator += (const vec2& _vec2);
	void operator -= (const float _xy);
	void operator -= (const vec2& _vec2);
	void operator *= (const float _xy);
	void operator *= (const vec2& _vec2);
	void operator /= (const float _xy);
	void operator /= (const vec2& _vec2);
};

// three dimension float vector
// 三维float向量
class vec3
{
public:
	vec3(void);
	vec3(const float _xyz);
	vec3(const float _x, const float _y);
	vec3(const float _x, const float _y, const float _z);
	vec3(const vec2& _vec2);
	vec3(const vec3& _vec3);
	vec3(const vec4& _vec4);
	vec3(const vec2& _vec2, const float _z);
	vec3(const float _x, const vec2& _vec2);

	~vec3(void);

	float x;
	float y;
	float z;

	bool equals(const vec3& _vec3);
	float length(void);
	void normalize(void);
	float dot(const vec3& _vec3);
	vec3 cross(const vec3& _vec3);
	//操作符重载
	vec3 operator + (const float _xyz) const;
	vec3 operator + (const vec3& _vec3) const;
	vec3 operator - (const float _xyz) const;
	vec3 operator - (const vec3& _vec3) const;
	vec3 operator * (const float _xyz) const;
	vec3 operator * (const vec3& _vec3) const;
	vec3 operator / (const float _xyz) const;
	vec3 operator / (const vec3& _vec3) const;
	void operator += (const float _xyz);
	void operator += (const vec3& _vec3);
	void operator -= (const float _xyz);
	void operator -= (const vec3& _vec3);
	void operator *= (const float _xyz);
	void operator *= (const vec3& _vec3);
	void operator /= (const float _xyz);
	void operator /= (const vec3& _vec3);
};

// float dimension float vector
// 四维float向量
class vec4
{
public:
	vec4(void);
	vec4(const float _xyzw);
	vec4(const float _xyz,const float _w);
	vec4(const float _x,const float _y, const float _z, const float _w);
	vec4(const vec2& _vec2, const float _z, const float _w);
	vec4(const float _x, const vec2& _vec2, const float _w);
	vec4(const float _x, const float _y, const vec2& _vec2);
	vec4(const vec3& _vec3, const float _w);
	vec4(const float _x,const vec3& _vec3);
	vec4(const vec4& _vec4);

	~vec4(void);

	//操作符重载
	vec4 operator + (const float _xyzw) const;
	vec4 operator + (const vec4& _vec4) const;
	vec4 operator - (const float _xyzw) const;
	vec4 operator - (const vec4& _vec4) const;
	vec4 operator * (const float _xyzw) const;
	vec4 operator * (const vec4& _vec4) const;
	vec4 operator / (const float _xyzw) const;
	vec4 operator / (const vec4& _vec4) const;
	void operator += (const float _xyzw);
	void operator += (const vec4& _vec4);
	void operator -= (const float _xyzw);
	void operator -= (const vec4& _vec4);
	void operator *= (const float _xyzw);
	void operator *= (const vec4& _vec4);
	void operator /= (const float _xyzw);
	void operator /= (const vec4& _vec4);

	float x;
	float y;
	float z;
	float w;

};

// two dimension double vector
// 二维double向量
class dvec2
{
public:
	dvec2(void);
	dvec2(const double _xy);
	dvec2(const double _x,const double _y);
	dvec2(const dvec2& _dvec2);
	dvec2(const dvec3& _dvec3);
	dvec2(const dvec4& _dvec4);
	~dvec2(void);

	bool equals(const dvec2& _dvec2);
	double length(void);
	void normalize(void);
	double dot(const dvec2& _dvec2);
	dvec3 cross(const dvec2& _dvec2);
	//操作符重载
	dvec2 operator + (const double _xy) const;
	dvec2 operator + (const dvec2& _dvec2) const;
	dvec2 operator - (const double _xy) const;
	dvec2 operator - (const dvec2& _dvec2) const;
	dvec2 operator * (const double _xy) const;
	dvec2 operator * (const dvec2& _dvec2) const;
	dvec2 operator / (const double _xy) const;
	dvec2 operator / (const dvec2& _dvec2) const;
	void operator += (const double _xy);
	void operator += (const dvec2& _dvec2);
	void operator -= (const double _xy);
	void operator -= (const dvec2& _dvec2);
	void operator *= (const double _xy);
	void operator *= (const dvec2& _dvec2);
	void operator /= (const double _xy);
	void operator /= (const dvec2& _dvec2);

	double x;
	double y;
};


// three dimension double vector
// 三维double向量

class dvec3
{
public:
	dvec3(void);
	dvec3(const double _xyz);
	dvec3(const double _x,double _y);
	dvec3(const double _x, const double _y, const double _z);
	dvec3(const dvec2& _dvec2);
	dvec3(const dvec2& _dvec2, const double _z);
	dvec3(const double _x, const dvec2 _dvec2);
	dvec3(const vec3& _vec3);
	dvec3(const dvec3& _dvec3);
	dvec3(const dvec4& _dvec4);

	~dvec3(void);

	bool equals(const dvec3& _dvec3);
	double length(void) const;
	void normalize(void);
	double dot(const dvec3& _dvec3);
	dvec3 cross(const dvec3& _dvec3);
	//操作符重载
	dvec3 operator + (const double _xyz) const;
	dvec3 operator + (const dvec3& _dvec3) const;
	dvec3 operator - (const double _xyz) const;
	dvec3 operator - (const dvec3& _dvec3) const;
	dvec3 operator * (const double _xyz) const;
	dvec3 operator * (const dvec3& _dvec3) const;
	dvec3 operator / (const double _xyz) const;
	dvec3 operator / (const dvec3& _dvec3) const;
	void operator += (const double _xyz);
	void operator += (const dvec3& _dvec3);
	void operator -= (const double _xyz);
	void operator -= (const dvec3& _dvec3);
	void operator *= (const double _xyz);
	void operator *= (const dvec3& _dvec3);
	void operator /= (const double _xyz);
	void operator /= (const dvec3& _dvec3);


	double x;
	double y;
	double z;
};


// four dimension double vector
// 四维double向量
class dvec4
{
public:
	dvec4(void);
	dvec4(const double _xyzw);
	dvec4(const double _xyz,const double _w);
	dvec4(const double _x,const double _y, const double _z, const double _w);
	dvec4(const dvec2& _dvec2, const double _z, const double _w);
	dvec4(const double _x, const dvec2& _dvec2, const double _w);
	dvec4(const double _x, const double _y, const dvec2& _dvec2);
	dvec4(const dvec3& _dvec3, const double _w);
	dvec4(const double _x,const dvec3& _dvec3);
	dvec4(const dvec4& _dvec4);

	~dvec4(void);

	//操作符重载
	dvec4 operator + (const double _xyzw) const;
	dvec4 operator + (const dvec4& _dvec4) const;
	dvec4 operator - (const double _xyzw) const;
	dvec4 operator - (const dvec4& _dvec4) const;
	dvec4 operator * (const double _xyzw) const;
	dvec4 operator * (const dvec4& _dvec4) const;
	dvec4 operator / (const double _xyzw) const;
	dvec4 operator / (const dvec4& _dvec4) const;
	void operator += (const double _xyzw);
	void operator += (const dvec4& _dvec4);
	void operator -= (const double _xyzw);
	void operator -= (const dvec4& _dvec4);
	void operator *= (const double _xyzw);
	void operator *= (const dvec4& _dvec4);
	void operator /= (const double _xyzw);
	void operator /= (const dvec4& _dvec4);

	double x;
	double y;
	double z;
	double w;
};

// two dimension unsigned int vector
// 二维unsigned int向量
class uvec2{
public:
	uvec2(void);
	~uvec2(void);

	unsigned int x;
	unsigned int y;
};


// three dimension unsigned int vector
// 三维unsigned int向量
class uvec3{
public:
	uvec3(void);
	~uvec3(void);


	//操作符重载
	uvec3 operator - (const unsigned int _xyz);
	uvec3 operator - (const uvec3& _uvec3);

	unsigned int x;
	unsigned int y;
	unsigned int z;
};

// four dimension unsigned int vector
// 四维unsigned int向量
class uvec4{
	uvec4(void);
	~uvec4(void);

	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int w;
};

#endif