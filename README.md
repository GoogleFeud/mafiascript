# mafiascript

A programming language for your users. Mafiascript is designed to be an easy language for configuring a service, or how your application behaves.

- Simple, yet customizable and powerful.
- Made for executing user-provided code. 
- Fast & with a small footprint.

## Language design

`Mafiascript` is a dynamically typed, interpreted programming lanuage, made to be easily embeddable in C/C++ and node.js applications, with the purpose of running user (aka unsafe) code. 

### Declaring variables

Declaring variables and functions is done via the `let` or `const` keywords. Just like in javascript, if a variable is declared with `let` it's mutable, if with `const` it's immutable. 

```
const value = 10;
value++; // Error!
let value2 = 10;
value2++; // Fine
```

### Primitive values

```
let string = "Some string";
let number = 69; // All numbers are 32 bit signed floats
let bool = false;
let noValue = null; // Default value for all variables with no initializors
let function = () => {
    // Code
};
```

### Objects

In mafiascript, every "object", or "class instance" is an `object`. An object contains key-value pairs. Values can be primitive data, other objects or functions. No prototypes, inheritence, OOP, any of that. Treat an object as an `unordered_map`.

```
const obj = {
    a: 1,
    b: 2
};
obj.a++;
obj["b"]--;
```

Don't be fooled: Just because the object is defined via `const` doesn't mean it's properties are also immutable. To make all properties of an object immutable, use the `final` keyword before the object declaration:

```
const obj = final{
     a: 1,
    b: 2
};
obj.a++; // Error!
```


### Arrays

```
const arr = [];
arr.push(1);
arr.push(2);
arr[0]; // 1
```

### Functions

Functions are always anonymous, and must be assigned to a variable to be called. 

```
const func = (a, b) => {
    return a + b;
}

func(1, 1); // 2
func(1, 11); // 12
```

Since Mafiascript has no garbage collector, all function scopes are `isolated`. They can access only variables from the global scope, since they last until the end of the program. To pass variables from other scopes to the function, either use parameters or `captures`. 

```
let fnThatHasAccessToTempVariable;
{
    const tempVariable = [1, 2, 3, 4];
    fnThatHasAccessToTempVariable = [tempVariable](num) => {
        return std.reduce(tempVariable, (a, b) => a + b, 0) + (num || 0);
    };
};

fnThatHasAccessToTempVariable(); // 10
fnThatHasAccessToTempVariable(5); // 15
```

Captured variables share the function's lifetime, when the last reference of the function is lost, the tempVariable is also lost.  

### Logic

```
let a = 10;
let b = 5;
if (a > b) { 
 // Code
} else if (condition) {
    // Code
} else  {
// Code
}
```

```
let a = 55;
if (a == 55) return true
else if (a == 50) return false
else return null;

```

### Loops

Mafiascript has only one keyword for loops, the `loop` keyword. All loop keyword have a timeout you can specify in the settings. 

#### Condition

```
loop (condition) {
  // Code
}
```

#### Array

```
const arr = [1, 2, 3, 4, 5, 6];
loop(arr) {
    //Code
}
```

#### Standard for loop

```
loop (before; condition; after) {
    // Code
}

loop (condition; after) {
    // Code
}
```

### Memory management & GC

`Mafiascript` does NOT have a real garbage collector. Complex types such as `string`, `array`, `object` and `function` are wrapped in a shared pointer, they get deleted when their reference count reaches zero. Simple and light types such as `number`, `bool` and `null` always get copied.


### Importing global objects

```cpp 
MSContextGenerator contextGen = MSContextGenerator(/* settings */);
MSContext context = contextGen.generate();
context.addString("name", "GoogleFeud", true); // key, value, mutatable
context.addBool("isAwesome", true); // key, value, mutatable
context.addFunction("add", // The function's name 
[](MSFunctionContext ctx) -> {ctx.args[0] + ctx.args[1]}, // The function to import
[MS.NUMBER, MS.NUMBER], // Argument type safety
false //mutatable
);

context.addObject("player"); // object name, mutatable
context.addProperty("player", "name", "Google", false) // object name, key, value, mutatable

context.run("code");
```

In MS:

```
name; // GoogleFeud
isAwesome // true
add(1, 2); // 3

player.name; // Google
player.name = "SomeoneElse" // Error!
```

### Getting global variables from MScript

```
MSContext context = contextGen.generate();
context.run("const add = (a, b) => a + b;");

MS_Value val = context.global.at("add");
val.index(); // The type of the variable
std::string realValue = downcast<std::string>(val); // Get the value of the object if it's string
float realValue = downcast<float>(val); // Get the value if it's a number
bool realValue = downcast<bool>(val); // Get the value if it's bool
vector<MS_Value>* realValue = downcast<vector<MS_Value>*>(val); // Get the value if it's an array
unordered_map<std::string, MS_Value>* realValue = downcast<unordered_map<std::string, MS_Value>*>(val) // Get if the value is an object
val.value.call(std::vector { 1, 1 }); // MSValue(2);
```



