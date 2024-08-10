import java.io.*;
import java.util.*;

class SymbolTable {
    String label;
    int address;

    SymbolTable(String label, int address) {
        this.label = label;
        this.address = address;
    }
}

class IntermediateCode {
    int loc;
    String label;
    String opcode;
    String operand;

    IntermediateCode(int loc, String label, String opcode, String operand) {
        this.loc = loc;
        this.label = label;
        this.opcode = opcode;
        this.operand = operand;
    }
}

public class AssemblerPassOne {
    public static void main(String[] args) throws IOException {
        passOne();
    }

    static void passOne() throws IOException {
        HashMap<String, Integer> symbolTable = new HashMap<>();
        ArrayList<IntermediateCode> intermediateCode = new ArrayList<>();

        int locctr = 0, start = 0, prolength = 0;

        try (BufferedReader input = new BufferedReader(new FileReader("input.txt"));
             BufferedReader optab = new BufferedReader(new FileReader("optab.txt"));
             BufferedWriter symbolTableFile = new BufferedWriter(new FileWriter("symbolTable.txt", true));
             BufferedWriter intermediateFile = new BufferedWriter(new FileWriter("intermediate.txt"));
             BufferedWriter lengthFile = new BufferedWriter(new FileWriter("length.txt"))) {

            String line;
            String[] tokens;

            // Read first line
            line = input.readLine();
            tokens = line.split("\\s+");
            String label = tokens[0];
            String opcode = tokens[1];
            String operand = tokens[2];

            if (opcode.equals("START")) {
                start = Integer.parseInt(operand);
                locctr = start;
                intermediateCode.add(new IntermediateCode(0, label, opcode, operand));
                line = input.readLine();
                tokens = line.split("\\s+");
                label = tokens[0];
                opcode = tokens[1];
                operand = tokens[2];
            }

            while (!opcode.equals("END")) {
                if (!label.startsWith(";")) { // Not a comment
                    if (!label.equals("**")) { // Symbol in label field
                        // Check for duplicate label
                        if (symbolTable.containsKey(label)) {
                            System.out.println("Error: Label '" + label + "' already exists.");
                        } else {
                            symbolTable.put(label, locctr);
                        }
                    }

                    // Search optab
                    boolean foundOpcode = false;
                    optab.mark(0); // Reset file pointer
                    String code, mnemonic;
                    while ((line = optab.readLine()) != null) {
                        tokens = line.split("\\s+");
                        code = tokens[0];
                        mnemonic = tokens[1];
                        if (opcode.equals(code)) {
                            locctr += 3;
                            foundOpcode = true;
                            break;
                        }
                    }
                    optab.reset();

                    if (!foundOpcode) {
                        // Handle special opcodes
                        if (opcode.equals("WORD")) {
                            locctr += 3;
                        } else if (opcode.equals("RESW")) {
                            locctr += 3 * Integer.parseInt(operand);
                        } else if (opcode.equals("BYTE")) {
                            locctr += operand.length() - 3; // Assuming C'..' or X'..'
                        } else if (opcode.equals("RESB")) {
                            locctr += Integer.parseInt(operand);
                        } else {
                            System.out.println("Error: Invalid opcode '" + opcode + "'");
                        }
                    }

                    intermediateCode.add(new IntermediateCode(locctr, label, opcode, operand));
                }

                line = input.readLine();
                if (line == null) {
                    break;
                }
                tokens = line.split("\\s+");
                label = tokens[0];
                opcode = tokens[1];
                operand = tokens[2];
            }

            // Write last line to intermediate file
            intermediateCode.add(new IntermediateCode(locctr, label, opcode, operand));

            // Write symbol table and intermediate code to files
            for (Map.Entry<String, Integer> entry : symbolTable.entrySet()) {
                symbolTableFile.write(entry.getKey() + "\t" + entry.getValue() + "\n");
            }
            for (IntermediateCode ic : intermediateCode) {
                intermediateFile.write(ic.loc + "\t" + ic.label + "\t" + ic.opcode + "\t" + ic.operand + "\n");
            }

            // Calculate and write program length
            prolength = locctr - start;
            lengthFile.write(Integer.toString(prolength));

            System.out.println("Pass One complete. Program length is " + prolength + " bytes.");
        }
    }
}
