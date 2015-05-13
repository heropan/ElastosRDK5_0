/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
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
 */

#ifndef CSSStyleDeclaration_h
#define CSSStyleDeclaration_h

#include "StyleBase.h"
#include <wtf/Forward.h>

namespace WebCore {

class CSSMutableStyleDeclaration;
class CSSProperty;
class CSSRule;
class CSSValue;

typedef int ExceptionCode;

class CSSStyleDeclaration : public StyleBase {
public:
    static bool isPropertyName(const WTF::String&);

    CSSRule* parentRule() const;

    virtual WTF::String cssText() const = 0;
    virtual void setCssText(const WTF::String&, ExceptionCode&) = 0;

    unsigned length() const { return virtualLength(); }
    virtual unsigned virtualLength() const = 0;
    bool isEmpty() const { return !length(); }
    virtual WTF::String item(unsigned index) const = 0;

    PassRefPtr<CSSValue> getPropertyCSSValue(const WTF::String& propertyName);
    WTF::String getPropertyValue(const WTF::String& propertyName);
    WTF::String getPropertyPriority(const WTF::String& propertyName);
    WTF::String getPropertyShorthand(const WTF::String& propertyName);
    bool isPropertyImplicit(const WTF::String& propertyName);

    virtual PassRefPtr<CSSValue> getPropertyCSSValue(int propertyID) const = 0;
    virtual WTF::String getPropertyValue(int propertyID) const = 0;
    virtual bool getPropertyPriority(int propertyID) const = 0;
    virtual int getPropertyShorthand(int propertyID) const = 0;
    virtual bool isPropertyImplicit(int propertyID) const = 0;

    void setProperty(const WTF::String& propertyName, const WTF::String& value, ExceptionCode&);
    void setProperty(const WTF::String& propertyName, const WTF::String& value, const WTF::String& priority, ExceptionCode&);
    WTF::String removeProperty(const WTF::String& propertyName, ExceptionCode&);
    virtual void setProperty(int propertyId, const WTF::String& value, bool important, ExceptionCode&) = 0;
    virtual WTF::String removeProperty(int propertyID, ExceptionCode&) = 0;

    virtual PassRefPtr<CSSMutableStyleDeclaration> copy() const = 0;
    virtual PassRefPtr<CSSMutableStyleDeclaration> makeMutable() = 0;

    void diff(CSSMutableStyleDeclaration*) const;

    PassRefPtr<CSSMutableStyleDeclaration> copyPropertiesInSet(const int* set, unsigned length) const;

protected:
    CSSStyleDeclaration(CSSRule* parentRule = 0);

    virtual bool cssPropertyMatches(const CSSProperty*) const;

};

} // namespace WebCore

#endif // CSSStyleDeclaration_h
