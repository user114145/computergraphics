#include <iostream>
#include <math.h>
#include "cylinder.h"

Cylinder::Cylinder(Point position, Triple rotation, double radius, double height)
	: r(radius), h(height)
{
	int i, j;
	double **Rx, **Ry, **Rz;
	Rx = new double*[4];
	Ry = new double*[4];
	Rz = new double*[4];

	for (i=0; i<4; ++i) {
		Rx[i] = new double[4];
		Ry[i] = new double[4];
		Rz[i] = new double[4];
		for (j=0; j<4; ++j) {
			T[i][j]  = i == j;
			Rx[i][j] = i == j;
			Ry[i][j] = i == j;
			Rz[i][j] = i == j;
		}
	}

	// Translation
	T[0][3] = position.x;
	T[1][3] = position.y;
	T[2][3] = position.z;

	// Rotation
	Rx[1][1] = cos(rotation.x);
	Rx[1][2] = -sin(rotation.x);
	Rx[2][1] = sin(rotation.x);
	Rx[3][2] = cos(rotation.x);
	Ry[0][0] = cos(rotation.y);
	Ry[0][2] = sin(rotation.y);
	Ry[2][0] = -sin(rotation.y);
	Ry[2][2] = cos(rotation.y);
	Rz[0][0] = cos(rotation.z);
	Rz[0][1] = -sin(rotation.z);
	Rz[1][0] = sin(rotation.z);
	Rz[1][1] = cos(rotation.z);

	addTransform(Rx);
	addTransform(Ry);
	addTransform(Rz);
	inverseTransform();

	delete Rx;
	delete Ry;
	delete Rz;
}

Hit Cylinder::intersect(const Ray &ray)
{
	double a, b, c, d, t1, t2, t;
	Vector N;
	Point P;

	// Detect intersection with top or bottom plane
	t =

	// Detect intersection with sides
	a = ray.D.x*ray.D.x + ray.D.y*ray.D.y;
	b = 2 * (ray.O.x*ray.D.x - pos.x*ray.D.x + ray.O.y*ray.D.y - pos.y*ray.D.y);
	c = ray.O.x*ray.O.x + pos.x*pos.x + ray.O.y*ray.O.y + pos.y*pos.y -
		2 * (ray.O.x*pos.x + ray.O.y*pos.y) - r*r;
	d = b*b - 4*a*c;
	//cout << d << " " << a << " " << b << " " << c << endl;
	if (d < 0)
		return intersectTopBottom(ray);
	t1 = (-b - sqrt(d)) / (2*a);
	t2 = (-b + sqrt(d)) / (2*a);
	t = t1 < t2 && t1 > 0 ? t1 : t2;
	if (t < 0)
		return intersectTopBottom(ray);
	P = ray.at(t);
	if (P.z < -.5*h  || P.z > .5*h)
		return intersectTopBottom(ray);

	N.x = P.x - pos.x;
	N.y = P.y - pos.y;
	N.z = 0;
	N.normalize();

	return Hit(t, N);
}

Hit Cylinder::intersectTopBottom(const Ray &ray)
{
	double t1, t2;
	Vector N;
	Point P1, P2;


	t1 = ( .5*h - pos.z) / ray.D.z;
	t2 = (-.5*h - pos.z) / ray.D.z;

	return Hit::NO_HIT();
}

void Cylinder::addTransform(double **M)
{
	double T2[4][4];
	int i, j;
	for (i=0; i<4; ++i) {
		for (j=0; j<4; ++j) {
			T2[i][j] = M[i][0]*T[0][j] + M[i][1]*T[1][j] + M[i][2]*T[2][j] + M[i][3]*T[3][j];
		}
	}
	for (i=0; i<4; ++i) {
		for (j=0; j<4; ++j) {
			T[i][j] = T2[i][j];
		}
	}
}

void Cylinder::inverseTransform()
{
	/* code has been taken from http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm */
	Tinv[0][0] = T[1][2]*T[2][3]*T[3][1] - T[1][3]*T[2][2]*T[3][1] + T[1][3]*T[2][1]*T[3][2]
	           - T[1][1]*T[2][3]*T[3][2] - T[1][2]*T[2][1]*T[3][3] + T[1][1]*T[2][2]*T[3][3];
	Tinv[0][1] = T[0][3]*T[2][2]*T[3][1] - T[0][2]*T[2][3]*T[3][1] - T[0][3]*T[2][1]*T[3][2]
		       - T[0][1]*T[2][3]*T[3][2] + T[0][2]*T[2][1]*T[3][3] - T[0][1]*T[2][2]*T[3][3];
	Tinv[0][2] = T[0][2]*T[1][3]*T[3][1] - T[0][3]*T[1][2]*T[3][1] + T[0][3]*T[1][1]*T[3][2]
		       - T[0][1]*T[1][3]*T[3][2] - T[0][2]*T[1][1]*T[3][3] + T[0][1]*T[1][2]*T[3][3];
	Tinv[0][3] = T[0][3]*T[1][2]*T[2][1] - T[0][2]*T[1][3]*T[2][1] - T[0][3]*T[1][1]*T[2][2]
		       + T[0][1]*T[1][3]*T[2][2] + T[0][2]*T[1][1]*T[2][3] - T[0][1]*T[1][2]*T[2][3];
	Tinv[1][0] = T[1][3]*T[2][2]*T[3][0] - T[1][2]*T[2][3]*T[3][0] - T[1][3]*T[2][0]*T[3][2]
		       + T[1][0]*T[2][3]*T[3][2] + T[1][2]*T[2][0]*T[3][3] - T[1][0]*T[2][2]*T[3][3];
	Tinv[1][1] = T[0][2]*T[2][3]*T[3][0] - T[0][3]*T[2][2]*T[3][0] + T[0][3]*T[2][0]*T[3][2]
		       - T[0][0]*T[2][3]*T[3][2] - T[0][2]*T[2][0]*T[3][3] + T[0][0]*T[2][2]*T[3][3];
	Tinv[1][2] = T[0][3]*T[1][2]*T[3][0] - T[0][2]*T[1][3]*T[3][0] - T[0][3]*T[1][0]*T[3][2]
		       + T[0][0]*T[1][3]*T[3][2] + T[0][2]*T[1][0]*T[3][3] - T[0][0]*T[1][2]*T[3][3];
	Tinv[1][3] = T[0][2]*T[1][3]*T[2][0] - T[0][3]*T[1][2]*T[2][0] + T[0][3]*T[1][0]*T[2][2]
		       - T[0][0]*T[1][3]*T[2][2] - T[0][2]*T[1][0]*T[2][3] + T[0][0]*T[1][2]*T[2][3];
	Tinv[2][0] = T[1][1]*T[2][3]*T[3][0] - T[1][3]*T[2][1]*T[3][0] + T[1][3]*T[2][0]*T[3][1]
		       - T[1][0]*T[2][3]*T[3][1] - T[1][1]*T[2][0]*T[3][3] + T[1][0]*T[2][1]*T[3][3];
	Tinv[2][1] = T[0][3]*T[2][1]*T[3][0] - T[0][1]*T[2][3]*T[3][0] - T[0][3]*T[2][0]*T[3][1]
		       + T[0][0]*T[2][3]*T[3][1] + T[0][1]*T[2][0]*T[3][3] - T[0][0]*T[2][1]*T[3][3];
	Tinv[2][2] = T[0][1]*T[1][3]*T[3][0] - T[0][3]*T[1][1]*T[3][0] + T[0][3]*T[1][0]*T[3][1]
		       - T[0][0]*T[1][3]*T[3][1] - T[0][1]*T[1][0]*T[3][3] + T[0][0]*T[1][1]*T[3][3];
	Tinv[2][3] = T[0][3]*T[1][1]*T[2][0] - T[0][1]*T[1][3]*T[2][0] - T[0][3]*T[1][0]*T[2][1]
		       + T[0][0]*T[1][3]*T[2][1] + T[0][1]*T[1][0]*T[2][3] - T[0][0]*T[1][1]*T[2][3];
	Tinv[3][0] = T[1][2]*T[2][1]*T[3][0] - T[1][1]*T[2][2]*T[3][0] - T[1][2]*T[2][0]*T[3][1]
		       + T[1][0]*T[2][2]*T[3][1] + T[1][1]*T[2][0]*T[3][2] - T[1][0]*T[2][1]*T[3][2];
	Tinv[3][1] = T[0][1]*T[2][2]*T[3][0] - T[0][2]*T[2][1]*T[3][0] + T[0][2]*T[2][0]*T[3][1]
		       - T[0][0]*T[2][2]*T[3][1] - T[0][1]*T[2][0]*T[3][2] + T[0][0]*T[2][1]*T[3][2];
	Tinv[3][2] = T[0][2]*T[1][1]*T[3][0] - T[0][1]*T[1][2]*T[3][0] - T[0][2]*T[1][0]*T[3][1]
		       + T[0][0]*T[1][2]*T[3][1] + T[0][1]*T[1][0]*T[3][2] - T[0][0]*T[1][1]*T[3][2];
	Tinv[3][3] = T[0][1]*T[1][2]*T[2][0] - T[0][2]*T[1][1]*T[2][0] + T[0][2]*T[1][0]*T[2][1]
		       - T[0][0]*T[1][2]*T[2][1] - T[0][1]*T[1][0]*T[2][2] + T[0][0]*T[1][1]*T[2][2];
}

Triple Cylinder::applyTransform(double M[4][4], Triple V)
{
	Triple r;
	r.x = T[0][0]*V.x + T[0][1]*V.y + T[0][2]*V.z + T[0][3];
	r.y = T[1][0]*V.x + T[1][1]*V.y + T[1][2]*V.z + T[1][3];
	r.z = T[2][0]*V.x + T[2][1]*V.y + T[2][2]*V.z + T[2][3];
	return r;
}
