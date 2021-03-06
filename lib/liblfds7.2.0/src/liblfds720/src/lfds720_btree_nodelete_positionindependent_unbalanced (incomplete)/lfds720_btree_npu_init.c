/***** includes *****/
#include "lfds720_btree_npu_internal.h"





/****************************************************************************/
void lfds720_btree_npu_init_valid_on_current_logical_core( struct lfds720_btree_npu_state *baus,
                                                           int (*key_compare_function)(void const *new_key, void const *existing_key),
                                                           enum lfds720_btree_npu_existing_key existing_key,
                                                           void *user_state )
{
  LFDS720_PAL_ASSERT( baus != NULL );
  LFDS720_PAL_ASSERT( (lfds720_pal_uint_t) &baus->root % LFDS720_PAL_SINGLE_POINTER_LENGTH_IN_BYTES == 0 );
  LFDS720_PAL_ASSERT( key_compare_function != NULL );
  // TRD : existing_key can be any value in its range
  // TRD : user_state can be NULL

  baus->root = 0;
  baus->key_compare_function = key_compare_function;
  baus->existing_key = existing_key;
  baus->user_state = user_state;

  lfds720_misc_internal_backoff_init( &baus->insert_backoff );

  LFDS720_MISC_BARRIER_STORE;

  lfds720_misc_force_store();

  return;
}

