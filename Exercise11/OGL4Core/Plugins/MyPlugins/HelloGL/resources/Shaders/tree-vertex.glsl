#version 420

// uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec4 ground_pos;

layout (location = 0) out vec4 out_color;

void main ()
{
  mat4 rot_matrix = mat4(1.0);
  vec4 world_position = vec4(0.0);

  //exercise 11.1c) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // TODO:
  // Berechnen Sie die Rotations-Matrix rotm fuer das Billboarding
  // Die Variable ground_pos enthält die Position der aktuellen Bauminstanz
  // Hinweise:
  // 1. Verwenden Sie zunächst die Variable ground_pos, um die richtige
  //    world_pos zu berechnen.
  // 2. Überlegen Sie sich dann, wie Sie den Verbindungsvektor Kamera-Baum
  //    erhalten, um damit die Rotationsmatrix zu erstellen
  // TODO: Calculate the rotation matrix rotm for the billboarding.
  // The variable ground_pos contains the position of the current tree instance.
  // Hints:
  // 1. For a start, use the variable ground_pos to calculate the correct world_pos
  // 2. Think of a way to retrieve the connection vector camera-tree to finish the rotation matrix.

  rot_matrix = inverse(view_matrix);

  rot_matrix = mat4(
    rot_matrix[0][0], rot_matrix[0][1], rot_matrix[0][2], 0,
    rot_matrix[1][0], rot_matrix[1][1], rot_matrix[1][2], 0,
    rot_matrix[2][0], rot_matrix[2][1], rot_matrix[2][2], 0,
    0,                0,                0,                1
  );

  world_position = rot_matrix*position+ground_pos;

  //~exercise 6.2c) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  gl_Position = proj_matrix*view_matrix*world_position;
  out_color = color;
}
