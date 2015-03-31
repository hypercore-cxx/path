#include "../path.h"
#include <istream>

using namespace nodeuv;

#define ASSERT(message, ...) do { \
    if(!(__VA_ARGS__)) { \
          std::cerr << "FAIL: " << message << std::endl; \
        } \
    else { \
          std::cout << "OK: " << message << std::endl; \
        } \
} while(0);

int main() {

  //
  // sanity test.
  //
  ASSERT("sanity: true is false", true == false);
  ASSERT("sanity: true is true", true == true);

  Path path;
  string f = "/test/parallel/test-path.js";

  //
  // basename
  //
  ASSERT("basename", path.basename(f) == "test-path.js");
  ASSERT("basename", path.basename(f, ".js") == "test-path");
  ASSERT("basename", path.basename("") == "");
  ASSERT("basename", path.basename("/dir/basename.ext") == "basename.ext");
  ASSERT("basename", path.basename("/basename.ext") == "basename.ext");
  ASSERT("basename", path.basename("basename.ext") == "basename.ext");
  ASSERT("basename", path.basename("basename.ext/") == "basename.ext");
  ASSERT("basename", path.basename("basename.ext//") == "basename.ext");

  //
  // extname
  //
  ASSERT("extname", path.extname(f) == ".js")
  ASSERT("extname", path.extname("") == "");
  ASSERT("extname", path.extname("/path/to/file") == "");
  ASSERT("extname", path.extname("/path/to/file.ext") == ".ext");
  ASSERT("extname", path.extname("/path.to/file.ext") == ".ext");
  ASSERT("extname", path.extname("/path.to/file") == "");
  ASSERT("extname", path.extname("/path.to/.file") == "");
  ASSERT("extname", path.extname("/path.to/.file.ext") == ".ext");
  ASSERT("extname", path.extname("/path/to/f.ext") == ".ext");
  ASSERT("extname", path.extname("/path/to/..ext") == ".ext");
  ASSERT("extname", path.extname("file") == "");
  ASSERT("extname", path.extname("file.ext") == ".ext");
  ASSERT("extname", path.extname(".file") == "");
  ASSERT("extname", path.extname(".file.ext") == ".ext");
  ASSERT("extname", path.extname("/file") == "");
  ASSERT("extname", path.extname("/file.ext") == ".ext");
  ASSERT("extname", path.extname("/.file") == "");
  ASSERT("extname", path.extname("/.file.ext") == ".ext");
  ASSERT("extname", path.extname(".path/file.ext") == ".ext");
  ASSERT("extname", path.extname("file.ext.ext") == ".ext");
  ASSERT("extname", path.extname("file.") == ".");
  ASSERT("extname", path.extname(".") == "");
  ASSERT("extname", path.extname("./") == "");
  ASSERT("extname", path.extname(".file.ext") == ".ext");
  ASSERT("extname", path.extname(".file") == "");
  ASSERT("extname", path.extname(".file.") == ".");
  ASSERT("extname", path.extname(".file..") == ".");
  ASSERT("extname", path.extname("..") == "");
  ASSERT("extname", path.extname("../") == "");
  ASSERT("extname", path.extname("..file.ext") == ".ext");
  ASSERT("extname", path.extname("..file") == ".file");
  ASSERT("extname", path.extname("..file.") == ".");
  ASSERT("extname", path.extname("..file..") == ".");
  ASSERT("extname", path.extname("...") == ".");
  ASSERT("extname", path.extname("...ext") == ".ext");
  ASSERT("extname", path.extname("....") == ".");
  ASSERT("extname", path.extname("file.ext/") == ".ext");
  ASSERT("extname", path.extname("file.ext//") == ".ext");
  ASSERT("extname", path.extname("file/") == "");
  ASSERT("extname", path.extname("file//") == "");
  ASSERT("extname", path.extname("file./") == ".");
  ASSERT("extname", path.extname("file.//") == ".");

  //
  // dirname
  //
  ASSERT("dirname", path.dirname("/a/b/") == "/a");
  ASSERT("dirname", path.dirname("/a/b") == "/a");
  ASSERT("dirname", path.dirname("/a") == "/");
  ASSERT("dirname", path.dirname("") == ".");
  ASSERT("dirname", path.dirname("/") == "/");
  ASSERT("dirname", path.dirname("////") == "/");

  // 
  // normalize
  //
  ASSERT("normalize", path.normalize("./fixtures///b/../b/c.js") == "fixtures/b/c.js");
  ASSERT("normalize", path.normalize("/foo/../../../bar") == "/bar");
  ASSERT("normalize", path.normalize("a//b//../b") == "a/b");
  ASSERT("normalize", path.normalize("a//b//./c") == "a/b/c");
  ASSERT("normalize", path.normalize("a//b//.") == "a/b");

  //
  // resolve
  //
  ASSERT("resolve", path.resolve("/var/lib", "../", "file/") == "/var/file");
  ASSERT("resolve", path.resolve("/var/lib", "/../", "file/") == "/file");
  ASSERT("resolve", path.resolve("/some/dir", ".", "/absolute/") == "/absolute");

  //
  // isAbsolute
  //
  ASSERT("isAbsolute", path.isAbsolute("/home/foo") == true);
  ASSERT("isAbsolute", path.isAbsolute("/home/foo/..") == true);
  ASSERT("isAbsolute", path.isAbsolute("bar/") == false);
  ASSERT("isAbsolute", path.isAbsolute("./baz") == false);

  //
  // join
  //
  ASSERT("join", path.join(".", "x/b", "..", "/b/c.js") == "x/b/c.js");
  ASSERT("join", path.join( "/.", "x/b", "..", "/b/c.js") == "/x/b/c.js");
  ASSERT("join", path.join("/foo", "../../../bar") == "/bar");
  ASSERT("join", path.join("foo", "../../../bar") == "../../bar");
  ASSERT("join", path.join("foo/", "../../../bar") == "../../bar");
  ASSERT("join", path.join("foo/x", "../../../bar") == "../bar");
  ASSERT("join", path.join("foo/x", "./bar") == "foo/x/bar");
  ASSERT("join", path.join("foo/x/", "./bar") == "foo/x/bar");
  ASSERT("join", path.join("foo/x/", ".", "bar") == "foo/x/bar");
  ASSERT("join", path.join("./") == "./");
  ASSERT("join", path.join(".", "./") == "./");
  ASSERT("join", path.join(".", ".", ".") == ".");
  ASSERT("join", path.join(".", "./", ".") == ".");
  ASSERT("join", path.join(".", "/./", ".") == ".");
  ASSERT("join", path.join(".", "/////./", ".") == ".");
  ASSERT("join", path.join(".") == ".");
  ASSERT("join", path.join("", ".") == ".");
  ASSERT("join", path.join("", "foo") == "foo");
  ASSERT("join", path.join("foo", "/bar") == "foo/bar");
  ASSERT("join", path.join("", "/foo") == "/foo");
  ASSERT("join", path.join("", "", "/foo") == "/foo");
  ASSERT("join", path.join("", "", "foo") == "foo");
  ASSERT("join", path.join("foo", "") == "foo");
  ASSERT("join", path.join("foo/", "") == "foo/");
  ASSERT("join", path.join("foo", "", "/bar") == "foo/bar");
  ASSERT("join", path.join("./", "..", "/foo") == "../foo");
  ASSERT("join", path.join("./", "..", "..", "/foo") == "../../foo");
  ASSERT("join", path.join(".", "..", "..", "/foo") == "../../foo");
  ASSERT("join", path.join("", "..", "..", "/foo") == "../../foo");
  ASSERT("join", path.join("/") == "/");
  ASSERT("join", path.join("/", ".") == "/");
  ASSERT("join", path.join("/", "..") == "/");
  ASSERT("join", path.join("/", "..", "..") == "/");
  ASSERT("join", path.join("") == ".");
  ASSERT("join", path.join("", "") == ".");
  ASSERT("join", path.join(" /foo") == " /foo");
  ASSERT("join", path.join(" ", "foo") == " /foo");
  ASSERT("join", path.join(" ", ".") == " ");
  ASSERT("join", path.join(" ", "/") == " /");
  ASSERT("join", path.join(" ", "") == " ");
  ASSERT("join", path.join("/", "foo") == "/foo");
  ASSERT("join", path.join("/", "/foo") == "/foo");
  ASSERT("join", path.join("/", "//foo") == "/foo");
  ASSERT("join", path.join("/", "", "/foo") == "/foo");
  ASSERT("join", path.join("", "/", "foo") == "/foo");
  ASSERT("join", path.join("", "/", "/foo") == "/foo");

  //
  // PathObject
  //
  string pstr = "/home/user/dir/file.txt";

  auto o = path.createObject();
  o.dir = "/home/user/dir";
  o.base = "file.txt";
  o.ext = ".txt";
  o.name = "file";

  ASSERT("format", path.format(o) == pstr);

  auto p = path.parse(pstr);

  ASSERT("parse", p.dir == "/home/user/dir");
  ASSERT("parse", p.base == "file.txt");
  ASSERT("parse", p.ext == ".txt");
  ASSERT("parse", p.name == "file");
  ASSERT("parse", p.root == "/");
}

