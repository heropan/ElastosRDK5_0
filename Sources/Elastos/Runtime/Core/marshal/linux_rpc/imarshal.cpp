
#include <pthread.h>
#include <prxstub.h>
#include <rot.h>

_ELASTOS_NAMESPACE_USING

EXTERN_C const InterfaceID EIID_IProxy;

#define ASSERT_PROXY(pProxy) \
    do {assert(((CObjectProxy*)pProxy)->m_cInterfaces > 0);} while (0);

namespace Elastos {
namespace RPC {

extern pthread_mutex_t g_marshalLock;

ECode StdMarshalInterface(
    /* [in] */ IInterface *pObj,
    /* [out] */ InterfacePack *pInterfacePack)
{
    ExportObject ex;
    ECode ec;
    IStub *pIStub;
    IProxy *pv;

    pthread_mutex_lock(&g_marshalLock);
    ec = FindExportObject(pObj, &ex);
    if (SUCCEEDED(ec)) {
        pInterfacePack->m_sNetAddress = ex.m_sNetAddress;
        ex.mIStub->GetClassID(&pInterfacePack->mClsid);
        ec = ex.mIStub->GetInterfaceIndex(pObj, &pInterfacePack->mIndex);
        if (ec == E_NO_INTERFACE) {
            ec = E_CONFLICT_WITH_LOCAL_KEYWORD;
        }
        pthread_mutex_unlock(&g_marshalLock);
        if (SUCCEEDED(ec)) ex.mIStub->AddRef();
    }
    else {
        pv = (IProxy*)pObj->Probe(EIID_IProxy);
        if (pv) {
            pthread_mutex_unlock(&g_marshalLock);
            pInterfacePack->m_sNetAddress = ((CObjectProxy*)pv)->m_sNetAddress;
            ASSERT_PROXY(pv);
            pv->GetClassID(&pInterfacePack->mClsid);
            ec = pv->GetInterfaceIndex(pObj, &(pInterfacePack->mIndex));
        }
        else {
            ec = CObjectStub::S_CreateObject(pObj, &pIStub);
            pthread_mutex_unlock(&g_marshalLock);
            if (SUCCEEDED(ec)) {
                pInterfacePack->m_sNetAddress = ((CObjectStub*)pIStub)->m_sNetAddress;
                pIStub->GetClassID(&(pInterfacePack->mClsid));
                ec = pIStub->GetInterfaceIndex(pObj, &(pInterfacePack->mIndex));
                if (ec == E_NO_INTERFACE) {
                    ec = E_CONFLICT_WITH_LOCAL_KEYWORD;
                }
                if (SUCCEEDED(ec)) pIStub->AddRef();
            }
        }
    }

    return ec;
}

static ECode StdUnmarshalCustomInterface(
    /* [in] */ ICustomMarshal *pCustomMarshal,
    /* [in] */ InterfacePack *pInterfacePack,
    /* [out] */ IInterface **ppObj)
{
    *ppObj = NULL;

    ECode ec;

    ClassID proxyClsid;
    char uunmbuf[80];
    proxyClsid.pUunm = uunmbuf;

    ec = pCustomMarshal->GetClsid(&proxyClsid);
    if (SUCCEEDED(ec)) {
        AutoPtr<ICustomMarshal> pITempCustMsh;
        ec = _CObject_CreateInstance(proxyClsid, RGM_SAME_DOMAIN,
                EIID_ICustomMarshal, (IInterface **)&pITempCustMsh);
        if (SUCCEEDED(ec)) {
            ec = pITempCustMsh->CreateObject(pCustomMarshal, ppObj);
        }
    }

    return ec;
}

ECode StdUnmarshalInterface(
    /* [in] */ UnmarshalFlag flag,
    /* [in] */ InterfacePack *pInterfacePack,
    /* [out] */ IInterface **ppObj)
{
    *ppObj = NULL;

    ImportObject im;
    ExportObject ex;
    ECode ec;
    ICustomMarshal * pCustomMarshal;

    pthread_mutex_lock(&g_marshalLock);
    ec = FindExportObject(pInterfacePack->m_sNetAddress, &ex);
    if (SUCCEEDED(ec) && (flag == UnmarshalFlag_Noncoexisting)) {
        pthread_mutex_unlock(&g_marshalLock);
        pCustomMarshal = ICustomMarshal::Probe(ex.mObject);
        if (!pCustomMarshal) {
            CObjectStub* pSt = (CObjectStub*)ex.mIStub;
            *ppObj = pSt->m_pInterfaces[pInterfacePack->mIndex].mObject;
            (*ppObj)->AddRef();
            ex.mIStub->Release();

            return NOERROR;
        }
        else {
            ec = StdUnmarshalCustomInterface(\
                pCustomMarshal, pInterfacePack, ppObj);
            return ec;
        }
    }

    ec = FindImportObject(pInterfacePack->m_sNetAddress, &im);
    if (SUCCEEDED(ec)) {
        pthread_mutex_unlock(&g_marshalLock);
        pCustomMarshal = ICustomMarshal::Probe(im.mIProxy);
        if (!pCustomMarshal) {
            ec = im.mIProxy->GetInterface(pInterfacePack->mIndex, ppObj);
        }
        else {
            ec = StdUnmarshalCustomInterface(\
                pCustomMarshal, pInterfacePack, ppObj);
        }
        im.mIProxy->Release();
    }
    else {
        AutoPtr<IProxy> pIProxy;
        ec = CObjectProxy::S_CreateObject(
            pInterfacePack->mClsid,
            pInterfacePack->m_sNetAddress,
            (IProxy **)&pIProxy);
        pthread_mutex_unlock(&g_marshalLock);
        if (SUCCEEDED(ec)) {
            pCustomMarshal = ICustomMarshal::Probe(pIProxy);
            if (!pCustomMarshal) {
                ec = pIProxy->GetInterface(pInterfacePack->mIndex, ppObj);
            }
            else {
                ec = StdUnmarshalCustomInterface(\
                    pCustomMarshal, pInterfacePack, ppObj);
            }
        }
    }

    return ec;
}

} // namespace RPC
} // namespace Elastos
