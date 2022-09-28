#include <windows.h>
#include <iostream>
#include <conio.h>

#include "Notifier.h"
#include "json/json.h"

using namespace std;

int main()
{
	char re[MAX_PATH + 1];
	string FP = string(re, GetModuleFileName(NULL, (LPWSTR) re, MAX_PATH));

	LONG ln = RegGetValue(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\Currentversion\\Run"), TEXT("MyApp"), RRF_RT_REG_SZ, 0, 0, 0);
	int tog = 0;
	if (ERROR_SUCCESS == ln)
	{
		tog = 1;
		Notifier::notifier();
	}
	
	/*
	if (tog == 1)
	{
		system("cls");
		cout << "ON" << endl;
	}
	else if (tog == 0)
	{
		system("cls");
		cout << "OFF" << endl;
	}
	*/

	//while (1)
	//{
		/*
		  char get = _getch();
		/ if (get == 't' || get == 'T')
		/ {
		
			if (tog == 1)
			{
				tog = 0;
				system("cls");
				cout << "OFF" << endl;

				HKEY hkey = HKEY_CURRENT_USER;
				RegOpenKey(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\Currentversion\\Run"), &hkey);
				RegDeleteValue(hkey, TEXT("MyApp"));
				RegCloseKey(hkey);
			}
			else */
			if (tog == 0)
			{
				tog = 1;
				/*
				system("cls");
				cout << "ON" << endl;
				*/

				HKEY hkey;
				LONG key = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\Currentversion\\Run"), 0, KEY_WRITE, &hkey);
				if (ERROR_SUCCESS == key)
				{
					key = RegSetValueEx(hkey, TEXT("MyApp"), 0, REG_SZ, (BYTE*)FP.c_str(), strlen(FP.c_str()));
				}
			}

			Notifier::notifier();
		// }
	//}
}
