CXX = g++
FLAGS = -O3 -Wall -Werror -pedantic
LDLIBS = -lgmp -lgmpxx
RM = rm -f
TARGET = extended_euclid

.PHONY: all clean

default: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(FLAGS) $? -o $@ $(LDLIBS) 
	
clean:
	$(RM) $(TARGET)
