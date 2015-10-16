#include "elastos/droid/widget/SeekBar.h"

using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

SeekBar::SeekBar()
{
}

SeekBar::SeekBar(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : AbsSeekBar(ctx, attrs, defStyle)
{
}

ECode SeekBar::Init(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return AbsSeekBar::Init(ctx, attrs, defStyle);
}

ECode SeekBar::SetOnSeekBarChangeListener(
    /* [in] */ ISeekBarOnSeekBarChangeListener* l)
{
    mOnSeekBarChangeListener = l;

    return NOERROR;
}

void SeekBar::OnProgressRefresh(
    /* [in] */ Float scale,
    /* [in] */ Boolean fromUser)
{
    AbsSeekBar::OnProgressRefresh(scale, fromUser);

    if (mOnSeekBarChangeListener != NULL) {
        mOnSeekBarChangeListener->OnProgressChanged(
                THIS_PROBE(ISeekBar), AbsSeekBar::GetProgress(), fromUser);
    }
}

void SeekBar::OnStartTrackingTouch()
{
    AbsSeekBar::OnStartTrackingTouch();

    if (mOnSeekBarChangeListener != NULL) {
        mOnSeekBarChangeListener->OnStartTrackingTouch(THIS_PROBE(ISeekBar));
    }
}

void SeekBar::OnStopTrackingTouch()
{
    AbsSeekBar::OnStopTrackingTouch();

    if (mOnSeekBarChangeListener != NULL) {
        mOnSeekBarChangeListener->OnStopTrackingTouch(THIS_PROBE(ISeekBar));
    }
}

ECode SeekBar::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    AbsSeekBar::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> clsName;
    CStringWrapper::New(String("CSeekBar"), (ICharSequence**)&clsName);
    event->SetClassName(clsName);
    return NOERROR;
}

ECode SeekBar::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    AbsSeekBar::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> clsName;
    CStringWrapper::New(String("CSeekBar"), (ICharSequence**)&clsName);
    info->SetClassName(clsName);
    return NOERROR;
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos


