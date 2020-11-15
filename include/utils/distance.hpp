#pragma once
#include <cmath>
struct utils
{
        static double distance(int x1, int y1, int x2, int y2)
        {
                //Return the distance between the two points
                return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
        }
};