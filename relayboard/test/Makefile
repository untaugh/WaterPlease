### Target Settings ###

TARGET = relayboard
SRC_DIR = ..
#SRC_FILES = relay
#SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
SRC_OBJS = relay.o relaytimer.o cmd.o water.o 
GCC_FLAGS = -Wall -Wextra -Wconversion

### CPPUTest Settings ###

TEST_DIR = 
TEST_SRC = *.cpp
TEST_TARGET = $(TARGET)_test
TEST_OBJS = AllTest.o RelayTest.o RelaytimerTest.o CmdTest.o WaterTest.o RelayinitTest.o 
CPPUTEST_PATH = /home/oscu/Documents/Tdd/cpputest-3.7.1
CPPUTEST_INCLUDE = -I$(CPPUTEST_PATH)/include/
CPPUTEST_LIB = -L$(CPPUTEST_PATH)/lib/
LD_LIBRARIES = $(CPPUTEST_LIB) -lCppUTest -lCppUTestExt
TEST_GCC = g++

### Rules ###

%.o : %.cpp
	$(TEST_GCC) -c $< $(GCC_FLAGS) -I$(SRC_DIR) $(CPPUTEST_INCLUDE)

%.o : $(SRC_DIR)/%.c
	$(TEST_GCC) -c $< $(GCC_FLAGS)

$(TEST_TARGET): $(TEST_OBJS) $(SRC_OBJS)
	$(TEST_GCC) $(TEST_OBJS) $(SRC_OBJS) -o $(TEST_TARGET) $(LD_LIBRARIES)

runtest:
	./$(TEST_TARGET)

clean:
	rm -f $(TEST_TARGET) $(TEST_OBJS) $(SRC_OBJS)
