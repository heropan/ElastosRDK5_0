
#include "elastos/droid/packages/systemui/SystemUI.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.CoreLibrary.Utility.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace SystemUI {

CAR_INTERFACE_IMPL(SystemUI, Object, ISystemUI);
SystemUI::SystemUI()
{}

SystemUI::~SystemUI()
{}

ECode SystemUI::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    return NOERROR;
}

ECode SystemUI::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args)
{
    return NOERROR;
}

ECode SystemUI::OnBootCompleted()
{
    return NOERROR;
}

AutoPtr<IInterface> SystemUI::GetComponent(
    /* [in] */ IClassInfo* interfaceType)
{
    if (mComponents == NULL) {
        return NULL;
    }
    AutoPtr<IInterface> obj;
    mComponents->Get(interfaceType, (IInterface**)&obj);
    return obj;
}

ECode SystemUI::PutComponent(
    /* [in] */ IClassInfo* interfaceType,
    /* [in] */ IInterface* component)
{
    if (mComponents != NULL) {
        mComponents->Put(interfaceType, component);
    }
    return NOERROR;
}

ECode SystemUI::SetContext(
    /* [in] */ IContext* context)
{
    mContext = context;
    return NOERROR;
}

ECode SystemUI::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context);
    *context = mContext;
    REFCOUNT_ADD(*context);
    return NOERROR;
}

ECode SystemUI::SetComponents(
    /* [in] */ IMap* components)
{
    mComponents = components;
    return NOERROR;
}

ECode SystemUI::GetComponents(
    /* [out] */ IMap** components)
{
    VALIDATE_NOT_NULL(components);
    *components = mComponents;
    REFCOUNT_ADD(*components);
    return NOERROR;
}

} // namespace SystemUI
} // namespace Packages
} // namespace Droid
} // namespace Elastos
