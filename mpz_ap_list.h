#ifndef MPZ_AP_LIST_H_INCLUDED
#define MPZ_AP_LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>


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
void mpz_ap_list_insert(mpz_ap_list_t ** list, mpz_t x, mpz_t y, mpz_t z);


/** \brief Pops the first arithmetic progression from the given list.
 *
 * The function would remove the first arithmetic progression and release all
 * memory used by that element.
 *
 * \param list mpz_ap_list_t** The list.
 * \return void
 */
void mpz_ap_list_pop(mpz_ap_list_t ** list);


/** \brief Cleans a given list.
 *
 * The function would clean the whole list and release all memory used by all
 * the elements previously in the list.
 *
 * \param list mpz_ap_list_t** The list.
 * \return void
 */
void mpz_ap_list_clean(mpz_ap_list_t ** list);


/** \brief Prints the given list of arithmetic progressions to the stdout.
 *
 * \param list mpz_ap_list_t* The list.
 * \return void
 */
void mpz_ap_list_print(mpz_ap_list_t * list);


#endif // MPZ_AP_LIST_H_INCLUDED
