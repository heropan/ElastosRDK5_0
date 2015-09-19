
#ifndef __ELASTOS_DROID_WEBKIT_CONSOLEMESSAGE_H__
#define __ELASTOS_DROID_WEBKIT_CONSOLEMESSAGE_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Public class representing a JavaScript console message from WebCore. This could be a issued
 * by a call to one of the <code>console</code> logging functions (e.g.
 * <code>console.log('...')</code>) or a JavaScript error on the  page. To receive notifications
 * of these messages, override the
 * {@link WebChromeClient#onConsoleMessage(ConsoleMessage)} function.
 */
class ConsoleMessage
{
public:
    ConsoleMessage();

    ConsoleMessage(
        /* [in] */ const String& message,
        /* [in] */ const String& sourceId,
        /* [in] */ Int32 lineNumber,
        /* [in] */ /*MessageLevel*/Int32 msgLevel);

    virtual CARAPI_(/*MessageLevel*/Int32) MessageLevel();

    virtual CARAPI_(String) Message();

    virtual CARAPI_(String) SourceId();

    virtual CARAPI_(Int32) LineNumber();

protected:
    CARAPI_(void) Init(
        /* [in] */ const String& message,
        /* [in] */ const String& sourceId,
        /* [in] */ Int32 lineNumber,
        /* [in] */ /*MessageLevel*/Int32 msgLevel);

private:
    /*MessageLevel*/Int32 mLevel;
    String mMessage;
    String mSourceId;
    Int32 mLineNumber;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_CONSOLEMESSAGE_H__
