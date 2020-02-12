TARGET_SERVER = Server
TARGET_CLIENT = Client
INSTALL_DIR = bin
SOURCE_SERVER += Serve.cpp test/ServerMain.cpp
SOURCE_CLIENT += Client.cpp test/ClientMain.cpp

OBJ_SERVER = $(patsubst %.cpp, %.o, $(SOURCE_SERVER))
OBJ_CLIENT = $(patsubst %.cpp, %.o, $(SOURCE_CLIENT))
CC = gcc
CXX = g++
CFLAGS += -lpthread -I./

all:$(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(OBJ_SERVER)
	$(CXX) $^   $(CFLAGS) -o $(INSTALL_DIR)/$(TARGET_SERVER)

$(TARGET_CLIENT): $(OBJ_CLIENT)
	$(CXX) $^  $(CFLAGS) -o $(INSTALL_DIR)/$(TARGET_CLIENT)

%.o:%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
clean:
	rm *.o $(INSTALL_DIR)/* test/*.o
