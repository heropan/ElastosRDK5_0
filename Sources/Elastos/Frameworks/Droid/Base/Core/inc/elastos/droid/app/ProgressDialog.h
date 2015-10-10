
#ifndef __ELASTOS_DROID_APP_PROGRESSDIALOG_H__
#define __ELASTOS_DROID_APP_PROGRESSDIALOG_H__

#include "elastos/droid/ext/frameworkext.h"
#include "app/AlertDialog.h"
#include "R.h"

using Elastos::Text::INumberFormat;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Widget::IProgressBar;
using Elastos::Droid::Widget::ITextView;

namespace Elastos {
namespace Droid {
namespace App {

class ProgressDialog : public AlertDialog
{
public:
    ProgressDialog();

    ProgressDialog(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme = Elastos::Droid::R::style::Theme_Dialog_Alert);

protected:

    // @Override
    CARAPI_(void) OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme = Elastos::Droid::R::style::Theme_Dialog_Alert);

public:

    // @Override
    CARAPI_(void) OnStart();

    // @Override
    CARAPI_(void) OnStop();

    virtual CARAPI SetProgress(
        /* [in] */ Int32 value);

    virtual CARAPI SetSecondaryProgress(
        /* [in] */ Int32 secondaryProgress);

    virtual CARAPI GetProgress(
        /* [out] */ Int32* progress);

    virtual CARAPI GetSecondaryProgress(
        /* [out] */ Int32* secondaryProgress);

    virtual CARAPI GetMax(
        /* [out] */ Int32* max);

    virtual CARAPI SetMax(
        /* [in] */ Int32 max);

    virtual CARAPI IncrementProgressBy(
        /* [in] */ Int32 diff);

    virtual CARAPI IncrementSecondaryProgressBy(
        /* [in] */ Int32 diff);

    virtual CARAPI SetProgressDrawable(
        /* [in] */ IDrawable* d);

    virtual CARAPI SetIndeterminateDrawable(
        /* [in] */ IDrawable* d);

    virtual CARAPI SetIndeterminate(
        /* [in] */ Boolean indeterminate);

    virtual CARAPI IsIndeterminate(
        /* [out] */ Boolean* isIndeterminate);

    // @Override
    virtual CARAPI SetMessage(
        /* [in] */ ICharSequence* message);

    virtual CARAPI SetProgressStyle(
        /* [in] */ Int32 style);

    virtual CARAPI SetProgressNumberFormat(
        /* [in] */ const String& format);

    virtual CARAPI SetProgressPercentFormat(
        /* [in] */ INumberFormat* format);

private:
    CARAPI_(void) InitFormats();

    CARAPI_(void) OnProgressChanged();

protected:

    AutoPtr<IProgressBar> mProgress;
    AutoPtr<ITextView> mMessageView;

    Int32 mProgressStyle;
    AutoPtr<ITextView> mProgressNumber;
    String mProgressNumberFormat;
    AutoPtr<ITextView> mProgressPercent;
    AutoPtr<INumberFormat> mProgressPercentFormat;

    Int32 mMax;
    Int32 mProgressVal;
    Int32 mSecondaryProgressVal;
    Int32 mIncrementBy;
    Int32 mIncrementSecondaryBy;
    AutoPtr<IDrawable> mProgressDrawable;
    AutoPtr<IDrawable> mIndeterminateDrawable;
    AutoPtr<ICharSequence> mMessage;
    Boolean mIndeterminate;

    Boolean mHasStarted;
    AutoPtr<IHandler> mViewUpdateHandler;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_PROGRESSDIALOG_H__
