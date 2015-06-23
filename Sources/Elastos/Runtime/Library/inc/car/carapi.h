/** @addtogroup CARRef
  *   @{
  */

//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CARAPI_H__
#define __CARAPI_H__

extern "C" {

struct EzMultiQI
{
    const _ELASTOS InterfaceID* mIID;
    PInterface mObject;
    _ELASTOS ECode mEC;
};
typedef struct EzMultiQI EzMultiQI, *PMULTIQI;

#define RGM_INVALID_DOMAIN          ((PRegime)0x0000)
#define RGM_SAME_DOMAIN             ((PRegime)0x0001)
#define RGM_DIFF_DOMAIN             ((PRegime)0x0002)
#define RGM_DEFAULT                 ((PRegime)0x0003)
#define RGM_DIFF_MACHINE            ((PRegime)0x0004)
#define RGM_SAME_PROCESS            RGM_SAME_DOMAIN
#define RGM_DIFF_PROCESS            RGM_DIFF_DOMAIN

#define RGM_MAX_NUMBER              ((PRegime)0xFFFF)
#define IS_INVALID_REGIME(dw)      ((dw == RGM_INVALID_DOMAIN))
#define IS_RGM_NUMBER(dw)           ((!IS_INVALID_REGIME(dw)) && \
	                                 ((dw == RGM_SAME_DOMAIN) || \
	                                  (dw == RGM_DIFF_DOMAIN) || \
	                                  (dw == RGM_DEFAULT) || \
	                                  (dw == RGM_DIFF_MACHINE)))

ELAPI _CObject_CreateInstance(
    /* [in] */ _ELASTOS RClassID rclsid,
    /* [in] */ PRegime pRegime,
    /* [in] */ _ELASTOS REIID riid,
    /* [out] */ PInterface *ppObject);

ELAPI _CObject_CreateInstanceEx(
    /* [in] */ _ELASTOS RClassID rclsid,
    /* [in] */ PRegime pRegime,
    /* [in] */ _ELASTOS UInt32 cmq,
    /* [out] */ PMULTIQI pResults);

ELAPI _CObject_AcquireClassFactory(
    /* [in] */ _ELASTOS RClassID rclsid,
    /* [in] */ PRegime pRegime,
    /* [in] */ _ELASTOS REIID iid,
    /* [out] */ PInterface *ppObject);

ELAPI_(_ELASTOS Boolean) _CObject_Compare(
    /* [in] */ PInterface pObjectA,
    /* [in] */ PInterface pObjectB);

ELAPI _CObject_AttachAspect(
    /* [in] */ PInterface pAggregator,
    /* [in] */ _ELASTOS RClassID rAspectClsid);

ELAPI _CObject_DetachAspect(
    /* [in] */ PInterface pAggregator,
    /* [in] */ _ELASTOS RClassID rAspectClsid);

ELAPI _CObject_EnterRegime(
    /* [in] */ PInterface pObj,
    /* [in] */ PRegime pRgm);

ELAPI _CObject_LeaveRegime(
    /* [in] */ PInterface pObj,
    /* [in] */ PRegime pRgm);

typedef interface ICallbackSink *PCALLBACKSINK;
typedef interface ICallbackRendezvous* PCallbackRendezvous;

ELAPI _CObject_AcquireCallbackSink(
    /* [in] */ PInterface pObject,
    /* [out] */ PCALLBACKSINK *ppCallbackSink);

ELAPI _CObject_AddCallback(
    /* [in] */ PInterface pServerObj,
    /* [in] */ _ELASTOS Int32 dwEvent,
    /* [in] */ _ELASTOS EventHandler delegate);

ELAPI _CObject_RemoveCallback(
    /* [in] */ PInterface pServerObj,
    /* [in] */ _ELASTOS Int32 dwEvent,
    /* [in] */ _ELASTOS EventHandler delegate);

ELAPI _CObject_RemoveAllCallbacks(
    /* [in] */ PInterface pServerObj);

ELAPI _CObject_AcquireCallbackRendezvous(
    /* [in] */ PInterface pServerObj,
    /* [in] */ _ELASTOS Int32 dwEvent,
    /* [out] */ PCallbackRendezvous* ppCallbackRendezvous);

ELAPI_(_ELASTOS Boolean) _CModule_CanUnloadAllModules();

}
// extern "C"

#endif // __CARAPI_H__
