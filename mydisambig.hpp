#include <iostream>
void Read_map_file(const char *fname);
void Read_text_file(const char *fname);
void build_dp_table(int line);

typedef struct{
    string word;
    double delta;
    int backtrack;
}DP_TABLE;
