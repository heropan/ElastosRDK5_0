
#ifndef __CWEBVIEWACTIVITY_H__
#define __CWEBVIEWACTIVITY_H__

#include "app/Activity.h"
#include "_CActivityOne.h"
#include <elrefbase.h>

using Elastos::ElRefBase;
using Elastos::Droid::App::Activity;
using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::App::IProgressDialog;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::Webkit::IWebChromeClient;
using Elastos::Droid::Webkit::IWebView;
using Elastos::Droid::Webkit::IWebViewClient;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IViewOnKeyListener;

namespace Elastos {
namespace Droid {
namespace DevSamples {
namespace WebViewDemo {

class CActivityOne
    : public Activity
{
public:
    class ButtonOnClickListener
        : public ElLightRefBase
        , public IViewOnClickListener
    {
        public:
            //ButtonOnClickListener():owner(NULL){}
            ButtonOnClickListener(CActivityOne* owner) { this->owner = owner;}

            CAR_INTERFACE_DECL();
            CARAPI OnClick(
                /* [in] */ IView* v);
        private:
            CActivityOne* owner;

    };

    class DialogOnClickListener
        : public ElLightRefBase
        , public IDialogInterfaceOnClickListener
    {
        public:
            DialogOnClickListener(CActivityOne* owner) { this->owner = owner; }
            CAR_INTERFACE_DECL();
            CARAPI OnClick(
                /* [in] */ IDialogInterface* dialog,
                /* [in] */ Int32 which);


        private:
            AutoPtr<CActivityOne> owner;
    };

    class ViewOnKeyListener
        : public ElLightRefBase
        , public IViewOnKeyListener
    {
        public:
            ViewOnKeyListener(CActivityOne* owner) { this->owner = owner; }
            CAR_INTERFACE_DECL();

            CARAPI OnKey(
                /* [in] */ IView * v,
                /* [in] */ Int32 keyCode,
                /* [in] */ IKeyEvent* event,
                /* [out] */ Boolean* result);

        private:
            AutoPtr<CActivityOne> owner;
    };

    //@Override
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI Init();

    CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI ConfirmExit();

    CARAPI Loadurl(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url);

public:
    virtual CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

private:
    CARAPI HandleMessage(
        /* [in] */ Int32 what);

    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what);

public:
    /** Called when the activity is first created. */
    AutoPtr<IWebView> wv;
    AutoPtr<IProgressDialog> pd;
    AutoPtr<IHandler> handler;
    AutoPtr<IViewOnClickListener> clickListener;
    AutoPtr<IViewOnKeyListener> keyListener;
};

} // namespace WebViewDemo
} // namespace DevSamples
} // namespace Droid
} // namespace Elastos

#endif //__CWEBVIEWACTIVITY_H__
