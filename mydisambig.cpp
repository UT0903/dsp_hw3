#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include "Ngram.h"
#include "mydisambig.hpp"

#define BUFFSIZE 10000
using namespace std;
map<string, vector<string>> MapData;
vector <vector <string>> TextData;
vector <vector<DP_TABLE>> dp_table;
int main(int argc, char **argv){
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
    map<string, vector<string>>::iterator iter;
    iter = MapData.find("£¢");
    cout << iter->second[0] << iter->second[1] << iter->second[2] << iter->second[3] << endl;
    for(int i = 0; i < TextData.size(); i++){
        //build_dp_table(i);
    }
    
    
}


void Read_map_file(const char *fname){
    char buf[BUFFSIZE];
    fstream File;
    File.open(fname, ios::in);
    string delim = " ";
    while(File.getline(buf, sizeof(buf))){
        string s = buf;
        size_t pos = 0;

        pos = s.find(delim);
        string key = s.substr(0, pos);
        s.erase(0, pos + delim.length());
        vector <string> tmp;
        while ((pos = s.find(delim)) != string::npos) {
            string token = s.substr(0, pos);
            if(token.length() == 2){
                //std::cout << token << std::endl;
                tmp.push_back(token);
            }
            s.erase(0, pos + delim.length());
        }
        MapData.insert(pair<string, vector<string>>(key, tmp));
    }
}

void Read_text_file(const char *fname){
    //read text file
    char buf[BUFFSIZE];
    fstream textFile;
    textFile.open(fname, ios::in);
    string delim = " ";
    while(textFile.getline(buf, sizeof(buf))){
        string s = buf;
        size_t pos = 0;
        vector <string> tmp_vec;
        while ((pos = s.find(delim)) != string::npos) {
            string token = s.substr(0, pos);
            if(token.length() == 2){
                //std::cout << token << std::endl;
                tmp_vec.push_back(token);
            }
            s.erase(0, pos + delim.length());
        }
        TextData.push_back(tmp_vec);
    }
}

void build_dp_table(int task_num){
    dp_table.clear();
    for(int i = 0; i < TextData[task_num].size(); i++){ //time i
        vector <DP_TABLE> tmp;
        vector <string> get_map = MapData[TextData[task_num][i]];
        for(int j = 0; j < get_map.size(); j++){
            DP_TABLE info;
            info.word = get_map[j];
            tmp.push_back(info);
        }
        dp_table.push_back(tmp);  
    }
    for(int i = 0; i < dp_table.size(); i++){
        for(int j = 0; j < dp_table[i].size(); j++){
            cout << " i: " << i << " j: " << j << " " << dp_table[i][j].word << endl;
        }
    }
}
