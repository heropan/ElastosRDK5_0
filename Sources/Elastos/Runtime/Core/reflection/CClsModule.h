//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CCLSMODULE_H__
#define __CCLSMODULE_H__

#include "refutil.h"

class CClsModule : public ElLightRefBase
{
public:
    CClsModule(
        /* [in] */ CLSModule *pClsMod,
        /* [in] */ Boolean bAllocedClsMod,
        /* [in] */ const String& path,
        /* [in] */ Void *pIModule);

    ~CClsModule();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetModuleInfo(
        /* [out] */ IModuleInfo ** ppModuleInfo);

    CARAPI InitOrgType();

    CARAPI AliasToOriginal(
        /* [in] */ TypeDescriptor *pTypeDype,
        /* [out] */ TypeDescriptor **ppOrgTypeDesc);

public:
    CLSModule*      m_pClsMod;
    Boolean         m_bAllocedClsMode;
    Int32           m_nBase;

private:
    TypeAliasDesc  *m_pTypeAliasList;
    String          m_sbPath;
    Void           *m_pIModule;
};

#endif // __CCLSMODULE_H__
