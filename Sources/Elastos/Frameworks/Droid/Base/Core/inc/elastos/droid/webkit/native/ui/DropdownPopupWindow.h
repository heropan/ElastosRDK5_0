// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_DROPDOWNPOPUPWINDOW_H_
#define _ELASTOS_DROID_WEBKIT_UI_DROPDOWNPOPUPWINDOW_H_

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/ListPopupWindow.h"
#include "elastos/droid/webkit/native/base/ViewAndroidDelegate.h"

// package org.chromium.ui;
// import android.content.Context;
// import android.graphics.Rect;
// import android.view.View;
// import android.view.View.MeasureSpec;
// import android.view.View.OnLayoutChangeListener;
// import android.view.ViewGroup;
// import android.widget.LinearLayout;
// import android.widget.ListAdapter;
// import android.widget.ListPopupWindow;
// import android.widget.PopupWindow;
// import org.chromium.ui.base.ViewAndroidDelegate;

using Elastos::Droid::Widget::ListPopupWindow;
using Elastos::Droid::Widget::IListAdapter;
using Elastos::Droid::Widget::IPopupWindowOnDismissListener;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewOnLayoutChangeListener;
using Elastos::Droid::Webkit::Base::ViewAndroidDelegate;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
  * The dropdown list popup window.
  */
class DropdownPopupWindow : public ListPopupWindow
{
public:
    class InnerOnLayoutChangeListener
        : public Object
        , public IViewOnLayoutChangeListener
    {
    public:
        InnerOnLayoutChangeListener(
            /* [in] */ DropdownPopupWindow* owner);

        // @Override
        CARAPI OnLayoutChange(
            /* [in] */ IView* v,
            /* [in] */ Int32 left,
            /* [in] */ Int32 top,
            /* [in] */ Int32 right,
            /* [in] */ Int32 bottom,
            /* [in] */ Int32 oldLeft,
            /* [in] */ Int32 oldTop,
            /* [in] */ Int32 oldRight,
            /* [in] */ Int32 oldBottom);

    private:
        DropdownPopupWindow* mOwner;
    };

    class InnerPopupWindowOnDismissListener
        : public Object
        , public IPopupWindowOnDismissListener
    {
    public:
        InnerPopupWindowOnDismissListener(
            /* [in] */ DropdownPopupWindow* owner);

        // @Override
        CARAPI OnDismiss();

    private:
        DropdownPopupWindow* mOwner;
    };

public:
    /**
      * Creates an DropdownPopupWindow with specified parameters.
      * @param context Application context.
      * @param viewAndroidDelegate View delegate used to add and remove views.
      */
    DropdownPopupWindow(
        /* [in] */ IContext* context,
        /* [in] */ ViewAndroidDelegate* viewAndroidDelegate);

    /**
      * Sets the location and the size of the anchor view that the DropdownPopupWindow will use to
      * attach itself.
      * @param x X coordinate of the top left corner of the anchor view.
      * @param y Y coordinate of the top left corner of the anchor view.
      * @param width The width of the anchor view.
      * @param height The height of the anchor view.
      */
    virtual CARAPI SetAnchorRect(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float width,
        /* [in] */ Float height);

    // @Override
    CARAPI SetAdapter(
        /* [in] */ IListAdapter* adapter);

    // @Override
    CARAPI Show();

    // @Override
    CARAPI SetOnDismissListener(
        /* [in] */ IPopupWindowOnDismissListener* listener);

private:
    /**
      * Measures the width of the list content.
      * @return The popup window width in pixels.
      */
    CARAPI_(Int32) MeasureContentWidth();

public:
    AutoPtr<IListAdapter> mAdapter;

private:
    /*const*/ AutoPtr<IContext> mContext;
    /*const*/ AutoPtr<ViewAndroidDelegate> mViewAndroidDelegate;
    /*const*/ AutoPtr<IView> mAnchorView;
    Float mAnchorWidth;
    Float mAnchorHeight;
    Float mAnchorX;
    Float mAnchorY;
    AutoPtr<IViewOnLayoutChangeListener> mLayoutChangeListener;
    AutoPtr<IPopupWindowOnDismissListener> mOnDismissListener;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_DROPDOWNPOPUPWINDOW_H_

