# CLI Threaded Search

Search text entered in the command line for a given string using multiple threads. 

### Initialization

After compilation, run search.c with two arguments. These arguments are a query string that will be found in the main text and the minimum size of the main text array, respectively. The main text can either be ported from a .txt file using the `<` command or entered into the command line as standard input after initializing  search. Enter `ctrl-d` when finished entering text.

### Note

This program is more a demonstration of threading in C than a robust search tool. Please note that the asynchronous nature of threads means the identified query location is not necessarily the first instance of the query string in the main text.
