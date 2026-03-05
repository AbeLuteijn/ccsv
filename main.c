#include <stdio.h>
#include "ccsv.h"

int main(void) {
	csv_file* dataframe = read_csv("./data/country_full.csv");
	printf("%s\n", get_token(dataframe, 0, 1)->data);
	destroy_csv(dataframe);
	return 0;
}
