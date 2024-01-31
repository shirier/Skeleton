#include "Order.h"

Order::Order(int id, int customerId, int distance): id(id), customerId(customerId), distance(distance),
 status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER){}

int Order::getId() const
{
    return this->id;
}

int Order::getCustomerId() const
{
    return this->customerId;
}

void Order::setStatus(OrderStatus status)//and also about failed input
{
    this->status = status;
}

void Order::setCollectorId(int collectorId)
{
    this->collectorId=collectorId;
}

void Order::setDriverId(int driverId)
{
    this->driverId=driverId;
}

int Order::getCollectorId() const
{
    return this->collectorId;
}

int Order::getDriverId() const
{
    return this->driverId;
}

OrderStatus Order::getStatus() const
{
    return this->status;
}

std::string orderStatusToString(OrderStatus status)
{
    if(status ==OrderStatus::PENDING)
    {
        return "PENDING";
    }

    if(status==OrderStatus::COLLECTING)
    {
        return "COLLECTING";
    }

    if(status==OrderStatus::DELIVERING)
    {     
        return "DELIVERING";
    } 
    if(status==OrderStatus::COMPLETED)
    {
        return "COMPLETED";
    }
    return "UNKNOWN";

    
}

const string Order::toString() const
{
    return "order id: " + std::to_string(this->id)+ " Customer Id: " +std::to_string(this->customerId)+ "Distance : "+std::to_string(this->distance)+" Order Status: "+orderStatusToString(this->status) + "Collector Id: "+std::to_string(collectorId) +" Driver Id: "+std::to_string(driverId);
}

int Order::getDistance() const
{
    return this->distance;
}

std::string Order::orderStatusToString(OrderStatus status)
{
    if (status == OrderStatus::PENDING) {
        return "PENDING";
    } 
    else if (status == OrderStatus::COLLECTING) {
        return "COLLECTING";
    } 
    else if (status == OrderStatus::DELIVERING) {
        return "DELIVERING";
    } 
    else if (status == OrderStatus::COMPLETED) {
        return "COMPLETED";
    }
}

Order *Order::clone()
{
    return new Order(*this);
}
