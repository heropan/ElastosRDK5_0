/** @addtogroup SystemRef
  *   @{
  */

//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __ELASYSAPI_H__
#define __ELASYSAPI_H__

#include <elapi.h>

_ELASTOS_NAMESPACE_BEGIN

/** @} */

/** @addtogroup CARRef
  *   @{
  */
class CReflector
{
public:
    STATIC CARAPI AcquireModuleInfo(
        /* [in] */ const String& name,
        /* [out] */ IModuleInfo **ppModuleInfo)
    {
        return _CReflector_AcquireModuleInfo(name, ppModuleInfo);
    }

    STATIC CARAPI AcquireIntrinsicTypeInfo(
        /* [in] */ CarDataType intrinsicType,
        /* [out] */ IDataTypeInfo **ppIntrinsicTypeInfo)
    {
        return _CReflector_AcquireIntrinsicTypeInfo(intrinsicType,
            ppIntrinsicTypeInfo);
    }

    STATIC CARAPI AcquireEnumInfo(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<String>* itemNames,
        /* [in] */ ArrayOf<Int32>* itemValues,
        /* [out] */ IEnumInfo **ppEnumInfo)
    {
        return _CReflector_AcquireEnumInfo(name, itemNames, itemValues,
            ppEnumInfo);
    }

    STATIC CARAPI AcquireCppVectorInfo(
        /* [in] */ IDataTypeInfo *pElementTypeInfo,
        /* [in] */ Int32 length,
        /* [out] */ ICppVectorInfo **ppCppVectorInfo)
    {
        return _CReflector_AcquireCppVectorInfo(pElementTypeInfo, length,
            ppCppVectorInfo);
    }

    STATIC CARAPI AcquireStructInfo(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<String>* fieldNames,
        /* [in] */ ArrayOf<IDataTypeInfo *>* fieldTypeInfos,
        /* [out] */ IStructInfo **ppStructInfo)
    {
        return _CReflector_AcquireStructInfo(name, fieldNames,
            fieldTypeInfos, ppStructInfo);
    }

    STATIC CARAPI AcquireCarArrayInfo(
        /* [in] */ CarDataType quintetType,
        /* [in] */ IDataTypeInfo *pElementTypeInfo,
        /* [out] */ ICarArrayInfo **ppCarArrayInfo)
    {
        return _CReflector_AcquireCarArrayInfo(quintetType, pElementTypeInfo,
            ppCarArrayInfo);
    }
};

class CObject
{
public:
    STATIC CARAPI_(Boolean) Compare(
        /* [in] */ PInterface pObjectA,
        /* [in] */ PInterface pObjectB)
    {
        return _CObject_Compare(pObjectA, pObjectB);
    }

    STATIC CARAPI EnterRegime(PInterface pObject, PRegime pRegime)
    {
        if (!pRegime) return E_NO_INTERFACE;
        return _CObject_EnterRegime(pObject, pRegime);
    }

    STATIC CARAPI LeaveRegime(PInterface pObject, PRegime pRegime)
    {
        if (!pRegime) return E_NO_INTERFACE;
        return _CObject_LeaveRegime(pObject, pRegime);
    }

    STATIC CARAPI CreateInstance(
        RClassID rclsid,
        PRegime pRegime,
        REIID riid,
        PInterface *ppObj)
    {
        return _CObject_CreateInstance(rclsid, pRegime, riid, ppObj);
    }

    STATIC CARAPI CreateInstanceEx(
        RClassID rclsid,
        PRegime pRegime,
        UInt32 cmq,
        PMULTIQI pResults)
    {
        return _CObject_CreateInstanceEx(rclsid, pRegime, cmq, pResults);
    }

    STATIC CARAPI AcquireClassFactory(
        RClassID rclsid,
        PRegime pRegime,
        REIID iid,
        PInterface *ppObject)
    {
        return _CObject_AcquireClassFactory(rclsid, pRegime, iid, ppObject);
    }

    STATIC CARAPI ReflectModuleInfo(
        PInterface pObj,
        IModuleInfo **piModuleInfo)
    {
        return _CObject_ReflectModuleInfo(pObj, piModuleInfo);
    }

    STATIC CARAPI ReflectClassInfo(
        PInterface pObj,
        IClassInfo **piClassInfo)
    {
        return _CObject_ReflectClassInfo(pObj, piClassInfo);
    }

    STATIC CARAPI ReflectInterfaceInfo(
        PInterface pObj,
        IInterfaceInfo **piInterfaceInfo)
    {
        return _CObject_ReflectInterfaceInfo(pObj, piInterfaceInfo);
    }
};

class CCallbackParcel
{
public:
    STATIC CARAPI New(
        /* [out] */ IParcel ** ppParcel)
    {
        return _CCallbackParcel_New(ppParcel);
    }
};

class CParcel
{
public:
    STATIC CARAPI New(
        /* [out] */ IParcel ** ppParcel)
    {
        return _CParcel_New(ppParcel);
    }
};

_ELASTOS_NAMESPACE_END

#endif // __ELASYSAPI_H__
/** @} */
