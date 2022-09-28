#include "Notifier.h"

Json::Value Notifier::REMAINDER{};
DWORD Notifier::TIMER{};

void Notifier::setNotifier(Json::Value latestRemainder) {
	REMAINDER = latestRemainder;

	if (REMAINDER) {
		setSleepTimer(calculateSleepTImer());
	}
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
		if (!REMAINDER) {
			break;
		}

		// sleep until the remainder occurs
		Sleep(TIMER);

		// display the popup
		MessageBox(0, getSummary(), getDescription(), MB_OK | MB_ICONINFORMATION);

		// setup next remainder but before make sure to delete the current remainder
		// create a method to delete event by ID
		remainderManagement.deleteOrModifyById(REMAINDER["id"].asCString());

		setNotifier(remainderManagement.getLatestRemainder());
	}
}