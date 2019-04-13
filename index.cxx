#include "index.hxx"
#include "deps/datcxx/cxx-util/index.hxx"

#include <vector>

namespace Path {
  using String = std::string;

  String delimiter () {
    return ":";
  }

  bool isAbsolute(const String& path) {
    return path[0] == Path::sep();
  }

  String basename(const String& path, const String& ext) {
    std::vector<String> parts = Util::String::split(path, String(1, Path::sep()));

    if (parts.empty()) {
      return "";
    }

    const auto begin = parts.begin();

    for (size_t i=0; i < parts.size(); i++) {
      if (!parts[i].empty()) {
        continue;
      }

      parts.erase(begin + i);
    }

    String s = parts.back();

    if (s.substr(s.length() - ext.length(), s.length()) == ext) {
      s = s.substr(0, s.length() - ext.length());
    }

    return s;
  }

  String basename(const String& path) {
    String ext;
    return Path::basename(path, ext);
  }

  String extname(const String& path) {
    std::vector<String> parts = Util::String::split(path, String(1, Path::sep()));
    auto last = parts.back();
    return last;
  }

  /*

  vector<string> splitPathList(string path) {
    vector<string> result;
    string::size_type start = 0;
    string::size_type end = 0;
    string delimiter = Path::sep();

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

  vector<string> normalizePathList(vector<string> parts, bool allowAboveRoot) {
    vector<string> res;
    int len = parts.size();

    for (int i = 0; i < len; i++) {
      string p = parts[i];

      if (p.empty() || p == ".") {
        continue;
      }

      if (p == "..") {
        if (res.empty() && res[res.size() - 1] != "..") {
          res.pop_back();
        }
        else if (allowAboveRoot) {
          res.emplace_back("..");
        }
      }
      else {
        res.push_back(p);
      }
    }
    return res; 
  }

  string joinPathList(vector<string> list) {
    string res;
    int len = list.size();
    int index = 0;

    for (const auto& el : list) {
      res += el;
      if (++index != len) {
        res += Path::sep();
      }
    }
    return res;
  }

  string normalize(string path) {

    bool isAbs = Path::isAbsolute(path);
    bool trailingSlash = path[path.length() -1] == Path::sep()[0];

    path = Path::joinPathList(normalizePathList(splitPathList(path), !isAbs));

    if (path.length() == 0 && !isAbs) {
      path = ".";
    }

    if (path.length() > 0 && trailingSlash) {
      path += Path::sep();
    }

    return (isAbs ? Path::sep() : "") + path;
  }

  std::vector<std::string> split(string path) {
    regex RE("^(\\/?|)([\\s\\S]*?)((?:\\.{1,2}|[^\\/]+?|)(\\.[^.\\/]*|))(?:[\\/]*)$");
    cmatch res;
    regex_search(path, &res, RE);
    return res;
  }

  string _join(vector<string> args) {

    string path;

    for (const auto &el : args) {
      if (el.length() > 0) {
        if (path.length() == 0) {
          path += el;
        } 
        else {
          path += Path::sep() + el;
        }
      }
    }
    return Path::normalize(path);
  }

  string _resolve(vector<string> args) {

    string resolvedPath = "";
    bool resolvedAbsolute = false;

    for (int i = args.size() - 1; i >= -1 && !resolvedAbsolute; i--) {
      string path = args[i];

      if (path.empty()) {
        continue;
      }

      resolvedPath = path + Path::sep() + resolvedPath;
      resolvedAbsolute = path[0] == Path::sep()[0];
    }

    // At this point the path should be resolved to a full absolute path, but
    // handle relative paths to be safe (might happen when process.cwd() fails)

    // Normalize the path
    resolvedPath = Path::joinPathList(
      Path::normalizePathList(splitPathList(resolvedPath), !resolvedAbsolute)
    );

    string p = ((resolvedAbsolute ? Path::sep() : "") + resolvedPath);

    if (p.empty()) {
      return ".";
    }

    return p;
  }

  string dirname(string path) {
    cmatch result = Path::split(path);
    string root;
    string dir;

    if (result[1].length() > 0) {
      root = result[1];
    }

    if (result[2].length() > 0) {
      dir = result[2];
    }

    if (!root.length() && !dir.length()) {
      return ".";
    }

    if (dir.length() > 0) {
      dir = dir.substr(0, dir.length() - 1);
    }
    return root + dir;
  }

  PathObject createObject() {
    Path::PathObject object;
    return object;
  }

  PathObject parse(const string path) {
    PathObject o;

    auto allParts = Path::split(path);

    if (allParts.empty() || allParts.size() != 5) {
      throw runtime_error("Invalid path '" + path + "'");
    }

    o.root = allParts[1];
    o.dir = string(allParts[1]) + string(allParts[2]).substr(0, allParts[2].length() - 1);
    o.base = allParts[3];
    o.ext = allParts[4];
    o.name = string(allParts[3]).substr(0, allParts[3].length() - allParts[4].length());
    return o;
  }

  string format(PathObject o) {
    string dir = o.dir.length() > 0 ? o.dir + Path::sep() : "";
    return dir + o.base;
  }

  string basename(string path, string ext) {
    string f = Path::split(path)[3];

    if (ext.length() > 0 && f.substr(f.length() - ext.length(), f.length()) == ext) {
      f = f.substr(0, f.length() - ext.length());
    }
    return f;
  }

  string basename(string path) {
    string ext;
    return Path::basename(path, ext);
  }

  string extname(string path) {
    return Path::split(path)[4];
  }

  */
} // namespace Path
