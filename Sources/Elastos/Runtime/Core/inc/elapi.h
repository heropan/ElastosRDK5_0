/** @addtogroup SystemRef
  *   @{
  *
  * Elastos API is Elastos's core set of application programming interfaces (APIs).
  */

//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __ELAPI_H__
#define __ELAPI_H__

#include <elastos.h>

#ifdef _UNDEFDLLEXP
#include <elasys_server.h>
#else
#include <elasys.h>
#endif

#include <stdint.h>

/** @} */
/** @addtogroup CARRef
  *   @{
  */
#include <callback.h>

ELAPI _CReflector_AcquireModuleInfo(
    /* [in] */ const _ELASTOS String& name,
    /* [out] */ IModuleInfo **piModuleInfo);

ELAPI _CReflector_AcquireIntrinsicTypeInfo(
    /* [in] */ CarDataType intrinsicType,
    /* [out] */ IDataTypeInfo **ppIntrinsicTypeInfo);

ELAPI _CReflector_AcquireEnumInfo(
    /* [in] */ const _ELASTOS String& name,
    /* [in] */ _ELASTOS ArrayOf<_ELASTOS String> *pItemNames,
    /* [in] */ _ELASTOS ArrayOf<_ELASTOS Int32> *pItemValues,
    /* [out] */ IEnumInfo **ppEnumInfo);

ELAPI _CReflector_AcquireStructInfo(
    /* [in] */ const _ELASTOS String& name,
    /* [in] */ _ELASTOS ArrayOf<_ELASTOS String> *pFieldNames,
    /* [in] */ _ELASTOS ArrayOf<IDataTypeInfo *> *pFieldTypeInfos,
    /* [out] */ IStructInfo **ppStructInfo);

ELAPI _CReflector_AcquireCppVectorInfo(
    /* [in] */ IDataTypeInfo *pElementTypeInfo,
    /* [in] */ _ELASTOS Int32 length,
    /* [out] */ ICppVectorInfo **ppCppVectorInfo);


ELAPI _CReflector_AcquireCarArrayInfo(
    /* [in] */ CarDataType quintetType,
    /* [in] */ IDataTypeInfo *pElementTypeInfo,
    /* [out] */ ICarArrayInfo **ppCarArrayInfo);

ELAPI _CObject_ReflectModuleInfo(
    /* [in] */ PInterface pObj,
    /* [out] */ IModuleInfo **piModuleInfo);

ELAPI _CObject_ReflectClassInfo(
    /* [in] */ PInterface pObj,
    /* [out] */ IClassInfo **piClassInfo);

ELAPI _CObject_ReflectInterfaceInfo(
    /* [in] */ PInterface pObj,
    /* [out] */ IInterfaceInfo **piInterfaceInfo);

ELAPI_(_ELASTOS Boolean) _Impl_CheckHelperInfoFlag(
    /* [in] */ _ELASTOS UInt32 flag);

ELAPI_(void) _Impl_SetHelperInfoFlag(
    /* [in] */ _ELASTOS UInt32 flag,
    /* [in] */ _ELASTOS Boolean bValue);

ELAPI _Impl_EnterProtectedZone();

ELAPI _Impl_LeaveProtectedZone();

ELAPI _Impl_InsideProtectedZone();

// callback helper api for making parameters
ELAPI _Impl_CheckClsId(
    /* [in] */ PInterface pServerObj,
    /* [in] */ const _ELASTOS ClassID* pClassID,
    /* [out] */ PInterface *ppServerObj);

ELAPI _Impl_AcquireCallbackHandler(
    /* [in] */ PInterface pServerObj,
    /* [in] */ _ELASTOS REIID iid,
    /* [out] */  PInterface *ppHandler);

ELAPI _CCallbackParcel_New(
    /* [out] */ IParcel **ppObj);

ELAPI _CParcel_New(
    /* [out] */ IParcel **ppObj);

ELAPI _CObject_MarshalInterface(
    /* [in] */ IInterface *pObj,
    /* [in] */ MarshalType type,
    /* [out] */ void **ppBuf,
    /* [out] */ _ELASTOS Int32 *pSize);

ELAPI _CObject_UnmarshalInterface(
    /* [in] */ void *pBuf,
    /* [in] */ MarshalType type,
    /* [in] */ UnmarshalFlag flag,
    /* [out] */ IInterface **ppObj,
    /* [out] */ _ELASTOS Int32 *pSize);

#endif // __ELAPI_H__
/** @} */
