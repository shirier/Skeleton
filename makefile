CC = g++
CFLAGS = -g -Wall -Weffc++ -std=c++11
LFLAGS = -L/usr/lib

# All Targets

all: warehouse


warehouse: clean bin/WareHouse.o bin/Order.o bin/Action.o bin/Volunteer.o bin/Customer.o bin/main.o

    @echo 'Invoking: C++ Linker'


	@echo 'Building target: WareHouse'
	$(CC) -o bin/WareHouse bin/WareHouse.o bin/Order.o bin/Action.o bin/Volunteer.o bin/Customer.o bin/main.o 
	$(LFLAGS)
    @echo 'Finished building target: WareHouse'
 
    @echo ' '

bin/main.o: src/main.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/main.o src/main.cpp

bin/WareHouse.o: src/WareHouse.cpp
	
	$(CC) $(CFLAGS) -c -Iinclude -o bin/WareHouse.o src/WareHouse.cpp

bin/Order.o: src/Order.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Order.o src/Order.cpp
	
bin/Action.o: src/Action.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp
	
bin/Volunteer.o: src/Volunteer.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Volunteer.o src/Volunteer.cpp
	
bin/Customer.o: src/Customer.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Customer.o src/Customer.cpp
clean:
	rm -f bin/*