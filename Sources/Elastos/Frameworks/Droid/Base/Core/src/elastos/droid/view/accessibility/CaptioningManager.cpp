
#include <Elastos.CoreLibrary.Utility.h>
#include "Elastos.Droid.Accounts.h"
#include "Elastos.Droid.App.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Database.h"
#include "Elastos.Droid.Location.h"
#include "Elastos.Droid.Net.h"
#include "Elastos.Droid.Provider.h"
#include "Elastos.Droid.Widget.h"
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/view/accessibility/CaptioningManager.h"
#include "elastos/droid/view/accessibility/CCaptioningManagerCaptionStyle.h"
#include "elastos/droid/graphics/Typeface.h"
#include "elastos/droid/os/CHandler.h"
// #include "elastos/droid/provider/Settings.h"
#include "elastos/droid/text/TextUtils.h"
#include <elastos/core/AutoLock.h>
#include <elastos/core/StringUtils.h>

using Elastos::Droid::Graphics::IColor;
using Elastos::Droid::Graphics::Typeface;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Provider::ISettingsSecure;
// using Elastos::Droid::Provider::Settings;
using Elastos::Droid::Text::TextUtils;
using Elastos::Core::StringUtils;
using Elastos::Utility::CArrayList;
using Elastos::Utility::CLocale;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

const Int32 CaptioningManager::DEFAULT_ENABLED = 0;
const Int32 CaptioningManager::DEFAULT_PRESET = 0;
const Float CaptioningManager::DEFAULT_FONT_SCALE = 1;

const Int32 CaptioningManager::CaptionStyle::COLOR_NONE_OPAQUE = 0x000000FF;
const Int32 CaptioningManager::CaptionStyle::COLOR_UNSPECIFIED = 0x000001FF;

AutoPtr<ICaptioningManagerCaptionStyle> CaptioningManager::CaptionStyle::InitSTYLE(
    /* [in] */ Int32 value1,
    /* [in] */ Int32 value2,
    /* [in] */ Int32 value3,
    /* [in] */ Int32 value4,
    /* [in] */ Int32 value5,
    /* [in] */ const String& value)
{
    AutoPtr<CCaptioningManagerCaptionStyle> style;
    CCaptioningManagerCaptionStyle::NewByFriend(value1, value2, value3, value4, value5, value,
            (CCaptioningManagerCaptionStyle**)&style);
    return (ICaptioningManagerCaptionStyle*)style.Get();
}

const AutoPtr<ICaptioningManagerCaptionStyle> CaptioningManager::CaptionStyle::WHITE_ON_BLACK = InitSTYLE(
        IColor::WHITE, IColor::BLACK, EDGE_TYPE_NONE, IColor::BLACK, COLOR_NONE_OPAQUE, String(NULL));

const AutoPtr<ICaptioningManagerCaptionStyle> CaptioningManager::CaptionStyle::BLACK_ON_WHITE = InitSTYLE(
        IColor::BLACK, IColor::WHITE, EDGE_TYPE_NONE, IColor::BLACK, COLOR_NONE_OPAQUE, String(NULL));

const AutoPtr<ICaptioningManagerCaptionStyle> CaptioningManager::CaptionStyle::YELLOW_ON_BLACK = InitSTYLE(
        IColor::YELLOW, IColor::BLACK, EDGE_TYPE_NONE, IColor::BLACK, COLOR_NONE_OPAQUE, String(NULL));

const AutoPtr<ICaptioningManagerCaptionStyle> CaptioningManager::CaptionStyle::YELLOW_ON_BLUE = InitSTYLE(
        IColor::YELLOW, IColor::BLUE, EDGE_TYPE_NONE, IColor::BLACK, COLOR_NONE_OPAQUE, String(NULL));

const AutoPtr<ICaptioningManagerCaptionStyle> CaptioningManager::CaptionStyle::UNSPECIFIED = InitSTYLE(
        COLOR_UNSPECIFIED, COLOR_UNSPECIFIED, EDGE_TYPE_UNSPECIFIED, COLOR_UNSPECIFIED, COLOR_UNSPECIFIED, String(NULL));

AutoPtr< ArrayOf<ICaptioningManagerCaptionStyle*> > CaptioningManager::CaptionStyle::InitPRESETS()
{
    AutoPtr< ArrayOf<ICaptioningManagerCaptionStyle*> > args = ArrayOf<ICaptioningManagerCaptionStyle*>::Alloc(5);
    (*args)[0] = CaptioningManager::CaptionStyle::WHITE_ON_BLACK;
    (*args)[1] = CaptioningManager::CaptionStyle::BLACK_ON_WHITE;
    (*args)[2] = CaptioningManager::CaptionStyle::YELLOW_ON_BLACK;
    (*args)[3] = CaptioningManager::CaptionStyle::YELLOW_ON_BLUE;
    (*args)[4] = CaptioningManager::CaptionStyle::UNSPECIFIED;

    return args;
}

const AutoPtr< ArrayOf<ICaptioningManagerCaptionStyle*> > CaptioningManager::CaptionStyle::PRESETS = InitPRESETS();

const AutoPtr<ICaptioningManagerCaptionStyle> CaptioningManager::CaptionStyle::DEFAULT_CUSTOM = WHITE_ON_BLACK;

const AutoPtr<ICaptioningManagerCaptionStyle> CaptioningManager::CaptionStyle::DEFAULT = WHITE_ON_BLACK;

/* CaptioningManager::CaptionStyle */
CAR_INTERFACE_IMPL(CaptioningManager::CaptionStyle, Object, ICaptioningManagerCaptionStyle)

CaptioningManager::CaptionStyle::CaptionStyle()
    : mForegroundColor(0)
    , mBackgroundColor(0)
    , mEdgeType(0)
    , mEdgeColor(0)
    , mWindowColor(0)
    , mRawTypeface(0)
    , mHasForegroundColor(FALSE)
    , mHasBackgroundColor(FALSE)
    , mHasEdgeType(FALSE)
    , mHasEdgeColor(FALSE)
    , mHasWindowColor(FALSE)
{}

CaptioningManager::CaptionStyle::~CaptionStyle()
{}

ECode CaptioningManager::CaptionStyle::constructor(
    /* [in] */ Int32 foregroundColor,
    /* [in] */ Int32 backgroundColor,
    /* [in] */ Int32 edgeType,
    /* [in] */ Int32 edgeColor,
    /* [in] */ Int32 windowColor,
    /* [in] */ const String& rawTypeface)
{
    mHasForegroundColor = foregroundColor != COLOR_UNSPECIFIED;
    mHasBackgroundColor = backgroundColor != COLOR_UNSPECIFIED;
    mHasEdgeType = edgeType != EDGE_TYPE_UNSPECIFIED;
    mHasEdgeColor = edgeColor != COLOR_UNSPECIFIED;
    mHasWindowColor = windowColor != COLOR_UNSPECIFIED;

    // Always use valid colors, even when no override is specified, to
    // ensure backwards compatibility with apps targeting KitKat MR2.
    mForegroundColor = mHasForegroundColor ? foregroundColor : IColor::WHITE;
    mBackgroundColor = mHasBackgroundColor ? backgroundColor : IColor::BLACK;
    mEdgeType = mHasEdgeType ? edgeType : EDGE_TYPE_NONE;
    mEdgeColor = mHasEdgeColor ? edgeColor : IColor::BLACK;
    mWindowColor = mHasWindowColor ? windowColor : COLOR_NONE_OPAQUE;

    mRawTypeface = rawTypeface;
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::GetForegroundColor(
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    *color = mForegroundColor;
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::GetBackgroundColor(
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    *color = mBackgroundColor;
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::GetEdgeType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mEdgeType;
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::GetEdgeColor(
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    *color = mEdgeColor;
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::GetWindowColor(
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    *color = mWindowColor;
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::GetRawTypeface(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mRawTypeface;
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::ApplyStyle(
    /* [in] */ ICaptioningManagerCaptionStyle* overlay,
    /* [out] */ ICaptioningManagerCaptionStyle** style)
{
    VALIDATE_NOT_NULL(style);

    Int32 newForegroundColor = mForegroundColor;
    Boolean res;
    if (overlay->HasForegroundColor(&res), res) {
        overlay->GetForegroundColor(&newForegroundColor);
    }

    Int32 newBackgroundColor = mBackgroundColor;
    if (overlay->HasBackgroundColor(&res), res) {
        overlay->GetBackgroundColor(&newBackgroundColor);
    }

    Int32 newEdgeType = mEdgeType;
    if (overlay->HasEdgeType(&res), res) {
        overlay->GetEdgeType(&newEdgeType);
    }

    Int32 newEdgeColor = mEdgeColor;
    if (overlay->HasEdgeColor(&res), res) {
        overlay->GetEdgeColor(&newEdgeColor);
    }

    Int32 newWindowColor = mWindowColor;
    if (overlay->HasWindowColor(&res), res) {
        overlay->GetWindowColor(&newWindowColor);
    }

    String newRawTypeface = mRawTypeface;
    String str;
    overlay->GetRawTypeface(&str);
    if (!str.IsNull()) {
        newRawTypeface = str;
    }
    AutoPtr<CCaptioningManagerCaptionStyle> styleObj;
    CCaptioningManagerCaptionStyle::NewByFriend(newForegroundColor, newBackgroundColor,
            newEdgeType, newEdgeColor, newWindowColor, newRawTypeface,
            (CCaptioningManagerCaptionStyle**)&styleObj);

    *style = (ICaptioningManagerCaptionStyle*)styleObj.Get();
    REFCOUNT_ADD(*style);
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::HasBackgroundColor(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHasBackgroundColor;
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::HasForegroundColor(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHasForegroundColor;
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::HasEdgeType(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHasEdgeType;
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::HasEdgeColor(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHasEdgeColor;
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::HasWindowColor(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHasWindowColor;
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::GetTypeface(
    /* [out] */ ITypeface** face)
{
    VALIDATE_NOT_NULL(face);
    if (mParsedTypeface == NULL && !TextUtils::IsEmpty(mRawTypeface)) {
        Typeface::Create(mRawTypeface, ITypeface::NORMAL, (ITypeface**)&mParsedTypeface);
    }
    *face = mParsedTypeface;
    REFCOUNT_ADD(*face);
    return NOERROR;
}

ECode CaptioningManager::CaptionStyle::GetCustomStyle(
    /* [in] */ IContentResolver* cr,
    /* [out] */ ICaptioningManagerCaptionStyle** style)
{
    VALIDATE_NOT_NULL(style);
    assert(0 && "TODO");

    // AutoPtr<ICaptioningManagerCaptionStyle> defStyle = DEFAULT_CUSTOM;
    // Int32 foregroundColor, result;
    // Settings::Secure::GetInt32(cr,
    //         ISettingsSecure::ACCESSIBILITY_CAPTIONING_FOREGROUND_COLOR,
    //         (defStyle->GetForegroundColor(&result), result), &foregroundColor);
    // Int32 backgroundColor;
    // Settings::Secure::GetInt32(cr,
    //         ISettingsSecure::ACCESSIBILITY_CAPTIONING_BACKGROUND_COLOR,
    //         (defStyle->GetBackgroundColor(&result), result), &backgroundColor);
    // Int32 edgeType;
    // Settings::Secure::GetInt32(cr,
    //         ISettingsSecure::ACCESSIBILITY_CAPTIONING_EDGE_TYPE,
    //         (defStyle->GetEdgeType(&result), result), &edgeType);
    // Int32 edgeColor;
    // Settings::Secure::GetInt32(cr,
    //         ISettingsSecure::ACCESSIBILITY_CAPTIONING_EDGE_COLOR,
    //         (defStyle->GetEdgeColor(&result), result), &edgeColor);
    // Int32 windowColor;
    // Settings::Secure::GetInt32(cr,
    //         ISettingsSecure::ACCESSIBILITY_CAPTIONING_WINDOW_COLOR,
    //         (defStyle->GetWindowColor(&result), result), &windowColor);

    // String rawTypeface;
    // Settings::Secure::GetString(cr, ISettingsSecure::ACCESSIBILITY_CAPTIONING_TYPEFACE, &rawTypeface);
    // if (rawTypeface.IsNull()) {
    //     defStyle->GetRawTypeface(&rawTypeface);
    // }

    // AutoPtr<CCaptioningManagerCaptionStyle> styleObj;
    // CCaptioningManagerCaptionStyle::NewByFriend(foregroundColor, backgroundColor,
    //         edgeType, edgeColor, windowColor, rawTypeface,
    //         (CCaptioningManagerCaptionStyle**)&styleObj);

    // *style = (ICaptioningManagerCaptionStyle*)styleObj.Get();
    // REFCOUNT_ADD(*style);
    return NOERROR;
}

/* CaptioningManager::CaptioningChangeListener */
CAR_INTERFACE_IMPL(CaptioningManager::CaptioningChangeListener, Object, ICaptioningManagerCaptioningChangeListener)

CaptioningManager::CaptioningChangeListener::OnEnabledChanged(
    /* [in] */ Boolean enabled)
{
    return NOERROR;
}

CaptioningManager::CaptioningChangeListener::OnUserStyleChanged(
    /* [in] */ ICaptioningManagerCaptionStyle* userStyle)
{
    return NOERROR;
}

CaptioningManager::CaptioningChangeListener::OnLocaleChanged(
    /* [in] */ ILocale* locale)
{
    return NOERROR;
}

CaptioningManager::CaptioningChangeListener::OnFontScaleChanged(
    /* [in] */ Float fontScale)
{
    return NOERROR;
}

/* CaptioningManager::MyContentObserver */
CaptioningManager::MyContentObserver::MyContentObserver(
    /* [in ] */ CaptioningManager* host)
    : mHost(host)
{}

CaptioningManager::MyContentObserver::~MyContentObserver()
{}

CaptioningManager::MyContentObserver::OnChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    String uriPath;
    uri->GetPath(&uriPath);
    String name = uriPath.Substring(uriPath.LastIndexOf('/') + 1);
    if (ISettingsSecure::ACCESSIBILITY_CAPTIONING_ENABLED.Equals(name)) {
        mHost->NotifyEnabledChanged();
    }
    else if (ISettingsSecure::ACCESSIBILITY_CAPTIONING_LOCALE.Equals(name)) {
        mHost->NotifyLocaleChanged();
    }
    else if (ISettingsSecure::ACCESSIBILITY_CAPTIONING_FONT_SCALE.Equals(name)) {
        mHost->NotifyFontScaleChanged();
    }
    else {
        // We only need a single callback when multiple style properties
        // change in rapid succession.
        mHost->mHandler->RemoveCallbacks(mHost->mStyleChangedRunnable);
        Boolean res;
        mHost->mHandler->Post(mHost->mStyleChangedRunnable, &res);
    }
    return NOERROR;
}

/* CaptioningManager::MyRunnable */
CaptioningManager::MyRunnable::MyRunnable(
    /* [in ] */ CaptioningManager* host)
    : mHost(host)
{}

CaptioningManager::MyRunnable::~MyRunnable()
{}

ECode CaptioningManager::MyRunnable::Run()
{
    mHost->NotifyUserStyleChanged();
    return NOERROR;
}

/* CaptioningManager */
CAR_INTERFACE_IMPL(CaptioningManager, Object, ICaptioningManager)

CaptioningManager::CaptioningManager()
{}

CaptioningManager::~CaptioningManager()
{}

ECode CaptioningManager::constructor(
    /* [in] */ IContext* context)
{
    CArrayList::New((IArrayList**)&mListeners);
    CHandler::New((IHandler**)&mHandler);
    context->GetContentResolver((IContentResolver**)&mContentResolver);
    mContentObserver = new MyContentObserver(this);
    mStyleChangedRunnable = new MyRunnable(this);
    return NOERROR;
}

ECode CaptioningManager::IsEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0 && "TODO");
    // Int32 value;
    // Settings::Secure::GetInt32(mContentResolver,
    //         ISettingsSecure::ACCESSIBILITY_CAPTIONING_ENABLED, DEFAULT_ENABLED, &value);
    // *result = value == 1;
    return NOERROR;
}

ECode CaptioningManager::GetRawLocale(
    /* [out] */ String* locale)
{
    VALIDATE_NOT_NULL(locale);
    assert(0 && "TODO");
    // Settings::Secure::GetString(mContentResolver,
    //         ISettingsSecure::ACCESSIBILITY_CAPTIONING_LOCALE, locale);
    return NOERROR;
}

ECode CaptioningManager::GetLocale(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);

    String rawLocale;
    GetRawLocale(&rawLocale);
    if (!TextUtils::IsEmpty(rawLocale)) {
        AutoPtr< ArrayOf<String> > splitLocale;
        StringUtils::Split(rawLocale, String("_"), (ArrayOf<String>**)&splitLocale);
        switch (splitLocale->GetLength()) {
            case 3:
                return CLocale::New((*splitLocale)[0], (*splitLocale)[1], (*splitLocale)[2], locale);
            case 2:
                return CLocale::New((*splitLocale)[0], (*splitLocale)[1], locale);
            case 1:
                return CLocale::New((*splitLocale)[0], locale);
            default:
                break;
        }
    }

    *locale = NULL;
    return NOERROR;
}

ECode CaptioningManager::GetFontScale(
    /* [out] */ Float* scale)
{
    VALIDATE_NOT_NULL(scale);
    assert(0 && "TODO");
    // Settings::Secure::GetFloat(mContentResolver,
    //         ISettingsSecure::ACCESSIBILITY_CAPTIONING_FONT_SCALE, DEFAULT_FONT_SCALE, scale);
    return NOERROR;
}

ECode CaptioningManager::GetRawUserStyle(
    /* [out] */ Int32* style)
{
    VALIDATE_NOT_NULL(style);
    assert(0 && "TODO");
    // Settings::Secure::GetInt32(mContentResolver,
    //         ISettingsSecure::ACCESSIBILITY_CAPTIONING_PRESET, DEFAULT_PRESET, style);
    return NOERROR;
}

ECode CaptioningManager::GetUserStyle(
    /* [out] */ ICaptioningManagerCaptionStyle** style)
{
    VALIDATE_NOT_NULL(style);

    Int32 preset;
    GetRawUserStyle(&preset);
    if (preset == ICaptioningManagerCaptionStyle::PRESET_CUSTOM) {
        return CaptionStyle::GetCustomStyle(mContentResolver, style);
    }
    *style = (*(CaptionStyle::PRESETS))[preset];
    REFCOUNT_ADD(*style);
    return NOERROR;
}

ECode CaptioningManager::AddCaptioningChangeListener(
    /* [in] */ ICaptioningManagerCaptioningChangeListener* listener)
{
    synchronized(mListeners) {
        Boolean res;
        if (mListeners->IsEmpty(&res), res) {
            RegisterObserver(ISettingsSecure::ACCESSIBILITY_CAPTIONING_ENABLED);
            RegisterObserver(ISettingsSecure::ACCESSIBILITY_CAPTIONING_FOREGROUND_COLOR);
            RegisterObserver(ISettingsSecure::ACCESSIBILITY_CAPTIONING_BACKGROUND_COLOR);
            RegisterObserver(ISettingsSecure::ACCESSIBILITY_CAPTIONING_WINDOW_COLOR);
            RegisterObserver(ISettingsSecure::ACCESSIBILITY_CAPTIONING_EDGE_TYPE);
            RegisterObserver(ISettingsSecure::ACCESSIBILITY_CAPTIONING_EDGE_COLOR);
            RegisterObserver(ISettingsSecure::ACCESSIBILITY_CAPTIONING_TYPEFACE);
            RegisterObserver(ISettingsSecure::ACCESSIBILITY_CAPTIONING_FONT_SCALE);
            RegisterObserver(ISettingsSecure::ACCESSIBILITY_CAPTIONING_LOCALE);
            RegisterObserver(ISettingsSecure::ACCESSIBILITY_CAPTIONING_PRESET);
        }

        mListeners->Add(listener);
    }
    return NOERROR;
}

void CaptioningManager::RegisterObserver(
    /* [in] */ const String& key)
{
    assert(0 && "TODO");
    // AutoPtr<IUri> uri;
    // Settings::Secure::GetUriFor(key, (IUri**)&uri);
    // mContentResolver->RegisterContentObserver(uri, FALSE, mContentObserver);
}

ECode CaptioningManager::RemoveCaptioningChangeListener(
    /* [in] */ ICaptioningManagerCaptioningChangeListener* listener)
{
    synchronized(mListeners) {
        mListeners->Remove(listener);

        Boolean res;
        if (mListeners->IsEmpty(&res), res) {
            mContentResolver->UnregisterContentObserver(mContentObserver);
        }
    }
    return NOERROR;
}

void CaptioningManager::NotifyEnabledChanged()
{
    Boolean enabled;
    IsEnabled(&enabled);
    synchronized(mListeners) {
        Int32 size;
        mListeners->GetSize(&size);
        for(Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> obj;
            mListeners->Get(i, (IInterface**)&obj);
            AutoPtr<ICaptioningManagerCaptioningChangeListener> listener =
                    ICaptioningManagerCaptioningChangeListener::Probe(obj);
            listener->OnEnabledChanged(enabled);
        }
    }
}

void CaptioningManager::NotifyUserStyleChanged()
{
    AutoPtr<ICaptioningManagerCaptionStyle> userStyle;
    GetUserStyle((ICaptioningManagerCaptionStyle**)&userStyle);
    synchronized(mListeners) {
        Int32 size;
        mListeners->GetSize(&size);
        for(Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> obj;
            mListeners->Get(i, (IInterface**)&obj);
            AutoPtr<ICaptioningManagerCaptioningChangeListener> listener =
                    ICaptioningManagerCaptioningChangeListener::Probe(obj);
            listener->OnUserStyleChanged(userStyle);
        }
    }
}

void CaptioningManager::NotifyLocaleChanged()
{
    AutoPtr<ILocale> locale;
    GetLocale((ILocale**)&locale);
    synchronized(mListeners) {
        Int32 size;
        mListeners->GetSize(&size);
        for(Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> obj;
            mListeners->Get(i, (IInterface**)&obj);
            AutoPtr<ICaptioningManagerCaptioningChangeListener> listener =
                    ICaptioningManagerCaptioningChangeListener::Probe(obj);
            listener->OnLocaleChanged(locale);
        }
    }
}

void CaptioningManager::NotifyFontScaleChanged()
{
    Float fontScale;
    GetFontScale(&fontScale);
    synchronized(mListeners) {
        Int32 size;
        mListeners->GetSize(&size);
        for(Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> obj;
            mListeners->Get(i, (IInterface**)&obj);
            AutoPtr<ICaptioningManagerCaptioningChangeListener> listener =
                    ICaptioningManagerCaptioningChangeListener::Probe(obj);
            listener->OnFontScaleChanged(fontScale);
        }
    }
}

} // Accessibility
} // View
} // Droid
} // Elastos
