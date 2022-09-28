#include "Notifier.h"

Json::Value Notifier::REMAINDER{};
DWORD Notifier::TIMER{};

void Notifier::setNotifier(Json::Value latestRemainder) {
	cout << "inside set noti" << endl;
	if (latestRemainder != NULL) {
		cout << "setup" << endl;
		REMAINDER = latestRemainder;
		setSleepTimer(calculateSleepTImer());
	}

	// cout << REMAINDER << endl;
	// cout << "timer: " << TIMER << endl;
}

int Notifier::getYear(string date) {
	return stoi(date.substr(0, 4));
}

int Notifier::getMonth(string date) {
	return stoi(date.substr(5, 2));
}

int Notifier::getDay(string date) {
	return stoi(date.substr(8, 2));
}

int Notifier::getHour(string time) {
	return stoi(time.substr(0, 2));
}

int Notifier::getMinute(string time) {
	return stoi(time.substr(3, 2));
}

DWORD Notifier::calculateSleepTImer() {
	string remainderDate = REMAINDER["date"].asCString();
	string remainderTime = REMAINDER["time"].asCString();

	char currentDate[32]{};
	char currentTime[32]{};

	time_t a = time(nullptr);
	struct tm d;
	if (localtime_s(&d, &a) == 0) {
		strftime(currentDate, sizeof(currentDate), "%F", &d);
		strftime(currentTime, sizeof(currentTime), "%H:%M", &d);
	}

	DWORD timer{};

	// years
	timer += (DWORD) ((getYear(remainderDate) - getYear(currentDate)) * 31556952000);

	// month
	timer += (DWORD) ((getMonth(remainderDate) - getMonth(currentDate)) * 2629800000);

	// days
	timer += (DWORD) ((getDay(remainderDate) - getDay(currentDate)) * 86400000);

	// hours
	timer += (DWORD) ((getHour(remainderTime) - getHour(currentTime)) * 3600000);

	// minutes
	timer += (DWORD) ((getMinute(remainderTime) - getMinute(currentTime)) * 60000);

	return timer < 0 ? 0 : timer;
}

void Notifier::setSleepTimer(DWORD timerMilli) {
	TIMER = timerMilli;
}

LPCWSTR Notifier::getSummary() {
	if (!REMAINDER) {
		return NULL;
	}

	string s = REMAINDER["summary"].asCString();
	wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();

	return sw;
}

LPCWSTR Notifier::getDescription() {
	if (!REMAINDER) {
		return NULL;
	}
	string s = REMAINDER["description"].asCString();
	wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();

	return sw;
}

void Notifier::notifier() {
	RemainderManagement remainderManagement;
	while (true) {
		// checks if a remainder is present
		cout << "Inside remainder" << endl;
		cout << REMAINDER << endl;
		if (!REMAINDER) {
			cout << "checking" << endl;
			setNotifier(remainderManagement.getLatestRemainder());
			if (!REMAINDER) {
				break;
			}
		}
		TIMER = 0;
		// sleep until the remainder occurs
		Sleep(TIMER);

		// display the popup
		MessageBoxA(0, (LPCSTR) REMAINDER["summary"].asCString(), (LPCSTR) REMAINDER["description"].asCString(), MB_OK | MB_ICONINFORMATION);

		// setup next remainder but before make sure to delete the current remainder
		// create a method to delete event by ID
		remainderManagement.deleteOrModifyById(REMAINDER["id"].asCString());

		cout << "after delt" << endl;

		REMAINDER = NULL;
		setNotifier(remainderManagement.getLatestRemainder());
		cout << "after setup bottom" << endl;
		if (!REMAINDER) {
			break;
		}
	}
}