#include "Action.h"
#include <iostream>
#include "Volunteer.h"
using namespace std;

extern WareHouse* backup;


//Base action status
BaseAction::BaseAction(){}

ActionStatus BaseAction::getStatus() const
{
    return status;
}



void BaseAction::complete()
{
    this->status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    this->status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}


//AddOrder class
AddOrder::AddOrder(int id): customerId(id){}

void AddOrder::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    if(!wareHouse.checkExistCustomer(customerId))
    {
        error("Cannot place this order");
        cout << "Error: " << getErrorMsg() << std::endl; /// the error string should be checked and presented in the main
    }
    else if(wareHouse.getCustomer(customerId).canMakeOrder())
    {
        Order *temp = new Order(wareHouse.getOrderCounter(),customerId, wareHouse.getCustomer(customerId).getCustomerDistance());
        wareHouse.addOrder(temp);
        
        this->orderId = wareHouse.getCustomer(customerId).addOrder(wareHouse.getOrderCounter()+1);
        complete();
    }
    else
    {
        error("Cannot place this order");
        cout << "Error: " << getErrorMsg() << endl;
    }
}
string AddOrder::toString() const 
{
    return "order " + to_string(this->customerId)  + (getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}





//AddCustomer class
AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders):
 customerName(customerName),
  customerType(customerType == "civilian" ? CustomerType::Civilian : CustomerType::Soldier),
   distance(distance),
    maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    if(this->distance < 0)
    {
        error("distance cannot be negative");
        cout << "Error: " << getErrorMsg() << endl;
    }
    else if(this->maxOrders < 0)
    {
        error("max orders cannot be negative");
        cout << "Error: " << getErrorMsg() << endl;
    }
    else
    {
        if(this->customerType == CustomerType::Civilian)
        {
            CivilianCustomer temp = CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
            wareHouse.addCustomer(&temp);
        }
        else
        {
            SoldierCustomer temp = SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
            wareHouse.addCustomer(&temp);
        }
    }
    complete();
}

AddCustomer *AddCustomer::clone() const
{
    AddCustomer* temp = new AddCustomer(customerName, customerType == CustomerType::Civilian ? "civilian" : "soldier", distance, maxOrders);
    if(getStatus() == ActionStatus::COMPLETED)
    {
        temp->complete();
    }
    temp->error(getErrorMsg());
    return temp;
}

string AddCustomer::toString() const
{
    string ret = "AddCustomer  ";
    return ret + "Name: " + customerName + ", Type: " + (customerType == CustomerType::Civilian ? "civilian" : "soldier") + ", Distance: " + to_string(distance) + ", MaxOrders: " + to_string(maxOrders) + ", Status:" + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}



//SimulateStep class
SimulateStep::SimulateStep(int numOfSteps):numOfSteps(numOfSteps){}
void SimulateStep::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    for(int i=0;i<numOfSteps;i++)
    {
        wareHouse.simulationAct();
    }
}

SimulateStep *SimulateStep::clone() const
{
    SimulateStep* temp = new SimulateStep(this->numOfSteps);
    if(getStatus() == ActionStatus::COMPLETED)
    {
        temp->complete();
    }
    temp->error(getErrorMsg());
    return temp;
}

std::string SimulateStep::toString() const
{
    return "simulateStep " + to_string(numOfSteps) + (getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}




//PrintOrderStatus class
PrintOrderStatus::PrintOrderStatus(int id):orderId(id)
{
}
void PrintOrderStatus::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    if(wareHouse.checkExistOrder(orderId))
    {
        Order currentOrder=wareHouse.getOrder(orderId);
        string status =Order::orderStatusToString(currentOrder.getStatus()); 
        cout <<"Order Id: " << orderId << "\n"<< "OrderStatus: " <<  status << "\n"<< "CustomerID: " <<currentOrder.getCustomerId() << endl;

        if(currentOrder.getCollectorId()==-1)
        {
            cout<< "Collector: None" << endl;
        }
        else
        {
            cout<< "Collector: "<<currentOrder.getCollectorId()<<endl;
        }
        if (currentOrder.getDriverId()==-1)
        {
            cout<< "Driver: None" << endl;
        }
        else
        {
            cout<<"Driver: "<< currentOrder.getDriverId()<< endl;
        }
    }
    else
    {
        error("Order doesn’t exist");
        cout << "Error: " << getErrorMsg() << endl;
    }
    complete();
}
PrintOrderStatus *PrintOrderStatus::clone() const
{
    PrintOrderStatus* temp = new PrintOrderStatus(this->orderId);
    if(getStatus() == ActionStatus::COMPLETED)
    {
        temp->complete();
    }
    temp->error(getErrorMsg());
    return temp;
    //return new PrintOrderStatus(*this);

}

string PrintOrderStatus::toString() const
{
 
    return "orderStatus: " + to_string(orderId) + (getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}





//PrintCustomerStatus class
PrintCustomerStatus::PrintCustomerStatus(int customerId):customerId(customerId)
{
}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    if(!(wareHouse.PrintExistCustomer(customerId)))
    {
        error("Customer doesn’t exist");
        cout << "Error: " << getErrorMsg() << endl;
    }
    complete();
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    PrintCustomerStatus* temp = new PrintCustomerStatus(customerId);
    if(getStatus() == ActionStatus::COMPLETED)
    {
        temp->complete();
    }
    temp->error(getErrorMsg());
    return temp;
}

string PrintCustomerStatus::toString() const
{
    return "customerStatus: " + to_string(customerId) + (getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}






//PrintVolunteerStatus class
PrintVolunteerStatus::PrintVolunteerStatus(int id):volunteerId(id)
{
}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    if(wareHouse.checkExistVolunteer(volunteerId))
    {
        wareHouse.getVolunteer(volunteerId).print();
    }
    else
    {
        error("Volunteer doesn’t exist");
        cout << "Error: " << getErrorMsg() << endl;
    }
    complete();
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    PrintVolunteerStatus* temp = new PrintVolunteerStatus(volunteerId);
    if(getStatus() == ActionStatus::COMPLETED)
    {
        temp->complete();
    }
    temp->error(getErrorMsg());
    return temp;
}

string PrintVolunteerStatus::toString() const
{
    return "volunteerStatus: " + to_string(volunteerId) + (getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}




//PrintActionsLog class
PrintActionsLog::PrintActionsLog()
{
}
void PrintActionsLog::act(WareHouse &wareHouse)
{
    if(wareHouse.getActions().empty())
    {
        error("No actions to print");
        cout << "Error: " << getErrorMsg() << endl;
    }
    else
    {
        wareHouse.PrintActions();
        complete();
    }
    wareHouse.addAction(this);

}
PrintActionsLog *PrintActionsLog::clone() const
{
    PrintActionsLog* temp = new PrintActionsLog();
    if(getStatus() == ActionStatus::COMPLETED)
    {
        temp->complete();
    }
    temp->error(getErrorMsg());
    return temp;
}
string PrintActionsLog::toString() const
{
    string ret = "log ";
    return ret + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}







//close class
Close::Close():BaseAction()
{
}

void Close::act(WareHouse &wareHouse)
{
   wareHouse.addAction(this);
   wareHouse.close();
}

Close *Close::clone() const
{
    Close* temp = new Close();
    if(getStatus() == ActionStatus::COMPLETED)
    {
        temp->complete();
    }
    temp->error(getErrorMsg());
    return temp; 
}

string Close::toString() const
{
    string ret = "Close  ";
    return ret + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}






//BackupWareHouse class
BackupWareHouse::BackupWareHouse():BaseAction(){}

void BackupWareHouse::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    if(backup!=nullptr)
    {
        delete backup;
        backup = nullptr;
    }
    backup = new WareHouse(wareHouse);
    complete();
}

BackupWareHouse *BackupWareHouse::clone() const
{
    BackupWareHouse* temp = new BackupWareHouse();
    if(getStatus() == ActionStatus::COMPLETED)
    {
        temp->complete();
    }
    temp->error(getErrorMsg());
    return temp;
}

string BackupWareHouse::toString() const
{
    string ret = "BackupWareHouse  ";
    return ret + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}








//RestoreWareHouse class
RestoreWareHouse::RestoreWareHouse():BaseAction()
{
}

void RestoreWareHouse::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    if(backup!=nullptr)
    {
        wareHouse.~WareHouse();
        wareHouse = *backup;
        complete();
    }
    else
    {
        error("No backup available");
        cout << "Error: " << getErrorMsg() << endl;
    }
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    RestoreWareHouse* temp = new RestoreWareHouse();
    if(getStatus() == ActionStatus::COMPLETED)
    {
        temp->complete();
    }
    temp->error(getErrorMsg());
    return temp;
}

string RestoreWareHouse::toString() const
{
    string ret = "RestoreWareHouse  ";
    return ret + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}