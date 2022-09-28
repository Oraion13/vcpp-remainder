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
	static void setNotifier(Json::Value latestRemainder);
	static void notifier();
	
private:
	static Json::Value REMAINDER;
	static DWORD TIMER;

	static DWORD calculateSleepTImer();
	static void setSleepTimer(DWORD timerMilli);

	static int getYear(string date);
	static int getMonth(string date);
	static int getDay(string date);
	static int getHour(string time);
	static int getMinute(string time);

	static LPCWSTR getSummary();
	static LPCWSTR getDescription();
};

