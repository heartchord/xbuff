#pragma once

#include "xzero.h"
#include <memory>

class IKG_Buffer : private xzero::KG_UnCopyable
{
public:
    IKG_Buffer() {}
    virtual ~IKG_Buffer() {}

public:
    virtual unsigned int ResSize() = 0;
    virtual unsigned int OriSize() = 0;
    virtual unsigned int CurSize() = 0;

    virtual int SetSize(unsigned int uNewSize) = 0;
    virtual int ResetSize()                    = 0;

    virtual void * ResBuf() const = 0;
    virtual void * Buf()    const = 0;
};

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

template <class MP>
PIKG_Buffer KG_GetBuffFromMemoryPool(MP &mp, unsigned int uRequiredSize, unsigned int uReservedSize = 0)
{
    KG_Buffer *  pResult   = NULL;
    unsigned int uBuffSize = 0;
    void      *  pvBuff    = NULL;
    void      *  pvData    = NULL;

    KG_PROCESS_ERROR(uRequiredSize > 0 && "It seems that uRequiredSize is 0!");

    uBuffSize = sizeof(KG_Buffer) + uRequiredSize + uReservedSize;
    pvBuff    = mp.Get(uBuffSize);
    KG_PROCESS_PTR_ERROR(pvBuff);

    pvData  = (void *)(((char *)pvBuff) + sizeof(KG_Buffer));
    pResult = ::new(pvBuff)KG_Buffer(uRequiredSize, uReservedSize, pvData);         // placement operator new

Exit0:
    return pBuffer;
}

template <class MP>
SPIKG_Buffer KG_GetSharedBuffFromMemoryPool(MP &mp, unsigned int uRequiredSize, unsigned int uReservedSize = 0)
{
    PIKG_Buffer pBuff = KG_GetBuffFromMemoryPool(mp, uRequiredSize, uReservedSize);
    return SPIKG_Buffer(pBuff);
}
