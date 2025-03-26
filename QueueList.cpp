#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {}

Customer* QueueList::get_head() { return head; }

void QueueList::enqueue(Customer* customer) {
  Customer* n=new Customer(customer->get_arrivalTime(), customer->get_numOfItems());
  n->set_departureTime(customer->get_departureTime());
  Customer* p = head;
  if (head != nullptr) {
    while (p->get_next() != nullptr){
      p = p->get_next();
    }

    p->set_next(n);
    p = p->get_next();
    p->set_next(nullptr);
  } else {
    head = n;
  }

  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head
}

Customer* QueueList::dequeue() {
  Customer* p = head;
  head = p->get_next();
  delete p;
  return head;
  // remove a customer from the head of the queue
  // and return a pointer to it
}

int QueueList::get_items() {
  Customer* p = head;
  int items = 0;
  if (head == nullptr) {
    return 0;
  } else {
    do {
      items = items + p->get_numOfItems();
      p=p->get_next();
    } while (p != nullptr);
  }
  return items;
  // count total number of items each customer in the queue has
}

bool QueueList::empty_queue() {
  Customer* gone;
  Customer* p = head;
  if (head == nullptr) {
    return false;
  } else {
    do {
      gone = p;
      p = p->get_next();
      delete gone;
    } while (p != nullptr);
    return true;
  }
  // if the queue is empty, return false
  // if the queue is not empty, delete all customers
  // and return true
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
