#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char label[31];
    int address;
} SymbolTable;

typedef struct {
    int loc;
    char label[31];
    char opcode[31];
    char operand[31];
} IntermediateCode;

void passOne() {
    IntermediateCode inter;
    SymbolTable sym;
    int locctr, start, prolength;
    char code[31], mnemonic[31];

    FILE *fp1, *fp2, *fp3, *fp4, *fp5; // input, optab, symbol, intermediate, length

    fp1 = fopen("input.txt", "r");
    fp2 = fopen("optab.txt", "r");
    fp3 = fopen("symbolTable.txt", "a+");
    fp4 = fopen("intermediate.txt", "w");
    fp5 = fopen("length.txt", "w");

    if (!fp1 || !fp2 || !fp3 || !fp4 || !fp5) {
        printf("\nError: Unable to open one or more files.\n");
        exit(1);
    }

    fscanf(fp1, "%s %s %s", inter.label, inter.opcode, inter.operand); // read first line

    if (strcmp(inter.opcode, "START") == 0) {
        start = atoi(inter.operand);
        locctr = start; // set locctr to start address
        fprintf(fp4, "\t%s\t%s\t%s\n", inter.label, inter.opcode, inter.operand); // start has no locctr
        fscanf(fp1, "%s %s %s", inter.label, inter.opcode, inter.operand); // read next line
    } else {
        locctr = 0;
    }

    while (strcmp(inter.opcode, "END") != 0) { // while opcode != END
        if (inter.label[0] != ';') { // if it is not a comment line
            if (strcmp(inter.label, "**") != 0) { // if there is a symbol in label field
                // Search the symbol in the symbol table
                int found = 0;
                rewind(fp3); // Reset file pointer to the beginning
                while (fscanf(fp3, "%s\t%d", sym.label, &sym.address) != EOF) {
                    if (strcmp(sym.label, inter.label) == 0) {
                        found = 1;
                        printf("\nError: Label '%s' already exists in the symbol table.\n", inter.label);
                        break;
                    }
                }
                if (!found) {
                    // If the label is not found, add it to the symbol table
                    fprintf(fp3, "%s\t%d\n", inter.label, locctr);
                    fflush(fp3); // Ensure that the file is updated
                }
            }

            // Search for the opcode in optab
            int foundOpcode = 0;
            rewind(fp2); // Reset file pointer to the beginning of optab
            while (fscanf(fp2, "%s\t%s", code, mnemonic) != EOF) {
                if (strcmp(inter.opcode, code) == 0) {
                    locctr += 3; // Increment locctr by 3 for each instruction
                    foundOpcode = 1;
                    break;
                }
            }

            if (!foundOpcode) {
                // Handle special opcodes like WORD, RESW, BYTE, RESB
                if (strcmp(inter.opcode, "WORD") == 0) {
                    locctr += 3;
                } else if (strcmp(inter.opcode, "RESW") == 0) {
                    locctr += 3 * atoi(inter.operand);
                } else if (strcmp(inter.opcode, "BYTE") == 0) {
                    locctr += strlen(inter.operand) - 3; // assuming C'..' or X'..'
                } else if (strcmp(inter.opcode, "RESB") == 0) {
                    locctr += atoi(inter.operand);
                } else {
                    printf("\nError: Invalid opcode '%s'.\n", inter.opcode);
                }
            }

            // Write to the intermediate file
            fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, inter.label, inter.opcode, inter.operand);
        }

        // Read the next line from the input file
        fscanf(fp1, "%s %s %s", inter.label, inter.opcode, inter.operand);
    }

    // Write the last line to the intermediate file
    fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, inter.label, inter.opcode, inter.operand);

    // Calculate program length
    prolength = locctr - start;
    fprintf(fp5, "%d\n", prolength);

    // Close all files
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);

    printf("\nPass One complete. Program length is %d bytes.\n", prolength);
}

int main() {
    passOne();
    return 0;
}
