#ifndef ADDEDFUN_H
#define ADDEDFUN_H
//compare two string
int compare_string(char*, char*);
//find string len
int string_length(char*);
//check records
int check_header_record(int record_type, int depth);
//clear name_buf
void name_clear(char *name);
void print_sod(int depth);
void print_eod(int depth);
void print_de(int depth, off_t size, mode_t mode, off_t si, char *filename);
#endif