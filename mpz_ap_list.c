#include "mpz_ap_list.h"


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
