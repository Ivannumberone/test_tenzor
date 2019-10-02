#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <experimental/filesystem>

#include <iostream>
#include <fstream>
#include <regex>
#include <streambuf>
#include <vector>

#include "included_file.h"

using namespace std;
using namespace  std::experimental::filesystem;

namespace analyzer_dependencies {
namespace helpers {

    class Parser final
    {
     public:
        Parser();
        Parser(const vector<path> &lib_folders);
        vector<shared_ptr<IncludedFile>> Parse(shared_ptr<IncludedFile> target);
     private:
        wregex rgx_comment_line_ = wregex(LR"(//.*)",regex::icase);
        wregex rgx_comment_block_ = wregex(LR"(/\*.*?\*/)",regex::icase | regex::extended);
        wregex rgx_include_lib_ = wregex(LR"(#include\s*<(.*?)>)");
        wregex rgx_include_ = wregex(LR"(#include\s*\"(.*?)\")");
        wregex rgx_cpp_h_file = wregex(LR"(\.cpp$|\.h$)");
        vector<path> lib_folders_;
        path SearchFile(const wstring &s_path,const path &base_dir,bool include_type);
        wstring LoadFile(const std::wstring& filename);

    };
}

}
#endif // FILEPARSER_H
