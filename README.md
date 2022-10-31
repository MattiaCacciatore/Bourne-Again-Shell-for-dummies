# Bourne Shell Again BASIC VERSION
My biggest personal and university project so far. Written entirely in C.

The purpose of this project is to implement a small shell in order to get familiar 
with the basic POSIX system calls for process management. It's pretty simple
and basic, it lacks a lot of features, for example, it's required there is no blanks between 
the '>' character and the name of the file to redirect the standard output of a command, 
also it doesn't handle filename expansion, escape sequences, strings, process groups, 
foreground/background processes and many other things.

- it waits for a command name to be entered (to get user's input I used the `fgetc()` instead of `fgets()` or `readline()` from the readline library simply because
- `fgets()` require a size limit for the input which cannot be determinate by default, and `readline()` wasn't very helpful for my learning purpose);
- if `exit` or `^D` are entered, it exits with *exit status* `EXIT_SUCCESS` freeing all memory allocated previously;
- it runs `/bin/<command given by user>`(or `/usr/bin/<command given by user>`, if the given command is not found in `/bin/`), through the `fork()` and `exec()` system calls.
- it uses `wait()` to wait for child process(es) to end;
- it performs command(s) like `ls -l >` *filename*, using the `dup()` system call.
- it performs command(s) like `ps aux | grep bash` using the `pipe()` system call. 

It can run one or more commands, redirect the standard input and output,
print the command history and it has an unique built-in command, cd 
(change directory).

$ make

$ ./ubash

For the readline library: $ sudo apt-get install libreadline-dev

Be aware that Prof. Lagorio is aware of this code. DON'T EVEN TRY.
