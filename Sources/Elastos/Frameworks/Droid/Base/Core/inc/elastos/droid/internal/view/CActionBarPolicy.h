#ifndef __ELASTOS_DROID_INTERNAL_VIEW_MENU_CACTIONBARPOLICY_H__
#define __ELASTOS_DROID_INTERNAL_VIEW_MENU_CACTIONBARPOLICY_H__

#include "_Elastos_Droid_Internal_View_Menu_CActionBarPolicy.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

CarClass(CActionBarPolicy)
{
public:
    CActionBarPolicy();

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI GetMaxActionButtons(
        /* [out] */ Int32* rst);

    CARAPI ShowsOverflowMenuButton(
        /* [out] */ Boolean* rst);

    CARAPI GetEmbeddedMenuWidthLimit(
        /* [out] */ Int32* rst);

    CARAPI HasEmbeddedTabs(
        /* [out] */ Boolean* rst);

    CARAPI GetTabContainerHeight(
        /* [out] */ Int32* rst);

    CARAPI EnableHomeButtonByDefault(
        /* [out] */ Boolean* rst);

    CARAPI GetStackedTabMaxWidth(
        /* [out] */ Int32* rst);
private:
    AutoPtr<IContext> mContext;

};

} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif
