#include <stdio.h>
#include <string.h>
#include <stdlib.h>
FILE *f1;

void absloader();

int main() {
    f1 = fopen("input.txt", "r");
    
    if (f1 == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    absloader();
    fclose(f1);
    return 0;
}

void absloader() {
    char prog_name[10];
    char line[100];
    char ogprog_name[20] = {0};
    char address[7]; // to hold the start address (6 digits + null terminator)
    char c;
    int j = 0;
    int startog;
    
    printf("Enter the program name: ");
    scanf("%s", prog_name);

    if (fgets(line, sizeof(line), f1) == NULL) {
        printf("Error reading file!\n");
        return;
    }
    line[strcspn(line, "\n")] = 0;
    printf("Line from file: %s\n", line);
    
    // Extract program name from the line after T^
    char *start = strchr(line, '^'); 
    start++;
    
    // Extract the program name (assuming it's 6 characters long)
    while (*start != '^' && *start != '\0' && j < sizeof(ogprog_name) - 1) {
        ogprog_name[j++] = *start++;
    }
    ogprog_name[j] = '\0';
    printf("Program name from file: %s\n", ogprog_name);
    
    // Compare with input program name
    if (strcmp(prog_name, ogprog_name) == 0) {
        printf("Program name verified successfully!\n");
        
        // Read next line for the start address
        fgets(line, sizeof(line), f1);

        // Process lines until 'E' is found (End of program)
        while (line[0] != 'E') {
            char *start_address = strchr(line, '^');
            if (start_address) {
                start_address++;  // Move past the first '^'
                    strncpy(address, start_address, 6);
                    address[7] = '\0'; // Null-terminate the address string
                    printf("Start address: %s\n", address);
                    startog = atoi(address);
                    int k=12;//start from records after text length
                    int m;
                    int len = strlen(line)-2; // for taking each 2 bytes of data toplace in locn
                    while(k<=len-1){
                        m=k;
                        printf("%d  %c%c\n",startog,line[m],line[m+1]);
                        m=m+1;
                        startog++;
                        k=k+2;
                        if(k==18)k++;
                    }
            }
            fgets(line, sizeof(line), f1);
        }
    } else {
        printf("Program name doesn't match!!\n");
    }
}
