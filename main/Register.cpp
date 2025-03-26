
// Created by Salma Emara on 2023-06-02.
#include "Register.h"

#include <iostream>
using namespace std;

Register::Register(int id, double timePerItem, double overhead,
                   double entryTime) {
  ID = id;
  secPerItem = timePerItem;
  overheadPerCustomer = overhead;
  availableTime = entryTime;
  next = nullptr;           // no other registers yet
  queue = new QueueList();  // no customers in line yet, but has to initialize a
                            // queue
}

Register::~Register() { }

QueueList* Register::get_queue_list() { return  queue; }

Register* Register::get_next() { return  next; }

int Register::get_ID() { return ID; }

double Register::get_secPerItem() { return secPerItem;  }

double Register::get_overheadPerCustomer() 
{    
    return overheadPerCustomer;
}


double Register::get_availableTime() {  

    return  availableTime;
}

void Register::set_next(Register* nextRegister) { 

    next = nextRegister;
}


void Register::set_availableTime(double availableSince) {
  availableTime = availableSince;
}

double Register::calculateDepartTime() {
  // Get the departure time of the first customer in the queue
  // returns -1 if no customer is in the queue

  //  ����ͻ������ѵ�ʱ���� = ����̨��secPerItem���ͻ����� + ����̨��setupTime
  //      1��	����ʱ�� = �˿͵Ĵ���ʱ�� + �˿͵ĵ���ʱ��  ����	//����̨���ú󵽴�
  //      2��	����ʱ�� = �˿͵Ĵ���ʱ�� + ����̨���õ�ʱ�� ����	//����ǰ���  ��һ���ͻ�
  //  availableTime?????????????


    double DepartTime =-1; 
    Customer *head = queue->get_head();

    if (head == nullptr) return DepartTime;

    double arrivalTime = head->get_arrivalTime();
    int nItems = head->get_numOfItems();


    if (availableTime > arrivalTime)
        DepartTime = nItems * secPerItem + availableTime+ overheadPerCustomer;   // arrivalTime ; 
    else
        DepartTime = nItems * secPerItem + arrivalTime+overheadPerCustomer;    // ;

    /*
    if (availableTime < queue->get_head()->get_arrivalTime())
        DepartTime = queue->get_head()->get_numOfItems() * secPerItem + queue->get_head()->get_arrivalTime() + overheadPerCustomer;
    else
        DepartTime = queue->get_head()->get_numOfItems() * secPerItem + queue->get_head()->get_arrivalTime() + overheadPerCustomer;
    */
    return DepartTime;
}

void Register::departCustomer(QueueList* doneList) {
  // dequeue the head, set last dequeue time, add to doneList,
  // update availableTime of the register

    double  DepartTime = calculateDepartTime();
    set_availableTime( DepartTime );

    Customer* pDequeueCustomer = queue->dequeue();

    pDequeueCustomer->set_departureTime(DepartTime);
    doneList->enqueue(pDequeueCustomer);

}

void Register::print() {
  std::cout << "Register ID: " << ID << std::endl;
  std::cout << "Time per item: " << secPerItem << std::endl;
  std::cout << "Overhead per customer: " << overheadPerCustomer << std::endl;
  if (queue->get_head() != nullptr) {
    std::cout << "Queue has customers: \n";
    queue->print();
  }
}
