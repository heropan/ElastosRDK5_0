// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_COMPONENTS_VALIDATIONMESSAGEBUBBLE_H_
#define _ELASTOS_DROID_WEBKIT_COMPONENTS_VALIDATIONMESSAGEBUBBLE_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "graphics/CPoint.h"
#include "graphics/CRectF.h"
#include "text/TextUtils.h"
#include "view/Gravity.h"
#include "view/View.h"
#include "view/ViewGroup.h"
#include "widget/PopupWindow.h"
#include "widget/RelativeLayout.h"
#include "widget/TextView.h"
#include "webkit/native/base/ApiCompatibilityUtils.h"
#include "webkit/native/content/browser/ContentViewCore.h"
#include "webkit/native/content/browser/RenderCoordinates.h"

// package org.chromium.components.web_contents_delegate_android;
// import android.graphics.Point;
// import android.graphics.RectF;
// import android.text.TextUtils;
// import android.view.Gravity;
// import android.view.View;
// import android.view.ViewGroup;
// import android.widget.PopupWindow;
// import android.widget.RelativeLayout;
// import android.widget.TextView;
// import org.chromium.base.ApiCompatibilityUtils;
// import org.chromium.base.CalledByNative;
// import org.chromium.content.R;
// import org.chromium.content.browser.ContentViewCore;
// import org.chromium.content.browser.RenderCoordinates;

using Elastos::Droid::Graphics::IPoint;
using Elastos::Droid::Graphics::IRectF;
using Elastos::Droid::Text::ITextUtils;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::Widget::IPopupWindow;
using Elastos::Droid::Widget::IRelativeLayout;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Webkit::Base::ApiCompatibilityUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

/**
  * This class is an implementation of validation message bubble UI.
  */
class ValidationMessageBubble : public Object
{
private:
    ValidationMessageBubble(
        /* [in] */ ContentViewCore* contentViewCore,
        /* [in] */ IRectF* anchor,
        /* [in] */ const String& mainText,
        /* [in] */ const String& subText);

    /**
      * Creates a popup window to show the specified messages, and show it on
      * the specified anchor rectangle.
      *
      * @param contentViewCore The ContentViewCore object to provide various
      *                        information.
      * @param anchorX Anchor position in the CSS unit.
      * @param anchorY Anchor position in the CSS unit.
      * @param anchorWidth Anchor size in the CSS unit.
      * @param anchorHeight Anchor size in the CSS unit.
      * @param mainText The main message. It will shown at the top of the popup
      *                 window, and its font size is larger.
      * @param subText The sub message. It will shown below the main message, and
      *                its font size is smaller.
      */
    // @CalledByNative
    static CARAPI_(AutoPtr<ValidationMessageBubble>) CreateAndShow(
        /* [in] */ ContentViewCore* contentViewCore,
        /* [in] */ Int32 anchorX,
        /* [in] */ Int32 anchorY,
        /* [in] */ Int32 anchorWidth,
        /* [in] */ Int32 anchorHeight,
        /* [in] */ const String& mainText,
        /* [in] */ const String& subText);

    // @CalledByNative
    CARAPI Close();

    /**
      * Moves the popup window on the specified anchor rectangle.
      *
      * @param contentViewCore The ContentViewCore object to provide various
      *                        information.
      * @param anchorX Anchor position in the CSS unit.
      * @param anchorY Anchor position in the CSS unit.
      * @param anchorWidth Anchor size in the CSS unit.
      * @param anchorHeight Anchor size in the CSS unit.
      */
    // @CalledByNative
    CARAPI SetPositionRelativeToAnchor(
        /* [in] */ ContentViewCore* contentViewCore,
        /* [in] */ Int32 anchorX,
        /* [in] */ Int32 anchorY,
        /* [in] */ Int32 anchorWidth,
        /* [in] */ Int32 anchorHeight);

    static CARAPI_(AutoPtr<IRectF>) MakePixRectInScreen(
        /* [in] */ ContentViewCore* contentViewCore,
        /* [in] */ Int32 anchorX,
        /* [in] */ Int32 anchorY,
        /* [in] */ Int32 anchorWidth,
        /* [in] */ Int32 anchorHeight);

    static CARAPI_(Float) GetWebViewOffsetYPixInScreen(
        /* [in] */ ContentViewCore* contentViewCore);

    static CARAPI UpdateTextViews(
        /* [in] */ IViewGroup* root,
        /* [in] */ const String& mainText,
        /* [in] */ const String& subText);

    CARAPI Measure(
        /* [in] */ RenderCoordinates* coordinates);

    CARAPI_(Float) GetAnchorOffset();

    /**
      * This adjusts the position if the popup protrudes the web view.
      */
    CARAPI_(AutoPtr<IPoint>) AdjustWindowPosition(
        /* [in] */ ContentViewCore* contentViewCore,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

private:
    AutoPtr<IPopupWindow> mPopup;
};

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_COMPONENTS_VALIDATIONMESSAGEBUBBLE_H_

