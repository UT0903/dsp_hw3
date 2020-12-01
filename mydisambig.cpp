#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream.h>
#include "Ngram.h"

using namespace std;
void Read_text_file(const char *fname){
    //read text file
    char buf[BUFFSIZE];
    fstream textFile;
    textFile.open(fname, "r");
    string delim = " ";
    while(getline(buf, sizeof(buf))){
        string s = buf;
        size_t pos = 0;
        string token;
        while ((pos = s.find(delim)) != string::npos) {
            token = s.substr(0, pos);
            std::cout << token << std::endl;
            s.erase(0, pos + delimiter.length());
        }
        exit(0);
    }
    

    
}
int main(){
    if (argc != 9) {
        fprintf(stderr, "Usage: ./mydisambig -text [sep file] -map [map file] -lm [lm file] -order [order]\n");
        exit(1);
    }

    const char* text_file = argv[2];
    const char* map_file = argv[4];
    const char* lm_file = argv[6];
    int order = atoi(argv[8]);
    
    //read lm file
    int ngram_order = 2;
    Vocab voc;
    Ngram lm(voc, ngram_order);
    File lmFile(lm_file, "r");
    lm.read(lmFile);
    lmFile.close();

    //read map file
    Read_text_file(text_file);

}