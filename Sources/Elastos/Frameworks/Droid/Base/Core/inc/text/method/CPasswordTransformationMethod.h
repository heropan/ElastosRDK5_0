
#ifndef __CPASSWORDTRANSFORMATIONMETHOD_H__
#define __CPASSWORDTRANSFORMATIONMETHOD_H__

#include "_CPasswordTransformationMethod.h"
#include "os/HandlerMacro.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Text::IGetChars;
using Elastos::Droid::Text::Style::IUpdateLayout;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CPasswordTransformationMethod)
{
private:
    //static
    class PasswordCharSequence
        : public ElRefBase
        , public IGetChars
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* object,
            /* [in] */ InterfaceID* iid);

    public:
        CARAPI GetLength(
            /* [out] */ Int32* number);

        CARAPI GetCharAt(
            /* [in] */ Int32 i,
            /* [out] */ Char32* c);

        CARAPI SubSequence(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ICharSequence** csq);

        CARAPI ToString(
            /* [out] */ String* str);

        CARAPI GetChars(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ ArrayOf<Char32>* dest,
            /* [in] */ Int32 off);

    public:
        PasswordCharSequence(
            /* [in] */ ICharSequence* source);

    private:
        AutoPtr<ICharSequence> mSource;
    };

    //static
    class Visible
        : public IPasswordTransformationMethodVisible
        , public ElRefBase
        , public Handler
        , public IHandler
        , public IUpdateLayout/*extends IUpdateAppearance*/
        , public IRunnable
    {
        friend class PasswordCharSequence;

    public:
        Visible(
            /* [in] */ ISpannable* sp,
            /* [in] */ IPasswordTransformationMethod* ptm);

        CAR_INTERFACE_DECL()

        IHANDLER_METHODS_DECL()

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI Run();

    private:
        AutoPtr<ISpannable> mText;
        AutoPtr<IPasswordTransformationMethod> mTransformer;
    };

    /**
     * Used to stash a reference back to the View in the Editable so we
     * can use it to check the settings.
     */
    //private static
    //class ViewReference extends WeakReference<View> implements NoCopySpan
    //{//public
    //    ViewReference(/* [in] */ IView* v);
    //};

public:
    CARAPI GetTransformation(
        /* [in] */ ICharSequence* source,
        /* [in] */ IView* view,
        /* [out] */ ICharSequence** csq);

    static CARAPI_(AutoPtr<IPasswordTransformationMethod>) GetInstance();

    CARAPI BeforeTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ Int32 after);

    CARAPI AfterTextChanged(
        /* [in] */ IEditable* s);

    CARAPI OnTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 count);

    CARAPI OnFocusChanged(
        /* [in] */ IView* view,
        /* [in] */ ICharSequence* sourceText,
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

private:
    static CARAPI_(void) RemoveVisibleSpans(
        /* [in] */ ISpannable* sp);

private:
    static AutoPtr<IPasswordTransformationMethod> sInstance;
    static Char32 DOT;// = '\u2022';
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CPASSWORDTRANSFORMATIONMETHOD_H__
