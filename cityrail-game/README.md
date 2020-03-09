Cityrail Game
=============

This is a terrible name for that game people play in Sydney where they try
to add up a bunch of numbers to 10. I thought I'd try write something that
could solve this problem for any number of numbers, to any sum.

E.g. for example, 4, 8, 4, 7 summing to 10:

- (4+8) / 4 + 7 == 10

The restrictions are:
- No unary -.
- No integer arithmetic - this problem uses real numbers. There should be no
  loss of precision from using floating point numbers (i.e. find a nicer way
  to represent this).
- Assume all numbers are several magnitudes smaller than INT\_MAX or INT\_MIN.

TreeSolver
----------

The treesolver code is a bit of a mess, spitting out memory everywhere.

The whole crap with this is that we maintain O(n^2) memory (where n is the length
of the input parameters), with each successive "level" storing nodes of a tree
that links up to the previous level's nodes.

On each level, for each iteration of some pair (i,j), we:
1. "Merge" (i,j) at position i on the next level.
2. Pad the rest of the list with the remaining elements
3. Twiddle the ith element for each operation (6 ops)

On the last level (when everything is merged), we used to copy the whole tree by
cloning it into the heap somewhere (probably... slow).

Actually now I've opted to change the signature to take a callback function, to
avoid this whole mess. I've left the tree code in there as a reminder.

To make matters worse, to avoid even more allocations:
1. I call `vector.reserve(x)` to preallocate the vectors.
2. The vector is a vector of variants, to allow for static polymorphism.

This means the code is a humongous mess trying to figure out what alternatives
there are. I mean, "theoretically" I can just reinterpret a pointer to the
variant as a `Node*` but I'm not sure if that's ... legal in C++. Works on
clang though?

Is this thing correct? NO IDEA. Haven't tested it yet. Generated formulas for 4
seems correct.

DFSSolver
---------

Yeah so this is something I want to write, but actually I DO NOT WANT TO WRITE.

Instead of merging two elements and attaching it to the nodes above,
this looks at the tree top down, and opts to "split" the elements

Honestly, sounds complex as hell. The main gist is we have a function that,
given some numbers, is able to generate all tree combinations and apply a func.
The function would then be something like this:

1. Generate all possible sublists split by size [1..N]
2. Pass each sublist recursively to itself. The callback would then take the
   node of the generated tree, then recursively call itself on the RHS list.
3. The second callback now has both sides of the generated tree, and it can
   create a node that represents the final tree, and it then calls it's own
   callback.

Usage
------

This output doesn't exist at the time, but I'd imagine it to look something
like this:

```
./unary 10 4 8 4 7
4+8=12 [12 4 7]
12/4=3 [3 7]
3+7=10 [10]
```

Alternatively there might be a tree representation of this somehow.
