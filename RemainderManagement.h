#pragma once

#include <random>
#include <iostream>
#include <string>
#include <unordered_map>

#include "json/json.h"

using namespace std;

class RemainderManagement
{
public:
	void addRemainder(string summary, string description, string date, string time, string recur);
	Json::Value readRemainder();
	void printRemainders(Json::Value remainders);
	void printRemaindersInMap(unordered_map<int, Json::Value> remaindersMap);
	unordered_map<int, Json::Value> getRemaindersInMap(Json::Value remainders);
	void writeRemaindersFromMap(unordered_map<int, Json::Value> remaindersMap);
	Json::Value getLatestRemainder();
	void deleteOrModifyById(string id);

private:
	string FILE_NAME = "remainders.json";

	ifstream getAFileToRead();
	ofstream getAFileToWrite();

	string readAFile(ifstream file);
	void writeInAFile(ofstream file, Json::Value content);

	string getUUID(size_t len);
};
