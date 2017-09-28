//#include "StdAfx.h"
#include "vec.h"
#include <math.h>

//全局操作符重载
vec2 operator - (const vec2& _vec2){
	return (_vec2 * (-1.0f));
}
vec3 operator - (const vec3& _vec3){
	return (_vec3 * (-1.0f));
}
vec4 operator - (const vec4& _vec4){
	return (_vec4 * (-1.0f));
}
dvec2 operator - (const dvec2& _dvec2){
	return (_dvec2 * (-1.0));
}
dvec3 operator - (const dvec3& _dvec3){
	return (_dvec3 * (-1.0));
}
dvec4 operator - (const dvec4& _dvec4){
	return (_dvec4 * (-1.0));
}
//数乘法
vec2 operator * (float _xy, const vec2& _vec2){
	return _vec2 * _xy;
}
vec3 operator * (float _xyz, const vec3& _vec3){
	return _vec3 * _xyz;
}
vec3 operator * (float _xyzw, const vec4& _vec4){
	return _vec4 * _xyzw;
}
dvec2 operator * (double _xy, const dvec2& _dvec2){
	return _dvec2 * _xy;
}
dvec3 operator * (double _xyz, const dvec3& _dvec3){
	return _dvec3 * _xyz;
}
dvec3 operator * (double _xyzw, const dvec4& _dvec4){
	return _dvec4 * _xyzw;
}




const float* vec::pointer(vec2& _vec2){
	return &(_vec2.x);
}
const float* vec::pointer(vec3& _vec3){
	return &(_vec3.x);
}
const float* vec::pointer(vec4& _vec4){
	return &(_vec4.x);
}
const double* vec::pointer(dvec2& _dvec2){
	return &(_dvec2.x);
}
const double* vec::pointer(dvec3& _dvec3){
	return &(_dvec3.x);
}
const double* vec::pointer(dvec4& _dvec4){
	return &(_dvec4.x);
}

vec2 vec::normalize(vec2& _vec2){
	vec2 _res = _vec2;
	_res.normalize();
	return _res;
}
vec3 vec::normalize(vec3& _vec3){
	vec3 _res =_vec3;
	_res.normalize();
	return _res;
}
dvec2 vec::normalize(dvec2& _dvec2){
	dvec2 _res = _dvec2;
	_res.normalize();
	return _res;
}
dvec3 vec::normalize(dvec3& _dvec3){
	dvec3 _res = _dvec3;
	_res.normalize();
	return _res;
}


// two dimensions float vector
vec2::vec2(void):
	x(0.0f),
	y(0.0f)
{

}

vec2::~vec2(void){

}

vec2::vec2(const float _xy):
	x(_xy),
	y(_xy)
{

}

vec2::vec2(const float _x,const float _y):
	x(_x),
	y(_y)
{

}

vec2::vec2(const vec2& _vec2):
	x(_vec2.x),
	y(_vec2.y)
{

}

vec2::vec2(const vec3& _vec3):
	x(_vec3.x),
	y(_vec3.y)
{

}

vec2::vec2(const vec4& _vec4):
	x(_vec4.x),
	y(_vec4.y)
{

}

bool vec2::equals(const vec2& _vec2){
	bool _equalx = abs(this->x - _vec2.x) < F_TOLERANCE;
	bool _equaly = abs(this->y - _vec2.y) < F_TOLERANCE;
	return ( _equalx && _equaly );
}

float vec2::length(void){
	float len;
	len = sqrt(this->x * this->x + this->y * this->y);
	return len;
}

void vec2::normalize(void){
	float len;
	len = this->length();
	if( len < F_TOLERANCE ){
		this->x = 0.0f;
		this->y = 0.0f;
		return;
	}
	this->x = this->x / len;
	this->y = this->y / len;
}

float vec2::dot(const vec2& _vec2){
	float res;
	res = this->x * _vec2.x + this->y * _vec2.y;
	return res;
}

vec3 vec2::cross(const vec2& _vec2){
	float _z;
	_z = this->x * _vec2.y - this->y * _vec2.x;
	return vec3(0.0f,0.0f,_z);
}

vec2 vec2::operator + (const float _xy) const{
	return vec2(
		this->x + _xy, 
		this->y + _xy);
}

vec2 vec2::operator + (const vec2& _vec2) const{
	return vec2(
		this->x + _vec2.x, 
		this->y + _vec2.y);
}

vec2 vec2::operator - (const float _xy) const{
	return vec2(
		this->x - _xy, 
		this->y - _xy);
}

vec2 vec2::operator - (const vec2& _vec2) const{
	return vec2(
		this->x - _vec2.x, 
		this->y - _vec2.y);
}

vec2 vec2::operator * (const float _xy) const{
	return vec2(
		this->x * _xy, 
		this->y * _xy);
}

vec2 vec2::operator * (const vec2& _vec2) const{
	return vec2(
		this->x * _vec2.x, 
		this->y * _vec2.y);
}

vec2 vec2::operator / (const float _xy) const{
	return vec2(
		this->x / _xy, 
		this->y / _xy);
}

vec2 vec2::operator / (const vec2& _vec2) const{
	return vec2(
		this->x / _vec2.x, 
		this->y / _vec2.y);
}

void vec2::operator += (const float _xy){
	this->x += _xy;
	this->y += _xy;
}
void vec2::operator += (const vec2& _vec2){
	this->x += _vec2.x;
	this->y += _vec2.y;
}
void vec2::operator -= (const float _xy){
	this->x -= _xy;
	this->x -= _xy;
}
void vec2::operator -= (const vec2& _vec2){
	this->x -= _vec2.x;
	this->y -= _vec2.y;
}
void vec2::operator *= (const float _xy){
	this->x *= _xy;
	this->x *= _xy;
}
void vec2::operator *= (const vec2& _vec2){
	this->x *= _vec2.x;
	this->y *= _vec2.y;
}
void vec2::operator /= (const float _xy){
	this->x /= _xy;
	this->x /= _xy;
}
void vec2::operator /= (const vec2& _vec2){
	this->x /= _vec2.x;
	this->y /= _vec2.y;
}




// Three dimensions float vector
vec3::vec3(void):
	x(0.0f),
	y(0.0f),
	z(0.0f)
{

}

vec3::~vec3(void)
{
}

vec3::vec3(const float _xyz):
	x(_xyz),
	y(_xyz),
	z(_xyz)
{

}

vec3::vec3(const float _x, const float _y):
	x(_x),
	y(_y),
	z(0.0f)
{

}

vec3::vec3(const float _x, const float _y, const float _z):
	x(_x),
	y(_y),
	z(_z)
{

}

vec3::vec3(const vec3& _vec3):
	x(_vec3.x),
	y(_vec3.y),
	z(_vec3.z)
{

}

vec3::vec3(const vec2& _vec2):
	x(_vec2.x),
	y(_vec2.y),
	z(0.0f)
{

}

vec3::vec3(const vec4& _vec4):
	x(_vec4.x),
	y(_vec4.y),
	z(_vec4.z)
{

}

vec3::vec3(const vec2& _vec2, const float _z):
	x(_vec2.x),
	y(_vec2.y),
	z(_z)
{

}

vec3::vec3(const float _x,const vec2& _vec2):
	x(_x),
	y(_vec2.x),
	z(_vec2.y)
{

}

bool vec3::equals(const vec3& _vec3){
	bool _equalx = abs(this->x - _vec3.x) < F_TOLERANCE;
	bool _equaly = abs(this->y - _vec3.y) < F_TOLERANCE;
	bool _equalz = abs(this->z - _vec3.z) < F_TOLERANCE;

	return (_equalx && _equaly && _equalz);
}

float vec3::length(void){
	float _xx = this->x * this->x;
	float _yy = this->y * this->y;
	float _zz = this->z * this->z;

	return sqrt(_xx + _yy + _zz);
}

void vec3::normalize(void){
	float len = this->length();
	if(len < F_TOLERANCE){
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
		return;
	}
	this->x = this->x / len;
	this->y = this->y / len;
	this->z = this->z / len;
}

float vec3::dot(const vec3& _vec3){
	float _xx = this->x * _vec3.x;
	float _yy = this->y * _vec3.y;
	float _zz = this->z * _vec3.z;
	return (_xx + _yy + _zz);
}

vec3 vec3::cross(const vec3& _vec3){
	float _x = this->y * _vec3.z - this->z * _vec3.y;
	float _y = this->z * _vec3.x - this->x * _vec3.z;
	float _z = this->x * _vec3.y - this->y * _vec3.x;
	return vec3(_x,_y,_z);
}

vec3 vec3::operator + (const float _xyz) const{
	return vec3(
		this->x + _xyz, 
		this->y + _xyz, 
		this->z + _xyz);
}
vec3 vec3::operator + (const vec3& _vec3) const{
	return vec3(
		this->x + _vec3.x, 
		this->y + _vec3.y, 
		this->z + _vec3.z);
}
vec3 vec3::operator - (const float _xyz) const{
	return vec3(
		this->x - _xyz, 
		this->y - _xyz, 
		this->z - _xyz);
}
vec3 vec3::operator - (const vec3& _vec3) const{
	return vec3(
		this->x - _vec3.x, 
		this->y - _vec3.y, 
		this->z - _vec3.z); 
}
vec3 vec3::operator * (const float _xyz) const{
	return vec3(
		this->x * _xyz, 
		this->y * _xyz, 
		this->z * _xyz);
}
vec3 vec3::operator * (const vec3& _vec3) const{
	return vec3(
		this->x * _vec3.x, 
		this->y * _vec3.y, 
		this->z * _vec3.z);
}
vec3 vec3::operator / (const float _xyz) const{
	return vec3(
		this->x / _xyz, 
		this->y / _xyz, 
		this->z / _xyz);
}
vec3 vec3::operator / (const vec3& _vec3) const{
	return vec3(
		this->x / _vec3.x, 
		this->y / _vec3.y, 
		this->z / _vec3.z);
}
void vec3::operator += (const float _xyz){
	this->x += _xyz;
	this->y += _xyz;
	this->z += _xyz;
}
void vec3::operator += (const vec3& _vec3){
	this->x += _vec3.x;
	this->y += _vec3.y;
	this->z += _vec3.z;
}
void vec3::operator -= (const float _xyz){
	this->x -= _xyz;
	this->y -= _xyz;
	this->z -= _xyz;
}
void vec3::operator -= (const vec3& _vec3){
	this->x -= _vec3.x;
	this->y -= _vec3.y;
	this->z -= _vec3.z;
}
void vec3::operator *= (const float _xyz){
	this->x *= _xyz;
	this->y *= _xyz;
	this->z *= _xyz;
}
void vec3::operator *= (const vec3& _vec3){
	this->x *= _vec3.x;
	this->y *= _vec3.y;
	this->z *= _vec3.z;
}
void vec3::operator /= (const float _xyz){
	this->x /= _xyz;
	this->y /= _xyz;
	this->z /= _xyz;
}
void vec3::operator /= (const vec3& _vec3){
	this->x /= _vec3.x;
	this->y /= _vec3.y;
	this->z /= _vec3.z;
}



// four dimensions float vector
vec4::vec4(void):
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(1.0f)
{

}

vec4::vec4(const float _xyzw):
	x(_xyzw),
	y(_xyzw),
	z(_xyzw),
	w(_xyzw)
{

}

vec4::vec4(const float _xyz,const float _w):
	x(_xyz),
	y(_xyz),
	z(_xyz),
	w(_w)
{

}

vec4::vec4(const float _x,const float _y, const float _z, const float _w):
	x(_x),
	y(_y),
	z(_z),
	w(_w)
{

}

vec4::vec4(const vec2& _vec2, const float _z, const float _w):
	x(_vec2.x),
	y(_vec2.y),
	z(_z),
	w(_w)
{

}

vec4::vec4(const float _x, const vec2& _vec2, const float _w):
	x(_x),
	y(_vec2.x),
	z(_vec2.y),
	w(_w)
{

}

vec4::vec4(const float _x, const float _y, const vec2& _vec2):
	x(_x),
	y(_y),
	z(_vec2.x),
	w(_vec2.y)
{

}

vec4::vec4(const vec3& _vec3,const float _w):
	x(_vec3.x),
	y(_vec3.y),
	z(_vec3.z),
	w(_w)
{

}

vec4::vec4(const float _x,const vec3& _vec3):
	x(_x),
	y(_vec3.x),
	z(_vec3.y),
	w(_vec3.z)
{

}

vec4::vec4(const vec4& _vec4):
	x(_vec4.x),
	y(_vec4.y),
	z(_vec4.z),
	w(_vec4.w)
{

}

vec4::~vec4(void)
{
}

vec4 vec4::operator + (const float _xyzw) const{
	return vec4(
		this->x + _xyzw,
		this->y + _xyzw,
		this->z + _xyzw,
		this->w + _xyzw);
}
vec4 vec4::operator + (const vec4& _vec4) const{
	return vec4(
		this->x + _vec4.x,
		this->y + _vec4.y,
		this->z + _vec4.z,
		this->w + _vec4.w);
}
vec4 vec4::operator - (const float _xyzw) const{
	return vec4(
		this->x - _xyzw,
		this->y - _xyzw,
		this->z - _xyzw,
		this->w - _xyzw);
}
vec4 vec4::operator - (const vec4& _vec4) const{
	return vec4(
		this->x - _vec4.x,
		this->y - _vec4.y,
		this->z - _vec4.z,
		this->w - _vec4.w);
}
vec4 vec4::operator * (const float _xyzw) const{
	return vec4(
		this->x * _xyzw,
		this->y * _xyzw,
		this->z * _xyzw,
		this->w * _xyzw);
}
vec4 vec4::operator * (const vec4& _vec4) const{
	return vec4(
		this->x * _vec4.x,
		this->y * _vec4.y,
		this->z * _vec4.z,
		this->w * _vec4.w);
}
vec4 vec4::operator / (const float _xyzw) const{
	return vec4(
		this->x / _xyzw,
		this->y / _xyzw,
		this->z / _xyzw,
		this->w / _xyzw);
}
vec4 vec4::operator / (const vec4& _vec4) const{
	return vec4(
		this->x / _vec4.x,
		this->y / _vec4.y,
		this->z / _vec4.z,
		this->w / _vec4.w);
}
void vec4::operator += (const float _xyzw){
	this->x += _xyzw;
	this->y += _xyzw;
	this->z += _xyzw;
	this->w += _xyzw;
}
void vec4::operator += (const vec4& _vec4){
	this->x += _vec4.x;
	this->y += _vec4.y;
	this->z += _vec4.z;
	this->w += _vec4.w;
}
void vec4::operator -= (const float _xyzw){
	this->x -= _xyzw;
	this->y -= _xyzw;
	this->z -= _xyzw;
	this->w -= _xyzw;
}
void vec4::operator -= (const vec4& _vec4){
	this->x -= _vec4.x;
	this->y -= _vec4.y;
	this->z -= _vec4.z;
	this->w -= _vec4.w;
}
void vec4::operator *= (const float _xyzw){
	this->x *= _xyzw;
	this->y *= _xyzw;
	this->z *= _xyzw;
	this->w *= _xyzw;
}
void vec4::operator *= (const vec4& _vec4){
	this->x *= _vec4.x;
	this->y *= _vec4.y;
	this->z *= _vec4.z;
	this->w *= _vec4.w;
}
void vec4::operator /= (const float _xyzw){
	this->x /= _xyzw;
	this->y /= _xyzw;
	this->z /= _xyzw;
	this->w /= _xyzw;
}
void vec4::operator /= (const vec4& _vec4){
	this->x /= _vec4.x;
	this->y /= _vec4.y;
	this->z /= _vec4.z;
	this->w /= _vec4.w;
}


// two dimensions double vector
dvec2::dvec2(void):
	x(0.0),
	y(0.0)
{

}

dvec2::dvec2(const double _xy):
	x(_xy),
	y(_xy)
{

}

dvec2::dvec2(const double _x,const double _y):
	x(_x),
	y(_y)
{

}

dvec2::dvec2(const dvec2& _dvec2):
	x(_dvec2.x),
	y(_dvec2.y)
{

}

dvec2::dvec2(const dvec3& _dvec3):
	x(_dvec3.x),
	y(_dvec3.y)
{

}

dvec2::dvec2(const dvec4& _dvec4):
	x(_dvec4.x),
	y(_dvec4.y)
{

}

dvec2::~dvec2(void)
{
}

bool dvec2::equals(const dvec2& _dvec2){
	bool _equalx = abs(this->x - _dvec2.x) < D_TOLERANCE;
	bool _equaly = abs(this->y - _dvec2.y) < D_TOLERANCE;

	return (_equalx && _equaly);
}

double dvec2::length(void){
	double _xx = this->x * this->x;
	double _yy = this->y * this->y;
	return sqrt(_xx + _yy);
}

void dvec2::normalize(void){
	double len = this->length();
	if(len < D_TOLERANCE){
		this->x = 0.0;
		this->y = 0.0;
		return;
	}
	this->x = this->x / len;
	this->y = this->y / len;
}

double dvec2::dot(const dvec2& _dvec2){
	double _xx = this->x * _dvec2.x;
	double _yy = this->y * _dvec2.y;
	return (_xx + _yy);
}

dvec3 dvec2::cross(const dvec2& _dvec2){
	double _z = this->x * _dvec2.y - this->y * _dvec2.x;
	return dvec3(0.0,0.0,_z);
}

dvec2 dvec2::operator + (const double _xy) const{
	return dvec2(
		this->x + _xy, 
		this->y + _xy);
}

dvec2 dvec2::operator + (const dvec2& _dvec2) const{
	return dvec2(
		this->x + _dvec2.x, 
		this->y + _dvec2.y);
}

dvec2 dvec2::operator - (const double _xy) const{
	return dvec2(
		this->x - _xy, 
		this->y - _xy);
}

dvec2 dvec2::operator - (const dvec2& _dvec2) const{
	return dvec2(
		this->x - _dvec2.x, 
		this->y - _dvec2.y);
}

dvec2 dvec2::operator * (const double _xy) const{
	return dvec2(
		this->x * _xy, 
		this->y * _xy);
}

dvec2 dvec2::operator * (const dvec2& _dvec2) const{
	return dvec2(
		this->x * _dvec2.x, 
		this->y * _dvec2.y);
}

dvec2 dvec2::operator / (const double _xy) const{
	return dvec2(
		this->x / _xy, 
		this->y / _xy);
}

dvec2 dvec2::operator / (const dvec2& _dvec2) const{
	return dvec2(
		this->x / _dvec2.x, 
		this->y / _dvec2.y);
}
void dvec2::operator += (const double _xy){
	this->x += _xy;
	this->y += _xy;
}
void dvec2::operator += (const dvec2& _dvec2){
	this->x += _dvec2.x;
	this->y += _dvec2.y;
}
void dvec2::operator -= (const double _xy){
	this->x -= _xy;
	this->x -= _xy;
}
void dvec2::operator -= (const dvec2& _dvec2){
	this->x -= _dvec2.x;
	this->y -= _dvec2.y;
}
void dvec2::operator *= (const double _xy){
	this->x *= _xy;
	this->x *= _xy;
}
void dvec2::operator *= (const dvec2& _dvec2){
	this->x *= _dvec2.x;
	this->y *= _dvec2.y;
}
void dvec2::operator /= (const double _xy){
	this->x /= _xy;
	this->x /= _xy;
}
void dvec2::operator /= (const dvec2& _dvec2){
	this->x /= _dvec2.x;
	this->y /= _dvec2.y;
}




// three dimension double vector
dvec3::dvec3(void):
	x(0.0),
	y(0.0),
	z(0.0)
{

}

dvec3::dvec3(const double _xyz):
	x(_xyz),
	y(_xyz),
	z(_xyz)
{

}

dvec3::dvec3(const double _x,const double _y):
	x(_x),
	y(_y),
	z(0.0)
{

}

dvec3::dvec3(const double _x, const double _y, const double _z):
	x(_x),
	y(_y),
	z(_z)
{

}

dvec3::dvec3(const dvec2& _dvec2, const double _z):
	x(_dvec2.x),
	y(_dvec2.y),
	z(_z)
{

}

dvec3::dvec3(const double _x, const dvec2 _dvec2):
	x(_x),
	y(_dvec2.x),
	z(_dvec2.y)
{

}

dvec3::dvec3(const vec3& _vec3):
	x(_vec3.x),
	y(_vec3.y),
	z(_vec3.z)
{

}

dvec3::dvec3(const dvec3& _dvec3):
	x(_dvec3.x),
	y(_dvec3.y),
	z(_dvec3.z)
{
}

dvec3::dvec3(const dvec4& _dvec4):
	x(_dvec4.x),
	y(_dvec4.y),
	z(_dvec4.z)
{

}

dvec3::~dvec3(void)
{
}

bool dvec3::equals(const dvec3& _dvec3){
	bool _equalx = abs(this->x - _dvec3.x) < D_TOLERANCE;
	bool _equaly = abs(this->y - _dvec3.y) < D_TOLERANCE;
	bool _equalz = abs(this->z - _dvec3.z) < D_TOLERANCE;

	return (_equalx && _equaly && _equalz);
}

double dvec3::length(void) const{
	double _xx = this->x * this->x;
	double _yy = this->y * this->y;
	double _zz = this->z * this->z;

	return sqrt(_xx + _yy + _zz);
}

void dvec3::normalize(void){
	double len = this->length();
	if(len < D_TOLERANCE){
		this->x = 0.0;
		this->y = 0.0;
		this->z = 0.0;
		return;
	}
	this->x = this->x / len;
	this->y = this->y / len;
	this->z = this->z / len;
}

double dvec3::dot(const dvec3& _dvec3){
	double _xx = this->x * _dvec3.x;
	double _yy = this->y * _dvec3.y;
	double _zz = this->z * _dvec3.z;

	return (_xx + _yy + _zz);
}

dvec3 dvec3::cross(const dvec3& _dvec3){
	double _x = this->y * _dvec3.z - this->z * _dvec3.y;
	double _y = this->z * _dvec3.x - this->x * _dvec3.z;
	double _z = this->x * _dvec3.y - this->y * _dvec3.x;
	return dvec3(_x,_y,_z);
}

dvec3 dvec3::operator + (const double _xyz) const{
	return dvec3(
		this->x + _xyz, 
		this->y + _xyz, 
		this->z + _xyz);
}
dvec3 dvec3::operator + (const dvec3& _dvec3) const{
	return dvec3(
		this->x + _dvec3.x, 
		this->y + _dvec3.y, 
		this->z + _dvec3.z);
}
dvec3 dvec3::operator - (const double _xyz) const{
	return dvec3(
		this->x - _xyz, 
		this->y - _xyz, 
		this->z - _xyz);
}
dvec3 dvec3::operator - (const dvec3& _dvec3) const{
	return dvec3(
		this->x - _dvec3.x, 
		this->y - _dvec3.y, 
		this->z - _dvec3.z); 
}
dvec3 dvec3::operator * (const double _xyz) const{
	return dvec3(
		this->x * _xyz, 
		this->y * _xyz, 
		this->z * _xyz);
}
dvec3 dvec3::operator * (const dvec3& _dvec3) const{
	return dvec3(
		this->x * _dvec3.x, 
		this->y * _dvec3.y, 
		this->z * _dvec3.z);
}
dvec3 dvec3::operator / (const double _xyz) const{
	return dvec3(
		this->x / _xyz, 
		this->y / _xyz, 
		this->z / _xyz);
}
dvec3 dvec3::operator / (const dvec3& _dvec3) const{
	return dvec3(
		this->x / _dvec3.x, 
		this->y / _dvec3.y, 
		this->z / _dvec3.z);
}
void dvec3::operator += (const double _xyz){
	this->x += _xyz;
	this->y += _xyz;
	this->z += _xyz;
}
void dvec3::operator += (const dvec3& _dvec3){
	this->x += _dvec3.x;
	this->y += _dvec3.y;
	this->z += _dvec3.z;
}
void dvec3::operator -= (const double _xyz){
	this->x -= _xyz;
	this->y -= _xyz;
	this->z -= _xyz;
}
void dvec3::operator -= (const dvec3& _dvec3){
	this->x -= _dvec3.x;
	this->y -= _dvec3.y;
	this->z -= _dvec3.z;
}
void dvec3::operator *= (const double _xyz){
	this->x *= _xyz;
	this->y *= _xyz;
	this->z *= _xyz;
}
void dvec3::operator *= (const dvec3& _dvec3){
	this->x *= _dvec3.x;
	this->y *= _dvec3.y;
	this->z *= _dvec3.z;
}
void dvec3::operator /= (const double _xyz){
	this->x /= _xyz;
	this->y /= _xyz;
	this->z /= _xyz;
}
void dvec3::operator /= (const dvec3& _dvec3){
	this->x /= _dvec3.x;
	this->y /= _dvec3.y;
	this->z /= _dvec3.z;
}





// four dimension double vector
dvec4::dvec4(void):
	x(0.0),
	y(0.0),
	z(0.0),
	w(1.0)
{

}

dvec4::dvec4(const double _xyzw):
	x(_xyzw),
	y(_xyzw),
	w(_xyzw),
	z(_xyzw)
{

}

dvec4::dvec4(const double _xyz,const double _w):
	x(_xyz),
	y(_xyz),
	z(_xyz),
	w(_w)
{

}

dvec4::dvec4(const double _x,const double _y, const double _z, const double _w):
	x(_x),
	y(_y),
	w(_w),
	z(_z)
{

}

dvec4::dvec4(const dvec2& _dvec2, const double _z, const double _w):
	x(_dvec2.x),
	y(_dvec2.y),
	z(_z),
	w(_w)
{

}

dvec4::dvec4(const double _x, const dvec2& _dvec2, const double _w):
	x(_x),
	y(_dvec2.x),
	z(_dvec2.y),
	w(_w)
{

}

dvec4::dvec4(const double _x, const double _y, const dvec2& _dvec2):
	x(_x),
	y(_y),
	z(_dvec2.x),
	w(_dvec2.y)
{

}

dvec4::dvec4(const dvec3& _dvec3, const double _w):
	x(_dvec3.x),
	y(_dvec3.y),
	z(_dvec3.z),
	w(_w)
{

}

dvec4::dvec4(const double _x,const dvec3& _dvec3):
	x(_x),
	y(_dvec3.x),
	z(_dvec3.y),
	w(_dvec3.z)
{

}

dvec4::dvec4(const dvec4& _dvec4):
	x(_dvec4.x),
	y(_dvec4.y),
	z(_dvec4.z),
	w(_dvec4.w)
{

}

dvec4::~dvec4(void)
{
}

dvec4 dvec4::operator + (const double _xyzw) const{
	return dvec4(
		this->x + _xyzw,
		this->y + _xyzw,
		this->z + _xyzw,
		this->w + _xyzw);
}
dvec4 dvec4::operator + (const dvec4& _dvec4) const{
	return dvec4(
		this->x + _dvec4.x,
		this->y + _dvec4.y,
		this->z + _dvec4.z,
		this->w + _dvec4.w);
}
dvec4 dvec4::operator - (const double _xyzw) const{
	return dvec4(
		this->x - _xyzw,
		this->y - _xyzw,
		this->z - _xyzw,
		this->w - _xyzw);
}
dvec4 dvec4::operator - (const dvec4& _dvec4) const{
	return dvec4(
		this->x - _dvec4.x,
		this->y - _dvec4.y,
		this->z - _dvec4.z,
		this->w - _dvec4.w);
}
dvec4 dvec4::operator * (const double _xyzw) const{
	return dvec4(
		this->x * _xyzw,
		this->y * _xyzw,
		this->z * _xyzw,
		this->w * _xyzw);
}
dvec4 dvec4::operator * (const dvec4& _dvec4) const{
	return dvec4(
		this->x * _dvec4.x,
		this->y * _dvec4.y,
		this->z * _dvec4.z,
		this->w * _dvec4.w);
}
dvec4 dvec4::operator / (const double _xyzw) const{
	return dvec4(
		this->x / _xyzw,
		this->y / _xyzw,
		this->z / _xyzw,
		this->w / _xyzw);
}
dvec4 dvec4::operator / (const dvec4& _dvec4) const{
	return dvec4(
		this->x / _dvec4.x,
		this->y / _dvec4.y,
		this->z / _dvec4.z,
		this->w / _dvec4.w);
}
void dvec4::operator += (const double _xyzw){
	this->x += _xyzw;
	this->y += _xyzw;
	this->z += _xyzw;
	this->w += _xyzw;
}
void dvec4::operator += (const dvec4& _dvec4){
	this->x += _dvec4.x;
	this->y += _dvec4.y;
	this->z += _dvec4.z;
	this->w += _dvec4.w;
}
void dvec4::operator -= (const double _xyzw){
	this->x -= _xyzw;
	this->y -= _xyzw;
	this->z -= _xyzw;
	this->w -= _xyzw;
}
void dvec4::operator -= (const dvec4& _dvec4){
	this->x -= _dvec4.x;
	this->y -= _dvec4.y;
	this->z -= _dvec4.z;
	this->w -= _dvec4.w;
}
void dvec4::operator *= (const double _xyzw){
	this->x *= _xyzw;
	this->y *= _xyzw;
	this->z *= _xyzw;
	this->w *= _xyzw;
}
void dvec4::operator *= (const dvec4& _dvec4){
	this->x *= _dvec4.x;
	this->y *= _dvec4.y;
	this->z *= _dvec4.z;
	this->w *= _dvec4.w;
}
void dvec4::operator /= (const double _xyzw){
	this->x /= _xyzw;
	this->y /= _xyzw;
	this->z /= _xyzw;
	this->w /= _xyzw;
}
void dvec4::operator /= (const dvec4& _dvec4){
	this->x /= _dvec4.x;
	this->y /= _dvec4.y;
	this->z /= _dvec4.z;
	this->w /= _dvec4.w;
}


// two dimension unsigned int vector
// 二维unsigned int向量
uvec2::uvec2(void):
	x(0),
	y(0)
{

}
uvec2::~uvec2(void){
	
}

// three dimension unsigned int vector
// 三维unsigned int向量
uvec3::uvec3(void):
	x(0),
	y(0),
	z(0)
{
}
uvec3::~uvec3(void){

}

uvec3 uvec3::operator-(const unsigned int _uint){
	uvec3 _res;
	_res.x = this->x - _uint;
	_res.y = this->y - _uint;
	_res.z = this->z - _uint;
	return _res;
}
uvec3 uvec3::operator-(const uvec3& _uint3){
	uvec3 _res;
	_res.x = this->x - _uint3.x;
	_res.y = this->y - _uint3.y;
	_res.z = this->z - _uint3.z;
	return _res;
}

// four dimension unsigned int vector
// 四维unsigned int向量
uvec4::uvec4(void):
	x(0),
	y(0),
	z(0),
	w(0)
{

}

uvec4::~uvec4(void){
	
}