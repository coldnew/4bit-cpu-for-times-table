# 4-bit CPU for times table

## About

How do you implement a 4-bit CPU for calculate times table from 1 to 9 ?

In c, we simple use follwoing code to acive this:

```c
    #include <stdio.h>

    int main(int argc, char *argv[])
    {

            for (int i = 1; i < 10; i++) {
                    for (int j = 1; j < 10; j++) {
                            printf("%d x %d = %d\n", i, j, i * j);
                    }
                    printf("\n");
            }

            return 0;
    }
```