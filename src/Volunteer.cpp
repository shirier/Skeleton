#include "Volunteer.h"
#include <iostream>
using namespace std;

Volunteer::Volunteer(int id, const string &name):id(id),name(name),completedOrderId(NO_ORDER), activeOrderId(NO_ORDER)
{
}

int Volunteer::getId() const
{
    return this->id;
}

const string &Volunteer::getName() const
{
    return this->name;
}

int Volunteer::getActiveOrderId() const
{
    return this->activeOrderId;
}

int Volunteer::getCompletedOrderId() const
{
    return this->completedOrderId;
}

bool Volunteer::isBusy() const
{
     return activeOrderId != NO_ORDER;;
}

//collector volunteer
CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown)
    : coolDown(coolDown)  ,Volunteer(id,name) //check
{
}

CollectorVolunteer *CollectorVolunteer::clone() const
{
    return new CollectorVolunteer(this->getId(),this->getName(),this->coolDown);
}
void CollectorVolunteer::step()
{
    if(activeOrderId != -1)
    {
        if(this->decreaseCoolDown() && timeLeft==0)
        {
            this->completedOrderId=activeOrderId;
            this->activeOrderId=-1;

        }
    }

}
int CollectorVolunteer::getCoolDown() const
{
    return this->coolDown;
}
int CollectorVolunteer::getTimeLeft() const
{
    return this->timeLeft;
}
bool CollectorVolunteer::decreaseCoolDown()
{
    if(this->timeLeft>0)
    {
        this->timeLeft--;
        return true;
    }
    return false;
}

bool CollectorVolunteer::hasOrdersLeft() const //check about this
{
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const// check what about the timeleft?
{
    return (!(this->isBusy())) && (order.getStatus()==OrderStatus::PENDING);
}


void CollectorVolunteer::acceptOrder(const Order &order)
{
    if(this->canTakeOrder(order))
    {

        this->activeOrderId=order.getId();
        RestTimeLeft();
        completedOrderId=NO_ORDER;
    }

}

string CollectorVolunteer::toString() const
{
    return "Volunteer name: " + this->getName() + "- Regular collector id: " + std::to_string(this->getId())+ " (there is no maxOrders)";
}

void CollectorVolunteer::print() const
{
    cout << "VolunteerID: " <<this->getId() <<endl;
    
    if(isBusy())
    {
        cout <<"isBusy: True" << "\n" <<"OrderID: " << this->activeOrderId <<"\n" <<"TimeLeft: " <<this->timeLeft <<endl;
    }
    else
    {
        cout <<"isBusy: False" << "\n" <<"OrderID: None"<< "\n" <<"TimeLeft: None"  << endl;
    }
    cout <<"OrdersLeft: No Limit" << endl;
}

void CollectorVolunteer::RestTimeLeft()
{
    this->timeLeft=this->coolDown;
}


//limeted collector class
LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders) 
    :   maxOrders(maxOrders),ordersLeft(maxOrders), CollectorVolunteer(id,name,coolDown){}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
    return new LimitedCollectorVolunteer(this->getId(),this->getName(),this->getCoolDown(),this->maxOrders);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const 
{
    return this->ordersLeft>0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{
    return  (!(this->isBusy())) && hasOrdersLeft() && (order.getStatus()==OrderStatus::PENDING);
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
    if(this->canTakeOrder(order))               
    {
        this->activeOrderId=order.getId();
        RestTimeLeft();
        this->ordersLeft--;
        completedOrderId=NO_ORDER;
    }
}

int LimitedCollectorVolunteer::getMaxOrders() const
{
    return this->maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const
{
    return this->ordersLeft;
}

string LimitedCollectorVolunteer::toString() const
{
    return "Volunteer name: " + this->getName()+ "- Limited collector id: " + "this->getId()"+ " maxOrders: "+ std::to_string(this->maxOrders) + " orders left: "+ std::to_string(this->ordersLeft);
}

void LimitedCollectorVolunteer::print() const
{
   cout << "VolunteerID: " <<this->getId() <<endl;
    if(isBusy())
    {
        cout <<"isBusy: True" << "\n"<<"OrderID: " << this->activeOrderId << "\n"<<"TimeLeft: " <<this->getTimeLeft() <<endl;
    }
    else
    {
        cout <<"isBusy: False" << "\n"<<"OrderID: None"<< "\n"<<"TimeLeft: None"  << endl;
    }
    cout<<"OrdersLeft: "<<this->ordersLeft << endl;
}

//driver volunteer
DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep)
    :maxDistance(maxDistance), distancePerStep(distancePerStep) , Volunteer(id ,name){}

DriverVolunteer *DriverVolunteer::clone() const
{
    return new DriverVolunteer(this->getId(),this->getName(),this->maxDistance,this->distancePerStep);
}

int DriverVolunteer::getDistanceLeft() const
{
    return this->distanceLeft;
}

int DriverVolunteer::getMaxDistance() const
{
    return this->maxDistance;
}

int DriverVolunteer::getDistancePerStep() const
{
    return this->distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft()
{
    if(this->distanceLeft>0)
    {
        this->distanceLeft-=distancePerStep;
        return true;
    }
    return false;
}

bool DriverVolunteer::hasOrdersLeft() const
{
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const
{

    return (!this->isBusy()) && hasOrdersLeft() && (order.getStatus()==OrderStatus::COLLECTING) && order.getDistance()<=this->maxDistance;
}

void DriverVolunteer::acceptOrder(const Order &order)
{
    if(this->canTakeOrder(order))
    {
        this->activeOrderId=order.getId();
        this->distanceLeft=order.getDistance();
        completedOrderId=NO_ORDER;

    }
}

void DriverVolunteer::step()
{
    distanceLeft-=distancePerStep;
    if(distanceLeft<=0)
    {
        completedOrderId=activeOrderId;
        activeOrderId=NO_ORDER;
    }
}

string DriverVolunteer::toString() const
{
    return "Volunteer name: " + this->getName() + "- Regular Driver id: " + std::to_string(this->getId())+ " (there is no maxOrders)"+" maxDistance: " + std::to_string(this->maxDistance)+" distance per stip: " + std::to_string(this->distancePerStep);
}

void DriverVolunteer::print() const
{
    cout << "VolunteerID: " <<this->getId() <<endl;
    if(isBusy())
    {
        cout <<"isBusy: True" << "\n" <<"OrderID: " << this->activeOrderId<< "\n" <<"TimeLeft: " <<this->distanceLeft <<endl;
    }
    else
    {
        cout <<"OrderID: None"<<"\n"<<"TimeLeft: None"  << endl;
    }
    cout<<"isBusy: False" << "\n" <<"OrdersLeft: None"<< endl;
}

void DriverVolunteer::SetDistanceLeft(int distace)
{
    this->distanceLeft=distace;
}


//limted driver class
LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders)
: maxOrders(maxOrders) ,ordersLeft(maxOrders) ,DriverVolunteer(id,name,maxDistance,distancePerStep)
{
}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const
{
    return new LimitedDriverVolunteer(this->getId(),this->getName(),this->getMaxDistance(),this->getDistancePerStep(),this->maxOrders);
}

int LimitedDriverVolunteer::getMaxOrders() const
{
    return this->maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const
{
    return this->ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const
{
    return this->ordersLeft>0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const
{
     return (!this->isBusy()) && hasOrdersLeft() && (order.getStatus()==OrderStatus::PENDING) && order.getDistance()<=this->getMaxDistance();
}

void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
    if(this->canTakeOrder(order))
    {
        this->activeOrderId=order.getId();
        this->SetDistanceLeft(order.getDistance());
        completedOrderId=NO_ORDER;
    }
}

string LimitedDriverVolunteer::toString() const
{
    return "Volunteer name: " + this->getName() + "- Limited Driver id: " + std::to_string(this->getId())+ "maxOrders: "+std::to_string(this->maxOrders) +" maxDistance: " + std::to_string(this->getMaxDistance())+" distance per stip: " + std::to_string(this->getDistancePerStep());
}

void LimitedDriverVolunteer::print() const
{
    cout << "VolunteerID: " <<this->getId() <<endl;
    if(isBusy())
    {
        cout<<"isBusy: True" <<"\n" <<"OrderID: " << this->activeOrderId<<"\n" <<"TimeLeft: " <<this->getDistanceLeft() <<endl;
    }
    else
    {
        cout <<"isBusy: False" <<"\n"<<"OrderID: None"<< "\n"<<"TimeLeft: None"  << endl;
    }
    cout<<"OrdersLeft: "<<this->ordersLeft << endl;
}
