#pragma once
struct Vision
{
	float radius;
	ID target = -1;
	bool targetFound = false;
	float timeSinceLastSeen = 0;
	float maxTimeSinceLastSeen = 1.5f;
};