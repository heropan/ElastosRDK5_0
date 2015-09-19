
#ifndef __ELASTOS_DROID_PROVIDER_CCALLS_H__
#define __ELASTOS_DROID_PROVIDER_CCALLS_H__

#include "_Elastos_Droid_Provider_CCalls.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Internal::Telephony::ICallerInfo;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CCalls)
{
public:
    /**
     * The content:// style URL for this table
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URL for filtering this table on phone numbers
     */
    CARAPI GetCONTENT_FILTER_URI(
        /* [out] */ IUri** uri);

    /**
     * Content uri with {@link #ALLOW_VOICEMAILS_PARAM_KEY} set. This can directly be used to
     * access call log entries that includes voicemail records.
     *
     * @hide
     */
    CARAPI GetCONTENT_URIWITHVOICEMAIL(
        /* [out] */ IUri** uri);

    /**
     * Adds a call to the call log.
     *
     * @param ci the CallerInfo object to get the target contact from.  Can be null
     * if the contact is unknown.
     * @param context the context used to get the ContentResolver
     * @param number the phone number to be added to the calls db
     * @param presentation the number presenting rules set by the network for
     *        "allowed", "payphone", "restricted" or "unknown"
     * @param callType enumerated values for "incoming", "outgoing", or "missed"
     * @param start time stamp for the call in milliseconds
     * @param duration call duration in seconds
     *
     * {@hide}
     */
    CARAPI AddCall(
        /* [in] */ ICallerInfo* ci,
        /* [in] */ IContext* context,
        /* [in] */ const String& number,
        /* [in] */ Int32 presentation,
        /* [in] */ Int32 callType,
        /* [in] */ Int64 start,
        /* [in] */ Int32 duration,
        /* [out] */ IUri** uri);

    /**
     * Query the call log database for the last dialed number.
     * @param context Used to get the content resolver.
     * @return The last phone number dialed (outgoing) or an empty
     * string if none exist yet.
     */
    CARAPI GetLastOutgoingCall(
        /* [in] */ IContext* context,
        /* [out] */ String* call);

private:
    CARAPI RemoveExpiredEntries(
        /* [in] */ IContext* context);

public:
    static AutoPtr<IUri> CONTENT_URI;

    static AutoPtr<IUri> CONTENT_FILTER_URI;

    static AutoPtr<IUri> CONTENT_URI_WITH_VOICEMAIL;
};

} //Provider
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCALLS_H__
