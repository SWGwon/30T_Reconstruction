#include "CommandLineParser.hxx"
#include <iostream>
#include <sstream>

CommandLineParser::CommandLineParser() {
    this->addOption("inputFileName", 'i', required_argument, "Path to the input file");
    this->addOption("entryDenominator", 'd', required_argument, "denominator for events loop");
    this->addOption("help", 'h', no_argument, "Display this help message");
}

void CommandLineParser::addOption(const std::string& longOption, char shortOption, int has_arg, const std::string& help) {
    OptionDetails details = {longOption, shortOption, has_arg, help, "", false};
    optionsMap[longOption] = details;
    longOptions.push_back({longOption.c_str(), has_arg, nullptr, shortOption});
    helpTexts.push_back("--" + longOption + " (" + shortOption + ") : " + help);
}

void CommandLineParser::parse(int argc, char* argv[]) {
    longOptions.push_back({0, 0, 0, 0});  // End of options

    std::stringstream ss;
    for (const auto& opt : optionsMap) {
        ss << opt.second.shortOption;
        if (opt.second.hasArg == required_argument) {
            ss << ":";
        } else if (opt.second.hasArg == optional_argument) {
            ss << "::";
        }
    }

    int option_index = 0;
    int c;
    while ((c = getopt_long(argc, argv, ss.str().c_str(), &longOptions[0], &option_index)) != -1) {
        for (const auto& opt : optionsMap) {
            if (opt.second.shortOption == c) {
                optionsMap[opt.first].set = true;
                if (opt.second.hasArg) {
                    optionsMap[opt.first].value = optarg ? optarg : "";
                }
                break;
            }
        }
    }
}

bool CommandLineParser::isSet(const std::string& option) const {
    auto it = optionsMap.find(option);
    return it != optionsMap.end() && it->second.set;
}

std::string CommandLineParser::getOptionValue(const std::string& option) const {
    auto it = optionsMap.find(option);
    if (it != optionsMap.end() && it->second.set) {
        return it->second.value;
    }
    return "";
}

void CommandLineParser::printHelp() const {
    for (const auto& text : helpTexts) {
        std::cout << text << std::endl;
    }
}

