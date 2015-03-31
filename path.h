
#include <string>
#include <regex>
#include <algorithm>
#include <iostream>

namespace nodeuv {

  using namespace std;

  struct PathObject {
    string root;
    string dir;
    string base;
    string ext;
    string name;
  };

  class Path {

    static string sep;
    static string delimiter;

    cmatch split(string);
    vector<string> normalizePathList(vector<string>, bool);
    vector<string> splitPathList(string);
    string joinPathList(vector<string>);
    string _join(vector<string>);
    string _resolve(vector<string>);

    public:
      bool isAbsolute(string);
      string normalize(string);

      template <typename... Args>
      string join(Args... args);

      template <typename... Args>
      string resolve(Args... args);

      string dirname(const string);

      string basename(string, string);
      string basename(string);

      string extname(const string);

      string format(PathObject);
      PathObject parse(const string);
      PathObject createObject();
  };

  template <typename ...Args>
  string Path::join(Args... argsT) {
    vector<string> args = { (string) argsT... };
    return this->_join(args);
  }

  template <typename ...Args>
  string Path::resolve(Args... argsT) {
    vector<string> args = { (string) argsT... };
    return this->_resolve(args);
  }
}

