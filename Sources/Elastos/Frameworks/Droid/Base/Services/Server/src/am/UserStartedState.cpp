
#include "am/UserStartedState.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

// 02edec2f-11c5-4a99-b562-2813a924ad2a
extern "C" const InterfaceID EIID_UserStartedState =
    { 0x02edec2f, 0x11c5, 0x4a99, { 0xb5, 0x62, 0x28, 0x13, 0xa9, 0x24, 0xad, 0x2a } };

const Int32 UserStartedState::STATE_BOOTING = 0;
const Int32 UserStartedState::STATE_RUNNING = 1;
const Int32 UserStartedState::STATE_STOPPING = 2;
const Int32 UserStartedState::STATE_SHUTDOWN = 3;

CAR_INTERFACE_IMPL_WITH_CPP_CAST(UserStartedState, IInterface, UserStartedState)

UserStartedState::UserStartedState(
    /* [in] */ IUserHandle* handle,
    /* [in] */ Boolean initial)
    : mHandle(handle)
    , mState(STATE_BOOTING)
    , mSwitching(FALSE)
    , mInitializing(FALSE)
{}

UserStartedState::~UserStartedState()
{}

void UserStartedState::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IPrintWriter* pw)
{
    pw->PrintString(prefix); pw->PrintString(String("mState="));
    switch (mState) {
        case STATE_BOOTING: pw->PrintString(String("BOOTING")); break;
        case STATE_RUNNING: pw->PrintString(String("RUNNING")); break;
        case STATE_STOPPING: pw->PrintString(String("STOPPING")); break;
        case STATE_SHUTDOWN: pw->PrintString(String("SHUTDOWN")); break;
        default: pw->PrintInt32(mState); break;
    }
    if (mSwitching) pw->PrintString(String(" SWITCHING"));
    if (mInitializing) pw->PrintString(String(" INITIALIZING"));
    pw->Println();
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
