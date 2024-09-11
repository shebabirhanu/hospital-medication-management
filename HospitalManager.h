#ifndef SENECA_LINEMANAGER_H
#define SENECA_LINEMANAGER_H

#include <vector>
#include "ServiceRoom.h"

namespace seneca
{   
    class HospitalManager{ //manages an assembly line of active rooms

    private: 
        std::vector<ServiceRoom*> m_activeLine; //the collection of servicerooms for the current assembly line.
        size_t m_cntPatientRecord; //the total number of PatientRecord objects
        ServiceRoom* m_firstRoom; //points to the first active room in the system

    public:
        HospitalManager(const std::string& fileName, const std::vector<ServiceRoom*>& rooms);
        void reorderRooms();
        bool run(std::ostream& os);
        void display(std::ostream& os) const;
        ~HospitalManager();
    };
    
}

#endif