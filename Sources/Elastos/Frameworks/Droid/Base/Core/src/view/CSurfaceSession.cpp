
#include "view/CSurfaceSession.h"

namespace Elastos {
namespace Droid {
namespace View {

CSurfaceSession::CSurfaceSession()
{
    mNativeClient = new android::SurfaceComposerClient();
}

CSurfaceSession::~CSurfaceSession()
{
    mNativeClient = NULL;
}

ECode CSurfaceSession::Kill()
{
    mNativeClient->dispose();;

    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
