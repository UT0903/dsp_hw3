SRIPATH ?= /root/srilm-1.5.10
MACHINE_TYPE ?= i686-m64
INC_PATH ?= inc
SRC_PATH ?= src

CXX = g++
CXXFLAGS = -O2 -I$(SRIPATH)/include -I$(INC_PATH)
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)
vpath %.c $(SRC_PATH)
vpath %.cpp $(SRC_PATH)

TARGET = mydisambig
SRC = mydisambig.cpp
OBJ = $(SRC:.cpp=.o)
FROM ?= Big5-ZhuYin.map
TO ?= ZhuYin-Big5.map
.PHONY: all clean map

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

all: $(TARGET)

map:
	@#TODO
	@#python3 mapping.py $(FROM) $(TO)
mytest:
	perl separator_big5.pl corpus.txt > corpus_sep.txt
	perl separator_big5.pl test_data/example.txt > example_sep.txt
	ngram-count -text corpus_sep.txt -write corpus_count.txt -order 2
	ngram-count -read corpus_count.txt -lm bigram.lm -order 2 -unk
	disambig -text example_sep.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > output
clean:
	$(RM) $(OBJ) $(TARGET)
