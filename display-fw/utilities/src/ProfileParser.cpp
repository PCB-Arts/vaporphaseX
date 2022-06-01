#include "ProfileParser.hpp"

#include <algorithm>

#include <regex>
extern "C"{
#include "ff.h"
}

#include "log.h"


bool IsNumber(std::string text){
	for(char& c: text){
		if(!std::isdigit(c)){
			return false;
		}
	}
	return true;
}

std::vector<std::string> FindCSVFiles()
{
	const std::string basepath = "profiles";
	DIR dir;
	FILINFO info;

	std::vector<std::string> files;
	if( f_findfirst( &dir, &info, basepath.c_str(), "*.CSV"  ) == FR_OK )
	{
		do
	    {
			std::string filename(info.fname);
			if(filename == ""){break;}
			files.push_back(basepath + "/" + filename);
	    }while(f_findnext(&dir, &info) == FR_OK);
	}

	    return files;
}

bool IsSolderprofileCSV(std::string& filepath){
	FIL file;
	auto result = f_open(&file, filepath.c_str(), FA_READ);
	if(result != FR_OK){
		return false;
	}


	char* buffer = new char[SOLDERPROFILE_FILEIDENTIFIER.size() + 1];
	buffer[SOLDERPROFILE_FILEIDENTIFIER.size()] = 0;
	unsigned int bytesRead;
	result = f_read(&file, buffer, SOLDERPROFILE_FILEIDENTIFIER.size(), &bytesRead);
	f_close(&file);

	if(result != FR_OK || bytesRead != SOLDERPROFILE_FILEIDENTIFIER.size()){
		return false;
	}

	std::string firstLine (buffer);
	delete[] buffer;


	if(firstLine != SOLDERPROFILE_FILEIDENTIFIER){
		return false;
	}
	return true;
}

bool isProfileStart(std::string& line){
	return line == "### Profile;";
}






int GetFilesize(std::string& filepath)
{
	FIL file;
	auto result = f_open(&file, filepath.c_str(), FA_READ);
	if(result != FR_OK){
		return -1;
	}

	int filesize = 0;
	char* tempBuffer[10];
	unsigned int bytesRead = 0;

	do{
		result = f_read(&file, tempBuffer, 10, &bytesRead);

		filesize += bytesRead;
	}while(result == FR_OK && bytesRead > 0);
	f_close(&file);
	return filesize;
}



SolderProfile loadSolderCsv(std::string& filepath)
{
	int filesize = GetFilesize(filepath);
	SolderProfile dummy;
	if(filesize == -1){
		return dummy;
	}

	char* buffer = new char[filesize + 1];
	buffer[filesize] = 0;

	FIL file;
	auto result = f_open(&file, filepath.c_str(), FA_READ);
	if(result != FR_OK){
		return dummy;
	}
	unsigned int bytesRead = 0;
	result = f_read(&file, buffer, filesize, &bytesRead);
	if(result != FR_OK){
		return dummy;
	}
	result = f_close(&file);
	if(result != FR_OK){
		return dummy;
	}

	std::string filecontent(buffer);
	delete[] buffer;
	return parseSolderCsv(filecontent);
}

SolderProfile parseSolderCsv(std::string& file){
	vpo_log("Loading CSV file");
	std::regex reg("\r\n|\r|\n");
	file = std::regex_replace(file, reg, "\n");
	std::istringstream stringStream(file);


	std::string line;
	std::getline(stringStream, line, '\n');

	SolderProfile profile;
    if(line != SOLDERPROFILE_FILEIDENTIFIER){
	    return profile;
    }

    std::getline(stringStream, line, '\n');

    // Parse Header
    while(!isProfileStart(line)){
		int splitterIndex = line.find(';');
		if(splitterIndex == -1 || splitterIndex == 0 || splitterIndex == line.size() - 1)
		{
			vpo_log("Error, header line doesn't match required information %s", line.c_str());
		}
		else
		{
			auto key = line.substr(0, splitterIndex);
			auto value = line.substr(splitterIndex + 1, line.length());

			// Parse boilingtemp
			/*
			 * read boilingtemp and act of profile and push it to core, after check
			 */
			auto boilingTemperature = line.substr(splitterIndex + 1, line.length());
			auto ACT = line.substr(splitterIndex+1, line.length());

			if (IsNumber(boilingTemperature) && key == "Boiling Temperature"){
			SolderBoilingTemperature medium;
			medium.boilingTemperature = std::stoi(boilingTemperature);
			if(medium.boilingTemperature > 100 && medium.boilingTemperature < 250){
			profile.boilingtemperature.push_back(medium);
			}
			else{
				medium.boilingTemperature = 230;
				vpo_log("Error, profile line doesn't have the format temperature in integer numbers, or is not between 100°C or 250°C");
			}
			}
			if (IsNumber(ACT) && key == "ACT"){
			SolderACT actemp;
			actemp.ACT = std::stoi(ACT);
			if(actemp.ACT > 0 && actemp.ACT <150){
				profile.actemperature.push_back(actemp);
			}
			else {
				actemp.ACT = 120;
				vpo_log("Error, Anti Condensation Temperature doesn't have the format temperature in integer numbers, or is not between 0°C or 150°C");
				profile.actemperature.push_back(actemp);
						}
					}

			KeyValuePair attribute(key, value);
			profile.headerAttributes.push_back(attribute);

		}
    	std::getline(stringStream, line, '\n');
    }





    //Skip table header
    std::getline(stringStream, line, '\n');

    //Parse Profile
    while(std::getline(stringStream, line, '\n')){
    	int splitterIndex = line.find(';');
    	if(splitterIndex == -1 || splitterIndex == 0 || splitterIndex == line.size() - 1)
    	{
    		vpo_log("Error, profile line doesn't match required information: %s", line.c_str());
    	}
    	else
    	{
			auto time = line.substr(0, splitterIndex);
			auto temperature = line.substr(splitterIndex + 1, line.length());

			if(IsNumber(time) && IsNumber(temperature)){
				SolderTemperatureDatapoint datapoint;
				datapoint.temperature = std::stoi(temperature);
				datapoint.time = std::stoi(time);
				profile.timeTemperatureData.push_back(datapoint);
			}
			else{
				vpo_log("Error, profile line doesn't have the format time;temperature in integer numbers");
			}
    	}
	}
    vpo_log("Loading finished");
    profile.valid = true;
    return profile;
}

