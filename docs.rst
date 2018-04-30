====
Slip
====
Syntax
------
The syntax is based on s-expressions and is somewhat similar to lisp. It differs
from lisp in that it does not support macros, is interpreted, and has no form of
memory management. The language is expression based and only executes a single
expression at a time. Expressions can be combined into files and executed one
after the other. In files, lines that start with # are not read.

Anything that does not start with a ( or a $ will be treated as a string and
left alone. Otherwise, it will be evaluated or substituted. If you want to treat
something like a string no matter what, wrap it in quotes. For example: ::

  `(+ 1 1)'

returns the string (+ 1 1) instead of 2.

Built in functions
------------------
There are a handful of built in functions.

(+)
  The addition operator. Adds all parameters together.

(-)
  The subtraction operator. Subtracts all parameters from the first.

(*)
  The multiplication operator. Multiplies all parameters together.

(-)
  The division operator. divides the first parameter by the rest.

>
  The greater than operator. Returns "1" if parameter 1 is greater than
  parameter 2. returns "0" otherwise.

<
  The less than operator. Returns "1" if parameter 1 is less than
  parameter 2. returns "0" otherwise.

=
  The equality than operator. Returns "1" if parameter 1 is equal to
  parameter 2. returns "0" otherwise.

++
  The concatenation operator. Combines all parameters together as strings.

at
  The index operator. Gets the character from the string in the first parameter
  based on the number in the second. (at hello 3) returns l.

id
  Returns the result of the last parameter.

isDef
  Returns "1" if the first parameter is defined and "0" if not.

print
  prints out each parameter separated by a space.

input
  Prints the first parameter followed by a space. Then grabs the next word the
  user enters and returns it.

load
  Executes the contents of each parameter. for example, (load std.lsp) would
  load the standard library.

define
  Associates a given value (in the second argument) with a name (in the first).


Standard Library
----------------
The standard library is pretty much empty but growing:

(if <1 or 0> <block if 1> <block if 0>)
  Executes the correct block depending on the condition. Ignores the other block
  of code. The first parameter must be a value while the other two must be
  functions.

(for <initial value> <condition> <body>)
  Executes the given condition every iteration. If true, it executes the body as
  well. If false, it returns the last value that the body returned. You can
  expect the last value returned by the body or the initial value to be stored
  in $p4.

(game)
  Play a guess the number game. Try to guess a number between 1 and 100.
