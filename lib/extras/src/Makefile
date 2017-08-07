CXX=g++
CC=gcc
CPPINCLUDE += -O3 -I/usr/local/include -I./src -I/usr/local/include/groupsock -I/usr/local/include/UsageEnvironment -I/usr/local/include/BasicUsageEnvironment -I/usr/local/include/liveMedia
CPPFLAGS += $(CPPINCLUDE) -fpermissive -Wdeprecated-declarations -w -O3 -std=c++11
CPPLDFLAGS += -L/usr/local/lib -lliveMedia -lBasicUsageEnvironment -lgroupsock -lUsageEnvironment
CPPSOURCES=$(wildcard *.cpp)
CPPOBJECTS=$(CPPSOURCES:.cpp=.o)
CSOURCES=$(wildcard *.c)
COBJECTS=$(CSOURCES:.c=.o)
TARGET=live555client

.PHONY: all clean

all: .dcpp .dc $(CPPSOURCES) $(CSOURCES) $(TARGET)

.dcpp: $(CPPSOURCES)
	$(CXX) $(CPPFLAGS) -MM $(CPPSOURCES) >.dcpp
.dc: $(CSOURCES)
	$(CC) $(CFLAGS) -MM $(CSOURCES) >.dc
-include .dcpp
-include .dc
$(TARGET): $(CPPOBJECTS) $(COBJECTS)
	$(CXX) $(CPPOBJECTS) $(COBJECTS) -o $@ $(CPPLDFLAGS)

clean:
	rm $(CPPOBJECTS) $(COBJECTS) .dcpp .dc
install:
	cp $(TARGET) /usr/local/bin
