#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/*
	Description: This file contains most all global definitions, external variables, etc.
*/

/// Defines
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400
#define WINDOW_PIXELS WINDOW_HEIGHT * WINDOW_WIDTH

#define TARGET_UPDATE_RATE 30.f
#define DELTA_TIME (1.0f/TARGET_UPDATE_RATE)
#define MAX_DELTA_TIME 4.0f * DELTA_TIME

#define DEFAULT_R 140
#define DEFAULT_G 220
#define DEFAULT_B 220

#define CAMERA_HEIGHT 100
#define CAMERA_FOV 90

#define MAX_SPEED_VERTICAL 40.f
#define VEHICLE_ENGINE_FORCE 5.0f
#define VEHICLE_DRAG_FORCE 0.0f
#define VEHICLE_ROLLING_RESISTANCE 0.0f

#define SEGMENTS 50
#define SEGMENT_WIDTH 20
#define PERCENT_RUNNER 0.10f

#define ROAD_WIDTH_DEFAULT 200.f

#define DEFAULT_DISTANCE_CULLING 750.f

/// Global Variables
class InputManager;
extern InputManager* G_INPUT;

#endif