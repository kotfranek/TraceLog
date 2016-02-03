ifdef ESYS_PATH
    $(info ESys location is '$(ESYS_PATH)')
else
    $(error ESYS_PATH environment variable undefined)
endif

TARGET = libtracelog
OUTDIR = lib
LIBS = -ltracelog -lesys
CC = g++
INCLUDES = include $(ESYS_PATH)/include
LIBDIRS = lib/ $(ESYS_PATH)/lib

EXAMPLE_TARGET = log-demo
EXAMPLE_UDPCLIENT = udp-client
EXAMPLE_OUTDIR = bin

INCPARAMS=$(foreach d, $(INCLUDES),-I$d)
LIBDIRPARAMS=$(foreach d, $(LIBDIRS),-L$d)

#OPTFLAGS = -s -O3
CFLAGS = -g -Wall -std=c++11 $(INCPARAMS) $(OPTFLAGS)
LFLAGS = -Wall $(LIBDIRPARAMS)
RUNARGS = 

SRCS = src/log/Logger.cpp src/log/LogEntry.cpp src/log/log.cpp src/log/ConsoleBackEnd.cpp src/log/UdpBackEnd.cpp
SRCS +=src/log/FileBackEnd.cpp src/log/LogPersistThread.cpp src/log/TraceBuffer.cpp src/log/TraceSharedContainer.cpp
SRCS +=src/log/UdpClientMediator.cpp

EXAMPLE_SRCS = example/main.cpp example/udpclient.cpp

OBJS = $(subst .cpp,.o,$(SRCS))

.PHONY: default all $(EXAMPLE_TARGET) clean

default: all
all: $(TARGET) $(EXAMPLE_TARGET) $(EXAMPLE_UDPCLIENT)

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
	
$(EXAMPLE_UDPCLIENT): udpclient.o $(TARGET)
	@mkdir -p $(EXAMPLE_OUTDIR)
	$(CC) $< $(LFLAGS) $(LIBS) -o $(EXAMPLE_OUTDIR)/$@ -pthread $(OPTFLAGS)	


main.o: example/main.cpp
	@$(CC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(CFLAGS) -c $< -o $@

udpclient.o: example/udpclient.cpp
	@$(CC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(CFLAGS) -c $< -o $@	
