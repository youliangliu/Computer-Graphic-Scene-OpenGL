#version 330 core

in vec3 world_normal;
in vec3 world_pos;

uniform vec3 color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

const int levels = 5;
const float scale_factor = 1.0 / levels;

out vec4 frag_color;

void main()
{
  // Light Direction, View Direction and Halfway
  vec3 light_direction = normalize(light_pos - world_pos);
  vec3 view_direction = normalize(view_pos - world_pos);
  vec3 halfway = normalize(light_direction + view_direction);
  
  // Ambient light
  vec3 ambient = ambient * light_color;
  
  // Diffuse light
  float diff = max(dot(world_normal, light_direction), 0.0);
  vec3 diffuse = light_color * diff;
  
  // Specular
  float spec = pow(max(dot(world_normal, halfway), 0.0), shininess);
  vec3 specular = light_color * spec;
  
  // Linear Attenuation
  float distance = length(light_pos - world_pos);
  float attenuation = 1.0 / (0.09 * distance);
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  
  // Combine
  vec3 result = ambient + diffuse + specular;
  
  // Edge Detection
  float edge_detection = (dot(view_direction, world_normal) > 0.5) ? 1 : 0;
  result = result * edge_detection;
  
  // Toon Shading
  float intensity = dot(light_direction, world_normal);
  if (intensity > 0.95)
    result = vec3(1.0, 1.0, 1.0) * result;
  else if (intensity > 0.5)
    result = vec3(0.7, 0.7, 0.7) * result;
  else if (intensity > 0.05)
    result = vec3(0.35, 0.35, 0.35) * result;
  else
    result = vec3(0.1, 0.1, 0.1) * result;
  
  frag_color = vec4(result, 1.0);
}
