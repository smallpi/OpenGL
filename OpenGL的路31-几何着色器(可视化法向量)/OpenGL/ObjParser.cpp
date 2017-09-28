#include "ObjParser.h"

#include <fstream>
#include <sstream>

ObjParser::ObjParser(void)
{
}


ObjParser::~ObjParser(void)
{
}

vector<vec3>& ObjParser::vertices(void){
	return this->m_vertices;
}
vector<vec3>& ObjParser::normals(void){
	return this->m_normals;
}
vector<vec2>& ObjParser::texCoords(void){
	return this->m_texCoords;
}
map<string,ObjData>& ObjParser::objDatas(void){
	return this->m_objDatas;
}
map<string,MtlData>& ObjParser::mtlDatas(void){
	return this->m_mtlDatas;
}
vector<float>& ObjParser::data(void){
	return this->m_datas;
}


string ObjParser::trim(string& str){
	if( str.empty() ){
		return "";
	}
	int begin = str.find_first_not_of(" ");
	if( begin == -1 ){
		return "";
	}
	int end = str.find_last_not_of(" ");
	return str.substr(begin,end-begin+1);
}

/*
*	����.obj�ļ�
*	���ݱ��������ݻ�����
*/
int ObjParser::parse(string filePath){

	m_vertices.clear();
	m_normals.clear();
	m_texCoords.clear();
	m_objDatas.clear();
	m_mtlDatas.clear();

	int fileNamePosition = filePath.find_last_of("/");
	int fileDotPosition = filePath.find_last_of(".");
	
	//�ļ�����
	string fileName = filePath.substr( fileNamePosition+1 );
	//�ļ�·��
	string pathName = filePath.substr( 0, fileNamePosition );
	//�ļ���չ��
	string fileExtName = filePath.substr( fileDotPosition+1 );
	//�ļ���ȥ��չ���� ����
	string fileSelfName = filePath.substr( fileNamePosition+1, fileDotPosition-fileNamePosition-1 );

	if( fileExtName.compare("obj") != 0 ){
		// ���� : �ļ�����չ������Ϊ.obj
		return ERROR_IMPROPER_EXTNAME;
	}

	string mtlFilePath = pathName + "/" + fileSelfName + "." + "mtl";


	//��mtl�ļ�
	ifstream mtlIfstream(mtlFilePath,ios::in);

	string line;
	char strBuff[256];
	MtlData* pMtl = NULL;
	map<string, MtlData> mtlMap;

	//�ж�mtl�ļ��Ƿ����
	if( mtlIfstream ){
		while( !mtlIfstream.eof() ){
			getline(mtlIfstream,line);
			
			if( line.substr(0,6).compare("newmtl") == 0 ){
				//���ʵ�����
				sscanf(line.c_str(), "newmtl %s", strBuff);
				mtlMap[strBuff] = MtlData();
				pMtl = &mtlMap[strBuff];
				pMtl->name = strBuff;
				pMtl->baseDir = pathName;
			}else if(line.substr(0,2).compare("Ka") == 0){
				if(!pMtl){
					return ERROR_NO_NEW_MTL;
				}
				sscanf(line.c_str(), "Ka %f %f %f", &pMtl->Ka.x, &pMtl->Ka.y, &pMtl->Ka.z); 
			}else if(line.substr(0,2).compare("Kd") == 0){
				if(!pMtl){
					return ERROR_NO_NEW_MTL;
				}
				sscanf(line.c_str(), "Kd %f %f %f", &pMtl->Kd.x, &pMtl->Kd.y, &pMtl->Kd.z);
			}else if(line.substr(0,2).compare("Ks") == 0){
				if(!pMtl){
					return ERROR_NO_NEW_MTL;
				}
				sscanf(line.c_str(), "Ks %f %f %f", &pMtl->Ks.x, &pMtl->Ks.y, &pMtl->Ks.z);
			}else if(line.substr(0,2).compare("Ns") == 0){
				if(!pMtl){
					return ERROR_NO_NEW_MTL;
				}
				sscanf(line.c_str(), "Ns %f", &pMtl->Ns);
			}else if(line.substr(0,6).compare("map_Kd") == 0){
				if(!pMtl){
					return ERROR_NO_NEW_MTL;
				}
				sscanf(line.c_str(), "map_Kd %s", strBuff);
				pMtl->map_Kd = strBuff;
			}else if(line.substr(0,6).compare("map_Ks") == 0){
				if(!pMtl){
					return ERROR_NO_NEW_MTL;
				}
				sscanf(line.c_str(), "map_Ks %s", strBuff);
				pMtl->map_Ks = strBuff;
			}
		}
		//�ر�mtl�ļ�
		mtlIfstream.close();
	}

	string mtlName;
	vec3 vertex;	// ��������
	vec3 normal;	// ����������
	vec2 texCoord;	// ��������
	uvec3 vertexIndex;
	uvec3 normalIndex;
	uvec3 texIndex;
	ObjData *pObjData = NULL;

	bool hasVertexTexture = false;
	bool hasVertexNormal = false;

	//��.obj�ļ�
	ifstream objIfstream(filePath);
	if( !objIfstream ){
		//���� : OBJ�ļ���ʧ��
		return ERROR_OPEN_FILE_FAIL;
	}

	while( !objIfstream.eof() ){
		getline(objIfstream, line);

		if( line.substr(0,2).compare("vn") == 0 ){
			//����������
			hasVertexNormal = true;
			sscanf( line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z );
			m_normals.push_back( normal );
		}else if( line.substr(0,2).compare("vt") == 0 ){
			//������������
			hasVertexTexture = true;
			sscanf( line.c_str(), "vt %f %f", &texCoord.x, &texCoord.y);
			m_texCoords.push_back(texCoord);
		}else if( line.substr(0,1).compare("v") == 0 ){
			//��������
			sscanf( line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z );
			m_vertices.push_back( vertex );

			//setMinMax( vertex );
		}else if( line.substr(0,6).compare("usemtl") == 0 ){
			//ģ�Ͷ�Ӧ�Ĳ���
			sscanf(line.c_str(),"usemtl %s",strBuff);
			map<string,ObjData>::iterator iter = m_objDatas.find(strBuff);
			if(iter != m_objDatas.end()){
				pObjData = &m_objDatas[strBuff];
			}else{
				m_objDatas[strBuff] = ObjData();
				pObjData = &m_objDatas[strBuff];
			}

		}else if( line.substr(0,1).compare("f") == 0 ){
			//������
			if( hasVertexTexture ){
				sscanf( line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u",
					&vertexIndex.x, &texIndex.x, &normalIndex.x,
					&vertexIndex.y, &texIndex.y, &normalIndex.y,
					&vertexIndex.z, &texIndex.z, &normalIndex.z );

				pObjData->vertexIndexes.push_back(vertexIndex - 1);
				pObjData->texIndexes.push_back(texIndex - 1);
				pObjData->normalIndexes.push_back(normalIndex - 1);
			}else{
				sscanf( line.c_str(), "f %u//%u %u//%u %u//%u",
					&vertexIndex.x, &normalIndex.x,
					&vertexIndex.y, &normalIndex.y,
					&vertexIndex.z, &normalIndex.z );
				//pObjData->triangleIndexes.push_back( vertexIndex - 1 );
				//normalIndexes.push_back( normalIndex - 1 );
			}
		}
	}

	/*unsigned int totalCount = 0;
	for(map<string,ObjData>::iterator iter = m_objDatas.begin();
		iter != m_objDatas.end();
		iter++)
	{
		totalCount += iter->second.vertexIndexes.size();
	}

	m_datas.reserve(totalCount*24);*/

	unsigned int offset = 0;
	map<string,ObjData>::iterator iter = m_objDatas.begin();
	while(iter != m_objDatas.end()){
		vector<uvec3>& vIdx = iter->second.vertexIndexes;
		vector<uvec3>& tIdx = iter->second.texIndexes;
		vector<uvec3>& nIdx = iter->second.normalIndexes;
		iter->second.first = offset;
		iter->second.count = vIdx.size();

		for(int i=0; i<vIdx.size(); i++){
			m_datas.push_back(m_vertices[vIdx[i].x].x);
			m_datas.push_back(m_vertices[vIdx[i].x].y);
			m_datas.push_back(m_vertices[vIdx[i].x].z);
			m_datas.push_back(m_texCoords[tIdx[i].x].x);
			m_datas.push_back(m_texCoords[tIdx[i].x].y);
			m_datas.push_back(m_normals[nIdx[i].x].x);
			m_datas.push_back(m_normals[nIdx[i].x].y);
			m_datas.push_back(m_normals[nIdx[i].x].z);

			m_datas.push_back(m_vertices[vIdx[i].y].x);
			m_datas.push_back(m_vertices[vIdx[i].y].y);
			m_datas.push_back(m_vertices[vIdx[i].y].z);
			m_datas.push_back(m_texCoords[tIdx[i].y].x);
			m_datas.push_back(m_texCoords[tIdx[i].y].y);
			m_datas.push_back(m_normals[nIdx[i].y].x);
			m_datas.push_back(m_normals[nIdx[i].y].y);
			m_datas.push_back(m_normals[nIdx[i].y].z);

			m_datas.push_back(m_vertices[vIdx[i].z].x);
			m_datas.push_back(m_vertices[vIdx[i].z].y);
			m_datas.push_back(m_vertices[vIdx[i].z].z);
			m_datas.push_back(m_texCoords[tIdx[i].z].x);
			m_datas.push_back(m_texCoords[tIdx[i].z].y);
			m_datas.push_back(m_normals[nIdx[i].z].x);
			m_datas.push_back(m_normals[nIdx[i].z].y);
			m_datas.push_back(m_normals[nIdx[i].z].z);
		}

		iter++;
		offset += vIdx.size();
	}

	m_mtlDatas = mtlMap;

	objIfstream.close();

	return PARSE_SUCCESS;
}

/*
*	���������ݡ����������ݡ�������������������������ɫ 
*	������TriangleModel��
*/

void ObjParser::to(vector<dvec3>& vertices){
	vertices.clear();
	if( m_vertices.empty() ){
		return;
	}
	vertices.resize( m_vertices.size() );

	copy( m_vertices.begin(), m_vertices.end(), vertices.begin() );
}

/*
*	���������Сֵ
*/
void ObjParser::setMinMax(dvec3& vertex){
	if( m_vertices.empty() ){
		m_xMin = vertex.x;
		m_xMax = vertex.x;
		m_yMin = vertex.y;
		m_yMax = vertex.y;
		m_zMin = vertex.z;
		m_zMax = vertex.z;
	}else{
		m_xMin = min(m_xMin, vertex.x);
		m_xMax = max(m_xMax, vertex.x);
		m_yMin = min(m_yMin, vertex.y);
		m_yMax = max(m_yMax, vertex.y);
		m_zMin = min(m_zMin, vertex.z);
		m_zMax = max(m_zMax, vertex.z);
	}
}