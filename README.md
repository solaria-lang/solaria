# The Solaria Programming Language

**WARNING** This programming language is in development.

## Name

The name is inspired by the science fiction series "The Foundation" by Isaac
Asimov. "Solaria" is the name of a world where robots do all the work to
maintain the planet, whereas human beings live in abundance.

## Building

With `gcc`:

```sh
gcc *.c -o solaria.out && ./solaria.out
```

## Project track

- [x] Add enhancement to store large amounts of constants (OP_CONSTANT_LONG).
- [x] Add support for keeping track of lines.
- [x] Add ability to store constants (OP_CONSTANT).
- [x] Add the "OP_RETURN" bytecode instruction.
- [x] Add a skeleton for debugging and inserting bytecode instructions.
