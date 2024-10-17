#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_CONTACTS 30000
#define MAX_ROW_SIZE 101
#define MAX_ENCODE_SIZE 6

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

char to_lower(char character) {
    if ('A' <= character && character <= 'Z') {
        character -= 32;
    }
    return character;
}

struct Phonebook {
    char *names;
    char *phones;
    int *size;
};

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
        if ('0' <= digit && digit <= '9') {
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
    for (int pb_idx = 0; pb_idx < *phonebook.size; ++pb_idx) {
        printf("%s, %s\n", get_name(phonebook, pb_idx), get_phone(phonebook, pb_idx));
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
    while (1) {
        // Get char and check if it ends line or file
        if (MAX_CONTACTS <= *phonebook.size) {
            fprintf(stderr, "To much contact on input\n");
            return 1;
        }
        if (MAX_ROW_SIZE <= buffer_size) {
            fprintf(stderr, "To much long row on input\n");
            return 1;
        }
        char stdin_char = getchar();
        // lower char
        stdin_char = to_lower(stdin_char);
        if (stdin_char == '\n' || stdin_char == EOF) {
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

            if (stdin_char == EOF) {
                return 0;
            }

            strcpy(line_buffer, "");
            buffer_size = 0;
            continue;
        }
        // Add character to line buffer
        line_buffer[buffer_size] = stdin_char;
        buffer_size++;
    }
}

void search_numbers(const struct Phonebook phonebook, const char *searched_nums, int find_match[]) {
    if (searched_nums == NULL) {
        for (int pb_idx = 0; pb_idx < *phonebook.size; ++pb_idx) {
            find_match[pb_idx] = 1;
        }
        return;
    }
    for (int pb_idx = 0; pb_idx < *phonebook.size; ++pb_idx) {
        char *phone_number = get_phone(phonebook, pb_idx);
        char *ptr = phone_number;
        int found_match = 0;
        while ((ptr = strstr(ptr, searched_nums)) != NULL) {
            if (ptr != NULL) {
                found_match = 1;
                break;
            }
        }
        find_match[pb_idx] = found_match;
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

    // Get chars from strings[index] and create combinations
    char char_for_new_combination;
    for (int str_idx = 0; (char_for_new_combination = current_string[str_idx]) != '\0'; ++str_idx) {
        char str_for_new_combination[] = {char_for_new_combination, '\0'};
        char new_combination[(allocated_size)];
        int new_index = (index + 1);

        strcpy(new_combination, current_combination);
        strcat(new_combination, str_for_new_combination);
        combination_of_strings(strings, expected_size, allocated_size, new_index, new_combination, results,
                               result_size);
    }
}

void search_names(struct Phonebook phonebook, char *entered_number, int *find_match_names) {
    if (entered_number == NULL) {
        for (int pb_idx = 0; pb_idx < MAX_CONTACTS; ++pb_idx) {
            find_match_names[pb_idx] = 1;
        }
        return;
    }
    char entered_num;
    char all_decoded[MAX_ENCODE_SIZE * strlen(entered_number)];
    int num_possible_combinations = 1;
    for (int pb_idx = 0; (entered_num = entered_number[pb_idx]) != '\0'; ++pb_idx) {
        char decoded_num[MAX_ENCODE_SIZE];
        decode_number(entered_num, decoded_num);
        num_possible_combinations *= (int)strlen(decoded_num);
        strcpy(&all_decoded[MAX_ENCODE_SIZE * pb_idx], decoded_num);
    }
    // Generate all combinations
    char current_combination_test[MAX_ENCODE_SIZE] = "";
    char combinations[MAX_ENCODE_SIZE * num_possible_combinations];
    int combinations_size = 0;
    int expected_combination_size = strlen(entered_number);
    combination_of_strings(all_decoded, expected_combination_size, MAX_ENCODE_SIZE, 0, current_combination_test,
                           combinations,
                           &combinations_size);

    // Search combination in string
    for (int combination_idx = 0; combination_idx < combinations_size; ++combination_idx) {
        for (int names_idx = 0; names_idx < *phonebook.size; ++names_idx) {
            char *phone_number = get_name(phonebook, names_idx);
            int found_match = 0;
            if (find_match_names[names_idx] == 1) {
                break;
            }
            while ((phone_number = strstr(phone_number, &combinations[MAX_ENCODE_SIZE * combination_idx])) != NULL) {
                if (phone_number != NULL) {
                    found_match = 1;
                    break;
                }
            }
            find_match_names[names_idx] = found_match;
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

    // Go through phonebook numbers
    int find_match_numbers[MAX_CONTACTS];
    search_numbers(phonebook, entered_number, find_match_numbers);

    // Go through phonebook names
    int find_match_names[MAX_CONTACTS];
    search_names(phonebook, entered_number, find_match_names);

    // Union of matches
    int matches[MAX_CONTACTS];
    for (int contact_idx = 0; contact_idx < MAX_CONTACTS; ++contact_idx) {
        matches[contact_idx] = (find_match_names[contact_idx] || find_match_numbers[contact_idx]);
    }

    for (int pb_idx = 0; pb_idx < *phonebook.size; ++pb_idx) {
        if (matches[pb_idx] == 1) {
            printf("Name %s, Phone %s\n", get_name(phonebook, pb_idx), get_phone(phonebook, pb_idx));
        }
    }

    // TODO: Remove clock
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);

    return 0;
}
