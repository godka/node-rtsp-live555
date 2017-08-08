CXX=g++
CC=gcc
CPPINCLUDE += -O3 -I./3rd-party/live555/liveMedia/include -I./3rd-party/live555/BasicUsageEnvironment/include -I./3rd-party/live555/groupsock/include -I./3rd-party/live555/UsageEnvironment/include
CPPFLAGS += $(CPPINCLUDE) -fpermissive -Wdeprecated-declarations -w -O3 -std=c++11
CPPLDFLAGS += -L./3rd-party/live555/liveMedia -L./3rd-party/live555/BasicUsageEnvironment -L./3rd-party/live555/groupsock -L./3rd-party/live555/UsageEnvironment -lliveMedia -lBasicUsageEnvironment -lgroupsock -lUsageEnvironment
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
