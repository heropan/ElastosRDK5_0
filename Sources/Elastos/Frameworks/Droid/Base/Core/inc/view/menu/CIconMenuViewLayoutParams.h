
#ifndef _CICONMENUVIEWLAYOUTPARAMS_H__
#define _CICONMENUVIEWLAYOUTPARAMS_H__

#include "_CIconMenuViewLayoutParams.h"
#include "view/menu/IconMenuViewLayoutParams.h"


namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CIconMenuViewLayoutParams), public IconMenuViewLayoutParams
{
public:
    IVIEWGROUPLP_METHODS_DECL();

    IVIEWGROUPMARGINLP_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI SetLeft(
        /* [in] */ Int32 left);

    CARAPI GetLeft(
        /* [out] */ Int32* left);

    CARAPI SetTop(
        /* [in] */ Int32 top);

    CARAPI GetTop(
        /* [out] */ Int32* top);

    CARAPI SetRight(
        /* [in] */ Int32 right);

    CARAPI GetRight(
        /* [out] */ Int32* right);

    CARAPI SetBottom(
        /* [in] */ Int32 bottom);

    CARAPI GetBottom(
        /* [out] */ Int32* bottom);

    CARAPI SetDesiredWidth(
        /* [in] */ Int32 desiredWidth);

    CARAPI GetDesiredWidth(
        /* [out] */ Int32* desiredWidth);

    CARAPI SetMaxNumItemsOnRow(
        /* [in] */ Int32 maxNumItemsOnRow);

    CARAPI GetMaxNumItemsOnRow(
        /* [out] */ Int32* maxNumItemsOnRow);
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif   //_CICONMENUVIEWLAYOUTPARAMS_H__
