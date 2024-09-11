#ifndef SENECA_SERVICEROOM_H
#define SENECA_SERVICEROOM_H

#include <deque>
#include "PatientRecord.h"


namespace seneca
{
    extern std::deque<PatientRecord> g_pending; //records to be placed onto the assembly line at the first room.
    extern std::deque<PatientRecord> g_completed; //records that have been removed from the last room and have been completely filled
    extern std::deque<PatientRecord> g_incomplete; //records that have been removed from the last room and could not be filled
    
    class ServiceRoom : public Room 
    {
    private:
        ServiceRoom* m_pNextRoom; //a pointer to the next ServiceRoom 

    public:
        std::deque<PatientRecord> m_records; //records that have been recorded in this room 
        ServiceRoom(const ServiceRoom& a) = delete; 
        ServiceRoom& operator=(const ServiceRoom& a) = delete; 


        ServiceRoom(ServiceRoom&& a) = delete; 
        ServiceRoom& operator=(ServiceRoom&&a) = delete;

        ~ServiceRoom();

        ServiceRoom(std::string& str);
        void fill(std::ostream& os);
        bool attemptToMoveRecord();
        void setNextRoom(ServiceRoom* room);
        ServiceRoom* getNextRoom() const;
        void display(std::ostream& os) const;
        ServiceRoom& operator+=(PatientRecord&& newRecord);

    };
}

#endif