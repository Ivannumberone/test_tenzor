#include <experimental/filesystem>
#include <iostream>
#include <vector>
#include "analyzer.h"

using namespace std;
using namespace  experimental::filesystem;

using namespace analyzer_dependencies;

void print_help(string name)
{
    cout << "Use: " << name.data()
              << " /mnt/c/mysources -I /mnt/c/mysources/includes -I /mnt/c/mylibrary"
              << endl;
}

bool ValidateArgs(int arg,char** args)
{
    if (arg <= 2)
    {
        cout << "need more args" << endl;
        return false;
    }
    if (args[1] == nullptr || args[2] == nullptr)
    {
        return false;
    }
    path pathTarget = path(args[1]);    
    path pathLibs = path(args[2]);
    if (!is_directory(pathTarget))
    {
         cout << "folder "<< pathTarget << " not found " << endl;
         return false;
    }
    if (!is_directory(pathLibs))
    {
        cout << "folder "<<pathLibs << " not found " << endl;
        return false;
    }
    return true;
}
int main(int arg,char** args)
{
    arg = 5;
    char* n_argv[] = {"",
        "../AnalyzerDependencies/AnalyzerDependencies/AnalyzerDependenciesProject/src",
        "C://Qt/Tools/mingw730_32/lib/gcc/i686-w64-mingw32/7.3.0/include/c++",
        "../AnalyzerDependencies/AnalyzerDependencies/AnalyzerDependenciesProject/src/helpers/",
        "C://Qt/Tools/mingw730_32/lib/gcc/i686-w64-mingw32/7.3.0/include/c+/regex/+",
        "C://Qt/Tools/mingw730_32/lib/gcc/i686-w64-mingw32/7.3.0/include/c++/experimental/"
    };
    args = n_argv;
    if (!ValidateArgs(arg,args))
        return EXIT_FAILURE;
    Analyzer* analyzer = new Analyzer();
    auto start_folder = path(args[1]);
    auto libs_folders = vector<path>();
    for (int i = 1; i < arg;i++)
    {
        libs_folders.push_back(path(args[i]));
    }
    analyzer->Analyze(start_folder,libs_folders);    
    analyzer->PrintTree();
    analyzer->PrintCount();
    analyzer->PrintLoop();
    return EXIT_SUCCESS;
}

