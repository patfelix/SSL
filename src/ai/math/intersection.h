#pragma once

#include "vector2d.h"
#include <rhoban_geometry/segment.h>

bool segment_intersection(const rhoban_geometry::Segment& segment1, const rhoban_geometry::Segment& segment2,
                          rhoban_geometry::Point& intersection);
