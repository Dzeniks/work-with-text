#include <stdio.h>

struct Row {
    char *Name;
    char *Number;
};

void load_stdin(char *input) {
    // TODO: handle possible out of boundaries
    char c;
    for (int i = 0; (c = getchar()) != EOF; i++) {
    input[i] = c;
    }
}

int main() {

    printf("Hello world!\n");
    int MAX_ROWS = 42;
    int MAX_ROW_SIZE = 100;

    // Array for 42 rows 100 characters long
    char input[MAX_ROWS * MAX_ROW_SIZE];


    load_stdin(input);
    printf("Loaded stdin!\n");
;
    printf("Loaded filter names and numbers!");

    // printf("%s\n", input);

    printf("End!\n");
    return 0;
}
