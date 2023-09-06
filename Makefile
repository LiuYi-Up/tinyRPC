PATH_BIN = bin
PATH_LIB = lib
PATH_OBJ = obj

PATH_TINYRPC = tinyrpc
PATH_COMM = $(PATH_TINYRPC)/common
PATH_NET = $(PATH_TINYRPC)/net
PATH_TCP = $(PATH_TINYRPC)/net/tcp


PATH_TESTCASES = testcases

# will install lib to /usr/lib/libtinyrpc.a
PATH_INSTALL_LIB_ROOT = /usr/lib

# will install all header file to /usr/include/tinyrpc
PATH_INSTALL_INC_ROOT = /usr/include

PATH_INSTALL_INC_COMM = $(PATH_INSTALL_INC_ROOT)/$(PATH_COMM)
PATH_INSTALL_INC_NET = $(PATH_INSTALL_INC_ROOT)/$(PATH_NET)
PATH_INSTALL_INC_TCP = $(PATH_INSTALL_INC_ROOT)/$(PATH_TCP)

# PATH_PROTOBUF = /usr/include/google
# PATH_TINYXML = /usr/include/tinyxml

CXX := g++

CXXFLAGS += -g -O0 -std=c++11 -Wall -Wno-deprecated -Wno-unused-but-set-variable

CXXFLAGS += -I./ -I$(PATH_TINYRPC)	-I$(PATH_COMM)  -I$(PATH_NET)

LIBS += /usr/lib/x86_64-linux-gnu/libprotobuf.a	/usr/lib/libtinyxml.a

COMM_OBJ := $(patsubst $(PATH_COMM)/%.cpp, $(PATH_OBJ)/%.o, $(wildcard $(PATH_COMM)/*.cpp))
NET_OBJ := $(patsubst $(PATH_NET)/%.cpp, $(PATH_OBJ)/%.o, $(wildcard $(PATH_NET)/*.cpp))
TCP_OBJ := $(patsubst $(PATH_TCP)/%.cpp, $(PATH_OBJ)/%.o, $(wildcard $(PATH_TCP)/*.cpp))

ALL_TESTS : $(PATH_BIN)/test_log $(PATH_BIN)/test_eventloop $(PATH_BIN)/test_tcp

TEST_CASE_OUT := $(PATH_BIN)/test_log $(PATH_BIN)/test_eventloop $(PATH_BIN)/test_tcp

LIB_OUT := $(PATH_LIB)/libtinyrpc.a

$(PATH_BIN)/test_log: $(LIB_OUT)
	$(CXX) $(CXXFLAGS) $(PATH_TESTCASES)/test_log.cpp -o $@ $(LIB_OUT) $(LIBS) -ldl -pthread

$(PATH_BIN)/test_eventloop: $(LIB_OUT)
	$(CXX) $(CXXFLAGS) $(PATH_TESTCASES)/test_eventloop.cpp -o $@ $(LIB_OUT) $(LIBS) -ldl -pthread

$(PATH_BIN)/test_tcp: $(LIB_OUT)
	$(CXX) $(CXXFLAGS) $(PATH_TESTCASES)/test_tcp.cpp -o $@ $(LIB_OUT) $(LIBS) -ldl -pthread

$(LIB_OUT): $(COMM_OBJ) $(NET_OBJ) $(TCP_OBJ)
	cd $(PATH_OBJ) && ar rcv libtinyrpc.a *.o && cp libtinyrpc.a ../lib/

$(PATH_OBJ)/%.o : $(PATH_COMM)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PATH_OBJ)/%.o : $(PATH_NET)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PATH_OBJ)/%.o : $(PATH_TCP)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# print something test
# like this: make PRINT-PATH_BIN, and then will print variable PATH_BIN
PRINT-% : ; @echo $* = $($*)


# to clean 
clean :
	rm -f $(COMM_OBJ) $(NET_OBJ) $(TCP_OBJ) $(TESTCASES) $(TEST_CASE_OUT) $(PATH_LIB)/libtinyrpc.a $(PATH_OBJ)/libtinyrpc.a $(PATH_OBJ)/*.o

# install
install:
	mkdir -p $(PATH_INSTALL_INC_COMM) \
		&& cp $(PATH_COMM)/*.h $(PATH_INSTALL_INC_COMM) \
		&& cp $(PATH_NET)/*.h $(PATH_INSTALL_INC_NET) \
		&& cp $(PATH_TCP)/*.h $(PATH_INSTALL_INC_TCP) \
		&& cp $(LIB_OUT) $(PATH_INSTALL_LIB_ROOT)/ \


# uninstall
uninstall:
	rm -rf $(PATH_INSTALL_INC_ROOT)/tinyrpc && rm -f $(PATH_INSTALL_LIB_ROOT)/libtinyrpc.a