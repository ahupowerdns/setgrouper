// read all input files, output for each line of text in which of the input files it was found
// public domain code by bert.hubert@netherlabs.nl
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <boost/dynamic_bitset.hpp>
#include <boost/algorithm/string.hpp>
#include "ext/CLI11/CLI11.hpp"

using namespace std;

bool g_ignore_case;
// this allows us to make a Case Insensitive container
struct CIStringCompare: public std::binary_function<string, string, bool> 
{
  bool operator()(const string& a, const string& b) const
  {
    if(g_ignore_case)
      return strcasecmp(a.c_str(), b.c_str()) < 0;
    else
      return a<b;
  }
};

int main(int argc, char**argv)
{
  CLI::App app("setgrouper");

  vector<std::string> files;
  CLI::Option *opt = app.add_option("-f,--file,file", files, "File name");
  CLI::Option *copt = app.add_flag("-i,--ignore-case", g_ignore_case, "Ignore case");
  
  try {
    app.parse(argc, argv);
  } catch(const CLI::Error &e) {
    return app.exit(e);
  }


  typedef map<string, boost::dynamic_bitset<>, CIStringCompare> presence_t;
  presence_t presence;

  string line;
  cout << '\t';

  int n=0;
  for(const auto& f : files) {
    cout << f << '\t';
    ifstream ifs(f);
    if(!ifs) {
      cerr<<"Unable to open '"<<f<<"' for reading\n"<<endl;
      exit(EXIT_FAILURE);
    }

    while(getline(ifs, line)) {
      boost::trim(line);
      if(line.empty())
        continue;
      presence_t::iterator iter = presence.find(line);
      if(iter == presence.end()) { // not present, do a very efficient 'insert & get location'
        iter = presence.insert(make_pair(line, boost::dynamic_bitset<>(files.size()))).first; 
      }
      iter->second[n]=1;
    }
    ++n;
  }
  cout << '\n';

  // this is where we store the reverse map, 'presence groups', so which lines where present in file1, but not file2 etc
  typedef map<boost::dynamic_bitset<>, vector<string> > revpresence_t;
  revpresence_t revpresence;

  for(auto& val : presence) {
    revpresence[val.second].push_back(val.first);
    cout << val.first << '\t';
    for (boost::dynamic_bitset<>::size_type i = 0; i < val.second.size(); ++i) {
      cout << val.second[i] << '\t';
    }
    cout << endl;
  }

  cout << "\nPer group output\t\n";
  for(auto& val: revpresence) {
    cout<<"\nGroup (size="<<val.second.size()<<"): \t";

    for (boost::dynamic_bitset<>::size_type i = 0; i < val.first.size(); ++i) {
      cout << val.first[i] << '\t';
    }

    cout << endl;

    for(string& entry : val.second) {
      cout << entry << "\t\n";
    }
  }
}
