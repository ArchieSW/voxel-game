#pragma once
#include <cglm/cglm.h>

// default camera values
#define YAW         -90.0f
#define PITCH        0.0f
#define SPEED        15.0f
#define SENSITIVITY  0.1f
#define ZOOM         45.0f

typedef enum {
    CM_FORWARD,
    CM_BACKWARD,
    CM_LEFT,
    CM_RIGHT
} CameraMovement;

typedef struct {
    // camera attributes
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 world_up;
    // Euler's angles
    float yaw;
    float pitch;
    // camera options
    float movement_speed;
    float mouse_sensitivity;
} Camera;


Camera* init_camera(vec3 position, vec3 up, float yaw, float pitch);
void delete_camera(Camera* camera);
void process_keyboard(Camera* camera, CameraMovement cm, float delta_time);
void process_mouse_movement(Camera* camera, float x_offset, float y_offset, bool constrain_pitch);
void get_view_matrix(Camera* camera, mat4 dest);