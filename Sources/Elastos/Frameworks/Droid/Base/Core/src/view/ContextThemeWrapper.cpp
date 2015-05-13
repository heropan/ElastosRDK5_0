
#include "content/res/CConfiguration.h"
#include "content/res/CResources.h"
#include "view/ContextThemeWrapper.h"
#include "view/LayoutInflater.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::EIID_IContext;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Content::Res::CResources;

namespace Elastos {
namespace Droid {
namespace View {

ContextThemeWrapper::ContextThemeWrapper()
{
}

ContextThemeWrapper::ContextThemeWrapper(
    /* [in] */ IContext* base,
    /* [in] */ Int32 themeres)
    : ContextWrapper(base)
    , mThemeResource(themeres)
{
}

ECode ContextThemeWrapper::AttachBaseContext(
    /* [in] */ IContext* newBase)
{
    return ContextWrapper::AttachBaseContext(newBase);
}

ECode ContextThemeWrapper::ApplyOverrideConfiguration(
    /* [in] */ IConfiguration* overrideConfiguration)
{
    if (mResources != NULL) {
        // throw new IllegalStateException("getResources() has already been called");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (mOverrideConfiguration != NULL) {
        // throw new IllegalStateException("Override configuration has already been set");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return CConfiguration::New(overrideConfiguration, (IConfiguration**)&mOverrideConfiguration);
}

ECode ContextThemeWrapper::GetResources(
    /* [out] */ IResources** resources)
{
    if (mResources != NULL) {
        *resources = mResources;
        INTERFACE_ADDREF(*resources);
        return NOERROR;
    }
    if (mOverrideConfiguration == NULL) {
        ContextWrapper::GetResources((IResources**)&mResources);
        *resources = mResources;
        INTERFACE_ADDREF(*resources);
        return NOERROR;
    }
    else {
        AutoPtr<IContext> resc;
        FAIL_RETURN(CreateConfigurationContext(mOverrideConfiguration, (IContext**)&resc));
        FAIL_RETURN(resc->GetResources((IResources**)&mResources));
        *resources = mResources;
        INTERFACE_ADDREF(*resources);
        return NOERROR;
    }
}

ECode ContextThemeWrapper::SetTheme(
    /* [in] */ Int32 resid)
{
    mThemeResource = resid;
    return InitializeTheme();
}

ECode ContextThemeWrapper::GetThemeResId(
    /* [out] */ Int32* resId)
{
    *resId = mThemeResource;
    return NOERROR;
}

ECode ContextThemeWrapper::GetTheme(
    /* [in] */ IResourcesTheme** theme)
{
    if (mTheme == NULL) {
        AutoPtr<IApplicationInfo> appInfo;
        GetApplicationInfo((IApplicationInfo**)&appInfo);
        Int32 version;
        appInfo->GetTargetSdkVersion(&version);
        mThemeResource = CResources::SelectDefaultTheme(mThemeResource, version);
        FAIL_RETURN(InitializeTheme());
    }

    *theme = mTheme;
    INTERFACE_ADDREF(*theme);
    return NOERROR;
    // if (mTheme != null) {
    //     return mTheme;
    // }

    // mThemeResource = Resources.selectDefaultTheme(mThemeResource,
    //         getApplicationInfo().targetSdkVersion);
    // initializeTheme();

    // return mTheme;
}

ECode ContextThemeWrapper::GetSystemService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** object)
{
    if (IContext::LAYOUT_INFLATER_SERVICE.Equals(name)) {
        if (mInflater == NULL) {
            AutoPtr<ILayoutInflater> inflater;
            LayoutInflater::From(mBase, (ILayoutInflater**)&inflater);
            inflater->CloneInContext(
                (IContext*)this->Probe(EIID_IContext),
                (ILayoutInflater**)&mInflater);
        }
        *object = mInflater;
        INTERFACE_ADDREF(*object);
        return NOERROR;
    }
    return mBase->GetSystemService(name, object);
}

/**
 * Called by {@link #setTheme} and {@link #getTheme} to apply a theme
 * resource to the current Theme object.  Can override to change the
 * default (simple) behavior.  This method will not be called in multiple
 * threads simultaneously.
 *
 * @param theme The Theme object being modified.
 * @param resid The theme style resource being applied to <var>theme</var>.
 * @param first Set to true if this is the first time a style is being
 *              applied to <var>theme</var>.
 */
ECode ContextThemeWrapper::OnApplyThemeResource(
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ Int32 resid,
    /* [in] */ Boolean first)
{
    return theme->ApplyStyle(resid, TRUE);
}

ECode ContextThemeWrapper::InitializeTheme()
{
    Boolean first = mTheme == NULL;
    if (first) {
        AutoPtr<IResources> resources;
        GetResources((IResources**)&resources);
        resources->NewTheme((IResourcesTheme**)&mTheme);
        AutoPtr<IResourcesTheme> theme;
        mBase->GetTheme((IResourcesTheme**)&theme);
        if (theme != NULL) {
            mTheme->SetTo(theme);
        }
    }
    return OnApplyThemeResource(mTheme, mThemeResource, first);
}

ECode ContextThemeWrapper::Init(
    /* [in] */ IContext* base,
    /* [in] */ Int32 themeres)
{
    ContextWrapper::Init(base);
    mThemeResource = themeres;
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
