#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <stack>
#include <float.h>
#include "Ngram.h"
#include "mydisambig.hpp"

#define BUFFSIZE 10000
using namespace std;
map<string, vector<string>> MapData;
vector <vector <string>> TextData;

vector<string> splitStr2Vec(string s, string splitSep);
void Print_DP_table();

vector <vector<string>> dp_table_word;
vector <vector<double>> dp_table_prob;
vector <vector<int>> dp_table_backtrack;


int main(int argc, char **argv){
    if (argc != 5) {
        fprintf(stderr, "Usage: ./mydisambig [seg file] [map file] [lm file] [output file]\n");
        exit(1);
    }

    const char* text_file = argv[1];
    const char* map_file = argv[2];
    const char* lm_file = argv[3];
    const char* out_file = argv[4];
    int order = 2;

    Vocab voc;
    Ngram lm(voc, order);

    //read lm file
    File lmFile(lm_file, "r");
    lm.read(lmFile);
    lmFile.close();

    fstream output_file;
    output_file.open(out_file, ios::trunc | ios::out);

    Read_text_file(text_file); //read text file
    Read_map_file(map_file); //read map file

    for(int task_num = 0; task_num < TextData.size(); task_num++){
        
        /// step1: build_dp_table
        // step1-1: reset table
        dp_table_word.clear();
        dp_table_prob.clear();
        dp_table_backtrack.clear();
        
        // step1-2: push <s> into table[0]
        vector <string> str_word;
        str_word.push_back("<s>");
        dp_table_word.push_back(str_word);

        vector <double> str_prob;
        str_prob.push_back(1);
        dp_table_prob.push_back(str_prob);

        vector <int> str_back;
        str_back.push_back(0);
        dp_table_backtrack.push_back(str_back);
        
        // step1-3: push map words into table
        for(int i = 0; i < TextData[task_num].size(); i++){
            vector <string> word;
            vector <double> prob;
            vector <int> back;

            map<string, vector<string>>::iterator iter;
            iter = MapData.find(TextData[task_num][i]);
            if(iter == MapData.end()){
                VocabIndex wid = voc.getIndex(TextData[task_num][i].c_str());
                if(wid == Vocab_None){
                    wid = voc.getIndex(Vocab_Unknown);
                }

                word.push_back(Vocab_Unknown);
                prob.push_back(-1);
                back.push_back(-1);
            }
            else{
                for(int j = 0; j < iter->second.size(); j++){
                    word.push_back(iter->second[j]);
                    prob.push_back(-1);
                    back.push_back(-1);
                }
            }
            dp_table_word.push_back(word);
            dp_table_prob.push_back(prob);
            dp_table_backtrack.push_back(back);
        }
        
        // step1-4: push </s> into table[-1]
        vector <string> end_word;
        end_word.push_back("</s>");
        dp_table_word.push_back(end_word);

        vector <double> end_prob;
        end_prob.push_back(-1);
        dp_table_prob.push_back(end_prob);

        vector <int> end_back;
        end_back.push_back(-1);
        dp_table_backtrack.push_back(end_back);
        

        ///step2: DP
        for(int t = 1; t < dp_table_word.size(); t++){
            for(int i = 0; i < dp_table_word[t].size(); i++){
                VocabIndex wi = voc.getIndex(dp_table_word[t][i].c_str());
                if(wi == Vocab_None){
                    wi = voc.getIndex(Vocab_Unknown);
                }
                double best_prob = -DBL_MAX;
                int best_j = -1;
                for(int j = 0; j < dp_table_word[t-1].size(); j++){
                    VocabIndex wj = voc.getIndex(dp_table_word[t-1][j].c_str());
                    if(wj == Vocab_None){
                        wj = voc.getIndex(Vocab_Unknown);
                    }
                    VocabIndex context[] = {wj, Vocab_None};
                    double new_prob = lm.wordProb(wi, context) + dp_table_prob[t-1][j];
                    if(new_prob > best_prob){
                        best_j = j;
                        best_prob = new_prob;
                    }
                }
                dp_table_prob[t][i] = best_prob;
                dp_table_backtrack[t][i] = best_j;
            }
        }
        ///step3: find max and backtrack
        //step3-1: find max of table[-1]
        int best_id = -1;
        double best = -DBL_MAX;
        for(int i = 0; i < dp_table_prob[dp_table_prob.size() - 1].size(); i++){
            if(dp_table_prob[dp_table_prob.size() - 1][i] > best){
                best = dp_table_prob[dp_table_prob.size() - 1][i];
                best_id = i;
            }
        }
        //step3-2: backtracking
        stack<string> myStack;
        for(int i = dp_table_word.size() - 1; i > 0; i--){
            myStack.push(dp_table_word[i][best_id]);
            best_id = dp_table_backtrack[i][best_id];
        }
        myStack.push(dp_table_word[0][best_id]);
        

        //step3-3: pop and write out answer
        output_file << myStack.top();
        myStack.pop();
        while(myStack.size() > 0){
            output_file << " " << myStack.top();
            myStack.pop();
        }
        output_file << endl;
    }
    output_file.close();
}


void Read_map_file(const char *fname){
    char buf[BUFFSIZE];
    fstream File;
    File.open(fname, ios::in);
    while(File.getline(buf, sizeof(buf))){
        string s = buf;
        vector<string> sep = splitStr2Vec(s, " \n");
        string key = sep[0];
        sep.erase(sep.begin());
        MapData.insert(pair<string, vector<string>>(key, sep));
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
        vector <string> sep = splitStr2Vec(s, " \n");
        TextData.push_back(sep);
    }
}


vector<string> splitStr2Vec(string s, string splitSep){
    vector<string> result;
    int current = 0;
    int next = 0;
    while (next != -1){
        next = s.find_first_of(splitSep, current); 
        if (next != current){
            string tmp = s.substr(current, next - current);
            if(!tmp.empty()){
                result.push_back(tmp);
            }
        }
        current = next + 1;
    }
    return result;
}

void Print_DP_table(){
    for(int i = 0; i < dp_table_word.size(); i++){
        assert(dp_table_word[i].size() == dp_table_prob[i].size() && dp_table_word[i].size() == dp_table_backtrack[i].size());
        for(int j = 0; j < dp_table_word[i].size(); j++){
            cout << dp_table_word[i][j] << "," << dp_table_prob[i][j] << "," << dp_table_backtrack[i][j] << " ";
        }
        cout << endl;
    }
}