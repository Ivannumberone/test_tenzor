#ifndef INCLUDEDFILE_H
#define INCLUDEDFILE_H

#include <experimental/filesystem>

using namespace std;
using namespace std::experimental::filesystem;

namespace analyzer_dependencies {
namespace helpers {


    class IncludedFile
    {        
    public:        
        IncludedFile();
        IncludedFile(const wstring &fileName,const path &absolute_path);
        wstring getStringId();
        path getAbsolutePath() const;
        wstring getFileName() const;
        bool fileExists() const;
        vector<shared_ptr<IncludedFile>> Childs = vector<shared_ptr<IncludedFile>>();
        bool is_loop;
        int count_included;
        bool operator ==(const IncludedFile &target ) const;
    private:
        path absolute_path_;
        wstring file_name_;
        bool file_exists_;

    };
  }
}
#endif // INCLUDEDFILE_H
