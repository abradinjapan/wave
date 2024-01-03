# Wave Programming Language

A simple, low-level oriented programming language.

## Basic Syntax

Everything in wave is written around statements.

Statements are a named function with zero or more input arguments and zero or more output arguments.

For example, if we wanted to perform integer addition, you would write:

```
wave.integer.add(a b)(c)
```

There are no commas between arguments, just whitespace.

## Functions

In order to declare a function, you simply must repeat the statement syntax, followed by an equals sign, followed by a scope that contain your statements.

```
test.function(a b c)(d e f) = {
    foo(a b)(e k)
    bar(k b)(e s)
    baz(a s)(e f)
}
```

The idea is that statements are just functions collapsed together, hence the '='.
