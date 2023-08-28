#include <Raytracer/Raytracer.h>
#include <HLSL.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace Raytracer;
using namespace Raytracer::Scenes;
using namespace Raytracer::Objects;

Sphere::Sphere(const float3 &center, float radius)
{
	this->center = center;
	this->radius = radius;
	this->radius2 = radius * radius;
	this->material = NULL;
}

Sphere::Sphere(const float3 &center, float radius, Material *material)
{
	this->center = center;
	this->radius = radius;
	this->radius2 = radius * radius;
	this->material = material;
}

void Sphere::GetExtent(float3 &min, float3 &max) const
{
	min = center - radius;
	max = center + radius;
};

void Sphere::GetIntersection(const Ray &ray, float distance, Intersection &intersection) const
{
	// TODO: Determine position, viewing direction, normals, and material at the intersection point 
	// of the given ray and the given distance.
	// I.e., insert those values into the intersection struct (3rd parameter)
	// The parameter "distance" (2nd parameter) contains the distance to the ray origin - intersection point.
	intersection.position = ray.GetOrigin() + distance * ray.GetDirection();
	intersection.viewDirection = ray.GetDirection();
	intersection.normal = normalize(intersection.position - center);
	intersection.material = material;
}

bool Sphere::HitTest(const Ray &ray, RayHit &hit) const
{
	hit.Set(&ray, 0, NULL);
	// TODO: Implement the intersection. If the ray hits the sphere set "hit = this;", 
	// set the distance to the next intersection point, and return true (see class RayHit).
	float3 direction = ray.GetDirection();
	float3 origin = ray.GetOrigin();
	float3 center_to_origin = (origin - center);

	float distance;
	float a = dot(direction, direction);
	float b = 2 * dot(direction,center_to_origin);
	float c = dot(center_to_origin, center_to_origin) - radius*radius;

	float discriminant = b*b - 4 * a * c;

	if (discriminant < 0) {
		return false;
	} else if (discriminant = 0) {
			distance = -b / (2*a);
			
	} else {
			float distance1 = (-b + sqrt(discriminant)) / (2*a);
			distance = (-b - sqrt(discriminant)) / (2*a);
			if (distance1 < distance) {
				distance = distance1;
			}
	}

	hit.Set(&ray, distance, this);
	return true;
}
