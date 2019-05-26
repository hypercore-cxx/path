# SYNOPSIS
Utilities for handling and transforming filesystem paths.


# USAGE
This module is designed to work with the [`datcxx`][0] build tool. To add this
module to your project us the following command...

```bash
build add datcxx/path
```


# TEST

```bash
build test
```


# API

#### `string` Path::join(string path1, string path2[, ...]);
Join all arguments together and normalize the resulting path. Arguments
must be strings.

```cpp
Path::join("/foo", "bar", "baz/asdf", "quux", "..");
// returns "/foo/bar/baz/asdf"
```

#### `bool` Path::isAbsolute(string path);
Determines whether path is an absolute path. An absolute path will always
resolve to the same location, regardless of the working directory.

```cpp
Path::isAbsolute("/foo/bar"); // true
Path::isAbsolute("/baz/.."); // true
Path::isAbsolute("qux/"); // false
Path::isAbsolute("."); // false
```

#### `string` Path::dirname(string path);
Return the directory name of a path. Similar to the Unix dirname command.

```cpp
Path::dirname("/foo/bar/baz/asdf/quux");
// returns "/foo/bar/baz/asdf"
```

#### `string` Path::basename(string path[, string ext]);
Return the last portion of a path. Similar to the Unix basename command.

```cpp
Path::basename("/foo/bar/baz/asdf/quux.html");
// returns "quux.html"

Path::basename("/foo/bar/baz/asdf/quux.html", ".html");
// returns "quux"
```

#### `string` path.extname(string path);
Return the extension of the path, from the last "." to end of string in the
last portion of the path. If there is no "." in the last portion of the path
or the first character of it is ".", then it returns an empty string. 
Examples:

```cpp
Path::extname("index.html");
// returns ".html"

Path::extname("index.coffee.md");
// returns ".md"

Path::extname("index.");
// returns "."

Path::extname("index");
// returns ""
```

#### `string` Path::parse(string path);
Returns an object from a path string.

```cpp
auto o = Path::parse("/home/user/dir/file.txt");
```

Returns a struct of type PathObject with the following members...
```cpp
o.root = "/";
o.dir = "/home/user/dir";
o.base = "file.txt";
o.ext = ".txt";
o.name = "file";
```

#### `PathObject` Path::format(struct pathObject);
Returns a path string from an object, the opposite of path.parse above.
The struct can be created with the `path.createObject()` method.

```cpp

auto o = Path::createObject();

o.dir = "/home/user/dir";
o.base = "file.txt";
o.ext = ".txt";
o.name = "file";

Path::format(o);
// returns "/home/user/dir/file.txt"
```

#### `string` Path::resolve(string path);
Resolves to to an absolute path. If to isn"t already absolute from 
arguments are prepended in right to left order, until an absolute path 
is found. The resulting path is normalized, and trailing slashes are 
removed unless the path gets resolved to the root directory. Non-string 
from arguments will throw an error.

Another way to think of it is as a sequence of cd commands in a shell.

```cpp
Path::resolve("/foo/bar", "./baz");
// returns "/foo/bar/baz"

Path::resolve("/foo/bar", "/tmp/file/");
// returns "/tmp/file"
```

[0]:https://github.com/datcxx/build
