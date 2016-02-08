ifdef ESYS_HOME
    $(info ESys location is '$(ESYS_HOME)')
else
    $(warning ESYS_HOME environment variable undefined)
    ESYS_HOME=../ESys
endif

TARGET = libtracelog
OUTDIR = lib
LIBS = -ltracelog -lesys
CC = g++
INCLUDES = include $(ESYS_HOME)/include
LIBDIRS = lib/ $(ESYS_HOME)/lib

EXAMPLE_TARGET = log-demo
EXAMPLE_UDPCLIENT = udp-client
EXAMPLE_OUTDIR = bin

INCPARAMS=$(foreach d, $(INCLUDES),-I$d)
LIBDIRPARAMS=$(foreach d, $(LIBDIRS),-L$d)

#OPTFLAGS = -s -O3
CFLAGS = -g -Wall -std=c++11 $(INCPARAMS) $(OPTFLAGS)
LFLAGS = -Wall $(LIBDIRPARAMS)
RUNARGS = 

SRCS = src/trace/Logger.cpp src/trace/LogEntry.cpp src/trace/log.cpp src/trace/ConsoleBackEnd.cpp src/trace/UdpBackEnd.cpp
SRCS +=src/trace/FileBackEnd.cpp src/trace/LogPersistThread.cpp src/trace/TraceBuffer.cpp src/trace/TraceSharedContainer.cpp
SRCS +=src/trace/UdpClientMediator.cpp

EXAMPLE_SRCS = example/main.cpp

OBJS = $(subst .cpp,.o,$(SRCS))

.PHONY: default all $(EXAMPLE_TARGET) clean

default: all
all: $(TARGET) $(EXAMPLE_TARGET)

.PRECIOUS: $(TARGET) $(OBJS)

DEPS := $(OBJS:.o=.d)
DEPS += main.d

-include $(DEPS)

%.o: %.cpp
	@$(CC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(CFLAGS) -c $< -o $@


$(TARGET): $(OBJS)
	@mkdir -p $(OUTDIR)
	@ar rvs $(OUTDIR)/$@.a $(OBJS)

clean:
	@rm -f $(OBJS)
	@rm -f $(DEPS)	
	@rm -f $(OUTDIR)/$(TARGET).a
	@rm -f $(EXAMPLE_OUTDIR)/$(EXAMPLE_TARGET)
	@rm -f $(EXAMPLE_OUTDIR)/$(EXAMPLE_UDPCLIENT)
	
# Example code
$(EXAMPLE_TARGET): main.o $(TARGET)
	@mkdir -p $(EXAMPLE_OUTDIR)
	$(CC) $< $(LFLAGS) $(LIBS) -o $(EXAMPLE_OUTDIR)/$@ -pthread $(OPTFLAGS)


main.o: example/main.cpp
	@$(CC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(CFLAGS) -c $< -o $@	
