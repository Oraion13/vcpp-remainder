#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

#include "ValidIOHandlers.h"
#include "RemainderManagement.h"
#include "json/json.h"

using namespace std;

class Remainder
{
public:
	Remainder() {
		feedRecurrence();
		validIOHandlers = new ValidIOHandlers();
		remainderManagement = new RemainderManagement();
	}

	void createEvent();
	void readEvent();
	void updateEvent();
	void deleteEvent();

private:
	ValidIOHandlers* validIOHandlers;
	unordered_map<int, string>* recurrence;
	RemainderManagement* remainderManagement;

	void feedRecurrence();
	void displayRecurrence();
	string getRecurrence(int key);

};

