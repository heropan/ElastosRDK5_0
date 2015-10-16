#ifndef __ELASTOS_DROID_TEXT_METHOD_MultiTapKeyListener_H__
#define __ELASTOS_DROID_TEXT_METHOD_MultiTapKeyListener_H__

#include "elastos/droid/text/method/BaseKeyListener.h"
#include "elastos/droid/os/HandlerRunnable.h"
#include "Elastos.Droid.Core_server.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Os::HandlerRunnable;
//using Elastos::Droid::Text::ITextKeyListener::Capitalize;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

/**
 * This is the standard key listener for alphabetic input on 12-key
 * keyboards.  You should generally not need to instantiate this yourself;
 * TextKeyListener will do it for you.
 * <p></p>
 * As for all implementations of {@link KeyListener}, this class is only concerned
 * with hardware keyboards.  Software input methods have no obligation to trigger
 * the methods in this class.
 */
class MultiTapKeyListener : public BaseKeyListener
{
private:
    class Timeout
        : public HandlerRunnable
    {
    public:
        friend class MultiTapKeyListener;
    public:

        Timeout(
            /* [in] */ IEditable* buffer);

    public:
        virtual CARAPI Run();

    private:
        AutoPtr<IEditable> mBuffer;
    };

public:
    MultiTapKeyListener(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext);

    CARAPI_(void) Init(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext);

    CARAPI_(Int32) GetInputType();

    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI OnSpanChanged(
        /* [in] */ ISpannable* buf,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 s,
        /* [in] */ Int32 e,
        /* [in] */ Int32 start,
        /* [in] */ Int32 stop);

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

protected:
    static HashMap<Int32, String> InitStaticRecs();
    MultiTapKeyListener();

private:
    static CARAPI_(void) RemoveTimeouts(
        /* [in] */ ISpannable* buf);

protected://private
    //private static final SparseArray<String> sRecs = new SparseArray<String>();
    static HashMap<Int32, String> sRecs;
    Capitalize mCapitalize;
    Boolean mAutoText;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_MultiTapKeyListener_H__
