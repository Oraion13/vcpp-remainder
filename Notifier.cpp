#include "Notifier.h"

Json::Value Notifier::REMAINDER{};
DWORD Notifier::TIMER{};
bool Notifier::FLAG{};
HANDLE Notifier::ghMutex = CreateMutex(
	NULL,              // default security attributes
	FALSE,             // initially not owned
	NULL);             // unnamed mutex;

ofstream Notifier::getAFileToWrite() {
	ofstream fileToWorkWith;

	fileToWorkWith.open("log.txt", std::ios_base::app);

	// If file does not exist, Create new file
	if (!fileToWorkWith)
	{
		// cout << "Cannot open file, file does not exist. Creating new file..";
		fileToWorkWith.open("log.txt", std::ios_base::app);
	}
	else
	{    // use existing file
		// cout << "success " << "log.txt" << " found. \n";
	}

	return fileToWorkWith;
}

void Notifier::writeLog(ofstream file, string txt) {
	if (ghMutex == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		return;
	}

	WaitForSingleObject(
		ghMutex,    // handle to mutex
		INFINITE);  // no time-out interval

	auto end = std::chrono::system_clock::now();
	time_t end_time = std::chrono::system_clock::to_time_t(end);
	char str[26] = {};
	ctime_s(str, 26, &end_time);
	file << str << ": " << txt << endl;
	file.close();

	ReleaseMutex(ghMutex);
}

void Notifier::setNotifier(Json::Value latestRemainder) {
	writeLog(getAFileToWrite(), "Setting a new remainder");
	if (latestRemainder != NULL) {
		//cout << "setting up notification..." << endl;
		REMAINDER = latestRemainder;
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

int Notifier::getSecond(string time) {
	return stoi(time.substr(6, 2));
}

bool Notifier::isAPastReminder() {
	writeLog(getAFileToWrite(), "Checking is a past remainder?...");
	string remainderDate = REMAINDER["date"].asCString();
	string remainderTime = REMAINDER["time"].asCString();

	char currentDate[32]{};
	char currentTime[32]{};

	time_t a = time(nullptr);
	struct tm d;
	if (localtime_s(&d, &a) == 0) {
		strftime(currentDate, sizeof(currentDate), "%F", &d);
		strftime(currentTime, sizeof(currentTime), "%H:%M:%S", &d);
	}

	// if old remainder
	if ((getYear(remainderDate) <= getYear(currentDate))
		&& (getMonth(remainderDate) <= getMonth(currentDate))
		&& (getDay(remainderDate) < getDay(currentDate))
		) {
		return true;
	}

	// same day, but old remainder
	if ((getYear(remainderDate) <= getYear(currentDate))
		&& (getMonth(remainderDate) <= getMonth(currentDate))
		&& (getDay(remainderDate) <= getDay(currentDate))
		&& (getHour(remainderTime) <= getHour(currentTime))
		&& (getMinute(remainderTime) <= getMinute(currentTime))) {
		return true;
	}

	return false;
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
		strftime(currentTime, sizeof(currentTime), "%H:%M:%S", &d);
	}

	if(isAPastReminder()) {
		writeLog(getAFileToWrite(), "Past event occured!");
		return 0;
	}

	DWORD timer{};

	// cout << "Time: " << currentTime << endl;
	writeLog(getAFileToWrite(), "Timer...:");
	// years
	timer += (DWORD) ((getYear(remainderDate) - getYear(currentDate)) * 31556952000);
	writeLog(getAFileToWrite(), "Year: " + timer);
	// month
	timer += (DWORD) ((getMonth(remainderDate) - getMonth(currentDate)) * 2629800000);
	writeLog(getAFileToWrite(), "Month: " + timer);
	// days
	timer += (DWORD) ((getDay(remainderDate) - getDay(currentDate)) * 86400000);
	writeLog(getAFileToWrite(), "Day: " + timer);
	// hours
	timer += (DWORD) ((getHour(remainderTime) - getHour(currentTime)) * 3600000);
	writeLog(getAFileToWrite(), "Hr: " + timer );
	// minutes
	timer += (DWORD) ((getMinute(remainderTime) - getMinute(currentTime)) * 60000);
	writeLog(getAFileToWrite(), "Min: " + timer );

	// seconds
	timer -= (DWORD)(getSecond(currentTime) * 1000);
	writeLog(getAFileToWrite(), "Sec: -" + timer);

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

string Notifier::jsonToString(Json::Value json) {
	Json::StreamWriterBuilder builder;
	builder["indentation"] = ""; // If you want whitespace-less output
	return Json::writeString(builder, json);
}

void Notifier::notifier() {
	RemainderManagement* remainderManagement = new RemainderManagement();
	setNotifier(remainderManagement->getLatestRemainder());

	while (true) {
		// empty remainder exit flag
		FLAG = false;
		// checks if a remainder is present
		writeLog(getAFileToWrite(), "Inside notifier");
		writeLog(getAFileToWrite(), jsonToString(REMAINDER));
		if (REMAINDER.isNull() || REMAINDER == 0) {
			writeLog(getAFileToWrite(), "No remainder present, checking for a remainder");
			setNotifier(remainderManagement->getLatestRemainder());
			// if an empty array of remainder is present, method will wait until a new remainder is added
			while ((REMAINDER.isNull() || REMAINDER == 0) && !FLAG) {
				writeLog(getAFileToWrite(), "Waiting for a new remainder...");
				Sleep(10000);
			}
			writeLog(getAFileToWrite(), "Remainder created");
		}
		writeLog(getAFileToWrite(), "Remainder present...waiting for timer");

		writeLog(getAFileToWrite(), jsonToString(REMAINDER));
		writeLog(getAFileToWrite(), "Timer: " + TIMER);

		//TIMER = 0;
		// sleep until the remainder occurs
		Sleep(TIMER);

		// display the popup
		MessageBoxA(0, (LPCSTR) REMAINDER["summary"].asCString(), (LPCSTR) REMAINDER["description"].asCString(), MB_OK | MB_ICONINFORMATION);

		// setup next remainder but before make sure to delete the current remainder
		// create a method to delete event by ID
		remainderManagement->deleteOrModifyById(REMAINDER["id"].asCString());

		REMAINDER = NULL;
		writeLog(getAFileToWrite(), "Success!");
	}
}

void Notifier::fileUpdateNotifier() {
	RemainderManagement remainderManagement;
	string oldContents = remainderManagement.giveMeContents();

	while (true) {
		// periodic
		writeLog(getAFileToWrite(), "Periodic check...");
		Sleep(5000);

		if (remainderManagement.isFileUpdated(oldContents)) {
			writeLog(getAFileToWrite(), "Json file modified");
			oldContents = remainderManagement.giveMeContents();
			setNotifier(remainderManagement.getLatestRemainder());

			FLAG = true;
		}
	}
}