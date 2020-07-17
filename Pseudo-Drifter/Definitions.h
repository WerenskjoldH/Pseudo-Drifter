#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/*
	Description: This file contains most all global definitions, external variables, etc.
*/

/// Defines
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400
#define WINDOW_PIXELS WINDOW_HEIGHT * WINDOW_WIDTH

#define DELTA_TIME 1/30.f
#define DEFAULT_R 140
#define DEFAULT_G 220
#define DEFAULT_B 220

#define CAMERA_HEIGHT 100
#define CAMERA_FOV 90

#define MAX_SPEED_VERTICAL 1.f
#define MAX_SPEED_HORIZONTAL 0.2f

#define SEGMENTS 50
#define SEGMENT_WIDTH 20
#define PERCENT_RUNNER 0.10f

#define ROAD_WIDTH_DEFAULT 200.f

#define DEFAULT_DISTANCE_CULLING 750.f

/// Global Variables
class InputManager;
extern InputManager* G_INPUT;

#endif