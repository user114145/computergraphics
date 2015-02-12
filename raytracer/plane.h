#ifndef PLANE_H
#define PLANE_H

#include "object.h"

class Plane : public Object
{
public:
    Plane(Point pos, Vector n): position(pos), normal(n) { }

    virtual Hit intersect(const Ray &ray);

    const Point position;
    const Vector normal;
};

#endif /* PLANE_H */
