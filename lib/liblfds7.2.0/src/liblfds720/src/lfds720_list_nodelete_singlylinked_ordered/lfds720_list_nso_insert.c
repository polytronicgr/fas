/***** includes *****/
#include "lfds720_list_nso_internal.h"





/****************************************************************************/
enum lfds720_list_nso_insert_result lfds720_list_nso_insert( struct lfds720_list_nso_state *lasos,
                                                             struct lfds720_list_nso_element *lasoe,
                                                             struct lfds720_list_nso_element **existing_lasoe )
{
  char unsigned 
    result;

  enum lfds720_misc_flag
    finished_flag = LFDS720_MISC_FLAG_LOWERED;

  int
    compare_result = 0;

  lfds720_pal_uint_t
    backoff_iteration = LFDS720_BACKOFF_INITIAL_VALUE;

  struct lfds720_list_nso_element
    *volatile lasoe_temp = NULL,
    *volatile lasoe_trailing;

  LFDS720_PAL_ASSERT( lasos != NULL );
  LFDS720_PAL_ASSERT( lasoe != NULL );
  LFDS720_PAL_ASSERT( (lfds720_pal_uint_t) &lasoe->next % LFDS720_PAL_SINGLE_POINTER_LENGTH_IN_BYTES == 0 );
  LFDS720_PAL_ASSERT( (lfds720_pal_uint_t) &lasoe->value % LFDS720_PAL_SINGLE_POINTER_LENGTH_IN_BYTES == 0 );
  // TRD : existing_lasoe can be NULL

  /* TRD : imagine a list, sorted small to large

           we arrive at an element
           we obtain its next pointer
           we check we are greater than the current element and smaller than the next element
           this means we have found the correct location to insert
           we try to CAS ourselves in; in the meantime,
           someone else has *aready* swapped in an element which is smaller than we are

           e.g.

           the list is { 1, 10 } and we are the value 5

           we arrive at 1; we check the next element and see it is 10
           so we are larger than the current element and smaller than the next
           we are in the correct location to insert and we go to insert...

           in the meantime, someone else with the value 3 comes along
           he too finds this is the correct location and inserts before we do
           the list is now { 1, 3, 10 } and we are trying to insert now after
           1 and before 3!

           our insert CAS fails, because the next pointer of 1 has changed aready;
           but we see we are in the wrong location - we need to move forward an
           element
  */

  LFDS720_MISC_BARRIER_LOAD;

  /* TRD : we need to begin with the leading dummy element
           as the element to be inserted
           may be smaller than all elements in the list
  */

  lasoe_trailing = lasos->start;
  lasoe_temp = lasos->start->next;

  while( finished_flag == LFDS720_MISC_FLAG_LOWERED )
  {
    if( lasoe_temp == NULL )
      compare_result = -1;

    if( lasoe_temp != NULL )
    {
      LFDS720_MISC_BARRIER_LOAD;
      compare_result = lasos->key_compare_function( lasoe->key, lasoe_temp->key );
    }

    if( compare_result == 0 )
    {
      if( existing_lasoe != NULL )
        *existing_lasoe = lasoe_temp;

      switch( lasos->existing_key )
      {
        case LFDS720_LIST_NSO_EXISTING_KEY_OVERWRITE:
          LFDS720_LIST_NSO_SET_VALUE_IN_ELEMENT( *lasoe_temp, lasoe->value );
          return LFDS720_LIST_NSO_INSERT_RESULT_SUCCESS_OVERWRITE;
        break;

        case LFDS720_LIST_NSO_EXISTING_KEY_FAIL:
          return LFDS720_LIST_NSO_INSERT_RESULT_FAILURE_EXISTING_KEY;
        break;
      }

      finished_flag = LFDS720_MISC_FLAG_RAISED;
    }

    if( compare_result < 0 )
    {
      lasoe->next = lasoe_temp;
      LFDS720_MISC_BARRIER_STORE;
      LFDS720_PAL_ATOMIC_CAS( lasoe_trailing->next, lasoe->next, lasoe, LFDS720_MISC_CAS_STRENGTH_WEAK, result );

      if( result == 1 )
        finished_flag = LFDS720_MISC_FLAG_RAISED;
      else
      {
        LFDS720_BACKOFF_EXPONENTIAL_BACKOFF( lasos->insert_backoff, backoff_iteration );
        // TRD : if we fail to link, someone else has linked and so we need to redetermine our position is correct
        lasoe_temp = lasoe_trailing->next;
      }
    }

    if( compare_result > 0 )
    {
      // TRD : move trailing along by one element
      lasoe_trailing = lasoe_trailing->next;

      /* TRD : set temp as the element after trailing
               if the new element we're linking is larger than all elements in the list,
               lasoe_temp will now go to NULL and we'll link at the end
      */
      lasoe_temp = lasoe_trailing->next;
    }
  }

  LFDS720_BACKOFF_AUTOTUNE( lasos->insert_backoff, backoff_iteration );

  return LFDS720_LIST_NSO_INSERT_RESULT_SUCCESS;
}

