#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility> 
#include <sstream> 

#include "PatientRecord.h"
#include "Utilities.h"
#include "Room.h" 

namespace seneca
{
    /* Static members */
    size_t PatientRecord::m_widthField;

    //Default constructor
    PatientRecord::PatientRecord() {}

    //Custom constructor
    PatientRecord::PatientRecord(std::string &str)
    {
        Utilities utils;

        try
        {
            size_t next_pos = 0;
            bool more = true;
            std::vector<Treatment*> tempTreatments; 

            m_name = utils.extractToken(str, next_pos, more);
            m_product = utils.extractToken(str, next_pos, more); 

            while (more)
            {
                std::string treatment = utils.extractToken(str, next_pos, more);

                tempTreatments.push_back(new Treatment(treatment));
            }

            m_cntTreatment = tempTreatments.size(); 

            m_1stTreatment = new Treatment*[m_cntTreatment];

            for (size_t i = 0; i < m_cntTreatment; ++i) {
                m_1stTreatment[i] = tempTreatments[i];
            }

            m_widthField = std::max(m_widthField, utils.getFieldWidth());
        }
        catch (...)
        {
            std::cout << "Error: constructor failed.";
        }
    }
    //Copy constructor
    PatientRecord::PatientRecord(const PatientRecord& other) {
        throw std::runtime_error("PatientRecord object does not allow copy operations.");
    }
    //Move constructor
    PatientRecord::PatientRecord(PatientRecord &&record) noexcept
        : m_name(std::move(record.m_name)),
            m_product(std::move(record.m_product)),
            m_cntTreatment(record.m_cntTreatment),
            m_1stTreatment(record.m_1stTreatment)
    {
        record.m_cntTreatment = 0;
        record.m_1stTreatment = nullptr;
    }
    //Move assignment operator
    PatientRecord& PatientRecord::operator=(PatientRecord &&record) noexcept
    {
        if (this != &record)
        {
            if (m_1stTreatment){
                for (size_t i = 0; i < m_cntTreatment; ++i) {
                    delete m_1stTreatment[i]; 
                }
                delete[] m_1stTreatment;  
            }

            m_name = std::move(record.m_name);
            m_product = std::move(record.m_product);
            m_cntTreatment = record.m_cntTreatment;
            m_1stTreatment = record.m_1stTreatment;
            
            record.m_cntTreatment = 0;
            record.m_1stTreatment = nullptr;
        }
        return *this;
    }
    PatientRecord::~PatientRecord() 
    {
        if (m_1stTreatment){
            for (size_t i = 0; i < m_cntTreatment; ++i) {
                delete m_1stTreatment[i]; 
            }
            delete[] m_1stTreatment; 
            m_1stTreatment = nullptr; 
        }
    }
    bool PatientRecord::isRecordFilled() const
    {
        return std::all_of(m_1stTreatment, m_1stTreatment + m_cntTreatment,
            [](Treatment* treatment) { return treatment->m_isFilled; });
    }
    bool PatientRecord::isTreatmentFilled(const std::string &treatmentName) const
    {
        bool result = true;

		for (size_t i = 0u; i < m_cntTreatment && result; i++)
		{
			if (m_1stTreatment[i]->m_treatmentName == treatmentName)
			{
				result = m_1stTreatment[i]->m_isFilled;
			}
		}
		return result;
    }
    
    void PatientRecord::fillTreatment(Room &room, std::ostream &os)
    {
        bool treatmentFilled = false;

        for (size_t i = 0; i < m_cntTreatment; i++){
            if (!m_1stTreatment[i]->m_isFilled && !treatmentFilled)
            {
                if (m_1stTreatment[i]->m_treatmentName == room.getTreatmentName()) //
                {
                    if (room.getQuantity() >= 1) //Check room's inventory 
                    {
                        // Fill record with single treatment
                        room.updateQuantity(); //subtract 1 from the inventory 
                        m_1stTreatment[i]->m_serialNumber = room.getNextPatientNumber(); 
                        m_1stTreatment[i]->m_isFilled = true; 
                        os << "    Filled " << m_name << ", " << m_product << " [" << m_1stTreatment[i]->m_treatmentName << "]\n";
                        treatmentFilled = true;
                    }
                    else {
                            os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_1stTreatment[i]->m_treatmentName << "]\n";
                    }
                }
            }
        }
    }

    void PatientRecord::display(std::ostream &os) const
    {
        // Print patient name and product name
        os << m_name << " - " << m_product << std::endl;

        // Print patient number, treatment name, and status
        for (size_t i = 0; i < m_cntTreatment; i++){

            std::ostringstream oss;
            oss << std::setw(6) << std::setfill('0') << m_1stTreatment[i]->m_serialNumber;
            std::string serialStr = oss.str();

            os << "[" << serialStr << "] ";

            os << std::setw(m_widthField) << std::left << std::setfill(' ') << m_1stTreatment[i]->m_treatmentName << " - ";
            os << (m_1stTreatment[i]->m_isFilled ? "FILLED" : "TO BE FILLED") << std::endl;

            os << std::setfill(' ');
        }
    }
}