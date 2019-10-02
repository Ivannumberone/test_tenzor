#pragma warning(Level: Warning)
#define BOOST_TEST_MODULE Test
#include <iostream>
#include <vector>
#include <boost/test/included/unit_test.hpp>
#include "analyzer.h"

using namespace std;
using namespace analyzer_dependencies;

BOOST_AUTO_TEST_SUITE(Test)

BOOST_AUTO_TEST_CASE(test1_simple_test)
{
    auto start_folder = path("../AnalyzerDependencies/AnalyzerDependencies/test/tests/test1/source");
    auto libs_folders = vector<path>();
    libs_folders.push_back(path("../AnalyzerDependencies/AnalyzerDependencies/test/tests/test1/libs"));
    libs_folders.push_back(path("../AnalyzerDependencies/AnalyzerDependencies/test/tests/test1/libs2"));
    Analyzer target = Analyzer();
    target.Analyze(start_folder,libs_folders);
    BOOST_CHECK_EQUAL(target.GetAllFiles().size(),3);
    BOOST_CHECK_EQUAL(target.GetLoopedFiles().size(),0);
}
BOOST_AUTO_TEST_CASE(test3_deep_tree)
{
    auto start_folder = path("../AnalyzerDependencies/AnalyzerDependencies/test/tests/test3_deep_tree/source");
    auto libs_folders = vector<path>();
    libs_folders.push_back(path("../AnalyzerDependencies/AnalyzerDependencies/test/tests/test3_deep_tree/libs"));
    libs_folders.push_back(path("../AnalyzerDependencies/AnalyzerDependencies/test/tests/test3_deep_tree/libs2"));
    Analyzer target = Analyzer();
    target.Analyze(start_folder,libs_folders);
    BOOST_CHECK_EQUAL(target.GetAllFiles().size(),6);
    BOOST_CHECK_EQUAL(target.GetLoopedFiles().size(),0);
}
BOOST_AUTO_TEST_CASE(test4_include_itself)
{
    auto start_folder = path("../AnalyzerDependencies/AnalyzerDependencies/test/tests/test4_include_itself/source");
    auto libs_folders = vector<path>();
    libs_folders.push_back(path("../AnalyzerDependencies/AnalyzerDependencies/test/tests/test4_include_itself/libs"));
    libs_folders.push_back(path("../AnalyzerDependencies/AnalyzerDependencies/test/tests/test4_include_itself/libs2"));
    Analyzer target = Analyzer();
    target.Analyze(start_folder,libs_folders);
    BOOST_CHECK_EQUAL(target.GetAllFiles().size(),1);
    BOOST_CHECK_EQUAL(target.GetLoopedFiles().size(),1);
}
BOOST_AUTO_TEST_CASE(test5_duplicate_file_name)
{
    auto start_folder = path("../AnalyzerDependencies/AnalyzerDependencies/test/tests/test5_duplicate_file_name/source");
    auto libs_folders = vector<path>();
    libs_folders.push_back(path("../AnalyzerDependencies/AnalyzerDependencies/test/tests/test5_duplicate_file_name/libs"));
    libs_folders.push_back(path("../AnalyzerDependencies/AnalyzerDependencies/test/tests/test5_duplicate_file_name/libs2"));
    Analyzer target = Analyzer();
    target.Analyze(start_folder,libs_folders);
    BOOST_CHECK_EQUAL(target.GetAllFiles().size(),4);
    BOOST_CHECK_EQUAL(target.GetLoopedFiles().size(),0);
}
BOOST_AUTO_TEST_CASE(test5_my_projects)
{
    auto start_folder = path("../AnalyzerDependencies/AnalyzerDependencies/AnalyzerDependenciesProject/src");
    auto libs_folders = vector<path>();
    libs_folders.push_back(path("C://Qt/Tools/mingw730_32/lib/gcc/i686-w64-mingw32/7.3.0/include/c++"));
    libs_folders.push_back(path("../AnalyzerDependencies/AnalyzerDependencies/AnalyzerDependenciesProject/src/helpers/"));
    Analyzer target = Analyzer();
    target.Analyze(start_folder,libs_folders);
    BOOST_CHECK_EQUAL(target.GetAllFiles().size(),6);
    BOOST_CHECK_EQUAL(target.GetLoopedFiles().size(),0);
}
BOOST_AUTO_TEST_SUITE_END()

