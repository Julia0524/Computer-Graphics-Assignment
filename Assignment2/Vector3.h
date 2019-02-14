#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <cmath>

class Vector3
{
public:
	float x;
	float y;
	float z;

	//���캯��
	Vector3();
	Vector3(float a, float b, float c);
	Vector3(float a);
	Vector3(const Vector3 &v);
	
	//�����������
	void operator=(const Vector3 &v);
	Vector3 operator+(const Vector3 &v) const;
	Vector3 operator-(const Vector3 &v) const;
	Vector3 operator - () const;
	//����
	Vector3 operator*(float a) const;
	Vector3 operator*(const Vector3 &v) const;
	Vector3& operator*=(float a);
	Vector3& operator+=(const Vector3 &v);
	bool operator==(const Vector3 &v);

	//�趨��������������ֵ
	void set(float a, float b, float c);
	//�õ���������
	float length() const;
	//������λ��
	Vector3& normal();
	//���
	float dotmul(const Vector3 &v) const;
	//���
	Vector3 crossmul(const Vector3 &v) const;
};

Vector3::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
}

Vector3::Vector3(float a)
{
	x = a;
	y = a;
	z = a;
}

Vector3::Vector3(const Vector3 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

void Vector3::operator=(const Vector3 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

Vector3 Vector3::operator+(const Vector3 &v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator-(const Vector3 &v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator - () const 
{ 
	return Vector3(-x, -y, -z); 
}

Vector3 Vector3::operator*(float a) const
{
	return Vector3(x * a, y * a, z * a);
}

Vector3 Vector3::operator*(const Vector3 &v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3& Vector3::operator*=(float a)
{
	(*this) = (*this) * a;
	return (*this);
}
Vector3& Vector3::operator+=(const Vector3 &v)
{
	(*this) = (*this) + v;
	return (*this);
}

bool Vector3::operator==(const Vector3 &v)
{
	return (x == v.x) && (y == v.y) && (z == v.z);
}

void Vector3::set(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
}

float Vector3::length() const
{
	return sqrt(x*x + y*y + z*z);
}

Vector3& Vector3::normal()
{
	float len = length();
	if (len > 0) {
		x = x / len, y = y / len, z = z / len;
	}
	return *this;
}

float Vector3::dotmul(const Vector3 &v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::crossmul(const Vector3 &v) const
{
	return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

#endif

