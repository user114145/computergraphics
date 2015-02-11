//
//  Framework for a raytracer
//  File: scene.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html
//

#include "scene.h"
#include "material.h"

Color Scene::trace(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

    switch (mode) {
    case PHONG:
        return renderPhong(material, hit, N, V);
    case ZBUFFER:
        return renderZBuffer(hit, N);
    case NORMAL:
        return renderNormal(hit, N);
    }
    return Color(0.0, 0.0, 0.0);
}

Color Scene::renderPhong(Material *m, Point hit, Vector N, Vector V)
{
    Color color;
    Vector L, R;
    for (std::vector<Light*>::iterator it=lights.begin(); it!=lights.end(); ++it) {
        // Ambient
        color += (*it)->color * m->color * m->ka;
        // Diffusion
        L = (*it)->position - hit;
        L.normalize();
        if (N.dot(L) < 0.0)
            continue;
        color += N.dot(L) * (*it)->color * m->color * m->kd;
        // Specular
        R = 2 * N.dot(L) * N - L;
        if (R.dot(V) < 0) continue;
        R.normalize();
        color += pow(V.dot(R), m->n) * (*it)->color * m->ks;
    }

    return color;
}

Color Scene::renderZBuffer(Point hit, Vector N)
{
    return Color(0.0, 0.0, 0.0);
}

Color Scene::renderNormal(Point hit, Vector N)
{
    return Color(0.0, 0.0, 0.0);
}

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray);
            col.clamp();
            img(x,y) = col;
        }
    }
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setEye(Triple e)
{
    eye = e;
}

void Scene::setMode(RENDER_MODE m)
{
    mode = m;
}

