// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_COLORPICKERSIMPLE_H_
#define _ELASTOS_DROID_WEBKIT_UI_COLORPICKERSIMPLE_H_

#include "elastos/droid/ext/frameworkext.h"
//#include "elastos/droid/widget/ListView.h"
#include "elastos/droid/webkit/native/ui/OnColorChangedListener.h"
#include "elastos/droid/webkit/native/ui/ColorSuggestionListAdapter.h"
#include "elastos/droid/webkit/native/ui/OnColorChangedListener.h"

// package org.chromium.ui;
// import android.content.Context;
// import android.graphics.Color;
// import android.util.AttributeSet;
// import android.widget.ListView;
// import org.chromium.ui.ColorSuggestionListAdapter.OnColorSuggestionClickListener;

//using Elastos::Droid::Widget::ListView;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Webkit::Ui::ColorSuggestionListAdapter;
using Elastos::Droid::Webkit::Ui::OnColorChangedListener;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

class ColorSuggestion;

/**
  * Draws a grid of (predefined) colors and allows the user to choose one of
  * those colors.
  */
class ColorPickerSimple
    //: public ListView
    : public ColorSuggestionListAdapter::OnColorSuggestionClickListener
{
public:
    ColorPickerSimple(
        /* [in] */ IContext* context);

    ColorPickerSimple(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    ColorPickerSimple(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    /**
      * Initializes the listener and sets the adapter for the given list of suggestions. If the
      * suggestions is null a default set of colors will be used.
      *
      * @param suggestions The list of suggestions that should be displayed.
      * @param onColorChangedListener The listener that gets notified when the user touches
      *                               a color.
      */// ColorSuggestion for suggestions
    virtual CARAPI Init(
        /* [in] */ ArrayOf<IInterface*>* suggestions,
        /* [in] */ OnColorChangedListener* onColorChangedListener);

    // @Override
    CARAPI OnColorSuggestionClick(
        /* [in] */ ColorSuggestion* suggestion);

private:
    static CARAPI_(AutoPtr< ArrayOf<Int32> >) MiddleInitDefaultColors();

    static CARAPI_(AutoPtr< ArrayOf<Int32> >) MiddleInitDefaultColorLabelIds();

private:
    AutoPtr<OnColorChangedListener> mOnColorChangedListener;
    static AutoPtr< ArrayOf<Int32> > DEFAULT_COLORS;
    static AutoPtr< ArrayOf<Int32> > DEFAULT_COLOR_LABEL_IDS;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_COLORPICKERSIMPLE_H_

