#include <stdio.h>
#include "ccsv.h"

int main(void) {
        csv_file* dataframe = read_csv("./data/country_full.csv");
        printf("%s\n", get_token(dataframe, 1, 0)->data);
        destroy_csv(dataframe);
}
