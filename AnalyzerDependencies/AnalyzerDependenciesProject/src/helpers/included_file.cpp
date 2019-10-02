#include "included_file.h"

using namespace std;
using namespace std::experimental::filesystem;

namespace analyzer_dependencies {
namespace helpers {

    IncludedFile::IncludedFile()
    {
        count_included = 0;
        is_loop = false;
    }
    IncludedFile::IncludedFile(const wstring &fileName,const path &absolute_path)
    {
        file_name_ = fileName;
        auto code = error_code();
        wstring tmp = canonical(absolute_path,code);
        #ifdef _WIN32
        transform(tmp.begin(), tmp.end(), tmp.begin(), towlower);
        #endif
        absolute_path_ = path(tmp);
        file_exists_ = exists(absolute_path);       
        count_included = 0;
        is_loop = false;
    }
    path IncludedFile::getAbsolutePath() const
    {
        return absolute_path_;
    }
    wstring IncludedFile::getFileName() const
    {
        return file_name_;
    }        
    bool IncludedFile::fileExists() const
    {
        return file_exists_;
    }
    bool IncludedFile::operator ==(const IncludedFile &target) const
    {
        if (!file_exists_ && !target.file_exists_)
        {
            return file_name_ == target.getFileName();
        }
        return absolute_path_ == target.getAbsolutePath();
    }
  }
}
