//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CCONSTRUCTORINFO_H__
#define __CCONSTRUCTORINFO_H__

#include "CMethodInfo.h"

class CConstructorInfo
    : public ElLightRefBase
    , public IConstructorInfo
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Init(
        /* [in] */ CClsModule * pCClsModule,
        /* [in] */ MethodDescriptor *pMethodDescriptor,
        /* [in] */ UInt32 uIndex,
        /* [in] */ ClassID *pClsId);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetParamCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllParamInfos(
        /* [out] */ ArrayOf<IParamInfo *> * pParamInfos);

    CARAPI GetParamInfoByIndex(
        /* [in] */ Int32 index,
        /* [out] */ IParamInfo ** ppParamInfo);

    CARAPI GetParamInfoByName(
        /* [in] */ CString name,
        /* [out] */ IParamInfo ** ppParamInfo);

    CARAPI CreateArgumentList(
        /* [out] */ IArgumentList ** ppArgumentList);

    CARAPI CreateObject(
        /* [in] */ IArgumentList * pArgumentList,
        /* [out] */ PInterface * pObject);

    CARAPI CreateObjectInRegime(
        /* [in] */ PRegime rgm,
        /* [in] */ IArgumentList * pArgumentList,
        /* [out] */ PInterface * pObject);

    CARAPI AcquireParamList();

    CARAPI CreateObjInRgm(
        /* [in] */ PRegime rgm,
        /* [in] */ IArgumentList * pArgumentList,
        /* [out] */ PInterface * pObject);

public:
    AutoPtr<CMethodInfo> m_pMethodInfo;

    char                 m_szUrn2[_MAX_PATH];
    ClassID              m_instClsId;

private:
    Int32                m_nOutParamIndex;

    char                 m_szUrn[_MAX_PATH];
    ClassID              m_clsId;
};

#endif // __CCONSTRUCTORINFO_H__
