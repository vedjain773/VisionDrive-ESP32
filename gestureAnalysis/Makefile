# Specifying which compiler to use
CC = g++							

# Defining the name of the object that will be created after using make
PROJECT = gesture_analysis		

# Here Include specifies which directories can be added in the include path of the main.cpp
CFLAGS = -std=c++11 -pthread -Iinclude/ -I/usr/local/include $(shell pkg-config --cflags opencv4)
# What libraries to be imported here it is opencv 
# LIBS = $(shell pkg-config --libs opencv4)
LIBS = $(shell pkg-config --libs opencv4) -lixwebsocket -lssl -lcrypto -lpthread -lz

# Similar to how we used to link ad make object files in the terminal
# the command make build SRC=<FILENAME> should build the executable in the subfolder
.PHONY: build
ifeq ($(SRC), $(link),)
build:
	$(error "SRC is not set")
else
build:
	@echo "Building..."
	@$(CC) $(SRC) $(link) -o $(PROJECT) $(CFLAGS) $(LIBS)
endif	

# if folder is not set, clean all build files all subfolders
.PHONY: clean
clean:
	@echo "Cleaning..."
	@rm -rf $(PROJECT)
