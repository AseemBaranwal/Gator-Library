//
// Created by Aseem Baranwal on 11/5/23.
//

#ifndef GATOR_LIBRARY_RESERVATION_H
#define GATOR_LIBRARY_RESERVATION_H

#include <chrono>
#include <ctime>

struct Reservation {
    int userID;
    int priority;
    std::chrono::time_point<std::chrono::system_clock> timestamp;

    Reservation();

    Reservation(int userID, int priority);
};

#endif //GATOR_LIBRARY_RESERVATION_H
