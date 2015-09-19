// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_VIEWPOSITIONOBSERVER_H_
#define _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_VIEWPOSITIONOBSERVER_H_

#include "ext/frameworkext.h"

using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewTreeObserver;
using Elastos::Utility::IArrayList;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
  * Used to register listeners that can be notified of changes to the position of a view.
  */
class ViewPositionObserver
    : public Object
    , public PositionObserver
{
public:
    class InnerViewTreeObserverOnPreDrawListener
        : public Object
        , public ViewTreeObserver::OnPreDrawListener
    {
    public:
        InnerViewTreeObserverOnPreDrawListener(
            /* [in] */ ViewPositionObserver* owner);

        // @Override
        CARAPI_(Boolean) OnPreDraw();

    private:
        ViewPositionObserver* mOwner;
    };

public:
    /**
      * @param view The view to observe.
      */
    ViewPositionObserver(
        /* [in] */ IView* view);

    /**
      * @return The current x position of the observed view.
      */
    // @Override
    CARAPI_(Int32) GetPositionX();

    /**
      * @return The current y position of the observed view.
      */
    // @Override
    CARAPI_(Int32) GetPositionY();

    /**
      * Register a listener to be called when the position of the underlying view changes.
      */
    // @Override
    CARAPI AddListener(
        /* [in] */ Listener* listener);

    /**
      * Remove a previously installed listener.
      */
    // @Override
    CARAPI RemoveListener(
        /* [in] */ Listener* listener);

private:
    CARAPI_(AutoPtr< ArrayOf<Int32> >) MiddleInitMposition();

    CARAPI NotifyListeners();

    CARAPI UpdatePosition();

private:
    AutoPtr<IView> mView;
    // Absolute position of the container view relative to its parent window.
    AutoPtr< ArrayOf<Int32> > mPosition;
    /*const*/ AutoPtr< IArrayList<Listener> > mListeners;
    AutoPtr<ViewTreeObserver::OnPreDrawListener> mPreDrawListener;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_VIEWPOSITIONOBSERVER_H_
