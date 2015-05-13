
#include "pm/SharedUserSetting.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

// {34B3DE70-FA92-4C61-BA7E-756E654D081A}
extern const InterfaceID EIID_SharedUserSetting =
    { 0x34b3de70, 0xfa92, 0x4c61, { 0xba, 0x7e, 0x75, 0x6e, 0x65, 0x4d, 0x8, 0x1a } };

CAR_INTERFACE_IMPL_WITH_CPP_CAST(SharedUserSetting, IInterface, SharedUserSetting)


SharedUserSetting::SharedUserSetting(
    /* [in] */ const String& name,
    /* [in] */ Int32 pkgFlags)
    : GrantedPermissions(pkgFlags)
    , mName(name)
    , mUserId(0)
    , mPackages(10)
    , mSignatures(new PackageSignatures())
{}

SharedUserSetting::~SharedUserSetting()
{
    mPackages.Clear();
}


} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos
