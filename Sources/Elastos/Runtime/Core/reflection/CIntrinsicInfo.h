//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CINTRINSICINFO_H__
#define __CINTRINSICINFO_H__

#include <elastos.h>
#include <clsdef.h>

_ELASTOS_NAMESPACE_USING

class CIntrinsicInfo
    : public ElLightRefBase
    , public IDataTypeInfo
{
public:
    CIntrinsicInfo(
        /* [in] */ CarDataType dataType,
        /* [in] */ UInt32 uSize);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetSize(
        /* [out] */ MemorySize * pSize);

    CARAPI GetDataType(
        /* [out] */ CarDataType * pDataType);

private:
    CarDataType m_dataType;
    UInt32      m_uSize;
};

#endif // __CINTRINSICINFO_H__
