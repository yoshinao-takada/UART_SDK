#ifndef BLBSEARCH_H_
#define BLBSEARCH_H_
#include    "base/BLbase.h"
#include    "base/BLsv.h"
#ifdef __cplusplus
extern "C" {
#endif

/*!
\brief three value comarator function
\param pv0 [in]
\param pv1 [in]
\param param [in] parameter
\return *pv0 < *pv1 : -1, *pv0 == *pv1 : 0, *pv0 > *pv1 : 1
*/
typedef int (*BLcompare_t)(const void* pv0, const void* pv1, const void* param);

/*!
\brief check if the target is in-range
\param array [in] table sorted from smaller to larger; i.e. the 1st is the smallest.
\param nmemb [in] element count in the array
\param size [in] element size
\param comp [in] comparator function
\param target [in] search target
\param param [in] extra parameter
\param range [in] 
\return 1: array[range[0]] <= target < array[range[1]], 0: otherwise
*/
int BLbsearch_inrange(
    const void* array, size_t nmemb, size_t size, BLcompare_t comp,
    const void* target, const void* param, const BL2u32_t range);

/*!
\brief binary search
\param array [in] table sorted from smaller to larger; i.e. the 1st is the smallest.
\param nmemb [in] element count in the array
\param size [in] element size
\param comp [in] comparator function
\param target [in] search target
\param param [in] extra parameter
\param range [out] found range; target < array[0]: {0,0}, 
    target >= array[nmemb - 1]: {nmemb - 1, nmemb},
    otherwise : {i,i+1}, array[i] <= target < array[i+1]
*/
void BLbsearch_find(
    const void* array, size_t nmemb, size_t size, BLcompare_t comp,
    const void* target, const void* param, BL2u32_t range);

/*!
\brief insert a new element in the array.
\param array [in] table sorted from smaller to larger; i.e. the 1st is the smallest.
\param nmemb [in] element count in the array
\param size [in] element size
*/
int BLbsearch_insert(void* array, size_t nmemb, size_t size,
    const void* new_element, size_t insert_at);

/*!
\brief move a element outside the array.
\param array [in] table sorted from smaller to larger; i.e. the 1st is the smallest.
\param nmemb [in] element count in the array
\param size [in] element size
\param moved_element [in] buffer for an element moved from the target position.
\param to_remove [in] index pointing at the target element.
*/
int BLbsearch_remove(void* array, size_t nmemb, size_t size,
    void* moved_element, size_t to_remove);
#ifdef __cplusplus
}
#endif
#endif /* BLBSEARCH_H_ */