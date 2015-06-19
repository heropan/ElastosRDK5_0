//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <elapi.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

#ifdef _win32
#include <e_time.h>
#endif

using namespace Elastos;

EXTERN_C const InterfaceID EIID_IProxy;

class CCallbackRendezvous : public ICallbackRendezvous
{
public:
    CARAPI_(PInterface) Probe(REIID riid);
    CARAPI_(UInt32) AddRef();
    CARAPI_(UInt32) Release();
    CARAPI GetInterfaceID(IInterface *pObject, InterfaceID *pIID);

    CCallbackRendezvous(
        PInterface pCallbackContext,
        ICallbackSink* pCallbackSink,
        Int32 eventId,
        Boolean* pbEventFlag,
        Boolean bNewCallback) :
            m_pCallbackContext(pCallbackContext),
            m_pCallbackSink(pCallbackSink),
            m_EventId(eventId),
            m_pbEventFlag(pbEventFlag),
            m_bNewCallback(bNewCallback),
            m_cRef(0)
    {
        if (m_pCallbackSink) m_pCallbackSink->AddRef();
    }

    virtual ~CCallbackRendezvous();

    CARAPI Wait(
            /* in */ Millisecond32 msTimeout,
            /* out */ WaitResult *result);

    PInterface                  m_pCallbackContext;
    ICallbackSink*              m_pCallbackSink;
    Int32                       m_EventId;
    Boolean*                    m_pbEventFlag;
    Boolean                     m_bNewCallback;
private:
    Int32                 m_cRef;
};

UInt32 CCallbackRendezvous::AddRef()
{
    Int32 ref = atomic_inc(&m_cRef);

    return (UInt32)ref;
}

UInt32 CCallbackRendezvous::Release()
{
    Int32 ref = atomic_dec(&m_cRef);

    if (0 == ref) {
        delete this;
    }
    assert(ref >= 0);
    return ref;
}

PInterface CCallbackRendezvous::Probe(REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    return NULL;
}

ECode CCallbackRendezvous::GetInterfaceID(IInterface *pObject, InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

CCallbackRendezvous::~CCallbackRendezvous()
{
    if (m_pCallbackSink) {
        if (m_bNewCallback) {
            PInterface pOrgCtx;
            pOrgCtx = (PInterface)pthread_getspecific(TL_CALLBACK_SLOT);
            if (m_pCallbackContext) m_pCallbackContext->AddRef();
            pthread_setspecific(TL_CALLBACK_SLOT, m_pCallbackContext);
            m_pCallbackSink->RemoveCallback(
                                            m_EventId, EventHandler::Make(NULL, NULL));
            if (m_pCallbackContext) m_pCallbackContext->Release();
            pthread_setspecific(TL_CALLBACK_SLOT, pOrgCtx);
        }
        m_pCallbackSink->Release();
    }
    if (m_pCallbackContext) m_pCallbackContext->Release();
}

ECode CCallbackRendezvous::Wait(
    Millisecond32 msTimeOut,
    WaitResult *result)
{

    ECode ec = _Impl_CallbackSink_WaitForCallbackEvent(
                            m_pCallbackContext,
                            msTimeOut,
                            result,
                            m_pbEventFlag,
                            0);

    return ec;
}

ELAPI _Impl_CCallbackRendezvous_New(
        PInterface pCallbackContext,
        ICallbackSink* pCallbackSink,
        CallbackEventId eventId,
        Boolean* pbEventFlag,
        Boolean bNewCallback,
        ICallbackRendezvous** ppICallbackRendezvous)
{
    if (NULL == ppICallbackRendezvous) return E_INVALID_ARGUMENT;
    *ppICallbackRendezvous = new CCallbackRendezvous(
                                    pCallbackContext,
                                    pCallbackSink,
                                    eventId,
                                    pbEventFlag,
                                    bNewCallback);
    if (*ppICallbackRendezvous == NULL) return E_OUT_OF_MEMORY;
    (*ppICallbackRendezvous)->AddRef();

    return NOERROR;
}

// ElAPI functions
//

pthread_key_t g_TlSystemSlots[10];

EXTERN_C pthread_key_t *getTlSystemSlotBase()
{
    return g_TlSystemSlots;
}

EXTERN void InitTLS()
{
    for (int i = 0; i < 10; i++) {
        pthread_key_create(&g_TlSystemSlots[i], NULL);
    }
}

EXTERN void UninitTLS()
{
    for (int i = 0; i < 10; i++) {
        pthread_key_delete(g_TlSystemSlots[i]);
    }
}

ELAPI _CObject_EnterRegime(PInterface pObject, PRegime pRegime)
{
    return pRegime->ObjectEnter(pObject);
}

ELAPI _CObject_LeaveRegime(PInterface pObject, PRegime pRegime)
{
    return pRegime->ObjectLeave(pObject);
}

ELAPI_(Boolean) _Impl_CheckHelperInfoFlag(UInt32 flag)
{
    UInt32 uFlag = 0;

    uFlag = (UInt32)pthread_getspecific(TL_HELPER_INFO_SLOT);

    if (uFlag & flag) return TRUE;
    else return FALSE;
}

ELAPI_(void) _Impl_SetHelperInfoFlag(UInt32 flag, Boolean bValue)
{
    UInt32 uFlag = 0;
    ECode ec;

    uFlag = (UInt32)pthread_getspecific(TL_HELPER_INFO_SLOT);

    if (bValue) {
        uFlag |= flag;
    }
    else {
        uFlag &= ~flag;
    }

    ec = pthread_setspecific(TL_HELPER_INFO_SLOT, (void*)uFlag);
    assert(SUCCEEDED(ec));
}

ELAPI _Impl_EnterProtectedZone()
{
    _Impl_SetHelperInfoFlag(HELPER_ENTERED_PROTECTED_ZONE, TRUE);
    return NOERROR;
}

ELAPI _Impl_LeaveProtectedZone()
{
    _Impl_SetHelperInfoFlag(HELPER_ENTERED_PROTECTED_ZONE, FALSE);
    return NOERROR;
}

ELAPI _Impl_InsideProtectedZone()
{
    if (!_Impl_CheckHelperInfoFlag(HELPER_ENTERED_PROTECTED_ZONE)) {
        return E_NOT_IN_PROTECTED_ZONE;
    }

    return NOERROR;
}

#include <marshal_ipc.h>
#include <marshal_rpc.h>

ELAPI _CObject_MarshalInterface(
    /* [in] */ IInterface *pObj,
    /* [in] */ MarshalType type,
    /* [out] */ void **ppBuf,
    /* [out] */ Int32 *pSize)
{
    if (ppBuf == NULL) {
        if (pSize) *pSize = 0;
        return E_INVALID_ARGUMENT;
    }
    *ppBuf = NULL;

    if (pSize == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *pSize = 0;

    if (pObj == NULL || pSize == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (type == MarshalType_IPC) {
        Elastos::IPC::InterfacePack* pItfPack = (Elastos::IPC::InterfacePack*)calloc(sizeof(Elastos::IPC::InterfacePack), 1);
        if (pItfPack == NULL) {
            return E_OUT_OF_MEMORY;
        }

        ECode ec = StdMarshalInterface(pObj, pItfPack);
        if (FAILED(ec)) {
            free(pItfPack);
            return ec;
        }

        *pSize = sizeof(Elastos::IPC::InterfacePack);
        *ppBuf = pItfPack;
        return NOERROR;
    }
    else if (type == MarshalType_RPC) {
        Elastos::RPC::InterfacePack* pItfPack = (Elastos::RPC::InterfacePack*)calloc(sizeof(Elastos::RPC::InterfacePack), 1);
        if (pItfPack == NULL) {
            return E_OUT_OF_MEMORY;
        }

        ECode ec = StdMarshalInterface(pObj, pItfPack);
        if (FAILED(ec)) {
            free(pItfPack);
            return ec;
        }

        *pSize = sizeof(Elastos::RPC::InterfacePack);
        *ppBuf = pItfPack;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ELAPI _CObject_UnmarshalInterface(
    /* [in] */ void *pBuf,
    /* [in] */ MarshalType type,
    /* [in] */ UnmarshalFlag flag,
    /* [out] */ IInterface **ppObj,
    /* [out] */ Int32 *pSize)
{
    if (ppObj == NULL) {
        if (pSize) *pSize = 0;
        return E_INVALID_ARGUMENT;
    }
    *ppObj = NULL;

    if (pSize == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *pSize = 0;

    if (pBuf == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (type == MarshalType_IPC) {
        ECode ec = StdUnmarshalInterface(flag, (Elastos::IPC::InterfacePack*)pBuf, ppObj);
        if (FAILED(ec)) {
            return ec;
        }

        *pSize = sizeof(Elastos::IPC::InterfacePack);
        return NOERROR;
    }
    else if (type == MarshalType_RPC) {
        ECode ec = StdUnmarshalInterface(flag, (Elastos::RPC::InterfacePack*)pBuf, ppObj);
        if (FAILED(ec)) {
            return ec;
        }

        *pSize = sizeof(Elastos::RPC::InterfacePack);
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}
