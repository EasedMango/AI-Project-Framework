﻿#pragma once
#include <glm/vec2.hpp>


struct Body
{
    glm::vec2 vel;
    glm::vec2 accel;
    float mass=1;
    float orientation;		// facing this direction
    float rotation;			// rotating at this speed (radians per second?)
    float angular;          // angular acceleration
    float maxSpeed=10;
    float maxAcceleration=100;
    float maxRotation;
    float maxAngular;
    float radius=1;           // for getting near walls
    float restitution=0.5;    // for bouncing off walls
};