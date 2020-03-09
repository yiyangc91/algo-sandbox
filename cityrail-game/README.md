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

Usage
------

This output doesn't exist at the time, but I'd imagine it to look something
like this:

   ./unary 10 4 8 4 7
   4+8=12 [12 4 7]
   12/4=3 [3 7]
   3+7=10 [10]

Alternatively there might be a tree representation of this somehow.
