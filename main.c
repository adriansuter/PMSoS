#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>


/** \brief Linked list of factor pairs. */
typedef struct mpz_factor_list
{
    /** \brief The first factor. */
    mpz_t factor1;
    /** \brief The second factor. */
    mpz_t factor2;
    /** \brief Pointer to the next item. */
    struct mpz_factor_list * next;
} mpz_factor_list_t;


/** \brief Pushes a new factor pair into the given list.
 *
 * The passed parameters \p factor1 and \p factor2 can safely be cleared after
 * calling the function as the function creates new ones internally.
 *
 * A new factor pair would be pushed to the beginning of the list.
 *
 * \param list mpz_factor_list_t** The list.
 * \param factor1 mpz_t The first factor.
 * \param factor2 mpz_t The second factor.
 * \return void
 */
void mpz_factor_list_push(mpz_factor_list_t ** list, mpz_t factor1, mpz_t factor2)
{
    // Create a new node.
    mpz_factor_list_t * newNode;
    newNode = malloc(sizeof(mpz_factor_list_t));

    // Set the factors of the node.
    mpz_init_set(newNode->factor1, factor1);
    mpz_init_set(newNode->factor2, factor2);

    // Insert the new node as first element of the list.
    newNode->next = *list;
    *list = newNode;
}


/** \brief Pops the first factor pair from the given list.
 *
 * The function would remove the first factor pair and release all memory used
 * by that element.
 *
 * \param list mpz_factor_list_t** The list.
 * \return void
 *
 */
void mpz_factor_list_pop(mpz_factor_list_t ** list)
{
    if (*list == NULL)
    {
        return;
    }

    mpz_clear((*list)->factor1);
    mpz_clear((*list)->factor2);

    mpz_factor_list_t * nextNode = NULL;
    nextNode = (*list)->next;

    free(*list);
    *list = nextNode;
}


/** \brief Cleans a given list.
 *
 * The function would clean the whole list and release all memory used by all
 * the elements previously in the list.
 *
 * \param list mpz_factor_list_t** The list.
 * \return void
 *
 */
void mpz_factor_list_clean(mpz_factor_list_t ** list)
{
    while ( *list != NULL )
    {
        mpz_factor_list_pop(list);
    }
}


/** \brief Prints the given list of factor pairs to the stdout.
 *
 * \param list mpz_factor_list_t* The list.
 * \return void
 */
void mpz_factor_list_print(mpz_factor_list_t * list)
{
    mpz_factor_list_t * current = list;

    while (current != NULL)
    {
        mpz_out_str(stdout, 10, current->factor1);
        printf(" x ");
        mpz_out_str(stdout, 10, current->factor2);
        printf("\n");

        current = current->next;
    }
}


/** \brief Linked list of arithmetic progressions. */
typedef struct mpz_ap_list
{
    /** \brief The small square number. */
    mpz_t x;
    /** \brief The middle square number. */
    mpz_t y;
    /** \brief The big square number. */
    mpz_t z;
    /** \brief The distance between the middle square and the small square number. */
    mpz_t d;
    /** \brief The pointer to the next item. */
    struct mpz_ap_list * next;
} mpz_ap_list_t;


/** \brief Inserts a new arithmetic progression into the given list at the
 * correct position.
 *
 * This function would not insert if there is already the same arithmetic
 * progression in the list. The list would always be ordered ascending by
 * distance, that is by \p y - \p x = \p z - \p y = \p d. Note that it is your
 * duty to make sure that \p x < \p y < \p z and that \p y - \p x = \p z -
 * \p y. The function would not check for that.
 *
 * The passed parameters \p x, \p y and \p z can be cleared after calling this
 * function as the function would create new ones internally.
 *
 * \param list mpz_ap_list_t** The list.
 * \param x mpz_t The small square number.
 * \param y mpz_t The middle square number.
 * \param z mpz_t The big square number.
 * \return void
 */
void mpz_ap_list_insert(mpz_ap_list_t ** list, mpz_t x, mpz_t y, mpz_t z)
{
    // Calculate the distance d = y - x.
    mpz_t d;
    mpz_init(d);
    mpz_sub(d, y, x);

    mpz_ap_list_t * current = * list;
    if ( current == NULL || mpz_cmp(d, current->d) < 0 )
    {
        // Either this is the first element or the existing first element has
        // a bigger distance. We have to insert the new element as first
        // element.
        mpz_ap_list_t * newNode;
        newNode = malloc(sizeof(mpz_ap_list_t));

        // Set the values.
        mpz_init_set(newNode->x, x);
        mpz_init_set(newNode->y, y);
        mpz_init_set(newNode->z, z);
        mpz_init_set(newNode->d, d);

        // Insert the new node as first element into the list.
        newNode->next = *list;
        *list = newNode;

        // Clear d and return.
        mpz_clear(d);
        return;
    }

    // Iterate through the list and search for the element that has a bigger d.
    // If found we will insert a new element before the found one. If not we
    // will append a new element to the list.
    while (current != NULL)
    {
        if ( mpz_cmp(x, current->x) == 0 )
        {
            // If there is already an element with the same x, then we can
            // assume that this arithmetic progression exists already. Therefore
            // we would not change anything.
            break;
        }

        if ( current->next == NULL )
        {
            // We have reached the end of the list. Therefore we have to append
            // this arithmetic progression to the end of the list.
            mpz_ap_list_t * newNode;
            newNode = malloc(sizeof(mpz_ap_list_t));

            // Set the values.
            mpz_init_set(newNode->x, x);
            mpz_init_set(newNode->y, y);
            mpz_init_set(newNode->z, z);
            mpz_init_set(newNode->d, d);

            // Append the new node as last element to the list.
            newNode->next = NULL;
            current->next = newNode;
            break;
        }

        if ( current->next != NULL )
        {
            if ( mpz_cmp(x, current->next->x) == 0 )
            {
                // If there is already an element with the same x, then we can
                // assume that this arithmetic progression exists already.
                // Therefore we would not change anything.
                break;
            }

            if ( mpz_cmp(current->next->d, d) >= 0 )
            {
                // The next element in the list has a bigger distance. Therefore
                // we will insert the new arithmetic progression right after
                // the current one but before the next one.
                mpz_ap_list_t * newNode;
                newNode = malloc(sizeof(mpz_ap_list_t));

                // Set the values.
                mpz_init_set(newNode->x, x);
                mpz_init_set(newNode->y, y);
                mpz_init_set(newNode->z, z);
                mpz_init_set(newNode->d, d);

                // Insert the new node between the current and the next one in
                // the list.
                newNode->next = current->next;
                current->next = newNode;
                break;
            }
        }

        current = current->next;
    }

    // Clear d.
    mpz_clear(d);
}


void mpz_ap_list_pop(mpz_ap_list_t ** list)
{
    if (*list == NULL)
    {
        return;
    }

    mpz_clear((*list)->x);
    mpz_clear((*list)->y);
    mpz_clear((*list)->z);
    mpz_clear((*list)->d);

    mpz_ap_list_t * nextNode = NULL;
    nextNode = (*list)->next;

    free(*list);
    *list = nextNode;
}


void mpz_ap_list_clean(mpz_ap_list_t ** list)
{
    while ( *list != NULL )
    {
        mpz_ap_list_pop(list);
    }
}


void mpz_ap_list_print(mpz_ap_list_t * list)
{
    mpz_ap_list_t * current = list;

    while (current != NULL)
    {
        mpz_out_str(stdout, 10, current->x);
        printf(", ");
        mpz_out_str(stdout, 10, current->y);
        printf(", ");
        mpz_out_str(stdout, 10, current->z);
        printf(" | ");
        mpz_out_str(stdout, 10, current->d);
        printf("\n");

        current = current->next;
    }
}


///
/// Calculate arithmetic progressions via Pythagorean triples.
///
void calc(mpz_ap_list_t ** arithmeticProgressions, mpz_t p1, mpz_t p2, mpz_t m, mpz_t n, mpz_t mSquared, mpz_t nSquared, mpz_t x1, mpz_t x2, mpz_t x3, mpz_t a1, mpz_t a2, mpz_t a3)
{
    int cmp;

    // m = floor(sqrt(p1))
    mpz_sqrt(m, p1);

    // n = 1
    mpz_set_ui(n, 1);

    mpz_pow_ui(mSquared, m, 2);
    mpz_pow_ui(nSquared, n, 2);
    while ( mpz_cmp(m, n) > 0 )
    {
        // x3 = m^2 + n^2
        mpz_add(x3, mSquared, nSquared);

        cmp = mpz_cmp(x3, p1);
        if ( cmp < 0 )
        {
            mpz_add_ui(n, n, 1);
            mpz_pow_ui(nSquared, n, 2);
        }
        else if ( cmp > 0 )
        {
            mpz_sub_ui(m, m, 1);
            mpz_pow_ui(mSquared, m, 2);
        }
        else
        {
            // x1 = m^2 - n^2
            mpz_sub(x1, mSquared, nSquared);

            // x2 = 2 * m * n
            mpz_mul(x2, m, n);
            mpz_mul_ui(x2, x2, 2);

            // Scale by p2
            mpz_mul(x1, x1, p2);
            mpz_mul(x2, x2, p2);
            mpz_mul(x3, x3, p2);

            // (x2 - x1)^2
            mpz_sub(a1, x2, x1);
            mpz_pow_ui(a1, a1, 2);

            // x3^2
            mpz_pow_ui(a2, x3, 2);

            // (x1 + x2)^2
            mpz_add(a3, x1, x2);
            mpz_pow_ui(a3, a3, 2);

            mpz_ap_list_insert(arithmeticProgressions, a1, a2, a3);

            mpz_sub_ui(m, m, 1);
            mpz_pow_ui(mSquared, m, 2);
        }
    }

}

int main()
{
    mpz_t input, number, numberSquared, numberSqrt, f1, f2;
    mpz_t m, n, mSquared, nSquared, x1, x2, x3, a1, a2, a3, a7, a8, a9;
    mpz_t a, b, c, d, e;

    mpz_init(input);
    mpz_init(number);
    mpz_init(numberSquared);
    mpz_init(numberSqrt);
    mpz_init(f1);
    mpz_init(f2);
    mpz_init(m);
    mpz_init(n);
    mpz_init(mSquared);
    mpz_init(nSquared);
    mpz_init(x1);
    mpz_init(x2);
    mpz_init(x3);
    mpz_init(a1);
    mpz_init(a2);
    mpz_init(a3);
    mpz_init(a7);
    mpz_init(a8);
    mpz_init(a9);

    mpz_init(a);
    mpz_init(b);
    mpz_init(c);
    mpz_init(d);
    mpz_init(e);

    mpz_factor_list_t * factorPairs = NULL;
    mpz_ap_list_t * arithmeticProgressions = NULL;
    mpz_ap_list_t * AP1;
    mpz_ap_list_t * AP2;
    mpz_ap_list_t * AP3;

    int factorPairsLength = 0;
    int plusMinus = 1;
    int read;
    int strlength = 0;
    char buf[BUFSIZ];

    while ( 1 )
    {
        //
        // Read from the input.
        //
        read = 1;
        while ( read == 1 )
        {
            fgets(buf, sizeof buf, stdin);
            strlength = 0;

            while (buf[strlength] != '\0') {
                strlength++;
            }

            if (strlength > 0 && buf[strlength - 1] == '\n')
            {
                if ( buf[0] == 'q' )
                {
                    exit(0);
                }
                else
                {
                    if ( buf[strlength - 2] == '+' )
                    {
                        plusMinus = 1;
                        buf[strlength - 2] = '\n';
                    }
                    else if ( buf[strlength - 2] == '-' )
                    {
                        plusMinus = -1;
                        buf[strlength - 2] = '\n';
                    }
                    else
                    {
                        plusMinus = 1;
                    }

                    if ( 0 == mpz_set_str(input, &buf[0], 10) )
                    {
                        read = 0;
                    }
                    else
                    {
                        // ERROR: Given input was not a valid number.
                        exit(1);
                    }
                }
            }
            else
            {
                // ERROR: Input was truncated or pipe stream was closed.
                exit(2);
            }
        }

        // number = 6 * input + plusMinus
        mpz_mul_ui(number, input, 6);
        if ( plusMinus > 0 )
        {
            mpz_add_ui(number, number, 1);
        }
        else
        {
            mpz_sub_ui(number, number, 1);
        }

        // numberSquared = number^2
        mpz_pow_ui(numberSquared, number, 2);

#ifdef DEBUG
        printf("Input: ");
        mpz_out_str(stdout, 10, input);
        printf(", %d\n", plusMinus);
        printf("Number: ");
        mpz_out_str(stdout, 10, number);
        printf(", Number^2: ");
        mpz_out_str(stdout, 10, numberSquared);
        printf("\n");
#endif


        /// ///
        /// Get a list of all possible factor pairs f1, f2 such that f1*f2 = number. Note that f1 and f2 may be equal, i.e. f1^2 = number.
        /// The algorithm used here is very simple - actually some sort of brute force. We know that it would be better to
        /// first find the prime factors and then to calculate all possible combinations of the found prime factors.
        /// But the simple approach is still quite speedy :-).
        /// ///
        mpz_sqrt(numberSqrt, number);
        mpz_set_ui(f1, 1);
        while ( mpz_cmp(f1, numberSqrt) <= 0)
        {
            if ( mpz_divisible_p(number, f1) != 0 )
            {
                mpz_div(f2, number, f1);

                mpz_factor_list_push(&factorPairs, f1, f2);
                factorPairsLength++;
            }

            mpz_add_ui(f1, f1, 1);
        }
#ifdef DEBUG
        printf("-- Factor Pairs --\n");
        mpz_factor_list_print(factorPairs);
#endif


        /// ///
        /// Iterate over all factor pairs and calculate the Arithmetic Progressions
        /// via Pythagorean Triples.
        /// ///
        while ( factorPairsLength > 0 )
        {
            mpz_set(f1, factorPairs->factor1);
            mpz_set(f2, factorPairs->factor2);
            mpz_factor_list_pop(&factorPairs);
            factorPairsLength --;

            calc(&arithmeticProgressions, f1, f2, m, n, mSquared, nSquared, x1, x2, x3, a1, a2, a3);
            if ( mpz_cmp(f1, f2) != 0 )
            {
                calc(&arithmeticProgressions, f2, f1, m, n, mSquared, nSquared, x1, x2, x3, a1, a2, a3);
            }
        }
#ifdef DEBUG
        printf("-- Arithmetic Progressions --\n");
        mpz_ap_list_print(arithmeticProgressions);
#endif


        long result = 0;

        /// ///
        /// Iterate through all combinations of arithmetic progressions AP1 and AP2
        /// with the condition that the distance of AP1 (call it a) is smaller than
        /// the distance of AP2 (call it b). Further we get the middle square number
        /// of AP1 (which is the same as the middle square number of AP2) and call
        /// it c.
        ///
        /// If b == 2a then we can continue with the next combination.
        /// Otherwise we will calculate d = a + b and e = a - b.
        /// If d >= c or e >= c we can continue with the next combination, as then
        /// d - e or c - e would be negative.
        /// ///
#ifdef DEBUG
        printf("-- Valid Combinations --\n");
#endif
        AP1 = arithmeticProgressions;
        while (AP1 != NULL)
        {
            AP2 = AP1->next;
            while ( AP2 != NULL )
            {
#ifdef DEBUG
                printf("(");
                mpz_out_str(stdout, 10, AP1->d);
                printf(", ");
                mpz_out_str(stdout, 10, AP2->d);
                printf(")");
#endif

                mpz_set(a, AP1->d);
                mpz_set(b, AP2->d);
                mpz_set(c, AP1->y);

                // a2 = 2 * a
                mpz_mul_ui(a2, a, 2);
                if ( mpz_cmp(b, a2) == 0 )
                {
#ifdef DEBUG
                    printf(" Skip as b = 2 * a\n");
#endif
                    AP2 = AP2->next;
                    continue;
                }

                // d = a + b
                mpz_add(d, a, b);
                if ( mpz_cmp(d, c) >= 0 )
                {
#ifdef DEBUG
                    printf(" Skip as a + b >= c\n");
#endif
                    AP2 = AP2->next;
                    continue;
                }

                // e = a - b
                mpz_sub(e, a, b);

                // s1 = c - b [x1]
                mpz_set(x1, AP2->x);

                // s2 = c + (a + b) [x2]
                mpz_add(x2, AP1->y, d);

                // s3 = c - a [x3]
                mpz_set(x3, AP1->x);

                // s4 = c - (a - b) [a1]
                mpz_sub(a1, AP1->y, e);

                // s5 = c [a2]
                mpz_set(a2, AP1->y);

                // s6 = c + (a - b) [a3]
                mpz_add(a3, AP1->y, e);

                // s7 = c + a [a7]
                mpz_set(a7, AP1->z);

                // s8 = c - (a + b) [a8]
                mpz_sub(a8, AP1->y, d);

                // s9 = c + b [a9]
                mpz_set(a9, AP2->z);

#ifdef DEBUG
                printf("...\n");
                mpz_out_str(stdout, 10, x1);
                printf(" ");
                mpz_out_str(stdout, 10, x2);
                printf(" ");
                mpz_out_str(stdout, 10, x3);
                printf("\n");
                mpz_out_str(stdout, 10, a1);
                printf(" ");
                mpz_out_str(stdout, 10, a2);
                printf(" ");
                mpz_out_str(stdout, 10, a3);
                printf("\n");
                mpz_out_str(stdout, 10, a7);
                printf(" ");
                mpz_out_str(stdout, 10, a8);
                printf(" ");
                mpz_out_str(stdout, 10, a9);
                printf("\n");
#endif

                int s2PerfectSquare = 0;
                if ( mpz_perfect_square_p(x2) != 0 )
                {
                    // This seems to be a perfect square.
                    mpz_sqrt(m, x2);
                    mpz_pow_ui(m, m, 2);
                    if ( mpz_cmp(m, x2) == 0 )
                    {
                        s2PerfectSquare = 1;
                    }
                }

                int s4PerfectSquare = 0;
                if ( mpz_perfect_square_p(a1) != 0 )
                {
                    // This seems to be a perfect square.
                    mpz_sqrt(m, a1);
                    mpz_pow_ui(m, m, 2);
                    if ( mpz_cmp(m, a1) == 0 )
                    {
                        s4PerfectSquare = 1;
                    }
                }

                int s6PerfectSquare = 0;
                if ( mpz_perfect_square_p(a3) != 0 )
                {
                    // This seems to be a perfect square.
                    mpz_sqrt(m, a3);
                    mpz_pow_ui(m, m, 2);
                    if ( mpz_cmp(m, a3) == 0 )
                    {
                        s6PerfectSquare = 1;
                    }
                }

                int s8PerfectSquare = 0;
                if ( mpz_perfect_square_p(a8) != 0 )
                {
                    // This seems to be a perfect square.
                    mpz_sqrt(m, a8);
                    mpz_pow_ui(m, m, 2);
                    if ( mpz_cmp(m, a8) == 0 )
                    {
                        s8PerfectSquare = 1;
                    }
                }

                int nrPerfectSquares = 5
                                       + s2PerfectSquare
                                       + s4PerfectSquare
                                       + s6PerfectSquare
                                       + s8PerfectSquare;

                if ( nrPerfectSquares > 6 )
                {
                    FILE *fp;
                    char filename[100];
                    char generator[80];

                    result ++;
                    mpz_get_str(generator, 10, input);

                    sprintf(filename, "ps,%d,%s%s,%ld.result", nrPerfectSquares, generator, plusMinus > 0 ? "P" : "M", result);

                    fp = fopen(filename, "w");
                    mpz_out_str(fp, 10, number);
                    fprintf(fp, "\n");
                    mpz_out_str(fp, 10, numberSquared);
                    fprintf(fp, "\n");
                    fprintf(fp, "1 %d 1 | %d 1 %d | 1 %d 1\n", s2PerfectSquare, s4PerfectSquare, s6PerfectSquare, s8PerfectSquare);

                    // The magic square
                    mpz_out_str(fp, 10, x1);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, x2);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, x3);
                    fprintf(fp, " | ");
                    mpz_out_str(fp, 10, a1);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a2);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a3);
                    fprintf(fp, " | ");
                    mpz_out_str(fp, 10, a7);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a8);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a9);
                    fprintf(fp, "\n");

                    fclose(fp);

                    printf("%s\n", filename);
                }

                /// ///
                /// Check if d and e are distances in any arithmetic progression.
                /// ///
                int dFound = 0;
                int eFound = 0;
                AP3 = arithmeticProgressions;
                while (AP3 != NULL)
                {
                    if ( mpz_cmp(d, AP3->d) == 0 )
                    {
                        dFound = 1;
                    }

                    if ( mpz_cmp(e, AP3->d) == 0 )
                    {
                        eFound = 1;
                    }

                    AP3 = AP3->next;
                }

                if ( dFound > 0 && eFound > 0 )
                {
                    // /
                    // HEUREKA
                    // /
                    FILE *fp;
                    char filename[100];
                    char generator[80];

                    result ++;
                    mpz_get_str(generator, 10, input);

                    sprintf(filename, "fh,%d,%s%s,%ld.result", nrPerfectSquares, generator, plusMinus > 0 ? "P" : "M", result);

                    fp = fopen(filename, "w");
                    mpz_out_str(fp, 10, number);
                    fprintf(fp, "\n");
                    mpz_out_str(fp, 10, numberSquared);
                    fprintf(fp, "\n");
                    fprintf(fp, "1 %d 1 | %d 1 %d | 1 %d 1\n", s2PerfectSquare, s4PerfectSquare, s6PerfectSquare, s8PerfectSquare);

                    // The magic square
                    mpz_out_str(fp, 10, x1);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, x2);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, x3);
                    fprintf(fp, " | ");
                    mpz_out_str(fp, 10, a1);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a2);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a3);
                    fprintf(fp, " | ");
                    mpz_out_str(fp, 10, a7);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a8);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a9);
                    fprintf(fp, "\n");

                    fclose(fp);

                    printf("%s\n", filename);
                }
                else if ( dFound > 0 )
                {
                    // /
                    // SEMI-HEUREKA 1
                    // /
                    FILE *fp;
                    char filename[100];
                    char generator[80];

                    result ++;
                    mpz_get_str(generator, 10, input);

                    sprintf(filename, "sh1,%d,%s%s,%ld.result", nrPerfectSquares, generator, plusMinus > 0 ? "P" : "M", result);

                    fp = fopen(filename, "w");
                    mpz_out_str(fp, 10, number);
                    fprintf(fp, "\n");
                    mpz_out_str(fp, 10, numberSquared);
                    fprintf(fp, "\n");
                    fprintf(fp, "1 %d 1 | %d 1 %d | 1 %d 1\n", s2PerfectSquare, s4PerfectSquare, s6PerfectSquare, s8PerfectSquare);

                    // The magic square
                    mpz_out_str(fp, 10, x1);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, x2);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, x3);
                    fprintf(fp, " | ");
                    mpz_out_str(fp, 10, a1);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a2);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a3);
                    fprintf(fp, " | ");
                    mpz_out_str(fp, 10, a7);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a8);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a9);
                    fprintf(fp, "\n");

                    fclose(fp);

                    printf("%s\n", filename);
                }
                else if ( eFound > 0 )
                {
                    // /
                    // SEMI-HEUREKA 2
                    // /
                    FILE *fp;
                    char filename[100];
                    char generator[80];

                    result ++;
                    mpz_get_str(generator, 10, input);

                    sprintf(filename, "sh2,%d,%s%s,%ld.result", nrPerfectSquares, generator, plusMinus > 0 ? "P" : "M", result);

                    fp = fopen(filename, "w");
                    mpz_out_str(fp, 10, number);
                    fprintf(fp, "\n");
                    mpz_out_str(fp, 10, numberSquared);
                    fprintf(fp, "\n");
                    fprintf(fp, "1 %d 1 | %d 1 %d | 1 %d 1\n", s2PerfectSquare, s4PerfectSquare, s6PerfectSquare, s8PerfectSquare);

                    // The magic square
                    mpz_out_str(fp, 10, x1);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, x2);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, x3);
                    fprintf(fp, " | ");
                    mpz_out_str(fp, 10, a1);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a2);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a3);
                    fprintf(fp, " | ");
                    mpz_out_str(fp, 10, a7);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a8);
                    fprintf(fp, " ");
                    mpz_out_str(fp, 10, a9);
                    fprintf(fp, "\n");

                    fclose(fp);

                    printf("%s\n", filename);
                }

                AP2 = AP2->next;
            }

            AP1 = AP1->next;
        }

        // Just in case: Clean the arithmetic progression list.
        mpz_ap_list_clean(&arithmeticProgressions);

        // Just in case: Clean the factor pairs list.
        mpz_factor_list_clean(&factorPairs);
        factorPairsLength = 0;

        printf("_\n");
        fflush(stdout);
    }


    return 0;
}
