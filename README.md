# RecursiveTokenizer
Assignment 2: Recursive Indexing
This program goes through a computer's directory and fins all the files that can be read, gets all the tokens (delimiters are everything except a-z, 0-9), and sorts them alphanumerical, meaning alphabetical chars come before digits. The first things we do is build a tokenizer that goes through a singular files and gets all the tokens. We did this essentially the same way we did Assingment 0, but it wasn’t exactly the same since we had to include digits this time. The run time of this is the amount of words in the file (n). Then we had to insert every single word into a linked lists of linked lists, which has a runtime of O(n*m) where m is the amount of folders. Once we inserted, we have to sort everything. First we inserted the order of the words. Which was O(n^2), since we used selection sort. Then we hard to order the occurrences, is O(m^2), and then order the files which is O(m^2).
NOTE:
./index output ~desktop/file.txt
Will create the output file where our executable is.
./index output ~desktop/folder
Will create the output file on the desktop, so where the folder to index is.
./index somePath/output folder
Will check if its a valid path and put output file in there if it doesn’t exist.
