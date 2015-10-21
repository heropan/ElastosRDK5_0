
#include "elastos/droid/webkit/webview/chromium/WebStorageAdapter.h"

using Elastos::Utility::IMap;
using Elastos::Utility::CHashMap;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

//=====================================================================
//  WebStorageAdapter::InnerValueCallback<AwQuotaManagerBridgeOrigins>
//=====================================================================
//CAR_INTERFACE_IMPL(WebStorageAdapter::InnerValueCallback, Object, IValueCallback)

WebStorageAdapter::InnerValueCallback::InnerValueCallback(
    /* [in] */ WebStorageAdapter* owner,
    /* [in] */ const IInterface/*IValueCallback*/* callback)
    : mOwner(owner)
    , mCallback(callback)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebStorageAdapter::InnerValueCallback::OnReceiveValue(
    /* [in] */ AwQuotaManagerBridge::Origins* origins)
{
    // ==================before translated======================
    // Map<String, Origin> originsMap = new HashMap<String, Origin>();
    // for (int i = 0; i < origins.mOrigins.length; ++i) {
    //     Origin origin = new Origin(origins.mOrigins[i], origins.mQuotas[i],
    //             origins.mUsages[i]) {
    //         // Intentionally empty to work around cross-package protected visibility
    //         // of Origin constructor.
    //     };
    //     originsMap.put(origins.mOrigins[i], origin);
    // }
    // callback.onReceiveValue(originsMap);

    assert(0);
    assert(NULL == mOwner);
    assert(NULL == mCallback);

    AutoPtr<IMap> originsMap;
    CHashMap::New((IMap**)&originsMap);

    for (int i = 0; i < origins->mOrigins->GetLength(); ++i) {
        AutoPtr< ArrayOf<String> > itemOrigins = ArrayOf<String>::Alloc(1);
        AutoPtr< ArrayOf<Int64> > itemUsages = ArrayOf<Int64>::Alloc(1);
        AutoPtr< ArrayOf<Int64> > itemQuotas = ArrayOf<Int64>::Alloc(1);
        itemOrigins->Set(0, (*origins->mOrigins)[i]);
        itemUsages->Set(0, (*origins->mQuotas)[i]);
        itemQuotas->Set(0, (*origins->mUsages)[i]);
        AutoPtr<AwQuotaManagerBridge::Origins> origin = new AwQuotaManagerBridge::Origins(itemOrigins, itemUsages, itemQuotas);

        AutoPtr<ICharSequence> keyTmp;
        CString::New((*origins->mOrigins)[i], (ICharSequence**)&keyTmp);
        AutoPtr<IInterface> valueTmp = origin->Probe(EIID_IInterface);
        originsMap->Put(keyTmp, valueTmp);
    }
    //mCallback->OnReceiveValue(originsMap);
    return NOERROR;
}

//=====================================================================
//                          WebStorageAdapter
//=====================================================================
WebStorageAdapter::WebStorageAdapter(
    /* [in] */ AwQuotaManagerBridge* quotaManagerBridge)
    : mQuotaManagerBridge(quotaManagerBridge)
{
    // ==================before translated======================
    // mQuotaManagerBridge = quotaManagerBridge;
}

ECode WebStorageAdapter::GetOrigins(
    /* [in] */ const IInterface/*IValueCallback*/* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // mQuotaManagerBridge.getOrigins(new ValueCallback<AwQuotaManagerBridge.Origins>() {
    //     @Override
    //     public void onReceiveValue(AwQuotaManagerBridge.Origins origins) {
    //         Map<String, Origin> originsMap = new HashMap<String, Origin>();
    //         for (int i = 0; i < origins.mOrigins.length; ++i) {
    //             Origin origin = new Origin(origins.mOrigins[i], origins.mQuotas[i],
    //                     origins.mUsages[i]) {
    //                 // Intentionally empty to work around cross-package protected visibility
    //                 // of Origin constructor.
    //             };
    //             originsMap.put(origins.mOrigins[i], origin);
    //         }
    //         callback.onReceiveValue(originsMap);
    //     }
    // });

    assert(0);
    AutoPtr<InnerValueCallback> valueCallback = new InnerValueCallback(this, callback);
    AutoPtr<IInterface> interfaceTmp = valueCallback->Probe(EIID_IInterface);
    mQuotaManagerBridge->GetOrigins(interfaceTmp);
    return NOERROR;
}

ECode WebStorageAdapter::GetUsageForOrigin(
    /* [in] */ const String& origin,
    /* [in] */ IInterface/*IValueCallback*/* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // mQuotaManagerBridge.getUsageForOrigin(origin, callback);

    mQuotaManagerBridge->GetUsageForOrigin(origin, callback);
    return NOERROR;
}

ECode WebStorageAdapter::GetQuotaForOrigin(
    /* [in] */ const String& origin,
    /* [in] */ IInterface/*IValueCallback*/* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // mQuotaManagerBridge.getQuotaForOrigin(origin, callback);

    mQuotaManagerBridge->GetQuotaForOrigin(origin, callback);
    return NOERROR;
}

ECode WebStorageAdapter::SetQuotaForOrigin(
    /* [in] */ const String& origin,
    /* [in] */ Int64 quota)
{
    // ==================before translated======================
    // // Intentional no-op for deprecated method.

    return NOERROR;
}

ECode WebStorageAdapter::DeleteOrigin(
    /* [in] */ const String& origin)
{
    // ==================before translated======================
    // mQuotaManagerBridge.deleteOrigin(origin);

    mQuotaManagerBridge->DeleteOrigin(origin);
    return NOERROR;
}

ECode WebStorageAdapter::DeleteAllData()
{
    // ==================before translated======================
    // mQuotaManagerBridge.deleteAllData();

    mQuotaManagerBridge->DeleteAllData();
    return NOERROR;
}

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


