# Wave Programming Language

A simple, low-level oriented programming language.

## Overview

Everything in wave is written around statements.

Statements are a named function with zero or more input arguments and zero or more output arguments.

Functions are declared as compressed statements.

```
example.function(a b c)(d e f) = {
    [ multiline comment ]
    [ statements ]
    foo(a b)(e k)
    bar(k b)(e s)
    baz(a s)(e f)

    [ offsets ]
    @offset
}
```
