//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CLOCALTYPEINFO_H__
#define __CLOCALTYPEINFO_H__

#include "CClsModule.h"

class CLocalTypeInfo
    : public ElLightRefBase
    , public IDataTypeInfo
{
public:
    CLocalTypeInfo(
        /* [in] */ MemorySize size);

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
    MemorySize      m_size;
};

#endif // __CLOCALTYPEINFO_H__
