#include <iostream>
#include "ValidIOHandlers.h"
#include "Remainder.h"

int main() {
	/*
	ValidIOHandlers* validIOhandlers = new ValidIOHandlers();

	string choice = validIOhandlers->getHourMinute();
	cout << boolalpha;
	cout << "Choice is: " << choice;
	*/

	Remainder* remainder = new Remainder();

	// remainder->createEvent();
	// remainder->readEvent();
	// remainder->deleteEvent();
	remainder->updateEvent();
}