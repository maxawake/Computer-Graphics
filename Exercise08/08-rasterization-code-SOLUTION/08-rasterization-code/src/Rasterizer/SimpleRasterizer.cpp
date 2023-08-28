#include <algorithm>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/matrix_inverse.hpp>

#define RAYTRACER_USE_FOREACH
#include <Raytracer/Raytracer.h>

#include <Rasterizer/SimpleRasterizer.h>

using namespace std;
using namespace glm;
using namespace Rasterizer;
using namespace Raytracer;
using namespace Raytracer::Objects;
using namespace Raytracer::Scenes;

SimpleRasterizer::SimpleRasterizer()
{
  ambientLight = vec3(0.01f);
}

bool SimpleRasterizer::CompareTriangle(const Triangle &t1, const Triangle &t2)
{
  // These aren't actually the mean values, but since both are off by a constant factor (3),
  // this inequation is equivalent.
  return (t1.position[0].z + t1.position[1].z + t1.position[2].z >
    t2.position[0].z + t2.position[1].z + t2.position[2].z);
}

void SimpleRasterizer::DrawSpan(int x1, int x2, int y, float z1, float z2, vec3 &color1,
                vec3 &color2)
{
  // TODO later exercise sheet
}

void SimpleRasterizer::DrawTriangle(const Triangle &t)
{
  for (int i = 0; i < 3; ++i)
  {
    int x = (int)t.position[i].x;
    int y = (int)t.position[i].y;
    if ((x > 0) && (x < image->GetWidth()) && (y > 0) && (y < image->GetHeight()))
    {
      image->SetPixel(x, y, t.color[i]);
    }
  }
  // TODO later exercise sheet
}

vec3 SimpleRasterizer::LightVertex(vec4 position, vec3 normal, vec3 color)
{
  vec3 result = color * ambientLight;

  foreach (const Light *, light, lights)
  {
    vec3 intensity = vec3(1.0f);
    if ((*light)->IsInstanceOf(SceneObjectType_PointLight))
      intensity = ((PointLight *)*light)->GetIntensity();

    vec3 distance = (*light)->GetPosition() - vec3(position);
    float attenuation = 1.0f / (0.001f + dot(distance, distance));
    vec3 direction = normalize(distance);

    float lambert = glm::max(0.0f, dot(normal, direction));

    if (lambert > 0)
      result += color * lambert * attenuation * intensity;
  }

  return result;
}


void SimpleRasterizer::SortTriangles(vector<Triangle> &triangles)
{
  sort(triangles.begin(), triangles.end(), CompareTriangle);
}


void SimpleRasterizer::TransformAndLightTriangle(Triangle &t, 
                                                 const mat4 &modelTransform,
                                                 const mat4 &modelTransformNormals)
{ 
  // Exercise 7.2 c)
  
  for (int i=0; i<3; i++) {
    // Apply model transform to go from model coordiantes to world coordinates
    vec4 worldCoords =  modelTransform * vec4(t.position[i], 1.0f);
    vec3 worldNormal = normalize(vec3(modelTransformNormals * vec4(t.normal[i], 0.0f)));

    // Light vertex in world coordinates
    t.color[i] = LightVertex(worldCoords, worldNormal, t.color[i]);

    // Get clip coordinates by ViewProjectionTransformation
    clipCoords = this->viewProjectionTransform * worldCoords;

    // Get normalized device coordinates (i.e. x,y in [-1,1])
    clipCoords.x /= clipCoords.w;
    clipCoords.y /= clipCoords.w;
    clipCoords.z /= clipCoords.w;
    
    // Apply viewport transform to get windows coordinates and set new positions
    t.position[i].x = (clipCoords.x +1.0)*(image->GetWidth()/2.0);
    t.position[i].y = (-1.0f*clipCoords.y +1.0)*(image->GetHeight()/2.0);
  }
}


void SimpleRasterizer::RenderMesh(const Mesh *mesh)
{
  if (mesh == NULL)
    return;

  // Exercise 8.1 a)

  const mat4 modelTransform = mesh->GetGlobalTransformation();
  const mat4 modelTransformNormals = inverseTranspose(modelTransform);

  for (Triangle t: mesh->GetTriangles()) {
    this->TransformAndLightTriangle(t, modelTransform, modelTransformNormals);
    this->DrawTriangle(t);
  }
}

void SimpleRasterizer::ScanObject(const Raytracer::Scenes::SceneObject *object)
{
  if (object == NULL)
    return;

  if (object->IsInstanceOf(SceneObjectType_Light))
    lights.push_back((const Light *)object);
  if (object->IsInstanceOf(SceneObjectType_Mesh))
    meshes.push_back((const Mesh *)object);

  foreach_c(SceneObject *, child, object->GetChildren())
    ScanObject(*child);
}

bool SimpleRasterizer::Render(Image &image, const Scene &scene)
{
  image.Clear(vec3(0));

  Camera *camera = scene.GetActiveCamera();
  if (camera == NULL)
    return false;

  zBuffer = new float[image.GetWidth() * image.GetHeight()];
  for (int i = 0; i < image.GetWidth() * image.GetHeight(); i++)
    zBuffer[i] = 1.0f;

  // Build lists of all lights and meshes in the scene.
  lights.clear();
  meshes.clear();
  ScanObject(&scene);

  //Exercise 8.1 b)

  const mat4 viewTransformation = lookAt(
    camera->GetEye(),    // The position of your camera, in world space
    camera->GetLookAt(), // where you want to look at, in world space
    camera->GetUp()      // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
  );

  // Generates a really hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless
  const mat4 projectionMatrix = perspective(
    camera->GetFov(),      // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
    camera->GetAspect(),   // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
    camera->GetNearClip(), // Near clipping plane. Keep as big as possible, or you'll get precision issues.
    camera->GetFarClip()   // Far clipping plane. Keep as little as possible.
  );

  this->viewProjectionTransform = projectionMatrix * viewTransformation;
  
  // Render all meshes we found.
  this->image = &image;
  foreach(const Mesh *, mesh, meshes)
    RenderMesh(*mesh);

  delete[] zBuffer;

  return true;
}
