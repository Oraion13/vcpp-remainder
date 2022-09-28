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
	while (true) {
		string summary{};
		string description{};
		string date{};
		string time{};
		string recur{ "ONCE" };

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

		// get the latest remainder and set notifier
		Notifier::setNotifier(remainderManagement->getLatestRemainder());

		if (!validIOHandlers->isY("Continue adding remainders [Y/n]?: ")) {
			break;
		}
	}
}

void Remainder::readEvent() {
	remainderManagement->printRemainders(remainderManagement->readRemainder());
}

void Remainder::deleteEvent() {
	while (true) {
		cout << "Enter '0' to exit..." << endl;

		unordered_map<int, Json::Value> remaindersMap = remainderManagement->getRemaindersInMap(
			remainderManagement->readRemainder()
		);
		if (remaindersMap.size() == 0) {
			cout << "No remainders found!" << endl;
			break;
		}

		remainderManagement->printRemaindersInMap(remaindersMap);

		int choice = validIOHandlers->getInt("Enter a Remainder Id to delete [Number / 0]: ");

		if (choice == 0) {
			break;
		}

		remaindersMap.erase(choice);
		remainderManagement->writeRemaindersFromMap(remaindersMap);
		cout << "Remainder deleted successfully!!" << endl;

		// get the latest remainder and set notifier
		Notifier::setNotifier(remainderManagement->getLatestRemainder());

		if (!validIOHandlers->isY("Continue deleting [Y/n]?: ")) {
			break;
		}
	}
	
}

void Remainder::updateEvent() {
	while (true) {
		cout << "Enter '0' to exit..." << endl;

		unordered_map<int, Json::Value> remaindersMap = remainderManagement->getRemaindersInMap(
			remainderManagement->readRemainder()
		);
		remainderManagement->printRemaindersInMap(remaindersMap);

		int choice = validIOHandlers->getInt("Enter a Remainder Id to update [Number / 0]: ");

		if (choice == 0) {
			break;
		}

		if (validIOHandlers->isY("Update remainder's summary [Y/n]?: ")) {
			remaindersMap[choice]["summary"] = validIOHandlers->getString("Enter a summary: ");
		}

		if (validIOHandlers->isY("Update remainder's description [Y/n]?: ")) {
			remaindersMap[choice]["description"] = validIOHandlers->getString("Enter a description: ");
		}

		if (validIOHandlers->isY("Update remainder's date [Y/n]?: ")) {
			remaindersMap[choice]["date"] = validIOHandlers->getDate();
		}

		if (validIOHandlers->isY("Update remainder's time [Y/n]?: ")) {
			remaindersMap[choice]["time"] = validIOHandlers->getHourMinute();
		}

		if (validIOHandlers->isY("Update remainder's recurrence [Y/n]?: ")) {
			displayRecurrence();

			int key = validIOHandlers->getInt("Enter a RecurId [Number]: ");
			if (key != 0) {
				remaindersMap[choice]["recurrence"] = getRecurrence(key);
			}
		}

		remainderManagement->writeRemaindersFromMap(remaindersMap);
		cout << "Remainder updated successfully!!" << endl;

		// get the latest remainder and set notifier
		Notifier::setNotifier(remainderManagement->getLatestRemainder());

		if (!validIOHandlers->isY("Continue updating [Y/n]?: ")) {
			break;
		}

	}
}