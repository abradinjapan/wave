# Wave Programming Language

A simple, low-level oriented programming language.

## Basic Syntax

Everything in wave is written around statements.

Statements are a named function with a set of input arguments and a set of output arguments.

For example, if we wanted to perform addition, you would write:

```
wave.integer.add(a b)(c)
```

There are no commas between arguments, just whitespace.

# Functions

In order to declare a function, you simply must repeat the statement syntax, followed by an equals sign, followed by a scope that contain your statements.

```
test.function(a b c)(d e f) = {
    foo(a b)(e k)
    bar(k b)(e s)
    baz(a s)(e f)
}
```
