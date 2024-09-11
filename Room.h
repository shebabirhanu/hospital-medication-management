#ifndef SENECA_ROOM_H
#define SENECA_ROOM_H

#include <string>

namespace seneca
{
    /* --- Manages information about a Room that holds treatments and fills patient records ---*/
    class Room
    {
        int m_roomID;                  // Room ID
        std::string m_roomName;        // Room name
        std::string m_roomDescription; // Room description
        unsigned int m_nextPatientNum;     // Next patient number assigned to patient
        unsigned int m_numTreatments;

        static size_t m_widthField; // Min. # of characters required to print item name for Room objects
        static int id_generator;    // Used to generate IDs for Room objects

    public:
        Room(const std::string &record);
        const std::string &getTreatmentName() const;
        size_t getNextPatientNumber();
        size_t getQuantity() const;
        void updateQuantity();
        void display(std::ostream &os, bool full) const;
    };

} // seneca

#endif