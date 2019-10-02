#include "analyzer.h"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace experimental::filesystem;
using namespace analyzer_dependencies::helpers;

namespace analyzer_dependencies {

    Analyzer::Analyzer()
    {

    }
    void Analyzer::Analyze(path start_folder,vector<path> lib_folders)
    {
        roots_files_.clear();
        cache_all_files_.clear();
        cache_branch_files_.clear();        
        start_folder_ = start_folder;
        parser_ = Parser(lib_folders);
        for(auto& file: recursive_directory_iterator(start_folder_))
        {
            if(regex_search(file.path().wstring(), rgx_cpp_file_))
            {
                auto absolute_path = canonical(file.path());
                auto tmp = shared_ptr<IncludedFile>(new IncludedFile(file.path().filename().wstring(),absolute_path));
                roots_files_.push_back(tmp);
            }
        }
        AnalyzeFiles(roots_files_);
    }

    shared_ptr<IncludedFile> Analyzer::FindFile(vector<shared_ptr<IncludedFile>> arr,shared_ptr<IncludedFile> target)
    {
         auto tmp = find_if(arr.begin(),arr.end(),[&](std::shared_ptr<IncludedFile> const& current){return *current.get() == *target.get();});

         if (tmp == arr.end())
         {
             return nullptr;
         }
         return tmp[0];
    }    
    void Analyzer::AnalyzeFiles(vector<shared_ptr<IncludedFile>> &targets)
    {
        for (auto target: targets)
        {
            CheckLoop(target);
            //если нет в кэше то нужно добавить и рекурсивно обработать
            //если есть значит файл уже обработан и нужно идти дальше
            if  (FindFile(cache_all_files_,target) == nullptr)
            {
                cache_all_files_.push_back(target);
            }
            else
            {
                auto tmp = FindFile(cache_all_files_,target);
                target = tmp;
                continue;
            }
            if (FindFile(cache_branch_files_,target) != nullptr)
            {
                continue;
            }
            cache_branch_files_.push_back(target);
            vector<shared_ptr<IncludedFile>> tmp = parser_.Parse(target);
            AnalyzeFiles(tmp);
            for(const auto &file: tmp)
            {
                if (file->getAbsolutePath() != target->getAbsolutePath())
                {
                    auto tmp2 = FindFile(cache_all_files_,file);
                    target->Childs.push_back(tmp2);
                }
            }
            cache_branch_files_.pop_back();

        }
    }
    void Analyzer::CheckLoop(shared_ptr<IncludedFile> target)
    {
        bool flag = false;
        for (auto &p: cache_branch_files_)
        {
            if (*p == *target)
            {
                flag = true;
            }
            if (flag)
            {
                target->is_loop = true;
                if(FindFile(cache_looped_files_,target) == nullptr)
                    cache_looped_files_.push_back(target);
            }
        }
    }    
    vector<shared_ptr<IncludedFile>> Analyzer::GetAllFiles()
    {
        return cache_all_files_;
    }
    vector<shared_ptr<IncludedFile>> Analyzer::GetLoopedFiles()
    {
        return cache_looped_files_;
    }
    bool cmp(shared_ptr<IncludedFile> first,
             shared_ptr<IncludedFile> second)
    {
        // Список частот включений файлов должен быть отсортирован в порядке убывания частоты;
        // записи с одинаковым значением частоты сортируются по алфавиту.
        if (first->count_included == second->count_included)
        {
            return first->getFileName().compare(second->getFileName()) == 1;
        }
        return first->count_included > second->count_included;
    }
    void Analyzer::PrintCount()
    {        
        sort(cache_all_files_.begin(),cache_all_files_.end(),cmp);        
        for (auto result: cache_all_files_)
        {
            cout << result->getFileName() << " " << result->count_included << endl;
        }
        cout << endl;
    }

    void Analyzer::PrintTree()
    {
        for(const auto &included_file: roots_files_)
        {
            auto target = FindFile(cache_all_files_,included_file);
            PrintBranch(target,0);
        }
        cout <<endl;
    }
    void Analyzer::PrintBranch(shared_ptr<IncludedFile> included_file,unsigned int count)
    {        
        cout << string(count * 2, '.') <<  included_file->getFileName();
        if (!included_file->fileExists())
        {            
            cout << "(!)";
        }
        cout << endl;
        for (const auto &included_file: included_file->Childs)
        {            
            included_file->count_included++;
            PrintBranch(included_file,count + 1);
        }        
    }    
    void Analyzer::PrintLoop()
    {
        cout << "LoopedFiles:";
        if (cache_looped_files_.size() == 0)
        {
            cout << " none" << endl;
            return;
        }
        cout << endl;
        for (const auto &file: cache_looped_files_)
        {
            cout << file->getFileName()  << endl;
        }
    }
}

