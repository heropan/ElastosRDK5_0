// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_COLORSUGGESTIONLISTADAPTER_H_
#define _ELASTOS_DROID_WEBKIT_UI_COLORSUGGESTIONLISTADAPTER_H_

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/BaseAdapter.h"

// package org.chromium.ui;
// import android.content.Context;
// import android.graphics.Color;
// import android.graphics.drawable.GradientDrawable;
// import android.graphics.drawable.LayerDrawable;
// import android.text.TextUtils;
// import android.view.View;
// import android.view.ViewGroup;
// import android.widget.AbsListView;
// import android.widget.BaseAdapter;
// import android.widget.LinearLayout;
// import org.chromium.base.ApiCompatibilityUtils;

using Elastos::Droid::Widget::BaseAdapter;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IViewOnClickListener;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

class ColorSuggestion;

/**
  * The adapter used to populate ColorPickerSimple.
  */
class ColorSuggestionListAdapter
    // BaseAdapter parent hasno Object
    : public Object
    , public BaseAdapter
    , public IViewOnClickListener
{
public:
    CAR_INTERFACE_DECL()

    /**
      * The callback used to indicate the user has clicked on a suggestion.
      */
    class OnColorSuggestionClickListener : public Object
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
        /* [in] */ ArrayOf<ColorSuggestion*>* suggestions);

    /**
      * Sets the listener that will be notified upon a click on a suggestion.
      */
    virtual CARAPI SetOnColorSuggestionClickListener(
        /* [in] */ OnColorSuggestionClickListener* listener);

    // @Override
    CARAPI OnClick(
        /* [in] */ IView* v);

    // @Override
    CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    // @Override
    CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 position);

    // @Override
    // consistent with the parent class
    CARAPI_(AutoPtr<IInterface>) GetItem(
        /* [in] */ Int32 position);

    // @Override
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
    AutoPtr<IContext> mContext;
    AutoPtr< ArrayOf<ColorSuggestion*> > mSuggestions;
    AutoPtr<OnColorSuggestionClickListener> mListener;
    static const Int32 COLORS_PER_ROW = 4;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_COLORSUGGESTIONLISTADAPTER_H_

