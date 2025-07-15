//This is a simple lexical analyzer which is basics of compiler design , it extracts the keyword,literals,identifiers and operators.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define LEN_MAX 50

char keywordSet[MAX][LEN_MAX];
char identifierSet[MAX][LEN_MAX];
char operatorsSet[MAX][LEN_MAX];
char literalSet[MAX][LEN_MAX];

int keywordCount = 0, identifierCount = 0, operatorCount = 0, literalCount = 0;

// All C Keywords
char keywords[32][10] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

int exists(char arr[MAX][50], int count, char *str) {
    for (int i = 0; i < count; i++) {
        if (strcmp(str, arr[i]) == 0) return 1;
    }
    return 0;
}

//function to add unnique keywords,operators,identifiers and literals
void add_unique(char arr[MAX][50], int *count, char *str) {
    if (!exists(arr, *count, str)) {
        strcpy(arr[*count], str);
        (*count)++;
    }
}

//function to check if a word is keyword
int is_keyword(char *word) {
    for (int i = 0; i < 32; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

//function to check if a character is a operator
int is_operator_char(char ch) {
    char operators[] = "+-*/=%<>&|!^";
    for (int i = 0; i < strlen(operators); i++) {
        if (ch == operators[i])
            return 1;
    }
    return 0;
}

//function to check for a number
int is_number(char *token) {
    int i = 0, dot = 0;

    if (token[i] == '-') i++;  // negative numbers

    while (token[i] != '\0') {
        if (token[i] == '.') {
            dot++;
            if (dot > 1) return 0;
        }
        else if (!isdigit(token[i])) {
            return 0;
        }
        i++;
    }

    return 1;
}

//main function
int main() {
    
    printf("\n===========Lexical Analyzer===========\n\n");
    char file[50];
    printf("Enter File Path : ");
    scanf("%s",file);
    FILE *fptr = fopen(file, "r");
    if (!fptr) {
        printf("Error: File not found.\n\n");
        return 1;
    }

    char ch, buffer[LEN_MAX];
    int i = 0;

    while ((ch = fgetc(fptr)) != EOF) {

        // Handle string literals
        if (ch == '"') {
            char strLiteral[LEN_MAX] = "\"";
            while ((ch = fgetc(fptr)) != EOF && ch != '"') {
                strncat(strLiteral, &ch, 1);
            }
            strcat(strLiteral, "\"");
            add_unique(literalSet, &literalCount, strLiteral);
            continue;
        }

        // Token builder
        if (isalnum(ch) || ch == '_') {
            buffer[i++] = ch;
        } else {
            if (i != 0) {
                buffer[i] = '\0';

                if (is_keyword(buffer)) {
                    add_unique(keywordSet, &keywordCount, buffer);
                } else if (is_number(buffer)) {
                    add_unique(literalSet, &literalCount, buffer);
                } else {
                    add_unique(identifierSet, &identifierCount, buffer);
                }

                i = 0;
            }

            // Handle multi-character operators
            if (is_operator_char(ch)) {
                char next = fgetc(fptr);
                char op[3] = {ch, '\0', '\0'};

                if (is_operator_char(next)) {
                    op[1] = next;
                    add_unique(operatorsSet, &operatorCount, op);
                } else {
                    ungetc(next, fptr); // push back if not part of op
                    add_unique(operatorsSet, &operatorCount, op);
                }
            }
        }
    }

    // Final token flush if file ends with a word
    if (i != 0) {
        buffer[i] = '\0';

        if (is_keyword(buffer)) {
            add_unique(keywordSet, &keywordCount, buffer);
        } else if (is_number(buffer)) {
            add_unique(literalSet, &literalCount, buffer);
        } else {
            add_unique(identifierSet, &identifierCount, buffer);
        }
    }

    fclose(fptr);

    // Display tokens
    printf("\nKeywords: ");
    for (int i = 0; i < keywordCount; i++) {
        printf("%s ", keywordSet[i]);
    }

    printf("\n\nIdentifiers: ");
    for (int i = 0; i < identifierCount; i++) {
        printf("%s ", identifierSet[i]);
    }

    printf("\n\nLiterals: ");
    for (int i = 0; i < literalCount; i++) {
        printf("%s ", literalSet[i]);
    }

    printf("\n\nOperators: ");
    for (int i = 0; i < operatorCount; i++) {
        printf("%s ", operatorsSet[i]);
    }

    printf("\n\n");

    return 0;
}
