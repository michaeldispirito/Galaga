#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Path.h"
#include "PathNames.h"

class PathManager
{
public:
	PathManager();

	std::vector<Path>& GetEntrancePaths();
	std::vector<Path>& GetAttackPaths();


private:
	nlohmann::json jsonPathData;
	std::vector<Path> entrancePaths;
	std::vector<Path> attackPaths;

};

