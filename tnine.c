#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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
        printf("%d %s - %s\n", i, get_name(phonebook, i), get_phone(phonebook, i));
    }
}

void extract_phonebook_from_stdin(struct Phonebook phonebook) {
    // Buffers
    char buffer[phonebook.max_row_size];
    char name_buffer[phonebook.max_row_size];
    char phone_buffer[phonebook.max_row_size];

    const int max_row_size = phonebook.max_row_size;
    const int max_rows = phonebook.max_rows;

    int buffer_size = 0;
    // Start with first line
    int line_count = 1;
    while (true) {
        // Raise error
        if (max_row_size <= buffer_size) {
            fprintf(stderr, "Array index out of range");
            return;
        }
        // Raise error
        if (max_rows < *phonebook.size) {
            fprintf(stderr, "Array index out of range");
            return;
        }
        char c = getchar();
        if (c == '\n' || c == EOF) {
            buffer[buffer_size] = '\0';
            if (line_count % 2 == 1) {
                strcpy(name_buffer, buffer);
            } else {
                strcpy(phone_buffer, buffer);
                add_contact(phonebook, name_buffer, phone_buffer);
                strcpy(name_buffer, "");
                strcpy(phone_buffer, "");
            }
            line_count++;

            if (c == EOF) {
                return;
            }

            strcpy(buffer, "");
            buffer_size = 0;
            continue;
        }
        buffer[buffer_size] = c;
        buffer_size++;
    }
}

int main(int argc, char *argv[]) {
    printf("Hello world!\n");

    int MAX_ROWS = 42;
    int MAX_ROW_SIZE = 100;

    char names[MAX_ROWS * MAX_ROW_SIZE];
    char phones[MAX_ROWS * MAX_ROW_SIZE];
    int size = 0;

    struct Phonebook phonebook = {names, phones, &size, MAX_ROWS, MAX_ROW_SIZE};

    extract_phonebook_from_stdin(phonebook);


    printf("End!\n");
    return 0;
}
