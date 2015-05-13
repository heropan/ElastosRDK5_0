/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
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

#ifndef HTMLFormElement_h
#define HTMLFormElement_h

#include "CheckedRadioButtons.h"
#include "FormState.h"
#include "FormSubmission.h"
#include "HTMLElement.h"
#include <wtf/OwnPtr.h>

namespace WebCore {

class Event;
class FormAssociatedElement;
class FormData;
class HTMLFormControlElement;
class HTMLImageElement;
class HTMLInputElement;
class HTMLFormCollection;
class TextEncoding;

struct CollectionCache;

class HTMLFormElement : public HTMLElement {
public:
    static PassRefPtr<HTMLFormElement> create(Document*);
    static PassRefPtr<HTMLFormElement> create(const QualifiedName&, Document*);
    virtual ~HTMLFormElement();

    PassRefPtr<HTMLCollection> elements();
    void getNamedElements(const AtomicString&, WTF::Vector<RefPtr<Node> >&);

    unsigned length() const;
    Node* item(unsigned index);

    WTF::String enctype() const { return m_attributes.encodingType(); }
    void setEnctype(const WTF::String&);

    WTF::String encoding() const { return m_attributes.encodingType(); }
    void setEncoding(const WTF::String& value) { setEnctype(value); }

    // FIXME: Rename this function to shouldAutocomplete.
    bool autoComplete() const;

    // FIXME: Should rename these two functions to say "form control" or "form-associated element" instead of "form element".
    void registerFormElement(FormAssociatedElement*);
    void removeFormElement(FormAssociatedElement*);

    void registerImgElement(HTMLImageElement*);
    void removeImgElement(HTMLImageElement*);

    bool prepareForSubmission(Event*);
    void submit();
    void submitFromJavaScript();
    void reset();

    // Used to indicate a malformed state to keep from applying the bottom margin of the form.
    // FIXME: Would probably be better to call this wasUnclosed; that's more specific.
    void setMalformed(bool malformed) { m_wasMalformed = malformed; }
    bool isMalformed() const { return m_wasMalformed; }

    void setDemoted(bool demoted) { m_wasDemoted = demoted; }

    void submitImplicitly(Event*, bool fromImplicitSubmissionTrigger);
    bool formWouldHaveSecureSubmission(const WTF::String& url);

    WTF::String name() const;

    bool noValidate() const;

    WTF::String acceptCharset() const { return m_attributes.acceptCharset(); }
    void setAcceptCharset(const WTF::String&);

    WTF::String action() const;
    void setAction(const WTF::String&);

    WTF::String method() const;
    void setMethod(const WTF::String&);

    virtual WTF::String target() const;

    bool wasUserSubmitted() const;

    HTMLFormControlElement* defaultButton() const;

    bool checkValidity();

    HTMLFormControlElement* elementForAlias(const AtomicString&);
    void addElementAlias(HTMLFormControlElement*, const AtomicString& alias);

    CheckedRadioButtons& checkedRadioButtons() { return m_checkedRadioButtons; }

    const WTF::Vector<FormAssociatedElement*>& associatedElements() const { return m_associatedElements; }

private:
    HTMLFormElement(const QualifiedName&, Document*);

    virtual bool rendererIsNeeded(RenderStyle*);
    virtual void insertedIntoDocument();
    virtual void removedFromDocument();

    virtual void handleLocalEvents(Event*);

    virtual void parseMappedAttribute(Attribute*);

    virtual bool isURLAttribute(Attribute*) const;

    virtual void documentDidBecomeActive();

    virtual void willMoveToNewOwnerDocument();
    virtual void didMoveToNewOwnerDocument();

    void submit(Event*, bool activateSubmitButton, bool processingUserGesture, FormSubmissionTrigger);

    unsigned formElementIndexWithFormAttribute(Element*);
    unsigned formElementIndex(FormAssociatedElement*);

    // Returns true if the submission should proceed.
    bool validateInteractively(Event*);

    // Validates each of the controls, and stores controls of which 'invalid'
    // event was not canceled to the specified vector. Returns true if there
    // are any invalid controls in this form.
    bool checkInvalidControlsAndCollectUnhandled(WTF::Vector<RefPtr<FormAssociatedElement> >&);

    friend class HTMLFormCollection;

    typedef WTF::HashMap<RefPtr<AtomicStringImpl>, RefPtr<HTMLFormControlElement> > AliasMap;

    FormSubmission::Attributes m_attributes;
    OwnPtr<AliasMap> m_elementAliases;
    OwnPtr<CollectionCache> m_collectionCache;

    CheckedRadioButtons m_checkedRadioButtons;

    unsigned m_associatedElementsBeforeIndex;
    unsigned m_associatedElementsAfterIndex;
    WTF::Vector<FormAssociatedElement*> m_associatedElements;
    WTF::Vector<HTMLImageElement*> m_imageElements;

    bool m_wasUserSubmitted;
    bool m_isSubmittingOrPreparingForSubmission;
    bool m_shouldSubmit;

    bool m_isInResetFunction;

    bool m_wasMalformed;
    bool m_wasDemoted;

    AtomicString m_name;
};

} // namespace WebCore

#endif // HTMLFormElement_h
