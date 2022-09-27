#include "Remainder.h"
#include "ValidIOHandlers.h"

void Remainder::feedRecurrence() {
	recurrence = new unordered_map<int, string>();

	recurrence->insert(make_pair(1, "HOURLY"));
	recurrence->insert(make_pair(2, "DAILY"));
	recurrence->insert(make_pair(3, "WEEKLY"));
	recurrence->insert(make_pair(4, "MONTHLY"));
	recurrence->insert(make_pair(5, "YEARLY"));
}

void Remainder::displayRecurrence() {
	//    iterating over all value of recurrence
	cout << "\nAll Elements : \n";
	for (auto& itr : *recurrence)
	{
		// itr works as a pointer to pair<int, string>
		// type itr->first stores the key part  and
		// itr->second stores the value part
		cout << itr.first << " : " << itr.second << endl;
	}
}

string Remainder::getRecurrence(int key) {
	return recurrence->at(key);
}

void Remainder::createEvent() {
	string summary{};
	string description{};
	string date{};
	string time{};
	string recur{NULL};

	summary = validIOHandlers->getString("Enter a summary: ");
	description = validIOHandlers->getString("Enter a description: ");
	date = validIOHandlers->getDate();
	time = validIOHandlers->getHourMinute();

	// display recurrence value
	if (validIOHandlers->isY("Is a recursive event [Y/n]: ")) {
		displayRecurrence();

		int key = validIOHandlers->getInt("Enter a RecurId [Number]: ");
		if (key != 0) {
		recur = getRecurrence(key);
		}
	}

	remainderManagement->addRemainder(summary, description, date, time, recur);
}