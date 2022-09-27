#pragma once

#include <random>
#include <iostream>
#include <string>
#include "json/json.h"

using namespace std;

class RemainderManagement
{
public:
	void addRemainder(string summary, string description, string date, string time, string recur);

private:
	string FILE_NAME = "remainders.json";

	ifstream getAFileToRead();
	ofstream getAFileToWrite();

	string getUUID(size_t len);
};
