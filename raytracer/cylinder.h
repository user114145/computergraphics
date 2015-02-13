#ifndef CYLINDER_H
#define CYLINDER_H

#include "object.h"

class Cylinder: public Object
{
public:
	Cylinder(Point position, Triple rotation, double radius, double height);

	virtual Hit intersect(const Ray &ray);

	const double r, h;
	double T[4][4], Tinv[4][4];
private:
	Hit intersectSide(const Ray & ray);
	Hit intersectTopBottom(const Ray &ray);
	void addTransform(double **M);
	void inverseTransform();
	Triple applyTransform(double M[4][4], Triple V);
};

#endif // CYLINDER_H
