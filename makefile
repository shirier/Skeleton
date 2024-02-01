CC = g++
CFLAGS = -g -Wall -Weffc++ -std=c++11
LFLAGS = -L/usr/lib
INCLUDES = -Iinclude


# List of source files
SRCS = src/main.cpp src/WareHouse.cpp src/Order.cpp src/Action.cpp src/Volunteer.cpp src/Customer.cpp

# List of object files
OBJS = $(patsubst src/%.cpp,bin/%.o,$(SRCS))

# Targets

all: warehouse

warehouse: $(OBJS)
	@echo 'Building target: warehouse'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/warehouse $(OBJS) $(LFLAGS)
	@echo 'Finished building target: warehouse'
	@echo ' '

# Rule to compile source files into object files
bin/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Clean the build directory
clean:
	@echo 'Cleaning'
	rm -f bin/*

# .PHONY is used to declare targets that are not real files
.PHONY: all warehouse clean