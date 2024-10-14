#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct Phonebook {
    char *names;
    char *phones;
    int *size;

    int max_rows;
    int max_row_size;
};

void add_contact(struct Phonebook phonebook, char name[], char number[]) {
    strcpy(&phonebook.names[*phonebook.size * phonebook.max_row_size], name);
    strcpy(&phonebook.phones[*phonebook.size * phonebook.max_row_size], number);
    *phonebook.size += 1;
}

char *get_name(struct Phonebook phonebook, int idx) {
    if (*phonebook.size < idx) {
        fprintf(stderr, "Out of boundaries");
        return NULL;
    }
    return &phonebook.names[idx * phonebook.max_row_size];
}

char *get_phone(struct Phonebook phonebook, int idx) {
    if (*phonebook.size < idx) {
        fprintf(stderr, "Out of boundaries");
        return NULL;
    }
    return &phonebook.phones[idx * phonebook.max_row_size];
}

void print_phonebook(struct Phonebook phonebook) {
    for (int i = 0; i < *phonebook.size; ++i) {
        printf("%s\n%s\n", get_name(phonebook, i), get_phone(phonebook, i));
    }
}

bool extract_phonebook_from_stdin(struct Phonebook phonebook) {
    // Extract output from stdin and return if raise error or end successfully
    // Buffers
    char line_buffer[phonebook.max_row_size];
    char name_buffer[phonebook.max_row_size];
    char phone_buffer[phonebook.max_row_size];

    int buffer_size = 0;
    // Start with first line
    int line_count = 1;
    while (true) {
        // Get char and check if it ends line or file
        if (phonebook.max_rows < *phonebook.size) {
            fprintf(stderr, "To much contact on input");
            return true;
        }
        char c = getchar();
        if (c == '\n' || c == EOF) {
            // If buffer ends line complete chars with '\0' to make string and copy it to right buffer
            line_buffer[buffer_size] = '\0';
            if (line_count % 2 == 1) {
                strcpy(name_buffer, line_buffer);
            } else {
                strcpy(phone_buffer, line_buffer);
                add_contact(phonebook, name_buffer, phone_buffer);
                strcpy(name_buffer, "");
                strcpy(phone_buffer, "");
            }
            line_count++;

            if (c == EOF) {
                return false;
            }

            strcpy(line_buffer, "");
            buffer_size = 0;
            continue;
        }
        // Add character to line buffer
        line_buffer[buffer_size] = c;
        buffer_size++;
    }
}


void search_numbers(const struct Phonebook phonebook, const char *searched_num, int find_match[]) {
    if (searched_num == NULL) {
        for (int i = 0; i < *phonebook.size; ++i) {
            find_match[i] = 1;
        }
        return;
    }
    for (int i = 0; i < *phonebook.size; ++i) {
        char *phone_number = get_phone(phonebook, i);
        // find first number from phone_number
        char *ptr = phone_number;
        char first_char = searched_num[0];
        int MATCH = 0;
        while ((ptr = strchr(ptr, first_char)) != NULL) {
            // Check if next character match
            for (int idx = 1; idx < (int) (strlen(searched_num)); ++idx) {
                // If match next another
                if (*(ptr + idx) == searched_num[idx]) {
                    MATCH = 1;
                } else {
                    MATCH = 0;
                    break;
                }
            }
            if (MATCH == 1) {
                break;
            }
            ptr++; // Move the pointer to search for the next occurrence
        }
        find_match[i] = MATCH;
    }
}

int main(int argc, char *argv[]) {
    // TODO: Remove clock
    clock_t start = clock();

    // Error flag
    bool raise_error;

    char *entered_number = NULL;
    if (1 < argc) {
        entered_number = argv[1];
    }

    int MAX_ROWS = 40000;
    int MAX_ROW_SIZE = 100;

    char names[MAX_ROWS * MAX_ROW_SIZE];
    char phones[MAX_ROWS * MAX_ROW_SIZE];
    int size = 0;

    struct Phonebook phonebook = {names, phones, &size, MAX_ROWS, MAX_ROW_SIZE};

    raise_error = extract_phonebook_from_stdin(phonebook);
    if (raise_error) { return -1; }

    int find_match[MAX_ROWS];
    // Go through phonebook numbers
    search_numbers(phonebook, entered_number, find_match);

    for (int i = 0; i < *phonebook.size; ++i) {
        if (find_match[i]) {
            printf("%s - %s\n", get_name(phonebook, i), get_phone(phonebook, i));
        }
    }

    printf("\n%d\n", *phonebook.size);
    // Search in names in order without break

    // Search in names in order with break

    printf("End!\n");
    // TODO: Remove clock
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds\n", cpu_time_used);
    return 0;
}
