#include "media/media/CMediaFile.h"
#include "os/CMessageHelper.h"
#include "os/Process.h"
#include "os/Looper.h"
#include "webkit/CWebViewCore.h"
#include "webkit/CookieManagerClassic.h"
#include "webkit/SslCertLookupTable.h"
#include "webkit/CCacheManager.h"
#include "webkit/CallbackProxy.h"
#include "webkit/GeolocationPermissionsClassic.h"
#include "webkit/CPluginManager.h"
#include "webkit/CPluginManagerHelper.h"
#include "webkit/WebCoreThreadWatchdog.h"
#include "webkit/WebSettingsClassic.h"
#include "webkit/WebIconDatabase.h"
#include "webkit/WebIconDatabaseClassic.h"
#include "webkit/WebStorageClassic.h"
#include "webkit/ViewStateSerializer.h"
#include "webkit/CWebViewClassic.h"
#include "webkit/CWebView.h"
#include "webkit/BrowserFrame.h"
#include "webkit/DebugFlags.h"
#include "webkit/GeolocationService.h"
#include "location/CLocation.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/IntegralToString.h>

using Elastos::Core::IFloat;
using Elastos::Core::IBoolean;
using Elastos::Core::IntegralToString;
using Elastos::Core::CStringWrapper;
using Elastos::Core::IArrayOf;
using Elastos::Core::IString;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::CBoolean;
using Elastos::Core::CFloat;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::App::IActivityManager;
using Elastos::Droid::App::IActivityManagerMemoryInfo;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Graphics::CPoint;
using Elastos::Droid::Media::IMediaFile;
using Elastos::Droid::Media::CMediaFile;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Utility::IDisplayMetrics;

namespace Elastos {
namespace Droid {
namespace Webkit {

//=================================================================
//     CWebViewCoreTextSelectionData
//=================================================================
const Int32 CWebViewCoreTextSelectionData::REASON_UNKNOWN;
const Int32 CWebViewCoreTextSelectionData::REASON_ACCESSIBILITY_INJECTOR;
const Int32 CWebViewCoreTextSelectionData::REASON_SELECT_WORD;

CAR_INTERFACE_IMPL_LIGHT(CWebViewCoreTextSelectionData, IInterface)


//=================================================================
//     CWebViewCoreWebKitHitTest
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCoreWebKitHitTest, IInterface)

struct ElaAndroidHitTestResultCallback CWebViewCoreWebKitHitTest::sElaAndroidHitTestResultCallback =
{
    &CWebViewCoreWebKitHitTest::CreateElaHitTest,
    &CWebViewCoreWebKitHitTest::SetLinkUrl,
    &CWebViewCoreWebKitHitTest::SetAnchorText,
    &CWebViewCoreWebKitHitTest::SetImageUrl,
    &CWebViewCoreWebKitHitTest::SetAltDisplayString,
    &CWebViewCoreWebKitHitTest::SetTitle,
    &CWebViewCoreWebKitHitTest::SetEditable,
    &CWebViewCoreWebKitHitTest::SetTouchRects,
    &CWebViewCoreWebKitHitTest::SetTapHighlightColor,
    &CWebViewCoreWebKitHitTest::SetEnclosingParentRects,
    &CWebViewCoreWebKitHitTest::SetHasFocus,
    &CWebViewCoreWebKitHitTest::SetIntentUrl
};

//begin callback functions
AutoPtr<IInterface> CWebViewCoreWebKitHitTest::CreateElaHitTest()
{
    AutoPtr<CWebViewCoreWebKitHitTest> wkht = new CWebViewCoreWebKitHitTest();
    return wkht;
}

void CWebViewCoreWebKitHitTest::SetLinkUrl(
    /* [in] */ IInterface* obj,
    /* [in] */ const Elastos::String& linkUrl)
{
    ((CWebViewCoreWebKitHitTest*)obj)->mLinkUrl = linkUrl;
}

void CWebViewCoreWebKitHitTest::SetAnchorText(
    /* [in] */ IInterface* obj,
    /* [in] */ const Elastos::String& anchorText)
{
    ((CWebViewCoreWebKitHitTest*)obj)->mAnchorText = anchorText;
}

void CWebViewCoreWebKitHitTest::SetImageUrl(
    /* [in] */ IInterface* obj,
    /* [in] */ const Elastos::String& imageUrl)
{
    ((CWebViewCoreWebKitHitTest*)obj)->mImageUrl = imageUrl;
}

void CWebViewCoreWebKitHitTest::SetAltDisplayString(
    /* [in] */ IInterface* obj,
    /* [in] */ const Elastos::String& altDisplayString)
{
    ((CWebViewCoreWebKitHitTest*)obj)->mAltDisplayString = altDisplayString;
}

void CWebViewCoreWebKitHitTest::SetTitle(
    /* [in] */ IInterface* obj,
    /* [in] */ const Elastos::String& title)
{
    ((CWebViewCoreWebKitHitTest*)obj)->mTitle = title;
}

void CWebViewCoreWebKitHitTest::SetEditable(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean editable)
{
    ((CWebViewCoreWebKitHitTest*)obj)->mEditable = editable;
}

void CWebViewCoreWebKitHitTest::SetTouchRects(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 array,
    /* [in] */ Int32 size)
{
    ElaRectElement* rectArray = (ElaRectElement*)array;
    AutoPtr< ArrayOf<IRect*> > touchRects = ArrayOf<IRect*>::Alloc(size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IRect> rect;
        CRect::New(rectArray[i].x, rectArray[i].y,
                rectArray[i].maxX, rectArray[i].maxY, (IRect**)&rect);
        touchRects->Set(i, rect);
    }
    ((CWebViewCoreWebKitHitTest*)obj)->mTouchRects = touchRects;
}

void CWebViewCoreWebKitHitTest::SetTapHighlightColor(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 tapHighlightColor)
{
    ((CWebViewCoreWebKitHitTest*)obj)->mTapHighlightColor = tapHighlightColor;
}

void CWebViewCoreWebKitHitTest::SetEnclosingParentRects(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 array,
    /* [in] */ Int32 size)
{
    ElaRectElement* rectArray = (ElaRectElement*)array;
    AutoPtr< ArrayOf<IRect*> > enclosingParentRects = ArrayOf<IRect*>::Alloc(size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IRect> rect;
        CRect::New(rectArray[i].x, rectArray[i].y,
                rectArray[i].maxX, rectArray[i].maxY, (IRect**)&rect);
        enclosingParentRects->Set(i, rect);
    }
    ((CWebViewCoreWebKitHitTest*)obj)->mEnclosingParentRects = enclosingParentRects;
}

void CWebViewCoreWebKitHitTest::SetHasFocus(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean hasFocus)
{
    ((CWebViewCoreWebKitHitTest*)obj)->mHasFocus = hasFocus;
}

void CWebViewCoreWebKitHitTest::SetIntentUrl(
    /* [in] */ IInterface* obj,
    /* [in] */ const Elastos::String& intentUrl)
{
    ((CWebViewCoreWebKitHitTest*)obj)->mIntentUrl = intentUrl;
}

//end callback functions

//=================================================================
//     CWebViewCoreAutoFillData
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCoreAutoFillData, IInterface)


//=================================================================
//     CWebViewCoreTextFieldInitData
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCoreTextFieldInitData, IInterface)

struct ElaWebViewCoreTextFieldInitDataCallback CWebViewCoreTextFieldInitData::sElaWebViewCoreTextFieldInitDataCallback =
{
    &CWebViewCoreTextFieldInitData::CreateTextFieldInitData,
    &CWebViewCoreTextFieldInitData::SetFieldPointer,
    &CWebViewCoreTextFieldInitData::SetText,
    &CWebViewCoreTextFieldInitData::SetType,
    &CWebViewCoreTextFieldInitData::SetIsSpellCheckEnabled,
    &CWebViewCoreTextFieldInitData::SetIsTextFieldNext,
    &CWebViewCoreTextFieldInitData::SetIsTextFieldPrev,
    &CWebViewCoreTextFieldInitData::SetIsAutoCompleteEnabled,
    &CWebViewCoreTextFieldInitData::SetName,
    &CWebViewCoreTextFieldInitData::SetLabel,
    &CWebViewCoreTextFieldInitData::SetMaxLength,
    &CWebViewCoreTextFieldInitData::SetContentBounds,
    &CWebViewCoreTextFieldInitData::SetNodeLayerId,
    &CWebViewCoreTextFieldInitData::SetClientRect,
};

//begin callback functions
AutoPtr<IInterface> CWebViewCoreTextFieldInitData::CreateTextFieldInitData()
{
    AutoPtr<CWebViewCoreTextFieldInitData> tfid = new CWebViewCoreTextFieldInitData();
    return tfid;
}

void CWebViewCoreTextFieldInitData::SetFieldPointer(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 fieldPointer)
{
    ((CWebViewCoreTextFieldInitData*)obj)->mFieldPointer = fieldPointer;
}

void CWebViewCoreTextFieldInitData::SetText(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& text)
{
    ((CWebViewCoreTextFieldInitData*)obj)->mText = text;
}

void CWebViewCoreTextFieldInitData::SetType(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 type)
{
    ((CWebViewCoreTextFieldInitData*)obj)->mType = type;
}

void CWebViewCoreTextFieldInitData::SetIsSpellCheckEnabled(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean isSpellCheckEnabled)
{
    ((CWebViewCoreTextFieldInitData*)obj)->mIsSpellCheckEnabled = isSpellCheckEnabled;
}

void CWebViewCoreTextFieldInitData::SetIsTextFieldNext(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean isTextFieldNext)
{
    ((CWebViewCoreTextFieldInitData*)obj)->mIsTextFieldNext = isTextFieldNext;
}

void CWebViewCoreTextFieldInitData::SetIsTextFieldPrev(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean isTextFieldPrev)
{
    ((CWebViewCoreTextFieldInitData*)obj)->mIsTextFieldPrev = isTextFieldPrev;
}

void CWebViewCoreTextFieldInitData::SetIsAutoCompleteEnabled(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean isAutoCompleteEnabled)
{
    ((CWebViewCoreTextFieldInitData*)obj)->mIsAutoCompleteEnabled = isAutoCompleteEnabled;
}

void CWebViewCoreTextFieldInitData::SetName(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& name)
{
    ((CWebViewCoreTextFieldInitData*)obj)->mName = name;
}

void CWebViewCoreTextFieldInitData::SetLabel(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& label)
{
    ((CWebViewCoreTextFieldInitData*)obj)->mLabel = label;
}

void CWebViewCoreTextFieldInitData::SetMaxLength(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 maxLength)
{
    ((CWebViewCoreTextFieldInitData*)obj)->mMaxLength = maxLength;
}

void CWebViewCoreTextFieldInitData::SetContentBounds(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    AutoPtr<IRect> contentBounds;
    CRect::New(x, y, x + width, y - height, (IRect**)&(contentBounds));
    ((CWebViewCoreTextFieldInitData*)obj)->mContentBounds = contentBounds;
}

void CWebViewCoreTextFieldInitData::SetNodeLayerId(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 nodeLayerId)
{
    ((CWebViewCoreTextFieldInitData*)obj)->mNodeLayerId = nodeLayerId;
}

void CWebViewCoreTextFieldInitData::SetClientRect(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    AutoPtr<IRect> clientRect;
    CRect::New(x, y, x + width, y - height, (IRect**)&(clientRect));
    ((CWebViewCoreTextFieldInitData*)obj)->mClientRect = clientRect;
}

//end callback functions

//=================================================================
//     CWebViewCoreFindAllRequest
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCoreFindAllRequest, IInterface)


//=================================================================
//     CWebViewCoreViewState
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCoreViewState, IInterface)


//=================================================================
//     CWebViewCoreDrawData
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCoreDrawData, IInterface)


//=================================================================
//     CWebViewCore::WebCoreThread::MyHandler
//=================================================================
ECode CWebViewCore::WebCoreThread::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case INITIALIZE: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCore* core = (CWebViewCore*)(IWebViewCore*)obj.Get();
            core->Initialize();
            break;
        }
        case REDUCE_PRIORITY:
            // 3 is an adjustable number.
            Process::SetThreadPriority(
                    IProcess::THREAD_PRIORITY_DEFAULT + 3 *
                    IProcess::THREAD_PRIORITY_LESS_FAVORABLE);
            break;

        case RESUME_PRIORITY:
            Process::SetThreadPriority(
                    IProcess::THREAD_PRIORITY_DEFAULT);
            break;

        case CWebViewCore::EventHub::ADD_PACKAGE_NAME: {
            if (BrowserFrame::sElastosBridge == NULL) {
                // throw new IllegalStateException(
                //         "No WebView has been created in this process!");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            String pname;
            ICharSequence::Probe(obj)->ToString(&pname);
            BrowserFrame::sElastosBridge->AddPackageName(pname);
            break;
        }
        case CWebViewCore::EventHub::REMOVE_PACKAGE_NAME: {
            if (BrowserFrame::sElastosBridge == NULL) {
                // throw new IllegalStateException(
                //         "No WebView has been created in this process!");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            String pname;
            ICharSequence::Probe(obj)->ToString(&pname);
            BrowserFrame::sElastosBridge->RemovePackageName(pname);
            break;
        }
        case CWebViewCore::EventHub::PROXY_CHANGED: {
            if (BrowserFrame::sElastosBridge == NULL) {
                // throw new IllegalStateException(
                //         "No WebView has been created in this process!");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            BrowserFrame::sElastosBridge->UpdateProxy(IProxyProperties::Probe(obj));
            break;
        }
        case CWebViewCore::EventHub::HEARTBEAT: {
            // Ping back the watchdog to let it know we're still processing
            // messages.
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IMessage* m = IMessage::Probe(obj);
            m->SendToTarget();
            break;
        }
        case CWebViewCore::EventHub::TRUST_STORAGE_UPDATED:
            // post a task to network thread for updating trust manager
            mOwner->NativeCertTrustChanged();
            assert(0);
            // CertificateChainValidator.handleTrustStorageUpdate();
            break;
    }
    return NOERROR;
}


//=================================================================
//     CWebViewCore::WebCoreThread
//=================================================================
// Message id for initializing a new WebViewCore.
const Int32 CWebViewCore::WebCoreThread::INITIALIZE;
const Int32 CWebViewCore::WebCoreThread::REDUCE_PRIORITY;
const Int32 CWebViewCore::WebCoreThread::RESUME_PRIORITY;

ECode CWebViewCore::WebCoreThread::Run()
{
    if (DebugFlags::WEB_VIEW_CORE) {
        Logger::V("CWebViewCore::WebCoreThread", "WebCoreThread::Run: Line: %d", __LINE__);
    }

    Looper::Prepare();
    assert(sWebCoreHandler != NULL);
    {
        Object::Autolock lock(CWebViewCore::sLock);

        sWebCoreHandler = new MyHandler(mOwner);
        CWebViewCore::sLock.Notify();
    }
    Looper::Loop();
    return NOERROR;
}


//=================================================================
//     CWebViewCore::BaseUrlData
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::BaseUrlData, IInterface)


//=================================================================
//     CWebViewCore::JSInterfaceData
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::JSInterfaceData, IInterface)


//=================================================================
//     CWebViewCore::JSKeyData
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::JSKeyData, IInterface)


//=================================================================
//     CWebViewCore::GetUrlData
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::GetUrlData, IInterface)


//=================================================================
//     CWebViewCore::PostUrlData
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::PostUrlData, IInterface)


//=================================================================
//     CWebViewCore::ReplaceTextData
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::ReplaceTextData, IInterface)


//=================================================================
//     CWebViewCore::GeolocationPermissionsData
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::GeolocationPermissionsData, IInterface)


//=================================================================
//     CWebViewCore::SaveViewStateRequest
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::SaveViewStateRequest, IInterface)


//=================================================================
//     CWebViewCore::EventHub::MyHandler
//=================================================================
ECode CWebViewCore::EventHub::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    if (DebugFlags::WEB_VIEW_CORE) {
        Int32 arg1, arg2;
        msg->GetArg1(&arg1);
        msg->GetArg2(&arg2);
        AutoPtr<IInterface> obj;
        msg->GetObj((IInterface**)&obj);
        if (what != 135 && what != 196 && what != 130)
        Logger::V(LOGTAG, "messageid:%d, %s arg1=%d arg2=%d obj=%p", what, what < FIRST_PACKAGE_MSG_ID
                || what > LAST_PACKAGE_MSG_ID
                ? IntegralToString::Int32ToString(what).string()
                : CWebViewCore::HandlerDebugString[what
                        - FIRST_PACKAGE_MSG_ID].string(), arg1, arg2, obj.Get());
    }
    switch (what) {
    case PAUSE_TIMERS:
        Process::GetThreadPriority(mOwner->mTid, &mOwner->mSavedPriority);
        Process::SetThreadPriority(mOwner->mTid, IProcess::THREAD_PRIORITY_BACKGROUND);
        mOwner->mOwner->PauseTimers();
        if (mOwner->mOwner->mNativeClass != 0) {
            mOwner->mOwner->NativeCloseIdleConnections(mOwner->mOwner->mNativeClass);
        }
        return NOERROR;

    case RESUME_TIMERS:
        Process::SetThreadPriority(mOwner->mTid, mOwner->mSavedPriority);
        mOwner->mOwner->ResumeTimers();
        return NOERROR;
    }

    if (mOwner->mOwner->mWebViewClassic == NULL || mOwner->mOwner->mNativeClass == 0) {
        if (DebugFlags::WEB_VIEW_CORE) {
            Logger::W(LOGTAG, "Rejecting message %d because we are destroyed", what);
        }
        return NOERROR;
    }
    if (mOwner->mDestroying == TRUE
            && what != EventHub::DESTROY) {
        if (DebugFlags::WEB_VIEW_CORE) {
            Logger::V(LOGTAG, "Rejecting message %d because we are being destroyed", what);
        }
        return NOERROR;
    }
    switch (what) {
        case WEBKIT_DRAW:
            mOwner->mOwner->WebkitDraw();
            break;

        case DESTROY:
            // Time to take down the world. Cancel all pending
            // loads and destroy the native view and frame.
            {
                Mutex::Autolock lock(mOwner->mOwner->_m_syncLock);

                mOwner->mOwner->mCallbackProxy->Shutdown();
                // Wake up the WebCore thread just in case it is waiting for a
                // JavaScript dialog.
                {
                    Object::Autolock lock(mOwner->mOwner->mCallbackProxy);
                    mOwner->mOwner->mCallbackProxy->Notify();
                }
                mOwner->mOwner->mBrowserFrame->Destroy();
                mOwner->mOwner->mBrowserFrame = NULL;
                mOwner->mOwner->mSettings->OnDestroyed();
                mOwner->mOwner->mNativeClass = 0;
                mOwner->mOwner->mWebViewClassic = NULL;
            }
            break;

        case REVEAL_SELECTION:
            mOwner->mOwner->NativeRevealSelection(mOwner->mOwner->mNativeClass);
            break;

        case SCROLL_TEXT_INPUT: {
            Float xPercent;
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            if (obj == NULL) {
                xPercent = 0;
            }
            else {
                IFloat::Probe(obj)->GetValue(&xPercent);
            }
            Int32 arg2;
            msg->GetArg2(&arg2);
            mOwner->mOwner->NativeScrollFocusedTextInput(
                    mOwner->mOwner->mNativeClass, xPercent, arg2);
            break;
        }

        case LOAD_URL: {
            if (DebugFlags::WEB_VIEW_CORE)
                Logger::D(LOGTAG, "CWebViewCore::HandleMessage receive message EventHub::LOAD_URL");
            CookieManagerClassic::GetInstance()->WaitForCookieOperationsToComplete();
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCore::GetUrlData* param = (CWebViewCore::GetUrlData*)obj.Get();
            mOwner->mOwner->LoadUrl(param->mUrl, param->mExtraHeaders);
            break;
        }

        case POST_URL: {
            CookieManagerClassic::GetInstance()->WaitForCookieOperationsToComplete();
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCore::PostUrlData* param = (CWebViewCore::PostUrlData*)obj.Get();
            mOwner->mOwner->mBrowserFrame->PostUrl(param->mUrl, param->mPostData);
            break;
        }
        case LOAD_DATA: {
            CookieManagerClassic::GetInstance()->WaitForCookieOperationsToComplete();
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCore::BaseUrlData* loadParams = (CWebViewCore::BaseUrlData*)obj.Get();
            String baseUrl = loadParams->mBaseUrl;
            if (!baseUrl.IsNull()) {
                Int32 i = baseUrl.IndexOf(':');
                if (i > 0) {
                    // In 1.0, WebView.loadDataWithBaseURL() could access local
                    // asset files using 'file' scheme URLs as long as the data is
                    // valid. Later versions of WebKit have tightened the
                    // restriction around when pages can access such local URLs.
                    // To maintain compatibility with 1.0, we register the scheme of
                    // the baseUrl to be considered local, as long as it is not
                    // http(s)/ftp(s)/about/javascript.
                    String scheme = baseUrl.Substring(0, i);
                    if (!scheme.StartWith("http") &&
                            !scheme.StartWith("ftp") &&
                            !scheme.StartWith("about") &&
                            !scheme.StartWith("javascript")) {
                        mOwner->mOwner->NativeRegisterURLSchemeAsLocal(mOwner->mOwner->mNativeClass,
                                scheme);
                    }
                }
            }
            mOwner->mOwner->mBrowserFrame->LoadData(baseUrl,
                    loadParams->mData,
                    loadParams->mMimeType,
                    loadParams->mEncoding,
                    loadParams->mHistoryUrl);
            mOwner->mOwner->NativeContentInvalidateAll(mOwner->mOwner->mNativeClass);
            break;
        }
        case STOP_LOADING:
            // If the WebCore has committed the load, but not
            // finished the first layout yet, we need to set
            // first layout done to trigger the interpreted side sync
            // up with native side
            if (mOwner->mOwner->mBrowserFrame->Committed()
                    && !mOwner->mOwner->mBrowserFrame->FirstLayoutDone()) {
                mOwner->mOwner->mBrowserFrame->DidFirstLayout();
            }
            // Do this after syncing up the layout state.
            mOwner->mOwner->StopLoading();
            break;

        case RELOAD:
            mOwner->mOwner->mBrowserFrame->Reload(FALSE);
            break;

        case KEY_DOWN: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->Key(IKeyEvent::Probe(obj), arg1, TRUE);
            break;
        }
        case KEY_UP: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->Key(IKeyEvent::Probe(obj), arg1, FALSE);
            break;
        }
        case KEY_PRESS: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->KeyPress(arg1);
            break;
        }
        case VIEW_SIZE_CHANGED: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewClassic::ViewSizeData* data = (CWebViewClassic::ViewSizeData*)obj.Get();
            mOwner->mOwner->ViewSizeChanged(data);
            break;
        }
        case SET_SCROLL_OFFSET: {
            // note: these are in document coordinates
            // (inv-zoom)
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IPoint* pt = IPoint::Probe(obj);
            Int32 arg1, x, y;
            msg->GetArg1(&arg1);
            pt->GetX(&x);
            pt->GetY(&y);
            mOwner->mOwner->NativeSetScrollOffset(mOwner->mOwner->mNativeClass,
                    arg1 == 1, x, y);
            break;
        }
        case SET_GLOBAL_BOUNDS: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IRect* r = IRect::Probe(obj);
            Int32 left, top, width, height;
            r->GetLeft(&left);
            r->GetTop(&top);
            r->GetWidth(&width);
            r->GetHeight(&height);
            mOwner->mOwner->NativeSetGlobalBounds(mOwner->mOwner->mNativeClass, left, top,
                width, height);
            break;
        }
        case GO_BACK_FORWARD: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            // If it is a standard load and the load is not
            // committed yet, we interpret BACK as RELOAD
            if (!mOwner->mOwner->mBrowserFrame->Committed() && arg1 == -1 &&
                    (mOwner->mOwner->mBrowserFrame->LoadType() ==
                    BrowserFrame::FRAME_LOADTYPE_STANDARD)) {
                mOwner->mOwner->mBrowserFrame->Reload(TRUE);
            }
            else {
                mOwner->mOwner->mBrowserFrame->GoBackOrForward(arg1);
            }
            break;
        }
        case RESTORE_STATE: {
            mOwner->mOwner->StopLoading();
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->RestoreState(arg1);
            break;
        }

        case ON_PAUSE:
            mOwner->mOwner->NativePause(mOwner->mOwner->mNativeClass);
            break;

        case ON_RESUME:
            mOwner->mOwner->NativeResume(mOwner->mOwner->mNativeClass);
            break;

        case FREE_MEMORY:
            mOwner->mOwner->ClearCache(FALSE);
            mOwner->mOwner->NativeFreeMemory(mOwner->mOwner->mNativeClass);
            break;

        case SET_NETWORK_STATE: {
            if (BrowserFrame::sElastosBridge == NULL) {
                // throw new IllegalStateException("No WebView " +
                //         "has been created in this process!");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
            Int32 arg1;
            msg->GetArg1(&arg1);
            BrowserFrame::sElastosBridge->SetNetworkOnLine(arg1 == 1);
            break;
        }
        case SET_NETWORK_TYPE: {
            if (BrowserFrame::sElastosBridge == NULL) {
                // throw new IllegalStateException("No WebView " +
                //         "has been created in this process!");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IMap* map = IMap::Probe(obj);
            AutoPtr<ICharSequence> key;
            CStringWrapper::New(String("type"), (ICharSequence**)&key);
            AutoPtr<IInterface> value;
            map->Get(key, (IInterface**)&value);
            String type;
            ICharSequence::Probe(value)->ToString(&type);
            key = NULL;
            CStringWrapper::New(String("subtype"), (ICharSequence**)&key);
            value = NULL;
            map->Get(key, (IInterface**)&value);
            String subtype;
            ICharSequence::Probe(value)->ToString(&subtype);
            BrowserFrame::sElastosBridge->SetNetworkType(type, subtype);
            break;
        }
        case CLEAR_CACHE: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->ClearCache(arg1 == 1);
            break;
        }
        case CLEAR_HISTORY:
            mOwner->mOwner->mCallbackProxy->GetBackForwardList()->Close(
                    mOwner->mOwner->mBrowserFrame->mNativeFrame);
            break;

        case REPLACE_TEXT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            ReplaceTextData* rep = (ReplaceTextData*)obj.Get();
            Int32 arg1, arg2;
            msg->GetArg1(&arg1);
            msg->GetArg2(&arg2);
            mOwner->mOwner->NativeReplaceTextfieldText(
                    mOwner->mOwner->mNativeClass, arg1,
                    arg2, rep->mReplace, rep->mNewStart,
                    rep->mNewEnd, rep->mTextGeneration);
            break;
        }
        case PASS_TO_JS: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            JSKeyData* jsData = (JSKeyData*)obj.Get();
            IKeyEvent* evt = jsData->mEvent;
            Int32 keyCode;
            evt->GetKeyCode(&keyCode);
            Int32 keyValue;
            evt->GetUnicodeChar(&keyValue);
            Int32 generation;
            msg->GetArg1(&generation);
            Boolean isDown, isShiftPressed, isAltPressed, isSymPressed;
            evt->IsDown(&isDown);
            evt->IsShiftPressed(&isShiftPressed);
            evt->IsAltPressed(&isAltPressed);
            evt->IsSymPressed(&isSymPressed);
            mOwner->mOwner->PassToJs(mOwner->mOwner->mNativeClass,
                    generation, jsData->mCurrentText,
                    keyCode, keyValue, isDown, isShiftPressed,
                    isAltPressed, isSymPressed);
            break;
        }

        case SAVE_DOCUMENT_STATE: {
            mOwner->mOwner->NativeSaveDocumentState(mOwner->mOwner->mNativeClass);
            break;
        }

        case CLEAR_SSL_PREF_TABLE:
            // FIXME: This will not work for connections currently in use, as
            // they cache the certificate responses. See http://b/5324235.
            SslCertLookupTable::GetInstance()->Clear();
            mOwner->mOwner->NativeCloseIdleConnections(mOwner->mOwner->mNativeClass);
            break;

        case SET_ACTIVE: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->NativeSetFocusControllerActive(mOwner->mOwner->mNativeClass, arg1 == 1);
            break;
        }
        case ADD_JS_INTERFACE: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            JSInterfaceData* jsData = (JSInterfaceData*)obj.Get();
            mOwner->mOwner->mBrowserFrame->AddJavascriptInterface(jsData->mObject,
                    jsData->mInterfaceName, jsData->mRequireAnnotation);
            break;
        }

        case REMOVE_JS_INTERFACE: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            JSInterfaceData* jsData = (JSInterfaceData*)obj.Get();
            mOwner->mOwner->mBrowserFrame->RemoveJavascriptInterface(
                    jsData->mInterfaceName);
            break;
        }
        case REQUEST_EXT_REPRESENTATION: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            mOwner->mOwner->mBrowserFrame->ExternalRepresentation(
                    IMessage::Probe(obj));
            break;
        }
        case REQUEST_DOC_AS_TEXT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            mOwner->mOwner->mBrowserFrame->DocumentAsText(IMessage::Probe(obj));
            break;
        }
        case SET_MOVE_MOUSE: {
            Int32 arg1, arg2;
            msg->GetArg1(&arg1);
            msg->GetArg2(&arg2);
            mOwner->mOwner->NativeMoveMouse(mOwner->mOwner->mNativeClass, arg1, arg2);
            break;
        }
        case REQUEST_CURSOR_HREF: {
            Int32 arg1, arg2;
            msg->GetArg1(&arg1);
            msg->GetArg2(&arg2);
            AutoPtr<CWebViewCoreWebKitHitTest> hit = mOwner->mOwner->PerformHitTest(arg1, arg2, 1, FALSE);
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IMessage* hrefMsg = IMessage::Probe(obj);
            AutoPtr<IBundle> data;
            hrefMsg->GetData((IBundle**)&data);
            data->PutString(CWebViewClassic::FocusNodeHref::URL, hit->mLinkUrl);
            data->PutString(CWebViewClassic::FocusNodeHref::TITLE, hit->mAnchorText);
            data->PutString(CWebViewClassic::FocusNodeHref::SRC, hit->mImageUrl);
            hrefMsg->SendToTarget();
            break;
        }

        case DOC_HAS_IMAGES: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IMessage* imageResult = IMessage::Probe(obj);
            imageResult->SetArg1(mOwner->mOwner->mBrowserFrame->DocumentHasImages() ? 1 : 0);
            imageResult->SendToTarget();
            break;
        }
        case DELETE_SELECTION: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCoreTextSelectionData* deleteSelectionData
                    = (CWebViewCoreTextSelectionData*)obj.Get();
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->NativeDeleteSelection(mOwner->mOwner->mNativeClass,
                    deleteSelectionData->mStart, deleteSelectionData->mEnd, arg1);
            break;
        }
        case SET_SELECTION: {
            Int32 arg1, arg2;
            msg->GetArg1(&arg1);
            msg->GetArg2(&arg2);
            mOwner->mOwner->NativeSetSelection(mOwner->mOwner->mNativeClass, arg1, arg2);
            break;
        }
        case MODIFY_SELECTION: {
            mOwner->mOwner->mTextSelectionChangeReason
                    = CWebViewCoreTextSelectionData::REASON_ACCESSIBILITY_INJECTOR;
            Int32 arg1, arg2;
            msg->GetArg1(&arg1);
            msg->GetArg2(&arg2);
            String modifiedSelectionString =
                mOwner->mOwner->NativeModifySelection(mOwner->mOwner->mNativeClass, arg1, arg2);
            AutoPtr<IMessageHelper> helper;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
            AutoPtr<ICharSequence> cs;
            CStringWrapper::New(modifiedSelectionString, (ICharSequence**)&cs);
            AutoPtr<IMessage> msg;
            helper->Obtain(mOwner->mOwner->mWebViewClassic->mPrivateHandler,
                    CWebViewClassic::SELECTION_STRING_CHANGED, cs.Get(), (IMessage**)&msg);
            msg->SendToTarget();
            mOwner->mOwner->mTextSelectionChangeReason
                    = CWebViewCoreTextSelectionData::REASON_UNKNOWN;
            break;
        }
        case LISTBOX_CHOICES: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IObjectInt32Map* choices = IObjectInt32Map::Probe(obj);
            Int32 choicesSize;
            msg->GetArg1(&choicesSize);
            AutoPtr< ArrayOf<Boolean> > choicesArray = ArrayOf<Boolean>::Alloc(choicesSize);
            for (Int32 c = 0; c < choicesSize; c++) {
                AutoPtr<IInterface> value;
                choices->Get(c, (IInterface**)&value);
                Boolean bv;
                IBoolean::Probe(value)->GetValue(&bv);
                (*choicesArray)[c] = bv;
            }
            mOwner->mOwner->NativeSendListBoxChoices(mOwner->mOwner->mNativeClass,
                    choicesArray, choicesSize);
            break;
        }
        case SINGLE_LISTBOX_CHOICE: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->NativeSendListBoxChoice(mOwner->mOwner->mNativeClass, arg1);
            break;
        }
        case SET_BACKGROUND_COLOR: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->NativeSetBackgroundColor(mOwner->mOwner->mNativeClass, arg1);
            break;
        }
        case DUMP_DOMTREE: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->NativeDumpDomTree(mOwner->mOwner->mNativeClass, arg1 == 1);
            break;
        }
        case DUMP_RENDERTREE: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->NativeDumpRenderTree(mOwner->mOwner->mNativeClass, arg1 == 1);
            break;
        }
        case SET_JS_FLAGS: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            String jsFlags;
            ICharSequence::Probe(obj)->ToString(&jsFlags);
            mOwner->mOwner->NativeSetJsFlags(mOwner->mOwner->mNativeClass, jsFlags);
            break;
        }
        case CONTENT_INVALIDATE_ALL:
            mOwner->mOwner->NativeContentInvalidateAll(mOwner->mOwner->mNativeClass);
            break;

        case SAVE_WEBARCHIVE: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewClassic::SaveWebArchiveMessage* saveMessage =
                (CWebViewClassic::SaveWebArchiveMessage*)obj.Get();
            saveMessage->mResultFile =
                mOwner->mOwner->SaveWebArchive(saveMessage->mBasename, saveMessage->mAutoname);
            AutoPtr<IMessageHelper> helper;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
            AutoPtr<IMessage> msg;
            helper->Obtain(mOwner->mOwner->mWebViewClassic->mPrivateHandler,
                    CWebViewClassic::SAVE_WEBARCHIVE_FINISHED, (IInterface*)saveMessage, (IMessage**)&msg);
            msg->SendToTarget();
            break;
        }
        case GEOLOCATION_PERMISSIONS_PROVIDE: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            GeolocationPermissionsData* data =
                    (GeolocationPermissionsData*)obj.Get();
            mOwner->mOwner->NativeGeolocationPermissionsProvide(mOwner->mOwner->mNativeClass,
                    data->mOrigin, data->mAllow, data->mRemember);
            break;
        }
        case CLEAR_CONTENT:
            // Clear the view so that onDraw() will draw nothing
            // but white background
            // (See public method WebView.clearView)
            mOwner->mOwner->ClearContent();
            break;

        case MESSAGE_RELAY: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IMessage::Probe(obj)->SendToTarget();
            break;
        }
        case POPULATE_VISITED_LINKS: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IArrayOf* arrayObj = IArrayOf::Probe(obj);
            Int32 size;
            AutoPtr< ArrayOf<String> > history = ArrayOf<String>::Alloc(size);
            for (Int32 i = 0; i < size; i++) {
                AutoPtr<IInterface> value;
                arrayObj->Get(i, (IInterface**)&value);
                String str;
                ICharSequence::Probe(value)->ToString(&str);
                (*history)[i] = str;
            }
            mOwner->mOwner->NativeProvideVisitedHistory(mOwner->mOwner->mNativeClass, history);
            break;
        }
        case HIDE_FULLSCREEN: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->NativeFullScreenPluginHidden(mOwner->mOwner->mNativeClass, arg1);
            break;
        }
        case PLUGIN_SURFACE_READY:
            mOwner->mOwner->NativePluginSurfaceReady(mOwner->mOwner->mNativeClass);
            break;

        case NOTIFY_ANIMATION_STARTED:
            mOwner->mOwner->NativeNotifyAnimationStarted(mOwner->mOwner->mNativeClass);
            break;

        case ADD_PACKAGE_NAMES: {
            if (BrowserFrame::sElastosBridge == NULL) {
                // throw new IllegalStateException("No WebView " +
                //         "has been created in this process!");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr< ArrayOf<IInterface*> > values;
            ISet::Probe(obj)->ToArray((ArrayOf<IInterface*>**)&values);
            AutoPtr< HashSet<String> > packages;
            if (values != NULL) {
                packages = new HashSet<String>();
                for (Int32 i = 0; i < values->GetLength(); i++) {
                    AutoPtr<ICharSequence> value = ICharSequence::Probe((*values)[i]);
                    String package;
                    value->ToString(&package);
                    packages->Insert(package);
                }
            }
            BrowserFrame::sElastosBridge->AddPackageNames(packages);
            break;
        }
        case SET_USE_MOCK_GEOLOCATION:
            mOwner->mOwner->SetUseMockGeolocation();
            break;

        case SET_USE_MOCK_DEVICE_ORIENTATION:
            mOwner->mOwner->SetUseMockDeviceOrientation();
            break;

        case AUTOFILL_FORM: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->NativeAutoFillForm(mOwner->mOwner->mNativeClass, arg1);
            AutoPtr<IMessageHelper> helper;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
            AutoPtr<IMessage> msg;
            helper->Obtain(mOwner->mOwner->mWebViewClassic->mPrivateHandler,
                    CWebViewClassic::AUTOFILL_COMPLETE, NULL, (IMessage**)&msg);
            msg->SendToTarget();
            break;
        }
        case EXECUTE_JS: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            if (IString::Probe(obj) != NULL) {
                String str;
                ICharSequence::Probe(obj)->ToString(&str);
                if (DebugFlags::WEB_VIEW_CORE) {
                    Logger::D(LOGTAG, "Executing JS : %s", str.string());
                }
                mOwner->mOwner->mBrowserFrame->StringByEvaluatingJavaScriptFromString(str);
            }
            break;
        }
        case SCROLL_LAYER: {
            Int32 nativeLayer;
            msg->GetArg1(&nativeLayer);
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IRect* rect = IRect::Probe(obj);
            mOwner->mOwner->NativeScrollLayer(mOwner->mOwner->mNativeClass, nativeLayer,
                    rect);
            break;
        }
        case DELETE_TEXT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<IArrayOf> handlesObj = IArrayOf::Probe(obj);
            Int32 size;
            handlesObj->GetLength(&size);
            assert(size >= 4);
            Int32 handles[4];
            for (Int32 i = 0; i < 4; i++) {
                AutoPtr<IInterface> value;
                handlesObj->Get(i, (IInterface**)&value);
                Int32 iv;
                IInteger32::Probe(value)->GetValue(&iv);
                handles[i] = iv;
            }
            mOwner->mOwner->NativeDeleteText(mOwner->mOwner->mNativeClass, handles[0],
                    handles[1], handles[2], handles[3]);
            break;
        }
        case COPY_TEXT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<IArrayOf> handlesObj = IArrayOf::Probe(obj);
            Int32 size;
            handlesObj->GetLength(&size);
            assert(size >= 4);
            Int32 handles[4];
            for (Int32 i = 0; i < 4; i++) {
                AutoPtr<IInterface> value;
                handlesObj->Get(i, (IInterface**)&value);
                Int32 iv;
                IInteger32::Probe(value)->GetValue(&iv);
                handles[i] = iv;
            }
            String copiedText = mOwner->mOwner->NativeGetText(mOwner->mOwner->mNativeClass,
                    handles[0], handles[1], handles[2],
                    handles[3]);
            if (!copiedText.IsNull()) {
                AutoPtr<IMessageHelper> helper;
                CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
                AutoPtr<ICharSequence> cs;
                CStringWrapper::New(copiedText, (ICharSequence**)&cs);
                AutoPtr<IMessage> msg;
                helper->Obtain(mOwner->mOwner->mWebViewClassic->mPrivateHandler,
                        CWebViewClassic::COPY_TO_CLIPBOARD, cs, (IMessage**)&msg);
                msg->SendToTarget();
            }
            break;
        }
        case INSERT_TEXT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            String text;
            ICharSequence::Probe(obj)->ToString(&text);
            mOwner->mOwner->NativeInsertText(mOwner->mOwner->mNativeClass, text);
            break;
        }
        case SELECT_TEXT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            Int32 handleId;
            IInteger32::Probe(obj)->GetValue(&handleId);
            Int32 arg1, arg2;
            msg->GetArg1(&arg1);
            msg->GetArg2(&arg2);
            mOwner->mOwner->NativeSelectText(mOwner->mOwner->mNativeClass, handleId,
                    arg1, arg2);
            break;
        }
        case SELECT_WORD_AT: {
            mOwner->mOwner->mTextSelectionChangeReason
                    = CWebViewCoreTextSelectionData::REASON_SELECT_WORD;
            Int32 x, y;
            msg->GetArg1(&x);
            msg->GetArg2(&y);
            if (!mOwner->mOwner->NativeSelectWordAt(mOwner->mOwner->mNativeClass, x, y)) {
                AutoPtr<IMessageHelper> helper;
                CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
                AutoPtr<IMessage> msg;
                helper->Obtain(mOwner->mOwner->mWebViewClassic->mPrivateHandler,
                        CWebViewClassic::SHOW_CARET_HANDLE, (IMessage**)&msg);
                msg->SendToTarget();
            }
            mOwner->mOwner->mTextSelectionChangeReason
                    = CWebViewCoreTextSelectionData::REASON_UNKNOWN;
            break;
        }
        case SELECT_ALL:
            mOwner->mOwner->NativeSelectAll(mOwner->mOwner->mNativeClass);
            break;
        case FIND_ALL: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCoreFindAllRequest* request = (CWebViewCoreFindAllRequest*)obj.Get();
            if (request != NULL) {
                Int32 matchCount = mOwner->mOwner->NativeFindAll(mOwner->mOwner->mNativeClass, request->mSearchText);
                Int32 matchIndex = mOwner->mOwner->NativeFindNext(mOwner->mOwner->mNativeClass, TRUE);
                {
                    Object::Autolock lock(request);
                    request->mMatchCount = matchCount;
                    request->mMatchIndex = matchIndex;
                    request->Notify();
                }
            }
            else {
                mOwner->mOwner->NativeFindAll(mOwner->mOwner->mNativeClass, String(NULL));
            }
            AutoPtr<IMessageHelper> helper;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
            AutoPtr<IMessage> msg;
            helper->Obtain(mOwner->mOwner->mWebViewClassic->mPrivateHandler,
                    CWebViewClassic::UPDATE_MATCH_COUNT, (IInterface*)request, (IMessage**)&msg);
            msg->SendToTarget();
            break;
        }
        case FIND_NEXT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCoreFindAllRequest* request = (CWebViewCoreFindAllRequest*)obj.Get();
            Int32 arg1;
            msg->GetArg1(&arg1);
            Int32 matchIndex = mOwner->mOwner->NativeFindNext(mOwner->mOwner->mNativeClass, arg1 != 0);
            {
                Object::Autolock lock(request);
                request->mMatchIndex = matchIndex;
            }
            AutoPtr<IMessageHelper> helper;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
            AutoPtr<IMessage> msg;
            helper->Obtain(mOwner->mOwner->mWebViewClassic->mPrivateHandler,
                    CWebViewClassic::UPDATE_MATCH_COUNT, (IInterface*)request, (IMessage**)&msg);
            msg->SendToTarget();
            break;
        }
        case SET_INITIAL_FOCUS: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mOwner->mOwner->NativeSetInitialFocus(mOwner->mOwner->mNativeClass, arg1);
            break;
        }
        case SAVE_VIEW_STATE: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            SaveViewStateRequest* request = (SaveViewStateRequest*)obj.Get();
            mOwner->mOwner->SaveViewState(request->mStream, request->mCallback);
            break;
        }
    }
    return NOERROR;
}


//=================================================================
//     CWebViewCore::EventHub
//=================================================================
// Message Ids
const Int32 CWebViewCore::EventHub::REVEAL_SELECTION;
const Int32 CWebViewCore::EventHub::SCROLL_TEXT_INPUT;
const Int32 CWebViewCore::EventHub::LOAD_URL;
const Int32 CWebViewCore::EventHub::STOP_LOADING;
const Int32 CWebViewCore::EventHub::RELOAD;
const Int32 CWebViewCore::EventHub::KEY_DOWN;
const Int32 CWebViewCore::EventHub::KEY_UP;
const Int32 CWebViewCore::EventHub::VIEW_SIZE_CHANGED;
const Int32 CWebViewCore::EventHub::GO_BACK_FORWARD;
const Int32 CWebViewCore::EventHub::SET_SCROLL_OFFSET;
const Int32 CWebViewCore::EventHub::RESTORE_STATE;
const Int32 CWebViewCore::EventHub::PAUSE_TIMERS;
const Int32 CWebViewCore::EventHub::RESUME_TIMERS;
const Int32 CWebViewCore::EventHub::CLEAR_CACHE;
const Int32 CWebViewCore::EventHub::CLEAR_HISTORY;
const Int32 CWebViewCore::EventHub::SET_SELECTION;
const Int32 CWebViewCore::EventHub::REPLACE_TEXT;
const Int32 CWebViewCore::EventHub::PASS_TO_JS;
const Int32 CWebViewCore::EventHub::SET_GLOBAL_BOUNDS;
const Int32 CWebViewCore::EventHub::SET_NETWORK_STATE;
const Int32 CWebViewCore::EventHub::DOC_HAS_IMAGES;
const Int32 CWebViewCore::EventHub::DELETE_SELECTION;
const Int32 CWebViewCore::EventHub::LISTBOX_CHOICES;
const Int32 CWebViewCore::EventHub::SINGLE_LISTBOX_CHOICE;
const Int32 CWebViewCore::EventHub::MESSAGE_RELAY;
const Int32 CWebViewCore::EventHub::SET_BACKGROUND_COLOR;
const Int32 CWebViewCore::EventHub::SAVE_DOCUMENT_STATE;
const Int32 CWebViewCore::EventHub::DELETE_SURROUNDING_TEXT;
const Int32 CWebViewCore::EventHub::WEBKIT_DRAW;
const Int32 CWebViewCore::EventHub::POST_URL;
const Int32 CWebViewCore::EventHub::CLEAR_CONTENT;
const Int32 CWebViewCore::EventHub::SET_MOVE_MOUSE;
const Int32 CWebViewCore::EventHub::REQUEST_CURSOR_HREF;
const Int32 CWebViewCore::EventHub::ADD_JS_INTERFACE;
const Int32 CWebViewCore::EventHub::LOAD_DATA;
const Int32 CWebViewCore::EventHub::SET_ACTIVE;
const Int32 CWebViewCore::EventHub::ON_PAUSE;
const Int32 CWebViewCore::EventHub::ON_RESUME;
const Int32 CWebViewCore::EventHub::FREE_MEMORY;
const Int32 CWebViewCore::EventHub::SAVE_WEBARCHIVE;
const Int32 CWebViewCore::EventHub::REMOVE_JS_INTERFACE;
const Int32 CWebViewCore::EventHub::CLEAR_SSL_PREF_TABLE;
const Int32 CWebViewCore::EventHub::REQUEST_EXT_REPRESENTATION;
const Int32 CWebViewCore::EventHub::REQUEST_DOC_AS_TEXT;
const Int32 CWebViewCore::EventHub::DUMP_DOMTREE;
const Int32 CWebViewCore::EventHub::DUMP_RENDERTREE;
const Int32 CWebViewCore::EventHub::SET_JS_FLAGS;
const Int32 CWebViewCore::EventHub::CONTENT_INVALIDATE_ALL;
const Int32 CWebViewCore::EventHub::GEOLOCATION_PERMISSIONS_PROVIDE;
const Int32 CWebViewCore::EventHub::POPULATE_VISITED_LINKS;
const Int32 CWebViewCore::EventHub::HIDE_FULLSCREEN;
const Int32 CWebViewCore::EventHub::SET_NETWORK_TYPE;
const Int32 CWebViewCore::EventHub::ADD_PACKAGE_NAMES;
const Int32 CWebViewCore::EventHub::ADD_PACKAGE_NAME;
const Int32 CWebViewCore::EventHub::REMOVE_PACKAGE_NAME;
const Int32 CWebViewCore::EventHub::MODIFY_SELECTION;
const Int32 CWebViewCore::EventHub::SET_USE_MOCK_DEVICE_ORIENTATION;
const Int32 CWebViewCore::EventHub::AUTOFILL_FORM;
const Int32 CWebViewCore::EventHub::PROXY_CHANGED;
const Int32 CWebViewCore::EventHub::EXECUTE_JS;
const Int32 CWebViewCore::EventHub::PLUGIN_SURFACE_READY;
const Int32 CWebViewCore::EventHub::NOTIFY_ANIMATION_STARTED;
const Int32 CWebViewCore::EventHub::HEARTBEAT;
const Int32 CWebViewCore::EventHub::SCROLL_LAYER;
const Int32 CWebViewCore::EventHub::DESTROY;
const Int32 CWebViewCore::EventHub::COPY_TEXT;
const Int32 CWebViewCore::EventHub::DELETE_TEXT;
const Int32 CWebViewCore::EventHub::INSERT_TEXT;
const Int32 CWebViewCore::EventHub::SELECT_TEXT;
const Int32 CWebViewCore::EventHub::SELECT_WORD_AT;
const Int32 CWebViewCore::EventHub::SELECT_ALL;
const Int32 CWebViewCore::EventHub::TRUST_STORAGE_UPDATED;
const Int32 CWebViewCore::EventHub::FIND_ALL;
const Int32 CWebViewCore::EventHub::FIND_NEXT;
const Int32 CWebViewCore::EventHub::KEY_PRESS;
const Int32 CWebViewCore::EventHub::SET_INITIAL_FOCUS;
const Int32 CWebViewCore::EventHub::SAVE_VIEW_STATE;
const Int32 CWebViewCore::EventHub::SET_USE_MOCK_GEOLOCATION;
const Int32 CWebViewCore::EventHub::FIRST_PACKAGE_MSG_ID;
const Int32 CWebViewCore::EventHub::LAST_PACKAGE_MSG_ID;

/**
 * Prevent other classes from creating an EventHub.
 */
CWebViewCore::EventHub::EventHub(
    /* [in] */ CWebViewCore* owner)
    : mBlockMessages(FALSE)
    , mDestroying(FALSE)
    , mTid(0)
    , mSavedPriority(0)
    , mOwner(owner)
{
    mMessages = new List< AutoPtr<IMessage> >();
}

CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::EventHub, IInterface);

/**
 * Transfer all messages to the newly created webcore thread handler.
 */
void CWebViewCore::EventHub::TransferMessages()
{
    mTid = Process::MyTid();
    Process::GetThreadPriority(mTid, &mSavedPriority);

    mHandler = new MyHandler(this);
    // Take all queued messages and resend them to the new handler.
    {
        Mutex::Autolock lock(mLock);

        List< AutoPtr<IMessage> >::Iterator it;
        for (it = mMessages->Begin(); it != mMessages->End(); ++it) {
            Boolean result;
            mHandler->SendMessage(*it, &result);
        }
        mMessages = NULL;
    }
}

ECode CWebViewCore::EventHub::GetWebKitLooper(
    /* [out] */ ILooper** looper)
{
    VALIDATE_NOT_NULL(looper);
    return mHandler->GetLooper(looper);
}

ECode CWebViewCore::EventHub::DispatchWebKitEvent(
    /* [in] */ WebViewInputDispatcher* dispatcher,
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int32 eventType,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* result)
{
    if (DebugFlags::WEB_VIEW_CORE)
        Logger::V(LOGTAG, "CWebViewCore::EventHub::DispatchWebKitEvent, eventType:%d, mNativeClass:%d",eventType, mOwner->mNativeClass);
    VALIDATE_NOT_NULL(result);
    if (mOwner->mNativeClass == 0) {
        *result = FALSE;
        return NOERROR;
    }
    switch (eventType) {
        case WebViewInputDispatcher::EVENT_TYPE_HIT_TEST: {
            Float fx, fy;
            event->GetX(&fx);
            event->GetY(&fy);
            Int32 x = Elastos::Core::Math::Round(fx);
            Int32 y = Elastos::Core::Math::Round(fy);
            AutoPtr<CWebViewCoreWebKitHitTest> hit = mOwner->PerformHitTest(x, y,
                    mOwner->mWebViewClassic->GetScaledNavSlop(), TRUE);
            AutoPtr<IMessageHelper> helper;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
            AutoPtr<IMessage> msg;
            helper->Obtain(mOwner->mWebViewClassic->mPrivateHandler,
                    CWebViewClassic::HIT_TEST_RESULT, (IInterface*)hit.Get(), (IMessage**)&msg);
            msg->SendToTarget();
            *result = FALSE;
            return NOERROR;
        }

        case WebViewInputDispatcher::EVENT_TYPE_CLICK:
            *result = mOwner->NativeMouseClick(mOwner->mNativeClass);
            return NOERROR;

        case WebViewInputDispatcher::EVENT_TYPE_TOUCH: {
            Int32 count;
            event->GetPointerCount(&count);
            AutoPtr< ArrayOf<Int32> > idArray = ArrayOf<Int32>::Alloc(count);
            AutoPtr< ArrayOf<Int32> > xArray = ArrayOf<Int32>::Alloc(count);
            AutoPtr< ArrayOf<Int32> > yArray = ArrayOf<Int32>::Alloc(count);
            for (Int32 i = 0; i < count; i++) {
                Int32 id;
                Float fx, fy;
                event->GetPointerId(i, &id);
                event->GetX(i, &fx);
                event->GetY(i, &fy);
                (*idArray)[i] = id;
                (*xArray)[i] = (Int32)fx;
                (*yArray)[i] = (Int32)fy;
            }
            Int32 mask, index, state;
            event->GetActionMasked(&mask);
            event->GetActionIndex(&index);
            event->GetMetaState(&state);
            Int32 touchFlags = mOwner->NativeHandleTouchEvent(mOwner->mNativeClass,
                    mask, idArray, xArray, yArray, count, index, state);
            if (touchFlags == 0
                    && (event->GetActionMasked(&mask), mask != IMotionEvent::ACTION_CANCEL)
                    && (flags & WebViewInputDispatcher::FLAG_PRIVATE) == 0) {
                dispatcher->SkipWebkitForRemainingTouchStream();
            }
            *result = (touchFlags & TOUCH_FLAG_PREVENT_DEFAULT) > 0;
            return NOERROR;
        }

        default:
            *result = FALSE;
            return NOERROR;
    }
}

void CWebViewCore::EventHub::SendMessage(
    /* [in] */ IMessage* msg)
{
    Mutex::Autolock lock(mLock);

    if (mBlockMessages) {
        return;
    }
     if (mMessages != NULL) {
         mMessages->PushBack(msg);
     }
     else {
         Boolean result;
         mHandler->SendMessage(msg, &result);
     }
}

void CWebViewCore::EventHub::RemoveMessages(
    /* [in] */ Int32 what)
{
    Mutex::Autolock lock(mLock);

    if (mBlockMessages) {
        return;
    }
    if (what == EventHub::WEBKIT_DRAW) {
        mOwner->mDrawIsScheduled = FALSE;
    }
     if (mMessages != NULL) {
         List< AutoPtr<IMessage> >::Iterator it = mMessages->Begin();
         while (it != mMessages->End()) {
             AutoPtr<IMessage> m = *it;
             Int32 w;
             m->GetWhat(&w);
             if (w == what) {
                 it = mMessages->Erase(it);
             }
             else ++it;
         }
     }
     else {
         mHandler->RemoveMessages(what);
     }
}

void CWebViewCore::EventHub::SendMessageDelayed(
    /* [in] */ IMessage* msg,
    /* [in] */ Int64 delay)
{
    Mutex::Autolock lock(mLock);
    if (mBlockMessages) {
        return;
    }
    Boolean result;
    mHandler->SendMessageDelayed(msg, delay, &result);
}

/**
 * Send a message internally to the front of the queue.
 */
void CWebViewCore::EventHub::SendMessageAtFrontOfQueue(
    /* [in] */ IMessage* msg)
{
    Mutex::Autolock lock(mLock);
    if (mBlockMessages) {
        return;
    }
     if (mMessages != NULL) {
         mMessages->PushFront(msg);
     }
     else {
         Boolean result;
         mHandler->SendMessageAtFrontOfQueue(msg, &result);
     }
}

/**
 * Remove all the messages.
 */
void CWebViewCore::EventHub::RemoveMessages()
{
    Mutex::Autolock lock(mLock);
    // reset mDrawIsScheduled flag as WEBKIT_DRAW may be removed
    mOwner->mDrawIsScheduled = FALSE;
     if (mMessages != NULL) {
         mMessages->Clear();
     }
     else {
         mHandler->RemoveCallbacksAndMessages(NULL);
     }
}

/**
 * Block sending messages to the EventHub.
 */
void CWebViewCore::EventHub::BlockMessages()
{
    Mutex::Autolock lock(mLock);
    mBlockMessages = TRUE;
}


//=================================================================
//     CWebViewCore::ShowRectData
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::ShowRectData, IInterface)


//=================================================================
//     CWebViewCore::QuotaUpdater
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::QuotaUpdater, IWebStorageQuotaUpdater)

ECode CWebViewCore::QuotaUpdater::UpdateQuota(
    /* [in] */ Int64 newQuota)
{
    mOwner->NativeSetNewStorageLimit(mOwner->mNativeClass, newQuota);
    return NOERROR;
}


//=================================================================
//     CWebViewCore::ValueCallback
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::ValueCallback, IValueCallback)

ECode CWebViewCore::ValueCallback::OnReceiveValue(
    /* [in] */ IInterface* value)
{
    mOwner->SendMessage(EventHub::POPULATE_VISITED_LINKS, value);
    return NOERROR;
}


//=================================================================
//     CWebViewCore::GeolocationPermissionsCallback
//=================================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewCore::GeolocationPermissionsCallback, IGeolocationPermissionsCallback)

ECode CWebViewCore::GeolocationPermissionsCallback::Invoke(
    /* [in] */ const String& origin,
    /* [in] */ Boolean allow,
    /* [in] */ Boolean retain)
{
    AutoPtr<GeolocationPermissionsData> data = new GeolocationPermissionsData();
    data->mOrigin = origin;
    data->mAllow = allow;
    data->mRemember = retain;
    // Marshall to WebCore thread.
    mOwner->SendMessage(EventHub::GEOLOCATION_PERMISSIONS_PROVIDE, (IInterface*)data.Get());
    return NOERROR;
}


//=================================================================
//     CWebViewCore
//=================================================================
const String CWebViewCore::LOGTAG("CWebViewCore");
Boolean CWebViewCore::sShouldMonitorWebCoreThread = FALSE;
const String CWebViewCore::THREAD_NAME("WebViewCoreThread");

AutoPtr<IHandler> CWebViewCore::sWebCoreHandler;

// mAction of TouchEventData can be MotionEvent.getAction() which uses the
// last two bytes or one of the following values
const Int32 CWebViewCore::ACTION_LONGPRESS;
const Int32 CWebViewCore::ACTION_DOUBLETAP;

const Int32 CWebViewCore::TOUCH_FLAG_HIT_HANDLER;
const Int32 CWebViewCore::TOUCH_FLAG_PREVENT_DEFAULT;

const String CWebViewCore::HandlerDebugString[] = {
    String("REVEAL_SELECTION"), // 96
    String(""), // 97
    String(""), // = 98
    String("SCROLL_TEXT_INPUT"), // = 99
    String("LOAD_URL"), // = 100;
    String("STOP_LOADING"), // = 101;
    String("RELOAD"), // = 102;
    String("KEY_DOWN"), // = 103;
    String("KEY_UP"), // = 104;
    String("VIEW_SIZE_CHANGED"), // = 105;
    String("GO_BACK_FORWARD"), // = 106;
    String("SET_SCROLL_OFFSET"), // = 107;
    String("RESTORE_STATE"), // = 108;
    String("PAUSE_TIMERS"), // = 109;
    String("RESUME_TIMERS"), // = 110;
    String("CLEAR_CACHE"), // = 111;
    String("CLEAR_HISTORY"), // = 112;
    String("SET_SELECTION"), // = 113;
    String("REPLACE_TEXT"), // = 114;
    String("PASS_TO_JS"), // = 115;
    String("SET_GLOBAL_BOUNDS"), // = 116;
    String(""), // = 117;
    String("CLICK"), // = 118;
    String("SET_NETWORK_STATE"), // = 119;
    String("DOC_HAS_IMAGES"), // = 120;
    String("FAKE_CLICK"), // = 121;
    String("DELETE_SELECTION"), // = 122;
    String("LISTBOX_CHOICES"), // = 123;
    String("SINGLE_LISTBOX_CHOICE"), // = 124;
    String("MESSAGE_RELAY"), // = 125;
    String("SET_BACKGROUND_COLOR"), // = 126;
    String("SET_MOVE_FOCUS"), // = 127
    String("SAVE_DOCUMENT_STATE"), // = 128;
    String("129"), // = 129;
    String("WEBKIT_DRAW"), // = 130;
    String("131"), // = 131;
    String("POST_URL"), // = 132;
    String(""), // = 133;
    String("CLEAR_CONTENT"), // = 134;
    String(""), // = 135;
    String(""), // = 136;
    String("REQUEST_CURSOR_HREF"), // = 137;
    String("ADD_JS_INTERFACE"), // = 138;
    String("LOAD_DATA"), // = 139;
    String(""), // = 140;
    String(""), // = 141;
    String("SET_ACTIVE"), // = 142;
    String("ON_PAUSE"),     // = 143
    String("ON_RESUME"),    // = 144
    String("FREE_MEMORY"),  // = 145
    String("VALID_NODE_BOUNDS"), // = 146
    String("SAVE_WEBARCHIVE"), // = 147
    String("WEBKIT_DRAW_LAYERS"), // = 148;
    String("REMOVE_JS_INTERFACE"), // = 149;
};

Boolean CWebViewCore::sRepaintScheduled = FALSE;
Object CWebViewCore::sLock;

struct ElaWebViewCoreFieldsCallback CWebViewCore::sElaWebViewCoreFieldsCallback =
{
    &CWebViewCore::SetNativeClass,
    &CWebViewCore::GetNativeClass,
    &CWebViewCore::SetViewportWidth,
    &CWebViewCore::SetViewportHeight,
    &CWebViewCore::SetViewportInitialScale,
    &CWebViewCore::SetViewportMinimumScale,
    &CWebViewCore::SetViewportMaximumScale,
    &CWebViewCore::SetViewportUserScalable,
    &CWebViewCore::SetViewportDensityDpi,
    &CWebViewCore::SetDrawIsPaused,
    &CWebViewCore::GetLowMemoryUsageThresholdMb,
    &CWebViewCore::GetHighMemoryUsageThresholdMb,
    &CWebViewCore::GetHighUsageDeltaMb,
};

struct ElaWebViewCoreMethodsCallback CWebViewCore::sElaWebViewCoreMethodsCallback =
{
    &CWebViewCore::ContentScrollTo,
    &CWebViewCore::ContentDraw,
    &CWebViewCore::RequestListBox,
    &CWebViewCore::OpenFileChooser,
    &CWebViewCore::RequestSingleListBox,
    &CWebViewCore::JsAlert,
    &CWebViewCore::JsConfirm,
    &CWebViewCore::JsPrompt,
    &CWebViewCore::JsUnload,
    &CWebViewCore::JsInterrupt,
    &CWebViewCore::GetWebView,
    &CWebViewCore::DidFirstLayout,
    &CWebViewCore::UpdateViewport,
    &CWebViewCore::SendNotifyProgressFinished,
    &CWebViewCore::SendViewInvalidate,
    &CWebViewCore::UpdateTextfield,
    &CWebViewCore::UpdateTextSelection,
    &CWebViewCore::UpdateTextSizeAndScroll,
    &CWebViewCore::ClearTextEntry,
    &CWebViewCore::RestoreScale,
    &CWebViewCore::NeedTouchEvents,
    &CWebViewCore::RequestKeyboard,
    &CWebViewCore::ExceededDatabaseQuota,
    &CWebViewCore::ReachedMaxAppCacheSize,
    &CWebViewCore::PopulateVisitedLinks,
    &CWebViewCore::GeolocationPermissionsShowPrompt,
    &CWebViewCore::GeolocationPermissionsHidePrompt,
    &CWebViewCore::GetDeviceMotionService,
    &CWebViewCore::GetDeviceOrientationService,
    &CWebViewCore::AddMessageToConsole,
    &CWebViewCore::FocusNodeChanged,
    &CWebViewCore::GetPluginClass,
    &CWebViewCore::ShowFullScreenPlugin,
    &CWebViewCore::HideFullScreenPlugin,
    &CWebViewCore::CreateSurface,
    &CWebViewCore::AddSurface,
    &CWebViewCore::UpdateSurface,
    &CWebViewCore::DestroySurface,
    &CWebViewCore::GetContext,
    &CWebViewCore::KeepScreenOn,
    &CWebViewCore::ShowRect,
    &CWebViewCore::CenterFitRect,
    &CWebViewCore::SetScrollbarModes,
    &CWebViewCore::ExitFullscreenVideo,
    &CWebViewCore::SetWebTextViewAutoFillable,
    &CWebViewCore::SelectAt,
    &CWebViewCore::InitEditField,
    &CWebViewCore::ChromeCanTakeFocus,
    &CWebViewCore::ChromeTakeFocus,
    &CWebViewCore::IsSupportedMediaMimeType
};

struct ElaMediaTextureCallback CWebViewCore::sElaMediaTextureCallback =
{
    &CWebView::PostInvalidate,
    &CWebViewCore::SendPluginDrawMsg
};

struct ElaGeolocationServiceBridgeCallback CWebViewCore::sGeolocationServiceBridgeCallback=
{
    &GeolocationService::sElaGeolocationServiceCallback,
    &Location::CLocation::sElaLocationCallback
};

struct ElaWebViewCoreCallback CWebViewCore::sElaWebViewCoreCallback =
{
    &sElaWebViewCoreFieldsCallback,
    &sElaWebViewCoreMethodsCallback,
    &CWebViewCoreTextFieldInitData::sElaWebViewCoreTextFieldInitDataCallback,
    &CWebViewCoreWebKitHitTest::sElaAndroidHitTestResultCallback,
    &sElaMediaTextureCallback,
    &sGeolocationServiceBridgeCallback
};

CWebViewCore::CWebViewCore()
    : mNativeClass(0)
    , mViewportWidth(-1)
    , mViewportHeight(-1)
    , mViewportInitialScale(0)
    , mViewportMinimumScale(0)
    , mViewportMaximumScale(0)
    , mViewportUserScalable(TRUE)
    , mViewportDensityDpi(-1)
    , mIsRestored(FALSE)
    , mRestoredScale(0)
    , mRestoredTextWrapScale(0)
    , mRestoredX(0)
    , mRestoredY(0)
    , mLowMemoryUsageThresholdMb(0)
    , mHighMemoryUsageThresholdMb(0)
    , mHighUsageDeltaMb(0)
    , mChromeCanFocusDirection(0)
    , mTextSelectionChangeReason(0)
    , mCurrentViewWidth(0)
    , mCurrentViewHeight(0)
    , mCurrentViewScale(1)
    , mDrawIsScheduled(FALSE)
    , mDrawIsPaused(FALSE)
    , mFirstLayoutForNonStandardLoad(FALSE)
    , mSkipDrawFlag(FALSE)
    , mDrawWasSkipped(FALSE)
{
    CMockGeolocation::NewByFriend(this, (CMockGeolocation**)&mMockGeolocation);
    mDeviceMotionAndOrientationManager = new DeviceMotionAndOrientationManager(this);
}

ECode CWebViewCore::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IWebViewClassic* w,
    /* [in] */ IHandler* proxy,
    /* [in] */ IMap* javascriptInterfaces)
{
    // No need to assign this in the WebCore thread.
    mCallbackProxy = (CallbackProxy*)proxy;
    mWebViewClassic = (CWebViewClassic*)w;
    mJavascriptInterfaces = javascriptInterfaces;
    // This context object is used to initialize the WebViewCore during
    // subwindow creation.
    mContext = ctx;

    // We need to wait for the initial thread creation before sending
    // a message to the WebCore thread.
    // XXX: This is the only time the UI thread will wait for the WebCore
    // thread!
    {
        Object::Autolock lock(sLock);
        if (sWebCoreHandler == NULL) {
            // Create a global thread and start it.
            AutoPtr<IRunnable> r = new WebCoreThread(this);
            AutoPtr<IThread> t;
            //CThread::New(NULL, r, THREAD_NAME, 256*1024, (IThread**)&t);
            CThread::New(r, (IThread**)&t);
            t->SetName(THREAD_NAME);
            t->Start();
            //try {
            sLock.Wait();
            //} catch (InterruptedException e) {
            //    Log.e(LOGTAG, "Caught exception while waiting for thread " +
            //           "creation.");
            //    Log.e(LOGTAG, Log.getStackTraceString(e));
            //}

            if (sShouldMonitorWebCoreThread) {
                // Start the singleton watchdog which will monitor the WebCore thread
                // to verify it's still processing messages. Note that this is the only
                // time we need to check the value as all the other public methods on
                // the WebCoreThreadWatchdog are no-ops if start() is not called.
                WebCoreThreadWatchdog::Start(sWebCoreHandler);
            }
        }
        // Make sure the Watchdog is aware of this new WebView.
        WebCoreThreadWatchdog::RegisterWebView((CWebViewClassic*)w);
    }
    // Create an EventHub to handle messages before and after the thread is
    // ready.
    mEventHub = new EventHub(this);
    // Create a WebSettings object for maintaining all settings
    mSettings = new WebSettingsClassic(mContext, mWebViewClassic);
    // The WebIconDatabase needs to be initialized within the UI thread so
    // just request the instance here.
    WebIconDatabase::GetInstance();
    // Create the WebStorageClassic singleton and the UI handler
    WebStorageClassic::GetInstance()->CreateUIHandler();
    // Create the UI handler for GeolocationPermissions
    GeolocationPermissionsClassic::GetInstance()->CreateUIHandler();

    // Get the memory class of the current device. V8 will use these values
    // to GC more effectively.
    AutoPtr<IActivityManager> manager;
    mContext->GetSystemService(IContext::ACTIVITY_SERVICE,
        (IInterface**)&manager);
    AutoPtr<IActivityManagerMemoryInfo> memInfo;
    manager->GetMemoryInfo((IActivityManagerMemoryInfo**)&memInfo);

    // Allow us to use up to our memory class value before V8's GC kicks in.
    // These values have been determined by experimentation.
    manager->GetLargeMemoryClass(&mLowMemoryUsageThresholdMb);
    mHighMemoryUsageThresholdMb = (Int32) (mLowMemoryUsageThresholdMb * 1.5);
    // Avoid constant V8 GC when memory usage equals to working set estimate.
    mHighUsageDeltaMb = mLowMemoryUsageThresholdMb / 32;

    // Send a message to initialize the WebViewCore.
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> init;
    helper->Obtain(sWebCoreHandler, WebCoreThread::INITIALIZE,
            (IWebViewCore*)this, (IMessage**)&init);
    Boolean result;
    sWebCoreHandler->SendMessage(init, &result);
    return NOERROR;
}

/* Initialize private data within the WebCore thread.
 */
void CWebViewCore::Initialize()
{
    /* Initialize our private BrowserFrame class to handle all
     * frame-related functions. We need to create a new view which
     * in turn creates a C level FrameView and attaches it to the frame.
     */
    mBrowserFrame = new BrowserFrame(mContext, this, mCallbackProxy,
            mSettings, mJavascriptInterfaces);
    mBrowserFrame->Release(); // call AddRef in BrowserFrame constructor
    mJavascriptInterfaces = NULL;
    // Sync the native settings and also create the WebCore thread handler.
    mSettings->SyncSettingsAndCreateHandler(mBrowserFrame);
    // Create the handler and transfer messages for the IconDatabase
    WebIconDatabaseClassic::GetInstance()->CreateHandler();
    // Create the handler for WebStorageClassic
    WebStorageClassic::GetInstance()->CreateHandler();
    // Create the handler for GeolocationPermissions.
    GeolocationPermissionsClassic::GetInstance()->CreateHandler();
    // The transferMessages call will transfer all pending messages to the
    // WebCore thread handler.
    mEventHub->TransferMessages();

    // Send a message back to WebView to tell it that we have set up the
    // WebCore thread.
    if (mWebViewClassic != NULL) {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::WEBCORE_INITIALIZED_MSG_ID,
                mNativeClass, 0, (IMessage**)&msg);
        msg->SendToTarget();
    }
}

/* Handle the initialization of WebViewCore during subwindow creation. This
 * method is called from the WebCore thread but it is called before the
 * INITIALIZE message can be handled.
 */
void CWebViewCore::InitializeSubwindow()
{
    // Go ahead and initialize the core components.
    Initialize();
    // Remove the INITIALIZE method so we don't try to initialize twice.
    sWebCoreHandler->RemoveMessages(WebCoreThread::INITIALIZE, (IWebViewCore*)this);
}

/* Get the BrowserFrame component. This is used for subwindow creation and
 * is called only from BrowserFrame in the WebCore thread. */
AutoPtr<BrowserFrame> CWebViewCore::GetBrowserFrame()
{
    Mutex::Autolock lock(mLock);
    return mBrowserFrame;
}

AutoPtr<WebViewInputDispatcher::IWebKitCallbacks> CWebViewCore::GetInputDispatcherCallbacks()
{
    return mEventHub;
}

//-------------------------------------------------------------------------
// Common methods
//-------------------------------------------------------------------------

/**
 * Causes all timers to pause. This applies to all WebViews in the current
 * app process.
 */
ECode CWebViewCore::PauseTimers()
{
    if (BrowserFrame::sElastosBridge == NULL) {
        //throw new IllegalStateException(
        //        "No WebView has been created in this process!");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    BrowserFrame::sElastosBridge->Pause();
    return NOERROR;
}

/**
 * Resume all timers. This applies to all WebViews in the current process.
 */
ECode CWebViewCore::ResumeTimers()
{
    if (BrowserFrame::sElastosBridge == NULL) {
        //throw new IllegalStateException(
        //        "No WebView has been created in this process!");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    BrowserFrame::sElastosBridge->Resume();
    return NOERROR;
}

ECode CWebViewCore::GetSettings(
    /* [out] */ IWebSettingsClassic** settings)
{
    VALIDATE_NOT_NULL(settings);
    *settings = (IWebSettingsClassic*)mSettings;
    REFCOUNT_ADD(*settings);
    return NOERROR;
}

/*
 * Given mimeType, check whether it's supported in Android media framework.
 * mimeType could be such as "audio/ogg" and "video/mp4".
 */
Boolean CWebViewCore::IsSupportedMediaMimeType(
    /* [in] */ const String& mimeType)
{
    AutoPtr<IMediaFile> mediaFile;
    CMediaFile::AcquireSingleton((IMediaFile**)&mediaFile);
    Int32 fileType;
    mediaFile->GetFileTypeForMimeType(mimeType, &fileType);
    Boolean isAudioFileType, isVideoFileType, isPlayListFileType;
    return (mediaFile->IsAudioFileType(fileType, &isAudioFileType), isAudioFileType)
        || (mediaFile->IsVideoFileType(fileType, &isVideoFileType), isVideoFileType)
        || (mediaFile->IsPlayListFileType(fileType, &isPlayListFileType), isPlayListFileType)
        // The following is not in Media framework, but it's supported.
        || (!mimeType.IsNull() && mimeType.StartWith("video/m4v"));
}

/**
 * Add an error message to the client's console.
 * @param message The message to add
 * @param lineNumber the line on which the error occurred
 * @param sourceID the filename of the source that caused the error.
 * @param msgLevel the log level of this message. This is a value casted to int
 *     from WebCore::MessageLevel in WebCore/page/Console.h.
 */
void CWebViewCore::AddMessageToConsole(
    /* [in] */ const String& message,
    /* [in] */ Int32 lineNumber,
    /* [in] */ const String& sourceID,
    /* [in] */ Int32 msgLevel)
{
    mCallbackProxy->AddMessageToConsole(message, lineNumber, sourceID, msgLevel);
}

/**
 * Invoke a javascript alert.
 * @param message The message displayed in the alert.
 */
void CWebViewCore::JsAlert(
    /* [in] */ const String& url,
    /* [in] */ const String& message)
{
    mCallbackProxy->OnJsAlert(url, message);
}

/**
 * Called by JNI when the focus node changed.
 */
void CWebViewCore::FocusNodeChanged(
    /* [in] */ Int32 nodePointer,
    /* [in] */ IInterface* hitTest)
{
    //CWebViewCoreWebKitHitTest hitTest
    if (mWebViewClassic == NULL) return;
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(mWebViewClassic->mPrivateHandler,
            CWebViewClassic::FOCUS_NODE_CHANGED,
            nodePointer, 0, hitTest, (IMessage**)&msg);
    msg->SendToTarget();
}

/**
 * Called by JNI to advance focus to the next view.
 */
void CWebViewCore::ChromeTakeFocus(
    /* [in] */ Int32 webkitDirection)
{
    if (mWebViewClassic == NULL) return;
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(mWebViewClassic->mPrivateHandler,
            CWebViewClassic::TAKE_FOCUS, (IMessage**)&msg);
    msg->SetArg1(MapDirection(webkitDirection));
    msg->SendToTarget();
}

/**
 * Called by JNI to see if we can take focus in the given direction.
 */
Boolean CWebViewCore::ChromeCanTakeFocus(
    /* [in] */ Int32 webkitDirection)
{
    Int32 direction = MapDirection(webkitDirection);
    return direction == mChromeCanFocusDirection && direction != 0;
}

/**
 * Maps a Webkit focus direction to a framework one
 */
Int32 CWebViewCore::MapDirection(
    /* [in] */ Int32 webkitDirection)
{
    switch (webkitDirection) {
    case 1:
        return IView::FOCUS_FORWARD;
    case 2:
        return IView::FOCUS_BACKWARD;
    case 3:
        return IView::FOCUS_UP;
    case 4:
        return IView::FOCUS_DOWN;
    case 5:
        return IView::FOCUS_LEFT;
    case 6:
        return IView::FOCUS_RIGHT;
    }
    return 0;
}

/**
 * Called by JNI.  Open a file chooser to upload a file.
 * @param acceptType The value of the 'accept' attribute of the
 *         input tag associated with this file picker.
 * @param capture The value of the 'capture' attribute of the
 *         input tag associated with this file picker.
 * @return String version of the URI.
 */
String CWebViewCore::OpenFileChooser(
    /* [in] */ const String& acceptType,
    /* [in] */ const String& capture)
{
    AutoPtr<IUri> uri = mCallbackProxy->OpenFileChooser(acceptType, capture);
    if (uri != NULL) {
        String filePath("");
        // Note - querying for MediaStore.Images.Media.DATA
        // seems to work for all content URIs, not just images
        AutoPtr<IContentResolver> resolver;
        mContext->GetContentResolver((IContentResolver**)&resolver);
        AutoPtr< ArrayOf<String> > projection = ArrayOf<String>::Alloc(1);
        (*projection)[0] = "_data";//IMediaStoreImagesMedia::DATA;
        AutoPtr<ICursor> cursor;
        resolver->Query(uri, projection, String(NULL), NULL, String(NULL), (ICursor**)&cursor);
        if (cursor != NULL) {
            //try {
            Boolean hasNext = FALSE;
            if (cursor->MoveToNext(&hasNext), hasNext) {
                cursor->GetString(0, &filePath);
            }
            //} finally {
            cursor->Close();
            //}
        }
        else {
            uri->GetLastPathSegment(&filePath);
        }
        String uriString;
        uri->ToString(&uriString);
        BrowserFrame::sElastosBridge->StoreFilePathForContentUri(filePath, uriString);
        return uriString;
    }
    return String("");
}

/**
 * Notify the embedding application that the origin has exceeded it's database quota.
 * @param url The URL that caused the overflow.
 * @param databaseIdentifier The identifier of the database.
 * @param quota The current quota for the origin.
 * @param estimatedDatabaseSize The estimated size of the database.
 */
void CWebViewCore::ExceededDatabaseQuota(
    /* [in] */ const String& url,
    /* [in] */ const String& databaseIdentifier,
    /* [in] */ Int64 quota,
    /* [in] */ Int64 estimatedDatabaseSize)
{
    // Inform the callback proxy of the quota overflow. Send an object
    // that encapsulates a call to the nativeSetDatabaseQuota method to
    // awaken the sleeping webcore thread when a decision from the
    // client to allow or deny quota is available.
    AutoPtr<IWebStorageQuotaUpdater> updater = new QuotaUpdater(this);
    mCallbackProxy->OnExceededDatabaseQuota(url, databaseIdentifier,
            quota, estimatedDatabaseSize, GetUsedQuota(), updater);
}

/**
 * Notify the embedding application that the appcache has reached or exceeded its maximum
 * allowed storage size.
 *
 * @param requiredStorage is the amount of storage, in bytes, that would be
 * needed in order for the last appcache operation to succeed.
 * @param maxSize maximum allowed Application Cache database size, in bytes.
 */
void CWebViewCore::ReachedMaxAppCacheSize(
    /* [in] */ Int64 requiredStorage,
    /* [in] */ Int64 maxSize)
{
    AutoPtr<IWebStorageQuotaUpdater> updater = new QuotaUpdater(this);
    mCallbackProxy->OnReachedMaxAppCacheSize(requiredStorage, maxSize, updater);
}

void CWebViewCore::PopulateVisitedLinks()
{
    AutoPtr<IValueCallback> callback = new ValueCallback(this);
    mCallbackProxy->GetVisitedHistory(callback);
}

/**
 * Shows a prompt to ask the user to set the Geolocation permission state
 * for the given origin.
 * @param origin The origin for which Geolocation permissions are
 *     requested.
 */
void CWebViewCore::GeolocationPermissionsShowPrompt(
    /* [in] */ const String& origin)
{
    AutoPtr<GeolocationPermissionsCallback> callback = new GeolocationPermissionsCallback(this);
    mCallbackProxy->OnGeolocationPermissionsShowPrompt(origin, callback);
}

/**
 * Hides the Geolocation permissions prompt.
 */
void CWebViewCore::GeolocationPermissionsHidePrompt()
{
    mCallbackProxy->OnGeolocationPermissionsHidePrompt();
}

/**
 * Invoke a javascript confirm dialog.
 * @param message The message displayed in the dialog.
 * @return True if the user confirmed or false if the user cancelled.
 */
Boolean CWebViewCore::JsConfirm(
    /* [in] */ const String& url,
    /* [in] */ const String& message)
{
    return mCallbackProxy->OnJsConfirm(url, message);
}

/**
 * Invoke a javascript prompt dialog.
 * @param message The message to be displayed in the dialog.
 * @param defaultValue The default value in the prompt input.
 * @return The input from the user or null to indicate the user cancelled
 *         the dialog.
 */
String CWebViewCore::JsPrompt(
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ const String& defaultValue)
{
    return mCallbackProxy->OnJsPrompt(url, message, defaultValue);
}

/**
 * Invoke a javascript before unload dialog.
 * @param url The url that is requesting the dialog.
 * @param message The message displayed in the dialog.
 * @return True if the user confirmed or false if the user cancelled. False
 *         will cancel the navigation.
 */
Boolean CWebViewCore::JsUnload(
    /* [in] */ const String& url,
    /* [in] */ const String& message)
{
    return mCallbackProxy->OnJsBeforeUnload(url, message);
}

/**
 *
 * Callback to notify that a JavaScript execution timeout has occured.
 * @return True if the JavaScript execution should be interrupted. False
 *         will continue the execution.
 */
Boolean CWebViewCore::JsInterrupt()
{
    return mCallbackProxy->OnJsTimeout();
}

/**
 * Notify the webview that we want to exit the video fullscreen.
 * This is called through JNI by webcore.
 */
void CWebViewCore::ExitFullscreenVideo()
{
    if (mWebViewClassic == NULL) return;
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(mWebViewClassic->mPrivateHandler,
            CWebViewClassic::EXIT_FULLSCREEN_VIDEO, (IMessage**)&msg);
    msg->SendToTarget();
}

/**
 * Clear the picture set. To be called only on the WebCore thread.
 */
void CWebViewCore::ClearContent()
{
    NativeClearContent(mNativeClass);
}

//-------------------------------------------------------------------------
// JNI methods
//-------------------------------------------------------------------------

String CWebViewCore::NativeFindAddress(
    /* [in] */ const String& addr,
    /* [in] */ Boolean caseInsensitive)
{
    return Elastos_WebViewCore_FindAddress(addr, caseInsensitive);
}

/**
 * Empty the picture set.
 */
void CWebViewCore::NativeClearContent(
    /* [in] */ Int32 nativeClass)
{
    Elastos_WebViewCore_ClearContent(nativeClass);
}

void CWebViewCore::NativeContentInvalidateAll(
    /* [in] */ Int32 nativeClass)
{
    Elastos_WebViewCore_ContentInvalidateAll(nativeClass);
}

/**
 * Redraw a portion of the picture set. The Point wh returns the
 * width and height of the overall picture.
 */
Int32 CWebViewCore::NativeRecordContent(
    /* [in] */ Int32 nativeClass,
    /* [in] */ IPoint* pt)
{
    //a little change here for easy-implementation
    Int32 x = 0;
    Int32 y = 0;
    Int32 result = Elastos_WebViewCore_RecordContent(nativeClass, &x, &y);
    pt->Set(x, y);
    return result;
}

/**
 * Notify webkit that animations have begun (on the hardware accelerated content)
 */
void CWebViewCore::NativeNotifyAnimationStarted(
    /* [in] */ Int32 nativeClass)
{
    Elastos_WebViewCore_NotifyAnimationStarted(nativeClass);
}

Boolean CWebViewCore::NativeKey(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 unichar,
    /* [in] */ Int32 repeatCount,
    /* [in] */ Boolean isShift,
    /* [in] */ Boolean isAlt,
    /* [in] */ Boolean isSym,
    /* [in] */ Boolean isDown)
{
    return Elastos_WebViewCore_Key(nativeClass,keyCode,unichar, repeatCount, isShift, isAlt, isSym, isDown);
}

// Set aside a predetermined amount of space in which to place the listbox
// choices, to avoid unnecessary allocations.
// The size here is arbitrary.  We want the size to be at least as great as the
// number of items in the average multiple-select listbox.
#define PREPARED_LISTBOX_STORAGE 10

void CWebViewCore::NativeSendListBoxChoices(
    /* [in] */ Int32 nativeClass,
    /* [in] */ ArrayOf<Boolean>* choices,
    /* [in] */ Int32 size)
{
    Elastos_WebViewCore_SendListBoxChoices(nativeClass,choices,size);
}

void CWebViewCore::NativeSendListBoxChoice(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 choice)
{
    Elastos_WebViewCore_SendListBoxChoice(nativeClass,choice);
}

void CWebViewCore::NativeCloseIdleConnections(
    /* [in] */ Int32 nativeClass)
{
    Elastos_WebViewCore_CloseIdleConnections(nativeClass);
}

/*  Tell webkit what its width and height are, for the purposes
    of layout/line-breaking. These coordinates are in document space,
    which is the same as View coords unless we have zoomed the document
    (see nativeSetZoom).
    textWrapWidth is used by layout to wrap column around. If viewport uses
    fixed size, textWrapWidth can be different from width with zooming.
    should this be called nativeSetViewPortSize?
*/
void CWebViewCore::NativeSetSize(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 textWrapWidth,
    /* [in] */ Float scale,
    /* [in] */ Int32 screenWidth,
    /* [in] */ Int32 screenHeight,
    /* [in] */ Int32 anchorX,
    /* [in] */ Int32 anchorY,
    /* [in] */ Boolean ignoreHeight)
{
    Elastos_WebViewCore_SetSize(nativeClass,width, height, textWrapWidth, scale,
            screenWidth, screenHeight, anchorX, anchorY, ignoreHeight);
}

Int32 CWebViewCore::NativeGetContentMinPrefWidth(
    /* [in] */ Int32 nativeClass)
{
    return Elastos_WebViewCore_GetContentMinPrefWidth(nativeClass);
}

// Start: functions that deal with text editing
void CWebViewCore::NativeReplaceTextfieldText(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 oldStart,
    /* [in] */ Int32 oldEnd,
    /* [in] */ const String& replace,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 textGeneration)
{
    Elastos_WebViewCore_ReplaceTextfieldText(nativeClass, oldStart, oldEnd, replace, start, end, textGeneration);
}

void CWebViewCore::PassToJs(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 generation,
    /* [in] */ const String& currentText,
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 keyValue,
    /* [in] */ Boolean down,
    /* [in] */ Boolean cap,
    /* [in] */ Boolean fn,
    /* [in] */ Boolean sym)
{
    Elastos_WebViewCore_PassToJs(nativeClass, generation, currentText, keyCode, keyValue, down, cap, fn, sym);
}

void CWebViewCore::NativeSetFocusControllerActive(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Boolean active)
{
    Elastos_WebViewCore_SetFocusControllerActive(nativeClass, active);
}

void CWebViewCore::NativeSaveDocumentState(
    /* [in] */ Int32 nativeClass)
{
}

void CWebViewCore::NativeMoveMouse(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Elastos_WebViewCore_MoveMouse(nativeClass, x, y);
}

String CWebViewCore::NativeRetrieveHref(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return Elastos_WebViewCore_RetrieveHref(nativeClass, x, y);
}

String CWebViewCore::NativeRetrieveAnchorText(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return Elastos_WebViewCore_RetrieveAnchorText(nativeClass, x, y);
}

String CWebViewCore::NativeRetrieveImageSource(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return Elastos_WebViewCore_RetrieveImageSource(nativeClass, x, y);
}

Boolean CWebViewCore::NativeMouseClick(
    /* [in] */ Int32 nativeClass)
{
    return Elastos_WebViewCore_MouseClick(nativeClass);
}

Int32 CWebViewCore::NativeHandleTouchEvent(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 action,
    /* [in] */ ArrayOf<Int32>* idArray,
    /* [in] */ ArrayOf<Int32>* xArray,
    /* [in] */ ArrayOf<Int32>* yArray,
    /* [in] */ Int32 count,
    /* [in] */ Int32 actionIndex,
    /* [in] */ Int32 metaState)
{
    return Elastos_WebViewCore_HandleTouchEvent(nativeClass, action, idArray, xArray, yArray,
                                                    count, actionIndex, metaState);
}

void CWebViewCore::NativeSetBackgroundColor(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 color)
{
    Elastos_WebViewCore_SetBackgroundColor(nativeClass, color);
}

void CWebViewCore::NativeDumpDomTree(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Boolean useFile)
{
    Elastos_WebViewCore_DumpDomTree(nativeClass, useFile);
}

void CWebViewCore::NativeDumpRenderTree(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Boolean useFile)
{
    Elastos_WebViewCore_DumpRenderTree(nativeClass, useFile);
}

void CWebViewCore::NativeSetJsFlags(
    /* [in] */ Int32 nativeClass,
    /* [in] */ const String& flags)
{
    Elastos_WebViewCore_SetJsFlags(nativeClass, flags);
}

/**
 *  Delete text from start to end in the focused textfield. If there is no
 *  focus, or if start == end, silently fail.  If start and end are out of
 *  order, swap them.
 * @param  nativeClass Pointer to the C++ WebViewCore object mNativeClass
 * @param  start   Beginning of selection to delete.
 * @param  end     End of selection to delete.
 * @param  textGeneration Text generation number when delete was pressed.
 */
void CWebViewCore::NativeDeleteSelection(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 textGeneration)
{
    Elastos_WebViewCore_DeleteSelection(nativeClass, start, end, textGeneration);
}

/**
 *  Set the selection to (start, end) in the focused textfield. If start and
 *  end are out of order, swap them.
 * @param  nativeClass Pointer to the C++ WebViewCore object mNativeClass
 * @param  start   Beginning of selection.
 * @param  end     End of selection.
 */
void CWebViewCore::NativeSetSelection(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Elastos_WebViewCore_SetSelection(nativeClass, start, end);
}

// Register a scheme to be treated as local scheme so that it can access
// local asset files for resources
void CWebViewCore::NativeRegisterURLSchemeAsLocal(
    /* [in] */ Int32 nativeClass,
    /* [in] */ const String& scheme)
{
    Elastos_WebViewCore_RegisterURLSchemeAsLocal(nativeClass, scheme);
}

/*
 * Inform webcore that the user has decided whether to allow or deny new
 * quota for the current origin or more space for the app cache, and that
 * the main thread should wake up now.
 * @param limit Is the new quota for an origin or new app cache max size.
 */
void CWebViewCore::NativeSetNewStorageLimit(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int64 quota)
{
    Elastos_WebViewCore_SetNewStorageLimit(nativeClass, quota);
}

/**
 * Provide WebCore with a Geolocation permission state for the specified
 * origin.
 * @param nativeClass Pointer to the C++ WebViewCore object mNativeClass
 * @param origin The origin for which Geolocation permissions are provided.
 * @param allow Whether Geolocation permissions are allowed.
 * @param remember Whether this decision should be remembered beyond the
 *     life of the current page.
 */
void CWebViewCore::NativeGeolocationPermissionsProvide(
    /* [in] */ Int32 nativeClass,
    /* [in] */ const String& origin,
    /* [in] */ Boolean allow,
    /* [in] */ Boolean remember)
{
    Elastos_WebViewCore_GeolocationPermissionsProvide(nativeClass, origin, allow, remember);
}

/**
 * Provide WebCore with the previously visted links from the history database
 * @param nativeClass TODO
 */
void CWebViewCore::NativeProvideVisitedHistory(
    /* [in] */ Int32 nativeClass,
    /* [in] */ ArrayOf<String>* hist)
{
    Elastos_WebViewCore_ProvideVisitedHistory(nativeClass, hist);
}

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
String CWebViewCore::NativeModifySelection(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 granularity)
{
    return Elastos_WebViewCore_ModifySelection(nativeClass, direction, granularity);
}

//-------------------------------------------------------------------------
// Methods called by host activity (in the same thread)
//-------------------------------------------------------------------------

void CWebViewCore::StopLoading()
{
    if (DebugFlags::WEB_VIEW_CORE) Logger::V(LOGTAG, "CORE stopLoading");
    if (mBrowserFrame != NULL) {
        mBrowserFrame->StopLoading();
    }
}

//-------------------------------------------------------------------------
// Methods called by WebView
// If it refers to local variable, it needs synchronized().
// If it needs WebCore, it has to send message.
//-------------------------------------------------------------------------

/**
 * @hide
 */
ECode CWebViewCore::SendMessage(
    /* [in] */ IMessage* msg)
{
    mEventHub->SendMessage(msg);
    return NOERROR;
}

void CWebViewCore::SendMessages(
    /* [in] */ List< AutoPtr<IMessage> >& messages)
{
    Mutex::Autolock lock(mEventHub->mLock);

    List< AutoPtr<IMessage> >::Iterator it;
    for (it = messages.Begin(); it != messages.End(); ++it) {
        mEventHub->SendMessage(*it);
    }
}

void CWebViewCore::SendMessage(
    /* [in] */ Int32 what)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(NULL, what, (IMessage**)&msg);
    mEventHub->SendMessage(msg);
}

void CWebViewCore::SendMessageAtFrontOfQueue(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(NULL, what, arg1, arg2, obj, (IMessage**)&msg);
    mEventHub->SendMessageAtFrontOfQueue(msg);
}

void CWebViewCore::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(NULL, what, obj, (IMessage**)&msg);
    mEventHub->SendMessage(msg);
}

void CWebViewCore::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1)
{
    // just ignore the second argument (make it 0)
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(NULL, what, arg1, 0, (IMessage**)&msg);
    mEventHub->SendMessage(msg);
}

void CWebViewCore::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(NULL, what, arg1, arg2, (IMessage**)&msg);
    mEventHub->SendMessage(msg);
}

void CWebViewCore::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ IInterface* obj)
{
    // just ignore the second argument (make it 0)
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(NULL, what, arg1, 0, obj, (IMessage**)&msg);
    mEventHub->SendMessage(msg);
}

void CWebViewCore::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    // just ignore the second argument (make it 0)
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(NULL, what, arg1, arg2, obj, (IMessage**)&msg);
    mEventHub->SendMessage(msg);
}

void CWebViewCore::SendMessageAtFrontOfQueue(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(NULL, what, obj, (IMessage**)&msg);
    mEventHub->SendMessageAtFrontOfQueue(msg);
}

void CWebViewCore::SendMessageDelayed(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj,
    /* [in] */ Int64 delay)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(NULL, what, obj, (IMessage**)&msg);
    mEventHub->SendMessageDelayed(msg, delay);
}

void CWebViewCore::RemoveMessages(
    /* [in] */ Int32 what)
{
    mEventHub->RemoveMessages(what);
}

void CWebViewCore::RemoveMessages()
{
    mEventHub->RemoveMessages();
}

/**
 * Sends a DESTROY message to WebCore.
 * Called from UI thread.
 */
void CWebViewCore::Destroy()
{
    Mutex::Autolock lock(mEventHub->mLock);

    // send DESTROY to front of queue
    // PAUSE/RESUME timers will still be processed even if they get handled later
    mEventHub->mDestroying = TRUE;
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(NULL, EventHub::DESTROY, (IMessage**)&msg);
    mEventHub->SendMessageAtFrontOfQueue(msg);
    mEventHub->BlockMessages();
    WebCoreThreadWatchdog::UnregisterWebView(mWebViewClassic);
}

//-------------------------------------------------------------------------
// WebViewCore private methods
//-------------------------------------------------------------------------

AutoPtr<CWebViewCoreWebKitHitTest> CWebViewCore::PerformHitTest(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 slop,
    /* [in] */ Boolean moveMouse)
{
    AutoPtr<CWebViewCoreWebKitHitTest> hit = NativeHitTest(mNativeClass, x, y, slop, moveMouse);
    hit->mHitTestX = x;
    hit->mHitTestY = y;
    hit->mHitTestSlop = slop;
    hit->mHitTestMovedMouse = moveMouse;
    return hit;
}

void CWebViewCore::ClearCache(
    /* [in] */ Boolean includeDiskFiles)
{
    mBrowserFrame->ClearCache();
    if (includeDiskFiles) {
        CCacheManager::RemoveAllCacheFiles();
    }
}

void CWebViewCore::LoadUrl(
    /* [in] */ const String& url,
    /* [in] */ IMap* extraHeaders)
{
    if (DebugFlags::WEB_VIEW_CORE) Logger::V(LOGTAG, " CWebViewCore::LoadUrl: %s", url.string());
    mBrowserFrame->LoadUrl(url, extraHeaders);
}

String CWebViewCore::SaveWebArchive(
    /* [in] */ const String& filename,
    /* [in] */ Boolean autoname)
{
    if (DebugFlags::WEB_VIEW_CORE) {
        Logger::V(LOGTAG, " CORE saveWebArchive %s %d", filename.string(), autoname);
    }
    return mBrowserFrame->SaveWebArchive(filename, autoname);
}

void CWebViewCore::Key(
    /* [in] */ IKeyEvent* evt,
    /* [in] */ Int32 canTakeFocusDirection,
    /* [in] */ Boolean isDown)
{
    mChromeCanFocusDirection = canTakeFocusDirection;
    Int32 keyCode, unicodeChar;
    evt->GetKeyCode(&keyCode);
    evt->GetUnicodeChar(&unicodeChar);

    if (DebugFlags::WEB_VIEW_CORE) {
        AutoPtr<ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
        Int64 now;
        system->GetCurrentTimeMillis(&now);
        Logger::V(LOGTAG, "CORE key at %lld, event:%p, keyCode:0x%x, unicodeChar:0x%x", now, evt, keyCode, unicodeChar);
    }

    String characters;
    if (keyCode == IKeyEvent::KEYCODE_UNKNOWN &&
        (evt->GetCharacters(&characters), !characters.IsNullOrEmpty())) {
        // we should only receive individual complex characters
        evt->GetCharacters(&characters);
        //assert(0);
//        unicodeChar = characters.CodePointAt(0);
        unicodeChar = characters.GetChar(0);
    }

    Int32 count;
    evt->GetRepeatCount(&count);
    Boolean isShiftPressed, isAltPressed, isSymPressed;
    evt->IsShiftPressed(&isShiftPressed);
    evt->IsAltPressed(&isAltPressed);
    evt->IsSymPressed(&isSymPressed);
    Boolean handled = NativeKey(mNativeClass, keyCode, unicodeChar, count,
            isShiftPressed, isAltPressed, isSymPressed, isDown);
    mChromeCanFocusDirection = 0;
    if (!handled && keyCode != IKeyEvent::KEYCODE_ENTER) {
        if (keyCode >= IKeyEvent::KEYCODE_DPAD_UP
                && keyCode <= IKeyEvent::KEYCODE_DPAD_RIGHT) {
            if (canTakeFocusDirection != 0 && isDown) {
                AutoPtr<IMessageHelper> helper;
                CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
                AutoPtr<IMessage> msg;
                helper->Obtain(mWebViewClassic->mPrivateHandler,
                        CWebViewClassic::TAKE_FOCUS, (IMessage**)&msg);
                msg->SetArg1(canTakeFocusDirection);
                msg->SendToTarget();
            }
            return;
        }
        // bubble up the event handling
        // but do not bubble up the ENTER key, which would open the search
        // bar without any text.
        mCallbackProxy->OnUnhandledKeyEvent(evt);
    }
}

void CWebViewCore::KeyPress(
    /* [in] */ Int32 unicodeChar)
{
    NativeKey(mNativeClass, 0, unicodeChar, 0, FALSE, FALSE, FALSE, TRUE);
    NativeKey(mNativeClass, 0, unicodeChar, 0, FALSE, FALSE, FALSE, FALSE);
}

// notify webkit that our virtual view size changed size (after inv-zoom)
void CWebViewCore::ViewSizeChanged(
    /* [in] */ CWebViewClassic::ViewSizeData* data)
{
    Int32 w = data->mWidth;
    Int32 h = data->mHeight;
    Int32 textwrapWidth = data->mTextWrapWidth;
    Float scale = data->mScale;
    if (DebugFlags::WEB_VIEW_CORE) {
        Logger::V(LOGTAG, "viewSizeChanged w=%d; h=%d; textwrapWidth=%d; scale=%f",
                w, h, textwrapWidth, scale);
    }
    if (w == 0) {
        Logger::W(LOGTAG, "skip viewSizeChanged as w is 0");
        return;
    }
    Int32 width = CalculateWindowWidth(w);
    Int32 height = h;
    if (width != w) {
        Float heightWidthRatio = data->mHeightWidthRatio;
        Float ratio = (heightWidthRatio > 0) ? heightWidthRatio : (Float) h / w;
        height = Elastos::Core::Math::Round(ratio * width);
    }
    Int32 screenHeight = data->mActualViewHeight > 0 ? data->mActualViewHeight : h;
    NativeSetSize(mNativeClass, width, height, textwrapWidth, scale,
            w, screenHeight, data->mAnchorX, data->mAnchorY, data->mIgnoreHeight);
    // Remember the current width and height
    Boolean needInvalidate = (mCurrentViewWidth == 0);
    mCurrentViewWidth = w;
    mCurrentViewHeight = h;
    mCurrentViewScale = scale;
    if (needInvalidate) {
        // ensure {@link #webkitDraw} is called as we were blocking in
        // {@link #contentDraw} when mCurrentViewWidth is 0
        if (DebugFlags::WEB_VIEW_CORE) Logger::V(LOGTAG, "viewSizeChanged");
        ContentDraw();
    }
}

// Calculate width to be used in webkit window.
Int32 CWebViewCore::CalculateWindowWidth(
    /* [in] */ Int32 viewWidth)
{
    Int32 width = viewWidth;
    if (mSettings->GetUseWideViewPort()) {
        if (mViewportWidth == -1) {
            // Fixed viewport width.
            width = CWebViewClassic::DEFAULT_VIEWPORT_WIDTH;
        }
        else if (mViewportWidth > 0) {
            // Use website specified or desired fixed viewport width.
            width = mViewportWidth;
        }
        else {
            // For mobile web site.
            width = Elastos::Core::Math::Round(mWebViewClassic->GetViewWidth() /
                    mWebViewClassic->GetDefaultZoomScale());
        }
    }
    return width;
}

// Utility method for exceededDatabaseQuota callback. Computes the sum
// of WebSQL database quota for all origins.
Int64 CWebViewCore::GetUsedQuota()
{
    AutoPtr<WebStorageClassic> webStorage = WebStorageClassic::GetInstance();
    AutoPtr<ICollection> origins = webStorage->GetOriginsSync();

    if (origins == NULL) {
        return 0;
    }
    Int64 usedQuota = 0;
    AutoPtr< ArrayOf<IInterface*> > originsArray;
    origins->ToArray((ArrayOf<IInterface*>**)&originsArray);
    for (Int32 i = 0; i < originsArray->GetLength(); i++) {
        IWebStorageOrigin* website = IWebStorageOrigin::Probe((*originsArray)[i]);
        Int64 quota;
        website->GetQuota(&quota);
        usedQuota += quota;
    }
    return usedQuota;
}

void CWebViewCore::PauseWebKitDraw()
{
    Mutex::Autolock lock(mSkipDrawFlagLock);
    if (!mSkipDrawFlag) {
        mSkipDrawFlag = TRUE;
    }
}

void CWebViewCore::ResumeWebKitDraw()
{
    Mutex::Autolock lock(mSkipDrawFlagLock);
    if (mSkipDrawFlag && mDrawWasSkipped) {
        // a draw was dropped, send a retry
        mDrawWasSkipped = FALSE;
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(NULL, EventHub::WEBKIT_DRAW, (IMessage**)&msg);
        mEventHub->SendMessage(msg);
    }
    mSkipDrawFlag = FALSE;
}

void CWebViewCore::WebkitDraw()
{
    {
        Mutex::Autolock lock(mSkipDrawFlagLock);
        if (mSkipDrawFlag) {
            mDrawWasSkipped = TRUE;
            return;
        }
    }

    mDrawIsScheduled = FALSE;
    AutoPtr<CWebViewCoreDrawData> draw = new CWebViewCoreDrawData();
    //if (DebugFlags::WEB_VIEW_CORE) Logger::V(LOGTAG, "webkitDraw start");
    draw->mBaseLayer = NativeRecordContent(mNativeClass, draw->mContentSize);
    if (draw->mBaseLayer == 0) {
        Boolean isPaused;
        if (mWebViewClassic != NULL && (mWebViewClassic->IsPaused(&isPaused), !isPaused)) {
            if (DebugFlags::WEB_VIEW_CORE) Logger::V(LOGTAG, "webkitDraw abort, resending draw message");
            AutoPtr<IMessageHelper> helper;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
            AutoPtr<IMessage> msg;
            helper->Obtain(NULL, EventHub::WEBKIT_DRAW, (IMessage**)&msg);
            mEventHub->SendMessageDelayed(msg, 10);
        }
        else {
            if (DebugFlags::WEB_VIEW_CORE) Logger::V(LOGTAG, "webkitDraw abort, webview paused");
        }
        return;
    }
    mLastDrawData = draw;
    WebkitDraw(draw);
}

void CWebViewCore::WebkitDraw(
    /* [in] */ CWebViewCoreDrawData* draw)
{
    if (mWebViewClassic != NULL) {
        AutoPtr<IPoint> p;
        CPoint::New(mCurrentViewWidth, mCurrentViewHeight, (IPoint**)&p);
        draw->mViewSize = p;
        if (mSettings->GetUseWideViewPort()) {
            draw->mMinPrefWidth = Elastos::Core::Math::Max(
                    mViewportWidth == -1 ? CWebViewClassic::DEFAULT_VIEWPORT_WIDTH
                            : (mViewportWidth == 0 ? mCurrentViewWidth
                                    : mViewportWidth),
                    NativeGetContentMinPrefWidth(mNativeClass));
        }
        if (mInitialViewState != NULL) {
            draw->mViewState = mInitialViewState;
            mInitialViewState = NULL;
        }
        if (mFirstLayoutForNonStandardLoad) {
            draw->mFirstLayoutForNonStandardLoad = TRUE;
            mFirstLayoutForNonStandardLoad = FALSE;
        }
        //if (DebugFlags::WEB_VIEW_CORE) Logger::V(LOGTAG, "webkitDraw NEW_PICTURE_MSG_ID");
        PauseWebKitDraw();
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::NEW_PICTURE_MSG_ID, (IInterface*)draw, (IMessage**)&msg);
        msg->SendToTarget();
    }
}

void CWebViewCore::SaveViewState(
    /* [in] */ IOutputStream* stream,
    /* [in] */ IValueCallback* callback)
{
    // TODO: Create a native method to do this better without overloading
    // the draw path (and fix saving <canvas>)
    AutoPtr<CWebViewCoreDrawData> draw = new CWebViewCoreDrawData();
    if (DebugFlags::WEB_VIEW_CORE) Logger::V(LOGTAG, "saveViewState start");
    draw->mBaseLayer = NativeRecordContent(mNativeClass, draw->mContentSize);
    Boolean result = FALSE;
    //try {
    result = ViewStateSerializer::SerializeViewState(stream, draw);
    //} catch (Throwable t) {
    //    Log.w(LOGTAG, "Failed to save view state", t);
    //}
    AutoPtr<IBoolean> resultObj;
    CBoolean::New(result, (IBoolean**)&resultObj);
    callback->OnReceiveValue(resultObj);
    if (draw->mBaseLayer != 0) {
        if (mDrawIsScheduled) {
            mDrawIsScheduled = FALSE;
            mEventHub->RemoveMessages(EventHub::WEBKIT_DRAW);
        }
        mLastDrawData = draw;
        WebkitDraw(draw);
    }
}

void CWebViewCore::ReducePriority()
{
    // remove the pending REDUCE_PRIORITY and RESUME_PRIORITY messages
    sWebCoreHandler->RemoveMessages(WebCoreThread::REDUCE_PRIORITY);
    sWebCoreHandler->RemoveMessages(WebCoreThread::RESUME_PRIORITY);
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(sWebCoreHandler,
            WebCoreThread::REDUCE_PRIORITY, (IMessage**)&msg);
    Boolean result;
    sWebCoreHandler->SendMessageAtFrontOfQueue(msg, &result);
}

void CWebViewCore::ResumePriority()
{
    // remove the pending REDUCE_PRIORITY and RESUME_PRIORITY messages
    sWebCoreHandler->RemoveMessages(WebCoreThread::REDUCE_PRIORITY);
    sWebCoreHandler->RemoveMessages(WebCoreThread::RESUME_PRIORITY);
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(sWebCoreHandler,
            WebCoreThread::RESUME_PRIORITY, (IMessage**)&msg);
    Boolean result;
    sWebCoreHandler->SendMessageAtFrontOfQueue(msg, &result);
}

void CWebViewCore::SendStaticMessage(
    /* [in] */ Int32 messageType,
    /* [in] */ IInterface* argument)
{
    if (sWebCoreHandler == NULL) return;

    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(sWebCoreHandler, messageType, argument, (IMessage**)&msg);
    Boolean result;
    sWebCoreHandler->SendMessage(msg, &result);
}

void CWebViewCore::PauseUpdatePicture(
    /* [in] */ CWebViewCore* core)
{
    // Note: there is one possible failure mode. If pauseUpdatePicture() is
    // called from UI thread while WEBKIT_DRAW is just pulled out of the
    // queue in WebCore thread to be executed. Then update won't be blocked.
    if (core != NULL) {
        AutoPtr<IWebSettingsClassic> settings;
        core->GetSettings((IWebSettingsClassic**)&settings);
        Boolean result;
        if (settings->EnableSmoothTransition(&result), !result) return;

        {
            Mutex::Autolock lock(core->mLock);
            if (core->mNativeClass == 0) {
                Logger::W(LOGTAG, "Cannot pauseUpdatePicture, core destroyed or not initialized!");
                return;
            }
            core->mDrawIsPaused = TRUE;
        }
    }
}

void CWebViewCore::ResumeUpdatePicture(
    /* [in] */ CWebViewCore* core)
{
    if (core != NULL) {
        // if mDrawIsPaused is true, ignore the setting, continue to resume
        if (!core->mDrawIsPaused) return;

        {
            Mutex::Autolock lock(core->mLock);
            if (core->mNativeClass == 0) {
                Logger::W(LOGTAG, "Cannot resumeUpdatePicture, core destroyed!");
                return;
            }
            core->mDrawIsPaused = FALSE;
            // always redraw on resume to reenable gif animations
            core->mDrawIsScheduled = FALSE;
        }
    }
}

Boolean CWebViewCore::IsUpdatePicturePaused(
    /* [in] */ CWebViewCore* core)
{
    return core != NULL ? core->mDrawIsPaused : FALSE;
}

//////////////////////////////////////////////////////////////////////////

void CWebViewCore::RestoreState(
    /* [in] */ Int32 index)
{
    AutoPtr<WebBackForwardListClassic> list = mCallbackProxy->GetBackForwardList();
    Int32 size;
    list->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        list->GetItemAtIndex(i)->Inflate(mBrowserFrame->mNativeFrame);
    }
    mBrowserFrame->mLoadInitFromElastos = TRUE;
    WebBackForwardListClassic::RestoreIndex(mBrowserFrame->mNativeFrame, index);
    mBrowserFrame->mLoadInitFromElastos = FALSE;
}

//-------------------------------------------------------------------------
// Implement abstract methods in WebViewCore, native WebKit callback part
//-------------------------------------------------------------------------

// called from JNI or WebView thread
/* package */
void CWebViewCore::ContentDraw()
{
    //if (DebugFlags::WEB_VIEW_CORE) {
        //Logger::V(LOGTAG, "CWebViewCore::ContentDraw");
    //}
    Mutex::Autolock lock(mLock);
    if (mWebViewClassic == NULL || mBrowserFrame == NULL) {
        // We were destroyed
        return;
    }
    // don't update the Picture until we have an initial width and finish
    // the first layout
    if (mCurrentViewWidth == 0 || !mBrowserFrame->FirstLayoutDone()) {
        return;
    }
    // only fire an event if this is our first request
    if (mDrawIsScheduled) return;
    mDrawIsScheduled = TRUE;
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(NULL, EventHub::WEBKIT_DRAW, (IMessage**)&msg);
    mEventHub->SendMessage(msg);
}

// called by JNI
void CWebViewCore::ContentScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Boolean animate,
    /* [in] */ Boolean onlyIfImeIsShowing)
{
    if (DebugFlags::WEB_VIEW_CORE) {
        Logger::V(LOGTAG, "CWebViewCore::ContentScrollTo, x:%d, y:%d, animate:%d, onlyIfImeIsShowing:%d", x,y,animate,onlyIfImeIsShowing);
    }
    if (!mBrowserFrame->FirstLayoutDone()) {
        /*
         * WebKit restore state will be called before didFirstLayout(),
         * remember the position as it has to be applied after restoring
         * zoom factor which is controlled by screenWidth.
         */
        mRestoredX = x;
        mRestoredY = y;
        return;
    }
    if (mWebViewClassic != NULL) {
        AutoPtr<IPoint> p;
        CPoint::New(x, y, (IPoint**)&p);
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler, CWebViewClassic::SCROLL_TO_MSG_ID,
                animate ? 1 : 0, onlyIfImeIsShowing ? 1 : 0, p, (IMessage**)&msg);
        if (mDrawIsScheduled) {
            AutoPtr<IMessage> m;
            helper->Obtain(NULL, EventHub::MESSAGE_RELAY, msg, (IMessage**)&m);
            mEventHub->SendMessage(m);
        }
        else {
            msg->SendToTarget();
        }
    }
}

// called by JNI
void CWebViewCore::SendNotifyProgressFinished()
{
    if (DebugFlags::WEB_VIEW_CORE) {
        Logger::V(LOGTAG, "CWebViewCore::SendNotifyProgressFinished");
    }
    ContentDraw();
}

/*  Called by JNI. The coordinates are in doc coordinates, so they need to
    be scaled before they can be used by the view system, which happens
    in WebView since it (and its thread) know the current scale factor.
 */
void CWebViewCore::SendViewInvalidate(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    if (mWebViewClassic != NULL) {
        AutoPtr<IRect> r;
        CRect::New(left, top, right, bottom, (IRect**)&r);
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::INVAL_RECT_MSG_ID, r, (IMessage**)&msg);
        msg->SendToTarget();
    }
}

/*
 * Called by the WebView thread
 */
void CWebViewCore::SignalRepaintDone()
{
    sRepaintScheduled = FALSE;
}

// Gets the WebViewClassic corresponding to this WebViewCore. Note that the
// WebViewClassic object must only be used on the UI thread.
AutoPtr<CWebViewClassic> CWebViewCore::GetWebViewClassic()
{
    return mWebViewClassic;
}

// Called by JNI
AutoPtr<IWebView> CWebViewCore::GetWebView()
{
    AutoPtr<IWebView> view;
    mWebViewClassic->GetWebView((IWebView**)&view);
    return view;
}

// Called by JNI
void CWebViewCore::SendPluginDrawMsg()
{
    SendMessage(EventHub::PLUGIN_SURFACE_READY);
}

void CWebViewCore::SetViewportSettingsFromNative(
    /* [in] */ Int32 nativeClass)
{
    Elastos_WebViewCore_SetViewportSettingsFromNative(nativeClass);
}

// called by JNI
void CWebViewCore::DidFirstLayout(
    /* [in] */ Boolean standardLoad)
{
    if (DebugFlags::WEB_VIEW_CORE) {
        Logger::V(LOGTAG, "didFirstLayout standardLoad =%d", standardLoad);
    }

    mBrowserFrame->DidFirstLayout();

    if (mWebViewClassic == NULL) return;

    Boolean updateViewState = standardLoad || mIsRestored;
    SetupViewport(updateViewState);
    // if updateRestoreState is true, ViewManager.postReadyToDrawAll() will
    // be called after the WebView updates its state. If updateRestoreState
    // is false, start to draw now as it is ready.
    if (!updateViewState) {
        mWebViewClassic->mViewManager->PostReadyToDrawAll();
    }

    // remove the touch highlight when moving to a new page
    Boolean result;
    mWebViewClassic->mPrivateHandler->SendEmptyMessage(
            CWebViewClassic::HIT_TEST_RESULT, &result);

    // reset the scroll position, the restored offset and scales
    mRestoredX = mRestoredY = 0;
    mIsRestored = FALSE;
    mRestoredScale = mRestoredTextWrapScale = 0;
}

// called by JNI
void CWebViewCore::UpdateViewport()
{
    // Update viewport asap to make sure we get correct one.
    SetupViewport(TRUE);
}

Float CWebViewCore::GetFixedDisplayDensity(
    /* [in] */ IContext* context)
{
    // We make bad assumptions about multiplying and dividing density by 100,
    // force them to be true with this hack
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> displayMetrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);
    Float density;
    displayMetrics->GetDensity(&density);
    return ((Int32) (density * 100)) / 100.0f;
}

void CWebViewCore::SetupViewport(
    /* [in] */ Boolean updateViewState)
{
    if (mWebViewClassic == NULL || mSettings == NULL) {
        // We've been destroyed or are being destroyed, return early
        return;
    }
    // set the viewport settings from WebKit
    SetViewportSettingsFromNative(mNativeClass);

    // clamp initial scale
    if (mViewportInitialScale > 0) {
        if (mViewportMinimumScale > 0) {
            mViewportInitialScale = Elastos::Core::Math::Max(mViewportInitialScale,
                    mViewportMinimumScale);
        }
        if (mViewportMaximumScale > 0) {
            mViewportInitialScale = Elastos::Core::Math::Min(mViewportInitialScale,
                    mViewportMaximumScale);
        }
    }

    Boolean result;
    if (mSettings->ForceUserScalable(&result), result) {
        mViewportUserScalable = TRUE;
        if (mViewportInitialScale > 0) {
            if (mViewportMinimumScale > 0) {
                mViewportMinimumScale = Elastos::Core::Math::Min(mViewportMinimumScale,
                        mViewportInitialScale / 2);
            }
            if (mViewportMaximumScale > 0) {
                mViewportMaximumScale = Elastos::Core::Math::Max(mViewportMaximumScale,
                        mViewportInitialScale * 2);
            }
        }
        else {
            if (mViewportMinimumScale > 0) {
                mViewportMinimumScale = Elastos::Core::Math::Min(mViewportMinimumScale, 50);
            }
            if (mViewportMaximumScale > 0) {
                mViewportMaximumScale = Elastos::Core::Math::Max(mViewportMaximumScale, 200);
            }
        }
    }

    // adjust the default scale to match the densityDpi
    Float adjust = 1.0f;
    if (mViewportDensityDpi == -1) {
        adjust = GetFixedDisplayDensity(mContext);
    }
    else if (mViewportDensityDpi > 0) {
        AutoPtr<IResources> res;
        mContext->GetResources((IResources**)&res);
        AutoPtr<IDisplayMetrics> displayMetrics;
        res->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);
        Int32 dip;
        displayMetrics->GetDensityDpi(&dip);
        adjust = (Float) dip / mViewportDensityDpi;
        adjust = ((Int32) (adjust * 100)) / 100.0f;
    }

    // Remove any update density messages in flight.
    // If the density is indeed different from WebView's default scale,
    // a new message will be queued.
    mWebViewClassic->mPrivateHandler->RemoveMessages(
            CWebViewClassic::UPDATE_ZOOM_DENSITY);
    if (adjust != mWebViewClassic->GetDefaultZoomScale()) {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        AutoPtr<IFloat> adjustObj;
        CFloat::New(adjust, (IFloat**)&adjustObj);
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::UPDATE_ZOOM_DENSITY, adjustObj.Get(), (IMessage**)&msg);
        msg->SendToTarget();
    }
    Int32 defaultScale = (Int32) (adjust * 100);

    if (mViewportInitialScale > 0) {
        mViewportInitialScale *= adjust;
    }
    if (mViewportMinimumScale > 0) {
        mViewportMinimumScale *= adjust;
    }
    if (mViewportMaximumScale > 0) {
        mViewportMaximumScale *= adjust;
    }

    // infer the values if they are not defined.
    if (mViewportWidth == 0) {
        if (mViewportInitialScale == 0) {
            mViewportInitialScale = defaultScale;
        }
    }
    if (mViewportUserScalable == false) {
        mViewportInitialScale = defaultScale;
        mViewportMinimumScale = defaultScale;
        mViewportMaximumScale = defaultScale;
    }
    if (mViewportMinimumScale > mViewportInitialScale
            && mViewportInitialScale != 0) {
        mViewportMinimumScale = mViewportInitialScale;
    }
    if (mViewportMaximumScale > 0
            && mViewportMaximumScale < mViewportInitialScale) {
        mViewportMaximumScale = mViewportInitialScale;
    }
    if (mViewportWidth < 0 && mViewportInitialScale == defaultScale) {
        mViewportWidth = 0;
    }

    // if mViewportWidth is 0, it means device-width, always update.
    if (mViewportWidth != 0 && !updateViewState) {
        // For non standard load, since updateViewState will be false.
        mFirstLayoutForNonStandardLoad = TRUE;
        AutoPtr<CWebViewCoreViewState> viewState = new CWebViewCoreViewState();
        viewState->mMinScale = mViewportMinimumScale / 100.0f;
        viewState->mMaxScale = mViewportMaximumScale / 100.0f;
        viewState->mDefaultScale = adjust;
        // as mViewportWidth is not 0, it is not mobile site.
        viewState->mMobileSite = FALSE;
        // for non-mobile site, we don't need minPrefWidth, set it as 0
        viewState->mScrollX = 0;
        viewState->mShouldStartScrolledRight = FALSE;
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::UPDATE_ZOOM_RANGE, viewState.Get(), (IMessage**)&msg);
        msg->SendToTarget();
        return;
    }

    // now notify webview
    // webViewWidth refers to the width in the view system
    Int32 webViewWidth;
    // viewportWidth refers to the width in the document system
    Int32 viewportWidth = mCurrentViewWidth;
    if (viewportWidth == 0) {
        // this may happen when WebView just starts. This is not perfect as
        // we call WebView method from WebCore thread. But not perfect
        // reference is better than no reference.
        webViewWidth = mWebViewClassic->GetViewWidth();
        viewportWidth = (Int32) (webViewWidth / adjust);
        if (viewportWidth == 0) {
            if (DebugFlags::WEB_VIEW_CORE) {
                Logger::V(LOGTAG, "Can't get the viewWidth yet");
            }
        }
    }
    else {
        webViewWidth = Elastos::Core::Math::Round(viewportWidth * mCurrentViewScale);
    }
    mInitialViewState = new CWebViewCoreViewState();
    mInitialViewState->mMinScale = mViewportMinimumScale / 100.0f;
    mInitialViewState->mMaxScale = mViewportMaximumScale / 100.0f;
    mInitialViewState->mDefaultScale = adjust;
    mInitialViewState->mScrollX = mRestoredX;
    mInitialViewState->mScrollY = mRestoredY;
    mInitialViewState->mShouldStartScrolledRight = (mRestoredX == 0)
            && (mRestoredY == 0)
            && (mBrowserFrame != NULL)
            && mBrowserFrame->GetShouldStartScrolledRight();

    mInitialViewState->mMobileSite = (0 == mViewportWidth);
    if (mIsRestored) {
        mInitialViewState->mIsRestored = TRUE;
        mInitialViewState->mViewScale = mRestoredScale;
        if (mRestoredTextWrapScale > 0) {
            mInitialViewState->mTextWrapScale = mRestoredTextWrapScale;
        }
        else {
            mInitialViewState->mTextWrapScale = mInitialViewState->mViewScale;
        }
    }
    else {
        if (mViewportInitialScale > 0) {
            mInitialViewState->mViewScale = mInitialViewState->mTextWrapScale =
                    mViewportInitialScale / 100.0f;
        }
        else if (mViewportWidth > 0 && mViewportWidth < webViewWidth &&
            !mSettings->GetUseFixedViewport()) {
            mInitialViewState->mViewScale = mInitialViewState->mTextWrapScale =
                    (Float) webViewWidth / mViewportWidth;
        }
        else {
            mInitialViewState->mTextWrapScale = adjust;
            if (mSettings->GetUseWideViewPort()) {
                // 0 will trigger WebView to turn on zoom overview mode
                mInitialViewState->mViewScale = 0;
            }
            else {
                mInitialViewState->mViewScale = adjust;
            }
        }
    }

    if (DebugFlags::WEB_VIEW_CORE) {
        Logger::V(LOGTAG, "CWebViewCore::SetupViewport, HeightCanMeasure:%d", mWebViewClassic->mHeightCanMeasure);
    }

    if (mWebViewClassic->mHeightCanMeasure) {
        // Trick to ensure that the Picture has the exact height for the
        // content by forcing to layout with 0 height after the page is
        // ready, which is indicated by didFirstLayout. This is essential to
        // get rid of the white space in the GMail which uses WebView for
        // message view.
        mWebViewClassic->mLastHeightSent = 0;
        // Send a negative scale to indicate that WebCore should reuse
        // the current scale
        AutoPtr<CWebViewClassic::ViewSizeData> data = new CWebViewClassic::ViewSizeData();
        data->mWidth = mWebViewClassic->mLastWidthSent;
        data->mHeight = 0;
        // if mHeightCanMeasure is true, getUseWideViewPort() can't be
        // true. It is safe to use mWidth for mTextWrapWidth.
        data->mTextWrapWidth = data->mWidth;
        data->mScale = -1.0f;
        data->mIgnoreHeight = FALSE;
        data->mAnchorX = data->mAnchorY = 0;
        // send VIEW_SIZE_CHANGED to the front of the queue so that we can
        // avoid pushing the wrong picture to the WebView side. If there is
        // a VIEW_SIZE_CHANGED in the queue, probably from WebView side,
        // ignore it as we have a new size. If we leave VIEW_SIZE_CHANGED
        // in the queue, as mLastHeightSent has been updated here, we may
        // miss the requestLayout in WebView side after the new picture.
        mEventHub->RemoveMessages(EventHub::VIEW_SIZE_CHANGED);
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(NULL,
                EventHub::VIEW_SIZE_CHANGED, data.Get(), (IMessage**)&msg);
        mEventHub->SendMessageAtFrontOfQueue(msg);
    }
    else {
        if (viewportWidth == 0) {
            // Trick to ensure VIEW_SIZE_CHANGED will be sent from WebView
            // to WebViewCore
            mWebViewClassic->mLastWidthSent = 0;
        }
        else {
            AutoPtr<CWebViewClassic::ViewSizeData> data = new CWebViewClassic::ViewSizeData();
            // mViewScale as 0 means it is in zoom overview mode. So we don't
            // know the exact scale. If mRestoredScale is non-zero, use it;
            // otherwise just use mTextWrapScale as the initial scale.
            Float tentativeScale = mInitialViewState->mViewScale;
            if (tentativeScale == 0) {
                // The following tries to figure out more correct view scale
                // and text wrap scale to be sent to webkit, by using some
                // knowledge from web settings and zoom manager.

                // Calculated window width will be used to guess the scale
                // in zoom overview mode.
                tentativeScale = mInitialViewState->mTextWrapScale;
                Int32 tentativeViewWidth = Elastos::Core::Math::Round(webViewWidth / tentativeScale);
                Int32 windowWidth = CalculateWindowWidth(tentativeViewWidth);
                // In viewport setup time, since no content width is known, we assume
                // the windowWidth will be the content width, to get a more likely
                // zoom overview scale.
                data->mScale = (Float) webViewWidth / windowWidth;
                if (!mSettings->GetLoadWithOverviewMode()) {
                    // If user choose non-overview mode.
                    data->mScale = Elastos::Core::Math::Max(data->mScale, tentativeScale);
                }
                if (mSettings->IsNarrowColumnLayout()) {
                    // In case of automatic text reflow in fixed view port mode.
                    mInitialViewState->mTextWrapScale =
                            mWebViewClassic->ComputeReadingLevelScale(data->mScale);
                }
            }
            else {
                // Scale is given such as when page is restored, use it.
                data->mScale = tentativeScale;
            }
            if (DebugFlags::WEB_VIEW_CORE) {
                Logger::V(LOGTAG, "setupViewport mRestoredScale=%f mViewScale=%f mTextWrapScale=%f data.mScale=%f ",
                        mRestoredScale, mInitialViewState->mViewScale, mInitialViewState->mTextWrapScale, data->mScale);
            }
            data->mWidth = Elastos::Core::Math::Round(webViewWidth / data->mScale);
            // We may get a call here when mCurrentViewHeight == 0 if webcore completes the
            // first layout before we sync our webview dimensions to it. In that case, we
            // request the real height of the webview. This is not a perfect solution as we
            // are calling a WebView method from the WebCore thread. But this is preferable
            // to syncing an incorrect height.
            data->mHeight = mCurrentViewHeight == 0 ?
                    Elastos::Core::Math::Round(mWebViewClassic->GetViewHeight() / data->mScale)
                    : Elastos::Core::Math::Round((Float) mCurrentViewHeight * data->mWidth / viewportWidth);
            data->mTextWrapWidth = Elastos::Core::Math::Round(webViewWidth
                    / mInitialViewState->mTextWrapScale);
            data->mIgnoreHeight = FALSE;
            data->mAnchorX = data->mAnchorY = 0;
            // send VIEW_SIZE_CHANGED to the front of the queue so that we
            // can avoid pushing the wrong picture to the WebView side.
            mEventHub->RemoveMessages(EventHub::VIEW_SIZE_CHANGED);
            // Let webkit know the scale and inner width/height immediately
            // in viewport setup time to avoid wrong information.
            ViewSizeChanged(data);
        }
    }
}

// called by JNI
void CWebViewCore::RestoreScale(
    /* [in] */ Float scale,
    /* [in] */ Float textWrapScale)
{
    if (mBrowserFrame->FirstLayoutDone() == FALSE) {
        mIsRestored = TRUE;
        mRestoredScale = scale;
        if (mSettings->GetUseWideViewPort()) {
            mRestoredTextWrapScale = textWrapScale;
        }
    }
}

// called by JNI
void CWebViewCore::NeedTouchEvents(
    /* [in] */ Boolean need)
{
    if (mWebViewClassic != NULL) {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::WEBCORE_NEED_TOUCH_EVENTS, need ? 1 : 0, 0, (IMessage**)&msg);
        msg->SendToTarget();
    }
}

// called by JNI
void CWebViewCore::UpdateTextfield(
    /* [in] */ Int32 ptr,
    /* [in] */ const String& text,
    /* [in] */ Int32 textGeneration)
{
    if (mWebViewClassic != NULL) {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(text, (ICharSequence**)&cs);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::UPDATE_TEXTFIELD_TEXT_MSG_ID, ptr, textGeneration, cs, (IMessage**)&msg);
        msg->SendToTarget();
    }
}

AutoPtr<CWebViewCoreTextSelectionData> CWebViewCore::CreateTextSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 selPtr)
{
    AutoPtr<CWebViewCoreTextSelectionData> data = new CWebViewCoreTextSelectionData(start, end, selPtr);
    data->mSelectionReason = mTextSelectionChangeReason;
    return data;
}

// called by JNI
void CWebViewCore::UpdateTextSelection(
    /* [in] */ Int32 pointer,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 textGeneration,
    /* [in] */ Int32 selectionPtr)
{
    if (mWebViewClassic != NULL) {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<CWebViewCoreTextSelectionData> data = CreateTextSelection(start, end, selectionPtr);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::UPDATE_TEXT_SELECTION_MSG_ID, pointer, textGeneration, data.Get(), (IMessage**)&msg);
        msg->SendToTarget();
    }
}

// called by JNI
void CWebViewCore::UpdateTextSizeAndScroll(
    /* [in] */ Int32 pointer,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY)
{
    if (mWebViewClassic != NULL) {
        AutoPtr<IRect> rect;
        CRect::New(-scrollX, -scrollY, width - scrollX,
                height - scrollY, (IRect**)&rect);
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::EDIT_TEXT_SIZE_CHANGED, pointer, 0, rect.Get(), (IMessage**)&msg);
        msg->SendToTarget();
    }
}

// called by JNI
void CWebViewCore::ClearTextEntry()
{
    if (mWebViewClassic == NULL) return;
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(mWebViewClassic->mPrivateHandler,
            CWebViewClassic::CLEAR_TEXT_ENTRY, (IMessage**)&msg);
    msg->SendToTarget();
}

// called by JNI
void CWebViewCore::InitEditField(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 selectionPtr,
    /* [in] */ CWebViewCoreTextFieldInitData* initData)
{
    if (mWebViewClassic == NULL) {
        return;
    }
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(mWebViewClassic->mPrivateHandler,
            CWebViewClassic::INIT_EDIT_FIELD, initData, (IMessage**)&msg);
    msg->SendToTarget();
    AutoPtr<CWebViewCoreTextSelectionData> data = CreateTextSelection(start, end, selectionPtr);
    msg = NULL;
    helper->Obtain(mWebViewClassic->mPrivateHandler,
            CWebViewClassic::UPDATE_TEXT_SELECTION_MSG_ID, initData->mFieldPointer, 0, data, (IMessage**)&msg);
    msg->SendToTarget();
}

void CWebViewCore::NativeRevealSelection(
    /* [in] */ Int32 nativeClass)
{
    Elastos_WebViewCore_RevealSelection(nativeClass);
}

String CWebViewCore::NativeRequestLabel(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 framePtr,
    /* [in] */ Int32 nodePtr)
{
    return Elastos_WebViewCore_RequestLabel(nativeClass, framePtr, nodePtr);
}

/**
 * Scroll the focused textfield to (xPercent, y) in document space
 */
void CWebViewCore::NativeScrollFocusedTextInput(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Float xPercent,
    /* [in] */ Int32 y)
{
    Elastos_WebViewCore_ScrollFocusedTextInput(nativeClass, xPercent, y);
}

// these must be in document space (i.e. not scaled/zoomed).
void CWebViewCore::NativeSetScrollOffset(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Boolean sendScrollEvent,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    Elastos_WebViewCore_SetScrollOffset(nativeClass, sendScrollEvent, dx, dy);
}

void CWebViewCore::NativeSetGlobalBounds(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    Elastos_WebViewCore_SetGlobalBounds(nativeClass, x, y, w, h);
}

// called by JNI
void CWebViewCore::RequestListBox(
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ ArrayOf<Int32>* enabledArray,
    /* [in] */ ArrayOf<Int32>* selectedArray)
{
    if (mWebViewClassic != NULL) {
        mWebViewClassic->RequestListBox(array, enabledArray, selectedArray);
    }
}

// called by JNI
void CWebViewCore::RequestListBox(
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ ArrayOf<Int32>* enabledArray,
    /* [in] */ Int32 selection)
{
    if (mWebViewClassic != NULL) {
        mWebViewClassic->RequestListBox(array, enabledArray, selection);
    }
}

// called by JNI
void CWebViewCore::RequestKeyboard(
    /* [in] */ Boolean showKeyboard)
{
    if (mWebViewClassic != NULL) {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::REQUEST_KEYBOARD, showKeyboard ? 1 : 0, 0, (IMessage**)&msg);
        msg->SendToTarget();
    }
}

void CWebViewCore::SetWebTextViewAutoFillable(
    /* [in] */ Int32 queryId,
    /* [in] */ const String& preview)
{
    if (mWebViewClassic != NULL) {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<CWebViewCoreAutoFillData> data = new CWebViewCoreAutoFillData(queryId, preview);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::SET_AUTOFILLABLE, (IInterface*)data.Get(), (IMessage**)&msg);
        msg->SendToTarget();
    }
}

AutoPtr<IContext> CWebViewCore::GetContext()
{
    if (DebugFlags::WEB_VIEW_CORE) {
        Logger::W(LOGTAG, "context is: %p", mContext.Get());
    }
    return mContext;
}

// called by JNI
void CWebViewCore::KeepScreenOn(
    /* [in] */ Boolean screenOn)
{
    if (mWebViewClassic != NULL) {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::SCREEN_ON, (IMessage**)&msg);
        msg->SetArg1(screenOn ? 1 : 0);
        msg->SendToTarget();
    }
}

// called by JNI
AutoPtr<IInterface> CWebViewCore::GetPluginClass(
    /* [in] */ const String& libName,
    /* [in] */ const String& clsName)
{
    if (mWebViewClassic == NULL) {
        return NULL;
    }

    AutoPtr<IPluginManagerHelper> pmh;
    CPluginManagerHelper::AcquireSingleton((IPluginManagerHelper**)&pmh);
    AutoPtr<CPluginManager> pluginManager;
    pmh->GetInstance(NULL, (IPluginManager**)&pluginManager);

    String pkgName = pluginManager->GetPluginsAPKName(libName);
    if (pkgName.IsNull()) {
        Logger::W(LOGTAG, "Unable to resolve %s to a plugin APK", libName.string());
        return NULL;
    }

    //try {
    return pluginManager->GetPluginClass(pkgName, clsName);
    //} catch (NameNotFoundException e) {
    //    Log.e(LOGTAG, "Unable to find plugin classloader for the apk (" + pkgName + ")");
    //} catch (ClassNotFoundException e) {
    //    Log.e(LOGTAG, "Unable to find plugin class (" + clsName +
    //            ") in the apk (" + pkgName + ")");
    //}

    return NULL;
}

// called by JNI. PluginWidget function to launch a full-screen view using a
// View object provided by the plugin class.
void CWebViewCore::ShowFullScreenPlugin(
    /* [in] */ IInterface* childView,
    /* [in] */ Int32 orientation,
    /* [in] */ Int32 npp)
{
    if (mWebViewClassic == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(mWebViewClassic->mPrivateHandler,
            CWebViewClassic::SHOW_FULLSCREEN, (IMessage**)&msg);
    msg->SetObj(((ViewManager::ChildView*)childView)->mView);
    msg->SetArg1(orientation);
    msg->SetArg2(npp);
    msg->SendToTarget();
}

// called by JNI
void CWebViewCore::HideFullScreenPlugin()
{
    if (mWebViewClassic == NULL) {
        return;
    }
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(mWebViewClassic->mPrivateHandler,
            CWebViewClassic::HIDE_FULLSCREEN, (IMessage**)&msg);
    msg->SendToTarget();
}

AutoPtr<IInterface> CWebViewCore::CreateSurface(
    /* [in] */ IInterface* vpluginView)
{
    IView* pluginView = IView::Probe(vpluginView);
    if (mWebViewClassic == NULL) {
        return NULL;
    }

    if (pluginView == NULL) {
        Logger::E(LOGTAG, "Attempted to add an empty plugin view to the view hierarchy");
        return NULL;
    }

    // ensures the view system knows the view can redraw itself
    pluginView->SetWillNotDraw(FALSE);

    if (ISurfaceView::Probe(pluginView) != NULL) {
        ISurfaceView::Probe(pluginView)->SetZOrderOnTop(TRUE);
    }

    AutoPtr<ViewManager::ChildView> view = mWebViewClassic->mViewManager->CreateView();
    view->mView = pluginView;
    return view;
}

// called by JNI.  PluginWidget functions for creating an embedded View for
// the surface drawing model.
AutoPtr<IInterface> CWebViewCore::AddSurface(
    /* [in] */ IInterface* pluginView,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    AutoPtr<IInterface> view = CreateSurface(pluginView);
    //TODO how to handle view is null
    ViewManager::ChildView* childView = (ViewManager::ChildView*)(view.Get());
    childView->AttachView(x, y, width, height);
    return view;
}

void CWebViewCore::UpdateSurface(
    /* [in] */ IInterface* vChildView,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ViewManager::ChildView* childView = (ViewManager::ChildView*)(vChildView);
    childView->AttachView(x, y, width, height);
}

void CWebViewCore::DestroySurface(
    /* [in] */ IInterface* vChildView)
{
    ViewManager::ChildView* childView = (ViewManager::ChildView*)(vChildView);
    childView->RemoveView();
}

void CWebViewCore::ShowRect(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 contentWidth,
    /* [in] */ Int32 contentHeight,
    /* [in] */ Float xPercentInDoc,
    /* [in] */ Float xPercentInView,
    /* [in] */ Float yPercentInDoc,
    /* [in] */ Float yPercentInView)
{
    if (mWebViewClassic != NULL) {
        AutoPtr<ShowRectData> data = new ShowRectData();
        data->mLeft = left;
        data->mTop = top;
        data->mWidth = width;
        data->mHeight = height;
        data->mContentWidth = contentWidth;
        data->mContentHeight = contentHeight;
        data->mXPercentInDoc = xPercentInDoc;
        data->mXPercentInView = xPercentInView;
        data->mYPercentInDoc = yPercentInDoc;
        data->mYPercentInView = yPercentInView;
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mWebViewClassic->mPrivateHandler,
                CWebViewClassic::SHOW_RECT_MSG_ID, data.Get(), (IMessage**)&msg);
        msg->SendToTarget();
    }
}

// called by JNI
void CWebViewCore::CenterFitRect(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (mWebViewClassic == NULL) {
        return;
    }
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IRect> r;
    CRect::New(x, y, x + width, y + height, (IRect**)&r);
    AutoPtr<IMessage> msg;
    helper->Obtain(mWebViewClassic->mPrivateHandler,
            CWebViewClassic::CENTER_FIT_RECT, r.Get(), (IMessage**)&msg);
    msg->SendToTarget();
}

// called by JNI
void CWebViewCore::SetScrollbarModes(
    /* [in] */ Int32 hMode,
    /* [in] */ Int32 vMode)
{
    if (mWebViewClassic == NULL) {
        return;
    }
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(mWebViewClassic->mPrivateHandler,
            CWebViewClassic::SET_SCROLLBAR_MODES, hMode, vMode, (IMessage**)&msg);
    msg->SendToTarget();
}

// called by JNI
void CWebViewCore::SelectAt(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    // TODO: Figure out what to do with this (b/6111818)
}

void CWebViewCore::SetUseMockDeviceOrientation()
{
    mDeviceMotionAndOrientationManager->SetUseMock();
}

void CWebViewCore::SetUseMockGeolocation()
{
    mMockGeolocation->SetUseMock();
}

ECode CWebViewCore::SetMockGeolocationPosition(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Double accuracy)
{
    mMockGeolocation->SetPosition(latitude, longitude, accuracy);
    return NOERROR;
}

ECode CWebViewCore::SetMockGeolocationError(
    /* [in] */ Int32 code,
    /* [in] */ const String& message)
{
    mMockGeolocation->SetError(code, message);
    return NOERROR;
}

ECode CWebViewCore::SetMockGeolocationPermission(
    /* [in] */ Boolean allow)
{
    mMockGeolocation->SetPermission(allow);
    return NOERROR;
}

ECode CWebViewCore::SetMockDeviceOrientation(
    /* [in] */ Boolean canProvideAlpha,
    /* [in] */ Double alpha,
    /* [in] */ Boolean canProvideBeta,
    /* [in] */ Double beta,
    /* [in] */ Boolean canProvideGamma,
    /* [in] */ Double gamma)
{
    mDeviceMotionAndOrientationManager->SetMockOrientation(canProvideAlpha, alpha,
            canProvideBeta, beta, canProvideGamma, gamma);
    return NOERROR;
}

AutoPtr<IInterface> CWebViewCore::GetDeviceMotionService()
{
    if (mDeviceMotionService == NULL) {
        mDeviceMotionService =
            new DeviceMotionService(mDeviceMotionAndOrientationManager, mContext);
    }
    return mDeviceMotionService;
}

AutoPtr<IInterface> CWebViewCore::GetDeviceOrientationService()
{
    if (mDeviceOrientationService == NULL) {
        mDeviceOrientationService =
                new DeviceOrientationService(mDeviceMotionAndOrientationManager, mContext);
    }
    return mDeviceOrientationService;
}

void CWebViewCore::SetShouldMonitorWebCoreThread()
{
    sShouldMonitorWebCoreThread = TRUE;
}

void CWebViewCore::NativePause(
    /* [in] */ Int32 nativeClass)
{
    Elastos_WebViewCore_Pause(nativeClass);
}

void CWebViewCore::NativeResume(
    /* [in] */ Int32 nativeClass)
{
    Elastos_WebViewCore_Resume(nativeClass);
}

void CWebViewCore::NativeFreeMemory(
    /* [in] */ Int32 nativeClass)
{
    Elastos_WebViewCore_FreeMemory(nativeClass);
}

void CWebViewCore::NativeFullScreenPluginHidden(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 npp)
{
    Elastos_WebViewCore_FullScreenPluginHidden(nativeClass, npp);
}

void CWebViewCore::NativePluginSurfaceReady(
    /* [in] */ Int32 nativeClass)
{
    Elastos_WebViewCore_PluginSurfaceReady(nativeClass);
}

AutoPtr<CWebViewCoreWebKitHitTest> CWebViewCore::NativeHitTest(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 slop,
    /* [in] */ Boolean doMoveMouse)
{
    AutoPtr<IInterface> wvcht = Elastos_WebViewCore_HitTest(nativeClass, x, y, slop, doMoveMouse);
    AutoPtr<CWebViewCoreWebKitHitTest> result = (CWebViewCoreWebKitHitTest*)(wvcht.Get());
    return result;
}

void CWebViewCore::NativeAutoFillForm(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 queryId)
{
    Elastos_WebViewCore_AutoFillForm(nativeClass, queryId);
}

void CWebViewCore::NativeScrollLayer(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 layer,
    /* [in] */ IRect* _rect)
{
    //ScrollRenderLayer
    CRect* r = (CRect*)_rect;
    Elastos_WebViewCore_ScrollRenderLayer(nativeClass, layer, r->mLeft, r->mTop, r->mRight, r->mBottom);
}

Int32 CWebViewCore::NativeFindAll(
    /* [in] */ Int32 nativeClass,
    /* [in] */ const String& text)
{
    return Elastos_WebViewCore_FindAll(nativeClass, text);
}

Int32 CWebViewCore::NativeFindNext(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Boolean forward)
{
    return Elastos_WebViewCore_FindNext(nativeClass, forward);
}

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
void CWebViewCore::NativeDeleteText(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 endX,
    /* [in] */ Int32 endY)
{
    Elastos_WebViewCore_DeleteText(nativeClass, startX, startY, endX, endY);
}

/**
 * Inserts text at the current cursor position. If the currently-focused
 * node does not have a cursor position then this function does nothing.
 */
void CWebViewCore::NativeInsertText(
    /* [in] */ Int32 nativeClass,
    /* [in] */ const String& text)
{
    Elastos_WebViewCore_InsertText(nativeClass, text);
}

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
String CWebViewCore::NativeGetText(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 endX,
    /* [in] */ Int32 endY)
{
    return Elastos_WebViewCore_GetText(nativeClass, startX, startY, endX, endY);
}

void CWebViewCore::NativeSelectText(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 handleId,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Elastos_WebViewCore_SelectText(nativeClass, handleId, x, y);
}

void CWebViewCore::NativeClearTextSelection(
    /* [in] */ Int32 nativeClass)
{
    //ClearSelection
    Elastos_WebViewCore_ClearSelection(nativeClass);
}

Boolean CWebViewCore::NativeSelectWordAt(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return Elastos_WebViewCore_SelectWordAt(nativeClass, x, y);
}

void CWebViewCore::NativeSelectAll(
    /* [in] */ Int32 nativeClass)
{
    Elastos_WebViewCore_SelectAll(nativeClass);
}

void CWebViewCore::NativeSetInitialFocus(
    /* [in] */ Int32 nativeClass,
    /* [in] */ Int32 keyDirection)
{
    Elastos_WebViewCore_SetInitialFocus(nativeClass, keyDirection);
}

void CWebViewCore::NativeCertTrustChanged()
{
    //nativeCertTrustChanged
    Elastos_WebViewCore_nativeCertTrustChanged();
}

void CWebViewCore::SetNativeClass(
    /* [in] */ Int32 nativeClass)
{
    mNativeClass = nativeClass;
}

Int32 CWebViewCore::GetNativeClass()
{
    return mNativeClass;
}

void CWebViewCore::SetViewportWidth(
    /* [in] */Int32 viewportWidth)
{
    mViewportWidth = viewportWidth;
}

void CWebViewCore::SetViewportHeight(
    /* [in] */ Int32 viewportHeight)
{
    mViewportHeight = viewportHeight;
}

void CWebViewCore::SetViewportInitialScale(
    /* [in] */ Int32 viewportInitialScale)
{
    mViewportInitialScale = viewportInitialScale;
}

void CWebViewCore::SetViewportMinimumScale(
    /* [in] */ Int32 viewportMinimumScale)
{
    mViewportMinimumScale = viewportMinimumScale;
}

void CWebViewCore::SetViewportMaximumScale(
    /* [in] */ Int32 viewportMaximumScale)
{
    mViewportMaximumScale = viewportMaximumScale;
}

void CWebViewCore::SetViewportUserScalable(
    /* [in] */ Boolean viewportUserScalable)
{
    mViewportUserScalable = viewportUserScalable;
}

void CWebViewCore::SetViewportDensityDpi(
    /* [in] */ Int32 viewportDensityDpi)
{
    mViewportDensityDpi = viewportDensityDpi;
}

void CWebViewCore::SetDrawIsPaused(
    /* [in] */ Boolean drawIsPaused)
{
    mDrawIsPaused = drawIsPaused;
}

Int32 CWebViewCore::GetLowMemoryUsageThresholdMb()
{
    return mLowMemoryUsageThresholdMb;
}

Int32 CWebViewCore::GetHighMemoryUsageThresholdMb()
{
    return mHighMemoryUsageThresholdMb;
}

Int32 CWebViewCore::GetHighUsageDeltaMb()
{
    return mHighUsageDeltaMb;
}

//begin Fields callback
void CWebViewCore::SetNativeClass(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 nativeClass)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SetNativeClass(nativeClass);
}

Int32 CWebViewCore::GetNativeClass(
    /* [in] */ IInterface* obj)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->GetNativeClass();
}

void CWebViewCore::SetViewportWidth(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 viewportWidth)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SetViewportWidth(viewportWidth);
}

void CWebViewCore::SetViewportHeight(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 viewportHeight)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SetViewportHeight(viewportHeight);
}

void CWebViewCore::SetViewportInitialScale(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 viewportInitialScale)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SetViewportInitialScale(viewportInitialScale);
}

void CWebViewCore::SetViewportMinimumScale(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 viewportMinimumScale)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SetViewportMinimumScale(viewportMinimumScale);
}

void CWebViewCore::SetViewportMaximumScale(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 viewportMaximumScale)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SetViewportMaximumScale(viewportMaximumScale);
}

void CWebViewCore::SetViewportUserScalable(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean viewportUserScalable)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SetViewportUserScalable(viewportUserScalable);
}

void CWebViewCore::SetViewportDensityDpi(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 viewportDensityDpi)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SetViewportDensityDpi(viewportDensityDpi);
}

void CWebViewCore::SetDrawIsPaused(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean drawIsPaused)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SetDrawIsPaused(drawIsPaused);
}

Int32 CWebViewCore::GetLowMemoryUsageThresholdMb(
    /* [in] */ IInterface* obj)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->GetLowMemoryUsageThresholdMb();
}

Int32 CWebViewCore::GetHighMemoryUsageThresholdMb(
    /* [in] */ IInterface* obj)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->GetHighMemoryUsageThresholdMb();
}

Int32 CWebViewCore::GetHighUsageDeltaMb(
    /* [in] */ IInterface* obj)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->GetHighUsageDeltaMb();
}
//end Fields callback

void CWebViewCore::SendPluginDrawMsg(
    /* [in] */ IInterface* obj)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SendPluginDrawMsg();
}
//begin Methods callback
void CWebViewCore::ContentScrollTo(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Boolean animate,
    /* [in] */ Boolean onlyIfImeIsShowing)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->ContentScrollTo(x, y, animate, onlyIfImeIsShowing);
}

void CWebViewCore::ContentDraw(
    /* [in] */ IInterface* obj)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->ContentDraw();
}

void CWebViewCore::RequestListBox(
    /* [in] */ IInterface* obj,
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ ArrayOf<Int32>* enabledArray,
    /* [in] */ ArrayOf<Int32>* selectedArray)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->RequestListBox(array, enabledArray, selectedArray);
}

String CWebViewCore::OpenFileChooser(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& acceptType,
    /* [in] */ const String& capture)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->OpenFileChooser(acceptType, capture);
}

void CWebViewCore::RequestSingleListBox(
    /* [in] */ IInterface* obj,
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ ArrayOf<Int32>* enabledArray,
    /* [in] */ Int32 selection)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->RequestListBox(array, enabledArray, selection);
}

void CWebViewCore::JsAlert(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ const String& message)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->JsAlert(url, message);
}

Boolean CWebViewCore::JsConfirm(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ const String& message)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->JsConfirm(url, message);
}

String CWebViewCore::JsPrompt(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ const String& defaultValue)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->JsPrompt(url, message, defaultValue);
}

Boolean CWebViewCore::JsUnload(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ const String& message)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->JsUnload(url, message);
}

Boolean CWebViewCore::JsInterrupt(
    /* [in] */ IInterface* obj)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->JsInterrupt();
}

AutoPtr<IInterface> CWebViewCore::GetWebView(
    /* [in] */ IInterface* obj)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->GetWebView();
}

void CWebViewCore::DidFirstLayout(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean standardLoad)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->DidFirstLayout(standardLoad);
}

void CWebViewCore::UpdateViewport(
    /* [in] */ IInterface* obj)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->UpdateViewport();
}

void CWebViewCore::SendNotifyProgressFinished(
    /* [in] */ IInterface* obj)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SendNotifyProgressFinished();
}

void CWebViewCore::SendViewInvalidate(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 right,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 bottom)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SendViewInvalidate(right, left, top, bottom);
}

void CWebViewCore::UpdateTextfield(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 ptr,
    /* [in] */ const String& text,
    /* [in] */ Int32 textGeneration)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->UpdateTextfield(ptr, text, textGeneration);
}

void CWebViewCore::UpdateTextSelection(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 pointer,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 textGeneration,
    /* [in] */ Int32 selectionPtr)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->UpdateTextSelection(pointer, start, end, textGeneration, selectionPtr);
}

void CWebViewCore::UpdateTextSizeAndScroll(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 pointer,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->UpdateTextSizeAndScroll(pointer, width, height, scrollX, scrollY);
}

void CWebViewCore::ClearTextEntry(
    /* [in] */ IInterface* obj)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->ClearTextEntry();
}

void CWebViewCore::RestoreScale(
    /* [in] */ IInterface* obj,
    /* [in] */ Float scale,
    /* [in] */ Float textWrapScale)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->RestoreScale(scale, textWrapScale);
}

void CWebViewCore::NeedTouchEvents(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean need)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->NeedTouchEvents(need);
}

void CWebViewCore::RequestKeyboard(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean showKeyboard)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->RequestKeyboard(showKeyboard);
}

void CWebViewCore::ExceededDatabaseQuota(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& url,
    /* [in] */ const String& databaseIdentifier,
    /* [in] */ Int64 quota,
    /* [in] */ Int64 estimatedDatabaseSize)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->ExceededDatabaseQuota(url, databaseIdentifier, quota, estimatedDatabaseSize);
}

void CWebViewCore::ReachedMaxAppCacheSize(
    /* [in] */ IInterface* obj,
    /* [in] */ Int64 requiredStorage,
    /* [in] */ Int64 maxSize)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->ReachedMaxAppCacheSize(requiredStorage, maxSize);
}

void CWebViewCore::PopulateVisitedLinks(
    /* [in] */ IInterface* obj)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->PopulateVisitedLinks();
}

void CWebViewCore::GeolocationPermissionsShowPrompt(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& origin)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->GeolocationPermissionsShowPrompt(origin);
}

void CWebViewCore::GeolocationPermissionsHidePrompt(
    /* [in] */ IInterface* obj)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->GeolocationPermissionsHidePrompt();
}

AutoPtr<IInterface> CWebViewCore::GetDeviceMotionService(
    /* [in] */ IInterface* obj,
    /* [out] */ Int32 callback)
{
    //VALIDATE_NOT_NULL(callback);
    if (callback) {
        struct ElaDeviceMotionClientImplCallback* dmciCallback
            = (ElaDeviceMotionClientImplCallback*)callback;
        dmciCallback->start = &DeviceMotionService::Start;
        dmciCallback->stop = &DeviceMotionService::Stop;
        dmciCallback->suspend = &DeviceMotionService::Suspend;
        dmciCallback->resume = &DeviceMotionService::Resume;
    }
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->GetDeviceMotionService();
}

AutoPtr<IInterface> CWebViewCore::GetDeviceOrientationService(
    /* [in] */ IInterface* obj,
    /* [out] */ Int32 callback)
{
    //VALIDATE_NOT_NULL(callback);
    if (callback) {
        struct ElaDeviceOrientationClientImplCallback* dociCallback
            = (ElaDeviceOrientationClientImplCallback*)callback;
        dociCallback->start = &DeviceOrientationService::Start;
        dociCallback->stop = &DeviceOrientationService::Stop;
        dociCallback->suspend = &DeviceOrientationService::Suspend;
        dociCallback->resume = &DeviceOrientationService::Resume;
    }
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->GetDeviceOrientationService();
}

void CWebViewCore::AddMessageToConsole(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& message,
    /* [in] */ Int32 lineNumber,
    /* [in] */ const String& sourceID,
    /* [in] */ Int32 msgLevel)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->AddMessageToConsole(message, lineNumber, sourceID, msgLevel);
}

void CWebViewCore::FocusNodeChanged(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 nodePointer,
    /* [in] */ IInterface* hitTest)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->FocusNodeChanged(nodePointer, hitTest);
}

AutoPtr<IInterface> CWebViewCore::GetPluginClass(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& libName,
    /* [in] */ const String& clsName)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->GetPluginClass(libName, clsName);
}

void CWebViewCore::ShowFullScreenPlugin(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* childView,
    /* [in] */ Int32 orientation,
    /* [in] */ Int32 npp)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->ShowFullScreenPlugin(childView, orientation, npp);
}

void CWebViewCore::HideFullScreenPlugin(
    /* [in] */ IInterface* obj)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->HideFullScreenPlugin();
}

AutoPtr<IInterface> CWebViewCore::CreateSurface(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* vpluginView)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->CreateSurface(vpluginView);
}

AutoPtr<IInterface> CWebViewCore::AddSurface(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* pluginView,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->AddSurface(pluginView, x, y, width, height);
}

void CWebViewCore::UpdateSurface(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* vChildView,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->UpdateSurface(vChildView, x, y, width, height);
}

void CWebViewCore::DestroySurface(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* vChildView)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->DestroySurface(vChildView);
}

AutoPtr<IInterface> CWebViewCore::GetContext(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean isApplicationContext)
{
    if (!isApplicationContext)
    {
        return ((CWebViewCore*)IWebViewCore::Probe(obj))->GetContext();
    }
    else
    {
        AutoPtr<IContext> result;
        ((CWebViewCore*)IWebViewCore::Probe(obj))->GetContext()->GetApplicationContext((IContext**)&result);
        if (DebugFlags::WEB_VIEW_CORE) {
            Logger::W(LOGTAG, "application context is: %p", result.Get());
        }
        return result;
    }
}

void CWebViewCore::KeepScreenOn(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean screenOn)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->KeepScreenOn(screenOn);
}

void CWebViewCore::ShowRect(
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
    /* [in] */ Float yPercentInView)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->ShowRect(left, top, width, height, contentWidth, contentHeight,
                                                xPercentInDoc, xPercentInView, yPercentInDoc, yPercentInView);
}

void CWebViewCore::CenterFitRect(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->CenterFitRect(x, y, width, height);
}

void CWebViewCore::SetScrollbarModes(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 hMode,
    /* [in] */ Int32 vMode)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SetScrollbarModes(hMode, vMode);
}

void CWebViewCore::ExitFullscreenVideo(
    /* [in] */ IInterface* obj)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->ExitFullscreenVideo();
}

void CWebViewCore::SetWebTextViewAutoFillable(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 queryId,
    /* [in] */ const String& preview)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SetWebTextViewAutoFillable(queryId, preview);
}

void CWebViewCore::SelectAt(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->SelectAt(x, y);
}

void CWebViewCore::InitEditField(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 selectionPtr,
    /* [in] */ IInterface* initData)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->InitEditField(start, end, selectionPtr,
                                            (CWebViewCoreTextFieldInitData*)initData);
}

Boolean CWebViewCore::ChromeCanTakeFocus(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 webkitDirection)
{
    return ((CWebViewCore*)IWebViewCore::Probe(obj))->ChromeCanTakeFocus(webkitDirection);
}

void CWebViewCore::ChromeTakeFocus(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 webkitDirection)
{
    ((CWebViewCore*)IWebViewCore::Probe(obj))->ChromeTakeFocus(webkitDirection);
}
//end Methods callback

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
