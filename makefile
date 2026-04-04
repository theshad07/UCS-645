
CXX      := g++
CXXFLAGS := -O3 -Wall -Wextra -fopenmp
LDFLAGS  := -fopenmp


TARGET  := program
SRCS    := main.cpp correlate.cpp
OBJS    := $(SRCS:.cpp=.o)


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY: all clean

clean:
	rm -f $(OBJS) $(TARGET)