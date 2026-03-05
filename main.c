#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char* data;
	size_t length;
} csv_token;

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


#define DEFAULT_CAPACITY 256

csv_file* init_csv_file(size_t rows, size_t columns, csv_file_data* source) {
	csv_file* newFile = (csv_file*)malloc(sizeof(csv_file));
	if (!newFile) {
		fprintf(stderr, "Malloc of CSV_FILE failed\n");
		return NULL;
	}
	newFile->rows = rows;
	newFile->columns = columns;
	newFile->data = source;
	return newFile;
}

csv_file_data* init_csv_data_buffer() { 
	csv_file_data* buffer = (csv_file_data*)malloc(sizeof(csv_file_data));
	if (!buffer) return NULL;
	buffer->size = 0;
	buffer->capacity = DEFAULT_CAPACITY;
	buffer->data = (csv_token**)malloc(sizeof(csv_token*) * DEFAULT_CAPACITY);
	if (!buffer->data) { 
		free(buffer);
		return NULL;
	}
	return buffer;
}

void push(csv_file_data* source, csv_token* data) {
	if (source->size >= source->capacity) {
		source->capacity *= 2;
		csv_token** newData = (csv_token**)realloc(source->data, source->capacity * sizeof(csv_token));
		if (!newData) {
			fprintf(stderr, "Realloc failed for push\n");
			exit(1);
		}
		source->data = newData;
	}
	source->data[source->size++] = data;
}

void free_csv(csv_file* source) {
	for (size_t i = 0; i < source->data->size; i++) {
    		free(source->data->data[i]->data);
    		free(source->data->data[i]);
	}
	free(source);
}

csv_token* create_csv_token(char* buffer, int size) {
	csv_token* newToken = (csv_token*)malloc(sizeof(csv_token));
	if (!newToken) return NULL;
	newToken->length = size;
	newToken->data = (char*)malloc(sizeof(char) * size);
	if (!newToken->data) {
		free(newToken);
		return NULL;
	}
	memcpy(newToken->data, buffer, size);
	return newToken;
}

csv_token* get(csv_file_data* source, int row, int column, int columns) {
	return source->data[row * columns + column];
}


int main(void) {
	FILE* inputFile;

	// TODO: make dynamic with arg parser
	inputFile = fopen("./data/country_full.csv", "r");

	if (inputFile == NULL) {
		fprintf(stderr, "Could not open the file\n");
		return 1;
	}
	int currentChar;
	char* token_buffer;

	int rows = 0; 
	int columns = 0; 
	int character_cursor = 0;
	int token_cursor = 0;

	// TODO: convert to booleans?
	int foundString = 0; 
	int foundAllColumns = 0;

	csv_file_data* file_data = init_csv_data_buffer();
	while((currentChar = fgetc(inputFile)) != EOF) {
		// are we starting/ending a string?
		if (currentChar == '\"') {
			foundString = !foundString;
			token_buffer[character_cursor++] = currentChar;
			continue;
		}

		// are we inside of the string?
		if (foundString) {
			token_buffer[character_cursor++] = currentChar;
			continue;
		}

		// have we found a delimiter?
		int isComma = (currentChar == ',');
		int isNewline = (currentChar == '\n');
		int isDelimiter = isComma || isNewline;

		if (!isDelimiter) {
			token_buffer[character_cursor++] = currentChar;
			continue;
		}

		// we have found our token 
		csv_token* nextToken = create_csv_token(token_buffer, character_cursor);
		push(file_data, nextToken);
		character_cursor = 0;
		token_cursor++;

		if (!foundAllColumns) { 
			columns++;
		}

		// we have found the data, start a new row.
		if (isNewline) { 
			rows++;
			foundAllColumns = 1;
		} 
	}

	fclose(inputFile);
	csv_file* myFile = init_csv_file(rows, columns, file_data);
	free_csv(myFile);
	return 0;
}
