#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join

// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);
void out(string mode, int w);
void st();

// Global variables
RegisterList *registerList;  // holding the list of registers
QueueList *doneList;         // holding the list of customers served
QueueList *singleQueue;      // holding customers in a single virtual queue
double expTimeElapsed;  // time elapsed since the beginning of the simulation
// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else if (command == "B") {
      break;
    } else {
      cout << "Invalid operation" << endl;
    }
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }
  st();
  // You have to make sure all dynamically allocated memory is freed
  // before return 0
  return 0;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments." << endl;
    return;
  }
  // Depending on the mode of the simulation (single or multiple),
  expTimeElapsed = timeElapsed + expTimeElapsed;
  Customer *p = new Customer(expTimeElapsed, items);

  if (mode == "single") {
    singleQueue->enqueue(p);
    // singleQueue->print();
  }

  else if (mode == "multiple") {
    if (registerList->get_head() != nullptr) {
      while (registerList->calculateMinDepartTimeRegister(expTimeElapsed) !=
                 nullptr &&
             registerList->calculateMinDepartTimeRegister(expTimeElapsed)
                     ->calculateDepartTime() <= expTimeElapsed) {
        cout << "Departed a customer at register ID "
             << registerList->calculateMinDepartTimeRegister(expTimeElapsed)
                    ->get_ID()
             << " at "
             << registerList->calculateMinDepartTimeRegister(expTimeElapsed)
                    ->calculateDepartTime()
             << endl;
        registerList->calculateMinDepartTimeRegister(expTimeElapsed)
            ->departCustomer(doneList);
      }
    }
    cout << "A customer entered" << endl;
    cout << "Queued a customer with quickest register "
         << registerList->get_min_items_register()->get_ID() << endl;
    registerList->get_min_items_register()->get_queue_list()->enqueue(p);

    // registerList->print();
  }
  // add the customer to the single queue or to the register with
  // fewest items
  // take out the Customer that needs to go
  out(mode, 3);
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID, i = 0;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is already open
  if (registerList->foundRegister(ID) == true) {
    cout << "Error: register " << ID << " is already open." << endl;
    return;
  }
  // If it's open, print an error message
  expTimeElapsed = timeElapsed + expTimeElapsed;
  Register *p = new Register(ID, secPerItem, setupTime, expTimeElapsed);
  registerList->enqueue(p);
  cout << "Opened register " << ID << endl;
  if (mode == "single") {
    if (singleQueue->get_head() != nullptr) {
      cout << "Queued a customer with free register " << p->get_ID() << endl;
      p->get_queue_list()->enqueue(singleQueue->get_head());
      p->get_queue_list()->get_head()->set_next(nullptr);
      singleQueue->dequeue();
      i = 1;
    }
  }
  out(mode, i);
  // Otherwise, open the register
  // If we were simulating a single queue,
  // and there were customers in line, then
  // assign a customer to the new register
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  expTimeElapsed = timeElapsed + expTimeElapsed;

  if (registerList->foundRegister(ID) == true) {
    out(mode, 0);
    cout << "Closed register " << ID << endl;
    registerList->dequeue(ID);
  } else {
    cout << "Error: register " << ID << " is not open" << endl;
  }

  // If it is open dequeue it and free it's memory
  // Otherwise, print an error message
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}

void out(string mode, int w) {
  int i = 0;
  if (mode == "single" && registerList->get_head() != nullptr) {
    while (registerList->get_free_register() != nullptr &&
           singleQueue->get_head() != nullptr) {
      i = 1;
      if (w == 3) cout << "A customer entered" << endl;
      cout << "Queued a customer with free register "
           << registerList->get_free_register()->get_ID() << endl;

      registerList->get_free_register()->get_queue_list()->enqueue(
          singleQueue->get_head());
      singleQueue->dequeue();
    }

    while (registerList->calculateMinDepartTimeRegister(expTimeElapsed) !=
               nullptr &&
           registerList->calculateMinDepartTimeRegister(expTimeElapsed)
                   ->calculateDepartTime() <= expTimeElapsed) {
      cout << "Departed a customer at register ID "
           << registerList->calculateMinDepartTimeRegister(expTimeElapsed)
                  ->get_ID()
           << " at "
           << registerList->calculateMinDepartTimeRegister(expTimeElapsed)
                  ->calculateDepartTime()
           << endl;
      registerList->calculateMinDepartTimeRegister(expTimeElapsed)
          ->departCustomer(doneList);

      if (singleQueue->get_head() != nullptr) {
        if (w == 3 && singleQueue->get_head()->get_next() == nullptr)
          cout << "A customer entered" << endl;
        cout << "Queued a customer with free register "
             << registerList->get_free_register()->get_ID() << endl;
        registerList->get_free_register()->get_queue_list()->enqueue(
            singleQueue->get_head());
        singleQueue->dequeue();
      }

      i = 2;
    }

    if (registerList->get_free_register() == nullptr && i == 0 && w != 1) {
      cout << "A customer entered" << endl;
      cout << "No free registers" << endl;
    }
  }
  if (mode == "multiple" && registerList->get_head() != nullptr && w != 3) {
    while (registerList->calculateMinDepartTimeRegister(expTimeElapsed) !=
               nullptr &&
           registerList->calculateMinDepartTimeRegister(expTimeElapsed)
                   ->calculateDepartTime() <= expTimeElapsed) {
      cout << "Departed a customer at register ID "
           << registerList->calculateMinDepartTimeRegister(expTimeElapsed)
                  ->get_ID()
           << " at "
           << registerList->calculateMinDepartTimeRegister(expTimeElapsed)
                  ->calculateDepartTime()
           << endl;
      registerList->calculateMinDepartTimeRegister(expTimeElapsed)
          ->departCustomer(doneList);
    }
  }
}

void st() {
  double max = 0, avg = 0, finish = 0, dev = 0, i = 0;
  ;
  Customer *p = doneList->get_head();
  while (p->get_next() != nullptr) {
    if ((p->get_departureTime() - p->get_arrivalTime()) > max) {
      max = p->get_departureTime() - p->get_arrivalTime();
    }
    i++;
    avg = avg + p->get_departureTime() - p->get_arrivalTime();
    p = p->get_next();
  }

  if ((p->get_departureTime() - p->get_arrivalTime()) > max) {
    max = p->get_departureTime() - p->get_arrivalTime();
  }
  i++;
  avg = avg + p->get_departureTime() - p->get_arrivalTime();
  finish = p->get_departureTime();
  avg = avg / i;

  p = doneList->get_head();

  while (p->get_next() != nullptr) {
    dev = dev + pow(p->get_departureTime() - p->get_arrivalTime() - avg, 2);
    p = p->get_next();
  }
  dev = sqrt((dev +pow(p->get_departureTime() - p->get_arrivalTime() - avg, 2)) / i);
  cout << "Finished at time " << expTimeElapsed << " Statistics:" << endl;
  cout << "Maximum wait time: " << max << endl;
  cout << "Average wait time: " << avg << endl;
  cout << "Standard Deviation of wait time: " << dev << endl;
}