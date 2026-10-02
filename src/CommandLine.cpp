#include "CommandLine.h"

#include "App.h"
#include "Config.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <string>

namespace {

std::string trim(std::string value) {
    const std::size_t first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return {};
    }

    const std::size_t last = value.find_last_not_of(" \t\r\n");
    value = value.substr(first, last - first + 1);

    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
        value = value.substr(1, value.size() - 2);
    }

    return value;
}

void printHelp() {
    std::cout <<
        "\n"
        "Enter the path to a scene file to render it.\n"
        "\n"
        "COMMANDS\n"
        "  help    Show this help message\n"
        "  exit    Close Tarsila Houses\n"
        "\n"
        "EXAMPLES\n"
        "  examples/houses.txt\n"
        "  \"C:\\path with spaces\\scene.txt\"\n"
        "\n"
        "A scene can also be opened directly from the command line:\n"
        "  tarsila-houses examples/houses.txt\n"
        "\n";
}

bool openScene(const std::string& filePath) {
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open scene file: " << filePath << "\n";
        return false;
    }

    file.close();

    try {
        Config config = loadConfig(filePath);
        App app("Tarsila Houses", config);
        app.run();
        return true;
    } catch (const std::exception& exception) {
        std::cerr << "Error: Could not load scene: " << exception.what() << "\n";
        return false;
    }
}

int runInteractive() {
    std::cout << "Tarsila Houses\n";
    std::cout << "Type help to see the available commands.\n\n";

    std::string input;
    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, input)) {
            std::cout << '\n';
            return 0;
        }

        input = trim(input);

        if (input.empty()) {
            continue;
        }

        if (input == "help") {
            printHelp();
            continue;
        }

        if (input == "exit" || input == "quit") {
            return 0;
        }

        openScene(input);
    }
}

} // namespace

int runCommandLine(int argc, char* argv[]) {
    if (argc == 1) {
        return runInteractive();
    }

    const std::string argument = argv[1];

    if (argument == "--help" || argument == "-h") {
        std::cout << "Tarsila Houses\n";
        printHelp();
        return 0;
    }

    if (argc > 2) {
        std::cerr << "Error: Provide only one scene file.\n";
        std::cerr << "Run with --help to see the available options.\n";
        return 1;
    }

    return openScene(argument) ? 0 : 1;
}
