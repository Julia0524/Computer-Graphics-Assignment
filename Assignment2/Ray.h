#ifndef _RAY_H_
#define _RAY_H_

#include "Vector3.h"
#include "Object.h"
#include <cmath>
#include <algorithm>
#include <vector>

#define PI 3.141592653589793
#define INFINITY 1e8
#define MAX_RAY_DEPTH 5
float Image[640 * 480 * 4];

using namespace std;

//计算混合颜色
float mix(const float &a, const float &b, const float &mix)
{
	return b * mix + a * (1 - mix);
}

Vector3 trace(
	const Vector3 &rayorigin,
	const Vector3 &raydirection,
	const vector<Object*> &objects,
	const Model &model,
	const int &depth
)
{
	float tnear = INFINITY;
	const Object* object = NULL;
	//计算光线与物体的交点
	for (unsigned i = 0; i < objects.size(); i++) {
		float t0 = INFINITY;
		float t1 = INFINITY;
		if (objects[i]->intersect(rayorigin, raydirection, t0, t1)) {
			if (t0 < 0) {
				t0 = t1;
			}
			if (t0 < tnear)
			{
				tnear = t0;
				object = objects[i];
			}
		}
	}

	float t = INFINITY;
	int face_id = model.intersect(rayorigin, raydirection, t);
	if (t < tnear) {
		tnear = t;
		object = NULL;
	}

	Vector3 surfColor = Vector3(0);

	if (face_id >= 0) {
		surfColor = model.surfaceColor * model.faceNorms[face_id].dotmul(Vector3(0) - raydirection);
		return surfColor;
	}

	//如果没有交点，返回背景色
	if (!object) {
		return Vector3(1);
	}
	//光线的颜色
	Vector3 surfaceColor = 0;
	//交点
	Vector3 phit = rayorigin + raydirection * tnear;

	Vector3 nhit = object->nhit(phit);
	//光线向量单位化
	nhit.normal();

	float bias = 1e-4;
	bool inside = false;
	if (raydirection.dotmul(nhit) > 0) {
		nhit = -nhit;
		inside = true;
	}
	//反射+折射
	if ((object->transparency > 0 || object->reflection > 0) && depth < MAX_RAY_DEPTH) {
		float facingratio = -raydirection.dotmul(nhit);
		float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
		//计算反射方向
		Vector3 reflect_direction = raydirection - nhit * 2 * raydirection.dotmul(nhit);
		reflect_direction.normal();
		Vector3 next_reflection = trace(phit + nhit * bias, reflect_direction, objects, model, depth + 1);
		Vector3 next_refraction = 0;
		//如果物体透明，计算折射光线
		if (object->transparency) {
			float ior = 1.1, eta = (inside) ? ior : 1 / ior;
			float cosi = -nhit.dotmul(raydirection);
			float k = 1 - eta * eta * (1 - cosi * cosi);
			Vector3 refraction_direction = raydirection * eta + nhit * (eta * cosi - sqrt(k));
			refraction_direction.normal();
			next_refraction = trace(phit - nhit * bias, refraction_direction, objects, model, depth + 1);
		}
		//计算表面颜色：反射mix折射
		surfaceColor = (next_reflection * fresneleffect + next_refraction * (1 - fresneleffect) * object->transparency) * object->surfaceColor;
	}
	//如果物体表面粗糙，没有反射和折射
	else {
		for (unsigned i = 0; i < objects.size(); i++) {
			if (objects[i]->emissionColor.x > 0) {
				Vector3 transmission = 1;
				Vector3 lightDirection = ((Sphere*)objects[i])->center - phit;
				lightDirection.normal();
				//如果光线与物体之间有遮挡，加入阴影
				for (unsigned j = 0; j < objects.size(); ++j) {
					if (i != j) {
						float t0, t1;
						if (objects[j]->intersect(phit + nhit * bias, lightDirection, t0, t1) && (t0 > 0 || t1 > 0)) {
							transmission = 0;
							break;
						}
					}
				}

				float tmp_t;
				int tmp_face_id = model.intersect(phit + nhit * bias, lightDirection, tmp_t);
				if (tmp_face_id > 0) {
					transmission = 0;
				}

				surfaceColor += object->surfaceColor * 0.3;
				surfaceColor += object->surfaceColor * transmission * max(float(0), nhit.dotmul(lightDirection)) * objects[i]->emissionColor;
			}
		}
	}

	return surfaceColor + object->emissionColor;

}

//渲染图像
void render(const vector<Object*> &objects, const Model &model)
{
	unsigned width = 640, height = 480;
	Vector3 *image = new Vector3[width * height], *pixel = image;
	float invWidth = 1 / float(width), invHeight = 1 / float(height);
	float fov = 60;
	float aspectratio = width / float(height);
	float angle = tan(PI * 0.5 * fov / 180.);

	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++, pixel++) {
			float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
			float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
			Vector3 raydir(xx, yy, -1);
			raydir.normal();
			*pixel = trace(Vector3(0), raydir, objects, model, 0);
		}
	}

	float *LinePD = Image;
	for (int i = height - 1; i >= 0; --i) {
		for (int j = 0; j < width; ++j) {
			LinePD[0] = min(float(1), image[j + i * width].x);
			LinePD[1] = min(float(1), image[j + i * width].y);
			LinePD[2] = min(float(1), image[j + i * width].z);
			LinePD[3] = 0.02;
			LinePD += 4;
		}
	}
	delete[] image;

}

#endif
