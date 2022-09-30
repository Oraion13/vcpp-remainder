//#include "operations.h"
//#include "json/json.h"
//
//#include <tchar.h>
//#include <strsafe.h>
//#include <Windows.h>
//#include <iostream>
//
//int main() {
//	new Operations();
//}

// ----------------------------------------------------------------------------------------
//#define BUF_SIZE 255
//
//DWORD WINAPI CrudFunction(LPVOID lpParam);
//DWORD WINAPI RemainderMainFunc(LPVOID lpParam);
//
//void ErrorHandler(LPTSTR lpszFunction);
//
//
//int _tmain()
//{
//    HANDLE crudThread{};
//    DWORD   crudThreadId{};
//
//    HANDLE remainderThread{};
//    DWORD   remainderThreadId{};
//
//        // Create the thread to begin execution on its own.
//
//        crudThread = CreateThread(
//            NULL,                   // default security attributes
//            0,                      // use default stack size  
//            CrudFunction,       // thread function name
//            NULL,          // argument to thread function 
//            0,                      // use default creation flags 
//            &crudThreadId);   // returns the thread identifier 
//
//        remainderThread = CreateThread(
//            NULL,                   // default security attributes
//            0,                      // use default stack size  
//            RemainderMainFunc,       // thread function name
//            NULL,          // argument to thread function 
//            0,                      // use default creation flags 
//            &remainderThreadId);   // returns the thread identifier 
//
//
//        // Check the return value for success.
//        // If CreateThread fails, terminate execution. 
//        // This will automatically clean up threads and memory. 
//
//       if (crudThread == NULL || remainderThread == NULL)
//       {
//            ExitProcess(3);
//       }
//       
//    // Wait until all threads have terminated.
//        WaitForSingleObject(crudThread, INFINITE);
//        WaitForSingleObject(remainderThread, INFINITE);
//
//    // Close all thread handles and free memory allocations.
//
//        CloseHandle(crudThread);
//        CloseHandle(remainderThread);
//
//    return 0;
//}
//
//DWORD WINAPI RemainderMainFunc(LPVOID lpParam) {
//    Notifier::notifier();
//
//    return 0;
//}
//
//DWORD WINAPI CrudFunction(LPVOID lpParam)
//{
//    new Operations();
//
//    return 0;
//}