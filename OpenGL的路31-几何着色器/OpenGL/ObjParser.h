
#ifndef _OBJPARSER_H_
#define _OBJPARSER_H_

#include "vec.h"

#include <string>
#include <vector>
#include <map>
using namespace std;

class MtlData{
public:
	string name;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Ns;
	string map_Kd;
	string map_Ks;
	string baseDir;
};

class ObjData{
public:
	vector<uvec3> vertexIndexes;
	vector<uvec3> normalIndexes;
	vector<uvec3> texIndexes;
	// count;
	unsigned count;
	// offset
	unsigned first;
};

#ifndef _OBJMODEL_H_
#define _OBJMODEL_H_

class ObjModel{

};

#endif


class ObjParser
{
public:
	ObjParser(void);
	~ObjParser(void);
	static enum ReturnCode{
		PARSE_SUCCESS,
		ERROR_IMPROPER_EXTNAME,
		ERROR_OPEN_FILE_FAIL,
		ERROR_NO_NEW_MTL,
	};

	//int parseString(string& objStr,string& mtlStr);
	int parse(string filePath);
	void to(vector<dvec3>& vertices);

	vector<vec3>& vertices(void);
	vector<vec3>& normals(void);
	vector<vec2>& texCoords(void);
	vector<float>& data(void);
	
	map<string,ObjData>& objDatas(void);
	map<string,MtlData>& mtlDatas(void);

protected:

private:

	string trim(string& str);

	//设置最大最小值
	void setMinMax(dvec3& vertex);
	//最大最小值
	double m_xMin;
	double m_xMax;
	double m_yMin;
	double m_yMax;
	double m_zMin;
	double m_zMax;

	// 法向量数据缓存
	vector<vec3> m_normals;
	// 顶点数据缓存
	vector<vec3> m_vertices;
	// 纹理坐标
	vector<vec2> m_texCoords;
	// 模型
	map<string,ObjData> m_objDatas;
	// 材质
	map<string,MtlData> m_mtlDatas;

	vector<float> m_datas;
};

#endif