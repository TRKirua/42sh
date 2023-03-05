#include "main.h"

int main(int argc, char **argv)
{
    if (argc > 3)
    {
        errx(1,
             "Syntax must be :\n"
             "./42sh -c $FILENAME\nOR\n"
             "./42sh < $FILENAME\nOR\n"
             "./42sh \"INPUT\"\nOR\n"
             "./42sh");
    }

    int r = 0;

    if (argc == 1)
    {
        if (isatty(0)) // When user types : ./42sh
            r = launch_shell();

        else // When user types : ./42sh < FILE
            r = input_stdin();
    }

    else if (argc == 2) // When user types : ./42sh FILE
        r = input_file(argv[1]);

    else // When user types : ./42sh -c "INPUT"
    {
        if (strcmp(argv[1], "-c"))
        {
            errx(1,
                 "Syntax must be :\n"
                 "./42sh -c $FILENAME\nOR\n"
                 "./42sh < $FILENAME\nOR\n"
                 "./42sh \"INPUT\"\nOR\n"
                 "./42sh\n");
        }

        r = input_quotes(argv[2]);
    }

    // printf("\n%d\n", r);
    return r;
}
