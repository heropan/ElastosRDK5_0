
#ifndef __ELASTOS_DROID_NET_CLINKCAPABILITIES_H__
#define __ELASTOS_DROID_NET_CLINKCAPABILITIES_H__

#include "_CLinkCapabilities.h"
#include "ext/frameworkext.h"

using Elastos::Utility::IObjectInt32Map;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CLinkCapabilities)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ILinkCapabilities* source);

    CARAPI Clear();

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI GetSize(
        /* [out] */ Int32* result);

    CARAPI GetValue(
        /* [in] */ Int32 key,
        /* [out] */ String* result);

    CARAPI GetCapabilities(
        /* [out] */ IObjectInt32Map** result);

    CARAPI Put(
        /* [in] */ Int32 key,
        /* [in] */ const String& value);

    CARAPI ContainsKey(
        /* [in] */ Int32 key,
        /* [out] */ Boolean* result);

    CARAPI ContainsValue(
        /* [in] */ const String& value,
        /* [out] */ Boolean* result);

    CARAPI GetEntrySet(
        /* [out, callee] */ ArrayOf<Int32>** keys,
        /* [out] */ IObjectContainer** values);

    CARAPI GetKeySet(
        /* [out, callee] */ ArrayOf<Int32>** keys);

    CARAPI GetValues(
        /* [out] */ IObjectContainer** result);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    static CARAPI CreateNeedsMap(
        /* [in] */ const String& applicationRole,
        /* [out] */ ILinkCapabilities** result);

protected:
    static void Log(
        /* [in] */ const String& s);

private:
    /** The Map of Keys to Values */
    AutoPtr<IObjectInt32Map> mCapabilities;

    static const String TAG;
    static const Boolean DBG;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_CLINKCAPABILITIES_H__
