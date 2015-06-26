
#ifndef __JSRESULTRECEIVER_H__
#define __JSRESULTRECEIVER_H__

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * This interface is used when the AwContentsClient offers a JavaScript
 * modal dialog (alert, beforeunload or confirm) to enable the client to
 * handle the dialog in their own way. AwContentsClient will offer an object
 * that implements this interface to the client and when the client has handled
 * the dialog, it must either callback with confirm() or cancel() to allow
 * processing to continue.
 */
class JsResultReceiver
{
public:
    virtual CARAPI_(void) Confirm() = 0;

    virtual CARAPI_(void) Cancel() = 0;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__JSRESULTRECEIVER_H__