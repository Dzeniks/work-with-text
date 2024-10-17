#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <regex.h>

#define MAX_CONTACTS 30000
#define MAX_ROW_SIZE 101
#define MAX_ENCODE_SIZE 6

struct Phonebook {
    char *names;
    char *phones;
    int *size;
};

void decode_number(char num, char *decoded_chars) {
    switch (num) {
        case '2':
            strcpy(decoded_chars, "abc2");
            break;
        case '3':
            strcpy(decoded_chars, "def3");
            break;
        case '4':
            strcpy(decoded_chars, "ghi4");
            break;
        case '5':
            strcpy(decoded_chars, "jkl5");
            break;
        case '6':
            strcpy(decoded_chars, "mno6");
            break;
        case '7':
            strcpy(decoded_chars, "pqrs7");
            break;
        case '8':
            strcpy(decoded_chars, "tuv8");
            break;
        case '9':
            strcpy(decoded_chars, "wxyz9");
            break;
        case '0':
            strcpy(decoded_chars, "+9");
            break;
        default: ;
    }
}

int add_contact(struct Phonebook phonebook, char name[], char number[]) {
    // Check data validity, add data and return was raised error
    if (strlen(name) <= 0) {
        fprintf(stderr, "Missing name for contact\n");
        return 1;
    }
    if (strlen(number) <= 0) {
        fprintf(stderr, "Missing number for contact\n");
        return 1;
    }
    char digit;
    for (int i = 0; (digit = number[i]) != '\0'; ++i) {
        if (!isdigit(digit)) {
            fprintf(stderr, "In the number of contact was found unsupported character\n");
            return 1;
        }
    }
    strcpy(&phonebook.names[*phonebook.size * MAX_ROW_SIZE], name);
    strcpy(&phonebook.phones[*phonebook.size * MAX_ROW_SIZE], number);
    *phonebook.size += 1;
    return 0;
}

char *get_name(struct Phonebook phonebook, int idx) {
    if (*phonebook.size < idx) {
        fprintf(stderr, "Out of boundaries");
        return NULL;
    }
    return &phonebook.names[idx * MAX_ROW_SIZE];
}

char *get_phone(struct Phonebook phonebook, int idx) {
    if (*phonebook.size < idx) {
        fprintf(stderr, "Out of boundaries");
        return NULL;
    }
    return &phonebook.phones[idx * MAX_ROW_SIZE];
}

void print_phonebook(struct Phonebook phonebook) {
    for (int i = 0; i < *phonebook.size; ++i) {
        printf("%s, %s\n", get_name(phonebook, i), get_phone(phonebook, i));
    }
}

int extract_phonebook_from_stdin(struct Phonebook phonebook) {
    // Extract output from stdin and return if raise error or end successfully
    // Buffers
    char line_buffer[MAX_ROW_SIZE];
    char name_buffer[MAX_ROW_SIZE];
    char phone_buffer[MAX_ROW_SIZE];

    int buffer_size = 0;
    // Start with first line
    int line_count = 1;
    while (true) {
        // Get char and check if it ends line or file
        if (MAX_CONTACTS < *phonebook.size) {
            fprintf(stderr, "To much contact on input");
            return true;
        }
        char c = tolower(getchar());
        if (c == '\n' || c == EOF) {
            // If buffer ends line complete chars with '\0' to make string and copy it to right buffer
            line_buffer[buffer_size] = '\0';
            if (line_count % 2 == 1) {
                strcpy(name_buffer, line_buffer);
            } else {
                strcpy(phone_buffer, line_buffer);
                int error = add_contact(phonebook, name_buffer, phone_buffer);
                if (error) {
                    fprintf(stderr, "Incorrect format of input data\n");
                    return -1;
                }
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

void search_numbers(const struct Phonebook phonebook, const char *searched_nums, int find_match[]) {
    if (searched_nums == NULL) {
        for (int i = 0; i < *phonebook.size; ++i) {
            find_match[i] = 1;
        }
        return;
    }
    for (int i = 0; i < *phonebook.size; ++i) {
        char *phone_number = get_phone(phonebook, i);
        char *ptr = phone_number;
        int MATCH = 0;
        while ((ptr = strstr(ptr, searched_nums)) != NULL) {
            if (ptr != NULL) {
                MATCH = 1;
                break;
            }
        }
        find_match[i] = MATCH;
    }
}

void combination_of_strings(char *strings, int expected_size, int allocated_size, int index, char *current_combination,
                     char *results, int *result_size) {
    if (index == expected_size) {
        strcpy(&results[*result_size * allocated_size], current_combination);
        *result_size += 1;
        return;
    }
    char *current_string = &strings[index * allocated_size];

    // Copy string
    char current_combination_copy[allocated_size];
    strcpy(current_combination_copy, current_combination);

    char c;
    // Get chars from strings[index] and create combinations
    for (int i = 0; (c = current_string[i]) != '\0'; ++i) {
        char c_str[] = {c, '\0'};
        char new_combination[(allocated_size)];
        int new_index = (index + 1);

        strcpy(new_combination, current_combination);
        strcat(new_combination, c_str);
        combination_of_strings(strings, expected_size, allocated_size, new_index, new_combination, results, result_size);
    }
}

void search_names(struct Phonebook phonebook, char *entered_number, int *find_match_names) {
    if (entered_number == NULL) {
        for (int i = 0; i < MAX_CONTACTS; ++i) {
            find_match_names[i] = 1;
        }
        return;
    }
    char entered_num;
    char all_decoded[MAX_ENCODE_SIZE * strlen(entered_number)];
    int num_possible_combinations = 1;
    for (int i = 0; (entered_num = entered_number[i]) != '\0'; ++i) {
        char decoded_num[MAX_ENCODE_SIZE];
        decode_number(entered_num, decoded_num);
        num_possible_combinations *= strlen(decoded_num);
        strcpy(&all_decoded[MAX_ENCODE_SIZE * i], decoded_num);
    }

    char current_combination_test[MAX_ENCODE_SIZE] = "";
    char combinations[MAX_ENCODE_SIZE * num_possible_combinations];
    int combinations_size = 0;
    combination_of_strings(all_decoded, strlen(entered_number), MAX_ENCODE_SIZE, 0, current_combination_test, combinations,
                    &combinations_size);

    for (int combination_idx = 0; combination_idx < combinations_size; ++combination_idx) {
        for (int names_idx = 0; names_idx < *phonebook.size; ++names_idx) {
            char *phone_number = get_name(phonebook, names_idx);
            int MATCH = 0;
            if (find_match_names[names_idx] == 1) {
                break;
            }
            while ((phone_number = strstr(phone_number, &combinations[MAX_ENCODE_SIZE * combination_idx])) != NULL) {
                if (phone_number != NULL) {
                    MATCH = 1;
                    break;
                }
            }
            find_match_names[names_idx] = MATCH;
        }
    }
}


int main(int argc, char *argv[]) {
    // TODO: Remove clock
    clock_t start = clock();

    // Error flag
    int raise_error;

    char *entered_number = NULL;
    if (1 < argc) {
        entered_number = argv[1];
    }

    char names[MAX_CONTACTS * MAX_ROW_SIZE];
    char phones[MAX_CONTACTS * MAX_ROW_SIZE];
    int size = 0;

    struct Phonebook phonebook = {names, phones, &size};

    raise_error = extract_phonebook_from_stdin(phonebook);
    if (raise_error) { return -1; }


    int find_match_numbers[MAX_CONTACTS];
    // Go through phonebook numbers
    search_numbers(phonebook, entered_number, find_match_numbers);

    int find_match_names[MAX_CONTACTS];
    // Go through phonebook names
    search_names(phonebook, entered_number, find_match_names);

    // Union of matches
    int matches[MAX_CONTACTS];
    for (int i = 0; i < MAX_CONTACTS; ++i) {
        matches[i] = (find_match_names[i] || find_match_numbers[i]);
    }

    printf("All matches\n");
    for (int i = 0; i < *phonebook.size; ++i) {
        if (matches[i] == 1) {
            printf("Name %s, Phone %s\n", get_name(phonebook, i), get_phone(phonebook, i));
        }
    }


    printf("End!\n");
    // TODO: Remove clock
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds\n", cpu_time_used);
    return 0;
}
