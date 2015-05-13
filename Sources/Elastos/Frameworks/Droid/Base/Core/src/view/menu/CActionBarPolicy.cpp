#include "view/menu/CActionBarPolicy.h"
#include "view/CViewConfigurationHelper.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::View::IViewConfiguration;
using Elastos::Droid::View::IViewConfigurationHelper;
using Elastos::Droid::View::CViewConfigurationHelper;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Utility::IDisplayMetrics;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CActionBarPolicy::CActionBarPolicy()
{}

ECode CActionBarPolicy::constructor(
        /* [in] */ IContext* ctx)
{
    mContext = ctx;
    return NOERROR;
}

ECode CActionBarPolicy::GetMaxActionButtons(
        /* [out] */ Int32* rst)
{
    AutoPtr<IResources> rs;
    mContext->GetResources((IResources**)&rs);
    return rs->GetInteger(R::integer::max_action_buttons, rst);
}

ECode CActionBarPolicy::ShowsOverflowMenuButton(
        /* [out] */ Boolean* rst)
{
    AutoPtr<IViewConfigurationHelper> helper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);
    AutoPtr<IViewConfiguration> vcf;
    helper->Get(mContext, (IViewConfiguration**)&vcf);
    Boolean isHas;
    vcf->HasPermanentMenuKey(&isHas);
    *rst = !isHas;
    return NOERROR;
}

ECode CActionBarPolicy::GetEmbeddedMenuWidthLimit(
        /* [out] */ Int32* rst)
{
    AutoPtr<IResources> rs;
    mContext->GetResources((IResources**)&rs);
    AutoPtr<IDisplayMetrics> dm;
    rs->GetDisplayMetrics((IDisplayMetrics**)&dm);
    Int32 widthPixels;
    dm->GetWidthPixels(&widthPixels);
    *rst = widthPixels/2;
    return NOERROR;
}

ECode CActionBarPolicy::HasEmbeddedTabs(
        /* [out] */ Boolean* rst)
{
    AutoPtr<IApplicationInfo> appInfo;
    mContext->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 targetSdk;
    appInfo->GetTargetSdkVersion(&targetSdk);
    AutoPtr<IResources> rs;
    mContext->GetResources((IResources**)&rs);
    if(targetSdk >= 16)
    {
        return rs->GetBoolean(R::bool_::action_bar_embed_tabs, rst);
    } else {
        return rs->GetBoolean(R::bool_::action_bar_embed_tabs_pre_jb, rst);
    }
}

ECode CActionBarPolicy::GetTabContainerHeight(
        /* [out] */ Int32* rst)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ActionBar),
            ARRAY_SIZE(R::styleable::ActionBar));
    AutoPtr<ITypedArray> a;
    mContext->ObtainStyledAttributesEx3(NULL, attrIds, R::attr::actionBarStyle, 0, (ITypedArray**)&a);

    Int32 height;
    a->GetLayoutDimensionEx(R::styleable::ActionBar_height, 0, &height);
    Boolean isHas;
    if(HasEmbeddedTabs(&isHas), !isHas)
    {
        AutoPtr<IResources> r;
        mContext->GetResources((IResources**)&r);
        Int32 pixelSize;
        r->GetDimensionPixelSize(R::dimen::action_bar_stacked_max_height, &pixelSize);
        height = (height > pixelSize ? pixelSize : height);
    }
    a->Recycle();
    *rst = height;
    return NOERROR;
}

ECode CActionBarPolicy::EnableHomeButtonByDefault(
        /* [out] */ Boolean* rst)
{
    AutoPtr<IApplicationInfo> appInfo;
    mContext->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 targetSdk;
    appInfo->GetTargetSdkVersion(&targetSdk);
    *rst = (targetSdk < 14);
    return NOERROR;
}

ECode CActionBarPolicy::GetStackedTabMaxWidth(
        /* [out] */ Int32* rst)
{
    AutoPtr<IResources> r;
    mContext->GetResources((IResources**)&r);
    r->GetDimensionPixelSize(R::dimen::action_bar_stacked_tab_max_width, rst);
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos
