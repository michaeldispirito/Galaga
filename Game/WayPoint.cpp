#include "Precompiled.h"
#include "WayPoint.h"

WayPoint::WayPoint(const float xCoord, const float yCoord):
	x(xCoord),
	y(yCoord)
{
}

void WayPoint::addNext(WayPoint* nextWP, const float depthPercent, const WayPoint::CurveDirection curveDirection)
{
	distanceToNext = sqrt((nextWP->x - x) * (nextWP->x - x) + (nextWP->y - y) * (nextWP->y - y));
	nextWP->totalDistance = totalDistance + distanceToNext;
	
	SetControlPoint(depthPercent, curveDirection, nextWP);
}

void WayPoint::SetControlPoint(const float depthPercent, const WayPoint::CurveDirection curveDirection, const WayPoint* const nextWP)
{
	int directionMultiplier = curveDirection == CurveDirection::None ? 0 : curveDirection == CurveDirection::Negative ? -1 : 1;
	float midpointX = (x + nextWP->x) / 2;
	float midpointY = (y + nextWP->y) / 2;

	if (curveDirection == CurveDirection::None) {
		controlPointX = midpointX;
		controlPointY = midpointY;
		return;
	}

	if (nextWP->y - y == 0) {
		controlPointX = midpointX;
		controlPointY = y + (directionMultiplier * 100 * depthPercent);
		return;
	}
	else if (nextWP->x - x == 0) {
		controlPointX = x + (directionMultiplier * 100 * depthPercent); 
		controlPointY = midpointY;
		return;
	}

	float negReciprocalSlope = -((nextWP->x - x) / (nextWP->y - y));
	float negReciprocalB = midpointY - (negReciprocalSlope * midpointX);
	float endPointX = NULL;
	if (curveDirection == CurveDirection::Negative) {
		endPointX = y < nextWP->y ? nextWP->x : x;
	}
	else if (curveDirection == CurveDirection::Positive) {
		endPointX = y < nextWP->y ? x : nextWP->x;
	}

	float xRange = fabsf(midpointX - endPointX);
	int directionToEndPoint = midpointX < endPointX ? 1 : -1;
	controlPointX = midpointX + (directionToEndPoint * xRange * depthPercent);
	controlPointY = negReciprocalSlope * controlPointX + negReciprocalB;
}
