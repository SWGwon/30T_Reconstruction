#ifndef COMMANDLINEPARSER_HXX
#define COMMANDLINEPARSER_HXX

#include <getopt.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>

class CommandLineParser {
    public:
        CommandLineParser();
        void addOption(const std::string& longOption, char shortOption, int has_arg, const std::string& help);
        void parse(int argc, char* argv[]);
        bool isSet(const std::string& option) const;
        std::string getOptionValue(const std::string& option) const;
        void printHelp() const;

    private:
        struct OptionDetails {
            std::string longOption;
            char shortOption;
            int hasArg;
            std::string help;
            std::string value;
            bool set;
        };

        std::map<std::string, OptionDetails> optionsMap;
        std::vector<option> longOptions; // getopt_long structure
        std::vector<std::string> helpTexts;
};

#endif // COMMANDLINEPARSER_HXX

