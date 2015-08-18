
#ifndef __ELASTOS_DROID_WEBKIT_CWEBVIEWCLASSIC_H__
#define __ELASTOS_DROID_WEBKIT_CWEBVIEWCLASSIC_H__

#include "_Elastos_Droid_Webkit_CWebViewClassic.h"
#include "content/BroadcastReceiver.h"
#include "database/DataSetObserver.h"
#include "graphics/drawable/Drawable.h"

#include "os/HandlerBase.h"
#include "os/Runnable.h"
#include "os/AsyncTask.h"
#include "webkit/AccessibilityInjector.h"
#include "webkit/AutoCompletePopup.h"
#include "webkit/CallbackProxy.h"
#include "webkit/WebViewDatabaseClassic.h"
#include "webkit/FindActionModeCallback.h"
#include "webkit/HTML5VideoViewProxy.h"
#include "webkit/OverScrollGlow.h"
#include "webkit/PluginFullScreenHolder.h"
#include "webkit/QuadF.h"
#include "webkit/SelectActionModeCallback.h"
#include "webkit/WebView.h"
#include "webkit/WebViewFactoryProvider.h"
#include "webkit/WebViewInputDispatcher.h"
#include "webkit/ZoomManager.h"
#include "webkit/ViewManager.h"
#include "widget/OverScroller.h"
#include "widget/PopupWindow.h"
#include "widget/ArrayAdapter.h"
#include "view/inputmethod/BaseInputConnection.h"

#include "view/VelocityTracker.h"
#include "view/View.h"
#include <elastos/utility/etl/Vector.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::IInteger32;
using Elastos::IO::IFile;
using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;
using Elastos::IO::IFileInputStream;
using Elastos::Utility::IMap;
using Elastos::Utility::Etl::Vector;
using Elastos::Utility::ISet;
using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IComponentCallbacks2;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::IDialogInterfaceOnCancelListener;
using Elastos::Droid::Content::IDialogInterfaceOnDismissListener;
using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Droid::Database::DataSetObserver;
using Elastos::Droid::Graphics::IColorFilter;
using Elastos::Droid::Graphics::IDrawFilter;
using Elastos::Droid::Graphics::IRegion;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::IPicture;
using Elastos::Droid::Graphics::IPoint;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::Drawable::Drawable;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Net::Http::ISslCertificate;
using Elastos::Droid::Os::IPowerManagerWakeLock;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::AsyncTask;
using Elastos::Droid::View::IKeyCharacterMap;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::VelocityTracker;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::View;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::View::InputMethod::BaseInputConnection;
using Elastos::Droid::View::InputMethod::IBaseInputConnection;
using Elastos::Droid::View::InputMethod::ICompletionInfo;
using Elastos::Droid::View::InputMethod::ICorrectionInfo;
using Elastos::Droid::View::InputMethod::IExtractedTextRequest;
using Elastos::Droid::View::InputMethod::IExtractedText;
using Elastos::Droid::Widget::IAdapter;
using Elastos::Droid::Widget::ArrayAdapter;
using Elastos::Droid::Widget::IArrayAdapter;
using Elastos::Droid::Widget::IListView;
using Elastos::Droid::Widget::IScroller;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Widget::PopupWindow;
using Elastos::Droid::Widget::IPopupWindow;
using Elastos::Droid::Widget::OverScroller;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;

#ifdef __cplusplus
extern "C"
{
#endif
    extern void Elastos_WebView_nativeCreate(Int32 callback, AutoPtr<IWeakReference> obj,
              Int32 viewImpl, const Elastos::String& drawableDir, Boolean isHighEndGfx);
    extern void Elastos_WebView_nativeDestroy(Int32 ptr);
    extern void Elastos_WebView_nativeDraw(Int32 nativeClass,Int32 canvas,
              Float left,Float top,Float right,Float bottom,Int32 color,Int32 extras);
    extern Int32 Elastos_WebView_nativeCreateDrawGLFunction(Int32 nativeView,
              Int32 jinvscreenrect[], Int32 jscreenrect[], Float jvisiblecontentrect[], Float scale, Int32 extras);
    extern Int32 Elastos_WebView_nativeGetDrawGLFunction(Int32 nativeView);
    extern void Elastos_WebView_nativeUpdateDrawGLFunction(Int32 nativeView,
              Int32 jinvscreenrect[], Int32 jscreenrect[], Float jvisiblecontentrect[], Float scale);
    extern void Elastos_WebView_nativeDumpDisplayTree(Int32 nativeClass, const Elastos::String& jurl);
    extern Boolean Elastos_WebView_nativeEvaluateLayersAnimations(Int32 nativeView);
    extern Elastos::String Elastos_WebView_nativeGetSelection(Int32 ptr);
    extern void Elastos_WebView_nativeSetHeightCanMeasure(Int32 ptr, Boolean measure);
    extern Boolean Elastos_WebView_nativeSetBaseLayer(Int32 nativeView,
              Int32 layer, Boolean showVisualIndicator, Boolean isPictureAfterFirstLayout, Int32 scrollingLayer);
    extern Int32 Elastos_WebView_nativeGetBaseLayer(Int32 nativeView);
    extern void Elastos_WebView_nativeCopyBaseContentToPicture(Int32 nativeView, Int32 nativePicture);
    extern Boolean Elastos_WebView_nativeDumpLayerContentToPicture(Int32 instance,
              const Elastos::String& jclassName, Int32 layerId, Int32 nativePicture);
    extern Boolean Elastos_WebView_nativeHasContent(Int32 ptr);
    extern void Elastos_WebView_nativeDiscardAllTextures();
    extern void Elastos_WebView_nativeTileProfilingStart();
    extern Float Elastos_WebView_nativeTileProfilingStop();
    extern void Elastos_WebView_nativeTileProfilingClear();
    extern Int32 Elastos_WebView_nativeTileProfilingNumFrames();
    extern Int32 Elastos_WebView_nativeTileProfilingNumTilesInFrame(Int32 frame);
    extern Int32 Elastos_WebView_nativeTileProfilingGetInt(Int32 frame, Int32 tile, const Elastos::String& jkey);
    extern Float Elastos_WebView_nativeTileProfilingGetFloat(Int32 frame, Int32 tile, const Elastos::String& jkey);
    extern void Elastos_WebView_nativeStopGL(Int32 ptr);
    extern Int32 Elastos_WebView_nativeScrollableLayer(Int32 nativeView, Int32 x, Int32 y, Int32 rect[], Int32 bounds[]);
    extern Boolean Elastos_WebView_nativeScrollLayer(Int32 nativeView, Int32 layerId, Int32 x, Int32 y);
    extern void Elastos_WebView_nativeSetIsScrolling(Int32 nativeView, Boolean isScrolling);
    extern void Elastos_WebView_nativeUseHardwareAccelSkia(Boolean enabled);
    extern Int32 Elastos_WebView_nativeGetBackgroundColor(Int32 nativeView);
    extern Boolean Elastos_WebView_nativeSetProperty(const Elastos::String& jkey, const Elastos::String& jvalue);
    extern Elastos::String Elastos_WebView_nativeGetProperty(const Elastos::String& jkey);
    extern void Elastos_WebView_nativeOnTrimMemory(Int32 level);
    extern void Elastos_WebView_nativeSetPauseDrawing(Int32 nativeView, Boolean pause);
    extern void Elastos_WebView_nativeSetTextSelection(Int32 nativeView, Int32 selectionPtr);
    extern Int32 Elastos_WebView_nativeGetHandleLayerId(Int32 nativeView, Int32 handleIndex,
              Int32 cursorPoint[], IInterface* textQuad);
    extern void Elastos_WebView_nativeMapLayerRect(Int32 nativeView, Int32 layerId, Int32 rect[]);
    extern Int32 Elastos_WebView_nativeSetHwAccelerated(Int32 nativeView, Boolean hwAccelerated);
    extern void Elastos_WebView_nativeFindMaxVisibleRect(Int32 nativeView, Int32 movingLayerId, Int32 visibleContentRect[]);
    extern Boolean Elastos_WebView_nativeIsHandleLeft(Int32 nativeView, Int32 handleId);
    extern Boolean Elastos_WebView_nativeIsPointVisible(Int32 nativeView, Int32 layerId, Int32 contentX, Int32 contentY);
#ifdef __cplusplus
}
#endif


namespace Elastos {
namespace Droid {
namespace Webkit {

struct ElaWebViewCallback
{
    void (*setNativeClass)(IInterface* obj, Int32);
    Int32 (*getNativeClass)(IInterface* obj);
    Boolean (*scrollBy)(IInterface* obj, Int32, Int32, Boolean);
    Int32 (*getScaledMaxXScroll)(IInterface* obj);
    Int32 (*getScaledMaxYScroll)(IInterface* obj);
    void (*updateRectsForGL)(IInterface* obj);
    void (*viewInvalidate)(IInterface* obj);
    void (*viewInvalidateRect)(IInterface* obj, Int32, Int32, Int32, Int32);
    void (*viewInvalidateDelayed)(IInterface* obj, Int64, Int32, Int32, Int32, Int32);
    void (*pageSwapCallback)(IInterface* obj, Boolean);
    void (*setQuadFP1)(IInterface*, Float, Float);
    void (*setQuadFP2)(IInterface*, Float, Float);
    void (*setQuadFP3)(IInterface*, Float, Float);
    void (*setQuadFP4)(IInterface*, Float, Float);

};

class CWebViewCore;
class CWebViewCoreAutoFillData;
class CWebViewCoreTextSelectionData;
class CWebViewCoreTextFieldInitData;
class CWebViewCoreFindAllRequest;
class CWebViewCoreWebKitHitTest;
class WebSettingsClassic;

/**
 * InputConnection used for ContentEditable. This captures changes
 * to the text and sends them either as key strokes or text changes.
 */
class _CWebViewClassicWebViewInputConnection
    : public ElRefBase
    , public BaseInputConnection
{
public:
    _CWebViewClassicWebViewInputConnection(
        /* [in] */ CWebViewClassic* owner);

    CAR_INTERFACE_DECL()

    CARAPI_(void) SetAutoFillable(
        /* [in] */ Int32 queryId);

    CARAPI_(Boolean) BeginBatchEdit();

    CARAPI_(Boolean) EndBatchEdit();

    CARAPI_(Boolean) GetIsAutoFillable();

    CARAPI_(Boolean) SendKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) SetTextAndKeepSelection(
        /* [in] */ ICharSequence* text);

    CARAPI_(void) ReplaceSelection(
        /* [in] */ ICharSequence* text);

    CARAPI_(Boolean) SetComposingText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

    CARAPI_(Boolean) CommitText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

    CARAPI_(Boolean) DeleteSurroundingText(
        /* [in] */ Int32 leftLength,
        /* [in] */ Int32 rightLength);

    CARAPI_(Boolean) PerformEditorAction(
        /* [in] */ Int32 editorAction);

    CARAPI_(void) InitEditorInfo(
        /* [in] */ CWebViewCoreTextFieldInitData* initData);

    CARAPI_(void) SetupEditorInfo(
        /* [in] */ IEditorInfo* outAttrs);

    CARAPI_(Boolean) SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(Boolean) SetComposingRegion(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

private:
    /**
     * Send the selection and composing spans to the IME.
     */
    CARAPI_(void) UpdateSelection();

    /**
     * Sends a text change to webkit indirectly. If it is a single-
     * character add or delete, it sends it as a key stroke. If it cannot
     * be represented as a key stroke, it sends it as a field change.
     * @param start The start offset (inclusive) of the text being changed.
     * @param end The end offset (exclusive) of the text being changed.
     * @param text The new text to replace the changed text.
     */
    CARAPI_(void) SetNewText(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ICharSequence* text);

    /**
     * Send a single character to the WebView as a key down and up event.
     * @param c The character to be sent.
     */
    CARAPI_(void) SendCharacter(
        /* [in] */ Char32 c);

    /**
     * Send a key event for a specific key code, not a standard
     * unicode character.
     * @param keyCode The key code to send.
     */
    CARAPI_(void) SendKey(
        /* [in] */ Int32 keyCode);

    CARAPI_(AutoPtr<ICharSequence>) LimitReplaceTextByMaxLength(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 numReplaced);

    CARAPI_(void) RestartInput();

private:
    // Used for mapping characters to keys typed.
    AutoPtr<IKeyCharacterMap> mKeyCharacterMap;
    Boolean mIsKeySentByMe;
    Int32 mInputType;
    Int32 mImeOptions;
    String mHint;
    Int32 mMaxLength;
    Boolean mIsAutoFillable;
    Boolean mIsAutoCompleteEnabled;
    String mName;
    Int32 mBatchLevel;
    CWebViewClassic* mOwner;
};

class CWebViewClassicWebViewInputConnection
    : public _CWebViewClassicWebViewInputConnection
    , public IBaseInputConnection
    , public IWeakReferenceSource
{
public:
    CWebViewClassicWebViewInputConnection(
        /* [in] */ CWebViewClassic* owner);

    CAR_INTERFACE_DECL();

    BASEINPUTCONNECTION_METHODS_DECL();

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);
};

/**
 * Implements a backend provider for the {@link WebView} public API.
 * @hide
 */
// TODO: Check if any WebView published API methods are called from within here, and if so
// we should bounce the call out via the proxy to enable any sub-class to override it.
CarClass(CWebViewClassic)
{
    friend class _CWebViewClassicWebViewInputConnection;

public:
    /**
     * Refer to {@link WebView#requestFocusNodeHref(Message)} for more information
     */
    class FocusNodeHref
    {
    public:
        static const String TITLE;
        static const String URL;
        static const String SRC;
    };

    // WebViewProvider bindings
    class Factory
        : public ElLightRefBase
        , public IWebViewFactoryProvider
        , public IWebViewFactoryProvider::IStatics
    {
    public:
        CAR_INTERFACE_DECL();

        CARAPI FindAddress(
            /* [in] */ const String& addr,
            /* [out] */ String* addrOut);

        CARAPI SetPlatformNotificationsEnabled(
            /* [in] */ Boolean enable);

        CARAPI GetStatics(
            /* [out] */ IWebViewFactoryProvider::IStatics** statics);

        CARAPI CreateWebView(
            /* [in] */ IWebView* webView,
            /* [in] */ IWebViewPrivateAccess* privateAccess,
            /* [out] */ IWebViewProvider** provider);

        CARAPI GetGeolocationPermissions(
            /* [out] */ IGeolocationPermissions** geolocationPermissions);

        CARAPI GetCookieManager(
            /* [out] */ ICookieManager** cookieManager);

        CARAPI GetWebIconDatabase(
            /* [out] */ IWebIconDatabase** database);

        CARAPI GetWebStorage(
            /* [out] */ IWebStorage** webStorage);

        CARAPI GetWebViewDatabase(
            /* [in] */ IContext* context,
            /* [out] */ IWebViewDatabase** database);

        CARAPI GetDefaultUserAgent(
            /* [in] */ IContext* context,
            /* [out] */ String* userAgent);
    };

    /* package */
    class SaveWebArchiveMessage
        : public ElLightRefBase
        , public IInterface
    {
    public:
        SaveWebArchiveMessage(
            /* [in] */ const String& basename,
            /* [in] */ Boolean autoname,
            /* [in] */ IValueCallback* callBack)
            : mBasename(basename)
            , mAutoname(autoname)
            , mCallback(callBack)
        {}

        CAR_INTERFACE_DECL()

    public:
        /* package */ String mBasename;
        /* package */ Boolean mAutoname;
        /* package */ AutoPtr<IValueCallback> mCallback;
        /* package */ String mResultFile;
    };

    class ViewSizeData
        : public ElLightRefBase
        , public IInterface
    {
    public:
        ViewSizeData()
            : mWidth(0)
            , mHeight(0)
            , mHeightWidthRatio(0)
            , mActualViewHeight(0)
            , mTextWrapWidth(0)
            , mAnchorX(0)
            , mAnchorY(0)
            , mScale(0)
            , mIgnoreHeight(FALSE)
        {}

        CAR_INTERFACE_DECL()

    public:
        Int32 mWidth;
        Int32 mHeight;
        Float mHeightWidthRatio;
        Int32 mActualViewHeight;
        Int32 mTextWrapWidth;
        Int32 mAnchorX;
        Int32 mAnchorY;
        Float mScale;
        Boolean mIgnoreHeight;
    };

    /**
     * General handler to receive message coming from webkit thread
     */
    class PrivateHandler
        : public HandlerBase
        , public WebViewInputDispatcher::IUiCallbacks
    {
    public:
        PrivateHandler(
            /* [in] */ CWebViewClassic* host)
            : mHost(host)
        {}

        CAR_INTERFACE_DECL();

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI GetUiLooper(
            /* [out] */ ILooper** looper);

        CARAPI DispatchUiEvent(
            /* [in] */ IMotionEvent* event,
            /* [in] */ Int32 eventType,
            /* [in] */ Int32 flags);

        CARAPI GetContext(
            /* [out] */ IContext** context);

        CARAPI ShouldInterceptTouchEvent(
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        CARAPI ShowTapHighlight(
            /* [in] */ Boolean show);

        CARAPI ClearPreviousHitTest();

    private:
        CWebViewClassic* mHost;
    };

    class _FocusTransitionDrawable
        : public Elastos::Droid::Graphics::Drawable::Drawable
    {
    public:
        _FocusTransitionDrawable(
            /* [in] */ CWebViewClassic* view);

        CARAPI SetColorFilter(
            /* [in] */ IColorFilter* cf);

        CARAPI SetAlpha(
            /* [in] */ Int32 alpha);

        CARAPI_(Int32) GetOpacity();

        CARAPI_(void) SetProgress(
            /* [in] */ Float p);

        CARAPI_(Float) GetProgress();

        CARAPI Draw(
             /* [in] */ ICanvas* canvas);

    public:
        AutoPtr<IRegion> mPreviousRegion;
        AutoPtr<IRegion> mNewRegion;
        Float mProgress;
        CWebViewClassic* mWebView;
        AutoPtr<IPaint> mPaint;
        Int32 mMaxAlpha;
        AutoPtr<IPoint> mTranslate;
    };

    class FocusTransitionDrawable
        : public ElLightRefBase
        , public _FocusTransitionDrawable
        , public IDrawable
    {
    public:
        FocusTransitionDrawable(
            /* [in] */ CWebViewClassic* view)
            : _FocusTransitionDrawable(view)
        {}

        CAR_INTERFACE_DECL();

        IDRAWABLE_METHODS_DECL();
    };

private:
    class PastePopupWindow
        : public ElLightRefBase
        , public PopupWindow
        , public IPopupWindow
        , public IViewOnClickListener
    {
    public:
        PastePopupWindow(
            /* [in] */ CWebViewClassic* owner);

        CAR_INTERFACE_DECL();

        CARAPI GetBackground(
            /* [out] */ IDrawable** background);

        CARAPI SetBackgroundDrawable(
            /* [in] */ IDrawable* background);

        CARAPI GetAnimationStyle(
            /* [out] */ Int32* style);

        CARAPI SetIgnoreCheekPress();

        CARAPI SetAnimationStyle(
            /* [in] */ Int32 animationStyle);

        CARAPI GetContentView(
            /* [out] */ IView** contentView);

        CARAPI SetContentView(
            /* [in] */ IView* contentView);

        CARAPI SetTouchInterceptor(
            /* [in] */ IViewOnTouchListener* l);

        CARAPI IsFocusable(
            /* [out] */ Boolean* isFocusable);

        CARAPI  SetFocusable(
             /* [in] */ Boolean focusable);

        CARAPI GetInputMethodMode(
            /* [out] */ Int32* inputMethodMode);

        CARAPI SetInputMethodMode(
            /* [in] */ Int32 mode);

        CARAPI SetSoftInputMode(
            /* [in] */ Int32 mode);

        CARAPI GetSoftInputMode(
            /* [out] */ Int32* softInputMode);

        CARAPI IsTouchable(
            /* [out] */ Boolean* touchable);

        CARAPI SetTouchable(
            /* [in] */ Boolean touchable);

        CARAPI IsOutsideTouchable(
            /* [out] */ Boolean* touchable);

        CARAPI SetOutsideTouchable(
            /* [in] */ Boolean touchable);

        CARAPI IsClippingEnabled(
            /* [out] */ Boolean* enabled);

        CARAPI SetClippingEnabled(
            /* [in] */ Boolean enabled);

        CARAPI SetClipToScreenEnabled(
            /* [in] */ Boolean enabled);

        CARAPI IsSplitTouchEnabled(
            /* [out] */ Boolean* enabled);

        CARAPI SetSplitTouchEnabled(
            /* [in] */ Boolean enabled);

        CARAPI IsLayoutInScreenEnabled(
            /* [out] */ Boolean* enabled);

        CARAPI SetLayoutInScreenEnabled(
            /* [in] */ Boolean enabled);

        CARAPI SetLayoutInsetDecor(
            /* [in] */ Boolean enabled);

        CARAPI SetWindowLayoutType(
            /* [in] */ Int32 layoutType);

        CARAPI GetWindowLayoutType(
            /* [out] */ Int32* layoutType);

        CARAPI SetTouchModal(
            /* [in] */ Boolean touchModal);

        CARAPI SetWindowLayoutMode(
            /* [in] */ Int32 widthSpec,
            /* [in] */ Int32 heightSpec);

        CARAPI GetHeight(
            /* [out] */ Int32* height);

        CARAPI SetHeight(
            /* [in] */ Int32 height);

        CARAPI GetWidth(
            /* [out] */ Int32* width);

        CARAPI SetWidth(
            /* [in] */ Int32 width);

        CARAPI IsShowing(
            /* [out] */ Boolean* isShowing);

        CARAPI ShowAtLocation(
            /* [in] */ IView* parent,
            /* [in] */ Int32 gravity,
            /* [in] */ Int32 x,
            /* [in] */ Int32 y);

        CARAPI ShowAtLocation(
            /* [in] */ IBinder* token,
            /* [in] */ Int32 gravity,
            /* [in] */ Int32 x,
            /* [in] */ Int32 y);

        CARAPI ShowAsDropDown(
            /* [in] */ IView* anchor);

        CARAPI ShowAsDropDown(
            /* [in] */ IView* anchor,
            /* [in] */ Int32 xoff,
            /* [in] */ Int32 yoff);

        CARAPI IsAboveAnchor(
            /* [out] */ Boolean* isAboveAnchor);

        CARAPI GetMaxAvailableHeight(
            /* [in] */ IView* anchor,
            /* [out] */ Int32* maxAvailableHeight);

        CARAPI GetMaxAvailableHeight(
            /* [in] */ IView* anchor,
            /* [in] */ Int32 yOffset,
            /* [out] */ Int32* maxAvailableHeight);

        CARAPI GetMaxAvailableHeight(
            /* [in] */ IView* anchor,
            /* [in] */ Int32 yOffset,
            /* [in] */ Boolean ignoreBottomDecorations,
            /* [out] */ Int32* maxAvailableHeight);

        CARAPI Dismiss();

        CARAPI SetOnDismissListener(
            /* [in] */ IPopupWindowOnDismissListener* l);

        CARAPI Update();

        CARAPI Update(
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI Update(
            /* [in] */ Int32 x,
            /* [in] */ Int32 y,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI Update(
            /* [in] */ Int32 x,
            /* [in] */ Int32 y,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height,
            /* [in] */ Boolean force);

        CARAPI Update(
            /* [in] */ IView* anchor,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI Update(
            /* [in] */ IView* anchor,
            /* [in] */ Int32 xoff,
            /* [in] */ Int32 yoff,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI SetAllowScrollingAnchorParent(
            /* [in] */ Boolean enabled);

        CARAPI_(void) Show(
            /* [in] */ IPoint* cursorBottom,
            /* [in] */ IPoint* cursorTop,
            /* [in] */ Int32 windowLeft,
            /* [in] */ Int32 windowTop);

        CARAPI_(void) Hide();

        CARAPI OnClick(
            /* [in] */ IView* view);

    protected:
        CARAPI_(void) MeasureContent();

    private:
        AutoPtr<IViewGroup> mContentView;
        AutoPtr<ITextView> mPasteTextView;
        CWebViewClassic* mOwner;
    };

    class OnTrimMemoryListener
        : public ElLightRefBase
        , public IComponentCallbacks2
    {
    public:
        CAR_INTERFACE_DECL();

        static CARAPI_(void) Init(
            /* [in] */ IContext* c);

        CARAPI OnConfigurationChanged(
            /* [in] */ IConfiguration* newConfig);

        CARAPI OnLowMemory();

        CARAPI OnTrimMemory(
            /* [in] */ Int32 level);

    private:
        OnTrimMemoryListener(
            /* [in] */ IContext* c);

    private:
        static AutoPtr<OnTrimMemoryListener> sInstance;
    };

    class TrustStorageListener : public BroadcastReceiver
    {
    public:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CWebViewClassic::TrustStorageListener: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    };

    class ProxyReceiver : public BroadcastReceiver
    {
    public:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CWebViewClassic::ProxyReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    };

    class PackageListener : public BroadcastReceiver
    {
    public:
        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CWebViewClassic::PackageListener: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);
    };

    class DestroyNativeRunnable : public Runnable
    {
    public:
        DestroyNativeRunnable(
            /* [in] */ Int32 nativePtr)
            : mNativePtr(nativePtr)
        {}

        CARAPI Run();

    private:
        Int32 mNativePtr;
    };

    class SelectionHandleAlpha
        : public ElLightRefBase
        , public IInterface
    {
    public:
        SelectionHandleAlpha(
            /* [in] */ CWebViewClassic* host)
            : mAlpha(0)
            , mTargetAlpha(0)
            , mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI_(void) SetAlpha(
            /* [in] */ Int32 alpha);

        CARAPI_(Int32) GetAlpha();

        CARAPI_(void) SetTargetAlpha(
            /* [in] */ Int32 alpha);

        CARAPI_(Int32) GetTargetAlpha();

    private:
        Int32 mAlpha;
        Int32 mTargetAlpha;
        CWebViewClassic* mHost;
    };

    /*
     * This class requests an Adapter for the AutoCompletePopup which shows past
     * entries stored in the database.  It is a Runnable so that it can be done
     * in its own thread, without slowing down the UI.
     */
    class _RequestFormData : public Runnable
    {
    public:
        _RequestFormData(
            /* [in] */ const String& name,
            /* [in] */ const String& url,
            /* [in] */ IMessage* msg,
            /* [in] */ Boolean autoFillable,
            /* [in] */ Boolean autoComplete,
            /* [in] */ CWebViewClassic* host);

        CARAPI Run();

    private:
        String mName;
        String mUrl;
        AutoPtr<IMessage> mUpdateMessage;
        Boolean mAutoFillable;
        Boolean mAutoComplete;
        AutoPtr<WebSettingsClassic> mWebSettings;
        CWebViewClassic* mHost;
    };

    // Class used to use a dropdown for a <select> element
    class InvokeListBox : public Runnable
    {
        friend class CWebViewClassic;

    private:
        class Container;

    public:
        /**
         *  Subclass ArrayAdapter so we can disable OptionGroupLabels,
         *  and allow filtering.
         */
        class MyArrayListAdapter
            : public ElLightRefBase
            , public ArrayAdapter
            , public IArrayAdapter
        {
        public:
            MyArrayListAdapter(
                /* [in] */ InvokeListBox* host);

            CAR_INTERFACE_DECL()

            CARAPI GetView(
                /* [in] */ Int32 position,
                /* [in] */ IView* convertView,
                /* [in] */ IViewGroup* parent,
                /* [out] */ IView** view);

            CARAPI HasStableIds(
                /* [out] */ Boolean* result);

            CARAPI GetItemId(
                /* [in] */ Int32 position,
                /* [out] */ Int64* id);

            CARAPI AreAllItemsEnabled(
                /* [out] */ Boolean* enabled);

            CARAPI IsEnabled(
                /* [in] */ Int32 position,
                /* [out] */ Boolean* enabled);

            CARAPI RegisterDataSetObserver(
                /* [in] */ IDataSetObserver* observer);

            CARAPI UnregisterDataSetObserver(
                /* [in] */ IDataSetObserver* observer);

            CARAPI GetCount(
                /* [out] */ Int32* count);

            CARAPI GetItem(
                /* [in] */ Int32 position,
                /* [out] */ IInterface** item);

            CARAPI GetItemViewType(
                /* [in] */ Int32 position,
                /* [out] */ Int32* viewType);

            CARAPI GetViewTypeCount(
                /* [out] */ Int32* count);

            CARAPI IsEmpty(
                /* [out] */ Boolean* isEmpty);

            CARAPI NotifyDataSetChanged();

            CARAPI NotifyDataSetInvalidated();

            CARAPI Add(
                /* [in] */ IInterface* object);

            CARAPI AddAll(
                /* [in] */ IObjectContainer* collection);

            CARAPI AddAll(
                /* [in] */ ArrayOf<IInterface* >* items);

            CARAPI Insert(
                /* [in] */ IInterface* object,
                /* [in] */ Int32 index);

            CARAPI Remove(
                /* [in] */ IInterface* object);

            CARAPI Clear();

            CARAPI Sort(
                /* [in] */ IComparator* comparator);

            CARAPI SetNotifyOnChange(
                /* [in] */ Boolean notifyOnChange);

            CARAPI GetContext(
                /* [out] */ IContext** context);

            CARAPI GetPosition(
                /* [in] */ IInterface* item,
                /* [out] */ Int32* position);

            CARAPI SetDropDownViewResource(
                /* [in] */ Int32 resource);

        private:
            CARAPI_(AutoPtr<Container>) Item(
                /* [in] */ Int32 position);

        private:
            AutoPtr<InvokeListBox> mHost;
        };

    private:
        // Need these to provide stable ids to my ArrayAdapter,
        // which normally does not have stable ids. (Bug 1250098)
        class Container
            : public ElLightRefBase
            , public IInterface
        {
        public:
            Container()
                : mEnabled(0)
                , mId(0)
            {}

            CAR_INTERFACE_DECL()

            CARAPI_(String) ToString()
            {
                return mString;
            }

        public:
            /**
             * Possible values for mEnabled.  Keep in sync with OptionStatus in
             * WebViewCore.cpp
             */
            const static Int32 OPTGROUP = -1;
            const static Int32 OPTION_DISABLED = 0;
            const static Int32 OPTION_ENABLED = 1;

            String mString;
            Int32 mEnabled;
            Int32 mId;
        };

        /*
         * Whenever the data set changes due to filtering, this class ensures
         * that the checked item remains checked.
         */
        class SingleDataSetObserver
            : public ElLightRefBase
            , public IDataSetObserver
            , public DataSetObserver
        {
        public:
            /*
             * Create a new observer.
             * @param id The ID of the item to keep checked.
             * @param l ListView for getting and clearing the checked states
             * @param a Adapter for getting the IDs
             */
            SingleDataSetObserver(
                /* [in] */ Int64 id,
                /* [in] */ IListView* l,
                /* [in] */ IAdapter* a,
                /* [in] */ InvokeListBox* host)
               : mCheckedId(id)
               , mListView(l)
               , mAdapter(a)
               , mHost(host)
            {}

            CAR_INTERFACE_DECL();

            CARAPI OnChanged();

            CARAPI OnInvalidated();

        private:
            Int64 mCheckedId;
            AutoPtr<IListView> mListView;
            AutoPtr<IAdapter> mAdapter;
            AutoPtr<InvokeListBox> mHost;
        };

    public:
        CARAPI Run();

    private:
        InvokeListBox(
            /* [in] */ ArrayOf<String>* array,
            /* [in] */ ArrayOf<Int32>* enabled,
            /* [in] */ ArrayOf<Int32>* selected,
            /* [in] */ CWebViewClassic* host);

        InvokeListBox(
            /* [in] */ ArrayOf<String>* array,
            /* [in] */ ArrayOf<Int32>* enabled,
            /* [in] */ Int32 selection,
            /* [in] */ CWebViewClassic* host);

    private:
        // Whether the listbox allows multiple selection.
        Boolean mMultiple;
        // Passed in to a list with multiple selection to tell
        // which items are selected.
        AutoPtr< ArrayOf<Int32> > mSelectedArray;
        // Passed in to a list with single selection to tell
        // where the initial selection is.
        Int32 mSelection;

        AutoPtr< ArrayOf<Container*> > mContainers;

        CWebViewClassic* mHost;
    };

    class PositiveButtonClickListener
        : public ElLightRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        PositiveButtonClickListener(
            /* [in] */ CWebViewClassic* host,
            /* [in] */ InvokeListBox::MyArrayListAdapter* adapter,
            /* [in] */ IListView* listView)
            : mHost(host)
            , mAdapter(adapter)
            , mListView(listView)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        CWebViewClassic* mHost;
        AutoPtr<InvokeListBox::MyArrayListAdapter> mAdapter;
        AutoPtr<IListView> mListView;
    };

    class NegativeButtonClickListener
        : public ElLightRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        NegativeButtonClickListener(
            /* [in] */ CWebViewClassic* host)
            : mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        CWebViewClassic* mHost;
    };

    class ItemClickListener
        : public ElLightRefBase
        , public IAdapterViewOnItemClickListener
    {
    public:
        ItemClickListener(
            /* [in] */ CWebViewClassic* host)
            : mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

    private:
        CWebViewClassic* mHost;
    };

    class CancelListener
        : public ElLightRefBase
        , public IDialogInterfaceOnCancelListener
    {
    public:
        CancelListener(
            /* [in] */ CWebViewClassic* host)
            : mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnCancel(
            /* [in] */ IDialogInterface* dialog);

    private:
        CWebViewClassic* mHost;
    };

    class _PictureWrapperView
        : public ElRefBase
        , public IView
        , public IDrawableCallback
        , public IKeyEventCallback
        , public IAccessibilityEventSource
        , public Elastos::Droid::View::View
    {
    public:
        _PictureWrapperView(
            /* [in] */ IContext* context)
            : View(context)
        {}

        CAR_INTERFACE_DECL();

        IVIEW_METHODS_DECL()
        IDRAWABLECALLBACK_METHODS_DECL()
        IKEYEVENTCALLBACK_METHODS_DECL()
        IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    };

    class PictureWrapperView
        : public _PictureWrapperView
    {
    public:
        PictureWrapperView(
            /* [in] */ IContext* context,
            /* [in] */ IPicture* picture,
            /* [in] */ IWebView* parent);

        CARAPI Post(
            /* [in] */ IRunnable* action,
            /* [out] */ Boolean* result);

    protected:
        CARAPI_(void) OnDraw(
            /* [in] */ ICanvas* canvas);

    public:
        AutoPtr<IPicture> mPicture;
        AutoPtr<IWebView> mWebView;
    };

    class CheckAsyncTask : public AsyncTask
    {
    public:
        CheckAsyncTask(
            /* [in] */ CWebViewClassic* host)
            : mHost(host)
        {}

    protected:
        CARAPI_(AutoPtr<IInterface>) DoInBackground(
            /* [in] */ ArrayOf<IInterface*>* params);

        CARAPI_(void) OnPostExecute(
            /* [in] */ IInterface* installedPackages);

    private:
        CWebViewClassic* mHost;
    };

    class SavePasswordButtonClickListener
        : public ElLightRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        SavePasswordButtonClickListener(
            /* [in] */ CWebViewClassic* host,
            /* [in] */ IMessage* msg)
            : mHost(host)
            , mMsg(msg)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        CWebViewClassic* mHost;
        AutoPtr<IMessage> mMsg;
    };

    class SavePasswordDismissListener
        : public ElLightRefBase
        , public IDialogInterfaceOnDismissListener
    {
    public:
        SavePasswordDismissListener(
            /* [in] */ CWebViewClassic* host,
            /* [in] */ IMessage* msg)
            : mHost(host)
            , mMsg(msg)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnDismiss(
            /* [in] */ IDialogInterface* dialog);

    private:
        CWebViewClassic* mHost;
        AutoPtr<IMessage> mMsg;
    };

    class RestorePictureRunnable : public Runnable
    {
    public:
        RestorePictureRunnable(
            /* [in] */ CWebViewClassic* host,
            /* [in] */ IFileInputStream* in,
            /* [in] */ IBundle* copy)
            : mHost(host)
            , mIn(in)
            , mCopy(copy)
        {}

        CARAPI Run();

    private:
        CWebViewClassic* mHost;
        AutoPtr<IFileInputStream> mIn;
        AutoPtr<IBundle> mCopy;
    };

    class RestorePictureRunnableInner : public Runnable
    {
    public:
        RestorePictureRunnableInner(
            /* [in] */ CWebViewClassic* host,
            /* [in] */ IPicture* p,
            /* [in] */ IBundle* copy)
            : mHost(host)
            , mPicture(p)
            , mCopy(copy)
        {}

        CARAPI Run();

    private:
        CWebViewClassic* mHost;
        AutoPtr<IPicture> mPicture;
        AutoPtr<IBundle> mCopy;
    };

    class LoadViewStateAsyncTask : public AsyncTask
    {
    public:
        LoadViewStateAsyncTask(
            /* [in] */ CWebViewClassic* host)
            : mHost(host)
        {}

    protected:
        CARAPI_(AutoPtr<IInterface>) DoInBackground(
            /* [in] */ ArrayOf<IInterface*>* params);

        CARAPI_(void) OnPostExecute(
            /* [in] */ IInterface* draw);

    private:
        CWebViewClassic* mHost;
    };

public:
    CWebViewClassic();

    ~CWebViewClassic();

    CARAPI constructor(
        /* [in] */ IWebView* webView,
        /* [in] */ IWebViewPrivateAccess* privateAccess);

    /* package */ CARAPI_(void) IncrementTextGeneration();

    //-------------------------------------------------------------------------
    // Main interface for backend provider of the WebView class.
    //-------------------------------------------------------------------------
    /**
     * Initialize this WebViewProvider instance. Called after the WebView has fully constructed.
     * @param javaScriptInterfaces is a Map of interface names, as keys, and
     * object implementing those interfaces, as values.
     * @param privateBrowsing If true the web view will be initialized in private / incognito mode.
     */
    CARAPI Init(
        /* [in] */ IMap* javaScriptInterfaces,
        /* [in] */ Boolean privateBrowsing);

    /**
     * @return The webview proxy that this classic webview is bound to.
     */
    CARAPI GetWebView(
        /* [out] */ IWebView** webView);

    CARAPI GetViewDelegate(
        /* [out] */ IWebViewProviderViewDelegate** delegate);

    CARAPI GetScrollDelegate(
        /* [out] */ IWebViewProviderScrollDelegate** delegate);

    static CARAPI_(AutoPtr<CWebViewClassic>) FromWebView(
        /* [in] */ IWebView* webView);

    // Accessors, purely for convenience (and to reduce code churn during webview proxy migration).
    CARAPI_(Int32) GetScrollX();

    CARAPI_(Int32) GetScrollY();

    CARAPI_(Int32) GetWidth();

    CARAPI_(Int32) GetHeight();

    CARAPI_(AutoPtr<IContext>) GetContext();

    CARAPI_(void) Invalidate();

    // Setters for the Scroll X & Y, without invoking the onScrollChanged etc code paths.
    CARAPI_(void) SetScrollXRaw(
        /* [in] */ Int32 mScrollX);

    CARAPI_(void) SetScrollYRaw(
        /* [in] */ Int32 mScrollY);

    CARAPI_(void) UpdateMultiTouchSupport(
        /* [in] */ IContext* context);

    CARAPI_(void) UpdateJavaScriptEnabled(
        /* [in] */ Boolean enabled);

    CARAPI ShouldDelayChildPressedState(
        /* [out] */ Boolean* result);

    CARAPI PerformAccessibilityAction(
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments,
        /* [out] */ Boolean* result);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI SetOverScrollMode(
        /* [in] */ Int32 mode);

    /* package */
    CARAPI_(void) AdjustDefaultZoomDensity(
        /* [in] */ Int32 zoomDensity);

    /* package */
    CARAPI_(void) UpdateDefaultZoomDensity(
        /* [in] */ Float density);

    /* package */
    CARAPI_(Int32) GetScaledNavSlop();

    /* package */
    CARAPI_(Boolean) OnSavePassword(
        /* [in] */ const String& schemePlusHost,
        /* [in] */ const String& username,
        /* [in] */ const String& password,
        /* [in] */ IMessage* resumeMsg);

    CARAPI SetScrollBarStyle(
        /* [in] */ Int32 style);

    CARAPI SetHorizontalScrollbarOverlay(
        /* [in] */ Boolean overlay);

    CARAPI SetVerticalScrollbarOverlay(
        /* [in] */ Boolean overlay);

    CARAPI OverlayHorizontalScrollbar(
        /* [out] */ Boolean* scrollbar);

    CARAPI OverlayVerticalScrollbar(
        /* [out] */ Boolean* scrollbar);

    /*
     * Return the width of the view where the content of WebView should render
     * to.
     * Note: this can be called from WebCoreThread.
     */
    /* package */
    CARAPI_(Int32) GetViewWidth();

    /**
     * Returns the height (in pixels) of the embedded title bar (if any). Does not care about
     * scrolling
     */
    CARAPI_(Int32) GetTitleHeight();

    CARAPI GetVisibleTitleHeight(
        /* [out] */ Int32* height);

    /*
     * Return the height of the view where the content of WebView should render
     * to.  Note that this excludes mTitleBar, if there is one.
     * Note: this can be called from WebCoreThread.
     */
    /* package */
    CARAPI_(Int32) GetViewHeight();

    CARAPI_(Int32) GetViewHeightWithTitle();

    CARAPI GetCertificate(
        /* [out] */ ISslCertificate** cer);

    CARAPI SetCertificate(
        /* [in] */ ISslCertificate* certificate);

    CARAPI SavePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI SetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI GetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [out, callee] */ ArrayOf<String>** up);

    /**
     * See {@link WebView#destroy()}.
     * As well as releasing the internal state and resources held by the implementation,
     * the provider should null all references it holds on the WebView proxy class, and ensure
     * no further method calls are made to it.
     */
    CARAPI Destroy();

    /**
     * See {@link WebView#enablePlatformNotifications()}
     */
    static CARAPI_(void) EnablePlatformNotifications();

    /**
     * See {@link WebView#disablePlatformNotifications()}
     */
    static CARAPI_(void) DisablePlatformNotifications();

    /**
     * Sets JavaScript engine flags.
     *
     * @param flags JS engine flags in a String
     *
     * This is an implementation detail.
     */
    CARAPI SetJsFlags(
        /* [in] */ const String& flags);

    CARAPI SetNetworkAvailable(
        /* [in] */ Boolean networkUp);

    /**
     * Inform WebView about the current network type.
     */
    CARAPI SetNetworkType(
        /* [in] */ const String& type,
        /* [in] */ const String& subtype);

    CARAPI SaveState(
        /* [in] */ IBundle* outState,
        /* [out] */ IWebBackForwardList** wbfl);

    CARAPI SavePicture(
        /* [in] */ IBundle* b,
        /* [in] */ IFile* dest,
        /* [out] */ Boolean* result);

    CARAPI RestorePicture(
        /* [in] */ IBundle* b,
        /* [in] */ IFile* src,
        /* [out] */ Boolean* result);

    /**
     * Saves the view data to the output stream. The output is highly
     * version specific, and may not be able to be loaded by newer versions
     * of WebView.
     * @param stream The {@link OutputStream} to save to
     * @param callback The {@link ValueCallback} to call with the result
     */
    CARAPI SaveViewState(
        /* [in] */ IOutputStream* stream,
        /* [in] */ IValueCallback* callback);

    /**
     * Loads the view data from the input stream. See
     * {@link #saveViewState(java.io.OutputStream, ValueCallback)} for more information.
     * @param stream The {@link InputStream} to load from
     */
    CARAPI LoadViewState(
        /* [in] */ IInputStream* stream);

    /**
     * Clears the view state set with {@link #loadViewState(InputStream)}.
     * This WebView will then switch to showing the content from webkit
     */
    CARAPI ClearViewState();

    CARAPI RestoreState(
        /* [in] */ IBundle* inState,
        /* [out] */ IWebBackForwardList** wbfl);

    CARAPI LoadUrl(
        /* [in] */ const String& url,
        /* [in] */ IMap* additionalHttpHeaders);

    CARAPI LoadUrl(
        /* [in] */ const String& url);

    CARAPI PostUrl(
        /* [in] */ const String& url,
        /* [in] */ ArrayOf<Byte>* postData);

    CARAPI LoadData(
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding);

    CARAPI LoadDataWithBaseURL(
        /* [in] */ const String& baseUrl,
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ const String& historyUrl);

    CARAPI SaveWebArchive(
        /* [in] */ const String& filename);

    CARAPI SaveWebArchive(
        /* [in] */ const String& basename,
        /* [in] */ Boolean autoname,
        /* [in] */ IValueCallback* callBack);

    CARAPI StopLoading();

    CARAPI Reload();

    CARAPI CanGoBack(
        /* [out] */ Boolean* result);

    CARAPI GoBack();

    CARAPI CanGoForward(
        /* [out] */ Boolean* result);

    CARAPI GoForward();

    CARAPI CanGoBackOrForward(
        /* [in] */ Int32 steps,
        /* [out] */ Boolean* result);

    CARAPI GoBackOrForward(
        /* [in] */ Int32 steps);

    CARAPI IsPrivateBrowsingEnabled(
        /* [out] */ Boolean* result);

    CARAPI PageUp(
        /* [in] */ Boolean top,
        /* [out] */ Boolean* result);

    CARAPI PageDown(
        /* [in] */ Boolean bottom,
        /* [out] */ Boolean* result);

    CARAPI ClearView();

    CARAPI CapturePicture(
        /* [out] */ IPicture** pic);

    CARAPI GetScale(
        /* [out] */ Float* scale);

    /**
     * Compute the reading level scale of the WebView
     * @param scale The current scale.
     * @return The reading level scale.
     */
    /*package*/
    CARAPI_(Float) ComputeReadingLevelScale(
        /* [in] */ Float scale);

    CARAPI SetInitialScale(
        /* [in] */ Int32 scaleInPercent);

    CARAPI InvokeZoomPicker();

    CARAPI GetHitTestResult(
        /* [out] */ IWebViewHitTestResult** result);

    CARAPI_(Int32) GetBlockLeftEdge(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Float readingScale);

    CARAPI RequestFocusNodeHref(
        /* [in] */ IMessage* hrefMsg);

    CARAPI RequestImageRef(
        /* [in] */ IMessage* msg);

    static CARAPI_(Int32) PinLoc(
        /* [in] */ Int32 x,
        /* [in] */ Int32 viewMax,
        /* [in] */ Int32 docMax);

    // Expects x in view coordinates
    CARAPI_(Int32) PinLocX(
        /* [in] */ Int32 x);

    // Expects y in view coordinates
    CARAPI_(Int32) PinLocY(
        /* [in] */ Int32 y);

    /**
     * Given an x coordinate in view space, convert it to content space.  Also
     * may be used for absolute heights.
     */
    /*package*/
    CARAPI_(Int32) ViewToContentX(
        /* [in] */ Int32 x);

    /**
     * Given a y coordinate in view space, convert it to content space.
     * Takes into account the height of the title bar if there is one
     * embedded into the WebView.
     */
    /*package*/
    CARAPI_(Int32) ViewToContentY(
        /* [in] */ Int32 y);

    /**
     * Given a distance in content space, convert it to view space. Note: this
     * does not reflect translation, just scaling, so this should not be called
     * with coordinates, but should be called for dimensions like width or
     * height.
     */
    /*package*/
    CARAPI_(Int32) ContentToViewDimension(
        /* [in] */ Int32 d);

    /**
     * Given an x coordinate in content space, convert it to view
     * space.
     */
    /*package*/
    CARAPI_(Int32) ContentToViewX(
        /* [in] */ Int32 x);

    /**
     * Given a y coordinate in content space, convert it to view
     * space.  Takes into account the height of the title bar.
     */
    /*package*/
    CARAPI_(Int32) ContentToViewY(
        /* [in] */ Int32 y);

    CARAPI_(AutoPtr<IRect>) SendOurVisibleRect();

    /**
     * Compute unzoomed width and height, and if they differ from the last
     * values we sent, send them to webkit (to be used as new viewport)
     *
     * @param force ensures that the message is sent to webkit even if the width
     * or height has not changed since the last message
     *
     * @return true if new values were sent
     */
    CARAPI_(Boolean) SendViewSizeZoom(
        /* [in] */ Boolean force);

    /**
     * Update the double-tap zoom.
     */
    /* package */
    CARAPI_(void) UpdateDoubleTapZoom(
        /* [in] */ Int32 doubleTapZoom);

    CARAPI ComputeHorizontalScrollRange(
        /* [out] */ Int32* range);

    CARAPI ComputeHorizontalScrollOffset(
        /* [out] */ Int32* offset);

    CARAPI ComputeVerticalScrollRange(
        /* [out] */ Int32* range);

    CARAPI ComputeVerticalScrollOffset(
        /* [out] */ Int32* offset);

    CARAPI ComputeVerticalScrollExtent(
        /* [out] */ Int32* extent);

    CARAPI OnDrawVerticalScrollBar(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IDrawable* scrollBar,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI OnOverScrolled(
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY,
        /* [in] */ Boolean clampedX,
        /* [in] */ Boolean clampedY);

    CARAPI GetUrl(
        /* [out] */ String* url);

    CARAPI GetOriginalUrl(
        /* [out] */ String* url);

    CARAPI GetTitle(
        /* [out] */ String* title);

    CARAPI GetFavicon(
        /* [out] */ IBitmap** favicon);

    CARAPI GetTouchIconUrl(
        /* [out] */ String* url);

    CARAPI GetProgress(
        /* [out] */ Int32* progress);

    CARAPI GetContentHeight(
        /* [out] */ Int32* height);

    CARAPI GetContentWidth(
        /* [out] */ Int32* width);

    CARAPI GetPageBackgroundColor(
        /* [out] */ Int32* color);

    CARAPI PauseTimers();

    CARAPI ResumeTimers();

    CARAPI OnPause();

    CARAPI OnWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

    CARAPI_(void) UpdateDrawingState();

    CARAPI OnResume();

    CARAPI IsPaused(
        /* [out] */ Boolean* result);

    CARAPI FreeMemory();

    CARAPI ClearCache(
        /* [in] */ Boolean includeDiskFiles);

    CARAPI ClearFormData();

    CARAPI ClearHistory();

    CARAPI ClearSslPreferences();

    CARAPI CopyBackForwardList(
        /* [out] */ IWebBackForwardList** wbfl);

    CARAPI SetFindListener(
        /* [in] */ IWebViewFindListener* listener);

    CARAPI FindNext(
        /* [in] */ Boolean forward);

    CARAPI FindAll(
        /* [in] */ const String& find,
        /* [out] */ Int32* all);

    CARAPI FindAllAsync(
        /* [in] */ const String& find);

    CARAPI ShowFindDialog(
        /* [in] */ const String& text,
        /* [in] */ Boolean showIme,
        /* [out] */ Boolean* result);

    /**
     * Return the first substring consisting of the address of a physical
     * location. Currently, only addresses in the United States are detected,
     * and consist of:
     * - a house number
     * - a street name
     * - a street type (Road, Circle, etc), either spelled out or abbreviated
     * - a city name
     * - a state or territory, either spelled out or two-letter abbr.
     * - an optional 5 digit or 9 digit zip code.
     *
     * All names must be correctly capitalized, and the zip code, if present,
     * must be valid for the state. The street type must be a standard USPS
     * spelling or abbreviation. The state or territory must also be spelled
     * or abbreviated using USPS standards. The house number may not exceed
     * five digits.
     * @param addr The string to search for addresses.
     *
     * @return the address, or if no address is found, return null.
     */
    static CARAPI_(String) FindAddress(
        /* [in] */ const String& addr);

    /**
     * Return the first substring consisting of the address of a physical
     * location. Currently, only addresses in the United States are detected,
     * and consist of:
     * - a house number
     * - a street name
     * - a street type (Road, Circle, etc), either spelled out or abbreviated
     * - a city name
     * - a state or territory, either spelled out or two-letter abbr.
     * - an optional 5 digit or 9 digit zip code.
     *
     * Names are optionally capitalized, and the zip code, if present,
     * must be valid for the state. The street type must be a standard USPS
     * spelling or abbreviation. The state or territory must also be spelled
     * or abbreviated using USPS standards. The house number may not exceed
     * five digits.
     * @param addr The string to search for addresses.
     * @param caseInsensitive addr Set to true to make search ignore case.
     *
     * @return the address, or if no address is found, return null.
     */
    static CARAPI_(String) FindAddress(
        /* [in] */ const String& addr,
        /* [in] */ Boolean caseInsensitive);

    CARAPI ClearMatches();

    /**
     * Called when the find ActionMode ends.
     */
    CARAPI_(void) NotifyFindDialogDismissed();

    CARAPI DocumentHasImages(
        /* [in] */ IMessage* response);

    /**
     * Request the scroller to abort any ongoing animation
     */
    CARAPI StopScroll();

    CARAPI ComputeScroll();

    /**
     * Called by CallbackProxy when the page starts loading.
     * @param url The URL of the page which has started loading.
     */
    /* package */
    CARAPI_(void) OnPageStarted(
        /* [in] */ const String& url);

    /**
     * Called by CallbackProxy when the page finishes loading.
     * @param url The URL of the page which has finished loading.
     */
    /* package */
    CARAPI_(void) OnPageFinished(
        /* [in] */ const String& url);

    CARAPI SetWebViewClient(
        /* [in] */ IWebViewClient* client);

    /**
     * Gets the WebViewClient
     * @return the current WebViewClient instance.
     *
     * This is an implementation detail.
     */
    CARAPI GetWebViewClient(
        /* [out] */ IWebViewClient** wvc);

    CARAPI SetDownloadListener(
        /* [in] */ IDownloadListener* listener);

    CARAPI SetWebChromeClient(
        /* [in] */ IWebChromeClient* client);

    /**
     * Gets the chrome handler.
     * @return the current WebChromeClient instance.
     *
     * This is an implementation detail.
     */
    CARAPI GetWebChromeClient(
        /* [out] */ IWebChromeClient** wcc);

    /**
     * Set the back/forward list client. This is an implementation of
     * WebBackForwardListClient for handling new items and changes in the
     * history index.
     * @param client An implementation of WebBackForwardListClient.
     */
    CARAPI SetWebBackForwardListClient(
        /* [in] */ IWebBackForwardListClient* client);

    /**
     * Gets the WebBackForwardListClient.
     */
    CARAPI GetWebBackForwardListClient(
        /* [out] */ IWebBackForwardListClient** wflc);

    CARAPI SetPictureListener(
        /* [in] */ IWebViewPictureListener* listener);

    /* FIXME: Debug only! Remove for SDK! */
    CARAPI ExternalRepresentation(
        /* [in] */ IMessage* callback);

    /* FIXME: Debug only! Remove for SDK! */
    CARAPI DocumentAsText(
        /* [in] */ IMessage* callback);

    CARAPI AddJavascriptInterface(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& interfaceName);

    CARAPI RemoveJavascriptInterface(
        /* [in] */ const String& interfaceName);

    CARAPI GetSettings(
        /* [out] */ IWebSettings** settings);

    /**
     * See {@link WebView#getPluginList()}
     */
    static CARAPI_(AutoPtr<IPluginList>) GetPluginList();

    /**
     * See {@link WebView#refreshPlugins(boolean)}
     */
    CARAPI RefreshPlugins(
        /* [in] */ Boolean reloadOpenPages);

    CARAPI OnDraw(
        /* [in] */ ICanvas* canvas);

    CARAPI SetLayoutParams(
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI PerformLongClick(
        /* [out] */ Boolean* result);

    /**
     * Select the word at the last click point.
     *
     * This is an implementation detail.
     */
    CARAPI SelectText(
        /* [out] */ Boolean* result);

    /**
     * Select the word at the indicated content coordinates.
     */
    CARAPI_(Boolean) SelectText(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI_(void) SetBaseLayer(
        /* [in] */ Int32 layer,
        /* [in] */ Boolean showVisualIndicator,
        /* [in] */ Boolean isPictureAfterFirstLayout);

    CARAPI_(Int32) GetBaseLayer();

    CARAPI_(void) OnFixedLengthZoomAnimationStart();

    CARAPI_(void) OnFixedLengthZoomAnimationEnd();

    // Only check the flag, can be called from WebCore thread
    CARAPI_(Boolean) DrawHistory();

    CARAPI_(Int32) GetHistoryPictureWidth();

    // Should only be called in UI thread
    CARAPI_(void) SwitchOutDrawHistory();

    /**
     *  Delete text from start to end in the focused textfield. If there is no
     *  focus, or if start == end, silently fail.  If start and end are out of
     *  order, swap them.
     *  @param  start   Beginning of selection to delete.
     *  @param  end     End of selection to delete.
     */
    /* package */
    CARAPI_(void) DeleteSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     *  Set the selection to (start, end) in the focused textfield. If start and
     *  end are out of order, swap them.
     *  @param  start   Beginning of selection.
     *  @param  end     End of selection.
     */
    /* package */
    CARAPI_(void) SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI OnCreateInputConnection(
        /* [in] */ IEditorInfo* outAttrs,
        /* [out] */ IInputConnection** inputConnection);

    /**
     * Called by AutoCompletePopup to find saved form data associated with the
     * textfield
     * @param name Name of the textfield.
     * @param nodePointer Pointer to the node of the textfield, so it can be
     *          compared to the currently focused textfield when the data is
     *          retrieved.
     * @param autoFillable true if WebKit has determined this field is part of
     *          a form that can be auto filled.
     * @param autoComplete true if the attribute "autocomplete" is set to true
     *          on the textfield.
     */
    /* package */
    CARAPI_(void) RequestFormData(
        /* [in] */ const String& name,
        /* [in] */ Int32 nodePointer,
        /* [in] */ Boolean autoFillable,
        /* [in] */ Boolean autoComplete);

    /**
     * Dump the display tree to "/sdcard/displayTree.txt"
     *
     * debug only
     */
    CARAPI DumpDisplayTree();

    /**
     * Dump the dom tree to adb shell if "toFile" is False, otherwise dump it to
     * "/sdcard/domTree.txt"
     *
     * debug only
     */
    CARAPI DumpDomTree(
        /* [in] */ Boolean toFile);

    /**
     * Dump the render tree to adb shell if "toFile" is False, otherwise dump it
     * to "/sdcard/renderTree.txt"
     *
     * debug only
     */
    CARAPI DumpRenderTree(
        /* [in] */ Boolean toFile);

    /**
     * Called by DRT on UI thread, need to proxy to WebCore thread.
     *
     * debug only
     */
    CARAPI SetUseMockDeviceOrientation();

    /**
     * Sets use of the Geolocation mock client. Also resets that client. Called
     * by DRT on UI thread, need to proxy to WebCore thread.
     *
     * debug only
     */
    CARAPI SetUseMockGeolocation();

    /**
     * Called by DRT on WebCore thread.
     *
     * debug only
     */
    CARAPI SetMockGeolocationPosition(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Double accuracy);

    /**
     * Called by DRT on WebCore thread.
     *
     * debug only
     */
    CARAPI SetMockGeolocationError(
        /* [in] */ Int32 code,
        /* [in] */ const String& message);

    /**
     * Called by DRT on WebCore thread.
     *
     * debug only
     */
    CARAPI SetMockGeolocationPermission(
        /* [in] */ Boolean allow);

    /**
     * Called by DRT on WebCore thread.
     *
     * debug only
     */
    CARAPI SetMockDeviceOrientation(
        /* [in] */ Boolean canProvideAlpha,
        /* [in] */ Double alpha,
        /* [in] */ Boolean canProvideBeta,
        /* [in] */ Double beta,
        /* [in] */ Boolean canProvideGamma,
        /* [in] */ Double gamma);

    CARAPI OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 repeatCount,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyPreIme(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    /**
     * Select all of the text in this WebView.
     *
     * This is an implementation detail.
     */
    CARAPI SelectAll();

    /**
     * Called when the selection has been removed.
     */
    CARAPI_(void) SelectionDone();

    /**
     * Copy the selection to the clipboard
     *
     * This is an implementation detail.
     */
    CARAPI CopySelection(
        /* [out] */ Boolean* selection);

    /**
     * Cut the selected text into the clipboard
     *
     * This is an implementation detail
     */
    CARAPI CutSelection();

    /**
     * Paste text from the clipboard to the cursor position.
     *
     * This is an implementation detail
     */
    CARAPI PasteFromClipboard();

    /**
     * Returns the currently highlighted text as a string.
     */
    CARAPI_(String) GetSelection();

    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

    CARAPI OnVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility);

    CARAPI_(void) SetActive(
        /* [in] */ Boolean active);

    // To avoid drawing the cursor ring, and remove the TextView when our window
    // loses focus.
    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    /*
     * Pass a message to WebCore Thread, telling the WebCore::Page's
     * FocusController to be  "inactive" so that it will
     * not draw the blinking cursor.  It gets set to "active" to draw the cursor
     * in WebViewCore.cpp, when the WebCore thread receives key events/clicks.
     */
    /* package */
    CARAPI_(void) SetFocusControllerActive(
        /* [in] */ Boolean active);

    CARAPI OnFocusChanged(
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    CARAPI_(void) UpdateRectsForGL();

    CARAPI SetFrame(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [out] */ Boolean* result);

    CARAPI OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 ow,
        /* [in] */ Int32 oh);

    //@Override
    CARAPI OnScrollChanged(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 oldl,
        /* [in] */ Int32 oldt);

    CARAPI DispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI_(void) OnPinchToZoomAnimationStart();

    CARAPI_(void) OnPinchToZoomAnimationEnd(
        /* [in] */ IScaleGestureDetector* detector);

    CARAPI OnHoverEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* result);

    CARAPI OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI SetMapTrackballToArrowKeys(
        /* [in] */ Boolean setMap);

    CARAPI_(void) ResetTrackballTime();

    CARAPI OnTrackballEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* result);

    /**
     * Compute the maximum horizontal scroll position. Used by {@link OverScrollGlow}.
     * @return Maximum horizontal scroll position within real content
     */
    CARAPI_(Int32) ComputeMaxScrollX();

    /**
     * Compute the maximum vertical scroll position. Used by {@link OverScrollGlow}.
     * @return Maximum vertical scroll position within real content
     */
    CARAPI_(Int32) ComputeMaxScrollY();

    CARAPI_(Boolean) UpdateScrollCoordinates(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI FlingScroll(
        /* [in] */ Int32 vx,
        /* [in] */ Int32 vy);

    CARAPI GetZoomControls(
        /* [out] */ IView** view);

    CARAPI_(void) DismissZoomControl();

    CARAPI_(Float) GetDefaultZoomScale();

    /**
     * Return the overview scale of the WebView
     * @return The overview scale.
     */
    CARAPI_(Float) GetZoomOverviewScale();

    CARAPI CanZoomIn(
        /* [out] */ Boolean* result);

    CARAPI CanZoomOut(
        /* [out] */ Boolean* result);

    CARAPI ZoomIn(
        /* [out] */ Boolean* result);

    CARAPI ZoomOut(
        /* [out] */ Boolean* result);

    /*
     * Return true if the rect (e.g. plugin) is fully visible and maximized
     * inside the WebView.
     */
    CARAPI_(Boolean) IsRectFitOnScreen(
        /* [in] */ IRect* rect);

    /*
     * Maximize and center the rectangle, specified in the document coordinate
     * space, inside the WebView. If the zoom doesn't need to be changed, do an
     * animated scroll to center it. If the zoom needs to be changed, find the
     * zoom center and do a smooth zoom transition. The rect is in document
     * coordinates
     */
    CARAPI_(void) CenterFitRect(
        /* [in] */ IRect* rect);

    CARAPI RequestFocus(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect,
        /* [out] */ Boolean* result);

    CARAPI OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI RequestChildRectangleOnScreen(
        /* [in] */ IView* child,
        /* [in] */ IRect* rect,
        /* [in] */ Boolean immediate,
        /* [out] */ Boolean* result);

    /* package */
    CARAPI_(void) ReplaceTextfieldText(
        /* [in] */ Int32 oldStart,
        /* [in] */ Int32 oldEnd,
        /* [in] */ const String& replace,
        /* [in] */ Int32 newStart,
        /* [in] */ Int32 newEnd);

    /* package */
    CARAPI_(void) PassToJavaScript(
        /* [in] */ const String& currentText,
        /* [in] */ IKeyEvent* event);

    CARAPI GetWebViewCore(
        /* [out] */ IWebViewCore** wvc);

    CARAPI SetNewPicture(
        /* [in] */ CWebViewCoreDrawData* draw,
        /* [in] */ Boolean updateBaseLayer);

    CARAPI_(void) SendBatchableInputMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj);

    /*
     * Request a dropdown menu for a listbox with multiple selection.
     *
     * @param array Labels for the listbox.
     * @param enabledArray  State for each element in the list.  See static
     *      integers in Container class.
     * @param selectedArray Which positions are initally selected.
     */
    CARAPI_(void) RequestListBox(
        /* [in] */ ArrayOf<String>* array,
        /* [in] */ ArrayOf<Int32>* enabledArray,
        /* [in] */ ArrayOf<Int32>* selectedArray);

    /*
     * Request a dropdown menu for a listbox with single selection or a single
     * <select> element.
     *
     * @param array Labels for the listbox.
     * @param enabledArray  State for each element in the list.  See static
     *      integers in Container class.
     * @param selection Which position is initally selected.
     */
    CARAPI_(void) RequestListBox(
        /* [in] */ ArrayOf<String>* array,
        /* [in] */ ArrayOf<Int32>* enabledArray,
        /* [in] */ Int32 selection);

    /**
     * See {@link WebView#setBackgroundColor(int)}
     */
    CARAPI SetBackgroundColor(
        /* [in] */ Int32 color);

    /**
     * Enable the communication b/t the webView and VideoViewProxy
     *
     * only used by the Browser
     */
    CARAPI SetHTML5VideoViewProxy(
        /* [in] */ HTML5VideoViewProxy* proxy);

    /**
     * Set the time to wait between passing touches to WebCore. See also the
     * TOUCH_SENT_INTERVAL member for further discussion.
     *
     * This is only used by the DRT test application.
     */
    CARAPI SetTouchInterval(
        /* [in] */ Int32 interval);

    /*package*/
    CARAPI_(void) AutoFillForm(
        /* [in] */ Int32 autoFillQueryId);

    /* package */
    CARAPI_(AutoPtr<ViewManager>) GetViewManager();

    /** discard all textures from tiles. Used in Profiled WebView */
    CARAPI DiscardAllTextures();

    CARAPI SetLayerType(
        /* [in] */ Int32 layerType,
        /* [in] */ IPaint* paint);

    /**
     * Begin collecting per-tile profiling data
     *
     * only used by profiling tests
     */
    CARAPI TileProfilingStart();

    /**
     * Return per-tile profiling data
     *
     * only used by profiling tests
     */
    CARAPI TileProfilingStop(
        /* [out] */ Float* stop);

    /** only used by profiling tests */
    CARAPI TileProfilingClear();

    /** only used by profiling tests */
    CARAPI TileProfilingNumFrames(
        /* [out] */ Int32* num);

    /** only used by profiling tests */
    CARAPI TileProfilingNumTilesInFrame(
        /* [in] */ Int32 frame,
        /* [out] */ Int32* num);

    /** only used by profiling tests */
    CARAPI TileProfilingGetInt(
        /* [in] */ Int32 frame,
        /* [in] */ Int32 tile,
        /* [in] */ const String& key,
        /* [out] */ Int32* Int);

    /** only used by profiling tests */
    CARAPI TileProfilingGetFloat(
        /* [in] */ Int32 frame,
        /* [in] */ Int32 tile,
        /* [in] */ const String& key,
        /* [out] */ Float* gileFloat);

    /**
     * Checks the focused content for an editable text field. This can be
     * text input or ContentEditable.
     * @return true if the focused item is an editable text field.
     */
    CARAPI_(Boolean) FocusCandidateIsEditableText();

    // Note: must be called before first WebViewClassic is created.
    static CARAPI_(void) SetShouldMonitorWebCoreThread();

    CARAPI DumpViewHierarchyWithProperties(
            /* [in] */ IBufferedWriter* out,
            /* [in] */ Int32 level);

    CARAPI FindHierarchyView(
        /* [in] */ const String& className,
        /* [in] */ Int32 hashCode,
        /* [out] */ IView** view);

    CARAPI_(Boolean) NativeSetProperty(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    CARAPI_(String) NativeGetProperty(
        /* [in] */ const String& key);

protected:
    /** Called by JNI when pages are swapped (only occurs with hardware
     * acceleration) */
    CARAPI_(void) PageSwapCallback(
        /* [in] */ Boolean notifyAnimationStarted);

    /** send content invalidate */
    CARAPI_(void) ContentInvalidateAll();

private:
    CARAPI_(void) OnHandleUiEvent(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Int32 eventType,
        /* [in] */ Int32 flags);

    CARAPI_(void) OnHandleUiTouchEvent(
        /* [in] */ IMotionEvent* ev);

    /**
     * Handles update to the trust storage.
     */
    static CARAPI_(void) HandleCertTrustChanged();

    /*
     * @param context This method expects this to be a valid context.
     */
    static CARAPI_(void) SetupTrustStorageListener(
        /* [in] */ IContext* context);

    /*
     * @param context This method expects this to be a valid context
     */
    static CARAPI_(void) SetupProxyListener(
        /* [in] */ IContext* context);

    /*
     * @param context This method expects this to be a valid context
     */
    static CARAPI_(void) DisableProxyListener(
        /* [in] */ IContext* context);

    static CARAPI_(void) HandleProxyBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI_(void) SetupPackageListener(
        /* [in] */ IContext* context);

    CARAPI_(void) Init();

    CARAPI_(Boolean) IsAccessibilityInjectionEnabled();

    CARAPI_(AutoPtr<AccessibilityInjector>) GetAccessibilityInjector();

    CARAPI_(Boolean) IsScrollableForAccessibility();

    CARAPI_(Int32) GetVisibleTitleHeightImpl();

    CARAPI_(Int32) GetOverlappingActionModeHeight();

    /**
     * Remove Find or Select ActionModes, if active.
     */
    CARAPI_(void) ClearActionModes();

    /**
     * Called to clear state when moving from one page to another, or changing
     * in some other way that makes elements associated with the current page
     * (such as ActionModes) no longer relevant.
     */
    CARAPI_(void) ClearHelpers();

    CARAPI_(void) CancelDialogs();

    CARAPI_(void) EnsureFunctorDetached();

    CARAPI_(void) DestroyCar();

    CARAPI_(void) DestroyNative();

    CARAPI_(void) RestoreHistoryPictureFields(
        /* [in] */ IPicture* p,
        /* [in] */ IBundle* b);

    CARAPI_(void) LoadUrlImpl(
        /* [in] */ const String& url,
        /* [in] */ IMap* extraHeaders);

    CARAPI_(void) LoadUrlImpl(
        /* [in] */ const String& url);

    CARAPI_(void) LoadDataImpl(
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding);

    CARAPI_(void) SaveWebArchiveImpl(
        /* [in] */ const String& basename,
        /* [in] */ Boolean autoname,
        /* [in] */ IValueCallback* callBack);

    CARAPI_(void) GoBackOrForwardImpl(
        /* [in] */ Int32 steps);

    CARAPI_(void) GoBackOrForward(
        /* [in] */ Int32 steps,
        /* [in] */ Boolean ignoreSnapshot);

    CARAPI_(void) StartPrivateBrowsing();

    CARAPI_(Boolean) ExtendScroll(
        /* [in] */ Int32 y);

    /**
     * Given a distance in view space, convert it to content space. Note: this
     * does not reflect translation, just scaling, so this should not be called
     * with coordinates, but should be called for dimensions like width or
     * height.
     */
    CARAPI_(Int32) ViewToContentDimension(
        /* [in] */ Int32 d);

    /**
     * Given a x coordinate in view space, convert it to content space.
     * Returns the result as a float.
     */
    CARAPI_(Float) ViewToContentXf(
        /* [in] */ Int32 x);

    /**
     * Given a y coordinate in view space, convert it to content space.
     * Takes into account the height of the title bar if there is one
     * embedded into the WebView. Returns the result as a float.
     */
    CARAPI_(Float) ViewToContentYf(
        /* [in] */ Int32 y);

    CARAPI_(AutoPtr<IRect>) ContentToViewRect(
        /* [in] */ IRect* x);

    /*  To invalidate a rectangle in content coordinates, we need to transform
        the rect into view coordinates, so we can then call invalidate(...).

        Normally, we would just call contentToView[XY](...), which eventually
        calls Math.round(coordinate * mActualScale). However, for invalidates,
        we need to account for the slop that occurs with antialiasing. To
        address that, we are a little more liberal in the size of the rect that
        we invalidate.

        This liberal calculation calls floor() for the top/left, and ceil() for
        the bottom/right coordinates. This catches the possible extra pixels of
        antialiasing that we might have missed with just round().
     */

    // Called by JNI to invalidate the View, given rectangle coordinates in
    // content space
    CARAPI_(void) ViewInvalidate(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    // Called by JNI to invalidate the View after a delay, given rectangle
    // coordinates in content space
    CARAPI_(void) ViewInvalidateDelayed(
        /* [in] */ Int64 delay,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI_(void) InvalidateContentRect(
        /* [in] */ IRect* r);

    // stop the scroll animation, and don't let a subsequent fling add
    // to the existing velocity
    CARAPI_(void) AbortAnimation();

    /* call from webcoreview.draw(), so we're still executing in the UI thread
    */
    CARAPI_(void) RecordNewContentSize(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Boolean updateLayout);

    // Sets r to be the visible rectangle of our webview in view coordinates
    CARAPI_(void) CalcOurVisibleRect(
        /* [in] */ IRect* r);

    // Sets r to be our visible rectangle in content coordinates
    CARAPI_(void) CalcOurContentVisibleRect(
        /* [in] */ IRect* r);

    // Sets r to be our visible rectangle in content coordinates. We use this
    // method on the native side to compute the position of the fixed layers.
    // Uses floating coordinates (necessary to correctly place elements when
    // the scale factor is not 1)
    CARAPI_(void) CalcOurContentVisibleRectF(
        /* [in] */ IRectF* r);

    CARAPI_(Int32) ComputeRealHorizontalScrollRange();

    CARAPI_(Int32) ComputeRealVerticalScrollRange();

    CARAPI_(Int32) FindAllBody(
        /* [in] */ const String& find,
        /* [in] */ Boolean isAsync);

    /**
     * Toggle whether the find dialog is showing, for both native and Java.
     */
    CARAPI_(void) SetFindIsUp(
        /* [in] */ Boolean isUp);

    CARAPI_(void) ScrollLayerTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    static CARAPI_(Int32) ComputeDuration(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    // helper to pin the scrollBy parameters (already in view coordinates)
    // returns true if the scroll was changed
    CARAPI_(Boolean) PinScrollBy(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy,
        /* [in] */ Boolean animate,
        /* [in] */ Int32 animationDuration);

    // helper to pin the scrollTo parameters (already in view coordinates)
    // returns true if the scroll was changed
    CARAPI_(Boolean) PinScrollTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Boolean animate,
        /* [in] */ Int32 animationDuration);

    // Scale from content to view coordinates, and pin.
    // Also called by jni webview.cpp
    CARAPI_(Boolean) SetContentScrollBy(
        /* [in] */ Int32 cx,
        /* [in] */ Int32 cy,
        /* [in] */ Boolean animate);

    // scale from content to view coordinates, and pin
    CARAPI_(void) ContentScrollTo(
        /* [in] */ Int32 cx,
        /* [in] */ Int32 cy,
        /* [in] */ Boolean animate);

    /**
     * These are from webkit, and are in content coordinate system (unzoomed)
     */
    CARAPI_(void) ContentSizeChanged(
        /* [in] */ Boolean updateLayout);

    CARAPI_(void) DrawContent(
        /* [in] */ ICanvas* canvas);

    /**
     * Draw the background when beyond bounds
     * @param canvas Canvas to draw into
     */
    CARAPI_(void) DrawOverScrollBackground(
        /* [in] */ ICanvas* canvas);

    CARAPI_(void) RemoveTouchHighlight();

    CARAPI_(void) OnZoomAnimationStart();

    CARAPI_(void) OnZoomAnimationEnd();

    CARAPI_(void) StartSelectingText();

    CARAPI_(void) AnimateHandle(
        /* [in] */ Boolean canShow,
        /* [in] */ IObjectAnimator* animator,
        /* [in] */ IPoint* selectionPoint,
        /* [in] */ Int32 selectionLayerId,
        /* [in] */ SelectionHandleAlpha* alpha);

    CARAPI_(void) AnimateHandles();

    CARAPI_(void) EndSelectingText();

    CARAPI_(void) EnsureSelectionHandles();

    CARAPI_(void) DrawHandle(
        /* [in] */ IPoint* point,
        /* [in] */ Int32 handleId,
        /* [in] */ IRect* bounds,
        /* [in] */ Int32 alpha,
        /* [in] */ ICanvas* canvas);

    CARAPI_(void) DrawTextSelectionHandles(
        /* [in] */ ICanvas* canvas);

    CARAPI_(Boolean) IsHandleVisible(
        /* [in] */ IPoint* selectionPoint,
        /* [in] */ Int32 layerId);

    /**
     * Takes an int[4] array as an output param with the values being
     * startX, startY, endX, endY
     */
    CARAPI_(void) GetSelectionHandles(
        /* [in] */ ArrayOf<Int32>* handles);

    CARAPI_(void) RelocateAutoCompletePopup();

    /**
     * Called in response to a message from webkit telling us that the soft
     * keyboard should be launched.
     */
    CARAPI_(void) DisplaySoftKeyboard(
        /* [in] */ Boolean isTextView);

    // Called by WebKit to instruct the UI to hide the keyboard
    CARAPI_(void) HideSoftKeyboard();

    CARAPI_(Boolean) IsEnterActionKey(
        /* [in] */ Int32 keyCode);

    CARAPI_(Boolean) StartSelectActionMode();

    CARAPI_(void) ShowPasteWindow();

    /**
     * Given segment AB, this finds the point C along AB that is closest to
     * point and then returns it scale along AB. The scale factor is AC/AB.
     *
     * @param x The x coordinate of the point near segment AB that determines
     * the scale factor.
     * @param y The y coordinate of the point near segment AB that determines
     * the scale factor.
     * @param a The first point of the line segment.
     * @param b The second point of the line segment.
     * @return The scale factor AC/AB, where C is the point on AB closest to
     *         point.
     */
    static CARAPI_(Float) ScaleAlongSegment(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ IPointF* a,
        /* [in] */ IPointF* b);

    CARAPI_(AutoPtr<IPoint>) CalculateBaseCaretTop();

    CARAPI_(AutoPtr<IPoint>) CalculateDraggingCaretTop();

    /**
     * Assuming arbitrary shape of a quadralateral forming text bounds, this
     * calculates the top of a caret.
     */
    static CARAPI_(AutoPtr<IPoint>) CalculateCaretTop(
        /* [in] */ IPoint* base,
        /* [in] */ QuadF* quad);

    CARAPI_(void) HidePasteButton();

    CARAPI_(void) SyncSelectionCursors();

    CARAPI_(Boolean) SetupWebkitSelect();

    CARAPI_(void) UpdateWebkitSelection(
        /* [in] */ Boolean isSnapped);

    CARAPI_(void) ResetCaretTimer();

    // Input : viewRect, rect in view/screen coordinate.
    // Output: contentRect, rect in content/document coordinate.
    CARAPI_(void) ViewToContentVisibleRect(
        /* [in] */ IRectF* contentRect,
        /* [in] */ IRect* viewRect);

    /**
     * Scrolls the edit field into view using the minimum scrolling necessary.
     * If the edit field is too large to fit in the visible window, the caret
     * dimensions are used so that at least the caret is visible.
     * A buffer of EDIT_RECT_BUFFER in view pixels is used to offset the
     * edit rectangle to ensure a margin with the edge of the screen.
     */
    CARAPI_(void) ScrollEditIntoView();

    CARAPI_(Boolean) InFullScreenMode();

    CARAPI_(void) DismissFullScreenMode();

    // See if there is a layer at x, y and switch to TOUCH_DRAG_LAYER_MODE if a
    // layer is found.
    CARAPI_(void) StartScrollingLayer(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI_(Float) CalculateDragAngle(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    /*
    * Common code for single touch and multi-touch.
    * (x, y) denotes current focus point, which is the touch point for single touch
    * and the middle point for multi-touch.
    */
    CARAPI_(void) HandleTouchEventCommon(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Int32 action,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    /**
     * Returns the text scroll speed in content pixels per millisecond based on
     * the touch location.
     * @param coordinate The x or y touch coordinate in content space
     * @param min The minimum coordinate (x or y) of the edit content bounds
     * @param max The maximum coordinate (x or y) of the edit content bounds
     */
    static CARAPI_(Float) GetTextScrollSpeed(
        /* [in] */ Int32 coordinate,
        /* [in] */ Int32 min,
        /* [in] */ Int32 max);

    static CARAPI_(Int32) GetSelectionCoordinate(
        /* [in] */ Int32 coordinate,
        /* [in] */ Int32 min,
        /* [in] */ Int32 max);

    CARAPI_(void) BeginScrollEdit();

    CARAPI_(void) ScrollDraggedSelectionHandleIntoView();

    CARAPI_(void) EndScrollEdit();

    static CARAPI_(Int32) ClampBetween(
        /* [in] */ Int32 value,
        /* [in] */ Int32 min,
        /* [in] */ Int32 max);

    static CARAPI_(Int32) GetTextScrollDelta(
        /* [in] */ Float speed,
        /* [in] */ Int64 deltaT);

    /**
     * Scrolls edit text a distance based on the last touch point,
     * the last scroll time, and the edit text content bounds.
     */
    CARAPI_(void) ScrollEditWithCursor();

    CARAPI_(void) StartTouch(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int64 eventTime);

    CARAPI_(void) StartDrag();

    CARAPI_(Boolean) DoDrag(
        /* [in] */ Int32 deltaX,
        /* [in] */ Int32 deltaY);

    CARAPI_(void) StopTouch();

    CARAPI_(void) CancelTouch();

    CARAPI_(void) SnapDraggingCursor();

    static CARAPI_(Float) ScaleCoordinate(
        /* [in] */ Float scale,
        /* [in] */ Float coord1,
        /* [in] */ Float coord2);

    CARAPI_(Int32) ScaleTrackballX(
        /* [in] */ Float xRate,
        /* [in] */ Int32 width);

    CARAPI_(Int32) ScaleTrackballY(
        /* [in] */ Float yRate,
        /* [in] */ Int32 height);

    CARAPI_(Int32) KeyCodeToSoundsEffect(
        /* [in] */ Int32 keyCode);

    CARAPI_(void) DoTrackball(
        /* [in] */ Int64 time,
        /* [in] */ Int32 metaState);

    CARAPI_(void) DoFling();

    // Called by JNI to handle a touch on a node representing an email address,
    // address, or phone number
    CARAPI_(void) OverrideLoading(
        /* [in] */ const String& url);

    CARAPI_(Boolean) CanTextScroll(
        /* [in] */ Int32 directionX,
        /* [in] */ Int32 directionY);

    CARAPI_(Int32) GetTextScrollX();

    CARAPI_(Int32) GetTextScrollY();

    CARAPI_(Int32) GetMaxTextScrollX();

    CARAPI_(Int32) GetMaxTextScrollY();

    CARAPI_(void) AcquireWakeLock();

    CARAPI_(void) ReleaseWakeLock();

    CARAPI_(void) SetHitTestTypeFromUrl(
        /* [in] */ const String& url);

    CARAPI_(void) SetHitTestResult(
        /* [in] */ CWebViewCoreWebKitHitTest* hit);

    CARAPI_(Boolean) ShouldDrawHighlightRect();

    CARAPI_(Boolean) ShouldAnimateTo(
        /* [in] */ CWebViewCoreWebKitHitTest* hit);

    CARAPI_(void) SetTouchHighlightRects(
        /* [in] */ CWebViewCoreWebKitHitTest* hit);

    /**
     * Used when receiving messages for REQUEST_KEYBOARD_WITH_SELECTION_MSG_ID
     * and UPDATE_TEXT_SELECTION_MSG_ID.
     */
    CARAPI_(void) UpdateTextSelectionFromMessage(
        /* [in] */ Int32 nodePointer,
        /* [in] */ Int32 textGeneration,
        /* [in] */ CWebViewCoreTextSelectionData* data);

    CARAPI_(void) ScrollEditText(
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY);

    CARAPI_(void) BeginTextBatch();

    CARAPI_(void) CommitTextBatch();

    CARAPI_(Int32) GetScaledMaxXScroll();

    CARAPI_(Int32) GetScaledMaxYScroll();

    /**
     * Called by JNI to invalidate view
     */
    CARAPI_(void) ViewInvalidate();

    /**
     * Pass the key directly to the page.  This assumes that
     * nativePageShouldHandleShiftAndArrows() returned true.
     */
    CARAPI_(void) LetPageHandleNavKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int64 time,
        /* [in] */ Boolean down,
        /* [in] */ Int32 metaState);

    CARAPI_(void) SendKeyEvent(
        /* [in] */ IKeyEvent* event);

    /**
     * Copy text into the clipboard. This is called indirectly from
     * WebViewCore.
     * @param text The text to put into the clipboard.
     */
    CARAPI_(void) CopyToClipboard(
        /* [in] */ const String& text);

    CARAPI_(void) UpdateHwAccelerated();

    // Called via JNI
    CARAPI_(void) PostInvalidate();

    CARAPI_(void) NativeCreate(
        /* [in] */ Int32 ptr,
        /* [in] */ const String& drawableDir,
        /* [in] */ Boolean isHighEndGfx);

    CARAPI_(void) NativeDebugDump();

    static CARAPI_(void) NativeDestroy(
        /* [in] */ Int32 ptr);

    CARAPI_(void) NativeDraw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRectF* visibleRect,
        /* [in] */ Int32 color,
        /* [in] */ Int32 extra);

    CARAPI_(void) NativeDumpDisplayTree(
        /* [in] */ const String& urlOrNull);

    CARAPI_(Boolean) NativeEvaluateLayersAnimations(
        /* [in] */ Int32 nativeInstance);

    CARAPI_(Int32) NativeCreateDrawGLFunction(
        /* [in] */ Int32 nativeInstance,
        /* [in] */ IRect* invScreenRect,
        /* [in] */ IRect* screenRect,
        /* [in] */ IRectF* visibleContentRect,
        /* [in] */ Float scale,
        /* [in] */ Int32 extras);

    CARAPI_(Int32) NativeGetDrawGLFunction(
        /* [in] */ Int32 nativeInstance);

    CARAPI_(void) NativeUpdateDrawGLFunction(
        /* [in] */ Int32 nativeInstance,
        /* [in] */ IRect* invScreenRect,
        /* [in] */ IRect* screenRect,
        /* [in] */ IRectF* visibleContentRect,
        /* [in] */ Float scale);

    CARAPI_(String) NativeGetSelection();

    CARAPI_(void) NativeSetHeightCanMeasure(
        /* [in] */ Boolean measure);

    CARAPI_(Boolean) NativeSetBaseLayer(
        /* [in] */ Int32 nativeInstance,
        /* [in] */ Int32 layer,
        /* [in] */ Boolean showVisualIndicator,
        /* [in] */ Boolean isPictureAfterFirstLayout,
        /* [in] */ Int32 scrollingLayer);

    CARAPI_(Int32) NativeGetBaseLayer(
        /* [in] */ Int32 nativeInstance);

    CARAPI_(void) NativeCopyBaseContentToPicture(
        /* [in] */ IPicture* pict);

    CARAPI_(Boolean) NativeDumpLayerContentToPicture(
        /* [in] */ Int32 nativeInstance,
        /* [in] */ const String& className,
        /* [in] */ Int32 layerId,
        /* [in] */ IPicture* pict);

    CARAPI_(Boolean) NativeHasContent();

    CARAPI_(void) NativeStopGL(
        /* [in] */ Int32 ptr);

    CARAPI_(void) NativeDiscardAllTextures();

    CARAPI_(void) NativeTileProfilingStart();

    CARAPI_(Float) NativeTileProfilingStop();

    CARAPI_(void) NativeTileProfilingClear();

    CARAPI_(Int32) NativeTileProfilingNumFrames();

    CARAPI_(Int32) NativeTileProfilingNumTilesInFrame(
        /* [in] */ Int32 frame);

    CARAPI_(Int32) NativeTileProfilingGetInt(
        /* [in] */ Int32 frame,
        /* [in] */ Int32 tile,
        /* [in] */ const String& key);

    CARAPI_(Float) NativeTileProfilingGetFloat(
        /* [in] */ Int32 frame,
        /* [in] */ Int32 tile,
        /* [in] */ const String& key);

    CARAPI_(void) NativeUseHardwareAccelSkia(
        /* [in] */ Boolean enabled);

    // Returns a pointer to the scrollable LayerAndroid at the given point.
    CARAPI_(Int32) NativeScrollableLayer(
        /* [in] */ Int32 nativeInstance,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ IRect* scrollRect,
        /* [in] */ IRect* scrollBounds);

    /**
     * Scroll the specified layer.
     * @param nativeInstance Native WebView instance
     * @param layer Id of the layer to scroll, as determined by nativeScrollableLayer.
     * @param newX Destination x position to which to scroll.
     * @param newY Destination y position to which to scroll.
     * @return True if the layer is successfully scrolled.
     */
    CARAPI_(Boolean) NativeScrollLayer(
        /* [in] */ Int32 nativeInstance,
        /* [in] */ Int32 layer,
        /* [in] */ Int32 newX,
        /* [in] */ Int32 newY);

    CARAPI_(void) NativeSetIsScrolling(
        /* [in] */ Boolean isScrolling);

    CARAPI_(Int32) NativeGetBackgroundColor(
        /* [in] */ Int32 nativeInstance);

    /**
     * See {@link ComponentCallbacks2} for the trim levels and descriptions
     */
    static CARAPI_(void) NativeOnTrimMemory(
        /* [in] */ Int32 level);

    static CARAPI_(void) NativeSetPauseDrawing(
        /* [in] */ Int32 instance,
        /* [in] */ Boolean pause);

    static CARAPI_(void) NativeSetTextSelection(
        /* [in] */ Int32 instance,
        /* [in] */ Int32 selection);

    static CARAPI_(Int32) NativeGetHandleLayerId(
        /* [in] */ Int32 instance,
        /* [in] */ Int32 handle,
        /* [in] */ IPoint* cursorLocation,
        /* [in] */ QuadF* textQuad);

    static CARAPI_(void) NativeMapLayerRect(
        /* [in] */ Int32 instance,
        /* [in] */ Int32 layerId,
        /* [in] */ IRect* rect);

    // Returns 1 if a layer sync is needed, else 0
    static CARAPI_(Int32) NativeSetHwAccelerated(
        /* [in] */ Int32 instance,
        /* [in] */ Boolean hwAccelerated);

    static CARAPI_(void) NativeFindMaxVisibleRect(
        /* [in] */ Int32 instance,
        /* [in] */ Int32 layerId,
        /* [in] */ IRect* visibleContentRect);

    static CARAPI_(Boolean) NativeIsHandleLeft(
        /* [in] */ Int32 instance,
        /* [in] */ Int32 handleId);

    static CARAPI_(Boolean) NativeIsPointVisible(
        /* [in] */ Int32 instance,
        /* [in] */ Int32 layerId,
        /* [in] */ Int32 contentX,
        /* [in] */ Int32 contentY);

    static void SetNativeClass(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 nativeClass);

    static Int32 GetNativeClass(
        /* [in] */ IInterface* obj);

    static Boolean ScrollBy(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 cx,
        /* [in] */ Int32 cy,
        /* [in] */ Boolean animate);

    static Int32 GetScaledMaxXScroll(
        /* [in] */ IInterface* obj);

    static Int32 GetScaledMaxYScroll(
        /* [in] */ IInterface* obj);

    static void UpdateRectsForGL(
        /* [in] */ IInterface* obj);

    static void ViewInvalidate(
        /* [in] */ IInterface* obj);

    static void ViewInvalidateRect(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    static void PostInvalidateDelayed(
        /* [in] */ IInterface* obj,
        /* [in] */ Int64 delay,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    static void PageSwapCallback(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean notifyAnimationStarted);

public:
    static const String LOGTAG;

    AutoPtr<CWebViewClassicWebViewInputConnection> mInputConnection;

    AutoPtr<IRect> mEditTextContentBounds;
    AutoPtr<IRect> mEditTextContent;
    Int32 mEditTextLayerId;
    Boolean mIsEditingText;
    List< AutoPtr<IMessage> > mBatchedTextChanges;
    Boolean mIsBatchingTextChanges;

    // Handler for dispatching UI messages.
    /* package */ AutoPtr<IHandler> mPrivateHandler;

    // Used by WebViewCore to create child views.
    /* package */ AutoPtr<ViewManager> mViewManager;

    // Used to display in full screen mode
    AutoPtr<PluginFullScreenHolder> mFullScreenHolder;

    /**
     * Helper class to get velocity for fling
     */
    AutoPtr<VelocityTracker> mVelocityTracker;

    /**
     * These prevent calling requestLayout if either dimension is fixed. This
     * depends on the layout parameters and the measure specs.
     */
    Boolean mWidthCanMeasure;
    Boolean mHeightCanMeasure;

    // Remember the last dimensions we sent to the native side so we can avoid
    // sending the same dimensions more than once.
    Int32 mLastWidthSent;
    Int32 mLastHeightSent;
    // Since view height sent to webkit could be fixed to avoid relayout, this
    // value records the last sent actual view height.
    Int32 mLastActualHeightSent;

    // Used by OverScrollGlow
    AutoPtr<OverScroller> mScroller;
    AutoPtr<IScroller> mEditTextScroller;

    static const Int32 HANDLE_ID_BASE = 0;
    static const Int32 HANDLE_ID_EXTENT = 1;

    // the color used to highlight the touch rectangles
    static const Int32 HIGHLIGHT_COLOR = 0x6633b5e5;

    /*
     * Package message ids
     */
    static const Int32 SCROLL_TO_MSG_ID                   = 101;
    static const Int32 NEW_PICTURE_MSG_ID                 = 105;
    static const Int32 WEBCORE_INITIALIZED_MSG_ID         = 107;
    static const Int32 UPDATE_TEXTFIELD_TEXT_MSG_ID       = 108;
    static const Int32 UPDATE_ZOOM_RANGE                  = 109;
    static const Int32 TAKE_FOCUS                         = 110;
    static const Int32 CLEAR_TEXT_ENTRY                   = 111;
    static const Int32 UPDATE_TEXT_SELECTION_MSG_ID       = 112;
    static const Int32 SHOW_RECT_MSG_ID                   = 113;
    static const Int32 LONG_PRESS_CENTER                  = 114;
    static const Int32 PREVENT_TOUCH_ID                   = 115;
    static const Int32 WEBCORE_NEED_TOUCH_EVENTS          = 116;
    // obj=Rect in doc coordinates
    static const Int32 INVAL_RECT_MSG_ID                  = 117;
    static const Int32 REQUEST_KEYBOARD                   = 118;
    static const Int32 SHOW_FULLSCREEN                    = 120;
    static const Int32 HIDE_FULLSCREEN                    = 121;
    static const Int32 UPDATE_MATCH_COUNT                 = 126;
    static const Int32 CENTER_FIT_RECT                    = 127;
    static const Int32 SET_SCROLLBAR_MODES                = 129;
    static const Int32 SELECTION_STRING_CHANGED           = 130;
    static const Int32 HIT_TEST_RESULT                    = 131;
    static const Int32 SAVE_WEBARCHIVE_FINISHED           = 132;

    static const Int32 SET_AUTOFILLABLE                   = 133;
    static const Int32 AUTOFILL_COMPLETE                  = 134;

    static const Int32 SCREEN_ON                          = 136;
    static const Int32 UPDATE_ZOOM_DENSITY                = 139;
    static const Int32 EXIT_FULLSCREEN_VIDEO              = 140;

    static const Int32 COPY_TO_CLIPBOARD                  = 141;
    static const Int32 INIT_EDIT_FIELD                    = 142;
    static const Int32 REPLACE_TEXT                       = 143;
    static const Int32 CLEAR_CARET_HANDLE                 = 144;
    static const Int32 KEY_PRESS                          = 145;
    static const Int32 RELOCATE_AUTO_COMPLETE_POPUP       = 146;
    static const Int32 FOCUS_NODE_CHANGED                 = 147;
    static const Int32 AUTOFILL_FORM                      = 148;
    static const Int32 SCROLL_EDIT_TEXT                   = 149;
    static const Int32 EDIT_TEXT_SIZE_CHANGED             = 150;
    static const Int32 SHOW_CARET_HANDLE                  = 151;
    static const Int32 UPDATE_CONTENT_BOUNDS              = 152;
    static const Int32 SCROLL_HANDLE_INTO_VIEW            = 153;

    static const String HandlerPrivateDebugString[];

    static const String HandlerPackageDebugString[];

    // If the site doesn't use the viewport meta tag to specify the viewport,
    // use DEFAULT_VIEWPORT_WIDTH as the default viewport width
    static const Int32 DEFAULT_VIEWPORT_WIDTH = 980;

    // normally we try to fit the content to the minimum preferred width
    // calculated by the Webkit. To avoid the bad behavior when some site's
    // minimum preferred width keeps growing when changing the viewport width or
    // the minimum preferred width is huge, an upper limit is needed.
    static Int32 sMaxViewportWidth;

    /* package */ static Boolean mLogEvent;

    /**
     * URI scheme for telephone number
     */
    static const String SCHEME_TEL;
    /**
     * URI scheme for email address
     */
    static const String SCHEME_MAILTO;
    /**
     * URI scheme for map address
     */
    static const String SCHEME_GEO;

    // The webview that is bound to this WebViewClassic instance. Primarily needed for supplying
    // as the first param in the WebViewClient and WebChromeClient callbacks.
    AutoPtr<IWebView> mWebView;
    // Callback interface, provides priviledged access into the WebView instance.
    AutoPtr<IWebViewPrivateAccess> mWebViewPrivate;
    // Cached reference to mWebView.getContext(), for convenience.
    AutoPtr<IContext> mContext;

    // No left edge for double-tap zoom alignment
    static const Int32 NO_LEFTEDGE = -1;

    Int64 mLastSwapTime;
    Double mAverageSwapFps;

    static ElaWebViewCallback sElaWebViewCallback;
private:
    // if AUTO_REDRAW_HACK is true, then the CALL key will toggle redrawing
    // the screen all-the-time. Good for profiling our drawing code
    static const Boolean AUTO_REDRAW_HACK = FALSE;

    // The rate at which edit text is scrolled in content pixels per millisecond
    static const Float TEXT_SCROLL_RATE = 0.01f;

    // The presumed scroll rate for the first scroll of edit text
    static const Int64 TEXT_SCROLL_FIRST_SCROLL_MS = 16;

    // Buffer pixels of the caret rectangle when moving edit text into view
    // after resize.
    static const Int32 EDIT_RECT_BUFFER = 10;

    static const Int64 SELECTION_HANDLE_ANIMATION_MS = 150;

    // true means redraw the screen all-the-time. Only with AUTO_REDRAW_HACK
    Boolean mAutoRedraw;

    // Reference to the AlertDialog displayed by InvokeListBox.
    // It's used to dismiss the dialog in destroy if not done before.
    AutoPtr<IAlertDialog> mListBoxDialog;

    // Reference to the save password dialog so it can be dimissed in
    // destroy if not done before.
    AutoPtr<IAlertDialog> mSavePasswordDialog;

    AutoPtr<ZoomManager> mZoomManager;

    AutoPtr<IRect> mInvScreenRect;
    AutoPtr<IRect> mScreenRect;
    AutoPtr<IRectF> mVisibleContentRect;
    Boolean mIsWebViewVisible;

    Int32 mFieldPointer;
    AutoPtr<PastePopupWindow> mPasteWindow;
    AutoPtr<AutoCompletePopup> mAutoCompletePopup;

    Int64 mLastEditScroll;
    AutoPtr<IPowerManagerWakeLock> mWakeLock;

    // A final CallbackProxy shared by WebViewCore and BrowserFrame.
    AutoPtr<CallbackProxy> mCallbackProxy;

    AutoPtr<WebViewDatabaseClassic> mDatabase;

    // SSL certificate for the main top-level page (if secure)
    AutoPtr<ISslCertificate> mCertificate;

    // Native WebView pointer that is 0 until the native object has been
    // created.
    Int32 mNativeClass;
    // This would be final but it needs to be set to null when the WebView is
    // destroyed.
    CWebViewCore* mWebViewCore;
    // Used to ignore changes to webkit text that arrives to the UI side after
    // more key events.
    Int32 mTextGeneration;

    /**
     * Position of the last touch event in pixels.
     * Use integer to prevent loss of dragging delta calculation accuracy;
     * which was done in Float and converted to integer, and resulted in gradual
     * and compounding touch position and view dragging mismatch.
     */
    Int32 mLastTouchX;
    Int32 mLastTouchY;
    Int32 mStartTouchX;
    Int32 mStartTouchY;
    Float mAverageAngle;

    /**
     * Time of the last touch event.
     */
    Int64 mLastTouchTime;

    /**
     * Time of the last time sending touch event to WebViewCore
     */
    Int64 mLastSentTouchTime;

    /**
     * The minimum elapsed time before sending another ACTION_MOVE event to
     * WebViewCore. This really should be tuned for each type of the devices.
     * For example in Google Map api test case, it takes Dream device at least
     * 150ms to do a full cycle in the WebViewCore by processing a touch event,
     * triggering the layout and drawing the picture. While the same process
     * takes 60+ms on the current high speed device. If we make
     * TOUCH_SENT_INTERVAL too small, there will be multiple touch events sent
     * to WebViewCore queue and the real layout and draw events will be pushed
     * to further, which slows down the refresh rate. Choose 50 to favor the
     * current high speed devices. For Dream like devices, 100 is a better
     * choice. Maybe make this in the buildspec later.
     * (Update 12/14/2010: changed to 0 since current device should be able to
     * handle the raw events and Map team voted to have the raw events too.
     */
    static const Int32 TOUCH_SENT_INTERVAL = 0;
    Int32 mCurrentTouchInterval;

    Int32 mMaximumFling;
    Float mLastVelocity;
    Float mLastVelX;
    Float mLastVelY;

    // The id of the native layer being scrolled.
    Int32 mCurrentScrollingLayerId;
    AutoPtr<IRect> mScrollingLayerRect;

    // only trigger accelerated fling if the new velocity is at least
    // MINIMUM_VELOCITY_RATIO_FOR_ACCELERATION times of the previous velocity
    static const Float MINIMUM_VELOCITY_RATIO_FOR_ACCELERATION = 0.2f;

    /**
     * Touch mode
     * TODO: Some of this is now unnecessary as it is handled by
     * WebInputTouchDispatcher (such as click, Int64 press, and double tap).
     */
    Int32 mTouchMode;
    static const Int32 TOUCH_INIT_MODE = 1;
    static const Int32 TOUCH_DRAG_START_MODE = 2;
    static const Int32 TOUCH_DRAG_MODE = 3;
    static const Int32 TOUCH_SHORTPRESS_START_MODE = 4;
    static const Int32 TOUCH_SHORTPRESS_MODE = 5;
    static const Int32 TOUCH_DOUBLE_TAP_MODE = 6;
    static const Int32 TOUCH_DONE_MODE = 7;
    static const Int32 TOUCH_PINCH_DRAG = 8;
    static const Int32 TOUCH_DRAG_LAYER_MODE = 9;
    static const Int32 TOUCH_DRAG_TEXT_MODE = 10;

    // true when the touch movement exceeds the slop
    Boolean mConfirmMove;
    Boolean mTouchInEditText;

    // Whether or not to draw the cursor ring.
    Boolean mDrawCursorRing;

    // true if onPause has been called (and not onResume)
    Boolean mIsPaused;

    AutoPtr<IWebViewHitTestResult> mInitialHitTestResult;
    AutoPtr<CWebViewCoreWebKitHitTest> mFocusedNode;

    /**
     * Customizable constant
     */
    // pre-computed square of ViewConfiguration.getScaledTouchSlop()
    Int32 mTouchSlopSquare;
    // pre-computed square of ViewConfiguration.getScaledDoubleTapSlop()
    Int32 mDoubleTapSlopSquare;
    // pre-computed density adjusted navigation slop
    Int32 mNavSlop;
    // This should be ViewConfiguration.getTapTimeout()
    // But system time out is 100ms, which is too short for the browser.
    // In the browser, if it switches out of tap too soon, jump tap won't work.
    // In addition, a double tap on a trackpad will always have a duration of
    // 300ms, so this value must be at least that (otherwise we will timeout the
    // first tap and convert it to a Int64 press).
    static const Int32 TAP_TIMEOUT = 300;
    // This should be ViewConfiguration.getLongPressTimeout()
    // But system time out is 500ms, which is too short for the browser.
    // With a short timeout, it's difficult to treat trigger a short press.
    static const Int32 LONG_PRESS_TIMEOUT = 1000;
    // needed to avoid flinging after a pause of no movement
    static const Int32 MIN_FLING_TIME = 250;
    // draw unfiltered after drag is held without movement
    static const Int32 MOTIONLESS_TIME = 100;
    // The amount of content to overlap between two screens when going through
    // pages with the space bar, in pixels.
    static const Int32 PAGE_SCROLL_OVERLAP = 24;

    Int32 mContentWidth;   // cache of value from WebViewCore
    Int32 mContentHeight;  // cache of value from WebViewCore

    // Need to have the separate control for horizontal and vertical scrollbar
    // style than the View's single scrollbar style
    Boolean mOverlayHorizontalScrollbar;
    Boolean mOverlayVerticalScrollbar;

    // our standard speed. this way small distances will be traversed in less
    // time than large distances, but we cap the duration, so that very large
    // distances won't take too Int64 to get there.
    static const Int32 STD_SPEED = 480;  // pixels per second
    // time for the longest scroll animation
    static const Int32 MAX_DURATION = 750;   // milliseconds

    Boolean mInOverScrollMode;
    static AutoPtr<IPaint> mOverScrollBackground;
    static AutoPtr<IPaint> mOverScrollBorder;

    Boolean mWrapContent;
    static const Int32 MOTIONLESS_FALSE = 0;
    static const Int32 MOTIONLESS_PENDING = 1;
    static const Int32 MOTIONLESS_TRUE = 2;
    static const Int32 MOTIONLESS_IGNORE = 3;
    Int32 mHeldMotionless;

    // Lazily-instantiated instance for injecting accessibility.
    AutoPtr<AccessibilityInjector> mAccessibilityInjector;

    /**
     * How Int64 the caret handle will last without being touched.
     */
    static const Int64 CARET_HANDLE_STAMINA_MS = 3000;

    AutoPtr<IDrawable> mSelectHandleLeft;
    AutoPtr<IDrawable> mSelectHandleRight;
    AutoPtr<IDrawable> mSelectHandleCenter;
    AutoPtr<IPoint> mSelectOffset;
    AutoPtr<IPoint> mSelectCursorBase;
    AutoPtr<IRect> mSelectHandleBaseBounds;
    Int32 mSelectCursorBaseLayerId;
    AutoPtr<QuadF> mSelectCursorBaseTextQuad;
    AutoPtr<IPoint> mSelectCursorExtent;
    AutoPtr<IRect> mSelectHandleExtentBounds;
    Int32 mSelectCursorExtentLayerId;
    AutoPtr<QuadF> mSelectCursorExtentTextQuad;
    AutoPtr<IPoint> mSelectDraggingCursor;
    AutoPtr<QuadF> mSelectDraggingTextQuad;
    Boolean mIsCaretSelection;

    // the region indicating where the user touched on the screen
    AutoPtr<IRegion> mTouchHighlightRegion;
    // the paint for the touch highlight
    AutoPtr<IPaint> mTouchHightlightPaint;
    // debug only
    static const Boolean DEBUG_TOUCH_HIGHLIGHT = TRUE;
    static const Int32 TOUCH_HIGHLIGHT_ELAPSE_TIME = 2000;
    AutoPtr<IPaint> mTouchCrossHairColor;
    Int32 mTouchHighlightX;
    Int32 mTouchHighlightY;
    Boolean mShowTapHighlight;

    // Basically this proxy is used to tell the Video to update layer tree at
    // SetBaseLayer time and to pause when WebView paused.
    AutoPtr<HTML5VideoViewProxy> mHTML5VideoViewProxy;

    // If we are using a set picture, don't send view updates to webkit
    Boolean mBlockWebkitViewMessages;

    // cached value used to determine if we need to switch drawing models
    Boolean mHardwareAccelSkia;

    /*
     * Private message ids
     */
    static const Int32 REMEMBER_PASSWORD = 1;
    static const Int32 NEVER_REMEMBER_PASSWORD = 2;
    static const Int32 SWITCH_TO_SHORTPRESS = 3;
    static const Int32 SWITCH_TO_LONGPRESS = 4;
    static const Int32 RELEASE_SINGLE_TAP = 5;
    static const Int32 REQUEST_FORM_DATA = 6;
    static const Int32 DRAG_HELD_MOTIONLESS = 8;
    static const Int32 PREVENT_DEFAULT_TIMEOUT = 10;
    static const Int32 SCROLL_SELECT_TEXT = 11;


    static const Int32 FIRST_PRIVATE_MSG_ID = REMEMBER_PASSWORD;
    static const Int32 LAST_PRIVATE_MSG_ID = SCROLL_SELECT_TEXT;

    static const Int32 FIRST_PACKAGE_MSG_ID = SCROLL_TO_MSG_ID;
    static const Int32 LAST_PACKAGE_MSG_ID = HIT_TEST_RESULT;

    // initial scale in percent. 0 means using default.
    Int32 mInitialScaleInPercent;

    // Whether or not a scroll event should be sent to webkit.  This is only set
    // to false when restoring the scroll position.
    Boolean mSendScrollEvent;

    Int32 mSnapScrollMode;
    static const Int32 SNAP_NONE = 0;
    static const Int32 SNAP_LOCK = 1; // not a separate state
    static const Int32 SNAP_X = 2; // may be combined with SNAP_LOCK
    static const Int32 SNAP_Y = 4; // may be combined with SNAP_LOCK
    Boolean mSnapPositive;

    // keep these in sync with their counterparts in WebView.cpp
    static const Int32 DRAW_EXTRAS_NONE = 0;
    static const Int32 DRAW_EXTRAS_SELECTION = 1;
    static const Int32 DRAW_EXTRAS_CURSOR_RING = 2;

    // keep this in sync with WebCore:ScrollbarMode in WebKit
    static const Int32 SCROLLBAR_AUTO = 0;
    static const Int32 SCROLLBAR_ALWAYSOFF = 1;
    // as we auto fade scrollbar, this is ignored.
    static const Int32 SCROLLBAR_ALWAYSON = 2;
    Int32 mHorizontalScrollBarMode;
    Int32 mVerticalScrollBarMode;

    /**
     * Max distance to overscroll by in pixels.
     * This how far content can be pulled beyond its normal bounds by the user.
     */
    Int32 mOverscrollDistance;

    /**
     * Max distance to overfling by in pixels.
     * This is how far flinged content can move beyond the end of its normal bounds.
     */
    Int32 mOverflingDistance;

    AutoPtr<OverScrollGlow> mOverScrollGlow;

    // Used to match key downs and key ups
    AutoPtr< Vector< AutoPtr<IInteger32> > > mKeysPressed;

    // for event log
    Int64 mLastTouchUpTime;

    CWebViewCoreAutoFillData* mAutoFillData;

    static Boolean sNotificationsEnabled;

    Int32 mBackgroundColor;

    static const Int64 SELECT_SCROLL_INTERVAL = 1000 / 60; // 60 / second
    Int32 mAutoScrollX;
    Int32 mAutoScrollY;
    Int32 mMinAutoScrollX;
    Int32 mMaxAutoScrollX;
    Int32 mMinAutoScrollY;
    Int32 mMaxAutoScrollY;
    AutoPtr<IRect> mScrollingLayerBounds;
    Boolean mSentAutoScrollMessage;

    // used for serializing asynchronously handled touch events.
    AutoPtr<WebViewInputDispatcher> mInputDispatcher;

    // Used to track whether picture updating was paused due to a window focus change.
    Boolean mPictureUpdatePausedForFocusChange;

    // Used to notify listeners of a new picture.
    AutoPtr<IWebViewPictureListener> mPictureListener;

    // Used to notify listeners about find-on-page results.
    AutoPtr<IWebViewFindListener> mFindListener;

    // Used to prevent resending save password message
    AutoPtr<IMessage> mResumeMsg;

    static AutoPtr<TrustStorageListener> sTrustStorageListener;

    /*
     * Receiver for PROXY_CHANGE_ACTION, will be null when it is not added handling broadcasts.
     */
    static AutoPtr<ProxyReceiver> sProxyReceiver;

    /*
     * A variable to track if there is a receiver added for ACTION_PACKAGE_ADDED
     * or ACTION_PACKAGE_REMOVED.
     */
    static Boolean sPackageInstallationReceiverAdded;

    /*
     * A set of Google packages we monitor for the
     * navigator.isApplicationInstalled() API. Add additional packages as
     * needed.
     */
    static AutoPtr<ISet> sGoogleApps;

    Int32 mCachedOverlappingActionModeHeight;

    // Used to avoid sending many visible rect messages.
    AutoPtr<IRect> mLastVisibleRectSent;
    AutoPtr<IRect> mLastGlobalRect;
    AutoPtr<IRect> mVisibleRect;
    AutoPtr<IRect> mGlobalVisibleRect;
    AutoPtr<IPoint> mScrollOffset;

    AutoPtr<IPoint> mGlobalVisibleOffset;

    AutoPtr<IRect> mTempContentVisibleRect;

    /**
     * Keep track of the find callback so that we can remove its titlebar if
     * necessary.
     */
    AutoPtr<FindActionModeCallback> mFindCallback;

    // Used to know whether the find dialog is open.  Affects whether
    // or not we draw the highlights for matches.
    Boolean mFindIsUp;

    // Keep track of the last find request sent.
    CWebViewCoreFindAllRequest* mFindRequest;

    Int32 mOrientation;

    /**
     * Keep track of the Callback so we can end its ActionMode or remove its
     * titlebar.
     */
    AutoPtr<SelectActionModeCallback> mSelectCallback;

    static const Int32 ZOOM_BITS = IPaint::FILTER_BITMAP_FLAG |
                                         IPaint::DITHER_FLAG |
                                         IPaint::SUBPIXEL_TEXT_FLAG;
    static const Int32 SCROLL_BITS = IPaint::FILTER_BITMAP_FLAG |
                                           IPaint::DITHER_FLAG;

    AutoPtr<IDrawFilter> mZoomFilter;
    // If we need to trade better quality for speed, set mScrollFilter to null
    AutoPtr<IDrawFilter> mScrollFilter;

    // draw history
    Boolean mDrawHistory;
    AutoPtr<IPicture> mHistoryPicture;
    Int32 mHistoryWidth;
    Int32 mHistoryHeight;

    // This is used to determine Int64 press with the center key.  Does not
    // affect Int64 press with the trackball/touch.
    Boolean mGotCenterDown;

    // updateRectsForGL() happens almost every draw call, in order to avoid creating
    // any object in this code path, we move the local variable out to be a private
    // final member, and we marked them as mTemp*.
    AutoPtr<IPoint> mTempVisibleRectOffset;
    AutoPtr<IRect> mTempVisibleRect;

    /*
     * Here is the snap align logic:
     * 1. If it starts nearly horizontally or vertically, snap align;
     * 2. If there is a dramitic direction change, let it go;
     *
     * Adjustable parameters. Angle is the radians on a unit circle, limited
     * to quadrant 1. Values range from 0f (horizontal) to PI/2 (vertical)
     */
    static const Float HSLOPE_TO_START_SNAP = 0.25f;
    static const Float HSLOPE_TO_BREAK_SNAP = 0.4f;
    static const Float VSLOPE_TO_START_SNAP = 1.25f;
    static const Float VSLOPE_TO_BREAK_SNAP = 0.95f;

    /*
     *  These values are used to influence the average angle when entering
     *  snap mode. If is is the first movement entering snap, we set the average
     *  to the appropriate ideal. If the user is entering into snap after the
     *  first movement, then we average the average angle with these values.
     */
    static const Float ANGLE_VERT = 2.0f;
    static const Float ANGLE_HORIZ = 0.0f;

    /*
     *  The modified moving average weight.
     *  Formula: MAV[t]=MAV[t-1] + (P[t]-MAV[t-1])/n
     */
    static const Float MMA_WEIGHT_N = 5;

    // 1/(density * density) used to compute the distance between points.
    // Computed in init().
    Float DRAG_LAYER_INVERSE_DENSITY_SQUARED;

    // The distance between two points reported in onTouchEvent scaled by the
    // density of the screen.
    static const Int32 DRAG_LAYER_FINGER_DISTANCE = 20000;

    Int64 mTrackballFirstTime;
    Int64 mTrackballLastTime;
    Float mTrackballRemainsX;
    Float mTrackballRemainsY;
    Int32 mTrackballXMove;
    Int32 mTrackballYMove;
    Boolean mSelectingText;
    Boolean mShowTextSelectionExtra;
    Boolean mSelectionStarted;
    static const Int32 TRACKBALL_KEY_TIMEOUT = 1000;
    static const Int32 TRACKBALL_TIMEOUT = 200;
    static const Int32 TRACKBALL_WAIT = 100;
    static const Int32 TRACKBALL_SCALE = 400;
    static const Int32 TRACKBALL_SCROLL_COUNT = 5;
    static const Int32 TRACKBALL_MOVE_COUNT = 10;
    static const Int32 TRACKBALL_MULTIPLIER = 3;
    static const Int32 SELECT_CURSOR_OFFSET = 16;
    static const Int32 SELECT_SCROLL = 5;
    Int32 mSelectX;
    Int32 mSelectY;
    Boolean mTrackballDown;
    Int64 mTrackballUpTime;
    Int64 mLastCursorTime;
    AutoPtr<IRect> mLastCursorBounds;
    AutoPtr<SelectionHandleAlpha> mBaseAlpha;
    AutoPtr<SelectionHandleAlpha> mExtentAlpha;
    AutoPtr<IObjectAnimator> mBaseHandleAlphaAnimator;
    AutoPtr<IObjectAnimator> mExtentHandleAlphaAnimator;

    // Set by default; BrowserActivity clears to interpret trackball data
    // directly for movement. Currently, the framework only passes
    // arrow key events, not trackball events, from one child to the next
    Boolean mMapTrackballToArrowKeys;

    AutoPtr<CWebViewCoreDrawData> mDelaySetPicture;
    AutoPtr<CWebViewCoreDrawData> mLoadedPicture;

    AutoPtr<FocusTransitionDrawable> mFocusTransition;

    AutoPtr<IMessage> mListBoxMessage;

    static Object sLock;
    Object mLock;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_CWEBVIEWCLASSIC_H__
