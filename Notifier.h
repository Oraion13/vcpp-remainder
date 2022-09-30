#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include <chrono>

#include "json/json.h"
#include "RemainderManagement.h"

using namespace std;

class Notifier
{
public:
	static bool FLAG;

	static void setNotifier(Json::Value latestRemainder);
	static void notifier();
	static void fileUpdateNotifier();
	
private:
	static Json::Value REMAINDER;
	static DWORD TIMER;

	static 	HANDLE ghMutex;

	static DWORD calculateSleepTImer();
	static void setSleepTimer(DWORD timerMilli);
	static bool isAPastReminder();

	static int getYear(string date);
	static int getMonth(string date);
	static int getDay(string date);
	static int getHour(string time);
	static int getMinute(string time);
	static int getSecond(string time);

	static LPCWSTR getSummary();
	static LPCWSTR getDescription();

	static ofstream getAFileToWrite();
	static void writeLog(ofstream file, string txt);

	static string jsonToString(Json::Value json);
};

