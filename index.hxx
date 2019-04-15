#ifndef PATH_H
#define PATH_H

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>

namespace Path {
  using String = std::string;

  struct PathObject {
    String root;
    String dir;
    String base;
    String ext;
    String name;
  };

  inline char sep () {
#if defined _WIN32 || defined __CYGWIN__
    return '\\';
#else
    return '/';
#endif
  }

  String delimiter();

  bool isAbsolute(const String&);
  String normalize(const String&);
  String basename(const String&, const String&);
  String basename(const String&);
  String dirname(const String&);
  String extname(const String&);

  String format(PathObject&);
  PathObject parse(const String&);
  PathObject createObject();

  String joinVector(const std::vector<String>&);
  String resolveFromVector(const std::vector<String>&);

  template <typename ...Args>
  String join(Args... argsT) {
    std::vector<String> args = { (String) argsT... };
    return Path::joinVector(args);
  }

  template <typename ...Args>
  String resolve(Args... argsT) {
    std::vector<String> args = { (String) argsT... };
    return Path::resolveFromVector(args);
  }

  /*
  smatch split(string);
  vector<string> normalizePathList(vector<string>, bool);
  vector<string> splitPathList(string);
  string joinPathList(vector<string>);
  string _join(vector<string>);
  string _resolve(vector<string>);

  template <typename... Args>
  string join(Args... args);

  template <typename... Args>
  string resolve(Args... args);

  string dirname(const string);


  template <typename ...Args>
  string join(Args... argsT) {
    vector<string> args = { (string) argsT... };
    return Path::_join(args);
  }

  template <typename ...Args>
  string resolve(Args... argsT) {
    vector<string> args = { (string) argsT... };
    return Path::_resolve(args);
  } */
}

#endif
