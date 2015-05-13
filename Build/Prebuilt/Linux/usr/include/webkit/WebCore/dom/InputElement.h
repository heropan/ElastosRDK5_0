/*
 * Copyright (C) 2008 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef InputElement_h
#define InputElement_h

#include "PlatformString.h"
#include <wtf/text/AtomicString.h>

namespace WebCore {

class Attribute;
class Document;
class Element;
class Event;
class InputElementData;

class InputElement {
public:
    virtual ~InputElement() { }

    virtual bool isAutofilled() const = 0;
    virtual bool isChecked() const = 0;
    virtual bool isIndeterminate() const = 0;
    virtual bool isInputTypeHidden() const = 0;
    virtual bool isPasswordField() const = 0;
    virtual bool isSearchField() const = 0;
    virtual bool isTextField() const = 0;
    virtual bool isRadioButton() const = 0;
    virtual bool isCheckbox() const = 0;

    virtual bool supportsMaxLength() const = 0;
    virtual bool hasSpinButton() const { return false; }
#if ENABLE(INPUT_SPEECH)
    virtual bool isSpeechEnabled() const = 0;
#endif

    virtual bool searchEventsShouldBeDispatched() const = 0;

    virtual int size() const = 0;
    virtual const WTF::String& suggestedValue() const = 0;
    virtual WTF::String value() const = 0;
    virtual void setValue(const WTF::String&, bool sendChangeEvent = false) = 0;
    virtual void setValueForUser(const WTF::String&) = 0;
    // The value which is drawn by a renderer.
    virtual WTF::String visibleValue() const = 0;
    virtual WTF::String convertFromVisibleValue(const WTF::String&) const = 0;

    // Returns true if the specified string can be set as the value of InputElement.
    virtual bool isAcceptableValue(const WTF::String&) const = 0;
    virtual WTF::String sanitizeValue(const WTF::String&) const = 0;
    virtual void setValueFromRenderer(const WTF::String&) = 0;

    virtual void cacheSelection(int start, int end) = 0;
    virtual void select() = 0;

#if ENABLE(WCSS)
    virtual InputElementData data() const = 0;
#endif

    static const int s_maximumLength;
    static const int s_defaultSize;

    // Replaces CRs and LFs, shrinks the value for s_maximumLength.
    // This should be applied to values from the HTML value attribute and the DOM value property.
    // This function should be called only by sanitizeValue() implementations.
    // Public so it can be called by InputType.
    static WTF::String sanitizeValueForTextField(const InputElement*, const WTF::String&);

protected:
    static void dispatchFocusEvent(InputElement*, Element*);
    static void dispatchBlurEvent(InputElement*, Element*);
    static void updateFocusAppearance(InputElementData&, InputElement*, Element*, bool restorePreviousSelection);
    static void updateSelectionRange(InputElement*, Element*, int start, int end);
    static void aboutToUnload(InputElement*, Element*);
    static void setValueFromRenderer(InputElementData&, InputElement*, Element*, const WTF::String&);
    // Replaces CRs and LFs, shrinks the value for the specified maximum length.
    // This should be applied to values specified by users.
    // The input string may be a fragment of the whole value.
    static WTF::String sanitizeUserInputValue(const InputElement*, const WTF::String&, int);
    static void handleBeforeTextInsertedEvent(InputElementData&, InputElement*, Element*, Event*);
    static void parseSizeAttribute(InputElementData&, Element*, Attribute*);
    static void parseMaxLengthAttribute(InputElementData&, InputElement*, Element*, Attribute*);
    static void updateValueIfNeeded(InputElementData&, InputElement*);
    static void notifyFormStateChanged(Element*);
#if ENABLE(WCSS)
    static bool isConformToInputMask(const InputElementData&, const WTF::String&);
    static bool isConformToInputMask(const InputElementData&, UChar, unsigned);
    static WTF::String validateInputMask(InputElementData&, WTF::String&);
#endif
};

// HTML/WMLInputElement hold this struct as member variable
// and pass it to the static helper functions in InputElement
class InputElementData {
public:
    InputElementData();
    ~InputElementData();

    const AtomicString& name() const;
    void setName(const AtomicString& value) { m_name = value; }

    // The null String represents "use the default value," and the empty String
    // represents the empty value.
    WTF::String value() const { return m_value; }
    void setValue(const WTF::String& value) { m_value = value; }

    const WTF::String& suggestedValue() const { return m_suggestedValue; }
    void setSuggestedValue(const WTF::String& value) { m_suggestedValue = value; }

    int size() const { return m_size; }
    void setSize(int value) { m_size = value; }

    int maxLength() const { return m_maxLength; }
    void setMaxLength(int value) { m_maxLength = value; }

    int cachedSelectionStart() const { return m_cachedSelectionStart; }
    void setCachedSelectionStart(int value) { m_cachedSelectionStart = value; }

    int cachedSelectionEnd() const { return m_cachedSelectionEnd; }
    void setCachedSelectionEnd(int value) { m_cachedSelectionEnd = value; }

#if ENABLE(WCSS)
    WTF::String inputFormatMask() const { return m_inputFormatMask; }
    void setInputFormatMask(const WTF::String& mask) { m_inputFormatMask = mask; }

    unsigned maxInputCharsAllowed() const { return m_maxInputCharsAllowed; }
    void setMaxInputCharsAllowed(unsigned maxLength) { m_maxInputCharsAllowed = maxLength; }

#endif

private:
    AtomicString m_name;
    WTF::String m_value;
    WTF::String m_suggestedValue;
    int m_size;
    int m_maxLength;
    int m_cachedSelectionStart;
    int m_cachedSelectionEnd;
#if ENABLE(WCSS)
    WTF::String m_inputFormatMask;
    unsigned m_maxInputCharsAllowed;
#endif
};

}

#endif
