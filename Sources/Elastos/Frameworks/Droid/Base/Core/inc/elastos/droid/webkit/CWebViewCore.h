
#ifndef __ELASTOS_DROID_WEBKIT_CWEBVIEWCORE_H__
#define __ELASTOS_DROID_WEBKIT_CWEBVIEWCORE_H__

#include "_Elastos_Droid_Webkit_CWebViewCore.h"
#include "elastos/droid/graphics/CPoint.h"
#include "elastos/droid/webkit/WebViewInputDispatcher.h"
#include "elastos/droid/webkit/ViewManager.h"
#include "elastos/droid/webkit/CallbackProxy.h"
#include "elastos/droid/webkit/CMockGeolocation.h"
#include "elastos/droid/webkit/CWebViewClassic.h"
#include "elastos/droid/webkit/DeviceMotionService.h"
#include "elastos/droid/webkit/DeviceOrientationService.h"
#include "elastos/droid/webkit/DeviceMotionAndOrientationManager.h"
#include "elastos/droid/webkit/WebSettingsClassic.h"
#include "elastos/droid/webkit/WebTextView.h"

using Elastos::IO::IOutputStream;
using Elastos::Utility::IMap;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::IPoint;
using Elastos::Droid::Graphics::CPoint;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;

struct ElaLocationCallback;

namespace Elastos {
namespace Droid {
namespace Webkit {

#ifdef __cplusplus
extern "C"
{
#endif
    extern void Elastos_WebViewCore_ClearContent(Int32 nativeClass);
    extern Boolean Elastos_WebViewCore_Key(Int32 nativeClass, Int32 keyCode,Int32 unichar,Int32 repeatCount,
            Boolean isShift, Boolean isAlt, Boolean isSym, Boolean isDown);
    extern void Elastos_WebViewCore_ContentInvalidateAll(Int32 nativeClass);
    extern void Elastos_WebViewCore_SendListBoxChoices(Int32 nativeClass, ArrayOf<Boolean>* choices, Int32 size);
    extern void Elastos_WebViewCore_SendListBoxChoice(Int32 nativeClass, Int32 choice);
    extern void Elastos_WebViewCore_SetSize(Int32 nativeClass,Int32 width, Int32 height, Int32 textWrapWidth,
            Float scale, Int32 screenWidth, Int32 screenHeight,
            Int32 anchorX, Int32 anchorY, Boolean ignoreHeight);
    extern void Elastos_WebViewCore_SetScrollOffset(Int32 nativeClass,Boolean sendScrollEvent,Int32 dx,Int32 dy);
    extern void Elastos_WebViewCore_SetGlobalBounds(Int32 nativeClass, Int32 x,Int32 y,Int32 w,Int32 h);
    extern void Elastos_WebViewCore_SetSelection(Int32 nativeClass,Int32 start, Int32 end);
    extern Elastos::String Elastos_WebViewCore_ModifySelection(Int32 nativeClass,
            Int32 direction,
            Int32 granularity);
    extern void Elastos_WebViewCore_DeleteSelection(Int32 nativeClass,Int32 start,Int32 end,
            Int32 textGeneration);
    extern void Elastos_WebViewCore_ReplaceTextfieldText(Int32 nativeClass, Int32 oldStart,Int32 oldEnd,
            const Elastos::String& replace, Int32 start, Int32 end, Int32 textGeneration);
    extern void Elastos_WebViewCore_MoveMouse(Int32 nativeClass, Int32 x, Int32 y);
    extern void Elastos_WebViewCore_PassToJs(Int32 nativeClass,
            Int32 generation,const Elastos::String& currentText, Int32 keyCode,
            Int32 keyValue, Boolean down, Boolean cap, Boolean fn, Boolean sym);
    extern void Elastos_WebViewCore_ScrollFocusedTextInput(Int32 nativeClass, Float xPercent, Int32 y);
    extern void Elastos_WebViewCore_SetFocusControllerActive(Int32 nativeClass, Boolean active);
    extern void Elastos_WebViewCore_SaveDocumentState(Int32 nativeClass);
    extern Elastos::String Elastos_WebViewCore_FindAddress(const Elastos::String& addr,Boolean caseInsensitive);
    extern Int32 Elastos_WebViewCore_HandleTouchEvent(Int32 nativeClass, Int32 action, ArrayOf<Int32>* idArray,
            ArrayOf<Int32>* xArray, ArrayOf<Int32>* yArray, Int32 count, Int32 actionIndex, Int32 metaState);
    extern Boolean Elastos_WebViewCore_MouseClick(Int32 nativeClass);
    extern Elastos::String Elastos_WebViewCore_RetrieveHref(Int32 nativeClass, Int32 x, Int32 y);
    extern Elastos::String Elastos_WebViewCore_RetrieveAnchorText(Int32 nativeClass, Int32 x, Int32 y);
    extern Elastos::String Elastos_WebViewCore_RetrieveImageSource(Int32 nativeClass, Int32 x, Int32 y);
    extern Int32 Elastos_WebViewCore_GetContentMinPrefWidth(Int32 nativeClass);
    extern void Elastos_WebViewCore_NotifyAnimationStarted(Int32 nativeClass);
    extern Int32 Elastos_WebViewCore_RecordContent(Int32 nativeClass, Int32* x, Int32* y);
    extern void Elastos_WebViewCore_SetViewportSettingsFromNative(Int32 nativeClass);
    extern void Elastos_WebViewCore_SetBackgroundColor(Int32 nativeClass, Int32 color);
    extern void Elastos_WebViewCore_RegisterURLSchemeAsLocal(Int32 nativeClass, const Elastos::String& scheme);
    extern void Elastos_WebViewCore_DumpDomTree(Int32 nativeClass, Boolean useFile);
    extern void Elastos_WebViewCore_DumpRenderTree(Int32 nativeClass, Boolean useFile);
    extern void Elastos_WebViewCore_SetNewStorageLimit(Int32 nativeClass, Int64 quota);
    extern void Elastos_WebViewCore_GeolocationPermissionsProvide(Int32 nativeClass,
            const Elastos::String& origin, Boolean allow, Boolean remember);
    extern void Elastos_WebViewCore_Pause(Int32 nativeClass);
    extern void Elastos_WebViewCore_Resume(Int32 nativeClass);
    extern void Elastos_WebViewCore_FreeMemory(Int32 nativeClass);
    extern void Elastos_WebViewCore_SetJsFlags(Int32 nativeClass, const Elastos::String& flags);
    extern Elastos::String Elastos_WebViewCore_RequestLabel(Int32 nativeClass, Int32 framePtr, Int32 nodePtr);
    extern void Elastos_WebViewCore_RevealSelection(Int32 nativeClass);
    extern void Elastos_WebViewCore_ProvideVisitedHistory(Int32 nativeClass, ArrayOf<Elastos::String>* hist);
    extern void Elastos_WebViewCore_FullScreenPluginHidden(Int32 nativeClass, Int32 npp);
    extern void Elastos_WebViewCore_PluginSurfaceReady(Int32 nativeClass);
    extern AutoPtr<IInterface> Elastos_WebViewCore_HitTest(Int32 nativeClass,
            Int32 x, Int32 y, Int32 slop, Boolean doMoveMouse);
    extern void Elastos_WebViewCore_AutoFillForm(Int32 nativeClass, Int32 queryId);
    extern void Elastos_WebViewCore_ScrollRenderLayer(Int32 nativeClass,Int32 layer,
            Int32 left,  Int32 top,   Int32 right, Int32 bottom);
    extern void Elastos_WebViewCore_CloseIdleConnections(Int32 nativeClass);
    extern void Elastos_WebViewCore_DeleteText(Int32 nativeClass, Int32 startX,
            Int32 startY, Int32 endX,  Int32 endY);
    extern void Elastos_WebViewCore_InsertText(Int32 nativeClass, const Elastos::String& text);
    extern Elastos::String Elastos_WebViewCore_GetText(Int32 nativeClass,
            Int32 startX, Int32 startY, Int32 endX, Int32 endY);
    extern void Elastos_WebViewCore_SelectText(Int32 nativeClass, Int32 handleId, Int32 x, Int32 y);
    extern void Elastos_WebViewCore_ClearSelection(Int32 nativeClass);
    extern Boolean Elastos_WebViewCore_SelectWordAt(Int32 nativeClass, Int32 x, Int32 y);
    extern void Elastos_WebViewCore_SelectAll(Int32 nativeClass);
    extern void Elastos_WebViewCore_nativeCertTrustChanged();
    extern Int32 Elastos_WebViewCore_FindAll(Int32 nativeClass, const Elastos::String& text);
    extern Int32 Elastos_WebViewCore_FindNext(Int32 nativeClass, Boolean forward);
    extern void Elastos_WebViewCore_SetInitialFocus(Int32 nativeClass, Int32 keyDirection);
#ifdef __cplusplus
}
#endif


struct ElaWebViewCoreTextFieldInitDataCallback
{
    AutoPtr<IInterface> (*createTextFieldInitData)();
    void (*setFieldPointer)(IInterface* obj, Int32 fieldPointer);
    void (*setText)(IInterface* obj, const Elastos::String& text);
    void (*setType)(IInterface* obj, Int32 type);
    void (*setIsSpellCheckEnabled)(IInterface* obj, Boolean isSpellCheckEnabled);
    void (*setIsTextFieldNext)(IInterface* obj, Boolean isTextFieldNext);
    void (*setIsTextFieldPrev)(IInterface* obj, Boolean isTextFieldPrev);
    void (*setIsAutoCompleteEnabled)(IInterface* obj, Boolean isAutoCompleteEnabled);
    void (*setName)(IInterface* obj, const Elastos::String& name);
    void (*setLabel)(IInterface* obj, const Elastos::String& label);
    void (*setMaxLength)(IInterface* obj, Int32 maxLength);
    void (*setContentBounds)(IInterface* obj, Int32 x, Int32 y, Int32 width, Int32 Height);
    void (*setNodeLayerId)(IInterface* obj, Int32 nodeLayerId);
    void (*setClientRect)(IInterface* obj, Int32 x, Int32 y, Int32 width, Int32 Height);
};

struct ElaAndroidHitTestResultCallback
{
    AutoPtr<IInterface> (*createElaHitTest)();
    void (*setLinkUrl)(IInterface*, const Elastos::String&);
    void (*setAnchorText)(IInterface*, const Elastos::String&);
    void (*setImageUrl)(IInterface*, const Elastos::String&);
    void (*setAltDisplayString)(IInterface*, const Elastos::String&);
    void (*setTitle)(IInterface*, const Elastos::String&);
    void (*setEditable)(IInterface*, Boolean);
    void (*setTouchRects)(IInterface*, Int32, Int32);
    void (*setTapHighlightColor)(IInterface*, Int32);
    void (*setEnclosingParentRects)(IInterface*, Int32, Int32);
    void (*setHasFocus)(IInterface*, Boolean);
    void (*setIntentUrl)(IInterface*, const Elastos::String&);
};

struct ElaMediaTextureCallback
{
    void (*postInvalidate)(IInterface*);//CWebView
    void (*sendPluginDrawMsg)(IInterface*);//CWebViewCore
};

struct ElaRectElement{
    Int32 x;
    Int32 y;
    Int32 maxX;
    Int32 maxY;
};

struct ElaWebViewCoreFieldsCallback
{
    void (*setNativeClass)(IInterface* obj, Int32 nativeClass);
    Int32 (*getNativeClass)(IInterface* obj);
    void (*setViewportWidth)(IInterface* obj, Int32 viewportWidth);
    void (*setViewportHeight)(IInterface* obj, Int32 viewportHeight);
    void (*setViewportInitialScale)(IInterface* obj, Int32 viewportInitialScale);
    void (*setViewportMinimumScale)(IInterface* obj, Int32 viewportMinimumScale);
    void (*setViewportMaximumScale)(IInterface* obj, Int32 viewportMaximumScale);
    void (*setViewportUserScalable)(IInterface* obj, Boolean viewportUserScalable);
    void (*setViewportDensityDpi)(IInterface* obj, Int32 viewportDensityDpi);
    void (*setDrawIsPaused)(IInterface* obj, Boolean drawIsPaused);
    Int32 (*getLowMemoryUsageThresholdMb)(IInterface* obj);
    Int32 (*getHighMemoryUsageThresholdMb)(IInterface* obj);
    Int32 (*getHighUsageDeltaMb)(IInterface* obj);
};

struct ElaWebViewCoreMethodsCallback
{
    void (*contentScrollTo)(IInterface* obj, Int32, Int32, Boolean, Boolean);
    void (*contentDraw)(IInterface* obj);
    void (*requestListBox)(IInterface* obj, ArrayOf<Elastos::String>*, ArrayOf<Int32>*, ArrayOf<Int32>*);
    Elastos::String (*openFileChooser)(IInterface* obj, const Elastos::String&, const Elastos::String&);
    void (*requestSingleListBox)(IInterface* obj, ArrayOf<Elastos::String>*, ArrayOf<Int32>*, Int32);
    void (*jsAlert)(IInterface* obj, const Elastos::String&, const Elastos::String&);
    Boolean (*jsConfirm)(IInterface* obj, const Elastos::String&, const Elastos::String&);
    Elastos::String (*jsPrompt)(IInterface* obj, const Elastos::String&,
            const Elastos::String&, const Elastos::String&);
    Boolean (*jsUnload)(IInterface* obj, const Elastos::String&, const Elastos::String&);
    Boolean (*jsInterrupt)(IInterface* obj);
    AutoPtr<IInterface> (*getWebView)(IInterface* obj);
    void (*didFirstLayout)(IInterface* obj, Boolean);
    void (*updateViewport)(IInterface* obj);
    void (*sendNotifyProgressFinished)(IInterface* obj);
    void (*sendViewInvalidate)(IInterface* obj, Int32, Int32, Int32, Int32);
    void (*updateTextfield)(IInterface* obj, Int32, const Elastos::String&, Int32);
    void (*updateTextSelection)(IInterface* obj, Int32, Int32, Int32, Int32, Int32);
    void (*updateTextSizeAndScroll)(IInterface* obj, Int32, Int32, Int32, Int32, Int32);
    void (*clearTextEntry)(IInterface* obj);
    void (*restoreScale)(IInterface* obj, Float, Float);
    void (*needTouchEvents)(IInterface* obj, Boolean);
    void (*requestKeyboard)(IInterface* obj, Boolean);
    void (*exceededDatabaseQuota)(IInterface* obj, const Elastos::String&, const Elastos::String&, Int64, Int64);
    void (*reachedMaxAppCacheSize)(IInterface* obj, Int64, Int64);
    void (*populateVisitedLinks)(IInterface* obj);
    void (*geolocationPermissionsShowPrompt)(IInterface* obj, const Elastos::String&);
    void (*geolocationPermissionsHidePrompt)(IInterface* obj);
    AutoPtr<IInterface> (*getDeviceMotionService)(IInterface* obj,Int32);
    AutoPtr<IInterface> (*getDeviceOrientationService)(IInterface* obj,Int32);
    void (*addMessageToConsole)(IInterface* obj, const Elastos::String&, Int32, const Elastos::String&, Int32);

    void (*focusNodeChanged)(IInterface* obj, Int32, IInterface*);
    AutoPtr<IInterface> (*getPluginClass)(IInterface* obj, const Elastos::String&, const Elastos::String&);
    void (*showFullScreenPlugin)(IInterface* obj, IInterface*, Int32, Int32);
    void (*hideFullScreenPlugin)(IInterface* obj);
    AutoPtr<IInterface> (*createSurface)(IInterface* obj, IInterface*);
    AutoPtr<IInterface> (*addSurface)(IInterface* obj, IInterface*, Int32, Int32, Int32, Int32);
    void (*updateSurface)(IInterface* obj, IInterface*, Int32, Int32, Int32, Int32);
    void (*destroySurface)(IInterface* obj, IInterface*);
    AutoPtr<IInterface> (*getContext)(IInterface* obj, Boolean isApplicationContext);
    void (*keepScreenOn)(IInterface* obj, Boolean);
    void (*showRect)(IInterface* obj, Int32, Int32, Int32, Int32, Int32, Int32, Float, Float, Float, Float);
    void (*centerFitRect)(IInterface* obj, Int32, Int32, Int32, Int32);
    void (*setScrollbarModes)(IInterface* obj, Int32, Int32);
    void (*exitFullscreenVideo)(IInterface* obj);
    void (*setWebTextViewAutoFillable)(IInterface* obj, Int32, const Elastos::String&);
    void (*selectAt)(IInterface* obj, Int32, Int32);
    void (*initEditField)(IInterface* obj, Int32, Int32, Int32, IInterface*);
    Boolean (*chromeCanTakeFocus)(IInterface* obj, Int32);
    void (*chromeTakeFocus)(IInterface* obj, Int32);
    Boolean (*isSupportedMediaMimeType)(const Elastos::String&);
};

struct ElaWebViewCoreCallback
{
    struct ElaWebViewCoreFieldsCallback* wvcfCallback;
    struct ElaWebViewCoreMethodsCallback* wvcmCallback;
    struct ElaWebViewCoreTextFieldInitDataCallback* wvctfidCallback;
    struct ElaAndroidHitTestResultCallback* ahtrCallback;
    struct ElaMediaTextureCallback* mediaTextureCallback;
    struct ElaGeolocationServiceBridgeCallback* geolocationServiceBridgeCallback;
};

struct ElaGeolocationServiceBridgeCallback
{
    struct ElaGeolocationServiceCallback* glsCallback;
    struct ElaLocationCallback* locationCallback;
};

class BrowserFrame;
class CallbackProxy;
class CMockGeolocation;
class WebSettingsClassic;
class DeviceMotionService;
class DeviceOrientationService;
class DeviceMotionAndOrientationManager;

class CWebViewCoreTextSelectionData
    : public ElLightRefBase
    , public IInterface
{
public:
    CWebViewCoreTextSelectionData(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 selectTextPtr)
        : mStart(start)
        , mEnd(end)
        , mSelectTextPtr(selectTextPtr)
        , mSelectionReason(REASON_UNKNOWN)
    {}

    CAR_INTERFACE_DECL()

public:
    static const Int32 REASON_UNKNOWN = 0;
    static const Int32 REASON_ACCESSIBILITY_INJECTOR = 1;
    static const Int32 REASON_SELECT_WORD = 2;

    Int32 mStart;
    Int32 mEnd;
    Int32 mSelectTextPtr;
    Int32 mSelectionReason;
};

class CWebViewCoreWebKitHitTest
    : public ElLightRefBase
    , public IInterface
{
public:
    CWebViewCoreWebKitHitTest()
        : mEditable(FALSE)
        , mTapHighlightColor(CWebViewClassic::HIGHLIGHT_COLOR)
        , mHasFocus(FALSE)
        , mHitTestX(0)
        , mHitTestY(0)
        , mHitTestSlop(0)
        , mHitTestMovedMouse(FALSE)
    {}

    CAR_INTERFACE_DECL()

private:
    //begin callback functions
    static AutoPtr<IInterface> CreateElaHitTest();

    static void SetLinkUrl(
        /* [in] */ IInterface* obj,
        /* [in] */ const Elastos::String& linkUrl);

    static void SetAnchorText(
        /* [in] */ IInterface* obj,
        /* [in] */ const Elastos::String& anchorText);

    static void SetImageUrl(
        /* [in] */ IInterface* obj,
        /* [in] */ const Elastos::String& imageUrl);

    static void SetAltDisplayString(
        /* [in] */ IInterface* obj,
        /* [in] */ const Elastos::String& altDisplayString);

    static void SetTitle(
        /* [in] */ IInterface* obj,
        /* [in] */ const Elastos::String& title);

    static void SetEditable(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean editable);

    static void SetTouchRects(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 array,
        /* [in] */ Int32 size);

    static void SetTapHighlightColor(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 tapHighlightColor);

    static void SetEnclosingParentRects(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 array,
        /* [in] */ Int32 size);

    static void SetHasFocus(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean hasFocus);

    static void SetIntentUrl(
        /* [in] */ IInterface* obj,
        /* [in] */ const Elastos::String& intentUrl);
    //end callback functions

public:
    String mLinkUrl;
    String mIntentUrl;
    String mAnchorText;
    String mImageUrl;
    String mAltDisplayString;
    String mTitle;
    AutoPtr< ArrayOf<IRect*> > mTouchRects;//TODO maybe memoryleak???
    Boolean mEditable;
    Int32 mTapHighlightColor;
    AutoPtr< ArrayOf<IRect*> > mEnclosingParentRects;//TODO maybe memoryleak???
    Boolean mHasFocus;

    // These are the input values that produced this hit test
    Int32 mHitTestX;
    Int32 mHitTestY;
    Int32 mHitTestSlop;
    Boolean mHitTestMovedMouse;
    static struct ElaAndroidHitTestResultCallback sElaAndroidHitTestResultCallback;
};

class CWebViewCoreAutoFillData
    : public ElLightRefBase
    , public IInterface
{
public:
    CWebViewCoreAutoFillData()
        : mQueryId(WebTextView::FORM_NOT_AUTOFILLABLE)
        , mPreview("")
    {}

    CWebViewCoreAutoFillData(
        /* [in] */ Int32 queryId,
        /* [in] */ const String& preview)
        : mQueryId(queryId)
        , mPreview(preview)
    {}

    CAR_INTERFACE_DECL()

    inline CARAPI_(Int32) GetQueryId() { return mQueryId; }

    inline CARAPI_(String) GetPreviewString() { return mPreview; }

private:
    Int32 mQueryId;
    String mPreview;
};

class CWebViewCoreTextFieldInitData
    : public ElLightRefBase
    , public IInterface
{
public:
    CWebViewCoreTextFieldInitData()
        : mFieldPointer(0)
        , mType(0)
        , mIsSpellCheckEnabled(FALSE)
        , mIsTextFieldNext(FALSE)
        , mIsTextFieldPrev(FALSE)
        , mIsAutoCompleteEnabled(FALSE)
        , mMaxLength(0)
        , mNodeLayerId(0)
    {}

    CAR_INTERFACE_DECL()

private:
    //begin callback functions
    static AutoPtr<IInterface> CreateTextFieldInitData();

    static void SetFieldPointer(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 fieldPointer);

    static void SetText(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& text);

    static void SetType(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 type);

    static void SetIsSpellCheckEnabled(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean isSpellCheckEnabled);

    static void SetIsTextFieldNext(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean isTextFieldNext);

    static void SetIsTextFieldPrev(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean isTextFieldPrev);

    static void SetIsAutoCompleteEnabled(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean isAutoCompleteEnabled);

    static void SetName(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& name);

    static void SetLabel(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& label);

    static void SetMaxLength(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 maxLength);

    static void SetContentBounds(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    static void SetNodeLayerId(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 nodeLayerId);

    static void SetClientRect(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);
    //end callback functions

public:
    Int32 mFieldPointer;
    String mText;
    Int32 mType;
    Boolean mIsSpellCheckEnabled;
    Boolean mIsTextFieldNext;
    Boolean mIsTextFieldPrev;
    Boolean mIsAutoCompleteEnabled;
    String mName;
    String mLabel;
    Int32 mMaxLength;
    AutoPtr<IRect> mContentBounds;
    Int32 mNodeLayerId;
    AutoPtr<IRect> mClientRect;
    static struct ElaWebViewCoreTextFieldInitDataCallback sElaWebViewCoreTextFieldInitDataCallback;
};

class CWebViewCoreFindAllRequest
    : public ElLightRefBase
    , public Object
    , public IInterface
{
public:
    CWebViewCoreFindAllRequest(
        /* [in] */ const String& text)
        : mSearchText(text)
        , mMatchCount(-1)
        , mMatchIndex(-1)
    {}

    CAR_INTERFACE_DECL()

public:
    String mSearchText;
    Int32 mMatchCount;
    Int32 mMatchIndex;
};

class CWebViewCoreViewState
    : public ElLightRefBase
    , public IInterface
{
public:
    CWebViewCoreViewState()
        : mMinScale(0)
        , mMaxScale(0)
        , mViewScale(0)
        , mTextWrapScale(0)
        , mDefaultScale(0)
        , mScrollX(0)
        , mScrollY(0)
        , mMobileSite(FALSE)
        , mIsRestored(FALSE)
        , mShouldStartScrolledRight(FALSE)
    {}

    CAR_INTERFACE_DECL()

public:
    Float mMinScale;
    Float mMaxScale;
    Float mViewScale;
    Float mTextWrapScale;
    Float mDefaultScale;
    Int32 mScrollX;
    Int32 mScrollY;
    Boolean mMobileSite;
    Boolean mIsRestored;
    Boolean mShouldStartScrolledRight;
};

class CWebViewCoreDrawData
    : public ElLightRefBase
    , public IInterface
{
public:
    CWebViewCoreDrawData()
        : mBaseLayer(0)
        , mMinPrefWidth(0)
        , mFirstLayoutForNonStandardLoad(FALSE)
    {
        CPoint::New((IPoint**)&mContentSize);
    }

    CAR_INTERFACE_DECL()

public:
    Int32 mBaseLayer;
    // view size that was used by webkit during the most recent layout
    AutoPtr<IPoint> mViewSize;
    AutoPtr<IPoint> mContentSize;
    Int32 mMinPrefWidth;
    // only non-null if it is for the first picture set after the first layout
    AutoPtr<CWebViewCoreViewState> mViewState;
    Boolean mFirstLayoutForNonStandardLoad;
};

/**
 * @hide
 */
CarClass(CWebViewCore)
{
    friend class CMockGeolocation;
    friend class DeviceMotionAndOrientationManager;
public:
    class BaseUrlData
        : public ElLightRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

    public:
        String mBaseUrl;
        String mData;
        String mMimeType;
        String mEncoding;
        String mHistoryUrl;
    };

    class JSInterfaceData
        : public ElLightRefBase
        , public IInterface
    {
    public:
        JSInterfaceData() : mRequireAnnotation(FALSE) {}

        CAR_INTERFACE_DECL()

    public:
        AutoPtr<IInterface> mObject;
        String mInterfaceName;
        Boolean mRequireAnnotation;
    };

    class JSKeyData
        : public ElLightRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

    public:
        String mCurrentText;
        AutoPtr<IKeyEvent> mEvent;
    };

    class MotionUpData : public ElLightRefBase
    {
    public:
        MotionUpData()
            : mFrame(0)
            , mNode(0)
            , mX(0)
            , mY(0)
        {}

    public:
        Int32 mFrame;
        Int32 mNode;
        AutoPtr<IRect> mBounds;
        Int32 mX;
        Int32 mY;
    };

    class GetUrlData
        : public ElLightRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

    public:
        String mUrl;
        AutoPtr<IMap> mExtraHeaders;
    };

    class PostUrlData
        : public ElLightRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

    public:
        String mUrl;
        AutoPtr< ArrayOf<Byte> > mPostData;
    };

    class ReplaceTextData
        : public ElLightRefBase
        , public IInterface
    {
    public:
        ReplaceTextData()
            : mNewStart(0)
            , mNewEnd(0)
            , mTextGeneration(0)
        {}

        CAR_INTERFACE_DECL()

    public:
        String mReplace;
        Int32 mNewStart;
        Int32 mNewEnd;
        Int32 mTextGeneration;
    };

    class TouchUpData : public ElLightRefBase
    {
    public:
        TouchUpData()
            : mMoveGeneration(0)
            , mFrame(0)
            , mNode(0)
            , mX(0)
            , mY(0)
            , mNativeLayer(0)
        {
            CRect::New((IRect**)&mNativeLayerRect);
        }

    public:
        Int32 mMoveGeneration;
        Int32 mFrame;
        Int32 mNode;
        Int32 mX;
        Int32 mY;
        Int32 mNativeLayer;
        AutoPtr<IRect> mNativeLayerRect;
    };

    class TouchHighlightData : public ElLightRefBase
    {
    public:
        TouchHighlightData()
            : mX(0)
            , mY(0)
            , mSlop(0)
            , mNativeLayer(0)
        {}

    public:
        Int32 mX;
        Int32 mY;
        Int32 mSlop;
        Int32 mNativeLayer;
        AutoPtr<IRect> mNativeLayerRect;
    };

    class TouchEventData : public ElLightRefBase
    {
    public:
        TouchEventData()
            : mAction(0)
            , mActionIndex(0)
            , mMetaState(0)
            , mReprocess(FALSE)
            , mNativeLayer(0)
            , mSequence(0)
            , mNativeResult(FALSE)
        {
            CRect::New((IRect**)&mNativeLayerRect);
        }

    public:
        Int32 mAction;
        AutoPtr< ArrayOf<Int32> > mIds;  // Ids of the touch points
        AutoPtr< ArrayOf<IPoint> > mPoints;
        AutoPtr< ArrayOf<IPoint> > mPointsInView;  // the point coordinates in view axis.
        Int32 mActionIndex;  // Associated pointer index for ACTION_POINTER_DOWN/UP
        Int32 mMetaState;
        Boolean mReprocess;
        AutoPtr<IMotionEvent> mMotionEvent;
        Int32 mNativeLayer;
        AutoPtr<IRect> mNativeLayerRect;
        Int64 mSequence;
        Boolean mNativeResult;
    };

    class GeolocationPermissionsData
        : public ElLightRefBase
        , public IInterface
    {
    public:
        GeolocationPermissionsData()
            : mAllow(FALSE)
            , mRemember(FALSE)
        {}

        CAR_INTERFACE_DECL()

    public:
        String mOrigin;
        Boolean mAllow;
        Boolean mRemember;
    };

    class SaveViewStateRequest
        : public ElLightRefBase
        , public IInterface
    {
    public:
        SaveViewStateRequest(
            /* [in] */ IOutputStream* s,
            /* [in] */ IValueCallback* cb)
            : mStream(s)
            , mCallback(cb)
        {}

        CAR_INTERFACE_DECL()

    public:
        AutoPtr<IOutputStream> mStream;
        AutoPtr<IValueCallback> mCallback;
    };

    /**
     * @hide
     */
    class EventHub
        : public ElLightRefBase
        , public WebViewInputDispatcher::IWebKitCallbacks
    {
        friend class CWebViewCore;
    private:
        class MyHandler : public HandlerBase
        {
        public:
            MyHandler(
                /* [in] */ EventHub* owner)
                : mOwner(owner)
            {}

            CARAPI HandleMessage(
                /* [in] */ IMessage* msg);

        private:
            EventHub* mOwner;
        };

    public:
        CAR_INTERFACE_DECL();

        CARAPI GetWebKitLooper(
            /* [out] */ ILooper** looper);

        CARAPI DispatchWebKitEvent(
            /* [in] */ WebViewInputDispatcher* dispatcher,
            /* [in] */ IMotionEvent* event,
            /* [in] */ Int32 eventType,
            /* [in] */ Int32 flags,
            /* [out] */ Boolean* result);

    private:
        /**
         * Prevent other classes from creating an EventHub.
         */
        EventHub(
            /* [in] */ CWebViewCore* owner);

        /**
         * Transfer all messages to the newly created webcore thread handler.
         */
        CARAPI_(void) TransferMessages();

        /**
         * Send a message internally to the queue or to the handler
         */
        CARAPI_(void) SendMessage(
            /* [in] */ IMessage* msg);

        CARAPI_(void) RemoveMessages(
            /* [in] */ Int32 what);

        CARAPI_(void) SendMessageDelayed(
            /* [in] */ IMessage* msg,
            /* [in] */ Int64 delay);

        /**
         * Send a message internally to the front of the queue.
         */
        CARAPI_(void) SendMessageAtFrontOfQueue(
            /* [in] */ IMessage* msg);

        /**
         * Remove all the messages.
         */
        CARAPI_(void) RemoveMessages();

        /**
         * Block sending messages to the EventHub.
         */
        CARAPI_(void) BlockMessages();

    public:
        // Message Ids
        static const Int32 REVEAL_SELECTION = 96;
        static const Int32 SCROLL_TEXT_INPUT = 99;
        static const Int32 LOAD_URL = 100;
        static const Int32 STOP_LOADING = 101;
        static const Int32 RELOAD = 102;
        static const Int32 KEY_DOWN = 103;
        static const Int32 KEY_UP = 104;
        static const Int32 VIEW_SIZE_CHANGED = 105;
        static const Int32 GO_BACK_FORWARD = 106;
        static const Int32 SET_SCROLL_OFFSET = 107;
        static const Int32 RESTORE_STATE = 108;
        static const Int32 PAUSE_TIMERS = 109;
        static const Int32 RESUME_TIMERS = 110;
        static const Int32 CLEAR_CACHE = 111;
        static const Int32 CLEAR_HISTORY = 112;
        static const Int32 SET_SELECTION = 113;
        static const Int32 REPLACE_TEXT = 114;
        static const Int32 PASS_TO_JS = 115;
        static const Int32 SET_GLOBAL_BOUNDS = 116;
        static const Int32 SET_NETWORK_STATE = 119;
        static const Int32 DOC_HAS_IMAGES = 120;
        static const Int32 DELETE_SELECTION = 122;
        static const Int32 LISTBOX_CHOICES = 123;
        static const Int32 SINGLE_LISTBOX_CHOICE = 124;
        static const Int32 MESSAGE_RELAY = 125;
        static const Int32 SET_BACKGROUND_COLOR = 126;
        static const Int32 SAVE_DOCUMENT_STATE = 128;
        static const Int32 DELETE_SURROUNDING_TEXT = 129;

        static const Int32 WEBKIT_DRAW = 130;
        static const Int32 POST_URL = 132;
        static const Int32 CLEAR_CONTENT = 134;

        // UI nav messages
        static const Int32 SET_MOVE_MOUSE = 135;
        static const Int32 REQUEST_CURSOR_HREF = 137;
        static const Int32 ADD_JS_INTERFACE = 138;
        static const Int32 LOAD_DATA = 139;

        // Used to tell the focus controller not to draw the blinking cursor,
        // based on whether the WebView has focus and whether the WebView's
        // cursor matches the webpage's focus.
        static const Int32 SET_ACTIVE = 142;

        // lifecycle activities for just this DOM (unlike pauseTimers, which
        // is global)
        static const Int32 ON_PAUSE = 143;
        static const Int32 ON_RESUME = 144;
        static const Int32 FREE_MEMORY = 145;

        // Load and save web archives
        static const Int32 SAVE_WEBARCHIVE = 147;

        static const Int32 REMOVE_JS_INTERFACE = 149;

        // Network-based messaging
        static const Int32 CLEAR_SSL_PREF_TABLE = 150;

        // Test harness messages
        static const Int32 REQUEST_EXT_REPRESENTATION = 160;
        static const Int32 REQUEST_DOC_AS_TEXT = 161;

        // debugging
        static const Int32 DUMP_DOMTREE = 170;
        static const Int32 DUMP_RENDERTREE = 171;

        static const Int32 SET_JS_FLAGS = 174;
        static const Int32 CONTENT_INVALIDATE_ALL = 175;
        // Geolocation
        static const Int32 GEOLOCATION_PERMISSIONS_PROVIDE = 180;

        static const Int32 POPULATE_VISITED_LINKS = 181;

        static const Int32 HIDE_FULLSCREEN = 182;

        static const Int32 SET_NETWORK_TYPE = 183;

        // navigator.isApplicationInstalled()
        static const Int32 ADD_PACKAGE_NAMES = 184;
        static const Int32 ADD_PACKAGE_NAME = 185;
        static const Int32 REMOVE_PACKAGE_NAME = 186;

        // accessibility support
        static const Int32 MODIFY_SELECTION = 190;

        static const Int32 SET_USE_MOCK_DEVICE_ORIENTATION = 191;

        static const Int32 AUTOFILL_FORM = 192;

        static const Int32 PROXY_CHANGED = 193;

        static const Int32 EXECUTE_JS = 194;

        static const Int32 PLUGIN_SURFACE_READY = 195;

        static const Int32 NOTIFY_ANIMATION_STARTED = 196;

        static const Int32 HEARTBEAT = 197;

        static const Int32 SCROLL_LAYER = 198;

        // for cut & paste
        static const Int32 COPY_TEXT = 210;
        static const Int32 DELETE_TEXT = 211;
        static const Int32 INSERT_TEXT = 212;
        static const Int32 SELECT_TEXT = 213;
        static const Int32 SELECT_WORD_AT = 214;
        static const Int32 SELECT_ALL = 215;

        // for updating state on trust storage change
        static const Int32 TRUST_STORAGE_UPDATED = 220;

        // find-on-page controls
        static const Int32 FIND_ALL = 221;
        static const Int32 FIND_NEXT = 222;

        // key was pressed (down and up)
        static const Int32 KEY_PRESS = 223;
        static const Int32 SET_INITIAL_FOCUS = 224;

        static const Int32 SAVE_VIEW_STATE = 225;
        static const Int32 SET_USE_MOCK_GEOLOCATION = 226;

    private:
        // private message ids
        static const Int32 DESTROY =     200;

        static const Int32 FIRST_PACKAGE_MSG_ID = REVEAL_SELECTION;
        static const Int32 LAST_PACKAGE_MSG_ID = REMOVE_JS_INTERFACE;

        AutoPtr<IHandler> mHandler;

        // Message queue for containing messages before the WebCore thread is
        // ready.
        AutoPtr< List< AutoPtr<IMessage> > > mMessages;

        // Flag for blocking messages. This is used during DESTROY to avoid
        // posting more messages to the EventHub or to WebView's event handler.
        Boolean mBlockMessages;
        Boolean mDestroying;

        Int32 mTid;
        Int32 mSavedPriority;

        Object mLock;
        CWebViewCore* mOwner;
    };

    // called by JNI
    class ShowRectData
        : public ElLightRefBase
        , public IInterface
    {
    public:
        ShowRectData()
            : mLeft(0)
            , mTop(0)
            , mWidth(0)
            , mHeight(0)
            , mContentWidth(0)
            , mContentHeight(0)
            , mXPercentInDoc(0)
            , mXPercentInView(0)
            , mYPercentInDoc(0)
            , mYPercentInView(0)
        {}

        CAR_INTERFACE_DECL()

    public:
        Int32 mLeft;
        Int32 mTop;
        Int32 mWidth;
        Int32 mHeight;
        Int32 mContentWidth;
        Int32 mContentHeight;
        Float mXPercentInDoc;
        Float mXPercentInView;
        Float mYPercentInDoc;
        Float mYPercentInView;
    };

private:
    // Class for providing Handler creation inside the WebCore thread.
    class WebCoreThread : public Runnable
    {
        friend class CWebViewCore;
    private:
        class MyHandler : public HandlerBase
        {
        public:
            MyHandler(
                /* [in] */ CWebViewCore* owner)
                : mOwner(owner)
            {}

            CARAPI HandleMessage(
                /* [in] */ IMessage* msg);

        private:
            CWebViewCore* mOwner;
        };

    public:
        WebCoreThread(
            /* [in] */ CWebViewCore* owner)
            : mOwner(owner)
        {}

        CARAPI Run();

    private:
        // Message id for initializing a new WebViewCore.
        static const Int32 INITIALIZE = 0;
        static const Int32 REDUCE_PRIORITY = 1;
        static const Int32 RESUME_PRIORITY = 2;

        CWebViewCore* mOwner;
    };

    class QuotaUpdater
        : public ElLightRefBase
        , public IWebStorageQuotaUpdater
    {
    public:
        QuotaUpdater(
            /* [in] */ CWebViewCore* owner)
            : mOwner(owner)
        {}

        CAR_INTERFACE_DECL()

        CARAPI UpdateQuota(
            /* [in] */ Int64 newQuota);

    private:
        CWebViewCore* mOwner;
    };

    class ValueCallback
        : public ElLightRefBase
        , public IValueCallback
    {
    public:
        ValueCallback(
            /* [in] */ CWebViewCore* owner)
            : mOwner(owner)
        {}

        CAR_INTERFACE_DECL();

        CARAPI OnReceiveValue(
            /* [in] */ IInterface* value);

    private:
        CWebViewCore* mOwner;
    };

    class GeolocationPermissionsCallback
        : public ElLightRefBase
        , public IGeolocationPermissionsCallback
    {
    public:
        GeolocationPermissionsCallback(
            /* [in] */ CWebViewCore* owner)
            : mOwner(owner)
        {}

        CAR_INTERFACE_DECL()

        CARAPI Invoke(
            /* [in] */ const String& origin,
            /* [in] */ Boolean allow,
            /* [in] */ Boolean retain);

    private:
        CWebViewCore* mOwner;
    };

public:
    CWebViewCore();

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IWebViewClassic* w,
        /* [in] */ IHandler* proxy,
        /* [in] */ IMap* javascriptInterfaces);

    /* Handle the initialization of WebViewCore during subwindow creation. This
     * method is called from the WebCore thread but it is called before the
     * INITIALIZE message can be handled.
     */
    /* package */
    CARAPI_(void) InitializeSubwindow();

    /* Get the BrowserFrame component. This is used for subwindow creation and
     * is called only from BrowserFrame in the WebCore thread. */
    /* package */
    CARAPI_(AutoPtr<BrowserFrame>) GetBrowserFrame();

    CARAPI_(AutoPtr<WebViewInputDispatcher::IWebKitCallbacks>) GetInputDispatcherCallbacks();

    //-------------------------------------------------------------------------
    // Common methods
    //-------------------------------------------------------------------------

    /**
     * Causes all timers to pause. This applies to all WebViews in the current
     * app process.
     */
    static CARAPI PauseTimers();

    /**
     * Resume all timers. This applies to all WebViews in the current process.
     */
    static CARAPI ResumeTimers();

    //-------------------------------------------------------------------------
    // Common methods
    //-------------------------------------------------------------------------

    CARAPI GetSettings(
        /* [out] */ IWebSettingsClassic** settings);

    /*
     * Given mimeType, check whether it's supported in Android media framework.
     * mimeType could be such as "audio/ogg" and "video/mp4".
     */
    /* package */
    static CARAPI_(Boolean) IsSupportedMediaMimeType(
        /* [in] */ const String& mimeType);

    /**
     * Clear the picture set. To be called only on the WebCore thread.
     */
    /* package */
    CARAPI_(void) ClearContent();

    //-------------------------------------------------------------------------
    // JNI methods
    //-------------------------------------------------------------------------

    static CARAPI_(String) NativeFindAddress(
        /* [in] */ const String& addr,
        /* [in] */ Boolean caseInsensitive);

    //-------------------------------------------------------------------------
    // Methods called by host activity (in the same thread)
    //-------------------------------------------------------------------------

    CARAPI_(void) StopLoading();

    /**
     * @hide
     */
    CARAPI SendMessage(
        /* [in] */ IMessage* msg);

    //-------------------------------------------------------------------------
    // Methods called by WebView
    // If it refers to local variable, it needs synchronized().
    // If it needs WebCore, it has to send message.
    //-------------------------------------------------------------------------

    CARAPI_(void) SendMessages(
        /* [in] */ List< AutoPtr<IMessage> >& messages);

    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what);

    CARAPI_(void) SendMessageAtFrontOfQueue(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj);

    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1);

    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2);

    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ IInterface* obj);

    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj);

    CARAPI_(void) SendMessageAtFrontOfQueue(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    CARAPI_(void) SendMessageDelayed(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj,
        /* [in] */ Int64 delay);

    CARAPI_(void) RemoveMessages(
        /* [in] */ Int32 what);

    CARAPI_(void) RemoveMessages();

    /**
     * Sends a DESTROY message to WebCore.
     * Called from UI thread.
     */
    CARAPI_(void) Destroy();

    CARAPI_(void) PauseWebKitDraw();

    CARAPI_(void) ResumeWebKitDraw();

    static CARAPI_(void) ReducePriority();

    static CARAPI_(void) ResumePriority();

    static CARAPI_(void) SendStaticMessage(
        /* [in] */ Int32 messageType,
        /* [in] */ IInterface* argument);

    static CARAPI_(void) PauseUpdatePicture(
        /* [in] */ CWebViewCore* core);

    static CARAPI_(void) ResumeUpdatePicture(
        /* [in] */ CWebViewCore* core);

    static CARAPI_(Boolean) IsUpdatePicturePaused(
        /* [in] */ CWebViewCore* core);

    // called from JNI or WebView thread
    /* package */
    CARAPI_(void) ContentDraw();

    /*
     * Called by the WebView thread
     */
    /* package */
    CARAPI_(void) SignalRepaintDone();

    // Gets the WebViewClassic corresponding to this WebViewCore. Note that the
    // WebViewClassic object must only be used on the UI thread.
    /* package */
    CARAPI_(AutoPtr<CWebViewClassic>) GetWebViewClassic();

    static CARAPI_(Float) GetFixedDisplayDensity(
        /* [in] */ IContext* context);

    CARAPI_(AutoPtr<IContext>) GetContext();

    CARAPI SetMockGeolocationPosition(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Double accuracy);

    CARAPI SetMockGeolocationError(
        /* [in] */ Int32 code,
        /* [in] */ const String& message);

    CARAPI SetMockGeolocationPermission(
        /* [in] */ Boolean allow);

    CARAPI SetMockDeviceOrientation(
        /* [in] */ Boolean canProvideAlpha,
        /* [in] */ Double alpha,
        /* [in] */ Boolean canProvideBeta,
        /* [in] */ Double beta,
        /* [in] */ Boolean canProvideGamma,
        /* [in] */ Double gamma);

    static CARAPI_(void) SetShouldMonitorWebCoreThread();

protected:
    /**
     * Add an error message to the client's console.
     * @param message The message to add
     * @param lineNumber the line on which the error occurred
     * @param sourceID the filename of the source that caused the error.
     * @param msgLevel the log level of this message. This is a value casted to int
     *     from WebCore::MessageLevel in WebCore/page/Console.h.
     */
    CARAPI_(void) AddMessageToConsole(
        /* [in] */ const String& message,
        /* [in] */ Int32 lineNumber,
        /* [in] */ const String& sourceID,
        /* [in] */ Int32 msgLevel);

    /**
     * Invoke a javascript alert.
     * @param message The message displayed in the alert.
     */
    CARAPI_(void) JsAlert(
        /* [in] */ const String& url,
        /* [in] */ const String& message);

    /**
     * Notify the embedding application that the origin has exceeded it's database quota.
     * @param url The URL that caused the overflow.
     * @param databaseIdentifier The identifier of the database.
     * @param quota The current quota for the origin.
     * @param estimatedDatabaseSize The estimated size of the database.
     */
    CARAPI_(void) ExceededDatabaseQuota(
        /* [in] */ const String& url,
        /* [in] */ const String& databaseIdentifier,
        /* [in] */ Int64 quota,
        /* [in] */ Int64 estimatedDatabaseSize);

    /**
     * Notify the embedding application that the appcache has reached or exceeded its maximum
     * allowed storage size.
     *
     * @param requiredStorage is the amount of storage, in bytes, that would be
     * needed in order for the last appcache operation to succeed.
     * @param maxSize maximum allowed Application Cache database size, in bytes.
     */
    CARAPI_(void) ReachedMaxAppCacheSize(
        /* [in] */ Int64 requiredStorage,
        /* [in] */ Int64 maxSize);

    CARAPI_(void) PopulateVisitedLinks();

    /**
     * Shows a prompt to ask the user to set the Geolocation permission state
     * for the given origin.
     * @param origin The origin for which Geolocation permissions are
     *     requested.
     */
    CARAPI_(void) GeolocationPermissionsShowPrompt(
        /* [in] */ const String& origin);

    /**
     * Hides the Geolocation permissions prompt.
     */
    CARAPI_(void) GeolocationPermissionsHidePrompt();

    /**
     * Invoke a javascript confirm dialog.
     * @param message The message displayed in the dialog.
     * @return True if the user confirmed or false if the user cancelled.
     */
    CARAPI_(Boolean) JsConfirm(
        /* [in] */ const String& url,
        /* [in] */ const String& message);

    /**
     * Invoke a javascript prompt dialog.
     * @param message The message to be displayed in the dialog.
     * @param defaultValue The default value in the prompt input.
     * @return The input from the user or null to indicate the user cancelled
     *         the dialog.
     */
    CARAPI_(String) JsPrompt(
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ const String& defaultValue);

    /**
     * Invoke a javascript before unload dialog.
     * @param url The url that is requesting the dialog.
     * @param message The message displayed in the dialog.
     * @return True if the user confirmed or false if the user cancelled. False
     *         will cancel the navigation.
     */
    CARAPI_(Boolean) JsUnload(
        /* [in] */ const String& url,
        /* [in] */ const String& message);

    /**
     *
     * Callback to notify that a JavaScript execution timeout has occured.
     * @return True if the JavaScript execution should be interrupted. False
     *         will continue the execution.
     */
    CARAPI_(Boolean) JsInterrupt();

    /**
     * Notify the webview that we want to exit the video fullscreen.
     * This is called through JNI by webcore.
     */
    CARAPI_(void) ExitFullscreenVideo();

    CARAPI_(AutoPtr<IInterface>) GetDeviceMotionService();

    CARAPI_(AutoPtr<IInterface>) GetDeviceOrientationService();

private:
    /* Initialize private data within the WebCore thread.
     */
    CARAPI_(void) Initialize();

    /**
     * Called by JNI when the focus node changed.
     */
    CARAPI_(void) FocusNodeChanged(
        /* [in] */ Int32 nodePointer,
        /* [in] */ IInterface* hitTest);

    /**
     * Called by JNI to advance focus to the next view.
     */
    CARAPI_(void) ChromeTakeFocus(
        /* [in] */ Int32 webkitDirection);

    /**
     * Called by JNI to see if we can take focus in the given direction.
     */
    CARAPI_(Boolean) ChromeCanTakeFocus(
        /* [in] */ Int32 webkitDirection);

    /**
     * Maps a Webkit focus direction to a framework one
     */
    CARAPI_(Int32) MapDirection(
        /* [in] */ Int32 webkitDirection);

    /**
     * Called by JNI.  Open a file chooser to upload a file.
     * @param acceptType The value of the 'accept' attribute of the
     *         input tag associated with this file picker.
     * @param capture The value of the 'capture' attribute of the
     *         input tag associated with this file picker.
     * @return String version of the URI.
     */
    CARAPI_(String) OpenFileChooser(
        /* [in] */ const String& acceptType,
        /* [in] */ const String& capture);

    /**
     * Empty the picture set.
     */
    CARAPI_(void) NativeClearContent(
        /* [in] */ Int32 nativeClass);

    CARAPI_(void) NativeContentInvalidateAll(
        /* [in] */ Int32 nativeClass);

    /**
     * Redraw a portion of the picture set. The Point wh returns the
     * width and height of the overall picture.
     */
    CARAPI_(Int32) NativeRecordContent(
        /* [in] */ Int32 nativeClass,
        /* [in] */ IPoint* wh);

    /**
     * Notify webkit that animations have begun (on the hardware accelerated content)
     */
    CARAPI_(void) NativeNotifyAnimationStarted(
        /* [in] */ Int32 nativeClass);

    CARAPI_(Boolean) NativeKey(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 unichar,
        /* [in] */ Int32 repeatCount,
        /* [in] */ Boolean isShift,
        /* [in] */ Boolean isAlt,
        /* [in] */ Boolean isSym,
        /* [in] */ Boolean isDown);

    CARAPI_(void) NativeSendListBoxChoices(
        /* [in] */ Int32 nativeClass,
        /* [in] */ ArrayOf<Boolean>* choices,
        /* [in] */ Int32 size);

    CARAPI_(void) NativeSendListBoxChoice(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 choice);

    CARAPI_(void) NativeCloseIdleConnections(
        /* [in] */ Int32 nativeClass);

    /*  Tell webkit what its width and height are, for the purposes
        of layout/line-breaking. These coordinates are in document space,
        which is the same as View coords unless we have zoomed the document
        (see nativeSetZoom).
        textWrapWidth is used by layout to wrap column around. If viewport uses
        fixed size, textWrapWidth can be different from width with zooming.
        should this be called nativeSetViewPortSize?
    */
    CARAPI_(void) NativeSetSize(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 textWrapWidth,
        /* [in] */ Float scale,
        /* [in] */ Int32 screenWidth,
        /* [in] */ Int32 screenHeight,
        /* [in] */ Int32 anchorX,
        /* [in] */ Int32 anchorY,
        /* [in] */ Boolean ignoreHeight);

    CARAPI_(Int32) NativeGetContentMinPrefWidth(
        /* [in] */ Int32 nativeClass);

    // Start: functions that deal with text editing
    CARAPI_(void) NativeReplaceTextfieldText(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 oldStart,
        /* [in] */ Int32 oldEnd,
        /* [in] */ const String& replace,
        /* [in] */ Int32 newStart,
        /* [in] */ Int32 newEnd,
        /* [in] */ Int32 textGeneration);

    CARAPI_(void) PassToJs(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 gen,
        /* [in] */ const String& currentText,
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 keyValue,
        /* [in] */ Boolean down,
        /* [in] */ Boolean cap,
        /* [in] */ Boolean fn,
        /* [in] */ Boolean sym);

    CARAPI_(void) NativeSetFocusControllerActive(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Boolean active);

    CARAPI_(void) NativeSaveDocumentState(
        /* [in] */ Int32 nativeClass);

    CARAPI_(void) NativeMoveMouse(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(String) NativeRetrieveHref(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(String) NativeRetrieveAnchorText(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(String) NativeRetrieveImageSource(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(Boolean) NativeMouseClick(
        /* [in] */ Int32 nativeClass);

    CARAPI_(Int32) NativeHandleTouchEvent(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 action,
        /* [in] */ ArrayOf<Int32>* idArray,
        /* [in] */ ArrayOf<Int32>* xArray,
        /* [in] */ ArrayOf<Int32>* yArray,
        /* [in] */ Int32 count,
        /* [in] */ Int32 actionIndex,
        /* [in] */ Int32 metaState);

    CARAPI_(void) NativeSetBackgroundColor(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 color);

    CARAPI_(void) NativeDumpDomTree(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Boolean useFile);

    CARAPI_(void) NativeDumpRenderTree(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Boolean useFile);

    CARAPI_(void) NativeSetJsFlags(
        /* [in] */ Int32 nativeClass,
        /* [in] */ const String& flags);

    /**
     *  Delete text from start to end in the focused textfield. If there is no
     *  focus, or if start == end, silently fail.  If start and end are out of
     *  order, swap them.
     * @param  nativeClass Pointer to the C++ WebViewCore object mNativeClass
     * @param  start   Beginning of selection to delete.
     * @param  end     End of selection to delete.
     * @param  textGeneration Text generation number when delete was pressed.
     */
    CARAPI_(void) NativeDeleteSelection(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 textGeneration);

    /**
     *  Set the selection to (start, end) in the focused textfield. If start and
     *  end are out of order, swap them.
     * @param  nativeClass Pointer to the C++ WebViewCore object mNativeClass
     * @param  start   Beginning of selection.
     * @param  end     End of selection.
     */
    CARAPI_(void) NativeSetSelection(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    // Register a scheme to be treated as local scheme so that it can access
    // local asset files for resources
    CARAPI_(void) NativeRegisterURLSchemeAsLocal(
        /* [in] */ Int32 nativeClass,
        /* [in] */ const String& scheme);

    /*
     * Inform webcore that the user has decided whether to allow or deny new
     * quota for the current origin or more space for the app cache, and that
     * the main thread should wake up now.
     * @param limit Is the new quota for an origin or new app cache max size.
     */
    CARAPI_(void) NativeSetNewStorageLimit(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int64 limit);

    /**
     * Provide WebCore with a Geolocation permission state for the specified
     * origin.
     * @param nativeClass Pointer to the C++ WebViewCore object mNativeClass
     * @param origin The origin for which Geolocation permissions are provided.
     * @param allow Whether Geolocation permissions are allowed.
     * @param remember Whether this decision should be remembered beyond the
     *     life of the current page.
     */
    CARAPI_(void) NativeGeolocationPermissionsProvide(
        /* [in] */ Int32 nativeClass,
        /* [in] */ const String& origin,
        /* [in] */ Boolean allow,
        /* [in] */ Boolean remember);

    /**
     * Provide WebCore with the previously visted links from the history database
     * @param nativeClass TODO
     */
    CARAPI_(void) NativeProvideVisitedHistory(
        /* [in] */ Int32 nativeClass,
        /* [in] */ ArrayOf<String>* history);

    /**
     * Modifies the current selection.
     *
     * Note: Accessibility support.
     * @param nativeClass Pointer to the C++ WebViewCore object mNativeClass
     * @param direction The direction in which to alter the selection.
     * @param granularity The granularity of the selection modification.
     *
     * @return The selection string.
     */
    CARAPI_(String) NativeModifySelection(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 granularity);

    //-------------------------------------------------------------------------
    // WebViewCore private methods
    //-------------------------------------------------------------------------

    CARAPI_(AutoPtr<CWebViewCoreWebKitHitTest>) PerformHitTest(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 slop,
        /* [in] */ Boolean moveMouse);

    CARAPI_(void) ClearCache(
        /* [in] */ Boolean includeDiskFiles);

    CARAPI_(void) LoadUrl(
        /* [in] */ const String& url,
        /* [in] */ IMap* extraHeaders);

    CARAPI_(String) SaveWebArchive(
        /* [in] */ const String& filename,
        /* [in] */ Boolean autoname);

    CARAPI_(void) Key(
        /* [in] */ IKeyEvent* evt,
        /* [in] */ Int32 canTakeFocusDirection,
        /* [in] */ Boolean isDown);

    CARAPI_(void) KeyPress(
        /* [in] */ Int32 unicodeChar);

    // notify webkit that our virtual view size changed size (after inv-zoom)
    CARAPI_(void) ViewSizeChanged(
        /* [in] */ CWebViewClassic::ViewSizeData* data);

    // Calculate width to be used in webkit window.
    CARAPI_(Int32) CalculateWindowWidth(
        /* [in] */ Int32 viewWidth);

    // Utility method for exceededDatabaseQuota callback. Computes the sum
    // of WebSQL database quota for all origins.
    CARAPI_(Int64) GetUsedQuota();

    CARAPI_(void) WebkitDraw();

    CARAPI_(void) WebkitDraw(
        /* [in] */ CWebViewCoreDrawData* draw);

    CARAPI_(void) SaveViewState(
        /* [in] */ IOutputStream* stream,
        /* [in] */ IValueCallback* callback);

    //////////////////////////////////////////////////////////////////////////

    CARAPI_(void) RestoreState(
        /* [in] */ Int32 index);

    // called by JNI
    CARAPI_(void) ContentScrollTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Boolean animate,
        /* [in] */ Boolean onlyIfImeIsShowing);

    // called by JNI
    CARAPI_(void) SendNotifyProgressFinished();

    /*  Called by JNI. The coordinates are in doc coordinates, so they need to
        be scaled before they can be used by the view system, which happens
        in WebView since it (and its thread) know the current scale factor.
     */
    CARAPI_(void) SendViewInvalidate(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    // Called by JNI
    CARAPI_(AutoPtr<IWebView>) GetWebView();

    // Called by JNI
    CARAPI_(void) SendPluginDrawMsg();

    CARAPI_(void) SetViewportSettingsFromNative(
        /* [in] */ Int32 nativeClass);

    // called by JNI
    CARAPI_(void) DidFirstLayout(
        /* [in] */ Boolean standardLoad);

    // called by JNI
    CARAPI_(void) UpdateViewport();

    CARAPI_(void) SetupViewport(
        /* [in] */ Boolean updateViewState);

    // called by JNI
    CARAPI_(void) RestoreScale(
        /* [in] */ Float scale,
        /* [in] */ Float textWrapScale);

    // called by JNI
    CARAPI_(void) NeedTouchEvents(
        /* [in] */ Boolean need);

    // called by JNI
    CARAPI_(void) UpdateTextfield(
        /* [in] */ Int32 ptr,
        /* [in] */ const String& text,
        /* [in] */ Int32 textGeneration);

    CARAPI_(AutoPtr<CWebViewCoreTextSelectionData>) CreateTextSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 selPtr);

    // called by JNI
    CARAPI_(void) UpdateTextSelection(
        /* [in] */ Int32 pointer,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 textGeneration,
        /* [in] */ Int32 selectionPtr);

    // called by JNI
    CARAPI_(void) UpdateTextSizeAndScroll(
        /* [in] */ Int32 pointer,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY);

    // called by JNI
    CARAPI_(void) ClearTextEntry();

    // called by JNI
    CARAPI_(void) InitEditField(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 selectionPtr,
        /* [in] */ CWebViewCoreTextFieldInitData* initData);

    CARAPI_(void) NativeRevealSelection(
        /* [in] */ Int32 nativeClass);

    CARAPI_(String) NativeRequestLabel(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 framePtr,
        /* [in] */ Int32 nodePtr);
    /**
     * Scroll the focused textfield to (xPercent, y) in document space
     */
    CARAPI_(void) NativeScrollFocusedTextInput(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Float xPercent,
        /* [in] */ Int32 y);

    // these must be in document space (i.e. not scaled/zoomed).
    CARAPI_(void) NativeSetScrollOffset(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Boolean sendScrollEvent,
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    CARAPI_(void) NativeSetGlobalBounds(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    // called by JNI
    CARAPI_(void) RequestListBox(
        /* [in] */ ArrayOf<String>* array,
        /* [in] */ ArrayOf<Int32>* enabledArray,
        /* [in] */ ArrayOf<Int32>* selectedArray);

    // called by JNI
    CARAPI_(void) RequestListBox(
        /* [in] */ ArrayOf<String>* array,
        /* [in] */ ArrayOf<Int32>* enabledArray,
        /* [in] */ Int32 selection);

    // called by JNI
    CARAPI_(void) RequestKeyboard(
        /* [in] */ Boolean showKeyboard);

    CARAPI_(void) SetWebTextViewAutoFillable(
        /* [in] */ Int32 queryId,
        /* [in] */ const String& preview);

    // called by JNI
    CARAPI_(void) KeepScreenOn(
        /* [in] */ Boolean screenOn);

    // called by JNI
    CARAPI_(AutoPtr<IInterface>) GetPluginClass(
        /* [in] */ const String& libName,
        /* [in] */ const String& clsName);

    // called by JNI. PluginWidget function to launch a full-screen view using a
    // View object provided by the plugin class.
    CARAPI_(void) ShowFullScreenPlugin(
        /* [in] */ IInterface* childView,
        /* [in] */ Int32 orientation,
        /* [in] */ Int32 npp);

    // called by JNI
    CARAPI_(void) HideFullScreenPlugin();

    CARAPI_(AutoPtr<IInterface>) CreateSurface(
        /* [in] */ IInterface* pluginView);

    // called by JNI.  PluginWidget functions for creating an embedded View for
    // the surface drawing model.
    CARAPI_(AutoPtr<IInterface>) AddSurface(
        /* [in] */ IInterface* pluginView,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI_(void) UpdateSurface(
        /* [in] */ IInterface* childView,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI_(void) DestroySurface(
        /* [in] */ IInterface* childView);

    CARAPI_(void) ShowRect(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 contentWidth,
        /* [in] */ Int32 contentHeight,
        /* [in] */ Float xPercentInDoc,
        /* [in] */ Float xPercentInView,
        /* [in] */ Float yPercentInDoc,
        /* [in] */ Float yPercentInView);

    // called by JNI
    CARAPI_(void) CenterFitRect(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    // called by JNI
    CARAPI_(void) SetScrollbarModes(
        /* [in] */ Int32 hMode,
        /* [in] */ Int32 vMode);

    // called by JNI
    CARAPI_(void) SelectAt(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(void) SetUseMockDeviceOrientation();

    CARAPI_(void) SetUseMockGeolocation();

    CARAPI_(void) NativePause(
        /* [in] */ Int32 nativeClass);

    CARAPI_(void) NativeResume(
        /* [in] */ Int32 nativeClass);

    CARAPI_(void) NativeFreeMemory(
        /* [in] */ Int32 nativeClass);

    CARAPI_(void) NativeFullScreenPluginHidden(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 npp);

    CARAPI_(void) NativePluginSurfaceReady(
        /* [in] */ Int32 nativeClass);

    CARAPI_(AutoPtr<CWebViewCoreWebKitHitTest>) NativeHitTest(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 slop,
        /* [in] */ Boolean moveMouse);

    CARAPI_(void) NativeAutoFillForm(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 queryId);

    CARAPI_(void) NativeScrollLayer(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 layer,
        /* [in] */ IRect* rect);

    CARAPI_(Int32) NativeFindAll(
        /* [in] */ Int32 nativeClass,
        /* [in] */ const String& text);

    CARAPI_(Int32) NativeFindNext(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Boolean forward);

    /**
     * Deletes editable text between two points. Note that the selection may
     * differ from the WebView's selection because the algorithms for selecting
     * text differs for non-LTR text. Any text that isn't editable will be
     * left unchanged.
     * @param nativeClass The pointer to the native class (mNativeClass)
     * @param startX The X position of the top-left selection point.
     * @param startY The Y position of the top-left selection point.
     * @param endX The X position of the bottom-right selection point.
     * @param endY The Y position of the bottom-right selection point.
     */
    CARAPI_(void) NativeDeleteText(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 endX,
        /* [in] */ Int32 endY);

    /**
     * Inserts text at the current cursor position. If the currently-focused
     * node does not have a cursor position then this function does nothing.
     */
    CARAPI_(void) NativeInsertText(
        /* [in] */ Int32 nativeClass,
        /* [in] */ const String& text);

    /**
     * Gets the text between two selection points. Note that the selection
     * may differ from the WebView's selection because the algorithms for
     * selecting text differs for non-LTR text.
     * @param nativeClass The pointer to the native class (mNativeClass)
     * @param startX The X position of the top-left selection point.
     * @param startY The Y position of the top-left selection point.
     * @param endX The X position of the bottom-right selection point.
     * @param endY The Y position of the bottom-right selection point.
     */
    CARAPI_(String) NativeGetText(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 endX,
        /* [in] */ Int32 endY);

    CARAPI_(void) NativeSelectText(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 handleId,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(void) NativeClearTextSelection(
        /* [in] */ Int32 nativeClass);

    CARAPI_(Boolean) NativeSelectWordAt(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(void) NativeSelectAll(
        /* [in] */ Int32 nativeClass);

    CARAPI_(void) NativeSetInitialFocus(
        /* [in] */ Int32 nativeClass,
        /* [in] */ Int32 keyDirection);

    static CARAPI_(void) NativeCertTrustChanged();

    CARAPI_(void) SetNativeClass(
        /* [in] */ Int32 nativeClass);

    CARAPI_(Int32) GetNativeClass();

    CARAPI_(void) SetViewportWidth(
        /* [in] */Int32 viewportWidth);

    CARAPI_(void) SetViewportHeight(
        /* [in] */ Int32 viewportHeight);

    CARAPI_(void) SetViewportInitialScale(
        /* [in] */ Int32 viewportInitialScale);

    CARAPI_(void) SetViewportMinimumScale(
        /* [in] */ Int32 viewportMinimumScale);

    CARAPI_(void) SetViewportMaximumScale(
        /* [in] */ Int32 viewportMaximumScale);

    CARAPI_(void) SetViewportUserScalable(
        /* [in] */ Boolean viewportUserScalable);

    CARAPI_(void) SetViewportDensityDpi(
        /* [in] */ Int32 viewportDensityDpi);

    CARAPI_(void) SetDrawIsPaused(
        /* [in] */ Boolean drawIsPaused);

    CARAPI_(Int32) GetLowMemoryUsageThresholdMb();

    CARAPI_(Int32) GetHighMemoryUsageThresholdMb();

    CARAPI_(Int32) GetHighUsageDeltaMb();

    //begin callback functions
    //begin Fields callback
    static void SetNativeClass(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 nativeClass);

    static Int32 GetNativeClass(
        /* [in] */ IInterface* obj);

    static void SetViewportWidth(
        /* [in] */ IInterface* obj,
        /* [in] */Int32 viewportWidth);

    static void SetViewportHeight(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 viewportHeight);

    static void SetViewportInitialScale(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 viewportInitialScale);

    static void SetViewportMinimumScale(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 viewportMinimumScale);

    static void SetViewportMaximumScale(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 viewportMaximumScale);

    static void SetViewportUserScalable(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean viewportUserScalable);

    static void SetViewportDensityDpi(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 viewportDensityDpi);

    static void SetDrawIsPaused(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean drawIsPaused);

    static Int32 GetLowMemoryUsageThresholdMb(
        /* [in] */ IInterface* obj);

    static Int32 GetHighMemoryUsageThresholdMb(
        /* [in] */ IInterface* obj);

    static Int32 GetHighUsageDeltaMb(
        /* [in] */ IInterface* obj);
    //end Fields callback

    static void SendPluginDrawMsg(
        /* [in] */ IInterface* obj);

    //begin Methods callback
    static void ContentScrollTo(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Boolean animate,
        /* [in] */ Boolean onlyIfImeIsShowing);

    static void ContentDraw(
        /* [in] */ IInterface* obj);

    static void RequestListBox(
        /* [in] */ IInterface* obj,
        /* [in] */ ArrayOf<String>* array,
        /* [in] */ ArrayOf<Int32>* enabledArray,
        /* [in] */ ArrayOf<Int32>* selectedArray);

    static String OpenFileChooser(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& acceptType,
        /* [in] */ const String& capture);

    static void RequestSingleListBox(
        /* [in] */ IInterface* obj,
        /* [in] */ ArrayOf<String>* array,
        /* [in] */ ArrayOf<Int32>* enabledArray,
        /* [in] */ Int32 selection);

    static void JsAlert(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url,
        /* [in] */ const String& message);

    static Boolean JsConfirm(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url,
        /* [in] */ const String& message);

    static String JsPrompt(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ const String& defaultValue);

    static Boolean JsUnload(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url,
        /* [in] */ const String& message);

    static Boolean JsInterrupt(
        /* [in] */ IInterface* obj);

    static AutoPtr<IInterface> GetWebView(
        /* [in] */ IInterface* obj);

    static void DidFirstLayout(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean standardLoad);

    static void UpdateViewport(
        /* [in] */ IInterface* obj);

    static void SendNotifyProgressFinished(
        /* [in] */ IInterface* obj);

    static void SendViewInvalidate(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 right,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 bottom);

    static void UpdateTextfield(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 ptr,
        /* [in] */ const String& text,
        /* [in] */ Int32 textGeneration);

    static void UpdateTextSelection(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 pointer,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 textGeneration,
        /* [in] */ Int32 selectionPtr);

    static void UpdateTextSizeAndScroll(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 pointer,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY);

    static void ClearTextEntry(
        /* [in] */ IInterface* obj);

    static void RestoreScale(
        /* [in] */ IInterface* obj,
        /* [in] */ Float scale,
        /* [in] */ Float textWrapScale);

    static void NeedTouchEvents(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean need);

    static void RequestKeyboard(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean showKeyboard);

    static void ExceededDatabaseQuota(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url,
        /* [in] */ const String& databaseIdentifier,
        /* [in] */ Int64 quota,
        /* [in] */ Int64 estimatedDatabaseSize);

    static void ReachedMaxAppCacheSize(
        /* [in] */ IInterface* obj,
        /* [in] */ Int64 requiredStorage,
        /* [in] */ Int64 maxSize);

    static void PopulateVisitedLinks(
        /* [in] */ IInterface* obj);

    static void GeolocationPermissionsShowPrompt(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& origin);

    static void GeolocationPermissionsHidePrompt(
        /* [in] */ IInterface* obj);

    /**
      * return the DeviceMotionService and set the callbacks point by dmciCallback
      */
    static AutoPtr<IInterface> GetDeviceMotionService(
        /* [in] */ IInterface* obj,
        /* [out] */ Int32);

    static AutoPtr<IInterface> GetDeviceOrientationService(
        /* [in] */ IInterface* obj,
        /* [out] */ Int32);

    static void AddMessageToConsole(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& message,
        /* [in] */ Int32 lineNumber,
        /* [in] */ const String& sourceID,
        /* [in] */ Int32 msgLevel);

    static void FocusNodeChanged(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 nodePointer,
        /* [in] */ IInterface* hitTest);

    static AutoPtr<IInterface> GetPluginClass(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& libName,
        /* [in] */ const String& clsName);

    static void ShowFullScreenPlugin(
        /* [in] */ IInterface* obj,
        /* [in] */ IInterface* childView,
        /* [in] */ Int32 orientation,
        /* [in] */ Int32 npp);

    static void HideFullScreenPlugin(
        /* [in] */ IInterface* obj);

    static AutoPtr<IInterface> CreateSurface(
        /* [in] */ IInterface* obj,
        /* [in] */ IInterface* vpluginView);

    static AutoPtr<IInterface> AddSurface(
        /* [in] */ IInterface* obj,
        /* [in] */ IInterface* pluginView,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    static void UpdateSurface(
        /* [in] */ IInterface* obj,
        /* [in] */ IInterface* vChildView,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    static void DestroySurface(
        /* [in] */ IInterface* obj,
        /* [in] */ IInterface* vChildView);

    /*
     *@param isApplicationContext if true, get the applicationContext from context
     *                            if false, return the context;
     */
    static AutoPtr<IInterface> GetContext(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean isApplicationContext);

    static void KeepScreenOn(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean screenOn);

    static void ShowRect(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 contentWidth,
        /* [in] */ Int32 contentHeight,
        /* [in] */ Float xPercentInDoc,
        /* [in] */ Float xPercentInView,
        /* [in] */ Float yPercentInDoc,
        /* [in] */ Float yPercentInView);

    static void CenterFitRect(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    static void SetScrollbarModes(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 hMode,
        /* [in] */ Int32 vMode);

    static void ExitFullscreenVideo(
        /* [in] */ IInterface* obj);

    static void SetWebTextViewAutoFillable(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 queryId,
        /* [in] */ const String& preview);

    static void SelectAt(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    static void InitEditField(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 selectionPtr,
        /* [in] */ IInterface* initData);

    static Boolean ChromeCanTakeFocus(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 webkitDirection);

    static void ChromeTakeFocus(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 webkitDirection);

    //end Methods callback
    //end callback functions
public:
    // The thread name used to identify the WebCore thread and for use in
    // debugging other classes that require operation within the WebCore thread.
    /* package */
    static const String THREAD_NAME;

    // mAction of TouchEventData can be MotionEvent.getAction() which uses the
    // last two bytes or one of the following values
    static const Int32 ACTION_LONGPRESS = 0x100;
    static const Int32 ACTION_DOUBLETAP = 0x200;

    static const String HandlerDebugString[];

    AutoPtr<CWebViewCoreDrawData> mLastDrawData;

    static struct ElaWebViewCoreCallback sElaWebViewCoreCallback;

private:
    static const String LOGTAG;

    /*
     * WebViewCore always executes in the same thread as the native webkit.
     */

    // The WebViewClassic that corresponds to this WebViewCore.
    AutoPtr<CWebViewClassic> mWebViewClassic;
    // Proxy for handling callbacks from native code
    AutoPtr<CallbackProxy> mCallbackProxy;
    // Settings object for maintaining all settings
    AutoPtr<WebSettingsClassic> mSettings;
    // Context for initializing the BrowserFrame with the proper assets.
    AutoPtr<IContext> mContext;
    // The pointer to a native view object.
    Int32 mNativeClass;
    // The BrowserFrame is an interface to the native Frame component.
    AutoPtr<BrowserFrame> mBrowserFrame;
    // Custom JS interfaces to add during the initialization.
    AutoPtr<IMap> mJavascriptInterfaces; //HashMap<String, AutoPtr<IInterface> >
    /*
     * range is from 200 to 10,000. 0 is a special value means device-width. -1
     * means undefined.
     */
    Int32 mViewportWidth;

    /*
     * range is from 200 to 10,000. 0 is a special value means device-height. -1
     * means undefined.
     */
    Int32 mViewportHeight;

    /*
     * scale in percent, range is from 1 to 1000. 0 means undefined.
     */
    Int32 mViewportInitialScale;

    /*
     * scale in percent, range is from 1 to 1000. 0 means undefined.
     */
    Int32 mViewportMinimumScale;

    /*
     * scale in percent, range is from 1 to 1000. 0 means undefined.
     */
    Int32 mViewportMaximumScale;

    Boolean mViewportUserScalable;

    /*
     * range is from 70 to 400.
     * 0 is a special value means device-dpi. The default scale factor will be
     * always 100.
     * -1 means undefined. The default scale factor will be
     * WebView.DEFAULT_SCALE_PERCENT.
     */
    Int32 mViewportDensityDpi;

    Boolean mIsRestored;
    Float mRestoredScale;
    Float mRestoredTextWrapScale;
    Int32 mRestoredX;
    Int32 mRestoredY;

    AutoPtr<CMockGeolocation> mMockGeolocation;

    AutoPtr<DeviceMotionAndOrientationManager> mDeviceMotionAndOrientationManager;
    AutoPtr<DeviceMotionService> mDeviceMotionService;
    AutoPtr<DeviceOrientationService> mDeviceOrientationService;

    Int32 mLowMemoryUsageThresholdMb;
    Int32 mHighMemoryUsageThresholdMb;
    Int32 mHighUsageDeltaMb;

    Int32 mChromeCanFocusDirection;
    Int32 mTextSelectionChangeReason;

    // Used to determine if we should monitor the WebCore thread for responsiveness.
    // If it "hangs", for example a web page enters a while(true) loop, we will
    // prompt the user with a dialog allowing them to terminate the process.
    static Boolean sShouldMonitorWebCoreThread;

    // EventHub for processing messages
    AutoPtr<EventHub> mEventHub;
    // WebCore thread handler
    static AutoPtr<IHandler> sWebCoreHandler;

    static const Int32 TOUCH_FLAG_HIT_HANDLER = 0x1;
    static const Int32 TOUCH_FLAG_PREVENT_DEFAULT = 0x2;

    // These values are used to avirtual CARAPI_(void) requesting a layout based on old values
    Int32 mCurrentViewWidth;
    Int32 mCurrentViewHeight;
    Float mCurrentViewScale;

    // Used to avirtual CARAPI_(void) posting more than one draw message.
    Boolean mDrawIsScheduled;

    // Used to suspend drawing.
    Boolean mDrawIsPaused;

    // mInitialViewState is set by didFirstLayout() and then reset in the
    // next webkitDraw after passing the state to the UI thread.
    AutoPtr<CWebViewCoreViewState> mInitialViewState;
    Boolean mFirstLayoutForNonStandardLoad;

    Object mSkipDrawFlagLock;
    Boolean mSkipDrawFlag;
    Boolean mDrawWasSkipped;

    static Boolean sRepaintScheduled;

    static Object sLock;
    Object mLock;

    static struct ElaWebViewCoreFieldsCallback sElaWebViewCoreFieldsCallback;
    static struct ElaWebViewCoreMethodsCallback sElaWebViewCoreMethodsCallback;
    static struct ElaMediaTextureCallback sElaMediaTextureCallback;
    static struct ElaGeolocationServiceBridgeCallback sGeolocationServiceBridgeCallback;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_CWEBVIEWCORE_H__
