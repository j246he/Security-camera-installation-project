# ECE650 : Project

Augment your code from Assignment 4 in the way that is decribed below.
Quantitatively analyze your software for various kinds of inputs.
Write a brief report (5 pages, 11 pt font, reasonable margins) with your analysis. Your report must be typeset, and must be in PDF.

You should augment your code from Assignment 4 in the following ways.
Make it multithreaded. You should have at least 4 threads: one for I/O, and one each for the different approaches to solve the minimum vertex cover problem.
Implement the following two additional ways to solve MIN-VERTEX-COVER, in addition to the REDUCTION-TO-CNF-SAT approach you had in Assignment 4. (We will call your
approach from Assignment 4, CNF-SAT-VC.)
1. Pick a vertex of highest degree (most incident edges). Add it to your vertex cover and throw away all edges incident on that vertex. Repeat till no edges remain. We will call
this algorithm APPROX-VC-1.
2. Pick an edge hu; vi, and add both u and v to your vertex cover. Throw away all edges attached to u and v. Repeat till no edges remain. We will call this algorithm APPROX-VC-2.
