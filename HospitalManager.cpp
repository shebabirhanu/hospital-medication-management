#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "Utilities.h"
#include "ServiceRoom.h"
#include "PatientRecord.h"
#include "HospitalManager.h"

using namespace std;

namespace seneca
{
    //  this constructor receives: 1) the name of the file that identifies the active rooms
    //  on the room system and 2) the collection of ServiceRooms
    //  available for configuring.
    HospitalManager::HospitalManager(const string &fileName, const vector<ServiceRoom*> &rooms)
        : m_activeLine(rooms)
    {
        ifstream infile(fileName);

        if (!infile.is_open())
        {
            cerr << "File failed to open" << endl;
            return;
        }
        else
        {
            try
            {
                m_activeLine.clear();

                vector<ServiceRoom*> tempActiveLine;
                Utilities utils; // Use utils to extract each token from the record
                string record;
                while (getline(infile, record))
                {
                    size_t next_pos = 0;
                    bool more = true;

                    string currentRoomName = utils.extractToken(record, next_pos, more);
                    string nextRoomName = more ? utils.extractToken(record, next_pos, more) : "";

                    ServiceRoom*currentServiceRoom= nullptr;
                    ServiceRoom*nextServiceRoom= nullptr;

                    std::for_each(rooms.begin(), rooms.end(), [&](ServiceRoom*ws)
                                  {
                        if (ws->getTreatmentName() == currentRoomName) {
                            currentServiceRoom= ws;
                        } else if (ws->getTreatmentName() == nextRoomName) {
                            nextServiceRoom= ws;
                        } });
                    if (currentServiceRoom)
                    {
                        if (m_activeLine.empty())
                        {
                            m_firstRoom = currentServiceRoom;
                        }

                        currentServiceRoom->setNextRoom(nextServiceRoom);

                        if (std::find(m_activeLine.begin(), m_activeLine.end(), currentServiceRoom) == m_activeLine.end())
                        {
                            m_activeLine.push_back(currentServiceRoom);
                        }
                    }
                }
            }
            catch (...)
            {
                cout << "Error: constructor failed.";
            }
        }

        infile.close();

        auto firstRoomIter = std::find_if(m_activeLine.begin(), m_activeLine.end(),
                                             [this](ServiceRoom*ws)
                                             {
                                                 return std::none_of(m_activeLine.begin(), m_activeLine.end(),
                                                                     [ws](ServiceRoom*otherWs)
                                                                     {
                                                                         return otherWs->getNextRoom() == ws;
                                                                     });
                                             });

        if (firstRoomIter != m_activeLine.end())
        {
            m_firstRoom = *firstRoomIter;
        }
        else
        {
            std::cerr << "Error: No valid first room found." << std::endl;
        }

        m_cntPatientRecord = g_pending.size();
    }

    void HospitalManager::reorderRooms()
    {
        std::vector<ServiceRoom*> orderedRooms;

        ServiceRoom*currentRoom = m_firstRoom;

        while (currentRoom != nullptr)
        {
            orderedRooms.push_back(currentRoom);
            currentRoom = currentRoom->getNextRoom();
        }
        m_activeLine = orderedRooms;
    }
    bool HospitalManager::run(ostream &os)
    {
        static size_t counter = 0; // number of times function called

        os << "Line Manager Iteration: " << ++counter << endl;

        // moves the room at the front of the g_pending queue to the m_firstRoom and remove it from the queue.
        if (!g_pending.empty())
        {
            (*m_firstRoom) += std::move(g_pending.front());
            g_pending.pop_front();
        }

        // execute one fill operation for each room
        std::for_each(m_activeLine.begin(), m_activeLine.end(), [&os](ServiceRoom*room)
                      { room->fill(os); });

        // attmpt to move each room
        std::for_each(m_activeLine.begin(), m_activeLine.end(), [](ServiceRoom*room)
                      { room->attemptToMoveRecord(); });

        // find if customer rooms have been filled

        bool roomsFilled = std::all_of(m_activeLine.begin(), m_activeLine.end(), [](ServiceRoom*room)
                                        { return std::all_of(room->m_records.begin(), room->m_records.end(), [](const PatientRecord &room)
                                                             { return room.isRecordFilled(); }); });

        roomsFilled = roomsFilled && g_pending.empty() &&
                       (g_completed.size() + g_incomplete.size() == m_cntPatientRecord);

        return roomsFilled;
    };

    void HospitalManager::display(ostream &os) const
    {
        // displays all active rooms on the assembly line in their current room
        std::for_each(m_activeLine.begin(), m_activeLine.end(), [&](ServiceRoom*room)
                      { room->display(std::cout); });
    }

    HospitalManager::~HospitalManager()
    {

        m_activeLine.clear();
    }
}