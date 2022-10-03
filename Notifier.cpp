#include "Notifier.h"

Json::Value Notifier::REMAINDER{};
DWORD Notifier::TIMER{};
bool Notifier::FLAG{};
bool Notifier::STOP_FLAG{0};
HANDLE Notifier::ghMutex = CreateMutex(
	NULL,              // default security attributes
	FALSE,             // initially not owned
	NULL);             // unnamed mutex;

ofstream Notifier::getAFileToWrite() {
	ofstream fileToWorkWith;

	fileToWorkWith.open("C:\\CustomRemainder\\log.txt", std::ios_base::app);

	// If file does not exist, Create new file
	if (!fileToWorkWith)
	{
		// cout << "Cannot open file, file does not exist. Creating new file..";
		fileToWorkWith.open("C:\\CustomRemainder\\log.txt", std::ios_base::app);
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
	writeLog(getAFileToWrite(), "Setting up the latest remainder");
	if (!latestRemainder.isNull() || latestRemainder != 0) {
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
		) {
		if ((getHour(remainderTime) - getHour(currentTime)) < 0) {
			return true;
		}
		if (((getHour(remainderTime) - getHour(currentTime)) == 0) && ((getMinute(remainderTime) - getMinute(currentTime)) <= 0)) {
			return true;
		}
	}

	return false;
}

DWORD Notifier::calculateSleepTImer() {
	if (REMAINDER.isNull() || REMAINDER == 0) {
		return 0;
	}
	cout << REMAINDER << endl;
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
	writeLog(getAFileToWrite(), "Year: " + to_string(timer));
	// month
	timer += (DWORD) ((getMonth(remainderDate) - getMonth(currentDate)) * 2629800000);
	writeLog(getAFileToWrite(), "Month: " + to_string(timer));
	// days
	timer += (DWORD) ((getDay(remainderDate) - getDay(currentDate)) * 86400000);
	writeLog(getAFileToWrite(), "Day: " + to_string(timer));
	// hours
	timer += (DWORD) ((getHour(remainderTime) - getHour(currentTime)) * 3600000);
	writeLog(getAFileToWrite(), "Hr: " + to_string(timer));
	// minutes
	timer += (DWORD) ((getMinute(remainderTime) - getMinute(currentTime)) * 60000);
	writeLog(getAFileToWrite(), "Min: " + to_string(timer));

	// seconds
	timer -= (DWORD)(getSecond(currentTime) * 1000);
	writeLog(getAFileToWrite(), "Sec: -" + to_string(timer));

	return timer < 0 ? 0 : timer;
}

void Notifier::setSleepTimer(DWORD timerMilli) {
	TIMER = timerMilli;
}

LPCWSTR Notifier::getSummary() {
	if (REMAINDER.isNull() || REMAINDER == 0) {
		return NULL;
	}

	string s = REMAINDER["summary"].asCString();
	wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();

	return sw;
}

LPCWSTR Notifier::getDescription() {
	if (REMAINDER.isNull() || REMAINDER == 0) {
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

	while (true && !STOP_FLAG) {
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
		writeLog(getAFileToWrite(), "Timer: " + to_string(TIMER));

		//TIMER = 0;
		// sleep until the remainder occurs
		Sleep(TIMER);

		if (REMAINDER.isNull() || REMAINDER == 0) {
			continue;
		}

		// -------------------------------------------------------------------------
		// display the popup
		//MessageBoxA(0, (LPCSTR) REMAINDER["summary"].asCString(), (LPCSTR) REMAINDER["description"].asCString(), MB_OK | MB_ICONINFORMATION | MB_SERVICE_NOTIFICATION);
		
		// -------------------------------------------------------------------------
		//DWORD result{};
		//if (WTSSendMessageA(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION,
		//	(LPSTR)REMAINDER["summary"].asCString(), (size_t) strlen(REMAINDER["summary"].asCString()),
		//	(LPSTR)REMAINDER["description"].asCString(), (size_t) strlen(REMAINDER["description"].asCString()),
		//	MB_OK | MB_ICONINFORMATION, 0, &result, FALSE)) {
		//	writeLog(getAFileToWrite(), "Notifier: " + to_string(result));
		//	writeLog(getAFileToWrite(), "Notification sent!");
		//}
		//else {
		//	writeLog(getAFileToWrite(), "Cannot send notification");
		//	writeLog(getAFileToWrite(), "Notifier: " + to_string(result));
		//	writeLog(getAFileToWrite(), "Error code: " + to_string(GetLastError()));
		//}
		
		// -------------------------------------------------------------------------
		//// additional information
		//STARTUPINFO si;
		//PROCESS_INFORMATION pi;

		//// set the size of the structures
		//ZeroMemory(&si, sizeof(si));
		//si.cb = sizeof(si);
		//ZeroMemory(&pi, sizeof(pi));

		//string cmd = REMAINDER["summary"].asCString();
		//cmd.append(" ");
		//cmd.append(REMAINDER["description"].asCString());
		//const char* execPath = "C:\\CustomRemainder\\Notifier.exe";

		////sprintf_s(szcmd, 200, "%s %s", REMAINDER["summary"].asCString(), REMAINDER["description"].asCString());

		//// start the program up
		//if (CreateProcess((LPCWSTR) execPath,   // the path
		//	(LPWSTR) wstring(cmd.begin(), cmd.end()).c_str(),        // Command line
		//	NULL,           // Process handle not inheritable
		//	NULL,           // Thread handle not inheritable
		//	FALSE,          // Set handle inheritance to FALSE
		//	0,              // No creation flags
		//	NULL,           // Use parent's environment block
		//	NULL,           // Use parent's starting directory 
		//	&si,            // Pointer to STARTUPINFO structure
		//	&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
		//)) {
		//	writeLog(getAFileToWrite(), "Process Started!");
		//}
		//else {
		//	writeLog(getAFileToWrite(), "Cannot start the notifier process!");
		//	writeLog(getAFileToWrite(), "Error: " + GetLastError());
		//}
		//
		//// Close process and thread handles.
		//WaitForSingleObject(pi.hProcess, INFINITE);
		//CloseHandle(pi.hProcess);
		//CloseHandle(pi.hThread);

		// ------------------------------------------------------------------------------------
		/*string privilage = "runas";
		string execPath = "Notifier.exe";
		string cmd = REMAINDER["summary"].asCString();
		cmd.append(" ");
		cmd.append(REMAINDER["description"].asCString());

		HINSTANCE result = ShellExecuteW(NULL, (LPCWSTR) privilage.c_str(), (LPCWSTR)execPath.c_str(), (LPCWSTR)cmd.c_str(), NULL, SW_SHOWNORMAL);
		cout << result << endl;
		writeLog(getAFileToWrite(), "Process code: " + to_string(GetLastError()));*/

		// -------------------------------------------------------------------------------------

		string str = REMAINDER["summary"].asCString();
		str.append(" ");
		str.append(REMAINDER["description"].asCString());
		const wchar_t* privilage = L"runas";
		const wchar_t* execPath = L"C:\\CustomRemainder\\Notifier.exe";

		wstring widestr = std::wstring(str.begin(), str.end());
		const wchar_t* cmd = widestr.c_str();
		//string execPath = "C:\\CustomRemainder\\Notifier.exe";
		//string cmd = REMAINDER["summary"].asCString();
		//cmd.append(" ");
		//cmd.append(REMAINDER["description"].asCString());

		SHELLEXECUTEINFO shExInfo = { 0 };
		shExInfo.cbSize = sizeof(shExInfo);
		shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		shExInfo.hwnd = 0;
		shExInfo.lpVerb = (LPCWSTR) privilage;                // Operation to perform
		shExInfo.lpFile = (LPCWSTR)execPath;       // Application to start    
		shExInfo.lpParameters = (LPCWSTR) cmd;                  // Additional parameters
		shExInfo.lpDirectory = 0;
		shExInfo.nShow = SW_HIDE;
		shExInfo.hInstApp = 0;

		if (ShellExecuteEx(&shExInfo))
		{
			writeLog(getAFileToWrite(), "Process Started!");
			WaitForSingleObject(shExInfo.hProcess, 30);
			CloseHandle(shExInfo.hProcess);
		}
		else {
			writeLog(getAFileToWrite(), "Cannot start the notifier process!");
			//	writeLog(getAFileToWrite(), "Error: " + GetLastError());
		}

		// -------------------------------------------------------------------------------------
		// setup next remainder but before make sure to delete the current remainder
		// create a method to delete event by ID
		remainderManagement->deleteOrModifyById(REMAINDER["id"].asCString());

		REMAINDER = NULL;
		writeLog(getAFileToWrite(), "Notified!");
	}
}

void Notifier::fileUpdateNotifier() {
	RemainderManagement remainderManagement;
	string oldContents = remainderManagement.giveMeContents();

	while (true && !STOP_FLAG) {
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