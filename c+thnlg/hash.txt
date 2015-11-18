DWORD
Hash(DWORD dwId) const
{
    BYTE    byByte = 0;
    DWORD   dwHval = 0;  // hash value
    DWORD   dwG = 0;     // intermediate hash value

    INT iCnt = 3;

    for (iCnt = 3; iCnt >= 0; iCnt--) {
        byByte = static_cast<BYTE>((dwId >> 8*iCnt) & 0x000000FF);
        dwHval = (dwHval << 4) + byByte;

        dwG = dwHval & 0xF0000000;
        if (0 != dwG) {
            dwHval = dwHval ^ (dwG >> 24);
            dwHval = dwHval ^ dwG;
        }
    }

    return (m_wSize == 0)? 0:(dwHval % m_wSize);
}

HashBucket *
LookupBucket(DWORD dwId) const
{
    HashBucket *pcBp = NULL;

    for (pcBp = m_ppcArray[Hash(dwId)]; NULL != pcBp; pcBp = pcBp->m_pcNext) {
        if (NULL != pcBp) {
            if (pcBp->m_dwId == dwId) {
                return pcBp;      // Found a match!
            }
        }
    }

    RETURN_NULL;
}

CL_BOOL
Insert(DWORD dwId, const TRF_SUB_Event & rcItem)
{
    if ((m_wMaxNrOfItems > 0) && (m_wCount == m_wMaxNrOfItems)) {
        return CL_FALSE;
    }

    DWORD dwChain = Hash(dwId);     //计算哈希数值

    HashBucket *pcBucket = LookupBucket(dwId);   //查看是否已存在

    if (NULL != pcBucket) {
        pcBucket->m_cItem = rcItem;
    }
    else {
        HashBucket *pcNewBucket = NULL;

        if (NULL != m_pcEmptyList) {
            pcNewBucket = m_pcEmptyList;
            m_pcEmptyList = m_pcEmptyList->m_pcNext;
        }
        else {
            pcNewBucket = new(static_cast<TRF_DS_TMCEventHashTable::HashBucket*>(TRF_DS_TMCEventHashTable::HashBucket::AllocBuffer())) HashBucket();
            if (NULL == pcNewBucket) {
                return CL_FALSE;
            }
        }
        pcNewBucket->m_dwId = dwId;
        pcNewBucket->m_cItem = rcItem;
        pcNewBucket->m_pcNext = m_ppcArray[dwChain];
        m_ppcArray[dwChain] = pcNewBucket;
        m_wCount++;
    }

    return CL_TRUE;
}

VOID
TRF_DS_TMCEventHashTable::Remove(DWORD dwId)
{
    DWORD dwChain = Hash(dwId);
    HashBucket *pcBucket = LookupBucket(dwId);

    if (NULL == pcBucket) {
        return;
    }

    if (m_ppcArray[dwChain] == pcBucket) {
        m_ppcArray[dwChain] = pcBucket->m_pcNext;
        pcBucket->m_pcNext = m_pcEmptyList;
        m_pcEmptyList = pcBucket;
    }
    else {
        /* or elsewhere...
         * At this point, back pointers would really be useful
         * in removing an item.  ASSUMPTION:  remove will be
         * an infrequent operation, so the space/time tradeoff
         * of scanning the whole chain -vs- another pointer field
         * falls to the side of more time and less space.
         *
         * We know that the bucket MUST be on this chain.
         * because Hash(symbol) pointed here  AND
         *        LookupBucket(symbol) suceeded
         */
        HashBucket *pcTmp = m_ppcArray[dwChain];

        /*   +-----+   +------+   +------+   +------+
         *   |     |-->|      |-->|      |-->|      |-->@
         *   +-----+   +------+   +------+   +------+
         *     tmp                  bucket
         */
        while ((NULL != pcTmp->m_pcNext) && (pcTmp->m_pcNext != pcBucket)) {
            pcTmp = pcTmp->m_pcNext;  // step thru till found
        }

        if (NULL == pcTmp->m_pcNext) {
            return;
        }

        /*   +-----+   +------+   +------+   +------+
         *   |     |-->|      |-->|      |-->|      |-->@
         *   +-----+   +------+   +------+   +------+
         *               tmp        bucket
         */
        pcTmp->m_pcNext = pcTmp->m_pcNext->m_pcNext;

        /*                      /----------\
         *   +-----+   +------+ | +------+ | +------+
         *   |     |-->|      |-/ |      | \>|      |-->@
         *   +-----+   +------+   +------+   +------+
         *               tmp        bucket
         */
        pcBucket->m_pcNext = m_pcEmptyList;
        m_pcEmptyList = pcBucket;
    }

    m_wCount--;
}
