// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_COLORSUGGESTIONLISTADAPTER_H_
#define _ELASTOS_DROID_WEBKIT_UI_COLORSUGGESTIONLISTADAPTER_H_

//package org.chromium.ui;

//import android.content.Context;
//import android.graphics.Color;
//import android.graphics.drawable.GradientDrawable;
//import android.graphics.drawable.LayerDrawable;
//import android.text.TextUtils;
//import android.view.View;
//import android.view.ViewGroup;
//import android.widget.AbsListView;
//import android.widget.BaseAdapter;
//import android.widget.LinearLayout;

//import org.chromium.base.ApiCompatibilityUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
 * The adapter used to populate ColorPickerSimple.
 */
class ColorSuggestionListAdapter
    : public BaseAdapter
    , public View::OnClickListener
{
public:
    /**
     * The callback used to indicate the user has clicked on a suggestion.
     */
    class OnColorSuggestionClickListener
    {
    public:
        /**
         * Called upon a click on a suggestion.
         *
         * @param suggestion The suggestion that was clicked.
         */
        virtual CARAPI OnColorSuggestionClick(
            /* [in] */ ColorSuggestion* suggestion) = 0;
    };

public:
    ColorSuggestionListAdapter(
        /* [in] */ IContext* context,
        /* [in] */ ArrayOf< AutoPtr< ColorSuggestion > >* suggestions);

    /**
     * Sets the listener that will be notified upon a click on a suggestion.
     */
    virtual CARAPI SetOnColorSuggestionClickListener(
        /* [in] */ OnColorSuggestionClickListener* listener);

    //@Override
    CARAPI OnClick(
        /* [in] */ IView* v);

    //@Override
    CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    //@Override
    CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 position);

    //@Override
    CARAPI_(AutoPtr<Object>) GetItem(
        /* [in] */ Int32 position);

    //@Override
    CARAPI_(Int32) GetCount();

private:
    /**
     * Sets up the color button to represent a color suggestion.
     *
     * @param button The button view to set up.
     * @param index The index of the suggestion in mSuggestions.
     */
    CARAPI SetUpColorButton(
        /* [in] */ IView* button,
        /* [in] */ Int32 index);

private:
    static const Int32 COLORS_PER_ROW = 4;
    AutoPtr<IContext> mContext;
    AutoPtr< ArrayOf< AutoPtr<ColorSuggestion> > > mSuggestions;
    AutoPtr<OnColorSuggestionClickListener> mListener;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_COLORSUGGESTIONLISTADAPTER_H_

