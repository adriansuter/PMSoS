#include "mpz_factor_list.h"


/** \brief Push a factor pair to the given list.
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


/** \brief Pop the last factor pair from the list.
 *
 * \param list mpz_factor_list_t** The list.
 * \return void
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


/** \brief Clear the whole list.
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


/** \brief Print the contents of the list to the output.
 *
 * \param list mpz_factor_list_t* The list.
 * \return void
 *
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
