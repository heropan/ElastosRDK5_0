
#ifndef __ELASTOS_DROID_WEBKIT_WEBBACKFORWARDLISTCLIENT_H__
#define __ELASTOS_DROID_WEBKIT_WEBBACKFORWARDLISTCLIENT_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Interface to receive notifications when items are added to the
 * {@link WebBackForwardList}.
 * {@hide}
 */
class WebBackForwardListClient : public Object
{
public:
    /**
     * Notify the client that <var>item</var> has been added to the
     * WebBackForwardList.
     * @param item The newly created WebHistoryItem
     */
    virtual CARAPI_(void) OnNewHistoryItem(
        /* [in] */ IWebHistoryItem* item)
    {
    }

    /**
     * Notify the client that the <var>item</var> at <var>index</var> is now
     * the current history item.
     * @param item A WebHistoryItem
     * @param index The new history index
     */
    virtual CARAPI_(void) OnIndexChanged(
        /* [in] */ IWebHistoryItem* item,
        /* [in] */ Int32 index)
    {
    }

    CARAPI ToString(
        /* [out] */ String* info);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBBACKFORWARDLISTCLIENT_H__
