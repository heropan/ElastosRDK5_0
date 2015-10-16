
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_PASSWORDENTRYKEYBOARD_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_PASSWORDENTRYKEYBOARD_H__

#include "elastos/droid/inputmethodservice/Keyboard.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Droid::InputMethodService::Keyboard;

/**
 * A basic, embed-able keyboard designed for password entry. Allows entry of all Latin-1 characters.
 *
 * It has two modes: alpha and numeric. In alpha mode, it allows all Latin-1 characters and enables
 * an additional keyboard with symbols.  In numeric mode, it shows a 12-key DTMF dialer-like
 * keypad with alpha characters hints.
 */
namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

extern "C" const InterfaceID EIID_LatinKey;

class PasswordEntryKeyboard : public Keyboard
{
public:
    PasswordEntryKeyboard(
        /* [in] */ IContext* context,
        /* [in] */ Int32 xmlLayoutResId);

    PasswordEntryKeyboard(
        /* [in] */ IContext* context,
        /* [in] */ Int32 xmlLayoutResId,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    PasswordEntryKeyboard(
        /* [in] */ IContext* context,
        /* [in] */ Int32 xmlLayoutResId,
        /* [in] */ Int32 mode);

    PasswordEntryKeyboard(
        /* [in] */ IContext* context,
        /* [in] */ Int32 xmlLayoutResId,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    PasswordEntryKeyboard(
        /* [in] */ IContext* context,
        /* [in] */ Int32 layoutTemplateResId,
        /* [in] */ ICharSequence* characters,
        /* [in] */ Int32 columns,
        /* [in] */ Int32 horizontalPadding);

    /**
     * Allows enter key resources to be overridden
     * @param res resources to grab given items from
     * @param previewId preview drawable shown on enter key
     * @param iconId normal drawable shown on enter key
     * @param labelId string shown on enter key
     */
    CARAPI_(void) SetEnterKeyResources(
        /* [in] */ IResources* res,
        /* [in] */ Int32 previewId,
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 labelId);

    /**
     * Allows shiftlock to be turned on.  See {@link #setShiftLocked(Boolean)}
     *
     */
    CARAPI_(void) EnableShiftLock();

    /**
     * Turn on shift lock. This turns on the LED for this key, if it has one.
     * It should be followed by a call to {@link KeyboardView#invalidateKey(Int32)}
     * or {@link KeyboardView#invalidateAllKeys()}
     *
     * @param shiftLocked
     */
    CARAPI_(void) SetShiftLocked(
        /* [in] */ Boolean shiftLocked);

    /**
     * Turn on shift mode. Sets shift mode and turns on icon for shift key.
     * It should be followed by a call to {@link KeyboardView#invalidateKey(Int32)}
     * or {@link KeyboardView#invalidateAllKeys()}
     *
     * @param shiftLocked
     */
    virtual CARAPI SetShifted(
        /* [in] */ Boolean shiftState,
        /* [out] */ Boolean* rst);

    /**
     * Whether or not keyboard is shifted.
     * @return TRUE if keyboard state is shifted.
     */
    virtual CARAPI IsShifted(
        /* [out] */ Boolean* rst);

    class LatinKey : public Key
    {
    public:
        LatinKey(
            /* [in] */ IResources* res,
            /* [in] */ Row* parent,
            /* [in] */ Int32 x,
            /* [in] */ Int32 y,
            /* [in] */ IXmlResourceParser* parser);

        virtual CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(void) SetEnabled(
            /* [in] */ Boolean enabled);

        CARAPI_(void) EnableShiftLock();

        virtual CARAPI OnReleased(
            /* [in] */ Boolean inside);

        /**
         * Overriding this method so that we can reduce the target area for certain keys.
         */
        virtual CARAPI IsInside(
            /* [in] */ Int32 x,
            /* [in] */ Int32 y,
            /* [out] */ Boolean* isInside);

    private:
        Boolean mShiftLockEnabled;
        Boolean mEnabled;
    };

protected:
    virtual CARAPI_(AutoPtr<Key>) CreateKeyFromXml(
        /* [in] */ IResources* res,
        /* [in] */ Row* parent,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ IXmlResourceParser* parser);

    PasswordEntryKeyboard();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 xmlLayoutResId);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 xmlLayoutResId,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 xmlLayoutResId,
        /* [in] */ Int32 mode);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 xmlLayoutResId,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 layoutTemplateResId,
        /* [in] */ ICharSequence* characters,
        /* [in] */ Int32 columns,
        /* [in] */ Int32 horizontalPadding);

private:
    CARAPI InternalInit(
        /* [in] */ IContext* context);

public:
    static const Int32 KEYCODE_SPACE;

    static Int32 sSpacebarVerticalCorrection;

private:
    static const Int32 SHIFT_OFF;
    static const Int32 SHIFT_ON;
    static const Int32 SHIFT_LOCKED;


    AutoPtr<IDrawable> mShiftIcon;
    AutoPtr<IDrawable> mShiftLockIcon;

    // These two arrays must be the same length
    AutoPtr<ArrayOf<IDrawable *> > mOldShiftIcons;// = { NULL, NULL };
    AutoPtr<ArrayOf<Key *> > mShiftKeys;// = { NULL, NULL };

    AutoPtr<Key> mEnterKey;
    AutoPtr<Key> mF1Key;
    AutoPtr<Key> mSpaceKey;
    Int32 mShiftState;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
