
#include "text/method/TextKeyListener.h"
#include "text/method/CQwertyKeyListener.h"
#include "text/method/CMultiTapKeyListener.h"
#include "text/SpannableStringInternal.h"
#include "text/Selection.h"
#include "text/TextUtils.h"
#include "provider/Settings.h"

using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Database::EIID_IContentObserver;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Provider::Settings;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::Content::EIID_IContentResolver;
using Elastos::Droid::View::IKeyCharacterMap;
using Elastos::Droid::Text::Method::CQwertyKeyListener;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

AutoPtr<TextKeyListener::NullKeyListener> TextKeyListener::NullKeyListener::sInstance;

PInterface TextKeyListener::NullKeyListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IKeyListener*)this;
    }
    else if (riid == EIID_IKeyListener) {
        return (IKeyListener*)this;
    }
    return NULL;
}

UInt32 TextKeyListener::NullKeyListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextKeyListener::NullKeyListener::Release()
{
    return ElRefBase::Release();
}

ECode TextKeyListener::NullKeyListener::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* IID)
{
    if (IID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(IKeyListener*)this) {
        *IID = EIID_IKeyListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode TextKeyListener::NullKeyListener::GetInputType(
    /* [out] */ Int32* inputType)
{
    VALIDATE_NOT_NULL(inputType);
    *inputType = IInputType::TYPE_NULL;

    return NOERROR;
}

ECode TextKeyListener::NullKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode TextKeyListener::NullKeyListener::OnKeyUp(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode TextKeyListener::NullKeyListener::OnKeyOther(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    PEL("TextKeyListener::NullKeyListener::OnKeyOther")
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode TextKeyListener::NullKeyListener::ClearMetaKeyState(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 states)
{
    return NOERROR;
}

AutoPtr<TextKeyListener::NullKeyListener> TextKeyListener::NullKeyListener::GetInstance()
{
    if (sInstance != NULL)
        return sInstance;

    sInstance = new NullKeyListener();
    return sInstance;
}

TextKeyListener::SettingsObserver::SettingsObserver(
    /* [in] */ TextKeyListener* host)
    : mHost(host)
{
}

//@Override
ECode TextKeyListener::SettingsObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    if (mHost->mResolver != NULL) {
        AutoPtr<IInterface> obj;
        mHost->mResolver->Resolve(EIID_IInterface, (IInterface**)&obj);
        if (obj == NULL) {
            mHost->mPrefsInited = FALSE;
        }
        else {
            AutoPtr<IContentResolver> resolver = (IContentResolver*)obj->Probe(EIID_IContentResolver);
            mHost->UpdatePrefs(resolver);
        }
    }
    else {
        mHost->mPrefsInited = FALSE;
    }
    return NOERROR;
}

const AutoPtr<IInterface> TextKeyListener::ACTIVE = MetaKeyKeyListener::NewNoCopySpan();
const AutoPtr<IInterface> TextKeyListener::CAPPED = MetaKeyKeyListener::NewNoCopySpan();
const AutoPtr<IInterface> TextKeyListener::INHIBIT_REPLACEMENT = MetaKeyKeyListener::NewNoCopySpan();
const AutoPtr<IInterface> TextKeyListener::LAST_TYPED = MetaKeyKeyListener::NewNoCopySpan();

const Int32 TextKeyListener::AUTO_CAP = 1;
const Int32 TextKeyListener::AUTO_TEXT = 2;
const Int32 TextKeyListener::AUTO_PERIOD = 4;
const Int32 TextKeyListener::SHOW_PASSWORD = 8;

const Int32 TextKeyListener::CAPITALIZELENGTH;// = 4;
//AutoPtr<ITextKeyListener> TextKeyListener::sInstance[TextKeyListener::CAPITALIZELENGTH * 2];

TextKeyListener::TextKeyListener()
{}

TextKeyListener::TextKeyListener(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext)
{
    Init(cap, autotext);
}

/**
 * Returns whether it makes sense to automatically capitalize at the
 * specified position in the specified text, with the specified rules.
 *
 * @param cap the capitalization rules to consider.
 * @param cs the text in which an insertion is being made.
 * @param off the offset into that text where the insertion is being made.
 *
 * @return whether the character being inserted should be capitalized.
 */
Boolean TextKeyListener::ShouldCap(
    /* [in] */ Capitalize cap,
    /* [in] */ ICharSequence* cs,
    /* [in] */ Int32 off)
{
    if (cap == Capitalize_NONE) {
        return FALSE;
    }
    if (cap == Capitalize_CHARACTERS) {
        return TRUE;
    }

    return TextUtils::GetCapsMode(cs, off, cap == Capitalize_WORDS
        ? ITextUtils::CAP_MODE_WORDS : ITextUtils::CAP_MODE_SENTENCES)
            != 0;
}

Int32 TextKeyListener::GetInputType()
{
    return MakeTextContentType(mAutoCap, mAutoText);
}

//@Override
Boolean TextKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IKeyListener> im = GetKeyListener(event);

    Boolean res;
    im->OnKeyDown(view, content, keyCode, event, &res);
    return res;
}

//@Override
Boolean TextKeyListener::OnKeyUp(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IKeyListener> im = GetKeyListener(event);

    Boolean res;
    im->OnKeyUp(view, content, keyCode, event, &res);
    return res;
}

//@Override
Boolean TextKeyListener::OnKeyOther(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IKeyListener> im = GetKeyListener(event);

    Boolean res;
    im->OnKeyOther(view, content, event, &res);
    return res;
}

/**
 * Clear all the input state (autotext, autocap, multitap, undo)
 * from the specified Editable, going beyond Editable.clear(), which
 * just clears the text but not the input state.
 *
 * @param e the buffer whose text and state are to be cleared.
 */
void TextKeyListener::Clear(
    /* [in] */ IEditable* e)
{
    e->Clear();
    e->RemoveSpan(ACTIVE);
    e->RemoveSpan(CAPPED);
    e->RemoveSpan(INHIBIT_REPLACEMENT);
    e->RemoveSpan(LAST_TYPED);

    Int32 len;
    AutoPtr< ArrayOf<IInterface*> > repl = NULL;
    e->GetSpans(0, (e->GetLength(&len), len), /*EIID_Replaced*/EIID_INoCopySpan, (ArrayOf<IInterface*>**)&repl);
    Int32 count = repl->GetLength();
    for (Int32 i = 0; i < count; i++) {
        e->RemoveSpan((*repl)[i]);
    }
}

ECode TextKeyListener::OnSpanAdded(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return NOERROR;
}

ECode TextKeyListener::OnSpanRemoved(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return NOERROR;
}

ECode TextKeyListener::OnSpanChanged(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 st,
    /* [in] */ Int32 en)
{
    if (what == Selection::SELECTION_END) {
        s->RemoveSpan(ACTIVE);
    }
    return NOERROR;
}

AutoPtr<IKeyListener> TextKeyListener::GetKeyListener(
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IKeyCharacterMap> kmap;
    event->GetKeyCharacterMap((IKeyCharacterMap**)&kmap);
    Int32 kind;
    kmap->GetKeyboardType(&kind);

    if (kind == IKeyCharacterMap::ALPHA) {
        AutoPtr<IQwertyKeyListener> qkl = CQwertyKeyListener::GetInstance(mAutoText, mAutoCap);
        AutoPtr<IKeyListener> kl = IKeyListener::Probe(qkl);
        return kl;
    }
    else if (kind == IKeyCharacterMap::NUMERIC) {
        AutoPtr<IMultiTapKeyListener> mtkl = CMultiTapKeyListener::GetInstance(mAutoText, mAutoCap);
        AutoPtr<IKeyListener> kl = IKeyListener::Probe(mtkl);
        return kl;
    }
    else if (kind == IKeyCharacterMap::FULL
        || kind == IKeyCharacterMap::SPECIAL_FUNCTION) {
        // We consider special function keyboards full keyboards as a workaround for
        // devices that do not have built-in keyboards.  Applications may try to inject
        // key events using the built-in keyboard device id which may be configured as
        // a special function keyboard using a default key map.  Ideally, as of Honeycomb,
        // these applications should be modified to use KeyCharacterMap.VIRTUAL_KEYBOARD.

        AutoPtr<IQwertyKeyListener> listener = CQwertyKeyListener::GetInstanceForFullKeyboard();
        return (IKeyListener*)listener->Probe(EIID_IKeyListener);
        //Java:    return QwertyKeyListener.getInstanceForFullKeyboard();
        /*
        AutoPtr<IQwertyKeyListener> qkl = CQwertyKeyListener::GetInstanceForFullKeyboard();
        AutoPtr<IKeyListener> kl = IKeyListener::Probe(qkl);
        return kl;
        */
    }

    return NullKeyListener::GetInstance();
}

ECode TextKeyListener::ReleaseListener()
{
    if (mResolver != NULL) {
        AutoPtr<IInterface> obj;
        mResolver->Resolve(EIID_IInterface, (IInterface**)&obj);
        if (obj != NULL) {
            AutoPtr<IContentResolver> resolver = (IContentResolver*)obj->Probe(EIID_IContentResolver);
            resolver->UnregisterContentObserver((IContentObserver*)(mObserver->Probe(EIID_IContentObserver)));
        }

        mObserver = NULL;
        mResolver = NULL;
        mPrefsInited = FALSE;
    }

    return NOERROR;
}

void TextKeyListener::InitPrefs(
    /* [in] */ IContext* context)
{
    mResolver = NULL;
    AutoPtr<IContentResolver> contentResolver;
    context->GetContentResolver((IContentResolver**)&contentResolver);
    if (contentResolver != NULL) {
        IWeakReferenceSource* wrs = (IWeakReferenceSource*)contentResolver->Probe(EIID_IWeakReferenceSource);
        assert(wrs != NULL);
        wrs->GetWeakReference((IWeakReference**)&mResolver);
    }

    if (mObserver == NULL) {
        mObserver = new SettingsObserver(this);
        contentResolver->RegisterContentObserver(Settings::System::CONTENT_URI, TRUE, (IContentObserver*)(mObserver->Probe(EIID_IContentObserver)));
    }

    UpdatePrefs(contentResolver);
    mPrefsInited = TRUE;
}

void TextKeyListener::UpdatePrefs(
    /* [in] */ IContentResolver* resolver)
{
    Int32 nHelp;
    Boolean cap;
    Int32 value;
    Settings::System::GetInt32(resolver, ISettingsSystem::TEXT_AUTO_CAPS, 1, &value);
    cap = value > 0;
    Boolean text;
    Settings::System::GetInt32(resolver, ISettingsSystem::TEXT_AUTO_REPLACE, 1, &value);
    text = value > 0;
    Boolean period;
    Settings::System::GetInt32(resolver, ISettingsSystem::TEXT_AUTO_PUNCTUATE, 1, &value);
    period = value > 0;
    Boolean pw;
    Settings::System::GetInt32(resolver, ISettingsSystem::TEXT_SHOW_PASSWORD, 1, &value);
    pw = value > 0;

    mPrefs = (cap ? AUTO_CAP : 0) |
             (text ? AUTO_TEXT : 0) |
             (period ? AUTO_PERIOD : 0) |
             (pw ? SHOW_PASSWORD : 0);
}

Int32 TextKeyListener::GetPrefs(
    /* [in] */ IContext* context)
{
    AutoLock lock(mLock);

    Boolean needInit = (mResolver == NULL);
    if (!needInit) {
        AutoPtr<IInterface> obj;
        mResolver->Resolve(EIID_IInterface, (IInterface**)&obj);
        needInit = (obj == NULL);
    }

    if (!mPrefsInited || needInit) {
        InitPrefs(context);
    }

    return mPrefs;
}

/**
 * Creates a new TextKeyListener with the specified capitalization
 * and correction properties.
 *
 * @param cap when, if ever, to automatically capitalize.
 * @param autotext whether to automatically do spelling corrections.
 */
ECode TextKeyListener::Init(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext)
{
    mAutoCap = cap;
    mAutoText = autotext;

    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
