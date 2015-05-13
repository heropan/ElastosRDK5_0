
#ifndef __CCONSOLEMESSAGE_H__
#define __CCONSOLEMESSAGE_H__

#include "_CConsoleMessage.h"
#include "webkit/ConsoleMessage.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CConsoleMessage), public ConsoleMessage
{
public:
    CARAPI constructor(
        /* [in] */ const String& message,
        /* [in] */ const String& sourceId,
        /* [in] */ Int32 lineNumber,
        /* [in] */ Elastos::Droid::Webkit::MessageLevel msgLevel);

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

#endif // __CCONSOLEMESSAGE_H__
