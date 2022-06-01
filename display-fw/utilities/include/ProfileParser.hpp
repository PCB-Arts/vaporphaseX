/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2021 - PCB Arts GmbH
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#pragma once

#include <string>
#include <vector>

const std::string SOLDERPROFILE_FILEIDENTIFIER = "### VPO;";

class KeyValuePair {
public:
	std::string Key;
	std::string Value;

	KeyValuePair(){}

	KeyValuePair(std::string key, std::string value): Key(key), Value(value){}
};

class SolderBoilingTemperature{
public:
//	std::string medium;
	int boilingTemperature;
};

class SolderACT{
public:
	int ACT;
};

class SolderTemperatureDatapoint{
public:
	int time;
	int temperature;
};

class SolderProfile {
public:
	bool valid = false;

	std::vector<KeyValuePair>  headerAttributes;
	std::vector<SolderTemperatureDatapoint> timeTemperatureData;
	std::vector<SolderBoilingTemperature> boilingtemperature;
	std::vector<SolderACT> actemperature;
};


std::vector<std::string> FindCSVFiles();
int GetFilesize(std::string& filepath);
bool IsSolderprofileCSV(std::string& filepath);
SolderProfile parseSolderCsv(std::string& file);

SolderProfile loadSolderCsv(std::string& filepath);
