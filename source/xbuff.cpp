#include "xbuff.h"

inline KG_Buffer::KG_Buffer(unsigned int uOriSize, unsigned int uResSize, void *pvBuff) : 
    m_uOriSize(uOriSize), m_uCurSize(uOriSize), m_uResSize(uResSize), m_pvBuff(pvBuff)
{
    KG_ASSERT(m_uOriSize > 0);
    KG_ASSERT(m_uCurSize > 0);
    KG_ASSERT(NULL != m_pvBuff);
}

inline KG_Buffer::~KG_Buffer()
{
}

inline unsigned int KG_Buffer::ResSize()
{
    return m_uResSize;
}

inline unsigned int KG_Buffer::OriSize()
{
    return m_uOriSize;
}

inline unsigned int KG_Buffer::CurSize()
{
    return m_uCurSize;
}

inline int KG_Buffer::SetSize(unsigned int uNewSize)
{
    int nResult = false;

    KG_PROCESS_ERROR(uNewSize > 0);
    KG_PROCESS_ERROR(uNewSize <= m_uOriSize && "It seems user wants to set a bigger size than original buffer size!");

    m_uCurSize = uNewSize;

    nResult = true;
Exit0:
    return nResult;
}

inline int KG_Buffer::ResetSize()
{
    m_uCurSize = m_uOriSize;
    return true;
}

inline void * KG_Buffer::ResBuf() const
{
    if (m_uResSize <= 0)
    {
        return NULL;
    }

    return m_pvBuff;
}

inline void * KG_Buffer::Buf() const
{
    if (m_uCurSize <= 0)
    {
        return NULL;
    }

    return (void *)((char *)m_pvBuff + m_uResSize);
}
