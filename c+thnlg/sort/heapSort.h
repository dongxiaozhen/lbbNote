class TRF_SUB_SortableArray
{
public:
    /*-----------------------------------------------------------------------*/
    /**
     * Constructor
     *
     */
    /*-----------------------------------------------------------------------*/
    TRF_SUB_SortableArray();

    /*-----------------------------------------------------------------------*/
    /**
     * Destructor
     *
     */
    /*-----------------------------------------------------------------------*/
    virtual ~TRF_SUB_SortableArray();

    /**
     * Sort the array based on the Compare and Swap methods
     */
    virtual VOID Sort(VOID);

protected:

    // Get the size of the array
    virtual WORD GetSize(VOID) const = 0;

    // Compare two elements indicated by the given indexes
    virtual TRF_SUB_CompareResult Compare(WORD wIndex1, WORD wIndex2) = 0;

    // Swap two elements indicated by the given indexes
    virtual VOID Swap(WORD wIndex1, WORD wIndex2) = 0;

private:

    // Build the heap used in heap sort
    VOID BuildHeap(WORD wSize);

    // Go down the heap, used in heap sort
    VOID DownHeap(WORD wSize, WORD wRoot);
};