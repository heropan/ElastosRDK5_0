
#ifndef __ELASTOS_DROID_INTERNAL_APP_PLATLOGOACTIVITY_H__
#define __ELASTOS_DROID_INTERNAL_APP_PLATLOGOACTIVITY_H__

#include "elastos/droid/app/Activity.h"
#include "elastos/droid/os/CHandler.h"

using Elastos::Droid::App::Activity;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Widget::IToast;
using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IViewOnLongClickListener;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class PlatLogoActivity;

class LocalViewOnClickListener
    : public ElRefBase
    , IViewOnClickListener
{
public:
    LocalViewOnClickListener(
        /* [in] */ PlatLogoActivity* host)
        : mHost(host)
    {}

    CAR_INTERFACE_DECL();

    CARAPI OnClick(
        /* [in] */ IView* v);

private:
    AutoPtr<PlatLogoActivity> mHost;
};

class LocalViewOnLongClickListener
    : public ElRefBase
    , public IViewOnLongClickListener
{
public:
    LocalViewOnLongClickListener(
        /* [in] */ PlatLogoActivity* host)
        : mHost(host)
    {}

    CAR_INTERFACE_DECL();

    CARAPI OnLongClick(
        /* [in] */ IView* v,
        /* [out] */ Boolean* result);

private:
    AutoPtr<PlatLogoActivity> mHost;
};

class PlatLogoActivity
    : public Activity
{
public:
    PlatLogoActivity()
        : mToast(NULL)
        , mContent(NULL)
        , mCount(0)
    {
        CHandler::New((IHandler**)&mHandler);
    }

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

private:
    CARAPI_(AutoPtr<IView>) MakeView();

public:
    AutoPtr<IToast> mToast;
    AutoPtr<IImageView> mContent;
    Int32 mCount;
    AutoPtr<IHandler> mHandler;
};

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_APP_PLATLOGOACTIVITY_H__
