#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_CONTACTS 27000
#define MAX_ROW_SIZE 101
#define MAX_ENCODE_SIZE 6


char encode_char(char c) {
    switch (c) {
        case 'a':
        case 'b':
        case 'c':
            return '2';
        case 'd':
        case 'e':
        case 'f':
            return '3';
        case 'g':
        case 'h':
        case 'i':
            return '4';
        case 'j':
        case 'k':
        case 'l':
            return '5';
        case 'm':
        case 'n':
        case 'o':
            return '6';
        case 'p':
        case 'q':
        case 'r':
        case 's':
            return '7';
        case 't':
        case 'u':
        case 'v':
            return '8';
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            return '9';
        case '+':
            return '0';
        case ' ':
            return ' ';
        default:
            return c;
    }
}

char *encode_str(char *str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        str[i] = encode_char(str[i]);
    }
    return str;
}

char to_lower(char character) {
    if ('A' <= character && character <= 'Z') {
        character += 32;
    }
    return character;
}

int is_digit(char digit) {
    return ('0' <= digit && digit <= '9');
}

int is_number(const char *str) {
    char char_digit;
    for (int i = 0; (char_digit = str[i]) != '\0'; ++i) {
        if (!is_digit(char_digit)) {
            return 0;
        }
    }
    return 1;
}

struct Phonebook {
    char *names;
    char *phones;
    int *size;
};

int add_contact(struct Phonebook phonebook, char name[], char number[]) {
    // Check data validity, add data
    if (strlen(name) <= 0) {
        fprintf(stderr, "Missing name for contact\n");
        return 1;
    }
    if (strlen(number) <= 0) {
        fprintf(stderr, "Missing number for contact\n");
        return 1;
    }
    int is_num = is_number(number);
    if (is_num == 0) {
        fprintf(stderr, "In the number of contact was found unsupported character\n");
        return 1;
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

void get_phone_cpy(struct Phonebook phonebook, int idx, char *str_out) {
    strcpy(str_out, get_phone(phonebook, idx));
}

void name_to_nums_cpy(struct Phonebook phonebook, int idx, char *str_out) {
    char *name = get_name(phonebook, idx);
    strcpy(str_out, name);
    encode_str(str_out);
}

// Search substring in string
int search_str_in_str(const char *str, const char *searched_str) {
    if (searched_str == NULL) {
        return 0;
    }
    if ((strstr(str, searched_str)) != NULL) {
        return 1;
    }
    return 0;
}

// Search chars of wanted string in order, recursively in provided str
void search_chars_in_str(const char *str, const char *searched_str, int expected_size, int cur_size, int *found) {
    char c = searched_str[cur_size];
    char *ptr = (strchr(str, c));

    if (expected_size == cur_size) {
        *found = 1;
        return;
    }
    if (ptr != NULL) {
        // Move + 1 to create new search string without found char
        ptr++;
        int new_size = cur_size + 1;
        search_chars_in_str(ptr, searched_str, expected_size, new_size, found);

    }
}

// Extract output from stdin and return if raise error or end successfully
int extract_phonebook_from_stdin(struct Phonebook phonebook) {
    char line_buffer[MAX_ROW_SIZE];
    char name_buffer[MAX_ROW_SIZE];
    char phone_buffer[MAX_ROW_SIZE];
    int buffer_size = 0;

    int line_count = 1;
    while (1) {
        if (MAX_CONTACTS <= *phonebook.size) {
            fprintf(stderr, "To much contact on input\n");
            return 1;
        }
        if (MAX_ROW_SIZE <= buffer_size) {
            fprintf(stderr, "To much long row on input\n");
            return 1;
        }
        char stdin_char = (char)getchar();
        stdin_char = to_lower(stdin_char);
        if (stdin_char == '\n' || stdin_char == EOF) {
            line_buffer[buffer_size] = '\0';
            if (line_count % 2 == 1) {
                strcpy(name_buffer, line_buffer);
            } else {
                strcpy(phone_buffer, line_buffer);
                int error = add_contact(phonebook, name_buffer, phone_buffer);
                if (error) {
                    fprintf(stderr, "Incorrect format of input data on line: %d\n", line_count);
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

// Search in phonebook using copied string from function and
// find_match: an array of int's that correspond to the indexes of the objects in the phonebook
int search_str(struct Phonebook phonebook, char *temp_wanted_str, const char *searched_str, int *find_match,
                     void (cpy_str)(struct Phonebook, int, char *)) {
    // Return match size and fill matches in find_match
    int match_count = 0;
    for (int entry_idx = 0; entry_idx < *phonebook.size; ++entry_idx) {
        cpy_str(phonebook, entry_idx, temp_wanted_str);
        int found = search_str_in_str(temp_wanted_str, searched_str);
        find_match[entry_idx] = found;
        match_count += found;
    }
    return match_count;
}

// Search in phonebook using copied string from function with breaks between wanted chars
// find_match: an array of int's that correspond to the indexes of the objects in the phonebook
int search_str_break(struct Phonebook phonebook, char *temp_wanted_str, const char *searched_str, int *find_match,
                     void (cpy_str)(struct Phonebook, int, char *)) {
    int match_count = 0;
    for (int entry_idx = 0; entry_idx < *phonebook.size; ++entry_idx) {
        cpy_str(phonebook, entry_idx, temp_wanted_str);
        search_chars_in_str(temp_wanted_str, searched_str, (int)strlen(searched_str), 0, &find_match[entry_idx]);
        match_count += find_match[entry_idx];
    }
    return match_count;
}

int search_numbers(const struct Phonebook phonebook, const char *searched_nums, int find_match_numbers[]) {
    char temp_num[MAX_ROW_SIZE];
    return search_str(phonebook, temp_num, searched_nums, find_match_numbers, get_phone_cpy);
}

int search_names(struct Phonebook phonebook, const char *searched_nums, int *find_match_names) {
    char temp_name[MAX_ROW_SIZE];
    return search_str(phonebook, temp_name, searched_nums, find_match_names, name_to_nums_cpy);
}

int search_names_break(const struct Phonebook phonebook, const char *searched_nums, int *find_match_names) {
    char temp_name[MAX_ROW_SIZE];
    return search_str_break(phonebook, temp_name, searched_nums, find_match_names, name_to_nums_cpy);
}


int main(int argc, char *argv[]) {
    // Error flag
    int raise_error;

    // Get arguments
    char *entered_search_number = NULL;
    int search_with_break = 0;
    if (argc == 2) {
        entered_search_number = argv[1];
    }
    if (argc == 3 && strcmp(argv[2], "-s") != 0) {
        search_with_break = 1;
        entered_search_number = argv[2];
    }

    // Extract stdin to structure
    char names[MAX_CONTACTS * MAX_ROW_SIZE];
    char phones[MAX_CONTACTS * MAX_ROW_SIZE];
    int size = 0;
    struct Phonebook phonebook = {names, phones, &size};
    raise_error = extract_phonebook_from_stdin(phonebook);
    if (raise_error) { return -1; }

    // search in phonebook numbers
    int find_match_numbers[MAX_CONTACTS];
    int numbers_match_count = search_numbers(phonebook, entered_search_number, find_match_numbers);

    // search in phonebook names
    int find_match_names[MAX_CONTACTS];
    int names_match_count = 0;
    if (search_with_break) {
        names_match_count = search_names_break(phonebook, entered_search_number, find_match_names);
    } else {
        names_match_count = search_names(phonebook, entered_search_number, find_match_names);
    }

    // Print results
    if ((names_match_count + numbers_match_count) == 0) {
        printf("Not found\n");
        return 0;
    }

    // Union of matches
    int matches[MAX_CONTACTS];
    for (int contact_idx = 0; contact_idx < MAX_CONTACTS; ++contact_idx) {
        matches[contact_idx] = (find_match_names[contact_idx] || find_match_numbers[contact_idx]);
    }

    for (int pb_idx = 0; pb_idx < *phonebook.size; ++pb_idx) {
        if (matches[pb_idx] == 1) {
            printf("%s, %s\n", get_name(phonebook, pb_idx), get_phone(phonebook, pb_idx));
        }
    }
    return 0;
}
