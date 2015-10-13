# RIO Engine


## Building

fips (CMake wrapper) is used as a building system.
Visual Studio solution (Mac OS X, iOS XCode projects, Linux, Android projects -- planned).
At the moment, Windows build is supported.


## Platforms

Graphics APIs:

* *OpenGL* (through bgfx)
* *OpenGL ES* (through bgfx)
* *DirectX* (through bgfx)

Platforms:

* *Windows* (MSVC 2013)
* *OS X* (To be added)
* *iOS* (To be added)
* *Linux* (To be added)
* *Android* (To be added)

## Dependencies

### Libraries

* bgfx (Rendering)
* ImGui (Widgets, GUI)
* NanoVg (2D graphics, debug drawing)

## Conventions

* Allman (BSD) Indenting
* camelCase
* Tabs for indentation

### Classes

PascalCase (Upper CamelCase) (e.g.: ResourceManager)

### Functions, Methods, and Variables

camelCase (e.g.: compileShader)

### Namespaces

PascalCase (e.g.: Rio:: )

The end of a namespace must appended by `namespace` then its name.

Example:

```c++
namespace Rio
{
namespace Inner
{
...
} // namespace Inner
} // namespace Rio
```

### Using and Typedef

In C++11, `using` is a superset of `typedef`. `using` is preferred in this style guide.

```c++
// Using
using FooBar = std::pair<Foo, Bar>;
// Typedef
typedef std::pair<Foo, Bar> FooBar;
```

### Comments

C++ style comments `//` should be used. Don't use C-style comments `/**/`. Reason: C-style comments cannot be nested and most IDEs/Editors can comments whole blocks in the C++ style which can allow for uncommenting a single line easily.

### Variables

camelCase (e.g.: bufferBegin, isRunning)

### Variable Prefixes

No variable prefix should be used.
Variable name is not about the variable type.
Hungarian notation was just a workaround and it is a mistake to use it now.

### Enums

Enum items : PascalCase (e.g.: `BehaviorType`).

Strong enum is preferred (`enum class`).

Example:

```c++
enum class BehaviorType
{
	Idle,
	Moving,
	Fighting,
};
```
### Annotations

* TODO      - todo, work in progress
* NOTE      - note to reader
* IMPORTANT - important
* FIXME     - fix as soon as possible (does not work as intended)