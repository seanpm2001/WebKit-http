/*
 * Copyright (C) 2018 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(LAYOUT_FORMATTING_CONTEXT)

#include "InlineItem.h"
#include "LayoutInlineTextBox.h"

namespace WebCore {
namespace Layout {

using InlineItems = Vector<InlineItem>;

class InlineTextItem : public InlineItem {
public:
    static void createAndAppendTextItems(InlineItems&, const InlineTextBox&);

    unsigned start() const { return m_startOrPosition; }
    unsigned end() const { return start() + length(); }
    unsigned length() const { return m_length; }

    bool isWhitespace() const { return m_textItemType == TextItemType::Whitespace; }
    bool isCollapsible() const { return m_isCollapsible; }
    bool hasTrailingSoftHyphen() const { return m_hasTrailingSoftHyphen; }
    Optional<InlineLayoutUnit> width() const { return m_hasWidth ? makeOptional(m_width) : Optional<InlineLayoutUnit> { }; }
    bool isEmptyContent() const;

    const InlineTextBox& inlineTextBox() const { return downcast<InlineTextBox>(layoutBox()); }

    InlineTextItem left(unsigned length) const;
    InlineTextItem right(unsigned length) const;

private:
    using InlineItem::TextItemType;

    InlineTextItem(const InlineTextBox&, unsigned start, unsigned length, bool hasTrailingSoftHyphen, Optional<InlineLayoutUnit> width, TextItemType);
    InlineTextItem(const InlineTextBox&);

    static InlineTextItem createWhitespaceItem(const InlineTextBox&, unsigned start, unsigned length, Optional<InlineLayoutUnit> width);
    static InlineTextItem createNonWhitespaceItem(const InlineTextBox&, unsigned start, unsigned length, bool hasTrailingSoftHyphen, Optional<InlineLayoutUnit> width);
    static InlineTextItem createEmptyItem(const InlineTextBox&);
};

inline InlineTextItem InlineTextItem::createWhitespaceItem(const InlineTextBox& inlineTextBox, unsigned start, unsigned length, Optional<InlineLayoutUnit> width)
{
    return { inlineTextBox, start, length, false, width, TextItemType::Whitespace };
}

inline InlineTextItem InlineTextItem::createNonWhitespaceItem(const InlineTextBox& inlineTextBox, unsigned start, unsigned length, bool hasTrailingSoftHyphen, Optional<InlineLayoutUnit> width)
{
    return { inlineTextBox, start, length, hasTrailingSoftHyphen, width, TextItemType::NonWhitespace };
}

inline InlineTextItem InlineTextItem::createEmptyItem(const InlineTextBox& inlineTextBox)
{
    return { inlineTextBox };
}

inline InlineTextItem::InlineTextItem(const InlineTextBox& inlineTextBox, unsigned start, unsigned length, bool hasTrailingSoftHyphen, Optional<InlineLayoutUnit> width, TextItemType textItemType)
    : InlineItem(inlineTextBox, Type::Text)
{
    m_startOrPosition = start;
    m_length = length;
    m_hasWidth = !!width;
    m_hasTrailingSoftHyphen = hasTrailingSoftHyphen;
    m_isCollapsible = textItemType == TextItemType::Whitespace && inlineTextBox.style().collapseWhiteSpace();
    m_width = width.valueOr(0);
    m_textItemType = textItemType;
}

inline InlineTextItem::InlineTextItem(const InlineTextBox& inlineTextBox)
    : InlineItem(inlineTextBox, Type::Text)
{
}

inline InlineTextItem InlineTextItem::left(unsigned length) const
{
    RELEASE_ASSERT(length <= this->length());
    ASSERT(m_textItemType != TextItemType::Undefined);
    ASSERT(length);
    return { inlineTextBox(), start(), length, false, WTF::nullopt, m_textItemType };
}

inline InlineTextItem InlineTextItem::right(unsigned length) const
{
    RELEASE_ASSERT(length <= this->length());
    ASSERT(m_textItemType != TextItemType::Undefined);
    ASSERT(length);
    return { inlineTextBox(), end() - length, length, hasTrailingSoftHyphen(), WTF::nullopt, m_textItemType };
}

}
}

SPECIALIZE_TYPE_TRAITS_INLINE_ITEM(InlineTextItem, isText())

#endif
