
#ifndef __ELASTOS_DROID_PROVIDER_CCONTACTSCONTACTMETHODSHELPER_H__
#define __ELASTOS_DROID_PROVIDER_CCONTACTSCONTACTMETHODSHELPER_H__

#include "CContactsContactMethods.h"
#include "_Elastos_Droid_Provider_CContactsContactMethodsHelper.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContactMethodsHelper)
{
public:
    /**
     * The content:// style URL for this table
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URL for sub-directory of e-mail addresses.
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI GetRAW_CONTENT_EMAIL_URL(
        /* [out] */ IUri** uri);

    /**
     * @deprecated see {@link android.provider.ContactsContract}
     */
    // @Deprecated
    CARAPI EncodePredefinedImProtocol(
        /* [in] */ Int32 protocol,
        /* [out] */ String* value);

    /**
     * @deprecated see {@link android.provider.ContactsContract}
     */
    // @Deprecated
    CARAPI EncodeCustomImProtocol(
        /* [in] */ const String& protocolString,
        /* [out] */ String* value);

    /**
     * @deprecated see {@link android.provider.ContactsContract}
     */
    // @Deprecated
    CARAPI DecodeImProtocol(
        /* [in] */ const String& encodedString,
        /* [out] */ IInterface** value);

    /**
     * This looks up the provider name defined in
     * from the predefined IM protocol id.
     * This is used for interacting with the IM application.
     *
     * @param protocol the protocol ID
     * @return the provider name the IM app uses for the given protocol, or null if no
     * provider is defined for the given protocol
     * @deprecated see {@link android.provider.ContactsContract}
     * @hide
     */
    // @Deprecated
    CARAPI LookupProviderNameFromId(
        /* [in] */ Int32 protocol,
        /* [out] */ String* name);

    /**
     * @deprecated see {@link android.provider.ContactsContract}
     */
    // @Deprecated
    CARAPI GetDisplayLabel(
        /* [in] */ IContext* context,
        /* [in] */ Int32 kind,
        /* [in] */ Int32 type,
        /* [in] */ ICharSequence* label,
        /* [out] */ ICharSequence** lb);
};

} // Provider
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCONTACTSCONTACTMETHODSHELPER_H__
