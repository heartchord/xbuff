#include "xbuff.h"

KG_NAMESPACE_BEGIN(xbuff)

KG_Buffer::KG_Buffer(UINT32 uOriSize, UINT32 uResSize, void *pvBuff) : 
    m_uOriSize(uOriSize), m_uCurSize(uOriSize), m_uResSize(uResSize), m_pvBuff(pvBuff)
{
    KG_ASSERT(m_uOriSize > 0);
    KG_ASSERT(m_uCurSize > 0);
    KG_ASSERT(NULL != m_pvBuff);
}

KG_Buffer::~KG_Buffer()
{
//#ifdef _DEBUG
//    printf("KG_Buffer::~KG_Buffer() is invoked.\n");
//#endif
}

UINT32 KG_Buffer::ResSize()
{
    return m_uResSize;
}

UINT32 KG_Buffer::OriSize()
{
    return m_uOriSize;
}

UINT32 KG_Buffer::CurSize()
{
    return m_uCurSize;
}

int KG_Buffer::SetSize(UINT32 uNewSize)
{
    int nResult = false;

    KG_PROCESS_ERROR(uNewSize > 0);
    KG_PROCESS_ERROR(uNewSize <= m_uOriSize && "It seems user wants to set a bigger size than original buffer size!");

    m_uCurSize = uNewSize;

    nResult = true;
Exit0:
    return nResult;
}

int KG_Buffer::ResetSize()
{
    m_uCurSize = m_uOriSize;
    return true;
}

void * KG_Buffer::ResBuf() const
{
    if (m_uResSize <= 0)
    {
        return NULL;
    }

    return m_pvBuff;
}

void * KG_Buffer::Buf() const
{
    if (m_uCurSize <= 0)
    {
        return NULL;
    }

    return (void *)((char *)m_pvBuff + m_uResSize);
}

KG_NAMESPACE_END
