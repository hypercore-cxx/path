#include "index.hxx"
#include "deps/datcxx/util-string/index.hxx"

#include <vector>
#include <regex>

namespace Hyper {
  namespace Util {
    namespace Path {
      using String = std::string;

      String delimiter () {
        return ":";
      }

      bool isAbsolute(const String& path) {
        return path[0] == Path::sep();
      }

      String basename(const String& path, const String& ext) {
        std::vector<String> parts = 
          Hyper::Util::String::split(path, String(1, Path::sep()));

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
        String ext;

        auto tmp = Util::String::trim(path, "/");

        if (tmp == "..") {
          return "";
        }
        
        if (tmp == ".") {
          return "";
        }
        
        if (tmp == "/") {
          return "";
        }
        
        if (tmp == "//") {
          return "";
        }

        size_t dotIdx = tmp.find_last_of(".");

        if (dotIdx != String::npos) {
          size_t dirSepIdx = tmp.find_last_of("/\\");

          if (dotIdx > dirSepIdx + 1) {
            ext = tmp.substr(dotIdx);
          }
        }


        return ext;
      }

      std::vector<String> splitPathList(const String& path) {
        std::vector<String> result;
        String::size_type start = 0;
        String::size_type end = 0;
        String delimiter = String(1, Path::sep());

        while (end != String::npos) {
          end = path.find(delimiter, start);

          if (start < path.size() && start != end) {
            String segment = path.substr(start, end - start);
            result.push_back(segment);
          }
          start = end + delimiter.size();
        }

        return result;
      }

      String joinPathList(const std::vector<String>& list) {
        String res;
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

      std::vector<String> normalizePathList(std::vector<String> parts, bool allowAboveRoot) {
        std::vector<String> res;
        int len = parts.size();

        for (int i = 0; i < len; i++) {
          String p = parts[i];

          if ((p.length() == 0) || p == ".") {
            continue;
          }

          if (p == "..") {
            if (!res.empty() && res[res.size() - 1] != "..") {
              res.pop_back();
            } else if (allowAboveRoot) {
              res.emplace_back("..");
            }
          } else {
            res.push_back(p);
          }
        }
        return res; 
      }

      String normalize(const String& path) {
        bool isAbs = Path::isAbsolute(path);
        bool trailingSlash = path[path.length() -1] == Path::sep();

        auto split = Path::splitPathList(path);
        auto normalized = Path::normalizePathList(split, !isAbs);
        auto tmp = Path::joinPathList(normalized);

        if (tmp.length() == 0 && !isAbs) {
          tmp = ".";
        }

        if (path.length() > 0 && trailingSlash) {
          tmp += Path::sep();
        }

        return (isAbs ? String(1, Path::sep()) : "") + tmp;
      }

      String joinVector(const std::vector<String>& args) {
        if (args.size() == 1 && args[0] == "/") {
          return "/";
        }

        String path;

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

      String resolveFromVector(const std::vector<String>& args) {
        String resolvedPath = "";
        bool resolvedAbsolute = false;

        for (int i = args.size() - 1; i >= -1 && !resolvedAbsolute; i--) {
          String path = args[i];

          if (path.empty()) {
            continue;
          }

          resolvedPath = path + Path::sep() + resolvedPath;
          resolvedAbsolute = path[0] == Path::sep();
        }

        // At this point the path should be resolved to a full absolute path, but
        // handle relative paths to be safe (might happen when process.cwd() fails)

        // Normalize the path
        auto split = Path::splitPathList(resolvedPath);
        auto normalized = Path::normalizePathList(split, !resolvedAbsolute);

        resolvedPath = Path::joinPathList(normalized);

        auto sep = String(1, Path::sep());
        String p = ((resolvedAbsolute ? sep : "") + resolvedPath);

        if (p.empty()) {
          return ".";
        }

        return p;
      }

      bool isPathSeparator (const char s) {
        return s == '/' || s == '\\';
      }

      String dirname(const String& path) {
        size_t len = path.length();

        if (len == 0) {
          return ".";
        }

        size_t rootEnd = -1;
        size_t offset = 0;
        const char ch = path[0];

        if (len == 1) {
          // `path` contains just a path separator, exit early to avoid
          // unnecessary work or a dot.
          return isPathSeparator(ch) ? path : ".";
        }

        // Try to match a root
        if (isPathSeparator(ch)) {
          // Possible UNC root

          rootEnd = offset = 1;

          if (isPathSeparator(path[1])) {
            // Matched double path separator at beginning
            size_t j = 2;
            size_t last = j;

            // Match 1 or more non-path separators
            while (j < len && !isPathSeparator(path[j])) {
              j++;
            }

            if (j < len && j != last) {
              // Matched!
              last = j;
              // Match 1 or more path separators
              while (j < len && isPathSeparator(path[j])) {
                j++;
              }

              if (j < len && j != last) {
                // Matched!
                last = j;
                // Match 1 or more non-path separators
                while (j < len && !isPathSeparator(path[j])) {
                  j++;
                }
                if (j == len) {
                  // We matched a UNC root only
                  return path;
                }
                if (j != last) {
                  // We matched a UNC root with leftovers

                  // Offset by 1 to include the separator after the UNC root to
                  // treat it as a "normal root" on top of a (UNC) root
                  rootEnd = offset = j + 1;
                }
              }
            }
          }
        // Possible device root
        } else if (Util::String::test(String(1, ch), "[a-zA-Z]") && path[1] == ':') {
          rootEnd = len > 2 && isPathSeparator(path[2]) ? 3 : 2;
          offset = rootEnd;
        }

        size_t end = -1;
        size_t matchedSlash = true;

        for (size_t i = len - 1; i >= offset; --i) {
          if (isPathSeparator(path[i])) {
            if (!matchedSlash) {
              end = i;
              break;
            }
          } else {
            // We saw the first non-path separator
            matchedSlash = false;
          }
        }

        if (end == -1) {
          if (rootEnd == -1) {
            return ".";
          }

          end = rootEnd;
        }

        return path.substr(0, end);
      }

      PathObject createObject () {
        Path::PathObject object;
        return object;
      }

      PathObject parse(const String& path) {
        PathObject o;

        if (path.empty()) {
          throw std::runtime_error("Invalid path '" + path + "'");
        }

        o.root = path[0] == sep() ? String(1, sep()) : "";
        o.dir = Path::dirname(path);
        o.base = Path::basename(path);
        o.ext = Path::extname(path);
        o.name = o.base.substr(0, o.base.length() - o.ext.length());
        return o;
      }

      String format(PathObject& o) {
        String dir = o.dir.length() > 0 ? o.dir + Path::sep() : "";
        return dir + o.base;
      }
    } // namespace Path
  } // namespace Util
} // namespace Hyper
