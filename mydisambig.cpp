#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "Ngram.h"

using namespace std;
map<string, vector<string>> MapData;
vector <vector <String>> TextData;
void Read_map_file(const char *fname);
void Read_text_file(const char *fname);

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

    
    Read_text_file(text_file); //read text file
    Read_map_file(map_file); //read map file

    for(int i = 0; i < TextData.size(); i++){
        vector <vector<double>> delta;
        for(int j = 0; j < TextData[i].size(); j++){ //time j
            
        }
    }
}


void Read_map_file(const char *fname){
    char buf[BUFFSIZE];
    fstream File;
    File.open(fname, "r");
    string delim = " ";
    while(File.getline(buf, sizeof(buf))){
        string s = buf;
        size_t pos = 0;

        pos = s.find(delim);
        string token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());

        while ((pos = s.find(delim)) != string::npos) {
            string token = s.substr(0, pos);
            //std::cout << token << std::endl;
            if(token.length() == 2){

            }
            s.erase(0, pos + delimiter.length());
        }
    }
}

void Read_text_file(const char *fname){
    //read text file
    char buf[BUFFSIZE];
    fstream textFile;
    textFile.open(fname, "r");
    string delim = " ";
    while(getline(buf, sizeof(buf))){
        string s = buf;
        size_t pos = 0;
        vector <string> tmp_vec;
        while ((pos = s.find(delim)) != string::npos) {
            string token = s.substr(0, pos);
            //std::cout << token << std::endl;
            if(token.length() == 2){
                tmp_vec.append(token);
            }
            s.erase(0, pos + delimiter.length());
        }
        TextData.append(tmp_vec);
    }
}