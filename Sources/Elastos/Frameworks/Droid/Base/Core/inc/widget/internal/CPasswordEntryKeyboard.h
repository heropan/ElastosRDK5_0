#ifndef __CPASSWORDENTRYKEYBOARD_H__
#define __CPASSWORDENTRYKEYBOARD_H__

#include "_CPasswordEntryKeyboard.h"
#include "widget/internal/PasswordEntryKeyboard.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CPasswordEntryKeyboard) , public PasswordEntryKeyboard
{
public:
    CPasswordEntryKeyboard();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 xmlLayoutResId);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 xmlLayoutResId,
        /* [in] */ Int32 modeId,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 xmlLayoutResId,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 xmlLayoutResId,
        /* [in] */ Int32 modeId);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 layoutTemplateResId,
        /* [in] */ ICharSequence* characters,
        /* [in] */ Int32 columns,
        /* [in] */ Int32 horizontalPadding);

    CARAPI GetKeys(
        /* [out] */ IObjectContainer** keys);

    CARAPI GetModifierKeys(
        /* [out] */ IObjectContainer** keys);

    CARAPI GetHorizontalGap(
        /* [out] */ Int32* hGap);

    CARAPI SetHorizontalGap(
        /* [in] */ Int32 gap);

    CARAPI GetVerticalGap(
        /* [out] */ Int32* vGap);

    CARAPI SetVerticalGap(
        /* [in] */ Int32 gap);

    CARAPI GetKeyHeight(
        /* [out] */ Int32* h);

    CARAPI SetKeyHeight(
        /* [in] */ Int32 height);

    CARAPI GetKeyWidth(
        /* [out] */ Int32* w);

    CARAPI SetKeyWidth(
        /* [in] */ Int32 width);

    CARAPI GetHeight(
        /* [out] */ Int32* h);

    CARAPI GetMinWidth(
        /* [out] */ Int32* minW);

    CARAPI SetShifted(
        /* [in] */ Boolean shiftState,
        /* [out] */ Boolean* res);

    CARAPI IsShifted(
        /* [out] */ Boolean* res);

    CARAPI GetShiftKeyIndex(
        /* [out] */ Int32* index);

    CARAPI GetShiftKeyIndices(
        /* [out, callee] */ ArrayOf<Int32>** keyIndices);

    CARAPI GetNearestKeys(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [out, callee] */ ArrayOf<Int32>** nearestKeys);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif