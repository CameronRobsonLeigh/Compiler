#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

// Define a Token structure for future use
struct Token {
    std::string type;
    std::string value;
};

// Function to read the source file into a string
std::string readFile(const std::string& filename) {
    std::ifstream sourceFile(filename);
    if (!sourceFile) {
        std::cerr << "Error opening source file: " << filename << std::endl;
        return "";
    }

    std::string sourceCode;
    std::string line;
    while (std::getline(sourceFile, line)) {
        sourceCode += line + "\n";
    }
    sourceFile.close();
    return sourceCode;
}

// Function to tokenize the source code
std::vector<Token> tokenize(const std::string& sourceCode) {
    std::vector<Token> tokens;

    // Tokenize based on simple rules (e.g., split by space, detect keywords)
    std::istringstream stream(sourceCode);
    std::string word;
    while (stream >> word) {
        if (word == "return") {
            tokens.push_back({"KEYWORD", "return"});
        } else {
            tokens.push_back({"NUMBER", word});
        }
    }

    return tokens;
}

// Function to parse tokens and extract the return value
int parseTokens(const std::vector<Token>& tokens) {
    if (tokens.empty() || tokens[0].type != "KEYWORD" || tokens[0].value != "return") {
        std::cerr << "\"return\" statement not found or invalid syntax.\n";
        return 0;
    }

    if (tokens.size() < 2 || tokens[1].type != "NUMBER") {
        std::cerr << "Expected a number after \"return\".\n";
        return 0;
    }

    // Convert the second token's value to an integer
    return std::stoi(tokens[1].value);
}

// Function to generate assembly code from the parsed return value
void generateAssembly(int value) {
    std::ofstream outFile("program.s");

    if (!outFile) {
        std::cerr << "Error opening file for writing assembly!" << std::endl;
        return;
    }

    // Assembly code for 64-bit mode
    outFile << "section .text" << std::endl;
    outFile << "global _start" << std::endl;
    outFile << "" << std::endl;
    outFile << "_start:" << std::endl;
    outFile << "    mov rax, 60" << std::endl;        // Syscall number for exit
    outFile << "    mov rdi, " << value << std::endl; // Exit status (value passed)
    outFile << "    syscall" << std::endl;           // Perform the system call

    outFile.close();
    std::cout << "Assembly code generated in 'program.s'" << std::endl;
}

// Main function to orchestrate the compilation process
int main() {
    // Step 1: Read the source code file
    std::string sourceCode = readFile("program.camlang");
    if (sourceCode.empty()) {
        return 1; // Exit if file reading fails
    }

    // Step 2: Tokenize the source code
    std::vector<Token> tokens = tokenize(sourceCode);

    // Step 3: Parse the tokens to extract the return value
    int returnValue = parseTokens(tokens);

    // Step 4: Generate assembly code based on the return value
    generateAssembly(returnValue);

    return 0;
}
