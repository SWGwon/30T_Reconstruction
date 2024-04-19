#include "FileReader.hxx"
#include "DataProcessor.hxx"
#include "CommandLineParser.hxx"

int main(int argc, char* argv[]) {
    std::string filename;
    int entryDenominator = 1;
    CommandLineParser parser;

    parser.parse(argc, argv);

    if (parser.isSet("help")) {
        parser.printHelp();
        return 0;
    }

    if (parser.isSet("inputFileName")) {
        filename = parser.getOptionValue("inputFileName");
        std::cout << "input file provided: " << filename << std::endl;
    } else {
        std::cout << "input file should be provided: " << std::endl;
        return 0;
    }

    if (parser.isSet("entryDenominator")) {
        entryDenominator = std::stoi(parser.getOptionValue("entryDenominator"));
    } 

    FileReader reader(filename.c_str());
    if (reader.open()) {
        TTree* tree = reader.getTree("output");
        std::cout << "tree->GetEntries()" << std::endl;
        std::cout << tree->GetEntries() << std::endl;
        DataProcessor processor(tree);
        processor.InitializeHistograms();
        processor.SetEntryDenominator(entryDenominator);
        processor.Process();
        processor.SaveResults("output.root");
    }
    return 0;
}

