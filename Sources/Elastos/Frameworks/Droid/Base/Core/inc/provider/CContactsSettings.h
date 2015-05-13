
#ifndef __CCONTACTSSETTINGS_H__
#define __CCONTACTSSETTINGS_H__

#include "_CContactsSettings.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IContentResolver;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsSettings)
{
public:
    /**
     * no public constructor since this is a utility class
     */
    CARAPI constructor();

    /**
     * The content:// style URL for this table
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);

    /**
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI GetSetting(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& account,
        /* [in] */ const String& key,
        /* [out] */ String* setting);

    /**
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI SetSetting(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& account,
        /* [in] */ const String& key,
        /* [in] */ const String& value);
};

} // Provider
} // Droid
} // Elastos

#endif //__CCONTACTSSETTINGS_H__