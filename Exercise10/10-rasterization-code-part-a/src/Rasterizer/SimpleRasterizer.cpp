#include <algorithm>
#include <vector>
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
  // Some convenience variables
  double t;
  double z;
  vec3 c;
  vec3 c1 = color1;
  vec3 c2 = color2;

  // Check if from left to right or from right to left
  if (x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(z1, z2);
    c2 = color1;
    c1 = color2;
  }

  // Save first value
  int x1_start = x1;

  // Loop from left to right and draw pixels accordingly
  for (x1; x1 < x2; x1++)
  {
    // Perfrom range checks, only draw pixel if inside canvas
    if ((x1 > 0) && (x1 < image->GetWidth()) && (y > 0) && (y < image->GetHeight()))
    {
      // Calculate interpolation parameter t
      t = (x1 == x2) ? 1.0f : (double)(x1 - x1_start) / (double)(x2 - x1_start);

      // Interpolate depth perspectively correct
      z = 1.0f / glm::mix(1.0f / z1, 1.0f / z2, t);

      // Check if corresponding zBuffer value is below the current z value
      if (zBuffer[x1 * image->GetHeight() + y] > z)
      {
        // Set new zBuffer value
        zBuffer[x1 * image->GetHeight() + y] = z;

        // Interpolate color perspectively correct
        c = glm::mix(c1 / z1, c2 / z2, t) / (float)(1.0f / z);

        // Finally set pixel
        image->SetPixel(x1, y, c);
      }
    }
  }
}

std::vector<vec2> SimpleRasterizer::DrawLine(vec3 pos0, vec3 pos1)
{
  // Bresenham Line Rasterization Algorithm
  int x0 = pos0.x;
  int y0 = pos0.y;
  int x1 = pos1.x;
  int y1 = pos1.y;
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;
  std::vector<vec2> line;
  while (1)
  {
    line.push_back(vec2(x0, y0));
    if (x0 == x1 && y0 == y1)
      break;
    e2 = 2 * err;
    if (e2 > dy)
    {
      err += dy;
      x0 += sx;
    }
    if (e2 < dx)
    {
      err += dx;
      y0 += sy;
    }
  }
  return line;
}

void SimpleRasterizer::DrawTriangle(const Triangle &t)
{
  int indices[3] = {0, 1, 2}; // Set of indices so the triangle object does not need to be modified

  // Sort vertices by y-value. Could also do std::sort but for three elements this is faster.
  if (t.position[indices[0]].y > t.position[indices[1]].y)
    std::swap(indices[0], indices[1]);
  if (t.position[indices[1]].y > t.position[indices[2]].y)
    std::swap(indices[1], indices[2]);
  if (t.position[indices[0]].y > t.position[indices[1]].y)
    std::swap(indices[0], indices[1]);

  // Apply line algorithm to draw lines between the vertices
  std::vector<vec2> p01 = DrawLine(t.position[indices[0]], t.position[indices[1]]);
  std::vector<vec2> p02 = DrawLine(t.position[indices[0]], t.position[indices[2]]);
  std::vector<vec2> p12 = DrawLine(t.position[indices[1]], t.position[indices[2]]);

  // Some convenience variables
  int y = t.position[indices[0]].y;
  int y_end = t.position[indices[2]].y;

  vec3 color0 = t.color[indices[0]];
  vec3 color1 = t.color[indices[1]];
  vec3 color2 = t.color[indices[2]];

  float z0 = t.position[0].z;
  float z1 = t.position[1].z;
  float z2 = t.position[2].z;

  vec3 c1;
  vec3 c2;

  double t1;
  double t2;

  float z01;
  float z02;
  float z12;

  // To iteration variables, one for the left and one for the right line
  int index_left = 0;
  int index_right = 0;

  // Main loop goign from top to bottom
  while (true)
  {
    // Find current interpolation parameter t for left and right
    t1 = (double)index_left / (double)(p01.size() - 1);
    t2 = (double)index_right / (double)(p02.size() - 1);

    // Interpolate depth for perspective correct interpolation
    z01 = 1.0f / glm::mix(1.0f / z0, 1.0f / z1, t1);
    z02 = 1.0f / glm::mix(1.0f / z0, 1.0f / z2, t2);

    // Interpolate color perspectivly correct
    c1 = glm::mix(color0 / z0, color1 / z1, t1) / (float)(1.0f / z01);
    c2 = glm::mix(color0 / z0, color2 / z2, t2) / (float)(1.0f / z02);

    // Call the DrawSpan() function
    DrawSpan(p01[index_left].x,
             p02[index_right].x,
             y,
             z01,
             z02,
             c1,
             c2);

    // Increment current line
    y++;

    // If the middle vertex is reached, break
    if (y >= t.position[indices[1]].y)
      break;

    // Iterate both lines till the next y value is reached
    while (p01[index_left].y != y)
    {
      index_left++;
    }
    while (p02[index_right].y != y)
    {
      index_right++;
    }
  }

  // Reinitialize the left iterator, since we now use the bottom line
  index_left = 0;
  // Decrement line such that the last drawspan is actually called
  y--;

  // Second loop, from middle vertex to bottom vertex
  while (y < y_end)
  {
    // Find current interpolation parameter t for left and right
    t1 = (double)index_left / (double)(p12.size() - 1);
    t2 = (double)index_right / (double)(p02.size() - 1);

    // Interpolate depth for perspective correct interpolation
    z02 = 1.0f / glm::mix(1.0f / z0, 1.0f / z2, t2);
    z12 = 1.0f / glm::mix(1.0f / z1, 1.0f / z2, t1);

    // Interpolate color perspectivly correct
    c1 = glm::mix(color1 / z1, color2 / z2, t1) / (float)(1.0f / z12);
    c2 = glm::mix(color0 / z0, color2 / z2, t2) / (float)(1.0f / z02);

    // Call the DrawSpan() function
    DrawSpan(p12[index_left].x,
             p02[index_right].x,
             y,
             z12,
             z02,
             c1,
             c2);

    // Increment current line
    y++;

    // Iterate both lines till the next y value is reached
    while (p12[index_left].y != y)
    {
      index_left++;
    }
    while (p02[index_right].y != y)
    {
      index_right++;
    }
  }
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
  // Exercise 8.1 c)

  for (int i = 0; i < 3; i++)
  {
    // Apply model transform to go from model coordiantes to world coordinates
    vec4 worldCoords = modelTransform * vec4(t.position[i], 1.0f);
    vec3 worldNormal = normalize(vec3(modelTransformNormals * vec4(t.normal[i], 0.0f)));

    // Light vertex in world coordinates
    t.color[i] = LightVertex(worldCoords, worldNormal, t.color[i]);

    // Get clip coordinates by ViewProjectionTransformation
    vec4 clipCoords = this->viewProjectionTransform * worldCoords;

    // Get normalized device coordinates (i.e. x,y in [-1,1])
    clipCoords.x /= clipCoords.w;
    clipCoords.y /= clipCoords.w;
    clipCoords.z /= clipCoords.w;

    // Apply viewport transform to get windows coordinates and set new positions
    t.position[i].x = (clipCoords.x + 1.0) * (image->GetWidth() / 2.0);
    t.position[i].y = (-1.0f * clipCoords.y + 1.0) * (image->GetHeight() / 2.0);
    t.position[i].z = clipCoords.z;
  }
}

void SimpleRasterizer::RenderMesh(const Mesh *mesh)
{
  if (mesh == NULL)
    return;

  // Exercise 8.1 a)

  const mat4 modelTransform = mesh->GetGlobalTransformation();
  const mat4 modelTransformNormals = inverseTranspose(modelTransform);

  for (Triangle t : mesh->GetTriangles())
  {
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

  // Exercise 8.1 b)

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
  foreach (const Mesh *, mesh, meshes)
    RenderMesh(*mesh);

  delete[] zBuffer;

  return true;
}
