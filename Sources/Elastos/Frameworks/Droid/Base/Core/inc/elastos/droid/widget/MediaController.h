
#ifndef __ELASTOS_DROID_WIDGET_MediaController_H__
#define __ELASTOS_DROID_WIDGET_MediaController_H__

#include "elastos/droid/widget/FrameLayout.h"
#include "elastos/droid/os/HandlerBase.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Widget::IMediaPlayerControl;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::Widget::IProgressBar;
using Elastos::Droid::Widget::IImageButton;
using Elastos::Droid::View::IViewOnTouchListener;
using Elastos::Droid::View::IViewOnLayoutChangeListener;
using Elastos::Droid::Widget::ISeekBar;

namespace Elastos{
namespace Droid{
namespace Widget{


class MediaController : public FrameLayout
{
private:

    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ MediaController* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        MediaController* mHost;
    };

    class MediaControllerOnTouchListener
        : public ElRefBase
        , public IViewOnTouchListener
    {
    public:
        MediaControllerOnTouchListener(
            /* [in] */ MediaController* host);

        CAR_INTERFACE_DECL()

        CARAPI OnTouch(
            /* [in] */ IView* v,
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* res);

    private:
        MediaController* host;
    };

    class MediaControllerOnLayoutChangeListener
        : public ElRefBase
        , public IViewOnLayoutChangeListener
    {
    public:
        MediaControllerOnLayoutChangeListener(
            /* [in] */ MediaController* host);

        CAR_INTERFACE_DECL()

        CARAPI OnLayoutChange(
            /* [in] */ IView* v,
            /* [in] */ Int32 left,
            /* [in] */ Int32 top,
            /* [in] */ Int32 right,
            /* [in] */ Int32 bottom,
            /* [in] */ Int32 oldLeft,
            /* [in] */ Int32 oldTop,
            /* [in] */ Int32 oldRight,
            /* [in] */ Int32 oldBottom);

    private:
        MediaController* host;
    };

    class PauseOnClickListener
        : public ElRefBase
        , public IViewOnClickListener
    {
    public:
        PauseOnClickListener(
            /* [in] */ MediaController* host);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        MediaController* host;
    };

    class FfwdOnClickListener
        : public ElRefBase
        , public IViewOnClickListener
    {
    public:
        FfwdOnClickListener(
            /* [in] */ MediaController* host);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        MediaController* host;
    };

    class RewOnClickListener
        : public ElRefBase
        , public IViewOnClickListener
    {
    public:
        RewOnClickListener(
            /* [in] */ MediaController* host);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        MediaController* host;
    };

    class MediaControllerOnSeekBarChangeListener
        : public ElRefBase
        , public ISeekBarOnSeekBarChangeListener
    {
    public:
        MediaControllerOnSeekBarChangeListener(
            /* [in] */ MediaController* host);

        CAR_INTERFACE_DECL()

        CARAPI OnStartTrackingTouch(
            /* [in] */ ISeekBar* seekBar);

        CARAPI OnProgressChanged(
            /* [in] */ ISeekBar* seekBar,
            /* [in] */ Int32 progress,
            /* [in] */ Boolean fromUser);

        CARAPI OnStopTrackingTouch(
            /* [in] */ ISeekBar* seekBar);

    private:
        MediaController* host;
    };

public:
    MediaController(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    MediaController(
        /* [in] */ IContext* context,
        /* [in] */ Boolean useFastForWard = TRUE);

    ~MediaController();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Boolean useFastForWard = TRUE);

    CARAPI OnFinishInflate();

    CARAPI SetMediaPlayer(
        /* [in] */ IMediaPlayerControl* player);

    CARAPI SetAnchorView(
        /* [in] */ IView* view);

    CARAPI Show();

    CARAPI Show(
        /* [in] */ Int32 timeout);

    CARAPI_(Boolean) IsShowing();

    CARAPI Hide();

    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    CARAPI SetPrevNextListeners(
        /* [in] */ IViewOnClickListener* next,
        /* [in] */ IViewOnClickListener* prev);

protected:
    MediaController();

    CARAPI_(AutoPtr<IView>) MakeControllerView();

private:
    CARAPI InitFloatingWindow();

    CARAPI InitFloatingWindowLayout();

    CARAPI UpdateFloatingWindowLayout();

    CARAPI InitControllerView(
        /* [in] */ IView* view);

    CARAPI DisableUnsupportedButtons();

    CARAPI_(String) StringForTime(
        /* [in] */ Int32 timeMs);

    CARAPI_(Int32) SetProgress();

    CARAPI UpdatePausePlay();

    CARAPI DoPauseResume();

    CARAPI InstallPrevNextListeners();

    CARAPI HandleShowProgress();

private:
    AutoPtr<IMediaPlayerControl> mPlayer;
    AutoPtr<IContext> mContext;
    AutoPtr<IView> mAnchor;
    AutoPtr<IView> mRoot;
    AutoPtr<IWindowManager> mWindowManager;
    AutoPtr<IWindow> mWindow;
    AutoPtr<IView> mDecor;
    AutoPtr<IWindowManagerLayoutParams> mDecorLayoutParams;

    AutoPtr<IProgressBar> mProgress;
    AutoPtr<ITextView> mEndTime;
    AutoPtr<ITextView> mCurrentTime;
    Boolean mShowing;
    Boolean mDragging;

    static const Int32 sDefaultTimeout = 3000;
    static const Int32 FADE_OUT = 1;
    static const Int32 SHOW_PROGRESS = 2;
    const static String MEDIACONTROLLER_NAME;

    Boolean mUseFastForward;
    Boolean mFromXml;
    Boolean mListenersSet;
    AutoPtr<IViewOnClickListener> mNextListener;
    AutoPtr<IViewOnClickListener> mPrevListener;
    AutoPtr<StringBuilder> mFormatBuilder;
    //AutoPtr<IFormatter> mFormatter;

    AutoPtr<IImageButton> mPauseButton;
    AutoPtr<IImageButton> mFfwdButton;
    AutoPtr<IImageButton> mRewButton;
    AutoPtr<IImageButton> mNextButton;
    AutoPtr<IImageButton> mPrevButton;
    AutoPtr<IViewOnLayoutChangeListener> mLayoutChangeListener;
    AutoPtr<IViewOnTouchListener> mTouchListener;
    AutoPtr<IHandler> mHandler;
    AutoPtr<IViewOnClickListener> mPauseListener;
    AutoPtr<ISeekBarOnSeekBarChangeListener> mSeekListener;
    AutoPtr<IViewOnClickListener> mRewListener;
    AutoPtr<IViewOnClickListener> mFfwdListener;

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_MediaController_H__
