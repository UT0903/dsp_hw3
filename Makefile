SRIPATH ?= /root/srilm-1.5.10
MACHINE_TYPE ?= i686-m64
INC_PATH ?= inc
SRC_PATH ?= src

CXX = g++
CXXFLAGS = -O3 -I$(SRIPATH)/include -I$(INC_PATH)
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)
vpath %.c $(SRC_PATH)
vpath %.cpp $(SRC_PATH)

TARGET = mydisambig
SRC = mydisambig.cpp
OBJ = $(SRC:.cpp=.o)
FROM ?= Big5-ZhuYin.map
TO ?= ZhuYin-Big5.map
.PHONY: all clean map


all: $(TARGET)


mydisambig: $(SRC_PATH)/mydisambig.cpp -loolm -ldstruct -lmisc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

map:
	python3 $(SRC_PATH)/mapping.py $(FROM) $(TO)

clean:
	$(RM) $(OBJ) $(TARGET) disambig.o
