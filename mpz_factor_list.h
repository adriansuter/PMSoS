#ifndef MPZ_FACTOR_LIST_H_INCLUDED
#define MPZ_FACTOR_LIST_H_INCLUDED

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
void mpz_factor_list_push(mpz_factor_list_t ** list, mpz_t factor1, mpz_t factor2);


/** \brief Pops the first factor pair from the given list.
 *
 * The function would remove the first factor pair and release all memory used
 * by that element.
 *
 * \param list mpz_factor_list_t** The list.
 * \return void
 *
 */
void mpz_factor_list_pop(mpz_factor_list_t ** list);


/** \brief Cleans a given list.
 *
 * The function would clean the whole list and release all memory used by all
 * the elements previously in the list.
 *
 * \param list mpz_factor_list_t** The list.
 * \return void
 *
 */
void mpz_factor_list_clean(mpz_factor_list_t ** list);


/** \brief Prints the given list of factor pairs to the stdout.
 *
 * \param list mpz_factor_list_t* The list.
 * \return void
 */
void mpz_factor_list_print(mpz_factor_list_t * list);


#endif // MPZ_FACTOR_LIST_H_INCLUDED
