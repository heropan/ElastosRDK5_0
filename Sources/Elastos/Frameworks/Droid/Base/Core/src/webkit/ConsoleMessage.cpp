
#include "webkit/ConsoleMessage.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ConsoleMessage::ConsoleMessage()
    : mLevel(0)
    , mLineNumber(0)
{
}

ConsoleMessage::ConsoleMessage(
    /* [in] */ const String& message,
    /* [in] */ const String& sourceId,
    /* [in] */ Int32 lineNumber,
    /* [in] */ /*MessageLevel*/Int32 msgLevel)
{
    Init(message, sourceId, lineNumber, msgLevel);
}

void ConsoleMessage::Init(
    /* [in] */ const String& message,
    /* [in] */ const String& sourceId,
    /* [in] */ Int32 lineNumber,
    /* [in] */ /*MessageLevel*/Int32 msgLevel)
{
    mMessage = message;
    mSourceId = sourceId;
    mLineNumber = lineNumber;
    mLevel = msgLevel;
}

/*MessageLevel*/Int32 ConsoleMessage::MessageLevel()
{
    return mLevel;
}

String ConsoleMessage::Message()
{
    return mMessage;
}

String ConsoleMessage::SourceId()
{
    return mSourceId;
}

Int32 ConsoleMessage::LineNumber()
{
    return mLineNumber;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos