
#include "elastos/droid/systemui/SystemUI.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.CoreLibrary.Utility.h"

namespace Elastos {
namespace Droid {
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
    /* [in] */ InterfaceID interfaceType)
{
    if (mComponents == NULL) {
        return NULL;
    }
    return (*mComponents)[interfaceType];
}

ECode SystemUI::PutComponent(
    /* [in] */ InterfaceID interfaceType,
    /* [in] */ IInterface* component)
{
    if (mComponents != NULL) {
        (*mComponents)[interfaceType] = component;
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
    /* [in] */ HashMap<InterfaceID, AutoPtr<IInterface> >* components)
{
    mComponents = components;
    return NOERROR;
}

AutoPtr<HashMap<InterfaceID, AutoPtr<IInterface> > > SystemUI::GetComponents()
{
    return mComponents;
}

} // namespace SystemUI
} // namespace Droid
} // namespace Elastos
