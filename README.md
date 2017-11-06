simple recursive descent parser and interpreter of a subset of pascal.

Thanks to a great guide here: https://ruslanspivak.com/lsbasi-part1/


```bash
g++ main.cpp
./a.out examples/part_10_ast.pascal
```

# TODO
* Continue to part 14 of the blog
* Procedures are still unfinished

# What I would have done differently
* `Node` shouldn't have an enum to determine what type of node it is. Instead it should
have different derived classes. This would also simplify the node visitor.
