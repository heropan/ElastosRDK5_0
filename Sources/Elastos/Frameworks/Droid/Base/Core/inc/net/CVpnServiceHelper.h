
#ifndef __CVPNSERVICEHELPER_H__
#define __CVPNSERVICEHELPER_H__

#include "_CVpnServiceHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CVpnServiceHelper)
{
public:
    /**
     * Prepare to establish a VPN connection. This method returns {@code null}
     * if the VPN application is already prepared. Otherwise, it returns an
     * {@link Intent} to a system activity. The application should launch the
     * activity using {@link Activity#startActivityForResult} to get itself
     * prepared. The activity may pop up a dialog to require user action, and
     * the result will come back via its {@link Activity#onActivityResult}.
     * If the result is {@link Activity#RESULT_OK}, the application becomes
     * prepared and is granted to use other methods in this class.
     *
     * <p>Only one application can be granted at the same time. The right
     * is revoked when another application is granted. The application
     * losing the right will be notified via its {@link #onRevoke}. Unless
     * it becomes prepared again, subsequent calls to other methods in this
     * class will fail.
     *
     * @see #onRevoke
     */
    CARAPI Prepare(
        /* [in] */ IContext* context,
        /* [out] */ IIntent** intent);
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __CVPNSERVICEHELPER_H__
