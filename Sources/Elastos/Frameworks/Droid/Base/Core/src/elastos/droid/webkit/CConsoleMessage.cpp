
#include "elastos/droid/webkit/CConsoleMessage.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CConsoleMessage::constructor(
    /* [in] */ const String& message,
    /* [in] */ const String& sourceId,
    /* [in] */ Int32 lineNumber,
    /* [in] */ Elastos::Droid::Webkit::MessageLevel msgLevel)
{
    ConsoleMessage::Init(message, sourceId, lineNumber, msgLevel);
    return NOERROR;
}

ECode CConsoleMessage::MessageLevel(
    /* [out] */ Elastos::Droid::Webkit::MessageLevel* level)
{
    VALIDATE_NOT_NULL(level);
    *level = ConsoleMessage::MessageLevel();
    return NOERROR;
}

ECode CConsoleMessage::Message(
    /* [out] */ String* message)
{
    VALIDATE_NOT_NULL(message);
    *message = ConsoleMessage::Message();
    return NOERROR;
}

ECode CConsoleMessage::SourceId(
    /* [out] */ String* sourceId)
{
    VALIDATE_NOT_NULL(sourceId);
    *sourceId = ConsoleMessage::SourceId();
    return NOERROR;}

ECode CConsoleMessage::LineNumber(
    /* [out] */ Int32* lineNumber)
{
    VALIDATE_NOT_NULL(lineNumber);
    *lineNumber = ConsoleMessage::LineNumber();
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
