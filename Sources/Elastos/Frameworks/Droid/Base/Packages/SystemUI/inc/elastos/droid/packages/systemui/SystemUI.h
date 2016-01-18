
#ifndef  __ELASTOS_DROID_PACKAGES_SYSTEMUI_SYSTEMUI_H__
#define  __ELASTOS_DROID_PACKAGES_SYSTEMUI_SYSTEMUI_H__

#include "_SystemUI.h"
#include <elastos/droid/ext/frameworkext.h>
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Core::Object;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IPrintWriter;
using Elastos::Utility::IMap;

namespace Elastos {
namespace Droid {
namespace Packages {
namespace SystemUI {

class SystemUI
    : public Object
    , public ISystemUI
{
public:
    CAR_INTERFACE_DECL();

    SystemUI();

    virtual ~SystemUI();

    virtual CARAPI Start() = 0;

    virtual CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    virtual CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

    virtual CARAPI OnBootCompleted();

    // @SuppressWarnings("unchecked")
    virtual CARAPI_(AutoPtr<IInterface>) GetComponent(
        /* [in] */ IClassInfo* interfaceType);

    virtual CARAPI PutComponent(
        /* [in] */ IClassInfo* interfaceType,
        /* [in] */ IInterface* component);

    virtual CARAPI SetContext(
        /* [in] */ IContext* context);

    virtual CARAPI GetContext(
        /* [out] */ IContext** context);

    virtual CARAPI SetComponents(
        /* [in] */ IMap* components);

    virtual CARAPI GetComponents(
        /* [out] */ IMap** components);

public:
    AutoPtr<IContext> mContext;
    AutoPtr<IMap/*<Class<?>, Object*/> mComponents;
};

} // namespace SystemUI
} // namespace Packages
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_PACKAGES_SYSTEMUI_SYSTEMUI_H__
