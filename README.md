# mafiascript

A semi-serious dynamically-typed interpreted programming language. Made mostly for learning experience.

[Check out the wiki for information about the language](https://github.com/GoogleFeud/mafiascript/wiki)

## Built with

- C++17
- g++ and Make
- The `Flex` scanner is used, alongside the `Bison` parser generator.

## Features

- Functional. Check out the [examples page](https://github.com/GoogleFeud/mafiascript/wiki/Examples) for working examples.
- All common programming language features are implemented:
    - Variables and scopes
    - `string`, `number`, `bool`, `null`, `array ([])`, `object ({})` and `function (() => {})` literals.
    - If statements and ternery
    - `for` and `while` loops, combined in a single keyword (`loop`)
    - All types have properties.

## What's missing

- Detailed and useful error messages
- An actual call stack

## Example

**Get a number in the fibonacci sequence in mafiascript:**

```
const fibonacci = (n) => {
   if (!n) return 0;
   const arr = [0, 1];
   loop (let i=2, i <= n, i += 1) {
      arr[i] = arr[i - 1] + arr[i - 2];
   };
   return arr[n];
};

fibonacci(10); // 55
```

## How to build and run

**Requirements:**
- gcc or any other `C++` compiler that supports C++17.
- `Flex` and `Bison` installed and included in path.

**Setup:**
- Clone this repository
- In the root directory of this repo, run `make`. You may have to edit the `make` file accordingly if you are using a different compiler.
- Run `./dist`
- You can edit `script.ms` to run something different.


