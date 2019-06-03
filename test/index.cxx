#include <istream>
#include <regex>

#include "../deps/heapwolf/cxx-tap/index.hxx"

#include "../index.hxx"

int main() {
  using namespace TAP;
  namespace Path = Hyper::Util::Path;
  
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

  t.test("normalize", [&] (auto t) {
    t->equal(Path::normalize("./fixtures///b/../b/c.js"), "fixtures/b/c.js");
    t->equal(Path::normalize("/foo/../../../bar"), "/bar");
    t->equal(Path::normalize("a//b//../b"), "a/b");
    t->equal(Path::normalize("a//b//./c"), "a/b/c");
    t->equal(Path::normalize("a//b//."), "a/b");
    t->end();
  });

  t.test("dirname", [&] (auto t) {
    t->equal(Path::dirname("/a/b/"), "/a");
    t->equal(Path::dirname("/a/b"), "/a");
    t->equal(Path::dirname("/a"), "/");
    t->equal(Path::dirname(""), ".");
    t->equal(Path::dirname("/"), "/");
    t->equal(Path::dirname("////"), "/");
    t->end();
  });

  t.test("join", [&] (auto t) {
    t->equal(Path::join(".", "x/b", "..", "/b/c.js"), "x/b/c.js");
    t->equal(Path::join( "/.", "x/b", "..", "/b/c.js"), "/x/b/c.js");
    t->equal(Path::join("/foo", "../../../bar"), "/bar");
    t->equal(Path::join("foo", "../../../bar"), "../../bar");
    t->equal(Path::join("foo/", "../../../bar"), "../../bar");
    t->equal(Path::join("foo/x", "../../../bar"), "../bar");
    t->equal(Path::join("foo/x", "./bar"), "foo/x/bar");
    t->equal(Path::join("foo/x/", "./bar"), "foo/x/bar");
    t->equal(Path::join("foo/x/", ".", "bar"), "foo/x/bar");
    t->equal(Path::join("./"), "./");
    t->equal(Path::join(".", "./"), "./");
    t->equal(Path::join(".", ".", "."), ".");
    t->equal(Path::join(".", "./", "."), ".");
    t->equal(Path::join(".", "/./", "."), ".");
    t->equal(Path::join(".", "/////./", "."), ".");
    t->equal(Path::join("."), ".");
    t->equal(Path::join("", "."), ".");
    t->equal(Path::join("", "foo"), "foo");
    t->equal(Path::join("foo", "/bar"), "foo/bar");
    t->equal(Path::join("", "/foo"), "/foo");
    t->equal(Path::join("", "", "/foo"), "/foo");
    t->equal(Path::join("", "", "foo"), "foo");
    t->equal(Path::join("foo", ""), "foo");
    t->equal(Path::join("foo/", ""), "foo/");
    t->equal(Path::join("foo", "", "/bar"), "foo/bar");
    t->equal(Path::join("./", "..", "/foo"), "../foo");
    t->equal(Path::join("./", "..", "..", "/foo"), "../../foo");
    t->equal(Path::join(".", "..", "..", "/foo"), "../../foo");
    t->equal(Path::join("", "..", "..", "/foo"), "../../foo");
    t->equal(Path::join("/"), "/");
    t->equal(Path::join("/", "."), "/");
    t->equal(Path::join("/", ".."), "/");
    t->equal(Path::join("/", "..", ".."), "/");
    t->equal(Path::join(""), ".");
    t->equal(Path::join("", ""), ".");
    t->equal(Path::join(" /foo"), " /foo");
    t->equal(Path::join(" ", "foo"), " /foo");
    t->equal(Path::join(" ", "."), " ");
    t->equal(Path::join(" ", "/"), " /");
    t->equal(Path::join(" ", ""), " ");
    t->equal(Path::join("/", "foo"), "/foo");
    t->equal(Path::join("/", "/foo"), "/foo");
    t->equal(Path::join("/", "//foo"), "/foo");
    t->equal(Path::join("/", "", "/foo"), "/foo");
    t->equal(Path::join("", "/", "foo"), "/foo");
    t->equal(Path::join("", "/", "/foo"), "/foo");
    t->end();
  });

  t.test("resolve", [&] (auto t) {
    t->equal(Path::resolve("/var/lib", "../", "file/"), "/var/file");
    t->equal(Path::resolve("/var/lib", "/../", "file/"), "/file");
    t->equal(Path::resolve("/some/dir", ".", "/absolute/"), "/absolute");
    t->end();
  });

  t.test("relative", [&] (auto t) {
    t->equal(Path::relative("/var/lib", "/var"), "..");
    t->equal(Path::relative("/var/lib", "/bin"), "../../bin");
    t->equal(Path::relative("/var/lib", "/var/lib"), "");
    t->equal(Path::relative("/var/lib", "/var/apache"), "../apache");
    t->equal(Path::relative("/var/", "/var/lib"), "lib");
    t->equal(Path::relative("/", "/var/lib"), "var/lib");
    t->equal(Path::relative("/foo/test", "/foo/test/bar/package.json"), "bar/package.json");
    t->equal(Path::relative("/Users/a/web/b/test/mails", "/Users/a/web/b"), "../..");
    t->equal(Path::relative("/foo/bar/baz-quux", "/foo/bar/baz"), "../baz");
    t->equal(Path::relative("/foo/bar/baz", "/foo/bar/baz-quux"), "../baz-quux");
    t->equal(Path::relative("/baz-quux", "/baz"), "../baz");
    t->equal(Path::relative("/baz", "/baz-quux"), "../baz-quux");
    t->end();
  });

  t.test("parse and format", [&] (auto t) {

    std::string stra = "/home/user/dir/file.txt";

    auto o = Path::createObject();
    o.dir = "/home/user/dir";
    o.base = "file.txt";
    o.ext = ".txt";
    o.name = "file";

    t->equal(Path::format(o), stra, "reformatted matches string");

    std::string strb = "/home/user/dir/file.txt";
    auto p = Path::parse(strb);

    t->equal(p.dir, "/home/user/dir", "found dir");
    t->equal(p.base, "file.txt", "found base");
    t->equal(p.ext, ".txt", "found ext");
    t->equal(p.name, "file", "found file");
    t->equal(p.root, "/", "found root");

    t->end();
  });
}
