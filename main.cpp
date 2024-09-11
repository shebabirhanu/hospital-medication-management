#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Room.h"
#include "ServiceRoom.h"
#include "PatientRecord.h"
#include "Utilities.h"
#include "HospitalManager.h"

int cout = 0; 

template<typename T>
static void loadFromFile(const char*, std::vector<T>&);
template<typename T>
static void loadFromFile(const char*, std::vector<T*>&);

int main(int argc, char** argv)
{
	std::cout << "Command Line: " << argv[0];
	for (int i = 1; i < argc; ++i)
		std::cout << " " << argv[i];
	std::cout << std::endl << std::endl;
	if (argc != 5) {
		std::cerr << "ERROR: Incorrect number of arguments\n";
		std::exit(1);
	}

	std::vector<seneca::ServiceRoom*> theRooms;
	std::vector<seneca::PatientRecord> theRecords;

	try {
		seneca::Utilities::setDelimiter(',');
		::loadFromFile(argv[1], theRooms);
		seneca::Utilities::setDelimiter('|');
		::loadFromFile(argv[2], theRooms);

		std::cout << "========================================" << std::endl;
		std::cout << "=         Rooms (summary)           =" << std::endl;
		std::cout << "========================================" << std::endl;
		for (const auto* room : theRooms)
			room->Room::display(std::cout, false);
		std::cout << std::endl << std::endl;

		std::cout << "========================================" << std::endl;
		std::cout << "=          Rooms (full)             =" << std::endl;
		std::cout << "========================================" << std::endl;
		for (const auto* room : theRooms)
			room->Room::display(std::cout, true);
		std::cout << std::endl << std::endl;

		//Select an object and verify all the functionality it working
		std::cout << "========================================" << std::endl;
		std::cout << "=          Manual Validation           =" << std::endl;
		std::cout << "========================================" << std::endl;
		std::cout << "getTreatmentName(): " << theRooms[0]->getTreatmentName() << std::endl;
		std::cout << "getNextPatientNumber(): " << theRooms[0]->getNextPatientNumber() << std::endl;
		std::cout << "getNextPatientNumber(): " << theRooms[0]->getNextPatientNumber() << std::endl;
		std::cout << "getQuantity(): " << theRooms[0]->getQuantity() << std::endl;
		theRooms[0]->updateQuantity();
		std::cout << "getQuantity(): " << theRooms[0]->getQuantity() << std::endl;
		std::cout << std::endl << std::endl;


		::loadFromFile<seneca::PatientRecord>(argv[3], theRecords);

		std::cout << "========================================" << std::endl;
		std::cout << "=                Records                =" << std::endl;
		std::cout << "========================================" << std::endl;
		for (auto& record : theRecords) {
			record.display(std::cout);
			seneca::g_pending.push_back(std::move(record));
		}
		std::cout << std::endl << std::endl;

		std::cout << "========================================" << std::endl;
		std::cout << "=       Display Rooms (loaded)      =" << std::endl;
		std::cout << "========================================" << std::endl;
		seneca::HospitalManager lm(argv[4], theRooms);
		lm.display(std::cout);
		std::cout << std::endl << std::endl;

		std::cout << "========================================" << std::endl;
		std::cout << "=      Display Rooms (ordered)      =" << std::endl;
		std::cout << "========================================" << std::endl;
		lm.reorderRooms();
		lm.display(std::cout);
		std::cout << std::endl << std::endl;

		std::cout << "========================================" << std::endl;
		std::cout << "=           Filling Rooms             =" << std::endl;
		std::cout << "========================================" << std::endl;
		//run the assembly line until all records processed
		while (!lm.run(std::cout));
		std::cout << std::endl << std::endl;
	}
	catch (const std::string& msg) {
		std::cerr << msg << '\n';
		std::exit(2);
	}

	std::cout << "========================================" << std::endl;
	std::cout << "=      Processed Rooms (complete)     =" << std::endl;
	std::cout << "========================================" << std::endl;
	for (const auto& o : seneca::g_completed)
		o.display(std::cout);
	std::cout << std::endl << std::endl;

	std::cout << "========================================" << std::endl;
	std::cout << "=     Processed Rooms (incomplete)    =" << std::endl;
	std::cout << "========================================" << std::endl;
	for (const auto& o : seneca::g_incomplete)
		o.display(std::cout);
	std::cout << std::endl << std::endl;

	std::cout << "========================================" << std::endl;
	std::cout << "=          Inventory (full)            =" << std::endl;
	std::cout << "========================================" << std::endl;
	for (const seneca::Room* theTreatment : theRooms)
		theTreatment->display(std::cout, true);
	std::cout << std::endl;

	// cleanup
	for (auto room : theRooms)
		delete room;

	return cout;
}

template<typename T>
static void loadFromFile(const char* filename, std::vector<T>& theCollection)
{
	if (!filename) {
		throw std::string("ERROR: No filename provided.");
	}
	std::ifstream file(filename);
	if (!file)
		throw std::string("Unable to open [") + filename + "] file.";

	std::string record;
	while (!file.eof())
	{
		std::getline(file, record);
		T elem(record);
		theCollection.push_back(std::move(elem));
	}

	file.close();
}

template<typename T>
static void loadFromFile(const char* filename, std::vector<T*>& theCollection)
{
	if (!filename) {
		throw std::string("ERROR: No filename provided.");
	}
	std::ifstream file(filename);
	if (!file)
		throw std::string("Unable to open [") + filename + "] file.";

	std::string record;
	while (!file.eof())
	{
		std::getline(file, record);
		theCollection.push_back(new T(record));
	}

	file.close();
}
