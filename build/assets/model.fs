#version 330 core

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 color;

uniform vec3 camera_position;

in vec3 object_normal;
in vec3 fragment_position;

out vec4 FragmentColor;

void main() {
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * light_color;

    vec3 norm = normalize(object_normal);
    vec3 light_direction = normalize(light_position - fragment_position);

    float diff = max(dot(norm, light_direction), 0.0f);
    vec3 diffuse = diff*light_color;

    vec3 viewPos = camera_position;
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - fragment_position);
    vec3 reflectDir = reflect(-light_direction, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * light_color;

    vec3 result = (ambient + diffuse + specular) * color;

    FragmentColor = vec4(result, 1.0f);
}