#pragma once

#include "xzero.h"
#include <memory>

KG_NAMESPACE_BEGIN(xbuff)

class IKG_Buffer : private xzero::KG_UnCopyable
{
public:
    IKG_Buffer();
    virtual ~IKG_Buffer();

public:
    virtual unsigned int ResSize() = 0;
    virtual unsigned int OriSize() = 0;
    virtual unsigned int CurSize() = 0;

    virtual int SetSize(unsigned int uNewSize) = 0;
    virtual int ResetSize()                    = 0;

    virtual void * ResBuf() const = 0;
    virtual void * Buf()    const = 0;
};

inline IKG_Buffer::IKG_Buffer()
{
}

inline IKG_Buffer::~IKG_Buffer()
{
//#ifdef _DEBUG
//    printf("IKG_Buffer::~IKG_Buffer() is invoked.\n");
//#endif
}

typedef IKG_Buffer *                PIKG_Buffer;
typedef std::shared_ptr<IKG_Buffer> SPIKG_Buffer;

class KG_Buffer : public IKG_Buffer
{
public:
    KG_Buffer(unsigned int uOriSize, unsigned int uResSize, void *pvBuff);
private:
    virtual ~KG_Buffer();

public:
    virtual unsigned int ResSize();
    virtual unsigned int OriSize();
    virtual unsigned int CurSize();

    virtual int SetSize(unsigned int uNewSize);
    virtual int ResetSize();

    virtual void * ResBuf() const;
    virtual void * Buf()    const;

private:
    unsigned int m_uResSize;
    unsigned int m_uOriSize;
    unsigned int m_uCurSize;
    void *       m_pvBuff;
};

typedef KG_Buffer *                PKG_Buffer;
typedef std::shared_ptr<KG_Buffer> SPKG_Buffer;

template <class MP>
PIKG_Buffer KG_GetBuffFromMemoryPool(MP *pMP, unsigned int uRequiredSize, unsigned int uReservedSize = 0)
{
    PIKG_Buffer  pResult   = NULL;
    int          nRetCode  = 0;
    unsigned int uBuffSize = 0;
    void *       pvBuff    = NULL;
    void *       pvData    = NULL;

    KG_PROCESS_PTR_ERROR(pMP);
    KG_PROCESS_ERROR(uRequiredSize > 0 && "uRequiredSize can't be 0!");

    uBuffSize = sizeof(KG_Buffer) + uRequiredSize + uReservedSize;
    nRetCode  = pMP->Get(&pvBuff, uBuffSize);
    KG_PROCESS_ERROR(nRetCode);
    KG_PROCESS_PTR_ERROR(pvBuff);

//#ifdef _DEBUG
//    printf("KG_GetBuffFromMemoryPool() pvBuff = %X\n", pvBuff);
//#endif

    pvData  = (void *)(((char *)pvBuff) + sizeof(KG_Buffer));
    pResult = ::new(pvBuff)KG_Buffer(uRequiredSize, uReservedSize, pvData);         // placement operator new

Exit0:
    return pResult;
}

template <class MP>
class KG_SharedBufferDeleter
{
public:
    KG_SharedBufferDeleter(MP *pMP);
    ~KG_SharedBufferDeleter();

public:
    void operator()(PIKG_Buffer piBuff);

private:
    MP *m_pMemoryPool;

};

template <class MP>
inline KG_SharedBufferDeleter<MP>::KG_SharedBufferDeleter(MP *pMemoryPool) : m_pMemoryPool(pMemoryPool)
{
    KG_ASSERT(NULL != m_pMemoryPool);
}

template <class MP>
inline KG_SharedBufferDeleter<MP>::~KG_SharedBufferDeleter()
{
}

template <class MP>
inline void KG_SharedBufferDeleter<MP>::operator()(PIKG_Buffer piBuff)
{
    int   nRetCode = false;
    void *pvBuff   = NULL;

    KG_PROCESS_PTR_ERROR(piBuff);
    KG_PROCESS_PTR_ERROR(m_pMemoryPool);

    pvBuff = (void *)piBuff;
    KG_PROCESS_PTR_ERROR(pvBuff);

    piBuff->~IKG_Buffer();                                              // this will invoke KG_Buffer::~KG_Buffer() automatically
    nRetCode = m_pMemoryPool->Put(&pvBuff);
    KG_PROCESS_ERROR(nRetCode);

Exit0:
    return;
}

template <class MP>
SPIKG_Buffer KG_GetSharedBuffFromMemoryPool(MP *pMP, unsigned int uRequiredSize, unsigned int uReservedSize = 0)
{
    PIKG_Buffer pBuff = KG_GetBuffFromMemoryPool(pMP, uRequiredSize, uReservedSize);
    return SPIKG_Buffer(pBuff, KG_SharedBufferDeleter<MP>(pMP));
}

KG_NAMESPACE_END