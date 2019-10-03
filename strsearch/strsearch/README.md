# String Search Toybox

This is just a personal project I made to learn C++, CMake, googletest,
googlebench, and think about some string searching algorithms all while doing it.

Currently I've got a bunch of algorithms implemented:

1. Naive string search
2. String search by tracking all substrings in a huge list
3. An efficient implementation of a string search that I came up with myself,
   before googling whether or not my algorithm actually had a proper name.
   Turns out it did - I re-invented Knuth-Morris-Pratt.
   
## What I learned

While building this project, I touched a bunch of fancy things:

- CMake. This was the first CMake thing I wrote. C++ dependencies are a pain in
  the rear, to say the least, compared to other languages I've worked with.
  I didn't want to install crap into `/usr/local`.
- googletest and googlebench. Of particular interest, I briefly glanced at
  the googletest source code which was the inspiration for the layout of this
  project.
- Automated test and benchmark discovery, and a macro to help me implement the
  string searching algorithms.

## The algorithms

### Naive

This is simply just brute force. It's slow.

### Giant-List-of-Substrings

This was an idea I had. To traverse the haystack only once, I kept a list of
every single substring match of the needle.

This is even slower than brute force, probably because of the painfully slow
speeds of allocating memory and dancing around lists.

### Knuth-Morris-Pratt

I came up with this myself. The intuition was:

- When I hit an "incorrect" character in the text, this character should be
  associated with a possible prefix of the needle.
- Prefixes can be easily computed, but how do we do this efficiently?
- Can take advantage of the recursive structure - if the needle contains the
  needle, then the needle must contain the needle... (lolwat?). We keep track
  of the longest prefix as a result of any incorrect character.
- We only need to consider a prefix if:
   - The corresponding character in the needle doesn't match the prefix of the
     needle (i.e. if the "correct" character was substituted in, then we have a
     substring) or
   - The needle recursively contains a prefix, in which case even if the
     character in the needle is currently matching, we can substitute in an
     "incorrect" character to get a possible prefix.
- Also thus, the actual search itself needs to attempt multiple possible
  prefixes every time it encounters an invalid character by repeatedly
  shrinking the needle + error array.
  
### Rotating Hash

The hash implementation is pretty basic. Instead of comparing each substring
why not maintain a rolling hash. The theory is that computing the hash should
obviously be much cheaper than comparing little bits of the string each time.

The hash function I use is simply the polynomial:

```
c_1*x^(n-1) + c_2*x^(n-2) ... + c_n
```

Where x is the maximum size of `unsigned char`.

On every iteration I multiply the whole expr by `x` and add `c_n`. The modulo
is just whatever `unsigned int` is. This is obviously safe for modulo arithmetic.

On every iteration, to pop the first element, I subtract the first term modulo
`unsigned int` size. This is safe:

* The whole expr `t` is obviously congruent to its modulo `t%m`.
* The first term `f` is obviously congruent to its modulo `f%m`.
* Therefore obviously `t-f == t%m - f%m (mod m)`.
* For an unsigned int, subtraction of modulo behaves sanely.
