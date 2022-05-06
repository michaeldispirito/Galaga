#include "Precompiled.h"
#include "PathManager.h"

PathManager::PathManager()
{

	std::ifstream pathFile("./PathData.json");
	try {
		jsonPathData = nlohmann::json::parse(pathFile);
	}
	catch (nlohmann::json::parse_error& e) {
		std::cout << std::endl << e.what();
	}

	entrancePaths.reserve(4);
	entrancePaths.emplace_back(PathNames::TopHookLeft, jsonPathData["TopHookLeft"]);
	entrancePaths.emplace_back(PathNames::TopHookRight, jsonPathData["TopHookRight"]);
	entrancePaths.emplace_back(PathNames::LeftSideLoop, jsonPathData["LeftSideLoop"]);
	entrancePaths.emplace_back(PathNames::RightSideLoop, jsonPathData["RightSideLoop"]);

	attackPaths.reserve(12);
	attackPaths.emplace_back(PathNames::Ushape_FarLeft, jsonPathData["Ushape_FarLeft"]);
	attackPaths.emplace_back(PathNames::Ushape_Left, jsonPathData["Ushape_Left"]);
	attackPaths.emplace_back(PathNames::Ushape_Middle, jsonPathData["Ushape_Middle"]);
	attackPaths.emplace_back(PathNames::Ushape_Right, jsonPathData["Ushape_Right"]);
	attackPaths.emplace_back(PathNames::Circle_Left, jsonPathData["Circle_Left"]);
	attackPaths.emplace_back(PathNames::Circle_Right, jsonPathData["Circle_Right"]);
	attackPaths.emplace_back(PathNames::Ushape_FarLeft, jsonPathData["Ushape_FarLeft"]);
	attackPaths.emplace_back(PathNames::Ushape_Left, jsonPathData["Ushape_Left"]);
	attackPaths.emplace_back(PathNames::Ushape_Middle, jsonPathData["Ushape_Middle"]);
	attackPaths.emplace_back(PathNames::Ushape_Right, jsonPathData["Ushape_Right"]);
	attackPaths.emplace_back(PathNames::Circle_Left, jsonPathData["Circle_Left"]);
	attackPaths.emplace_back(PathNames::Circle_Right, jsonPathData["Circle_Right"]);
	
}

 std::vector<Path>& PathManager::GetEntrancePaths()
{
	return entrancePaths;
}

 std::vector<Path>& PathManager::GetAttackPaths()
 {
	 return attackPaths;
 }
