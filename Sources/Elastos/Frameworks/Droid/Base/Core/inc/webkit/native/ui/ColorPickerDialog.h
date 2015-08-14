// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_COLORPICKERDIALOG_H_
#define _ELASTOS_DROID_WEBKIT_UI_COLORPICKERDIALOG_H_

//package org.chromium.ui;

//import android.app.AlertDialog;
//import android.app.Dialog;
//import android.content.Context;
//import android.content.DialogInterface;
//import android.view.LayoutInflater;
//import android.view.View;
//import android.widget.Button;
//import android.widget.TextView;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
 * UI for the color chooser that shows on the Android platform as a result of
 * &lt;input type=color &gt; form element.
 */
class ColorPickerDialog
    : public AlertDialog
    , public OnColorChangedListener
{
private:
    class InnerPositiveButtonClickListener : Dialog::OnClickListener
    {
    public:
        InnerPositiveButtonClickListener(
            /* [in] */ ColorPickerDialog* dialog);

        //@Override
        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialogInterface,
            /* [in] */ Int32 i);

    private:
        ColorPickerDialog* mColorPickerDialog;
    };

    class InnerNegativeButtonClickListener : Dialog::OnClickListener
    {
    public:
        InnerNegativeButtonClickListener(
            /* [in] */ ColorPickerDialog* dialog);

        //@Override
        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialogInterface,
            /* [in] */ Int32 i);

    private:
        ColorPickerDialog* mColorPickerDialog;
    };

    class InnerOnCancelListener : DialogInterface::OnCancelListener
    {
    public:
        InnerOnCancelListener(
            /* [in] */ ColorPickerDialog* dialog);

        //@Override
        CARAPI OnCancel(
            /* [in] */ IDialogInterface* arg0);

    private:
        ColorPickerDialog* mColorPickerDialog;
    };

    class InnerOnClickListener : Button::OnClickListener
    {
    public:
        InnerOnClickListener(
            /* [in] */ ColorPickerDialog* dialog);

        //@Override
        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        ColorPickerDialog* mColorPickerDialog;
    };

    friend class InnerPositiveButtonClickListener;
    friend class InnerNegativeButtonClickListener;
    friend class InnerOnCancelListener;
    friend class InnerOnClickListener;

public:
    /**
     * @param context The context the dialog is to run in.
     * @param listener The object to notify when the color is set.
     * @param color The initial color to set.
     * @param suggestions The list of suggestions.
     */
    ColorPickerDialog(
        /* [in] */ IContext* context,
        /* [in] */ OnColorChangedListener* listener,
        /* [in] */ Int32 color,
        /* [in] */ ArrayOf< AutoPtr<ColorSuggestion> >* suggestions);

    /**
     * Listens to the ColorPicker for when the user has changed the selected color, and
     * updates the current color (the color shown in the title) accordingly.
     *
     * @param color The new color chosen by the user.
     */
    //@Override
    CARAPI OnColorChanged(
        /* [in] */ Int32 color);

private:
    /**
     * Hides the simple view (the default) and shows the advanced one instead, hiding the
     * "More" button at the same time.
     */
    CARAPI ShowAdvancedView();

    /**
     * Tries to notify any listeners that the color has been set.
     */
    CARAPI TryNotifyColorSet(
        /* [in] */ Int32 color);

    /**
     * Updates the internal cache of the currently selected color, updating the colorful little
     * box in the title at the same time.
     */
    CARAPI UpdateCurrentColor(
        /* [in] */ Int32 color);

private:
    /* const */ AutoPtr<ColorPickerAdvanced> mAdvancedColorPicker;
    /* const */ AutoPtr<ColorPickerSimple> mSimpleColorPicker;
    /* const */ AutoPtr<IButton> mMoreButton;
    // The view up in the corner that shows the user the color they've currently selected.
    /* const */ AutoPtr<IView> mCurrentColorView;
    const AutoPtr<OnColorChangedListener> mListener;
    const Int32 mInitialColor;
    Int32 mCurrentColor;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_COLORPICKERDIALOG_H_

