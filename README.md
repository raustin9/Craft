# Craft Language

The Craft language is a systems programming language with the goal of being fun and easy to use. It is inspired by C, C++, and Rust.\

Hello World program:\
```cxx
define main(argc: i32, argv: std::Vector<std::String>): i32 {
    std::print("Hello, World!\n");

    return std::SUCCESS;
}
```

## Syntax
All the keywords for the language can be found in [docs/keyword.md](/docs/keywords.txt). \
There are relatively few, and the main goal of that is to keep the syntax of the language relatively simple.\
If you are familar with the [Rust](https://www.rust-lang.org) language, you will probably be familar with many constructs.

### Functions
Functions are similar to other languages in that they take parameters as input, and return a result as output. \
A function is declared and defined like below:
```cxx
define func(param1_name: Param1Type, param2_name: Param2Type): FunctionReturnType {
    ...
    return result;
}
```
As seen above, we use the `define` keyword to declare the function, and we define the parameters and their types in the parentheses. \

## Data Types
Like all languages, Craft includes primitive builtin data types.\
These include:
- Signed Integers (number is size in bits):
    - `i8`
    - `i16`
    - `i32`
    - `i64`
- Unsigned Integers (number is size in bits):
    - `u8`
    - `u16`
    - `u32`
    - `u64`
- IEEE Floating Point Numbers (number is size in bits):
    - `f32`
    - `f64`
- Boolean values
    - `true`
    - `false`
- String literals (not the `std::string` data type)
    - ```"string literal"```
- Pointers
    - `*i32`: Pointer to a 32 bit integer
    - `*std::string`: Pointer to a `std::string` instance
    - Dereferencing:
```rust
let mut i: i32 = 100;
let ptr: *i32 = &i; // assign the pointer to the address of i

i = 20;

std::print("i:   {i}\n");  // prints "20"
std::print("ptr: {*ptr}"); // prints "20" as well. NOTE we are dereferencing ptr to access the value it is pointing to
```

## Structures
Like most languagaes, Craft allows the programmer to create custom data structurses known as `struct`s. \
They are very similar to structures in C and Rust.\
Unlike other languages, but like C, **ALL** fields and methods in a `struct` are public.
```rust
struct Person {
    name: std::string,
    age: u32
}
```
You can also attach **methods** to structures to give them behavior.
```cxx
struct Person {
    name: std::string;
    age: u32;

    // public method that each instance can call
    method print() {
        std::print("Name: {self->name}. Age {self->age}");
    }

    // private method that only functions attached to this type have access to
    method get_older() {
        self->age += 1;
    }

    // Static method attached to the data type rather than any instance
    static method new(name: std::string, age: u32): Person {
        return Person {
            name: name,
            age: age,
        };
    }
}

// Create and use a Person instance
define func(): i32 {
    let p: Person = Person::new(std::string("John Doe"), 32);
    p.print();

    return std::SUCCESS;
}
```
As we can see above, we have both `method` and `static` member functions of the struct. \
`static` functions are attached to the **type** rather than an instance. We can see that when we call `Person::new(...)`. We are calling the **type**'s method rather than an instance's. \
On the other hand, member functions defined using the `method` keyword are attached to an instance, and as such can have access to their fields using the `self` keyword. \
`self` is used similar to the `this` keyword in C++. We can also see that `self` is a pointer to the instance calling the object, and as such we **have to derefernce it**. 

## Enums and Sum types
One of the most powerful features offered by the Rust language is its usage of Sum types through their enums. \
Craft offers similar power in the same way. Here is a standard enum in Craft:
```rust
enum Parent {
    Mother,
    Father,
}
```
This is great, but we can do cooler things than just that:
```
enum Option<T> {
    Some(T),
    None
}
```
This is like the [Option](https://doc.rust-lang.org/std/option/) type in Rust. We can see that the `Some` variant of the enum holds a value `T`. \
Here is how we would use such an `enum`:
```rust
define func(input: i32): Option<i32> {
    if input > 0 {
        return Option<i32>::Some(100);
    } else {
        return Optino<i32>::None;
    }
}

let i: Option<i32> = func(-1);
match i {
    Some(value) => {
        std::print("Has value of {value}\n");
    }
    None => {
        std::print("Has no value :(\n");
    }
}
```
We have to use the `match` statement to confirm which variant of the `Option` enum it is. We need to declare a branch in the match statement for each variant. In this case there are only 2.\

