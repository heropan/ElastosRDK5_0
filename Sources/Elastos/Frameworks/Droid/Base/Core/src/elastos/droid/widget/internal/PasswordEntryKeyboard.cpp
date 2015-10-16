
#include "elastos/droid/internal/PasswordEntryKeyboard.h"

using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

//0c08c3b7-3cd7-4772-8340-d42f0613bd47
extern "C" const InterfaceID EIID_LatinKey =
        { 0x0c08c3b7, 0x3cd7, 0x4772, { 0x83, 0x40, 0xd4, 0x2f, 0x06, 0x13, 0xbd, 0x47 } };

Int32 const PasswordEntryKeyboard::KEYCODE_SPACE = ' ';
Int32 PasswordEntryKeyboard::sSpacebarVerticalCorrection;
Int32 const PasswordEntryKeyboard::SHIFT_OFF = 0;
Int32 const PasswordEntryKeyboard::SHIFT_ON = 1;
Int32 const PasswordEntryKeyboard::SHIFT_LOCKED = 2;

PasswordEntryKeyboard::PasswordEntryKeyboard()
    : mShiftState(SHIFT_OFF)
{}

PasswordEntryKeyboard::PasswordEntryKeyboard(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId)
    : Keyboard(context, xmlLayoutResId, 0)
    , mShiftState(SHIFT_OFF)
{
    mOldShiftIcons = ArrayOf<IDrawable*>::Alloc(2);
    mOldShiftIcons->Set(0, NULL);
    mOldShiftIcons->Set(1, NULL);

    mShiftKeys = ArrayOf<Key*>::Alloc(2);
    mShiftKeys->Set(0, NULL);
    mShiftKeys->Set(1, NULL);
    ASSERT_SUCCEEDED(InternalInit(context));
}

PasswordEntryKeyboard::PasswordEntryKeyboard(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : Keyboard(context, xmlLayoutResId, 0, width, height)
    , mShiftState(SHIFT_OFF)
{
    mOldShiftIcons = ArrayOf<IDrawable*>::Alloc(2);
    mOldShiftIcons->Set(0, NULL);
    mOldShiftIcons->Set(1, NULL);

    mShiftKeys = ArrayOf<Key*>::Alloc(2);
    mShiftKeys->Set(0, NULL);
    mShiftKeys->Set(1, NULL);
    ASSERT_SUCCEEDED(InternalInit(context));
}

PasswordEntryKeyboard::PasswordEntryKeyboard(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 mode)
    : Keyboard(context, xmlLayoutResId, mode)
    , mShiftState(SHIFT_OFF)
{
    mOldShiftIcons = ArrayOf<IDrawable*>::Alloc(2);
    mOldShiftIcons->Set(0, NULL);
    mOldShiftIcons->Set(1, NULL);

    mShiftKeys = ArrayOf<Key*>::Alloc(2);
    mShiftKeys->Set(0, NULL);
    mShiftKeys->Set(1, NULL);
    ASSERT_SUCCEEDED(InternalInit(context));
}

PasswordEntryKeyboard::PasswordEntryKeyboard(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : Keyboard(context, xmlLayoutResId, mode, width, height)
    , mShiftState(SHIFT_OFF)
{
    mOldShiftIcons = ArrayOf<IDrawable*>::Alloc(2);
    mOldShiftIcons->Set(0, NULL);
    mOldShiftIcons->Set(1, NULL);

    mShiftKeys = ArrayOf<Key*>::Alloc(2);
    mShiftKeys->Set(0, NULL);
    mShiftKeys->Set(1, NULL);
    ASSERT_SUCCEEDED(InternalInit(context));
}

ECode PasswordEntryKeyboard::InternalInit(
    /* [in] */ IContext* context)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetDrawable(R::drawable::sym_keyboard_shift, (IDrawable**)&mShiftIcon);
    res->GetDrawable(R::drawable::sym_keyboard_shift_locked, (IDrawable**)&mShiftLockIcon);
    res->GetDimensionPixelOffset(
            R::dimen::password_keyboard_spacebar_vertical_correction, &sSpacebarVerticalCorrection);
    return NOERROR;
}

PasswordEntryKeyboard::PasswordEntryKeyboard(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layoutTemplateResId,
    /* [in] */ ICharSequence* characters,
    /* [in] */ Int32 columns,
    /* [in] */ Int32 horizontalPadding)
    : Keyboard(context, layoutTemplateResId, characters, columns, horizontalPadding)
    , mShiftState(SHIFT_OFF)
{
    mOldShiftIcons = ArrayOf<IDrawable*>::Alloc(2);
    mOldShiftIcons->Set(0, NULL);
    mOldShiftIcons->Set(1, NULL);

    mShiftKeys = ArrayOf<Key*>::Alloc(2);
    mShiftKeys->Set(0, NULL);
    mShiftKeys->Set(1, NULL);
}

ECode PasswordEntryKeyboard::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId)
{
    ASSERT_SUCCEEDED(Keyboard::Init(context, xmlLayoutResId, 0));
    ASSERT_SUCCEEDED(InternalInit(context));
    return NOERROR;
}

ECode PasswordEntryKeyboard::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ASSERT_SUCCEEDED(Keyboard::Init(context, xmlLayoutResId, 0, width, height));
    ASSERT_SUCCEEDED(InternalInit(context));
    return NOERROR;
}

ECode PasswordEntryKeyboard::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 mode)
{
    ASSERT_SUCCEEDED(Keyboard::Init(context, xmlLayoutResId, mode));
    ASSERT_SUCCEEDED(InternalInit(context));
    return NOERROR;
}

ECode PasswordEntryKeyboard::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ASSERT_SUCCEEDED(Keyboard::Init(context, xmlLayoutResId, mode, width, height));
    ASSERT_SUCCEEDED(InternalInit(context));
    return NOERROR;
}

ECode PasswordEntryKeyboard::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layoutTemplateResId,
    /* [in] */ ICharSequence* characters,
    /* [in] */ Int32 columns,
    /* [in] */ Int32 horizontalPadding)
{
    ASSERT_SUCCEEDED(Keyboard::Init(context, layoutTemplateResId, characters, columns, horizontalPadding));
    return NOERROR;
}

AutoPtr<Keyboard::Key> PasswordEntryKeyboard::CreateKeyFromXml(
    /* [in] */ IResources* res,
    /* [in] */ Row* parent,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ IXmlResourceParser* parser)
{
    AutoPtr<LatinKey> key = new LatinKey(res, parent, x, y, parser);
    Int32 code = (*(key->mCodes))[0];
    if (code >= 0 && code != '\n' && (code < 32 || code > 127)) {
        // Log.w(TAG, "Key code for " + key.label + " is not latin-1");
        CStringWrapper::New(String(" "), (ICharSequence**)(&(key->mLabel)));
        key->SetEnabled(FALSE);
    }
    switch ((*(key->mCodes))[0]) {
        case 10:
            mEnterKey = key;
            break;
        case -103://PasswordEntryKeyboardView::KEYCODE_F1:
            mF1Key = key;
            break;
        case 32:
            mSpaceKey = key;
            break;
    }
    return key;
}

/**
 * Allows enter key resources to be overridden
 * @param res resources to grab given items from
 * @param previewId preview drawable shown on enter key
 * @param iconId normal drawable shown on enter key
 * @param labelId string shown on enter key
 */
void PasswordEntryKeyboard::SetEnterKeyResources(
    /* [in] */ IResources* res,
    /* [in] */ Int32 previewId,
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 labelId)
{
    if (mEnterKey != NULL) {
        // Reset some of the rarely used attributes.
        mEnterKey->mPopupCharacters = NULL;
        mEnterKey->mPopupResId = 0;
        mEnterKey->mText = NULL;

        mEnterKey->mIconPreview = NULL;
        mEnterKey->mIcon = NULL;
        mEnterKey->mLabel = NULL;
        res->GetDrawable(previewId, (IDrawable**)(&(mEnterKey->mIconPreview)));
        res->GetDrawable(iconId, (IDrawable**)(&(mEnterKey->mIcon)));
        res->GetText(labelId, (ICharSequence**)(&(mEnterKey->mLabel)));

        // Set the initial size of the preview icon
        if (mEnterKey->mIconPreview != NULL) {
            Int32 wd, ht;
            mEnterKey->mIconPreview->GetIntrinsicWidth(&wd);
            mEnterKey->mIconPreview->GetIntrinsicHeight(&ht);
            mEnterKey->mIconPreview->SetBounds(0, 0, wd, ht);
        }
    }
}

/**
 * Allows shiftlock to be turned on.  See {@link #setShiftLocked(Boolean)}
 *
 */
void PasswordEntryKeyboard::EnableShiftLock()
{
    Int32 i = 0;
    AutoPtr<ArrayOf<Int32> > indices;
    GetShiftKeyIndices((ArrayOf<Int32>**)&indices);
    for (Int32 j = 0; j < indices->GetLength(); j++) {
        Int32 index = (*indices)[j];
        if (index >= 0 && i < mShiftKeys->GetLength()) {
            List<AutoPtr<Key> > keys = GetKeys();
            mShiftKeys->Set(i, keys[index]);
            if ((*mShiftKeys)[i]->Probe(EIID_LatinKey)) {
                AutoPtr<LatinKey> latinKey = reinterpret_cast<LatinKey*>((*mShiftKeys)[i]->Probe(EIID_LatinKey));
                latinKey->EnableShiftLock();
            }
            mOldShiftIcons->Set(i, (*mShiftKeys)[i]->mIcon);
            i++;
        }
    }
}

/**
 * Turn on shift lock. This turns on the LED for this key, if it has one.
 * It should be followed by a call to {@link KeyboardView#invalidateKey(Int32)}
 * or {@link KeyboardView#invalidateAllKeys()}
 *
 * @param shiftLocked
 */
void PasswordEntryKeyboard::SetShiftLocked(
    /* [in] */ Boolean shiftLocked)
{
    for (Int32 i = 0; i < mShiftKeys->GetLength(); i++)
    {
        AutoPtr<Key> shiftKey = (*mShiftKeys)[i];

        if (shiftKey != NULL) {
            shiftKey->mOn = shiftLocked;
            shiftKey->mIcon = mShiftLockIcon;
        }
    }
    mShiftState = shiftLocked ? SHIFT_LOCKED : SHIFT_ON;
}

/**
 * Turn on shift mode. Sets shift mode and turns on icon for shift key.
 * It should be followed by a call to {@link KeyboardView#invalidateKey(Int32)}
 * or {@link KeyboardView#invalidateAllKeys()}
 *
 * @param shiftLocked
 */
ECode PasswordEntryKeyboard::SetShifted(
    /* [in] */ Boolean shiftState,
    /* [out] */ Boolean* rst)
{
    Boolean shiftChanged = FALSE;
    if (shiftState == FALSE) {
        shiftChanged = mShiftState != SHIFT_OFF;
        mShiftState = SHIFT_OFF;
    } else if (mShiftState == SHIFT_OFF) {
        shiftChanged = mShiftState == SHIFT_OFF;
        mShiftState = SHIFT_ON;
    }
    for (Int32 i = 0; i < mShiftKeys->GetLength(); i++) {
        if ((*mShiftKeys)[i] != NULL) {
            if (shiftState == FALSE) {
                (*mShiftKeys)[i]->mOn = FALSE;
                (*mShiftKeys)[i]->mIcon = (*mOldShiftIcons)[i];
            } else if (mShiftState == SHIFT_OFF) {
                (*mShiftKeys)[i]->mOn = FALSE;
                (*mShiftKeys)[i]->mIcon = mShiftIcon;
            }
        } else {
            Keyboard::SetShifted(shiftState, rst);
            return NOERROR;
        }
    }
    *rst = shiftChanged;
    return NOERROR;
}

/**
 * Whether or not keyboard is shifted.
 * @return TRUE if keyboard state is shifted.
 */
ECode PasswordEntryKeyboard::IsShifted(
    /* [out] */ Boolean* rst)
{
    if ((*mShiftKeys)[0] != NULL) {
        *rst = mShiftState != SHIFT_OFF;
    } else {
        return Keyboard::IsShifted(rst);
    }
    return NOERROR;
}


PasswordEntryKeyboard::LatinKey::LatinKey(
    /* [in] */ IResources* res,
    /* [in] */ Row* parent,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ IXmlResourceParser* parser)
    : Key(res, parent, x, y, parser)
    , mShiftLockEnabled(FALSE)
    , mEnabled(FALSE)
{
    Int32 length;
    if (mPopupCharacters != NULL && (mPopupCharacters->GetLength(&length), length) == 0) {
        // If there is a keyboard with no keys specified in popupCharacters
        mPopupResId = 0;
    }
}

PInterface PasswordEntryKeyboard::LatinKey::Probe(
    /* [in] */ REIID riid)
{
    if(riid == EIID_LatinKey) {
        return reinterpret_cast<PInterface>(this);
    } else{
        return Key::Probe(riid);
    }
}

void PasswordEntryKeyboard::LatinKey::SetEnabled(
    /* [in] */ Boolean enabled)
{
    mEnabled = enabled;
}

void PasswordEntryKeyboard::LatinKey::EnableShiftLock()
{
    mShiftLockEnabled = TRUE;
}

ECode PasswordEntryKeyboard::LatinKey::OnReleased(
    /* [in] */ Boolean inside)
{
    if (!mShiftLockEnabled) {
        return Key::OnReleased(inside);
    } else {
       mPressed = !mPressed;
    }
    return NOERROR;
}

/**
 * Overriding this method so that we can reduce the target area for certain keys.
 */
ECode PasswordEntryKeyboard::LatinKey::IsInside(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Boolean* isInside)
{
    if (!mEnabled) {
        return FALSE;
    }
    Int32 code = (*mCodes)[0];
    if (code == KEYCODE_SHIFT || code == KEYCODE_DELETE) {
        y -= mHeight / 10;
        if (code == KEYCODE_SHIFT) x += mWidth / 6;
        if (code == KEYCODE_DELETE) x -= mWidth / 6;
    } else if (code == KEYCODE_SPACE) {
        y += PasswordEntryKeyboard::sSpacebarVerticalCorrection;
    }
    return Key::IsInside(x, y, isInside);
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
