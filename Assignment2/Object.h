#ifndef _OBJECT_H_
#define _OBJECT_H_

//��ͷ�ļ��ж����˶���������

#include "Vector3.h"
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

//������
class Object
{
public:
	Vector3 surfaceColor; //������ɫ
	Vector3 emissionColor; //���������ɫ
	float transparency; //͸����
	float reflection; //������

	//���캯��
	Object(const Vector3 &sc,
		const Vector3 &ec,
		const float &t = 0,
		const float &r = 0);
	/*
	�жϹ����������Ƿ��н���
	������originRay->��ǰ��Դλ��
	directionRay->��ǰ���ߵķ�������
	t0,t1->��Դ�����������ľ���
	*/
	virtual bool intersect(const Vector3 &originRay,
		const Vector3 &directionRay,
		float &t0, float &t1) const = 0;

	virtual Vector3 nhit(const Vector3 &phit) const = 0;
};

Object::Object(const Vector3 &sc, const Vector3 &ec, const float &t, const float &r)
{
	surfaceColor = sc;
	emissionColor = ec;
	transparency = t;
	reflection = r;
}

//����Sphere
class Sphere :public Object
{
public:
	Vector3 center; //��������
	float radius; //�뾶

	//���캯��
	Sphere(const Vector3 &c,
		const float &ra,
		const Vector3 &sc,
		const Vector3 &ec = 0,
		const float &t = 0,
		const float &r = 0);
	bool intersect(const Vector3 &originRay,
		const Vector3 &directionRay,
		float &t0, float &t1) const;
	Vector3 nhit(const Vector3 &phit) const;
};

Sphere::Sphere(const Vector3 &c, const float &ra, const Vector3 &sc, const Vector3 &ec, const float &t, const float &r) :
	center(c), radius(ra), Object(sc, ec, t, r) {}

bool Sphere::intersect(const Vector3 &originRay,
	const Vector3 &directionRay,
	float &t0, float &t1) const
{
	//center���Դλ�����ߵķ�������
	Vector3 v = center - originRay;
	//�жϹ����Ƿ��������н���
	float a0 = v.dotmul(directionRay);
	//���������v�нǴ���pi/2����û�н���
	if (a0 < 0) {
		return false;
	}
	//a^2=b^2+c^2
	float dist = v.dotmul(v) - a0 * a0;
	//�����Դ�����߾�����ڰ뾶����û�н���
	if (dist > radius*radius) {
		return false;
	}
	float a1 = sqrt(radius*radius - dist);
	//t1: ��һ�����㵽��Դ�ľ���, t2: �ڶ������㵽��Դ�ľ���
	t0 = a0 - a1;
	t1 = a0 + a1;

	return true;
}

Vector3 Sphere::nhit(const Vector3 &phit) const
{
	//normal of the intersection point
	Vector3 nhit = phit - center;
	nhit.normal();
	return nhit;
}

//��������Cube
class Cube :public Object
{
public:
	Vector3 vertex; //����
	float xLen, yLen, zLen; //�����

							//���캯��
	Cube(const Vector3 &v,
		const float &x,
		const float &y,
		const float &z,
		const Vector3 &sc,
		const Vector3 &ec,
		const float &t = 0,
		const float &r = 0);

	bool intersect(const Vector3 &originRay,
		const Vector3 &directionRay,
		float &t1, float &t2) const;
	Vector3 nhit(const Vector3 &phit) const;
};

Cube::Cube(const Vector3 &v, const float &x, const float &y, const float &z, const Vector3 &sc, const Vector3 &ec, const float &t, const float &r) :
	vertex(v), xLen(x), yLen(y), zLen(z), Object(sc, ec, t, r) {}

bool Cube::intersect(const Vector3 &originRay, const Vector3 &directionRay, float &t1, float &t2) const
{
	float t01 = -(vertex.x + Vector3(-1, 0, 0).dotmul(originRay)) / (Vector3(-1, 0, 0).dotmul(directionRay));
	float t02 = -(-vertex.x - xLen + Vector3(1, 0, 0).dotmul(originRay)) / (Vector3(1, 0, 0).dotmul(directionRay));
	float t11 = -(vertex.y + Vector3(0, -1, 0).dotmul(originRay)) / (Vector3(0, -1, 0).dotmul(directionRay));
	float t12 = -(-vertex.y - yLen + Vector3(0, 1, 0).dotmul(originRay)) / (Vector3(0, 1, 0).dotmul(directionRay));
	float t21 = -(vertex.z + Vector3(0, 0, -1).dotmul(originRay)) / (Vector3(0, 0, -1).dotmul(directionRay));
	float t22 = -(-vertex.z - zLen + Vector3(0, 0, 1).dotmul(originRay)) / (Vector3(0, 0, 1).dotmul(directionRay));

	float t0min = t01 < t02 ? t01 : t02;
	float t0max = t01 > t02 ? t01 : t02;
	float t1min = t11 < t12 ? t11 : t12;
	float t1max = t11 > t12 ? t11 : t12;
	float t2min = t21 < t22 ? t21 : t22;
	float t2max = t21 > t22 ? t21 : t22;

	float tmp = t0min > t1min ? t0min : t1min;
	float tmin = tmp > t2min ? tmp : t2min;
	tmp = t0max < t1max ? t0max : t1max;
	float tmax = tmp < t2max ? tmp : t2max;

	if (tmin < 0 && tmax < 0)
	{
		return false;
	}

	if (tmin < tmax)
	{
		t1 = tmin;
		t2 = tmax;
		return true;
	}
	return false;
}

Vector3 Cube::nhit(const Vector3 &phit) const
{
	float distance[6];
	distance[0] = abs(phit.x - vertex.x);
	distance[1] = abs(phit.x - vertex.x - xLen);
	distance[2] = abs(phit.y - vertex.y);
	distance[3] = abs(phit.y - vertex.y - yLen);
	distance[4] = abs(phit.z - vertex.z);
	distance[5] = abs(phit.z - vertex.z - zLen);
	int minIndex = 0;
	for (int i = 1; i < 6; i++)
	{
		if (distance[i] < distance[minIndex])
		{
			minIndex = i;
		}
	}
	switch (minIndex)
	{
	case 0:
		return Vector3(-1, 0, 0);
		break;
	case 1:
		return Vector3(1, 0, 0);
		break;
	case 2:
		return Vector3(0, -1, 0);
		break;
	case 3:
		return Vector3(0, 1, 0);
		break;
	case 4:
		return Vector3(0, 0, -1);
		break;
	case 5:
		return Vector3(0, 0, 1);
		break;
	default:
		return Vector3();
		break;
	}

}

//ģ����
class Model
{
public:
	int verNum, faceNum; //�������Ƭ������
	vector<Vector3> vers; //model�е����ж���
	vector<Vector3> faces;
	vector<Vector3> faceNorms;
	Vector3 surfaceColor;

	//���캯��
	Model(char *filename,
		const Vector3 &sc,
		const Vector3 &ec,
		const float &t = 0,
		const float &r = 0);
	int intersect(const Vector3 &originRay, const Vector3 &directionRay, float &t) const;

private:
	bool intersect(int i, const Vector3 &rs, const Vector3 &rd, float &t) const;
};

Model::Model(char *filename, const Vector3 &sc, const Vector3 &ec, const float &t, const float &r)
{
	surfaceColor = sc;
	ifstream fin(filename);
	char tmpchar[100];
	int tmpnum;

	fin >> tmpchar;
	fin >> verNum >> faceNum >> tmpnum;
	//���붥������
	for (int i = 0; i < verNum; ++i) {
		Vector3 p;
		fin >> p.x >> p.y >> p.z;
		p.z = -p.z;
		p = p * 140 + Vector3(-10, -10, -40);
		vers.push_back(p);
	}
	//������Ƭ
	for (int i = 0; i < faceNum; ++i) {
		Vector3 face;
		Vector3 normal;
		fin >> tmpnum >> face.x >> face.y >> face.z;
		faces.push_back(face);

		Vector3 v1 = vers[face.y] - vers[face.x], v2 = vers[face.z] - vers[face.y];
		normal = v1.crossmul(v2);
		normal.normal();
		faceNorms.push_back(normal);
	}
	fin.close();
}

int Model::intersect(const Vector3 &originRay, const Vector3 &directionRay, float &t) const
{
	//�жϹ�����ĳһ��Ƭ�Ƿ��н���
	for (int i = 0; i < faceNum; ++i) {
		if (intersect(i, originRay, directionRay, t))
			return i;
	}

	return -1;
}

bool Model::intersect(int i, const Vector3 &rs, const Vector3 &rd, float &t) const
{
	Vector3 nm = faceNorms[i], p1 = vers[faces[i].x], p2 = vers[faces[i].y], p3 = vers[faces[i].z];
	float tmp_t;
	float dir_dot_norm = rd.dotmul(nm);
	if (dir_dot_norm >= 0) {
		return false;
	}
	tmp_t = nm.dotmul(p1 - rs) * (1 / dir_dot_norm);

	Vector3 p = rs + rd * tmp_t;
	//�ж�p�Ƿ�����������
	float area = ((p2 - p1).crossmul(p3 - p2)).length();
	float area1 = ((p2 - p1).crossmul(p - p2)).length();
	float area2 = ((p3 - p2).crossmul(p - p3)).length();
	float area3 = ((p1 - p3).crossmul(p - p1)).length();
	if (area1 + area2 + area3 > area + area1 / 1000) {
		//p������������
		return false;
	}

	t = tmp_t;
	return true;
}


#endif
