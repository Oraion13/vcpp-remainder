#include "RemainderMain.h"

//int main()
//{
//	char re[MAX_PATH + 1];
//	string FP = string(re, GetModuleFileName(NULL, (LPWSTR) re, MAX_PATH));
//
//	LONG ln = RegGetValue(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\Currentversion\\Run"), TEXT("MyApp"), RRF_RT_REG_SZ, 0, 0, 0);
//	int tog = 0;
//	if (ERROR_SUCCESS == ln)
//	{
//		tog = 1;
//	}
//
//			if (tog == 0)
//			{
//				tog = 1;
//
//				HKEY hkey;
//				LONG key = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\Currentversion\\Run"), 0, KEY_WRITE, &hkey);
//				if (ERROR_SUCCESS == key)
//				{
//					key = RegSetValueEx(hkey, TEXT("MyApp"), 0, REG_SZ, (BYTE*)FP.c_str(), strlen(FP.c_str()));
//				}
//			}
//
//			Notifier::notifier();
//}

// -----------------------------------------------------------------------------------------------------------

void RemainderMain::remainderMain() {

    DWORD WINAPI RemainderFunction(LPVOID lpParam);
    DWORD WINAPI FileUpdateCheckerFunc(LPVOID lpParam);

    void ErrorHandler(LPTSTR lpszFunction);

        HANDLE remainderThread{};
        DWORD   remainderThreadId{};

        HANDLE fileUpdateThread{};
        DWORD   fileUpdateThreadId{};

        // Create the thread to begin execution on its own.

        remainderThread = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
            RemainderFunction,       // thread function name
            NULL,          // argument to thread function 
            0,                      // use default creation flags 
            &remainderThreadId);   // returns the thread identifier 

        fileUpdateThread = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
            FileUpdateCheckerFunc,       // thread function name
            NULL,          // argument to thread function 
            0,                      // use default creation flags 
            &fileUpdateThreadId);   // returns the thread identifier 


        // Check the return value for success.
        // If CreateThread fails, terminate execution. 
        // This will automatically clean up threads and memory. 

        if (remainderThread == NULL || fileUpdateThread == NULL)
        {
            ExitProcess(3);
        }

        // Wait until all threads have terminated.
        WaitForSingleObject(remainderThread, INFINITE);
        WaitForSingleObject(fileUpdateThread, INFINITE);

        // Close all thread handles and free memory allocations.

        CloseHandle(remainderThread);
        CloseHandle(fileUpdateThread);

    }

    DWORD WINAPI RemainderFunction(LPVOID lpParam) {
        Notifier::notifier();

        return 0;
    }

    DWORD WINAPI FileUpdateCheckerFunc(LPVOID lpParam)
    {
        Notifier::fileUpdateNotifier();

        return 0;
    }
