#include <stdbool.h>
#include "mitsu.h"


bool validateArgs(int temp, int on, int operation);

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Usage: #>sudo mitsu <temp> <on> <operation>\n");
        printf("\t<temp> = 10, 16 ... 31\n");
        printf("\t<on> = 1=ON | 0=OFF\n");
        printf("\t<operation> = 0=HEATING | 1=COOLING \n");
        printf("\nExample for heating 22C:\n\t#>sudo mitsu 22 1 0\n");
        return 0;
    }

    int temp = atoi(argv[1]);
    int on = atoi(argv[2]);
    int operation = atoi(argv[3]);
    if (validateArgs(temp, on, operation)) {
        return 1;
    }

    send(temp, on, operation);

    printf("Command sent.\n");

    return 0;
}

bool validateArgs(int temp, int on, int operation) {
    if (temp != 10 && (temp < 16 || temp > 31)) {
        fprintf(stderr, "Invalid temperature!\n");
        return true;
    }
    if (on != 1 && on != 0) {
        fprintf(stderr, "On can only be 1 or 0!\n");
        return true;
    }
    if (operation != 1 && operation != 0) {
        fprintf(stderr, "Unknown operation mode: %d!\n", operation);
        return true;
    }
    return false;
}
