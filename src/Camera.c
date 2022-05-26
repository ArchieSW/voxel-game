#include <math.h>
#include <stdbool.h>

#include "Camera.h"


static void update_camera_vectors(Camera* camera) {
    vec3 front;
    front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    front[1] = sin(glm_rad(camera->pitch));
    front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    glm_normalize(front);
    glm_vec3_copy(front, camera->front);
    glm_vec3_cross(camera->front, camera->world_up, camera->right);
    glm_normalize(camera->right);
    glm_vec3_cross(camera->right, camera->front, camera->up);
    glm_normalize(camera->up);
}


Camera* init_camera(vec3 position, vec3 up, float yaw, float pitch) {
    Camera* camera = malloc(sizeof(Camera));
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->front);
    camera->movement_speed = SPEED;
    camera->mouse_sensitivity = SENSITIVITY;
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy(up, camera->world_up);
    camera->yaw = yaw;
    camera->pitch = pitch;
    update_camera_vectors(camera);
    return camera;
}


void delete_camera(Camera* camera) {
    free(camera);
}


void get_view_matrix(Camera* camera, mat4 dest) {
    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(camera->position, center, camera->up, dest);
}


void process_keyboard(Camera* camera, CameraMovement cm, float delta_time) {
    float velocity = camera->movement_speed * delta_time;
    if (cm == CM_FORWARD) {
        vec3 tmp;
        glm_vec3_scale(camera->front, velocity, tmp);
        glm_vec3_add(camera->position, tmp, camera->position); 
    } else if (cm == CM_BACKWARD) {
        vec3 tmp;
        glm_vec3_scale(camera->front, -velocity, tmp);
        glm_vec3_add(camera->position, tmp, camera->position);
    } else if (cm == CM_LEFT) {
        vec3 tmp;
        glm_vec3_scale(camera->right, -velocity, tmp);
        glm_vec3_add(camera->position, tmp, camera->position);
    } else if (cm == CM_RIGHT) {
        vec3 tmp;
        glm_vec3_scale(camera->right, velocity, tmp);
        glm_vec3_add(camera->position, tmp, camera->position);
    }
}


void process_mouse_movement(Camera* camera, float x_offset, float y_offset, bool constrain_pitch) {
    x_offset *= camera->mouse_sensitivity;
    y_offset *= camera->mouse_sensitivity;

    camera->yaw += x_offset;
    camera->pitch += y_offset;

    if (constrain_pitch) {
        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    }

    update_camera_vectors(camera);
}