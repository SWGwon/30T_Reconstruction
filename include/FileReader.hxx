#ifndef FILEREADER_HXX
#define FILEREADER_HXX

#include <TFile.h>
#include <TTree.h>
#include <string>

class FileReader {
    public:
        FileReader(const std::string& rootFileName);
        ~FileReader();

        bool open();
        TTree* getTree(const std::string& treeName);

    private:
        std::string rootFileName;
        std::unique_ptr<TFile> rootFile;
};

#endif // FILEREADER_HXX
