#include <string>
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <vector>
#include <dirent.h>

#include <TCanvas.h>

#include "FileReader.hxx"
#include "DataProcessor.hxx"
#include "HistogramFitter.hxx"

Double_t combinedFunc(Double_t *x, Double_t *par) {
    // 가우시안 파트
    Double_t gauss = par[0] * exp(-0.5 * pow((x[0] - par[1])/par[2], 2));
    // 3차 다항식 파트
    Double_t poly3 = par[3] + par[4]*x[0] + par[5]*x[0]*x[0] + par[6]*x[0]*x[0]*x[0];

    Double_t penalty = 0;
    if (par[2] < 100 || par[2] > 200) {
        penalty += 100 * (par[2] - 150)*(par[2] - 150); // 예시 페널티, 범위를 벗어날수록 \(\chi^2\) 가 증가
    }
    if (par[1] < 400 || par[1] > 600) {
        penalty += 100 * (par[1] - 500)*(par[1] - 500); // 예시 페널티, 범위를 벗어날수록 \(\chi^2\) 가 증가
    }
    return gauss + poly3 + penalty; // 두 함수의 합 반환
}

Double_t combinedFunc2(Double_t *x, Double_t *par) {
    Double_t gauss;
    if (x[0] < 700) {
        gauss = par[0] * exp(-0.5 * pow((x[0] - par[1])/par[2], 2));
    } else {
        gauss = 0;
    }
    Double_t penalty = 0;
    if (par[2] < 100 || par[2] > 200) {
        penalty += 100 * (par[2] - 150)*(par[2] - 150); // 예시 페널티, 범위를 벗어날수록 \(\chi^2\) 가 증가
    }
    if (par[1] < 400 || par[1] > 600) {
        penalty += 100 * (par[1] - 500)*(par[1] - 500); // 예시 페널티, 범위를 벗어날수록 \(\chi^2\) 가 증가
    }
    if (par[4] < 0 || par[4] > 0.03) {
        penalty += 100 * (par[4] - 0.03)*(par[4] - 0.03);
    }

    Double_t poly1;
    if (x[0] > 700) {
        poly1 = par[3] + par[4]*x[0];
    } else {
        poly1 = 0;
    }

    return gauss + poly1 + penalty; // 두 함수의 합 반환
}

Double_t combinedFunc3(Double_t *x, Double_t *par) {
    Double_t gauss;
    if (x[0] < 700) {
        gauss = par[0] * exp(-0.5 * pow((x[0] - par[1])/par[2], 2));
    } else {
        gauss = 0;
    }
    Double_t penalty = 0;
    if (par[2] < 100 || par[2] > 200) {
        penalty += 100 * (par[2] - 150)*(par[2] - 150); // 예시 페널티, 범위를 벗어날수록 \(\chi^2\) 가 증가
    }
    if (par[1] < 400 || par[1] > 600) {
        penalty += 100 * (par[1] - 500)*(par[1] - 500); // 예시 페널티, 범위를 벗어날수록 \(\chi^2\) 가 증가
    }

    Double_t poly1;
    if (x[0] > 700) {
        poly1 = par[3] + par[4]*x[0];
    } else {
        poly1 = 0;
    }

    return gauss + poly1 + penalty; // 두 함수의 합 반환
}

void printHeader(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

void printMenu() {
    printHeader("Main Menu");
    std::cout << "Choose an option:\n"
              << "1. Data processing: Make 2D and 1D histograms from data and save them as a root file\n"
              << "2. Fitting histograms: Fit 1D histograms from a processed data root file\n"
              << "3. Exit\n";
}

int getChoice() {
    int choice;
    std::cout << "Enter your choice: ";
    while (!(std::cin >> choice)) {
        std::cin.clear(); // clear input buffer to restore cin to a usable state
        std::cin.ignore(INT_MAX, '\n'); // ignore last input
        std::cout << "Invalid input. Please enter a number between 1 and 3.\n";
        std::cout << "Enter your choice: ";
    }
    return choice;
}

char* GetNonEmptyInput(const char* prompt) {
    char* input;
    while (true) {
        input = readline(prompt);
        if (!input) {  // NULL 반환은 EOF(입력 종료)를 의미
            std::cout << "No input received. Exiting program." << std::endl;
            exit(1);  // 프로그램 종료
        }
        if (strlen(input) > 0) {  // 입력된 문자열의 길이가 0보다 크면 유효
            return input;
        }
        std::cout << "Empty input is not allowed. Please try again." << std::endl;
        free(input);  // 사용하지 않는 입력 메모리 해제
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

void processData() {
    printHeader("Data Processing");
    char* filename = GetNonEmptyInput("Enter input file path: ");
    add_history(filename);
    FileReader reader(filename);
    free(filename);

    if (reader.open()) {
        TTree* tree = reader.getTree("output");
        if (tree) {
            DataProcessor processor(tree);

            char* entryDenominator = GetNonEmptyInput("Enter entry denominator: ");
            add_history(entryDenominator);
            processor.SetEntryDenominator(std::stoi(entryDenominator));
            free(entryDenominator);

            processor.SetEnergyBinCount(200);
            std::cout << "Start processing..." << std::endl;
            processor.Process();
            std::cout << "Done processing data." << std::endl;

            char* outputFile = GetNonEmptyInput("Enter output file path: ");
            add_history(outputFile);
            processor.SaveResults(outputFile);
            free(outputFile);

        } else {
            std::cout << "Failed to retrieve tree from file." << std::endl;
        }
    } else {
        std::cout << "Failed to open file." << std::endl;
    }
}

void fitHistograms() {
    printHeader("Histogram Fitting");
    char* filename = GetNonEmptyInput("Enter processed input file path for fitting: ");
    add_history(filename);
    std::unique_ptr<TFile> file(new TFile(filename, "READ"));
    free(filename);

    if (!file->IsZombie()) {
        std::cout << "Setting histograms...";
        DataProcessor processor(nullptr);
        HistogramFitter histogramFitter;
        histogramFitter.SetFile(file.get());
        histogramFitter.SetHistogramAxis(processor.GetPmtBinCount(), processor.GetTimeBinCount());
        histogramFitter.InitializeHistograms();
        histogramFitter.SetHistograms();
        std::cout << "done" << std::endl;

        std::cout << "Start fitting..." << std::endl;
        //std::unique_ptr<TF1> func(new TF1("func", combinedFunc1, 100, processor.GetMaxEnergy(), 7));
        std::unique_ptr<TF1> func(new TF1("func", combinedFunc2, 100, processor.GetMaxEnergy(), 5));
        func->SetParLimits(4,0,1000);
        histogramFitter.FitHistograms(func.get(),"",100,processor.GetMaxEnergy()-100);

        char* outputFile = GetNonEmptyInput("Enter output file path: ");
        add_history(outputFile);
        histogramFitter.SaveResults(outputFile);
        free(outputFile);


        // Histogram fitting logic would go here.
        std::cout << "Histogram fitting completed successfully." << std::endl;
    } else {
        std::cout << "Failed to open file for fitting." << std::endl;
    }
}

int main() {
    rl_attempted_completion_function = filepathCompleter;
    rl_completion_append_character = '\0';

    while (true) {
        printMenu();
        int choice = getChoice();

        switch (choice) {
            case 1:
                processData();
                break;
            case 2:
                fitHistograms();
                break;
            case 3:
                std::cout << "Exiting program.\n";
                return 0;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}

