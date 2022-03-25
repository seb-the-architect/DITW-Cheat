#pragma once
#include <vector>

struct GameStatistics
{
	double* experience;
	double* health;
	double* sanity;
	double* stamina;
	float* posx;
	float* posy;
	int* strength;
	int* agility;
	int* endurance;
	int* willpower;
	//This is used in conjunction with the ListBox in the menu. The indexes of the names match up with the posx values
	std::vector<float> posxList;
	std::vector<float> posyList;
};

struct MenuStatistics
{
	int experience;
	int strength;
	int agility;
	int endurance;
	int willpower;
	bool bNeverTired = false;
	bool bNeverInsane = false;
	bool bNeverBleed = false;
};

extern GameStatistics gameStats;
extern MenuStatistics menuStats;

void maxAllStats();
//bForced forces gameStats to be updated - may block control flow
bool initGameStatistics(bool bForced);