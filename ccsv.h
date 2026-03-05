#ifndef CCSV_H
#define CCSV_H

#include <stddef.h>

typedef struct {
    char* data;
    size_t length;
} csv_token;

typedef struct {
    csv_token** row_entries;
    size_t entries;
} csv_row;

typedef struct {
    size_t size;
    size_t capacity;
    csv_token** data;
} csv_file_data;

typedef struct {
    size_t columns;
    size_t rows;
    csv_file_data* data;
} csv_file;


void push(csv_file_data* source, csv_token* data);

void destroy_csv(csv_file* source);

csv_token* create_csv_token(char* buffer, size_t size);

csv_row* get_row(csv_file* source, int row);

csv_token* get_token(csv_file* source, int row, int column);

csv_file* read_csv(const char path[]);

#endif
