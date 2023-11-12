//
// Created by Aseem Baranwal on 11/12/23.
//


#include "Reservation.h"

Reservation::Reservation() {}

Reservation::Reservation(int userID, int priority) {
    this->userID = userID;
    this->priority = priority;
    this->timestamp = std::chrono::system_clock::now();
}
