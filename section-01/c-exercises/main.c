#include <stdio.h>
#include <stdlib.h>

// Exercise 1: Hello World and compiling with gcc
void hello_world()
{
    printf("Hello World\n");
}

// Bonus: Swap to integers using pointers.
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Exercise 1.1: Improving code
matrix_t matrix_transpose(matrix_t m)
{
    if (!m.data || m.rows <= 0 || m.cols <= 0) Make sure the input is valid
        return create_matrix(0, 0);

    matrix_t mt = create_matrix(m.cols, m.rows);

    for (int i = 0; i < m.rows; ++i)
    {
        for (int j = 0; j < m.cols; ++j)
        {
            mt.data[j * m.rows + i] = m.data[i * m.cols + j];
        }
    }

    return mt;
}
*/

// Exercise 2: FizzBuzz
void fizzbuzz(int n)
{
    if (!(n % 15))
        printf("FizzBuzz\n");
    else if (!(n % 3))
        printf("Fizz\n");
    else if (!(n % 5))
        printf("Buzz\n");
}

// Exercise 3: Function pointer practice
int compare(const void *a, const void *b)
{
    return  (*(int *)b - *(int *)a);
}

int main()
{
    // Exercise 1
    hello_world();

    int a = 0, b = 1;
    swap(&a, &b);
    printf("a: %d, b: %d\n", a, b);

    // Exercise 2
    int *arr = (int *)malloc(20 * sizeof(int));

    for (int i = 0; i < 20; i++)
        *(arr + i) = i + 1;

    for (int i = 0; i < 20; i++)
        fizzbuzz(*(arr + i));

    for (int i = 0; i < 30; i++)
        fizzbuzz(i);

    //Exercise 3
    qsort(arr, 20, sizeof(int), compare);
    for (int i = 0; i < 20; i++)
        printf("%d ", *(arr + i));

    free(arr);
    return 0;
}
