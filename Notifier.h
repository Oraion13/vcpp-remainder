#pragma once
#include <iostream>
#include <string>
#include <Windows.h>

#include "json/json.h"
#include "RemainderManagement.h"

using namespace std;

class Notifier
{
public:
	void setNotifier(Json::Value latestRemainder);
	void notifier();
	
private:
	Json::Value REMAINDER;
	DWORD TIMER;

	DWORD calculateSleepTImer();
	void setSleepTimer(DWORD timerMilli);

	int getYear(string date);
	int getMonth(string date);
	int getDay(string date);
	int getHour(string time);
	int getMinute(string time);

	LPCWSTR getSummary();
	LPCWSTR getDescription();
};

