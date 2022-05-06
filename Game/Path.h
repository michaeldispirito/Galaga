#pragma once
#include "PathNames.h"
#include "WayPoint.h"
#include <vector>
#include <nlohmann/json.hpp>

class Path
{
public:
	Path(const PathNames name, nlohmann::json jsonPathData);
	
	WayPoint* GetWayPoint(const int index);
	int GetLength();

	bool InUse();
	void SetInUse();
	void SetAvailable();
	
	
private:
	int length;
	PathNames name;
	std::vector<WayPoint> wayPointVector;
	bool inUse = false;

};

