
#ifndef __ELASTOS_DROID_WEBKIT_CCONSOLEMESSAGE_H__
#define __ELASTOS_DROID_WEBKIT_CCONSOLEMESSAGE_H__

#include "_Elastos_Droid_Webkit_CConsoleMessage.h"
#include "elastos/droid/webkit/ConsoleMessage.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CConsoleMessage), public ConsoleMessage
{
public:
    CAR_OBJECT_DECL();

    CARAPI MessageLevel(
        /* [out] */ Elastos::Droid::Webkit::MessageLevel* level);

    CARAPI Message(
        /* [out] */ String* message);

    CARAPI SourceId(
        /* [out] */ String* sourceId);

    CARAPI LineNumber(
        /* [out] */ Int32* lineNumber);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CCONSOLEMESSAGE_H__
