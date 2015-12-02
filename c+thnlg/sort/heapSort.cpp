#ifndef CXX_TRF_SUB_SORTABLEARRAY_H
#   include "TRF_COMM_SortableArray.h"
#endif

TRF_SUB_SortableArray::TRF_SUB_SortableArray()
{
}

TRF_SUB_SortableArray::~TRF_SUB_SortableArray()
{
}

/// Sort the array
VOID
TRF_SUB_SortableArray::Sort(VOID)
{
    WORD wSize = GetSize();

    BuildHeap(wSize);

    while (wSize > 1)
    {
        wSize--;
        Swap(0, wSize);
        DownHeap(wSize, 0);
    }
}

// Build the heap
VOID
TRF_SUB_SortableArray::BuildHeap(WORD wSize)
{
    LONG lRoot = static_cast<LONG>(wSize) / 2 - 1;

    // The second test (root < size) is to prevent underflow
    for (; lRoot >= 0; --lRoot)
    {
        DownHeap(wSize, static_cast<WORD>(lRoot));
    }
}

// Go down the heap
VOID
TRF_SUB_SortableArray::DownHeap(WORD wSize, WORD wRoot)
{
    WORD wChild = static_cast<WORD>(2 * wRoot + 1);

    while (wChild < wSize)
    {
        if ((wChild+1) < wSize)
        {
            if (TRF_SUB_COMPARE_GREATER == Compare(wChild+1, wChild))
            {
                ++wChild;
            }
        }

        if (TRF_SUB_COMPARE_GREATER == Compare(wRoot,wChild))
        {
            return;
        }

        Swap(wRoot, wChild);
        wRoot = wChild;
        wChild = 2 * wRoot + 1;
    }
}
/* EOF */
