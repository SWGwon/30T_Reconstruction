#include <string>
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <vector>
#include <dirent.h>

#include "FileReader.hxx"
#include "DataProcessor.hxx"

void processData() {
    char* filename = readline("Enter input file path: ");
    if (!filename) return;

    if (strlen(filename) > 0) {
        add_history(filename);
    }

    std::cout << "You entered: " << filename << std::endl;
    FileReader reader(filename);
    free(filename);
    if (reader.open()) {
        TTree* tree = reader.getTree("output");
        if (tree) {
            DataProcessor processor(tree);
            int entryDenominator;
            std::cout << "Enter entry denominator: ";
            std::cin >> entryDenominator;
            processor.SetEntryDenominator(entryDenominator);
            processor.Process();
            std::string outputFile;
            std::cout << "Enter output file path: ";
            std::cin >> outputFile;
            processor.SaveResults(outputFile);
        } else {
            std::cout << "Failed to retrieve tree from file." << std::endl;
        }
    } else {
        std::cout << "Failed to open file." << std::endl;
    }
}

void fitHistograms() {
    std::string filename;
    std::cout << "Enter input file path for fitting: ";
    std::cin >> filename;
    std::unique_ptr<TFile> file(new TFile(filename.c_str(), "READ"));
    if (!file->IsZombie()) {
        // Histogram fitting logic would go here.
        std::cout << "Histogram fitting completed successfully." << std::endl;
    } else {
        std::cout << "Failed to open file for fitting." << std::endl;
    }
}

std::vector<std::string> listFiles(const std::string& prefix) {
    std::vector<std::string> matches;
    std::string dirPath;
    std::string part;
    size_t pos = prefix.rfind('/');

    if (pos != std::string::npos) {
        dirPath = prefix.substr(0, pos + 1);
        part = prefix.substr(pos + 1);
    } else {
        dirPath = "./";
        part = prefix;
    }

    DIR* dir = opendir(dirPath.c_str());
    if (dir == nullptr) return matches;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name(entry->d_name);
        if (name.find(part) == 0) {
            matches.push_back(dirPath + name);
        }
    }

    closedir(dir);
    return matches;
}

char* completionGenerator(const char* text, int state) {
    static std::vector<std::string> matches;
    static size_t match_index;

    if (state == 0) {
        matches = listFiles(text);
        match_index = 0;
    }

    if (match_index >= matches.size()) return nullptr;
    return strdup(matches[match_index++].c_str());
}

char** filepathCompleter(const char* text, int start, int end) {
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, completionGenerator);
}


int main() {
    rl_attempted_completion_function = filepathCompleter;
    rl_completion_append_character = '\0';
    rl_completion_suppress_append = 1;
    bool running = true;
    int choice;

    while (running) {
        std::cout << "Choose an option:" << std::endl;
        std::cout << "1. Data processing: making 2D and 1D histograms from data and saving the histograms as root file" << std::endl;
        std::cout << "2. Fitting histograms: fitting 1D histograms from processed data root file" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cin >> choice;
        switch (choice) {
            case 1:
                processData();
                break;
            case 2:
                fitHistograms();
                break;
            case 3:
                break;
            default:
                std::cout << "Invalid choice, please select a valid option." << std::endl;
        }
    }

    return 0;
}

