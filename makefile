ifdef ESYS_HOME
    $(info ESys location is '$(ESYS_HOME)')
else
    $(warning ESYS_HOME environment variable undefined)
    ESYS_HOME=../ESys
endif

TARGET = libtracelog
TARGET_COMMON = libtracecommon
OUTDIR = lib
LIBS = -ltracelog -ltracecommon -lesys
CC = g++
INCLUDES = include $(ESYS_HOME)/include
LIBDIRS = lib/ $(ESYS_HOME)/lib

EXAMPLE_TARGET = log-demo
EXAMPLE_OUTDIR = bin

INCPARAMS=$(foreach d, $(INCLUDES),-I$d)
LIBDIRPARAMS=$(foreach d, $(LIBDIRS),-L$d)

OPTFLAGS = -s -Os
CFLAGS = -g -Wall -std=c++11 $(INCPARAMS) $(OPTFLAGS)
LFLAGS = -Wall $(LIBDIRPARAMS)
RUNARGS = 

SRCS_COMMON = src/trace/entry/Payload.cpp src/trace/entry/LogEntry.cpp 
SRCS_COMMON+= src/trace/entry/TraceLevelInfo.cpp

SRCS = src/trace/Logger.cpp src/trace/log.cpp src/trace/ConsoleBackEnd.cpp src/trace/UdpBackEnd.cpp
SRCS +=src/trace/FileBackEnd.cpp src/trace/LogPersistThread.cpp src/trace/TraceBuffer.cpp src/trace/TraceSharedContainer.cpp
SRCS +=src/trace/UdpClientMediator.cpp

EXAMPLE_SRCS = example/main.cpp

OBJS_COMMON = $(subst .cpp,.o,$(SRCS_COMMON))
OBJS = $(subst .cpp,.o,$(SRCS))

.PHONY: default all $(TARGET_COMMON) $(EXAMPLE_TARGET) clean

default: all
all: $(TARGET_COMMON) $(TARGET) $(EXAMPLE_TARGET)

.PRECIOUS: $(TARGET) $(TARGET_COMMON) $(OBJS)

DEPS = $(OBJS_COMMON:.o=.d)
DEPS += $(OBJS:.o=.d)
DEPS += main.d

-include $(DEPS)

%.o: %.cpp
	@$(CC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(CFLAGS) -c $< -o $@

# Main Library
$(TARGET): $(OBJS)
	@mkdir -p $(OUTDIR)
	@ar rvs $(OUTDIR)/$@.a $(OBJS)
	
# Common Library, used also for the Client	
$(TARGET_COMMON): $(OBJS_COMMON)
	@mkdir -p $(OUTDIR)
	@ar rvs $(OUTDIR)/$@.a $(OBJS_COMMON)	

clean:
	@rm -f $(OBJS)
	@rm -f $(OBJS_COMMON)
	@rm -f $(DEPS)	
	@rm -f $(OUTDIR)/*.a
	@rm -f example/main.o
	@rm -f $(EXAMPLE_OUTDIR)/$(EXAMPLE_TARGET)
	
# Example code
$(EXAMPLE_TARGET): example/main.o $(TARGET)
	@mkdir -p $(EXAMPLE_OUTDIR)
	$(CC) $< $(LFLAGS) $(LIBS) -o $(EXAMPLE_OUTDIR)/$@ -pthread $(OPTFLAGS)


example/main.o: example/main.cpp
	@$(CC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(CFLAGS) -c $< -o $@	
