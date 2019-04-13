#include "../index.hxx"
#include "../deps/heapwolf/cxx-tap/index.hxx"
#include <istream>
#include <regex>

int main() {
  using namespace TAP;
  
  Test t;

  const std::string f = "/test/parallel/test-path.js";

  t.test("isAbsolute", [](auto t) {
    t->equal(Path::isAbsolute("/home/foo"), true);
    t->equal(Path::isAbsolute("/home/foo/.."), true);
    t->equal(Path::isAbsolute("bar/"), false);
    t->equal(Path::isAbsolute("./baz"), false);
    t->end();
  });

  t.test("basename", [&](auto t) {
    t->equal(Path::basename(f), "test-path.js");
    t->equal(Path::basename(f, ".js"), "test-path");
    t->equal(Path::basename("/dir/basename.ext"), "basename.ext");
    t->equal(Path::basename("/basename.ext"), "basename.ext");
    t->equal(Path::basename("basename.ext"), "basename.ext");
    t->equal(Path::basename("basename.ext/"), "basename.ext");
    t->equal(Path::basename("basename.ext//"), "basename.ext");
    t->end();
  });

  t.test("extname", [&](auto t) {
    t->equal(Path::extname(f), ".js");
    t->ok(Path::extname("").empty());
    t->ok(Path::extname("/path/to/file").empty());
    t->equal(Path::extname("/path/to/file.ext"), ".ext");
    t->equal(Path::extname("/Path::to/file.ext"), ".ext");
    t->ok(Path::extname("/Path::to/file").empty());
    t->ok(Path::extname("/Path::to/.file").empty());
    t->equal(Path::extname("/Path::to/.file.ext"), ".ext");
    t->equal(Path::extname("/path/to/f.ext"), ".ext");
    t->equal(Path::extname("/path/to/..ext"), ".ext");
    t->ok(Path::extname("file").empty());
    t->equal(Path::extname("file.ext"), ".ext");
    t->ok(Path::extname(".file").empty());
    t->equal(Path::extname(".file.ext"), ".ext");
    t->ok(Path::extname("/file").empty());
    t->equal(Path::extname("/file.ext"), ".ext");
    t->ok(Path::extname("/.file").empty());
    t->equal(Path::extname("/.file.ext"), ".ext");
    t->equal(Path::extname(".path/file.ext"), ".ext");
    t->equal(Path::extname("file.ext.ext"), ".ext");
    t->equal(Path::extname("file."), ".");
    t->ok(Path::extname(".").empty());
    t->ok(Path::extname("./").empty());
    t->equal(Path::extname(".file.ext"), ".ext");
    t->ok(Path::extname(".file").empty());
    t->equal(Path::extname(".file."), ".");
    t->equal(Path::extname(".file.."), ".");
    t->ok(Path::extname("..").empty());
    t->ok(Path::extname("../").empty());
    t->equal(Path::extname("..file.ext"), ".ext");
    t->equal(Path::extname("..file"), ".file");
    t->equal(Path::extname("..file."), ".");
    t->equal(Path::extname("..file.."), ".");
    t->equal(Path::extname("..."), ".");
    t->equal(Path::extname("...ext"), ".ext");
    t->equal(Path::extname("...."), ".");
    t->equal(Path::extname("file.ext/"), ".ext");
    t->equal(Path::extname("file.ext//"), ".ext");
    t->ok(Path::extname("file/").empty());
    t->ok(Path::extname("file//").empty());
    t->equal(Path::extname("file./"), ".");
    t->equal(Path::extname("file.//"), ".");
    t->end();
  });

  return 0;

  /*
  //
  // extname
  //


  //
  // dirname
  //
  ASSERT("dirname", Path::dirname("/a/b/") == "/a");
  ASSERT("dirname", Path::dirname("/a/b") == "/a");
  ASSERT("dirname", Path::dirname("/a") == "/");
  ASSERT("dirname", Path::dirname("") == ".");
  ASSERT("dirname", Path::dirname("/") == "/");
  ASSERT("dirname", Path::dirname("////") == "/");

  // 
  // normalize
  //
  ASSERT("normalize", Path::normalize("./fixtures///b/../b/c.js") == "fixtures/b/c.js");
  ASSERT("normalize", Path::normalize("/foo/../../../bar") == "/bar");
  ASSERT("normalize", Path::normalize("a//b//../b") == "a/b");
  ASSERT("normalize", Path::normalize("a//b//./c") == "a/b/c");
  ASSERT("normalize", Path::normalize("a//b//.") == "a/b");

  //
  // resolve
  //
  ASSERT("resolve", Path::resolve("/var/lib", "../", "file/") == "/var/file");
  ASSERT("resolve", Path::resolve("/var/lib", "/../", "file/") == "/file");
  ASSERT("resolve", Path::resolve("/some/dir", ".", "/absolute/") == "/absolute");

  //
  // join
  //
  ASSERT("join", Path::join(".", "x/b", "..", "/b/c.js") == "x/b/c.js");
  ASSERT("join", Path::join( "/.", "x/b", "..", "/b/c.js") == "/x/b/c.js");
  ASSERT("join", Path::join("/foo", "../../../bar") == "/bar");
  ASSERT("join", Path::join("foo", "../../../bar") == "../../bar");
  ASSERT("join", Path::join("foo/", "../../../bar") == "../../bar");
  ASSERT("join", Path::join("foo/x", "../../../bar") == "../bar");
  ASSERT("join", Path::join("foo/x", "./bar") == "foo/x/bar");
  ASSERT("join", Path::join("foo/x/", "./bar") == "foo/x/bar");
  ASSERT("join", Path::join("foo/x/", ".", "bar") == "foo/x/bar");
  ASSERT("join", Path::join("./") == "./");
  ASSERT("join", Path::join(".", "./") == "./");
  ASSERT("join", Path::join(".", ".", ".") == ".");
  ASSERT("join", Path::join(".", "./", ".") == ".");
  ASSERT("join", Path::join(".", "/./", ".") == ".");
  ASSERT("join", Path::join(".", "/////./", ".") == ".");
  ASSERT("join", Path::join(".") == ".");
  ASSERT("join", Path::join("", ".") == ".");
  ASSERT("join", Path::join("", "foo") == "foo");
  ASSERT("join", Path::join("foo", "/bar") == "foo/bar");
  ASSERT("join", Path::join("", "/foo") == "/foo");
  ASSERT("join", Path::join("", "", "/foo") == "/foo");
  ASSERT("join", Path::join("", "", "foo") == "foo");
  ASSERT("join", Path::join("foo", "") == "foo");
  ASSERT("join", Path::join("foo/", "") == "foo/");
  ASSERT("join", Path::join("foo", "", "/bar") == "foo/bar");
  ASSERT("join", Path::join("./", "..", "/foo") == "../foo");
  ASSERT("join", Path::join("./", "..", "..", "/foo") == "../../foo");
  ASSERT("join", Path::join(".", "..", "..", "/foo") == "../../foo");
  ASSERT("join", Path::join("", "..", "..", "/foo") == "../../foo");
  ASSERT("join", Path::join("/") == "/");
  ASSERT("join", Path::join("/", ".") == "/");
  ASSERT("join", Path::join("/", "..") == "/");
  ASSERT("join", Path::join("/", "..", "..") == "/");
  ASSERT("join", Path::join("") == ".");
  ASSERT("join", Path::join("", "") == ".");
  ASSERT("join", Path::join(" /foo") == " /foo");
  ASSERT("join", Path::join(" ", "foo") == " /foo");
  ASSERT("join", Path::join(" ", ".") == " ");
  ASSERT("join", Path::join(" ", "/") == " /");
  ASSERT("join", Path::join(" ", "") == " ");
  ASSERT("join", Path::join("/", "foo") == "/foo");
  ASSERT("join", Path::join("/", "/foo") == "/foo");
  ASSERT("join", Path::join("/", "//foo") == "/foo");
  ASSERT("join", Path::join("/", "", "/foo") == "/foo");
  ASSERT("join", Path::join("", "/", "foo") == "/foo");
  ASSERT("join", Path::join("", "/", "/foo") == "/foo");

  //
  // PathObject
  //
  std::string pstr = "/home/user/dir/file.txt";

  auto o = Path::createObject();
  o.dir = "/home/user/dir";
  o.base = "file.txt";
  o.ext = ".txt";
  o.name = "file";

  ASSERT("format", Path::format(o) == pstr);

  auto p = Path::parse(pstr);

  ASSERT("parse", p.dir == "/home/user/dir");
  ASSERT("parse", p.base == "file.txt");
  ASSERT("parse", p.ext == ".txt");
  ASSERT("parse", p.name == "file");
  ASSERT("parse", p.root == "/"); */
}
