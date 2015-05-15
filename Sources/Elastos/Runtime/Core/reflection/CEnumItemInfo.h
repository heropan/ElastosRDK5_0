//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CENUMITEMINFO_H__
#define __CENUMITEMINFO_H__

#include "refutil.h"

class CEnumItemInfo : public IEnumItemInfo
{
public:
    CEnumItemInfo(
        /* [in] */ IEnumInfo * pEnumInfo,
        /* [in] */ const String& name,
        /* [in] */ Int32 value);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetEnumInfo(
        /* [out] */ IEnumInfo ** ppEnumInfo);

    CARAPI GetValue(
        /* [out] */ Int32 * pValue);

private:
    IEnumInfo *m_pEnumInfo;
    String     m_sName;
    Int32      m_iValue;
};

#endif // __CENUMITEMINFO_H__
