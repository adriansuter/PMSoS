#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include "mpz_factor_list.h"
#include "mpz_ap_list.h"


/** \brief Calculate arithmetic progressions via Pythagorean triples.
 *
 * To avoid the initialization of mpz_t variables within the function, you have
 * to pass initialized mpz_t variables via parameters that then can be used
 * by the function.
 *
 * Note that the middle value of any arithmetic progression found would be
 * (p1 * p2)^2.
 *
 * \param arithmeticProgressions mpz_ap_list_t** The list of arithmetic progressions.
 * \param p1 mpz_t The first factor.
 * \param p2 mpz_t The second factor.
 * \param m mpz_t An mpz_t variable that can be used by the function.
 * \param n mpz_t An mpz_t variable that can be used by the function.
 * \param mSquared mpz_t An mpz_t variable that can be used by the function.
 * \param nSquared mpz_t An mpz_t variable that can be used by the function.
 * \param x1 mpz_t An mpz_t variable that can be used by the function.
 * \param x2 mpz_t An mpz_t variable that can be used by the function.
 * \param x3 mpz_t An mpz_t variable that can be used by the function.
 * \param a1 mpz_t An mpz_t variable that can be used by the function.
 * \param a2 mpz_t An mpz_t variable that can be used by the function.
 * \param a3 mpz_t An mpz_t variable that can be used by the function.
 * \return void
 */
void calc(mpz_ap_list_t ** arithmeticProgressions, mpz_t p1, mpz_t p2, mpz_t m, mpz_t n, mpz_t mSquared, mpz_t nSquared, mpz_t x1, mpz_t x2, mpz_t x3, mpz_t a1, mpz_t a2, mpz_t a3)
{
    int cmp;

    // m = floor(sqrt(p1))
    mpz_sqrt(m, p1);

    // n = 1
    mpz_set_ui(n, 1);

    // mSquared = m^2
    mpz_pow_ui(mSquared, m, 2);

    // nSquared = n^2
    mpz_pow_ui(nSquared, n, 2);

    while ( mpz_cmp(m, n) > 0 )
    {
        // x3 = m^2 + n^2
        mpz_add(x3, mSquared, nSquared);

        cmp = mpz_cmp(x3, p1);
        if ( cmp < 0 )// if ( x3 < p1 )
        {
            // n = n + 1
            mpz_add_ui(n, n, 1);

            // nSquared = n^2
            mpz_pow_ui(nSquared, n, 2);
        }
        else if ( cmp > 0 )// if ( x3 > p1 )
        {
            // m = m - 1
            mpz_sub_ui(m, m, 1);

            // mSquared = m^2
            mpz_pow_ui(mSquared, m, 2);
        }
        else// if ( x3 == p1 )
        {
            // x1 = m^2 - n^2
            mpz_sub(x1, mSquared, nSquared);

            // x2 = 2 * m * n
            mpz_mul(x2, m, n);
            mpz_mul_ui(x2, x2, 2);

            // Scale x1, x2 and x3 by p2.
            mpz_mul(x1, x1, p2);
            mpz_mul(x2, x2, p2);
            mpz_mul(x3, x3, p2);

            // a1 = (x2 - x1)^2
            mpz_sub(a1, x2, x1);
            mpz_pow_ui(a1, a1, 2);

            // a2 = x3^2
            mpz_pow_ui(a2, x3, 2);

            // a3 = (x1 + x2)^2
            mpz_add(a3, x1, x2);
            mpz_pow_ui(a3, a3, 2);

            // Insert the arithmetic progression [a1, a2, a3] into the list.
            mpz_ap_list_insert(arithmeticProgressions, a1, a2, a3);

            // m = m - 1
            mpz_sub_ui(m, m, 1);

            // mSquared = m^2
            mpz_pow_ui(mSquared, m, 2);
        }
    }
}


/** \brief The main function.
 *
 * \param argc int Number of command line arguments given.
 * \param argv char** Array of command line arguments given.
 * \return int
 */
int main(int argc, char **argv)
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
    long result;
    char buf[BUFSIZ];

    while ( 1 )
    {
        // Read from the input.
        read = 1;
        while ( read == 1 )
        {
            fgets(buf, sizeof buf, stdin);

            // Measure the input string length.
            strlength = 0;
            while (buf[strlength] != '\0')
            {
                strlength++;
            }

            if (strlength > 0 && buf[strlength - 1] == '\n')
            {
                if ( buf[0] == 'q' )
                {
                    // Quit command read > so exit.
                    exit(0);
                }
                else
                {
                    if ( buf[strlength - 2] == '+' )
                    {
                        // The given command ends by a '+'.
                        plusMinus = 1;
                        buf[strlength - 2] = '\n';
                    }
                    else if ( buf[strlength - 2] == '-' )
                    {
                        // The given command ends by a '-'.
                        plusMinus = -1;
                        buf[strlength - 2] = '\n';
                    }
                    else
                    {
                        // The given command does not end by '+' nor by '-'.
                        // In this case we default to '+'.
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


        result = 0;

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

                // a = "arithmetic progression distance of AP1"
                mpz_set(a, AP1->d);

                // b = "arithmetic progression distance of AP2"
                mpz_set(b, AP2->d);

                // c = "middle square number of AP1"
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


                ///
                /// Now we can actually calculate the nine numbers of the
                /// magic square and then check whether or not they are perfect
                /// square numbers.
                ///

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

                // s1, s3, s5, s7 and s9 are perfect square numbers (by construction).
                // Therefore we would have at least 5 perfect square numbers.
                // Calculate the total number of perfect square numbers in our
                // magic square.
                int nrPerfectSquares = 5
                                       + s2PerfectSquare
                                       + s4PerfectSquare
                                       + s6PerfectSquare
                                       + s8PerfectSquare;

                if ( nrPerfectSquares > 6 )
                {
                    /// ///
                    /// We have found a magic square of more than 6 perfect
                    /// square numbers. So write that down to disk.
                    /// We would also write the filename to the stdout.
                    /// ///
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
                /// Check if d and e are distances in any other arithmetic progression.
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
                    /// ///
                    /// HEUREKA
                    /// ///
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
                    /// ///
                    /// SEMI-HEUREKA 1
                    /// ///
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
                    /// ///
                    /// SEMI-HEUREKA 2
                    /// ///
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
