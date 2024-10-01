# Medication Management System

A personal project using C++ Object Oriented programming, with a focus on C++ STL algorithms and containers (vector, deque, list). This medication management system organizes where each patient receives a set of medications.

**NOTE:** This is a re-factored educational project that does not consider standards or safety when dispensing medication.  The provided test data uses over the counter medication. 

<img width="1186" alt="medication-management-system" src="https://github.com/user-attachments/assets/6c723a92-020a-4457-87e6-80a27915aa0e">

In-patient hospital units are reqired to record dispensing and inventory of all medications. This program simulates a system that fills prescriptions in hospital rooms from inventory.

This system assumes a single room holds a range of inventory, with  multiple rooms. Each room is equipped with a range of medications to be supplied to patients.  Each patient record consists of a prescription that must be filled by giving medication to the patient. 

**Installation**

Clone the Repository:
```
git clone https://github.com/yourusername/HospitalManager.git
```
**Compilation**
```
g++ -std=c++14 -o HospitalManager main.cpp HospitalManager.cpp PatientRecord.cpp Room.cpp ServiceRoom.cpp Utilities.cpp
```
**Run Application**
```
./HospitalManager Rooms1.txt Rooms2.txt PatientRecord.txt SystemLine.txt
```


