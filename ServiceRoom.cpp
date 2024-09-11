#include <deque>
#include <queue>
#include <iostream>

#include "ServiceRoom.h"
#include "PatientRecord.h"
#include "HospitalManager.h"

using namespace std;

namespace seneca
{
    deque<PatientRecord> g_pending;
    deque<PatientRecord> g_completed;
    deque<PatientRecord> g_incomplete;
    
    ServiceRoom::ServiceRoom(string &str) : Room(str)
    {
        m_pNextRoom = nullptr;
    }
    void ServiceRoom::fill(ostream &os)
    {
        if(!m_records.empty()){ 
            m_records.front().fillTreatment(*this, os);
        } 
    }
    bool ServiceRoom::attemptToMoveRecord() {
        if (m_records.empty()) {
            return false; // early exit
        }

        // Check if the treatment at this room is filled or the room is out of stock
        if (m_records.front().isTreatmentFilled(getTreatmentName()) || this->getQuantity() == 0) {
            if (m_pNextRoom == nullptr) {
                if (m_records.front().isRecordFilled()) {
                    g_completed.emplace_back(std::move(m_records.front()));
                } else {
                    g_incomplete.emplace_back(std::move(m_records.front()));
                }
            } else {
                (*m_pNextRoom) += std::move(m_records.front()); // move record to the next room
            }
            m_records.pop_front();
            return true; 
        }
        
        return false; 
    }


    void ServiceRoom::setNextRoom(ServiceRoom *room)
    {
        m_pNextRoom = room;
        room = nullptr;
    }

    ServiceRoom *ServiceRoom::getNextRoom() const
    {
        return m_pNextRoom;
    }

    void ServiceRoom::display(ostream &os) const
    {
        os << getTreatmentName();  

        if (m_pNextRoom == nullptr) {
            os << " --> End of Line";
        } else {
            os << " --> " << m_pNextRoom->getTreatmentName();
        }
        os << "\n"; 
    }

    ServiceRoom &ServiceRoom::operator+=(PatientRecord&& newRecord)
    {
        m_records.push_back(std::move(newRecord));
        return *this; 
    }
    ServiceRoom::~ServiceRoom(){}

}