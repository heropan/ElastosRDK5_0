
#ifndef __ELASTOS_DROID_TEXT_METHOD_TEXTKEYLISTENER_H__
#define __ELASTOS_DROID_TEXT_METHOD_TEXTKEYLISTENER_H__

#include "elastos/droid/text/method/BaseKeyListener.h"
#include "elastos/droid/database/ContentObserver.h"
//#include "Elastos.Droid.Core_server.h"

using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Database::ContentObserver;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

class TextKeyListener
    : public BaseKeyListener
{
private:
    class NullKeyListener
        : public ElRefBase
        , public IKeyListener
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* object,
            /* [out] */ InterfaceID* IID);

        CARAPI GetInputType(
            /* [out] */ Int32* inputType);

        CARAPI OnKeyDown(
            /* [in] */ IView* view,
            /* [in] */ IEditable* content,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* res);

        CARAPI OnKeyUp(
            /* [in] */ IView* view,
            /* [in] */ IEditable* content,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* res);

        CARAPI OnKeyOther(
            /* [in] */ IView* view,
            /* [in] */ IEditable* content,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* res);

        CARAPI ClearMetaKeyState(
            /* [in] */ IView* view,
            /* [in] */ IEditable* content,
            /* [in] */ Int32 states);

        static AutoPtr<NullKeyListener> GetInstance();

    private:
        static AutoPtr<NullKeyListener> sInstance;
    };

    class SettingsObserver : public ContentObserver
    {
    public:
        SettingsObserver(
            /* [in] */ TextKeyListener* host);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        TextKeyListener* mHost;
    };

public:
    friend class SettingsObserver;

    TextKeyListener();

    TextKeyListener(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext);

    static CARAPI_(Boolean) ShouldCap(
        /* [in] */ Capitalize cap,
        /* [in] */ ICharSequence* cs,
        /* [in] */ Int32 off);

    CARAPI_(Int32) GetInputType();

    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnKeyOther(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ IKeyEvent* event);

    static CARAPI_(void) Clear(
        /* [in] */ IEditable* e);

    CARAPI OnSpanAdded(
        /* [in] */ ISpannable* s,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI OnSpanRemoved(
        /* [in] */ ISpannable* s,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI OnSpanChanged(
        /* [in] */ ISpannable* s,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 st,
        /* [in] */ Int32 en);

    CARAPI ReleaseListener();

    CARAPI_(Int32) GetPrefs(
        /* [in] */ IContext* context);

protected:
    CARAPI Init(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext);

private:
    CARAPI_(AutoPtr<IKeyListener>) GetKeyListener(
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) InitPrefs(
        /* [in] */ IContext* context);

    CARAPI_(void) UpdatePrefs(
        /* [in] */ IContentResolver* resolver);

public:
    static const AutoPtr<IInterface> ACTIVE;
    static const AutoPtr<IInterface> CAPPED;
    static const AutoPtr<IInterface> INHIBIT_REPLACEMENT;
    static const AutoPtr<IInterface> LAST_TYPED;

public:
    static const Int32 AUTO_CAP;// = 1;
    static const Int32 AUTO_TEXT;// = 2;
    static const Int32 AUTO_PERIOD;// = 4;
    static const Int32 SHOW_PASSWORD;// = 8;

protected://private:
    static const Int32 CAPITALIZELENGTH = 4;
    //static AutoPtr<ITextKeyListener> sInstance[CAPITALIZELENGTH * 2];

protected:
    Object mLock;
private:
    Capitalize mAutoCap;
    Boolean mAutoText;

    Int32 mPrefs;
    Boolean mPrefsInited;

    //WeakReference<ContentResolver> mResolver;
    AutoPtr<IWeakReference> mResolver;
    AutoPtr<SettingsObserver> mObserver;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_METHOD_TEXTKEYLISTENER_H__
