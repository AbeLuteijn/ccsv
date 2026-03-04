#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char* data;
	size_t length;
} csv_token;

typedef struct {
	size_t columns;
	size_t rows;
	csv_token* data;
} csv_file;

csv_token* create_csv_token(char* buffer, int size) {
	csv_token* newToken = (csv_token*)malloc(sizeof(char) * size + sizeof(size_t));
	newToken->length = size;
	newToken->data = buffer;
	return newToken;
}

int main(void) {
	FILE* inputFile;

	// TODO: make dynamic with arg parser
	inputFile = fopen("./data/country_full.csv", "r");

	if (inputFile == NULL) {
		fprintf(stderr, "Could not open the file\n");
		return 1;
	}
	char currentChar;
	char* token_buffer;

	int rows = 0; 
	int columns = 0; 
	int character_cursor = 0;
	int token_cursor = 0;

	// TODO: convert to booleans?
	int foundString = 0; 
	int foundAllColumns = 0;

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
		character_cursor = 0;
		token_cursor++;

		if (!foundAllColumns) { 
			printf("COLUMN: ");
			columns++;
		}

		// TODO: printing the token, remove later
		printf("(%p) ", nextToken);
		for (int i = 0; i < nextToken->length; i++) {
			printf("%c", nextToken->data[i]);
		}
		printf("\n");
	

		// we have found the data, start a new row.
		if (isNewline) { 
			rows++;
			foundAllColumns = 1;
			// TODO: REMOVE JUST FOR LOOKS RIGHT NOW
			printf("\n");
		} 
	}

	fclose(inputFile);
	
	// INFO HEADERS
	printf("\nROWS: %d | COLUMNS: %d\n", rows, columns);
	return 0;
}
