#ifndef SENECA_PATIENTRECORD_H
#define SENECA_PATIENTRECORD_H

#include <string>
#include "Room.h"

namespace seneca
{
    class PatientRecord
    {
        private:
            struct Treatment
            {
                std::string m_treatmentName;
                size_t m_serialNumber;
	            bool m_isFilled;

                Treatment(const std::string& src) : m_treatmentName(src), m_serialNumber (0), m_isFilled(false) {};
            };

            std::string m_name;
            std::string m_product;
            size_t m_cntTreatment;
            Treatment** m_1stTreatment; //Array of Pointers/Pointers to Pointers of type struct Treatment 

            static size_t m_widthField; 

        public:
            PatientRecord(); 
            PatientRecord(std::string& str); //Custom constructor 
            PatientRecord(const PatientRecord& room); //Copy constructor
            PatientRecord(PatientRecord&& room) noexcept; //Move constructor
            
            PatientRecord& operator=(const PatientRecord& room) noexcept = delete; //Copy assignment operator
            PatientRecord& operator=(PatientRecord &&room) noexcept; //Move assignment operator
            ~PatientRecord();

            bool isRecordFilled() const;
            bool isTreatmentFilled(const std::string& treatmentName) const;
            void fillTreatment(Room& room, std::ostream& os);
            void display(std::ostream& os) const;
    };
}

#endif