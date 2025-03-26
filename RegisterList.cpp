#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  Register* p = head;
  Register* kill;
  if (head == nullptr) {
    return;
  } else {
    do {
      kill = p;
      p = p->get_next();
      delete kill;
      size = size - 1;
    } while (p != nullptr);
  }
  // Delete all registers in the list
}

Register* RegisterList::get_head() { return head; }

int RegisterList::get_size() {
  return size;
  // return number of registers
}

Register* RegisterList::get_min_items_register() {
  Register* p = head;
  int min = 99999999;
  
  if (head == nullptr) {
    return nullptr;
  } 
  else if(head->get_queue_list()->get_head()==nullptr) {
    return head;
  }
  else{
    do {
      if(p->get_queue_list()->get_head() == nullptr){
        return p;
      }
      if (min > p->get_queue_list()->get_items()) {
        min = p->get_queue_list()->get_items();
      }
      p = p->get_next();
    } while (p != nullptr);
    p = head;
    do {
      if (min == p->get_queue_list()->get_items()) {
        return p;
      }
      p = p->get_next();
    } while (p != nullptr);
  }

  // loop all registers to find the register with least number of items
}

Register* RegisterList::get_free_register() {
  Register* p = head;
  if (head == nullptr) {
    return nullptr;
  } else {
    do {
      if (p->get_queue_list()->get_head() == nullptr) {
        return p;
      }
      p = p->get_next();
    } while (p != nullptr);
  }
  return nullptr;
  // return the register with no customers
  // if all registers are occupied, return nullptr
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size
  Register* p = head;
  if (head == nullptr) {
    head = newRegister;
    head->set_next(nullptr);
    size++;
  } else {
    while (p->get_next() != nullptr) {
      p = p->get_next();
    }
    p->set_next(newRegister);
    newRegister->set_next(nullptr);
    size++;
  }
}

bool RegisterList::foundRegister(int ID) {
  Register* p = head;
  if (head == nullptr) {
    return false;
  } else {
    do {
      if (p->get_ID() == ID) {
        return true;
      }
      p = p->get_next();
    } while (p != nullptr);
    return false;
  }
  // look for a register with the given ID
  // return true if found, false otherwise
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID
  Register* p = head;
  Register* r = head;
  if (head == nullptr) {
    return nullptr;
  } else {
    do {
      if (p->get_ID() == ID) {
        if (p == head) {
          head=p->get_next();
          p->set_next(nullptr);
          delete p;
          size--;
          return head;
        }
        r->set_next(p->get_next());
        p->set_next(nullptr);
        delete p;
        size--;
        return r;
      }

      r = p;
      p = p->get_next();
    } while (p != nullptr);

    return nullptr;
  }
  // return the dequeued register
  // return nullptr if register was not found
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  Register* p = head;
  double min = 999999999;
  int i=0;
  if (p == nullptr) {
    return nullptr;
  } else {
    do {
      if (p->get_queue_list()->get_head()== nullptr) {
        i++;
      }
      if(i == size){
        return nullptr;
      }
      p = p->get_next();
    } while (p != nullptr);

    p = head;


    do {
      if (min > p->calculateDepartTime() && p->get_queue_list()->get_head()!= nullptr) {
        min = p->calculateDepartTime();
      }
      p = p->get_next();
    } while (p != nullptr);
    p = head;
    do {
      if (min == p->calculateDepartTime()&& p->get_queue_list()->get_head()!= nullptr) {
        return p;
      }
      p = p->get_next();
    } while (p != nullptr);
  }

  // return the register with minimum time of departure of its customer
  
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
