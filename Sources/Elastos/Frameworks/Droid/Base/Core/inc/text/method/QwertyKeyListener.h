
#ifndef __QWERTYKEYLISTENER_H__
#define __QWERTYKEYLISTENER_H__

#include "text/method/BaseKeyListener.h"
#include "ext/frameworkext.h"
#include <elastos/HashMap.h>

extern "C" const InterfaceID EIID_Replaced;

using Elastos::Utility::HashMap;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

class QwertyKeyListener : public BaseKeyListener
{
protected:
    class Replaced : public ElRefBase, public INoCopySpan
    {
    public:
        Replaced(
            /* [in] */ ArrayOf<Char8>* text);

        ~Replaced();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [out] */ InterfaceID* pIID);

    public:
        AutoPtr<ArrayOf<Char8> > mText;
    };

public:
    friend class TextKeyListener;
    QwertyKeyListener();

    QwertyKeyListener(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext);

    CARAPI_(Int32) GetInputType();

    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    static CARAPI_(void) MarkAsReplaced(
        /* [in] */ ISpannable* content,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ const String& original);

protected:
    QwertyKeyListener(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext,
        /* [in] */ Boolean fullKeyboard);

    CARAPI_(void) Init(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext);

    CARAPI_(void) Init(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext,
        /* [in] */ Boolean fullKeyboard);

private:

    CARAPI_(String) GetReplacement(
        /* [in] */ ICharSequence* src,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IView* view);

    CARAPI_(Boolean) ShowCharacterPicker(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Char32 c,
        /* [in] */ Boolean insert,
        /* [in] */ Int32 count);

    static CARAPI_(String) ToTitleCase(
        /* [in] */ const String& src);

public:
    static HashMap<Char32, String> PICKER_SETS;
    static Boolean sInitPickerSet;

protected://private:
    Capitalize mAutoCap;
    Boolean mAutoText;
    Boolean mFullKeyboard;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__QWERTYKEYLISTENER_H__
