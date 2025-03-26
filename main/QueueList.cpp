#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {
  
}

Customer* QueueList::get_head() 
{  
    return head;
}

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head

    //

    if (head == nullptr) head = customer;
    else {
        Customer* p = head;
        while (p->get_next() != nullptr)  p = p->get_next();
        p->set_next(customer);
        
        customer->set_next(nullptr);
    }
     
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue 
  // and return a pointer to it
    
    if (head == nullptr) return  nullptr;

    Customer *p = head;
    head = head->get_next();

    p->set_next(nullptr);
    return p;
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has
    int count = 0;
    Customer * p = head;
  
    if (head == nullptr) return 0;

    do{
        count+=p->get_numOfItems();
        p = p->get_next();
    } while (p != nullptr);

    return count;
}

bool QueueList::empty_queue() {
  // if the queue is empty, return false
  // if the queue is not empty, delete all customers
  // and return true
    if(head == nullptr) 
        return false;
    else
    {
        Customer* p = head;
        Customer* t = head;

        while (p->get_next() != nullptr) {
            t= p->get_next();
            delete p;
            p=t;
        }
    }

    head = nullptr;
    return true;  
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
