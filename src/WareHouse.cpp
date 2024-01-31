#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "WareHouse.h"
#include "Action.h"
#include "Customer.h"
#include "Volunteer.h"
#include <sstream>

using namespace std;

std::vector<std::string> split(const std::string& s, char delimiter);

WareHouse::WareHouse(const string &configFilePath)
{
    std::ifstream input( configFilePath );
    for( std::string line; getline( input, line );)
    {
        char delimiter = ' ';
        std::vector<std::string> substrings = split(line, delimiter);
        
        //if(line.find("customer") != std::string::npos)
        if(substrings[0] == "customer")
        {
            AddCustomer(substrings[1],substrings[2],std::stoi(substrings[3]),std::stoi(substrings[4])).act(*this);
        }
        else if(substrings[0] == "Volunteer")
        {
            Volunteer *volunteer;
           if(substrings[2] == "collector")
           {
            volunteer=new CollectorVolunteer(volunteerCounter,substrings[1],std::stoi(substrings[3]));
           }
           else  if(substrings[2] == "limited_collector")
           {
            volunteer=new LimitedCollectorVolunteer(volunteerCounter,substrings[1],std::stoi(substrings[3]),std::stoi(substrings[4]));
           }
           else  if(substrings[2] == "driver")
           {
            volunteer=new DriverVolunteer(volunteerCounter,substrings[1],std::stoi(substrings[3]),std::stoi(substrings[4]));
           }
           else  if(substrings[2] == "limited_driver")
           {
            volunteer=new LimitedDriverVolunteer(volunteerCounter,substrings[1],std::stoi(substrings[3]),std::stoi(substrings[4]),std::stoi(substrings[5]));
           }
            volunteers.push_back(volunteer);
            volunteerCounter++;
        }
    }
}

void WareHouse::start()
{
    cout << "WareHouse is now open!" << endl;
    while(isOpen)
    {
        std::string input;
        std::getline(std::cin, input);
        char delimiter = ' ';
        std::vector<std::string> substrings = split(input, delimiter);
        if(substrings[0] == "order") ///ok
        {
            AddOrder* action = new AddOrder(std::stoi(substrings[1])); 
            action->act(*this);
        }
        else if(substrings[0] == "customerStatus")///ok
        {
            PrintCustomerStatus* action = new PrintCustomerStatus(std::stoi(substrings[1]));
            action->act(*this);
        }
        else if(substrings[0] == "step") ///ok
        {
            SimulateStep* action = new SimulateStep(std::stoi(substrings[1]));
            action->act(*this);
        }
        else if(substrings[0] == "orderStatus") ///ok
        {
            PrintOrderStatus* action = new PrintOrderStatus(std::stoi(substrings[1]));
            action->act(*this);
        }
        else if(substrings[0] == "volunteerStatus")///ok
        {
            PrintVolunteerStatus* action = new PrintVolunteerStatus(std::stoi(substrings[1]));
            action->act(*this);
        }
        else if(substrings[0] == "customer")///ok
        {
            //customer <customer_name> <customer_type> <customer_distance> <max_orders> 
            AddCustomer* action = new AddCustomer(substrings[1], substrings[2], stoi(substrings[3]), stoi(substrings[4]));
        }
        else if(substrings[0] == "restore")///ok
        {
            RestoreWareHouse* action = new RestoreWareHouse;
            action->act(*this);
        }
        else if(substrings[0] == "close")///ok
        {
            Close* action = new Close();
            action->act(*this);
        }
        else if(substrings[0] == "log")///ok
        {
            PrintActionsLog* action = new PrintActionsLog;
            action->act(*this);
        }
        else if(substrings[0] == "backup")///ok
        {
            BackupWareHouse* action = new BackupWareHouse;
            action->act(*this);
        } 
        else
        {
            cout << "Error: " << "Invalid input" << endl;
        }
    }

}

void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);  
}

void WareHouse::addCustomer(Customer *customer)
{
    customers.push_back(customer);
    customerCounter++;
}

void WareHouse::addVolunteer(Volunteer *Volunteer)
{
    volunteers.push_back(Volunteer);
    volunteerCounter++;
}


Customer &WareHouse::getCustomer(int customerId) const
{
    for (const auto &customer : customers) {
        if (customer->getId() == customerId) {
            return *customer;
        }
        
    }
    return ;
}


Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for(auto& vol : volunteers)
    {
        if(vol->getId()== volunteerId)
        {
            return *vol;      
        }
    }
    return ;
}

Order &WareHouse::getOrder(int orderId) const
{
    for (const auto &order : pendingOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (const auto &order : inProcessOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (const auto &order : completedOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    return ;//check about that with tom after finnish
}

const vector<BaseAction *> &WareHouse::getActions() const
{
    return this->actionsLog;
}

void WareHouse::close()
{

    for (const auto &order : pendingOrders) {
        std::cout << "OrderID: " << order->getId() << " , CustomerID: " << order->getCustomerId() << " , OrderStatus: " << Order::orderStatusToString(order->getStatus()) << std::endl;
    }
    for (const auto &order : inProcessOrders) {
        std::cout << "OrderID: " << order->getId() << " , CustomerID: " << order->getCustomerId() << " , OrderStatus: " << Order::orderStatusToString(order->getStatus()) << std::endl;
    }
    for (const auto &order : completedOrders) {
        std::cout << "OrderID: " << order->getId() << " , CustomerID: " << order->getCustomerId() << " , OrderStatus: " << Order::orderStatusToString(order->getStatus()) << std::endl;
    }


    //free meomry
    this->~WareHouse();
    this->isOpen = false;

}

void WareHouse::open()
{
    this->isOpen = true;
}

int &WareHouse::getCustomerCounter()
{
    return customerCounter;
}

int &WareHouse::getVolunteerCounter()
{
    return volunteerCounter;
}

int WareHouse::getOrderCounter()
{
    return pendingOrders.size() + inProcessOrders.size() + completedOrders.size();
}

bool WareHouse::getIsOpen()
{
    return isOpen;
}

// Destructor
WareHouse::~WareHouse() {
    for (auto order : pendingOrders) {
        delete order;
    }
    for (auto order : inProcessOrders) {
        delete order;
    }
    for (auto order : completedOrders) {
        delete order;
    }
    for (auto volunteer : volunteers) {
        delete volunteer;
    }
    for (auto customer : customers) {
        delete customer;
    }
    for (auto action : actionsLog) {
        delete action;
    }
}

// Copy constructor

WareHouse::WareHouse(const WareHouse& other):
customerCounter(other.customerCounter) , volunteerCounter(other.volunteerCounter) , isOpen(other.isOpen)
{
    if (this != &other) {
        //check free memory
        // Copy each dynamically allocated resource
        for (const auto order : other.pendingOrders) {
            pendingOrders.push_back(order->clone());
        }
        for(const auto order : other.inProcessOrders)
        {
            inProcessOrders.push_back(order->clone());
        }
        for(const auto order : other.completedOrders)
        {
            completedOrders.push_back(order->clone());
        }
        for (const auto customer : other.customers) {
            customers.push_back(customer->clone());
        }
        for (const auto volunteer : other.volunteers) {
            volunteers.push_back(volunteer->clone());
        }
        for (const auto action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }

    }
}


// Copy assignment operator
WareHouse& WareHouse::operator=(const WareHouse& other){
    if (this != &other) {
        // Free existing resources
        for (auto order : pendingOrders) {
            delete order;
        }
        for (auto order : inProcessOrders) {
            delete order;
        }
        for (auto order : completedOrders) {
            delete order;
        }
        for (auto volunteer : volunteers) {
            delete volunteer;
        }
         for (auto customer : customers) {
            delete customer;
        }
        for (auto action : actionsLog) {
            delete action;
        }

        // Copy each dynamically allocated resource
        for (const auto order : other.pendingOrders) {
            pendingOrders.push_back(order->clone());
        }
        for(const auto order : other.inProcessOrders)
        {
            inProcessOrders.push_back(order->clone());
        }
        for(const auto order : other.completedOrders)
        {
            completedOrders.push_back(order->clone());
        }
        for (const auto customer : other.customers) {
            customers.push_back(customer->clone());
        }
        for (const auto volunteer : other.volunteers) {
            volunteers.push_back(volunteer->clone());
        }
        for (const auto action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }
        this->customerCounter = other.customerCounter;
        this->volunteerCounter = other.volunteerCounter;
        this->isOpen = other.isOpen;
        }
    return *this;
}

// Move constructor
WareHouse::WareHouse(WareHouse&& other) noexcept:
customerCounter(other.customerCounter) , volunteerCounter(other.volunteerCounter) , isOpen(other.isOpen)  {
    if (this != &other) {
        //check free memory
        // Copy each dynamically allocated resource
        for (const auto order : other.pendingOrders) {
            pendingOrders.push_back(order);
        }
        for(const auto order : other.inProcessOrders)
        {
            inProcessOrders.push_back(order);
        }
        for(const auto order : other.completedOrders)
        {
            completedOrders.push_back(order);
        }
        for (const auto customer : other.customers) {
            customers.push_back(customer);
        }
        for (const auto volunteer : other.volunteers) {
            volunteers.push_back(volunteer);
        }
        for (const auto action : other.actionsLog) {
            actionsLog.push_back(action);
        }
         //change memory in the other warehouse
        for (auto order : other.pendingOrders) {
            order=nullptr;
        }
        for(auto order : other.inProcessOrders)
        {
            order=nullptr;
        }
        for(auto order : other.completedOrders)
        {
            order=nullptr;
        }
        for (auto customer : other.customers) {
            customer=nullptr;
        }
        for (auto volunteer : other.volunteers) {
            volunteer=nullptr;
        }
        for (auto action : other.actionsLog) {
            action=nullptr;
        }
        other.customerCounter=0;
        other.volunteerCounter=0;
        other.isOpen=false;
    }

   
}

// Move assignment operator
WareHouse& WareHouse::operator=(WareHouse&& other) noexcept {
    if (this != &other) {
        //check free memory
        // Copy each dynamically allocated resource
        for (const auto order : other.pendingOrders) {
            pendingOrders.push_back(order);
        }
        for(const auto order : other.inProcessOrders)
        {
            inProcessOrders.push_back(order);
        }
        for(const auto order : other.completedOrders)
        {
            completedOrders.push_back(order);
        }
        for (const auto customer : other.customers) {
            customers.push_back(customer);
        }
        for (const auto volunteer : other.volunteers) {
            volunteers.push_back(volunteer);
        }
        for (const auto action : other.actionsLog) {
            actionsLog.push_back(action);
        }
         //change memory in the other warehouse
        for (auto order : other.pendingOrders) {
            order=nullptr;
        }
        for(auto order : other.inProcessOrders)
        {
            order=nullptr;
        }
        for(auto order : other.completedOrders)
        {
            order=nullptr;
        }
        for (auto customer : other.customers) {
            customer=nullptr;
        }
        for (auto volunteer : other.volunteers) {
            volunteer=nullptr;
        }
        for (auto action : other.actionsLog) {
            action=nullptr;
        }
        this->customerCounter = other.customerCounter;
        this->volunteerCounter = other.volunteerCounter;
        this->isOpen = other.isOpen;
        other.customerCounter=0;
        other.volunteerCounter=0;
        other.isOpen=false;
    }
    return *this;

}


std::vector<std::string> split(const std::string& s, char delimiter) 
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) 
    {
        tokens.push_back(token);
    }
    return tokens;
}


void WareHouse::simulationAct()
{

    //first step
    std::vector<Order*> RemovePendingOrders;
    for (auto& order : pendingOrders) {
        if(order->getStatus()==OrderStatus::PENDING)
        {
            
            for(auto& vol : volunteers)
            {
                if(vol->canTakeOrder(*order))
                {
                    inProcessOrders.push_back(order);
                    order->setStatus(OrderStatus::COLLECTING);
                    vol->acceptOrder(*order);
                    order->setCollectorId(vol->getId());
                    RemovePendingOrders.push_back(order);
                }
            }
        }
        else
        {
            for(auto& vol : volunteers)
            {
                if(vol->canTakeOrder(*order))
                {
                    inProcessOrders.push_back(order);
                    order->setStatus(OrderStatus::DELIVERING);
                    vol->acceptOrder(*order);
                    order->setDriverId(vol->getId());
                    RemovePendingOrders.push_back(order);
                }
            }
        }
    }
    //Remove orders from there past status
    for (auto& order : RemovePendingOrders) {
        pendingOrders.erase(std::remove(pendingOrders.begin(), pendingOrders.end(), order), pendingOrders.end());
    } 



    //second step
    for(auto& vol : volunteers)
    {
        vol->step();
    }



    //third step
    std::vector<Order*> ordersToRemove;
      
    for(auto& vol : volunteers)
    {
        if(vol->getActiveOrderId()==-1 && vol->getCompletedOrderId()!=-1)
        {
            int orderId=vol->getCompletedOrderId();
            for (const auto& order : inProcessOrders)
            {
                if(orderId==order->getId())
                {
                    if(order->getStatus()==OrderStatus::COLLECTING)
                    {
                        //when i finnish should i put ordercollector id -1?
                        pendingOrders.push_back(order);
                        ordersToRemove.push_back(order);
                    }
                    else if(order->getStatus()==OrderStatus::DELIVERING)
                    {
                        completedOrders.push_back(order);
                        ordersToRemove.push_back(order);
                    }
                }
            }
        }
    }
    //Remove orders from there past status
    for (auto& order : ordersToRemove) {
        inProcessOrders.erase(std::remove(inProcessOrders.begin(), inProcessOrders.end(), order), inProcessOrders.end());
    } 



    //fourth step
    for(auto& vol : volunteers)
    {
        if(!(vol->hasOrdersLeft()))
        {
            volunteers.erase(std::remove(volunteers.begin(), volunteers.end(), vol), volunteers.end());
        }
    }

    /*// Step 4: Removing Volunteers with No Orders Left
    volunteers.erase(std::remove_if(volunteers.begin(), volunteers.end(),
        [](const Volunteer* vol) {
            return !vol->hasOrdersLeft();
        }), volunteers.end());*/
}

bool WareHouse::checkExistOrder(int orderId)
{
    for (const auto &order : pendingOrders) {
        if (order->getId() == orderId) {
            return true;
        }
    }
    for (const auto &order : inProcessOrders) {
        if (order->getId() == orderId) {
            return true;
        }
    }
    for (const auto &order : completedOrders) {
        if (order->getId() == orderId) {
            return true;
        }
    }
    return false;
}

bool WareHouse::PrintExistCustomer(int customerId)
{
    for (const auto &customer : customers) {
        if (customer->getId() == customerId) {
            cout <<"CustomerID: "  << customerId << endl;
            for (const auto &order : pendingOrders) {
                if (order->getCustomerId() == customerId) {
                    cout << "OrderID: " << order->getId()<<"\n" << "OrderStatus: " << Order::orderStatusToString(order->getStatus())<<endl;
                }
            }
            for (const auto &order : inProcessOrders) {
                if (order->getCustomerId() == customerId) {
                    cout << "OrderID: " << order->getId()<<"\n" << "OrderStatus: " << Order::orderStatusToString(order->getStatus())<<endl;
                }
            }
            for (const auto &order : completedOrders) {
                if (order->getCustomerId() == customerId) {
                    cout << "OrderID: " << order->getId()<<"\n" << "OrderStatus: " << Order::orderStatusToString(order->getStatus())<<endl;
                }
            }
            cout << "numOrdersLeft:" << customer->getNumOrdersLeft() << endl;
            return true;
        }
    }
    return false;
}

bool WareHouse::checkExistVolunteer(int volunteerId)
{
    for(auto& vol : volunteers)
    {
        if(vol->getId()== volunteerId)
        {
            return true;      
        }
    }
    return false;
}

bool WareHouse::checkExistCustomer(string customerName)
{
    for(auto& customer : customers)
    {
        if(customer->getName()== customerName)
        {
            return true;      
        }
    }
    return false;
}


void WareHouse::PrintActions()
{
    for(auto& Action : actionsLog)
    {
        cout << Action->toString() << endl;
    }
}
