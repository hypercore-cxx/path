# SYNOPSIS
This module contains utilities for handling and transforming file paths.
It is a direct port from the iojs project but does not support windows.
Almost all these methods perform only string transformations. The file 
system is not consulted to check whether paths are valid.

# AS A MODULE USING `BALE`.
```
import p "path"
p.extname("/path.to/.file.ext") == ".ext";
```

# AS A REGLUAR C++ LIBRARY.
Build with `path.cc` as a dependency and include the header file.

```
#include "path.h"
Path p;
p.extname("/path.to/.file.ext") == ".ext";
```

# API

## CONSTRUCTOR

### Path p;

## INSTANCE METHODS

### string path.join(string path1, string path2[, ...]);
Join all arguments together and normalize the resulting path. Arguments
must be strings.

```cpp
path.join("/foo", "bar", "baz/asdf", "quux", "..");
// returns "/foo/bar/baz/asdf"
```

### bool path.isAbsolute(string path);
Determines whether path is an absolute path. An absolute path will always
resolve to the same location, regardless of the working directory.

```cpp
path.isAbsolute("/foo/bar"); // true
path.isAbsolute("/baz/.."); // true
path.isAbsolute("qux/"); // false
path.isAbsolute("."); // false
```

### string path.dirname(string path);
Return the directory name of a path. Similar to the Unix dirname command.

```cpp
path.dirname("/foo/bar/baz/asdf/quux");
// returns "/foo/bar/baz/asdf"
```

### string path.basename(string path[, string ext]);
Return the last portion of a path. Similar to the Unix basename command.

```cpp
path.basename("/foo/bar/baz/asdf/quux.html");
// returns "quux.html"

path.basename("/foo/bar/baz/asdf/quux.html", ".html");
// returns "quux"
```

### string path.extname(string path);
Return the extension of the path, from the last "." to end of string in the
last portion of the path. If there is no "." in the last portion of the path
or the first character of it is ".", then it returns an empty string. 
Examples:

```cpp
path.extname("index.html");
// returns ".html"

path.extname("index.coffee.md");
// returns ".md"

path.extname("index.");
// returns "."

path.extname("index");
// returns ""
```

### string path.parse(string path);
Returns an object from a path string.

```cpp
auto o = path.parse("/home/user/dir/file.txt");
```

Returns a struct of type PathObject with the following members...
```cpp
o.root = "/";
o.dir = "/home/user/dir";
o.base = "file.txt";
o.ext = ".txt";
o.name = "file";
```

### PathObject path.format(struct pathObject);
Returns a path string from an object, the opposite of path.parse above.
The struct can be created with the `path.createObject()` method.

```cpp

auto o = path.createObject();

o.dir = "/home/user/dir";
o.base = "file.txt";
o.ext = ".txt";
o.name = "file";

path.format(o);
// returns "/home/user/dir/file.txt"
```

### string path.resolve(string path);
Resolves to to an absolute path. If to isn"t already absolute from 
arguments are prepended in right to left order, until an absolute path 
is found. The resulting path is normalized, and trailing slashes are 
removed unless the path gets resolved to the root directory. Non-string 
from arguments will throw an error.

Another way to think of it is as a sequence of cd commands in a shell.

```cpp
path.resolve("/foo/bar", "./baz");
// returns "/foo/bar/baz"

path.resolve("/foo/bar", "/tmp/file/");
// returns "/tmp/file"
```

### STATIC MEMBERS

### path.sep
The platform-specific file separator. "\\" or "/".

### path.delimiter
The platform-specific path delimiter, ";" or ":"

