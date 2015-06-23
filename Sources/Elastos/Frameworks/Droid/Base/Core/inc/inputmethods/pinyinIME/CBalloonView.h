
#ifndef  __CBALLOONVIEW_H__
#define  __CBALLOONVIEW_H__

#include "_CBalloonView.h"


namespace Elastos {
namespace Droid {
namespace Inputmethods {
namespace PinyinIME {

class BalloonView: public Elastos::Droid::View::View
{
public:
    BalloonView();

protected:
    void OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    void OnDraw(
        /* [in] */ ICanvas* canvas);

    String GetLimitedLabelForDrawing(
        /* [in] */ const String& rawLabel,
        /* [in] */ Float widthToDraw);

protected:
    /**
     * Suspension points used to display long items.
     */
    static String SUSPENSION_POINTS;

    /**
     * The icon to be shown. If it is not NULL, {@link #mLabel} will be
     * ignored.
     */
    AutoPtr<IDrawable> mIcon;

    /**
     * The label to be shown. It is enabled only if {@link #mIcon} is NULL.
     */
    String mLabel;

    Int32 mLabeColor;
    AutoPtr<IPaint> mPaintLabel;
    AutoPtr<IPaintFontMetricsInt> mFmi;

    /**
     * The width to show suspension points.
     */
    Float mSuspensionPointsWidth;
};

CarClass(CBalloonView), public BalloonView
{
public:
    IVIEW_METHODS_DECL();
    IDRAWABLECALLBACK_METHODS_DECL();
    IKEYEVENTCALLBACK_METHODS_DECL();
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetTextConfig(
        /* [in] */ const String& label,
        /* [in] */ Float fontSize,
        /* [in] */ Boolean textBold,
        /* [in] */ Int32 textColor);
};

} // namespace PinyinIME
} // namespace Inputmethods
} // namespace Droid
} // namespace Elastos

#endif  //__CBALLOONVIEW_H__
