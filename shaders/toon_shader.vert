#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out vec3 world_normal;
out vec3 world_pos;

void main()
{
  world_normal = normalize(mat3(transpose(inverse(model))) * in_normal);
  world_pos = vec3(model * vec4(in_position, 1.0));
  gl_Position = projection * view * model * vec4(in_position, 1.0);
}
