#include <Raytracer/Raytracer.h>

using namespace Raytracer;
using namespace Raytracer::Scenes;

PointLight::PointLight(const float3 &position, const float3 &intensity)
{
  this->position = position;
  this->intensity = intensity;
}

float3 PointLight::ComputeDirectContribution(const Intersection &intersection, const Scene &scene)
{
#ifndef SKELETON
  float3 c = float3(0.0f);

  float3 distance = position - intersection.position;
  float attenuation = 1.0f / (0.001f + dot(distance, distance));
  float3 direction = normalize(distance);

  float lambert = max(0.0f, dot(intersection.normal, direction));

  if (lambert > 0)
  {
    if (scene.GetAccelerator() != NULL)
    {
      Ray shadowRay(intersection.position + direction * 0.001f, direction);

      if (scene.GetAccelerator()->Cast(shadowRay, length(distance) - 0.001f))
        return c;
    }

    c = intersection.material->GetDiffuse() * lambert * attenuation * intensity;

    if (length(intersection.material->GetSpecular()) > 0) 
    {
      float specular = dot(intersection.viewDirection,
        Ray::Reflect(direction, intersection.normal));

      if (specular > 0) 
      {
        if (specular > 1.0f)
          specular = 1.0f;

        c += intersection.material->GetSpecular() *
          (powf(specular, intersection.material->GetShininess())) * attenuation *
          intensity;
      }
    }	
  }

  return c;
#else
   //TODO: Implementieren Sie die Berechnung der diffusen und spekularen 
  // Beleuchtung am durch intersection angegebenen Punkt nach dem Phong-Beleuchtungsmodell. 
  // Berücksichtigen Sie auch die Abschwächung der Beleuchtung mit zunehmender Entfernung 
  // von der Lichtquelle und den Schattenwurf durch andere Objekte.
  return float3(0, 0, 0);
#endif
}
