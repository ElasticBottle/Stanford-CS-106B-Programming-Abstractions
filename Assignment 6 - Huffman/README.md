# Assignment 6 - Huffman 

Details for [Huffman][1]!

The main aim of this assignment is to take a string of text and encode it using the Huffman encryption algorithm.

In order to do this, there is 4 parts:
1. Build a frequency table for the occurrences of each character (including space and punctuations).
2. Create a Encoding tree whereby all the characters are stored in a structure containing its value and character count.
    * These nodes are then stored in a Priority queue (implmented in assingment [PQueue][2]!). 
    * The priority value is the frequency in which the characters occur.
    * while there is more than one element in the queue:
        * Dequeue two elements, Their parent is a node whose value is empty and its count is the sum of the character count of both nodes.
        * insert the parent element back into the priority queue.
3. Build an encoding map mapping each character to its Huffman encoding value.
    * To do so, traverse the tree
        * Every left is a 0 inserted on the right most side of the characters encoding value thus far
        * Every right is a 1 inserted on the right most side of the characters encoding value thus far
        * When you hit a node with a character (leaf node) you have gotten the Huffman encoding value for that character
4. Encode the data.
    * Take each of the encoidng value for each character and mash them all together!

Of course, there is an issue of decrypting too.

In order to achieve that, we'll need some intermediary from step 1 to 3.

If we are outtputting to the file, we can store the frequency table as the header.

When that happens, encoding small files ends up making the file size larger becuase of the header.

But for large files?

The size of the ehader will become practically nothing and the size savings will ensue.

## Links to example runs

[Example run 1][3]

[Example run 2][4]

[Example run 3][5]

[1]: http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/huffman.html
[2]: ../Assignment%205%20-%20Priority%20queue/ 
[3]: http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164/assnFiles/huffman/output/expected-output-7.txt
[4]: http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164/assnFiles/huffman/output/expected-output-3.txt
[5]: http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164/assnFiles/huffman/output/expected-output-6.txt