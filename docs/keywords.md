# Keywords
## Declarations
- `define`: Used for defining a function.
- `let`: Used for declaring a variable.
- `struct`: Used for defining a struct.
- `enum`: Used for declaring a Sum type.

## Data Type Annotations
- `i8`, `i16`, `i32`, `i64`: Type annotation for **signed** integers. The number corresponds to how many bits are in the integer.
- `u8`, `u16`, `u32`, `u64`: Type annotation for **unsigned** integers. The number corresponds to how many bits are in the unsigned integer.
- `f32`, `f64`: Type annotation for floating point numbers. The number corresponds to how many bits are in the floating point number.
- `bool`: Type annotation for boolean (true or false) values.

## Literal Values
- `true` & `false`: Boolean literal values for true and false.

## Control Flow
- `if`: Declares a branch. A condition that evaluates to a boolean follows, and if this condition results in a `true` value then the branch executescondition results in a `true` value then the branch executes.
- `elif`: Follows an `if` branch. If the `if` branch's condition is false, and the condition for this statement is `true` then this branch will execute.
- `else`: Follows either an `if` or `elif` branch. If none of those conditions are `true`, then this branch will execute.
- `for`: Defines a traditional for loop. `for [init]; [condition]; [action] {...}` The `[init]` executes before the loop body, and the `[action]` will execute at the end of each iteration of the loop. The loop body will execute while the `[condition]` evaluates to `true`.
- `while`: Defines a while loop. `while [condition] {...}`. The body of the loop will execute while the `[condition]` evaluates to `true`.
- `continue`: When in a loop, this will skip the remainder of the loop body.
- `break`: This will break whatever loop it is nested in regardless of if the condition says otherwise.
- `return`: Whatever expression follows this is returned as the result of the parent function.
- `yield`: Most statements are expressions, meaning they evaluate to a result. `yield [expr]` will result in `[expr]` being returned as the value for whatever statement it belongs in. Note that this is different from `return` as it does not end the function, nor does it assign the function's result. You still need to `return` from functions rather than `yield`.
- `match`: Similar to switch statements in other languages and the match statement in Rust. `match [expr] {...}`. Inside the body are the branches of what `[expr]` can evaluate or **matched** to. This means that for `enum`s the variations of the enum can serve as branches.
- `default`: When defining a `match` expression, if none of the defined branches are matched, the `default` branch will execute.
- `defer`: Whatever statements are told to `defer` will be executed at the end of the code block automatically. If multiple `defer` statements are used, they will execute at the end of the scope in the order they were defined.
- 
