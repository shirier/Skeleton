#include "../include/Customer.h"
#include "Customer.h"

using namespace std;

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders): id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId()
{ // Add opening curly brace
}

const string &Customer::getName() const
{
    return this->name;
}

int Customer::getId() const
{
    return this->id;
}

int Customer::getCustomerDistance() const
{
    return this->locationDistance;
}

int Customer::getMaxOrders() const
{
    return this->maxOrders;
}

int Customer::getNumOrders() const
{
    return this->ordersId.size();
}

bool Customer::canMakeOrder() const
{
    return getNumOrders() < getMaxOrders();
}

const vector<int> &Customer::getOrdersIds() const
{
    return this->ordersId;
}

int Customer::addOrder(int orderId)
{
    ordersId.push_back(orderId);
    return orderId; //check value
}

int Customer::getNumOrdersLeft() const
{
    return maxOrders-getNumOrders();
}

/*C
ustomer::Customer(const Customer& other) 
        : id(other.id), 
          name(other.name), 
          locationDistance(other.locationDistance), 
          maxOrders(other.maxOrders), 
          ordersId(other.ordersId) {}
          */



//Soldier class
SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders):Customer(id, name, locationDistance, maxOrders){}

SoldierCustomer *SoldierCustomer::clone() const
{
    return nullptr;
}




//civilian class
CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders):Customer(id, name, locationDistance, maxOrders){}

Customer *CivilianCustomer::clone() const
{
    return nullptr;
}

