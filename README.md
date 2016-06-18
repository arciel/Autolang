# Autolang 
[![Build Status](https://travis-ci.org/TusharRakheja/Autolang.svg?branch=master)](https://travis-ci.org/TusharRakheja/Autolang)

**Autolang** is a procedural, interpreted programming language for math lovers.

It supports **primitive** data types (_int_, _char_, and _logical_), **containers** (*sets*, *tuples*, and *maps*), and a *derived* data type, *strings*. 

### Building

**Dependencies**

 - `gcc` 4.8 or later
 - GNU `make`

Clone (or alternatively download) the repo.
```
$ git clone https://github.com/TusharRakheja/Autolang
```
Then navigate into the directory and run make.
```
$ cd Autolang
$ make
$ make clean
```

### Usage

Autolang can be used either with a file, or interactively. The filename argument is optional.


`$ auto filename.al`

### Examples

The real joy of Autolang is its very math-oriented syntax. Here are some cool examples you can try. 

##### 1.
```python
>>> set A = {1, 2, 3} x {'A', 'B'}      # Cartesian Product
>>> print A
{(1, 'A'), (1, 'B'), (2, 'A'), (2, 'B'), (3, 'A'), (3, 'B')} 
```

##### 2. 
```python
>>> print {(1, 'B')} c A                # Is this set a subset of A?
True
```

##### 3.
```python
>>> print (2 + 10 * 2) == (2 * 10 + 2)  # Beware, no operator precedence!
False
```

##### 4.
```python
>>> string first = "Tushar"
>>> string last = "Rakheja"
>>> tuple entry = (first, last)         # Identifiers can be used as elements.
>>> print entry
("Tushar", "Rakheja")
```

##### 5.
```python
>>> if (True V False)                   # Will quit the shell.
{ quit }
```

To see an example of how the while loop (the only looping construct in the language, right now) and some other things in the language work, check `Examples/example6.al` out. A technical guide is coming soon.

##### Future Plans/Ideas

 * [ ] Lexical scoping.
 
 * [ ] Automatic memory management.
 
 * [ ] Abstract Maps/Lambda Expressions (Mapping criteria instead of fixed mappings, eg x --> x + 2)

 * [ ] Abstract Sets (Membership criteria instead of fixed members). Probably can be simulated via Abstract Maps.
 

##### License

Copyright (c) 2016 Tushar Rakheja (The MIT License)