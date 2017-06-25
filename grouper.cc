// read all input files, output for each line of text in which of the input files it was found
// public domain code by bert.hubert@netherlabs.nl
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <strings.h>
#include <fstream>
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

static void trim(std::string& in)
{
  if(in.empty())
    return;
  auto pos=in.find_last_not_of(" \t\r\n");
  if(pos != string::npos)
    in.resize(pos+1);
}

int main(int argc, char**argv)
{
  CLI::App app("setgrouper");

  vector<std::string> files;
  app.add_option("-f,--file,file", files, "File name");
  app.add_flag("-i,--ignore-case", g_ignore_case, "Ignore case");
  
  try {
    app.parse(argc, argv);
  } catch(const CLI::Error &e) {
    return app.exit(e);
  }


  typedef map<string, vector<bool>, CIStringCompare> presence_t;
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
      trim(line);
      if(line.empty())
        continue;
      presence_t::iterator iter = presence.find(line);
      if(iter == presence.end()) { // not present, do a very efficient 'insert & get location'
        iter = presence.insert(make_pair(line, vector<bool>(files.size()))).first; 
      }
      iter->second[n]=1;
    }
    ++n;
  }
  cout << '\n';

  // this is where we store the reverse map, 'presence groups', so which lines where present in file1, but not file2 etc
  typedef map<vector<bool>, vector<string> > revpresence_t;
  revpresence_t revpresence;

  for(const auto& val : presence) {
    revpresence[val.second].push_back(val.first);
    cout << val.first << '\t';
    for (vector<bool>::size_type i = 0; i < val.second.size(); ++i) {
      cout << val.second[i] << '\t';
    }
    cout << endl;
  }

  cout << "\nPer group output\t\n";
  for(const auto& val: revpresence) {
    cout<<"\nGroup (size="<<val.second.size()<<"): \t";

    for (vector<bool>::size_type i = 0; i < val.first.size(); ++i) {
      cout << val.first[i] << '\t';
    }

    cout << endl;

    for(const string& entry : val.second) {
      cout << entry << "\t\n";
    }
  }
}
