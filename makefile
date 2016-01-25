TARGET = libtracelog
OUTDIR = lib
LIBS = -lesys -ltracelog
CC = g++
INCLUDES = include ../ESys/include
LIBDIRS = lib/ ../ESys/lib

EXAMPLE_TARGET = log-demo
EXAMPLE_OUTDIR = bin

INCPARAMS=$(foreach d, $(INCLUDES),-I$d)
LIBDIRPARAMS=$(foreach d, $(LIBDIRS),-L$d)

#OPTFLAGS = -s -O3
CFLAGS = -g -Wall -std=c++11 $(INCPARAMS) $(OPTFLAGS)
LFLAGS = -Wall $(LIBDIRPARAMS)
RUNARGS = 

SRCS = src/log/Logger.cpp src/log/LogEntry.cpp src/log/log.cpp src/log/ConsoleBackEnd.cpp src/log/UdpBackEnd.cpp
SRCS +=src/log/FileBackEnd.cpp src/log/LogPersistThread.cpp src/log/TraceBuffer.cpp src/log/TraceSharedContainer.cpp

EXAMPLE_SRCS = example/main.cpp 

OBJS = $(subst .cpp,.o,$(SRCS))

.PHONY: default all $(EXAMPLE_TARGET) clean

default: $(TARGET)
all: default $(EXAMPLE_TARGET)

.PRECIOUS: $(TARGET) $(OBJS)

DEPS := $(OBJS:.o=.d)

-include $(DEPS)

%.o: %.cpp
	@$(CC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(CFLAGS) -c $< -o $@


$(TARGET): $(OBJS)
	@mkdir -p $(OUTDIR)
	@ar rvs $(OUTDIR)/$@.a $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f $(OUTDIR)/$(TARGET).a
	rm -f $(EXAMPLE_OUTDIR)/$(EXAMPLE_TARGET)
	
# Example code
$(EXAMPLE_TARGET): main.o $(OBJS) $(TARGET)
	@mkdir -p $(EXAMPLE_OUTDIR)
	$(CC) $< $(LFLAGS) $(LIBS) -o $(EXAMPLE_OUTDIR)/$@ -pthread $(OPTFLAGS)


main.o: example/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	
