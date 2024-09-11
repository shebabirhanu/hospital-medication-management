#include <iomanip>
#include <iostream>
#include <string>
#include <algorithm>
#include "Room.h"
#include "Utilities.h"

namespace seneca
{
    size_t Room::m_widthField = 0;
    int Room::id_generator = 0;
    
    /* Custom Constructor */
    /* record (one line) is retrieved from input file */
    Room::Room(const std::string& record)
    { 
        Utilities utils; // Use utils to extract each token from the record
        m_roomID = ++id_generator; // Increment class variable to reflect the instantiation of utils
        size_t next_pos = 0; 
        bool more = true;

        try {
            // Populate Room object using utils
            m_roomName = utils.extractToken(record, next_pos, more);
            m_nextPatientNum = std::stoi(utils.extractToken(record, next_pos, more)); // Convert to int
            m_numTreatments = std::stoi(utils.extractToken(record, next_pos, more)); // Convert to int

            m_widthField = std::max(m_widthField, utils.getFieldWidth());
            
            m_roomDescription = utils.extractToken(record, next_pos, more);
        } catch (...) {
            std::cout << "Error: constructor failed.";
        }

        
    }
    const std::string &Room::getTreatmentName() const
    {
        return m_roomName;
    }
    size_t Room::getNextPatientNumber()
    {
        return m_nextPatientNum++;
    }
    size_t Room::getQuantity() const
    {
        return m_numTreatments;
    }
    void Room::updateQuantity()
    {
        // Check that number of items does not drop below 0
        if (m_numTreatments > 0)
        {
            m_numTreatments -= 1; // subtract 1 from current items
        }
        else
        {
            std::cout << "Quantity is below 0, cannot update.";
        }
    }
    /* Display current Room object information */
    void Room::display(std::ostream &os, bool full) const
    {
        // Reset field formatting
        os << std::resetiosflags(std::ios::adjustfield) << std::setfill(' ');  

        //Display room id
        os << std::setw(3) << std::setfill('0') << m_roomID << " | ";
        
        // Display room name
        os << std::setw(m_widthField) << std::left << std::setfill(' ') << m_roomName << " | ";
        
        // Reset field formatting before displaying next serial number
        os << std::resetiosflags(std::ios::adjustfield) << std::setfill(' '); 

        //Display next patient number
        os << std::setw(6) << std::setfill('0') << m_nextPatientNum << " | ";
        
        if (full)
        {   //DIsplay room quantity and room description
            os << std::setw(4) << std::right << std::setfill(' ') << getQuantity() << " | " 
                << m_roomDescription;
        }
        os << '\n';
    }
}