/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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

#include "BoundaryPoint.h"

namespace WebCore {

class Range;

struct SimpleRange {
    BoundaryPoint start;
    BoundaryPoint end;

    Node& startContainer() const { return start.container.get(); }
    unsigned startOffset() const { return start.offset; }
    Node& endContainer() const { return end.container.get(); }
    unsigned endOffset() const { return end.offset; }

    bool collapsed() const { return start == end; }

    WEBCORE_EXPORT SimpleRange(const BoundaryPoint&, const BoundaryPoint&);
    WEBCORE_EXPORT SimpleRange(BoundaryPoint&&, BoundaryPoint&&);

    // Convenience overloads to help with transition from using a lot of live ranges.
    // FIXME: Move to the Range class header as either makeSimpleRange or Range::operator SimpleRange.
    WEBCORE_EXPORT SimpleRange(const Range&);
    SimpleRange(const Ref<Range>&);
};

WEBCORE_EXPORT SimpleRange makeRangeSelectingNodeContents(Node&);

bool operator==(const SimpleRange&, const SimpleRange&);

class IntersectingNodeRange;
IntersectingNodeRange intersectingNodes(const SimpleRange&);

struct OffsetRange {
    unsigned start { 0 };
    unsigned end { 0 };
};
OffsetRange characterDataOffsetRange(const SimpleRange&, const Node&);

class IntersectingNodeIterator : public std::iterator<std::forward_iterator_tag, Node> {
public:
    IntersectingNodeIterator(RefPtr<Node>&&);

    Node& operator*() const { ASSERT(m_node); return *m_node; }
    Node* operator->() const { ASSERT(m_node); return m_node.get(); }

    operator bool() const { return m_node; }
    bool operator!() const { return !m_node; }
    bool operator!=(const IntersectingNodeIterator& other) const { return m_node != other.m_node; }

    IntersectingNodeIterator& operator++();

private:
    RefPtr<Node> m_node;
};

class IntersectingNodeRange {
public:
    IntersectingNodeRange(const SimpleRange&);

    IntersectingNodeIterator begin() const { return first(); }
    IntersectingNodeIterator end() const { return sentinel(); }

private:
    RefPtr<Node> first() const;
    RefPtr<Node> sentinel() const;

    const SimpleRange& m_range;
};

inline IntersectingNodeIterator::IntersectingNodeIterator(RefPtr<Node>&& node)
    : m_node(WTFMove(node))
{
}

inline IntersectingNodeRange::IntersectingNodeRange(const SimpleRange& range)
    : m_range(range)
{
}

inline IntersectingNodeRange intersectingNodes(const SimpleRange& range)
{
    return { range };
}

inline SimpleRange::SimpleRange(const Ref<Range>& range)
    : SimpleRange(range.get())
{
}

}
