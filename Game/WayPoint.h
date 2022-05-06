#pragma once
#include <math.h>

/*
src: Basic functionality of Class taken from https://stackoverflow.com/questions/62738408/enemies-path-following-space-shooter-game

NOTES:
    Direction (is based on change in Y value):
        - Negative = UP
        - Positive = DOWN
        - None = FLAT

        Endpoints:
            - Negative Dir = Use endpoint with highest Y value for range
            - Positive Dir = Use endpoint with lowest Y value for range

    SetControlPoint depthPercent:
        - 0 < depthPercent <= 1. 
        - lower percent is less intense curve (closer to midpoint).
        - Higher percent is more intense (closer to endPoint)
*/

class WayPoint
{
public:
    enum class CurveDirection {
        None,
        Positive,
        Negative
    };

    WayPoint(const float xCoord, const float yCoord);
    void addNext(WayPoint* nextWayPoint, const float depthPercent, const WayPoint::CurveDirection curveDirection);


    float x;
    float y;
    float distanceToNext = 0.0f;
    float totalDistance = 0.0f; 

    //Bezier Curve control point
    float controlPointX = 0;
    float controlPointY = 0;

private:
    void SetControlPoint(const float depthPercent, const WayPoint::CurveDirection curveDirection, const WayPoint* const nextWP);

    
};

