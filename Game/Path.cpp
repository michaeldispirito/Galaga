#include "Precompiled.h"
#include "Path.h"

Path::Path(const PathNames name, nlohmann::json jsonPathData):
	name(name),
	length(jsonPathData["Length"])
{
	wayPointVector.reserve(jsonPathData["Length"]);

	for (int i = 0; i < length; i++) {
		nlohmann::json curWayPointData = jsonPathData["Points"][i];
		float x = curWayPointData["X"];
		float y = curWayPointData["Y"];
		wayPointVector.emplace_back(x, y);

		if (i > 0) {
			float curveDepth = curWayPointData["CurveDepth"];
			WayPoint::CurveDirection direction = static_cast<WayPoint::CurveDirection>(curWayPointData["CurveDirection"]);
			wayPointVector[i - 1].addNext(&wayPointVector[i], curveDepth, direction);
		}
	}
}

WayPoint* Path::GetWayPoint(const int index)
{
	return &(wayPointVector.at(index));
}


int Path::GetLength()
{
	return length;
}

bool Path::InUse()
{
	return inUse;
}

void Path::SetInUse()
{
	inUse = true;
}

void Path::SetAvailable()
{
	inUse = false;
}

