A recreation of the linux terminal pipes ( | ), including redirections ( > and >> ), and HEREDOC.

# Intro

This pipex is written with the intention to make it as easily to read and follow for other 42 students as possible.
For that reason, I have split functions that could have been compressed into one, into multiple easy-to-read functions.
Each function is also complimented with a rundown of what is happening. 

Should you find the code difficult to understand, I would highly suggest to read up on the following functions/concepts:

fork()
pipe()
dup2()
access()
open()
close()

The better you understand how these work together, the easier this project will be for you.
