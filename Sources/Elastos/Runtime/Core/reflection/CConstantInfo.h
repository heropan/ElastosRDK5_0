//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CCONSTANTINFO_H__
#define __CCONSTANTINFO_H__

#include "CClsModule.h"

class CConstantInfo
    : public ElLightRefBase
    , public IConstantInfo
{
public:
    CConstantInfo(
        /* [in] */ CClsModule * pCClsModule,
        /* [in] */ ConstDirEntry * pConstDirEntry);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetValue(
        /* [out] */ Int32 * pValue);

    CARAPI GetModuleInfo(
        /* [out] */ IModuleInfo ** ppModuleInfo);

private:
    AutoPtr<CClsModule>  m_pCClsModule;
    ConstDirEntry       *m_pConstDirEntry;
};

#endif // __CCONSTANTINFO_H__
