Assumptions:
    1. For cd:
        - If number of arguments after cd >= 2, program will throw an error
        - If the arguments are different from the flags, program will throw an error

    2. For echo:
        - No assumptions made

    3. For pwd:
        - No assumptions made

    4. For ls:
        - It prints contents of each directory/file, in case of multiple inputs separately, with their names mentioned on the top.
        - Ordering of flags and directory / file names does not matter

    5. For bg & fg:
        - No assumptions made
    
    6. For pinfo:
        - No assumptions made

    7. For discover:
        - If flags (-f or -d) are mentioned when a file is given, it given an error
        - Only -d anf -f flags are executable. -df and -fd are not.

    8. For history:
        - It stores all commands typed on the terminal, even if they are not valid or throw an error.

Execution:
    - Running "make all" command on the terminal in the appropriate directory will compile all the .c files