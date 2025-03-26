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
// Customer


// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);

// Global variables
RegisterList *registerList; // holding the list of registers
QueueList *doneList; // holding the list of customers served
QueueList *singleQueue; // holding customers in a single virtual queue
double expTimeElapsed; // time elapsed since the beginning of the simulation

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

// Customer
void departureCustomer(double expTimeElapsed);  // departure
void deCustomer(double exTimeElapsed, string mode);   // departureTime <   expTimeElapsed
void printStatistics(QueueList* deCustomerList, string mode);  //Statistics
void freeMemory( );  //free all Memory

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

    } else if (command == "^D") {

        printStatistics(doneList, mode);
        freeMemory();
        return 0;
    }
    else {
      cout << "Invalid operation" << endl;
    }

    cout << "> ";  // Prompt for input
    getline(cin, line);
  }

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
next:
  getline(cin, mode);

  if (mode == "single") {
      cout << "Simulating a single queue ..." << endl;
  }
  else if (mode == "multiple") {
      cout << "Simulating multiple queues ..." << endl;
  }
  else {
      //cout <<">";
      //goto next;
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
  // add the customer to the single queue or to the register with
  // fewest items

  double entryTime = timeElapsed + expTimeElapsed;  
  expTimeElapsed = entryTime;

  deCustomer(expTimeElapsed, mode);

  Customer* newCustomer = new  Customer(entryTime,items );
  cout << "A customer entered" << endl;

  if (mode == "single")
  {

      Register* freeRegister = registerList->get_free_register();
      if (freeRegister != nullptr && singleQueue->get_head() == nullptr) {

          QueueList* freeQueue = freeRegister->get_queue_list();
          freeQueue->enqueue(newCustomer);

          int ID = freeRegister->get_ID();   
          
          cout << "Queued a customer with free register " << ID << endl;
      }
      else {    
          singleQueue->enqueue(newCustomer);
          cout << "No free registers" << endl;
      }
  }

  // add the customer to the register with fewest items

  if (mode == "multiple")
  {
      // include   pCustomer!=nullptr  and   pCustomer==nullptr   

      if (registerList->get_size() > 0 ) {
          //find free Register
          Register* freeRegister = registerList->get_free_register();
          if (freeRegister != nullptr) {

              QueueList* freeQueue = freeRegister->get_queue_list();
              freeQueue->enqueue(newCustomer);

              int ID = freeRegister->get_ID();
              cout << "Queued a customer with quickest register " << ID << endl;
          }
          else {               
              //find min items  Register

              Register* minItems_Register = registerList->get_min_items_register(); 
              QueueList* pCustomerQueue = minItems_Register->get_queue_list();

              pCustomerQueue->enqueue(newCustomer);

              int ID = minItems_Register->get_ID();
              cout << "Queued a customer with quickest register " << ID << endl;
          }
      }
      else {   
          cout << "No  registers opened " << endl;   ////////////
      }
  }

  //deCustomer(expTimeElapsed, mode);

}

void deCustomer(double exTimeElapsed, string mode)    // departureTime <   expTimeElapsed
{
    Register* minDepartTimeRegister = nullptr;
    Customer* departureCustomer = nullptr;
    Customer* headCustomer = nullptr;

    QueueList* pCustomerQueue = nullptr;
    double departureTime = 0;
    int ID = 0;

    do {
        minDepartTimeRegister = registerList->calculateMinDepartTimeRegister(exTimeElapsed);

        if (minDepartTimeRegister != nullptr) {

            //departureCustomer = minDepartTimeRegister->get_queue_list()->get_head();
            pCustomerQueue = minDepartTimeRegister->get_queue_list();
            departureCustomer = pCustomerQueue->dequeue();

            ID = minDepartTimeRegister->get_ID();
            departureTime = departureCustomer->get_departureTime();
            minDepartTimeRegister->set_availableTime(departureTime); // next Customer's availableTime

            cout << "Departed a customer at register ID " << ID << " at " << departureTime << endl;
            doneList->enqueue(departureCustomer);

            if (mode == "single") {
                headCustomer = singleQueue->get_head();
                if (headCustomer != nullptr) {
                    headCustomer = singleQueue->dequeue();
                    pCustomerQueue->enqueue(headCustomer);

                    cout << "Queued a customer with free register " << ID << endl;                    
                }
            }
        }
    } while (minDepartTimeRegister != nullptr);
}

void freeMemory() {

    Register* p = registerList->get_head();
    Register* r = nullptr;

    //free registerList
    while (p != nullptr) {
        r = p;
        p->get_queue_list()->empty_queue();

        p = p->get_next();
        delete r;
    }

    doneList->empty_queue();
    singleQueue->empty_queue();

    if (registerList != nullptr) delete registerList;
    if (doneList != nullptr) delete doneList;
    if (singleQueue != nullptr) delete  singleQueue;
}


void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {

    openRegister(lineStream, mode);

  } else if (operation == "close") {

    closeRegister(lineStream, mode);
  } 
  else {
    cout << "Invalid operation" << endl;
  }
}

void printStatistics(QueueList* deCustomerList, string mode) {
    //expTimeElapsed
    Customer* deCustomer = deCustomerList->get_head();
    double maxWaiTime = 0;
    double avgWaiTime = 0;
    double deviationTime = 0;
    //double deviationTime = 0;

    double X = 0;
    int N = 0;

    while (deCustomer != nullptr) {
        N++;
        X = deCustomer->get_departureTime() - deCustomer->get_arrivalTime();
        
        if (maxWaiTime < X)     maxWaiTime = X;
        avgWaiTime += X;

        deCustomer = deCustomer->get_next();
    }

    if( N!=0 ) avgWaiTime = avgWaiTime / N;

    deCustomer = doneList->get_head();

    N = 0;
    while (deCustomer != nullptr) {
        N++;
        X = deCustomer->get_departureTime() - deCustomer->get_arrivalTime();
        deviationTime += (X- avgWaiTime) * (X - avgWaiTime);
        
        deCustomer = deCustomer->get_next();
    }
        
    if (N != 0)  deviationTime = sqrt(deviationTime / N);

    if (mode == "multiple")  cout << "Finished at time " << expTimeElapsed << " Statistics:" << endl;
    else {
        cout << "Finished at time " << expTimeElapsed << endl;
        cout << "Statistics:" << endl;
    }

    cout << "Maximum wait time : "<< maxWaiTime << endl;
    cout << "Average wait time : " << avgWaiTime <<endl;
    cout << "Standard Deviation of wait time : " << deviationTime <<endl;
}


void openRegister(stringstream &lineStream, string mode) {

  int ID;
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

  //departureCustomer(expTimeElapsed);
  //deCustomer(expTimeElapsed, mode);

  // Check if the register is already open
// If it's open, print an error message

  //registerList->

  bool isOpen = registerList->foundRegister(ID);

  if (isOpen) {
      cout << "Error: register "<<ID<<" is already open" << endl;
      return;
  }
  else   // Otherwise, open the register
  {  
      double overheadPerCustomer = setupTime;
      double entryTime = timeElapsed + expTimeElapsed;

      expTimeElapsed = entryTime;


      deCustomer(expTimeElapsed, mode);

      Register* newRegister = new Register(ID, secPerItem, overheadPerCustomer, entryTime);
      newRegister->set_availableTime(entryTime);

      //newRegister->set_next(nullptr);  //类中enqueue（）已经实现
      registerList->enqueue(newRegister);
      
      cout << "Opened register " << ID << endl;

        // If we were simulating a single queue, 
        // and there were customers in line, then 
        // assign a customer to the new register

      if (mode == "single")
      {
          Customer* pCustomer = singleQueue->dequeue();

          if (pCustomer != nullptr) {
              newRegister->get_queue_list()->enqueue(pCustomer);
              cout << "Queued a customer with free register " << ID << endl;
          }
      }
  }


  // Check if the register is already open
  // If it's open, print an error message
  // Otherwise, open the register
  // If we were simulating a single queue, 
  // and there were customers in line, then 
  // assign a customer to the new register
  
}



void closeRegister(stringstream& lineStream, string mode) {
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
    // If it is open dequeue it and free it's memory

    double closeTime = timeElapsed + expTimeElapsed;
    expTimeElapsed = closeTime;

    //departureCustomer(expTimeElapsed);
    deCustomer(expTimeElapsed, mode);

    //newRegister->set_availableTime(closeTime);

    bool isOpen = registerList->foundRegister(ID);
    if (isOpen) {
        Register* deRegister = registerList->dequeue(ID);
        if (deRegister != nullptr) {

            delete deRegister;

            cout << "Closed register " << ID << endl;
        }
    }        
    else  // Otherwise, print an error message 
    {
        cout << "Error: register " << ID << " is not open." << endl;  //关闭不存在的收银台 
    }

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
