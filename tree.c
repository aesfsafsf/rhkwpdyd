#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define ERROR 1
#define TRUE  2
#define FALSE 3

const char* p;

void trim(char* buf) {
    char* src = buf;
    char* dst = buf;

    while (*src != '\0') {
        if (!isspace((unsigned char)*src)) {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
}

int check_paren(char* buf, int size) {
    int count = 0;
    for (int i = 0; i < size && buf[i] != '\0'; i++) {
        if (buf[i] == '(') {
            count++;
        }
        else if (buf[i] == ')') {
            count--;
            if (count < 0) return 0;
        }
    }
    return (count == 0);
}

char nextChar(void) {
    return *p;
}

int parseNode(void) {
    char c = nextChar();
    if (!isalpha((unsigned char)c))
        return ERROR;
    p++;

    int childCount = 0;
    while (1) {
        char ch = nextChar();
        if (ch == '(') {
            p++;
            childCount++;
            if (childCount > 2)
                return FALSE;

            int r = parseNode();
            if (r != TRUE)
                return r;

            ch = nextChar();
            if (ch != ')')
                return ERROR;
            p++;
        }
        else {
            break;
        }
    }
    return TRUE;
}

int main(void) {
    char input[1000];

    printf("트리 문자열을 입력하세요: ");
    if (fgets(input, sizeof(input), stdin) == NULL)
        exit(ERROR);

    trim(input);

    if (!check_paren(input, (int)strlen(input))) {
        printf("ERROR\n");
        exit(0);
    }

    p = input;
    int result = parseNode();

    if (result == TRUE) {
        if (*p != '\0' && *p != '\n')
            result = ERROR;
    }

    if (result == TRUE)
        printf("TRUE\n");
    else if (result == FALSE)
        printf("FALSE\n");
    else
        printf("ERROR\n");

    exit(0);
}
