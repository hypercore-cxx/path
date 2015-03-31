#include "../path.h"

namespace nodeuv {

  using namespace std;

  string Path::sep = "/";
  string Path::delimiter = ":";

  regex splitPathRE("^(\\/?|)([\\s\\S]*?)((?:\\.{1,2}|[^\\/]+?|)(\\.[^.\\/]*|))(?:[\\/]*)$");

  bool Path::isAbsolute(string path) {
    return path[0] == '/';
  }

  vector<string> Path::splitPathList(string path) {
    vector<string> result;
    string::size_type start = 0;
    string::size_type end = 0;
    string delimiter = Path::sep;

    while (end != string::npos) {
      end = path.find(delimiter, start);

      if (start < path.size() && start != end) {
        string segment = path.substr(start, end - start);
        result.push_back(segment);
      }
      start = end + delimiter.size();
    }
    return result;
  }

  vector<string> Path::normalizePathList(vector<string> parts, bool allowAboveRoot) {
    vector<string> res;
    int len = parts.size();

    for (int i = 0; i < len; i++) {
      string p = parts[i];

      if (p.size() == 0 || p == ".") {
        continue;
      }

      if (p == "..") {
        if (res.size() && res[res.size() - 1] != "..") {
          res.pop_back();
        }
        else if (allowAboveRoot) {
          res.push_back("..");
        }
      }
      else {
        res.push_back(p);
      }
    }
    return res; 
  }

  string Path::joinPathList(vector<string> list) {
    string res = "";
    int len = list.size();
    int index = 0;

    for (auto el : list) {
      res += el;
      if (++index != len) {
        res += Path::sep;
      }
    }
    return res;
  }

  string Path::normalize(string path) {

    bool isAbs = isAbsolute(path);
    bool trailingSlash = path[path.length() -1] == Path::sep[0];

    path = joinPathList(normalizePathList(splitPathList(path), !isAbs));

    if (!path.length() && !isAbs) {
      path = ".";
    }

    if (path.length() && trailingSlash) {
      path += Path::sep;
    }

    return (isAbs ? Path::sep : "") + path;
  }

  cmatch Path::split(string path) {
    cmatch res;
    regex_search(path.c_str(), res, splitPathRE);
    return res;
  }

  string Path::_join(vector<string> args) {

    string path = "";

    for (auto &el : args) {
      if (el.length()) {
        if (!path.length()) {
          path += el;
        } 
        else {
          path += Path::sep + el;
        }
      }
    }
    return this->normalize(path);
  }

  string Path::_resolve(vector<string> args) {

    string resolvedPath = "";
    bool resolvedAbsolute = false;

    for (int i = args.size() - 1; i >= -1 && !resolvedAbsolute; i--) {
      string path = args[i]; /* process.cwd() */

      if (path == "") {
        continue;
      }

      resolvedPath = path + Path::sep + resolvedPath;
      resolvedAbsolute = path[0] == Path::sep[0];
    }

    // At this point the path should be resolved to a full absolute path, but
    // handle relative paths to be safe (might happen when process.cwd() fails)

    // Normalize the path
    resolvedPath = joinPathList(
      normalizePathList(splitPathList(resolvedPath), !resolvedAbsolute)
    );

    string p = ((resolvedAbsolute ? Path::sep : "") + resolvedPath);

    if (!p.size()) {
      return ".";
    }

    return p;
  }

  string Path::dirname(const string path) {
    cmatch result = this->split(path);
    string root;
    string dir;

    if (result[1].length()) {
      root = result[1];
    }

    if (result[2].length()) {
      dir = result[2];
    }

    if (!root.length() && !dir.length()) {
      return ".";
    }

    if (dir.length()) {
      dir = dir.substr(0, dir.length() - 1);
    }
    return root + dir;
  }

  PathObject Path::createObject() {
    PathObject object;
    return object;
  }

  PathObject Path::parse(const string path) {
    PathObject o;

    auto allParts = this->split(path);

    if (!allParts.size() || allParts.size() != 5) {
      throw runtime_error("Invalid path '" + path + "'");
    }

    o.root = allParts[1];
    o.dir = string(allParts[1]) + string(allParts[2]).substr(0, allParts[2].length() - 1);
    o.base = allParts[3];
    o.ext = allParts[4];
    o.name = string(allParts[3]).substr(0, allParts[3].length() - allParts[4].length());
    return o;
  }

  string Path::format(PathObject o) {
    string dir = o.dir.length() ? o.dir + Path::sep : "";
    return dir + o.base;
  }

  string Path::basename(string path) {
    return this->basename(path, "");
  }

  string Path::basename(string path, string ext) {
    string f = this->split(path)[3];

    if (ext.length() > 0 && f.substr(f.length() - ext.length(), f.length()) == ext) {
      f = f.substr(0, f.length() - ext.length());
    }
    return f;
  }

  string Path::extname(const string path) {
    return this->split(path)[4];
  }
}

