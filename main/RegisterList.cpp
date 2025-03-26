#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  // Delete all registers in the list
}

Register* RegisterList::get_head() { return head; }

int RegisterList::get_size() { 
  // return number of registers 
    return  size;
}


Register* RegisterList::get_min_items_register() {
  // loop all registers to find the register with least number of items

    Register* t = head;
    Register* minCustomers = nullptr;
    int minItems = 9999999, nItems=0;

    // loop RegisterList   the end of the queue
    // find the register with least number of items

    minItems = t->get_queue_list()->get_items();

    while (t != nullptr)
    {
        nItems = t->get_queue_list()->get_items();

        if (nItems < minItems) {
            minItems = nItems;
            minCustomers = t;            
        }
        t = t->get_next();
    }

    // find the register with least number of items , also is   the closest register to the head of the register list
    t = head;
    while (t != nullptr)
    {
        nItems = t->get_queue_list()->get_items();

        if (nItems == minItems) {
            minCustomers = t;
            break;
        }
        t = t->get_next();
    }

    return minCustomers;
}

Register* RegisterList::get_free_register() {
  // return the register with no customers
  // if all registers are occupied, return nullptr

    Register* t = head;
    Register* noCustomers = nullptr;

    // loop RegisterList   the end of the queue
    while (t != nullptr)
    {
        Customer* p = t->get_queue_list()->get_head();
        if (p == nullptr) {
            noCustomers = t;
            break;  // is   the closest register to the head of the register list
        }
        t = t->get_next();
    }

    return noCustomers;
}

void RegisterList::enqueue(Register* newRegister) {
    // a register is placed at the end of the queue
    // if the register's list is empty, the register becomes the head
    // Assume the next of the newRegister is set to null
    // You will have to increment size 

    newRegister->set_next(nullptr);

    Register* t = head;

    if (t == nullptr) {
        head = newRegister;
    }
    else {
        // move RegisterList   the end of the queue
        while ( t->get_next() != nullptr)   t = t->get_next();
        t->set_next(newRegister);
    }
    size++;  
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise

    Register* t = head;
    bool isFound  = false;
    int nId = 0;

    // loop RegisterList   the end of the queue
    while (t != nullptr)
    {
        nId = t->get_ID();
        if (nId  == ID ) {
            isFound = true;
            break;
        }
        t = t->get_next();
    }

    return  isFound;
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID
  // return the dequeued register
  // return nullptr if register was not found

    Register* t = head;
    Register* p = head;
    Register*  pFound = nullptr;

    int nId = head->get_ID();

    if (nId == ID) {
        pFound = head;
        head = head->get_next();        
    }
    else{
        p = head;
        t=t->get_next();
        while (t != nullptr)
        {
            nId = t->get_ID();
            if (nId == ID) {
                pFound = t;
                p->set_next(t->get_next());
                break;
            }
            p = t;
            t = t->get_next();
        }
    }
    pFound->set_next(nullptr);

    if(size>0)  size--;

    return pFound;
}


Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
    // return the register with minimum time of departure of its customer 
    // if all registers are free, return nullptr
    // expTimeElapsed

    Register* minDepartTimeRegister = nullptr;
    Register* tempRegister = head;
    QueueList* pCustomerQueue = nullptr;
    Customer* pCustomer = nullptr;
    double minTime = 999999999, departureTime = 0;

    if (head == nullptr) return nullptr;

    //minTime = head->get_queue_list()->get_head()->get_departureTime();

    while (tempRegister != nullptr)
    {
        pCustomerQueue = tempRegister->get_queue_list();
        if(pCustomerQueue !=  nullptr ){
            pCustomer = tempRegister->get_queue_list()->get_head();
            if (pCustomer != nullptr) {

                if (pCustomer->get_arrivalTime() >= tempRegister->get_availableTime()) {
                    departureTime = pCustomer->get_numOfItems() * tempRegister->get_secPerItem() + tempRegister->get_overheadPerCustomer() + pCustomer->get_arrivalTime();
                    pCustomer->set_departureTime(departureTime);
                    
                }
                else {
                    departureTime = pCustomer->get_numOfItems() * tempRegister->get_secPerItem() + tempRegister->get_overheadPerCustomer() + tempRegister->get_availableTime();
                    pCustomer->set_departureTime(departureTime);                   
                }

                //departureTime = pCustomer->get_departureTime();
                if (minTime > departureTime)
                {
                    minTime = departureTime;
                    minDepartTimeRegister = tempRegister;
                }
            }
       }
        tempRegister = tempRegister->get_next();
    }

    if (minTime <= expTimeElapsed)     return minDepartTimeRegister;
    else return nullptr;
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
