#include "file_parser.h"

using namespace std;
using namespace std::experimental::filesystem;

namespace analyzer_dependencies {
namespace helpers {
    Parser::Parser()
    {

    }
    Parser::Parser(const vector<path> &lib_folders)
    {
        lib_folders_ = lib_folders;
    }       
    wstring Parser::LoadFile(const std::wstring& filename)
    {
        std::wstring buffer;
        FILE* f = _wfopen(filename.c_str(), L"rtS, ccs=UTF-8");        
        if (f == nullptr)
        {
            return buffer;
        }
        struct _stat fileinfo;
        _wstat(filename.c_str(), &fileinfo);
        size_t filesize = static_cast<size_t>(fileinfo.st_size);
        if (filesize > 0)
        {
            buffer.resize(filesize);
            size_t wchars_read = fread(&(buffer.front()), sizeof(wchar_t), filesize, f);
            buffer.resize(wchars_read);
            buffer.shrink_to_fit();
        }
        fclose(f);
        return buffer;
    }

    vector<shared_ptr<IncludedFile>> Parser::Parse(shared_ptr<IncludedFile> target)
    {
        auto result = vector<shared_ptr<IncludedFile>>();
        if (!exists(target->getAbsolutePath()) || !is_regular_file(target->getAbsolutePath()))
            return result;
        if (!regex_search(target->getAbsolutePath().extension().wstring().data(),rgx_cpp_h_file))
            return result;
        wstring code = LoadFile(target->getAbsolutePath().wstring());
        code = regex_replace(code,rgx_comment_block_,L"");
        code = regex_replace(code,rgx_comment_line_,L"");
        wcmatch match;
        auto str = wstring(code.data());
        while (regex_search(str.data(),match, rgx_include_))
        {
            wstring filename = match.str(1);
            if (!regex_search(filename.data(),rgx_cpp_h_file))
            {
                code = match.suffix().str();
                continue;
            }
            auto absolutePath = SearchFile(filename,target->getAbsolutePath().parent_path(),true);
            result.push_back(shared_ptr<IncludedFile>(new IncludedFile(filename,absolutePath)));
            str = match.suffix().str();
        }
        str = wstring(code.data());
        while (regex_search(code.data(),match, rgx_include_lib_))
        {
            wstring filename = match.str(1);
            if (!regex_search(filename.data(),rgx_cpp_h_file))
            {
                code = match.suffix().str();
                continue;
            }
            auto absolutePath = SearchFile(filename,target->getAbsolutePath(),false);
            result.push_back(shared_ptr<IncludedFile>(new IncludedFile(filename,absolutePath)));
            code = match.suffix().str();
        }
        return result;
    }
    path Parser::SearchFile(const wstring &s_path,const path &base_dir, bool include_type)
    {        
        auto target = path(s_path);
        if (include_type)
        {
            auto code = error_code();
            auto absolutePath = canonical(s_path,base_dir,code);
            if (exists(absolutePath))
            {
                return absolutePath;
            }
        }
        for(const auto &folder: lib_folders_)
        {
            auto code = error_code();
            auto absolutePath = canonical(s_path,folder,code);
            if (exists(absolutePath))
            {
                return absolutePath;
            }
        }
        return path("");
    }    
  }
}

