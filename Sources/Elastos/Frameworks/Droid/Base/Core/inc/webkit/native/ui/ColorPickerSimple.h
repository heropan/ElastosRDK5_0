// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_COLORPICKERSIMPLE_H_
#define _ELASTOS_DROID_WEBKIT_UI_COLORPICKERSIMPLE_H_

//package org.chromium.ui;

//import android.content.Context;
//import android.graphics.Color;
//import android.util.AttributeSet;
//import android.widget.ListView;

//import org.chromium.ui.ColorSuggestionListAdapter.OnColorSuggestionClickListener;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
 * Draws a grid of (predefined) colors and allows the user to choose one of
 * those colors.
 */
class ColorPickerSimple
    : public ListView
    , public OnColorSuggestionClickListener
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
     */
    virtual CARAPI Init(
        /* [in] */ ArrayOf< AutoPtr<ColorSuggestion> >* suggestions,
        /* [in] */ OnColorChangedListener* onColorChangedListener);

    //@Override
    OnColorSuggestionClick(
        /* [in] */ ColorSuggestion* suggestion);

    static CARAPI_(AutoPtr< ArrayOf<Int32> >) InitDefaultColors();
    static CARAPI_(AutoPtr< ArrayOf<Int32> >) InitDefaultColorLabelIds();

private:
    AutoPtr<OnColorChangedListener> mOnColorChangedListener;
    static const AutoPtr< ArrayOf<Int32> > DEFAULT_COLORS;
    static const AutoPtr< ArrayOf<Int32> > DEFAULT_COLOR_LABEL_IDS;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_COLORPICKERSIMPLE_H_

