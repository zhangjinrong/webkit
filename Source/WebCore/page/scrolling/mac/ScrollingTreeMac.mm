/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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

#include "config.h"
#include "ScrollingTreeMac.h"

#include "Logging.h"
#include "PlatformCALayer.h"
#include "ScrollingTreeFixedNode.h"
#include "ScrollingTreeFrameHostingNode.h"
#include "ScrollingTreeFrameScrollingNodeMac.h"
#include "ScrollingTreeOverflowScrollProxyNode.h"
#include "ScrollingTreeOverflowScrollingNodeMac.h"
#include "ScrollingTreePositionedNode.h"
#include "ScrollingTreeStickyNode.h"
#include "WebLayer.h"
#include "WheelEventTestMonitor.h"
#include <wtf/text/TextStream.h>

#if ENABLE(ASYNC_SCROLLING) && ENABLE(SCROLLING_THREAD)

using namespace WebCore;

Ref<ScrollingTreeMac> ScrollingTreeMac::create(AsyncScrollingCoordinator& scrollingCoordinator)
{
    return adoptRef(*new ScrollingTreeMac(scrollingCoordinator));
}

ScrollingTreeMac::ScrollingTreeMac(AsyncScrollingCoordinator& scrollingCoordinator)
    : ThreadedScrollingTree(scrollingCoordinator)
{
}

Ref<ScrollingTreeNode> ScrollingTreeMac::createScrollingTreeNode(ScrollingNodeType nodeType, ScrollingNodeID nodeID)
{
    switch (nodeType) {
    case ScrollingNodeType::MainFrame:
    case ScrollingNodeType::Subframe:
        return ScrollingTreeFrameScrollingNodeMac::create(*this, nodeType, nodeID);
    case ScrollingNodeType::FrameHosting:
        return ScrollingTreeFrameHostingNode::create(*this, nodeID);
    case ScrollingNodeType::Overflow:
        return ScrollingTreeOverflowScrollingNodeMac::create(*this, nodeID);
    case ScrollingNodeType::OverflowProxy:
        return ScrollingTreeOverflowScrollProxyNode::create(*this, nodeID);
    case ScrollingNodeType::Fixed:
        return ScrollingTreeFixedNode::create(*this, nodeID);
    case ScrollingNodeType::Sticky:
        return ScrollingTreeStickyNode::create(*this, nodeID);
    case ScrollingNodeType::Positioned:
        return ScrollingTreePositionedNode::create(*this, nodeID);
    }
    ASSERT_NOT_REACHED();
    return ScrollingTreeFixedNode::create(*this, nodeID);
}


static void collectDescendantLayersAtPoint(Vector<CALayer *, 16>& layersAtPoint, CALayer *parent, CGPoint point)
{
    if (parent.masksToBounds && ![parent containsPoint:point])
        return;

    for (CALayer *layer in [parent sublayers]) {
        CALayer *layerWithResolvedAnimations = layer;

        if ([[layer animationKeys] count])
            layerWithResolvedAnimations = [layer presentationLayer];

        CGPoint subviewPoint = [layerWithResolvedAnimations convertPoint:point fromLayer:parent];

        auto handlesEvent = [&] {
            if (CGRectIsEmpty([layerWithResolvedAnimations frame]))
                return false;

            if (![layerWithResolvedAnimations containsPoint:subviewPoint])
                return false;

            auto platformCALayer = PlatformCALayer::platformCALayerForLayer((__bridge void*)layer);
            if (platformCALayer) {
                // Scrolling changes boundsOrigin on the scroll container layer, but we computed its event region ignoring scroll position, so factor out bounds origin.
                FloatPoint boundsOrigin = layer.bounds.origin;
                FloatPoint localPoint = subviewPoint - toFloatSize(boundsOrigin);
                return platformCALayer->eventRegionContainsPoint(IntPoint(localPoint));
            }
            
            return false;
        }();

        if (handlesEvent)
            layersAtPoint.append(layer);

        if ([layer sublayers])
            collectDescendantLayersAtPoint(layersAtPoint, layer, subviewPoint);
    };
}

static ScrollingNodeID scrollingNodeIDForLayer(CALayer *layer)
{
    auto platformCALayer = PlatformCALayer::platformCALayerForLayer((__bridge void*)layer);
    return platformCALayer ? platformCALayer->scrollingNodeID() : 0;
}

static bool isScrolledBy(const ScrollingTree& tree, ScrollingNodeID scrollingNodeID, CALayer *hitLayer)
{
    for (CALayer *layer = hitLayer; layer; layer = [layer superlayer]) {
        auto nodeID = scrollingNodeIDForLayer(layer);
        if (nodeID == scrollingNodeID)
            return true;

        auto* scrollingNode = tree.nodeForID(nodeID);
        if (is<ScrollingTreeOverflowScrollProxyNode>(scrollingNode)) {
            ScrollingNodeID actingOverflowScrollingNodeID = downcast<ScrollingTreeOverflowScrollProxyNode>(*scrollingNode).overflowScrollingNodeID();
            if (actingOverflowScrollingNodeID == scrollingNodeID)
                return true;
        }

        if (is<ScrollingTreePositionedNode>(scrollingNode)) {
            if (downcast<ScrollingTreePositionedNode>(*scrollingNode).relatedOverflowScrollingNodes().contains(scrollingNodeID))
                return false;
        }
    }

    return false;
}

RefPtr<ScrollingTreeNode> ScrollingTreeMac::scrollingNodeForPoint(FloatPoint point)
{
    auto* rootScrollingNode = rootNode();
    if (!rootScrollingNode)
        return nullptr;

    LockHolder lockHolder(m_layerHitTestMutex);

    auto rootContentsLayer = static_cast<ScrollingTreeFrameScrollingNodeMac*>(rootScrollingNode)->rootContentsLayer();
    FloatPoint scrollOrigin = rootScrollingNode->scrollOrigin();
    auto pointInContentsLayer = point;
    pointInContentsLayer.moveBy(scrollOrigin);

    Vector<CALayer *, 16> layersAtPoint;
    collectDescendantLayersAtPoint(layersAtPoint, rootContentsLayer.get(), pointInContentsLayer);

    LOG_WITH_STREAM(Scrolling, stream << "ScrollingTreeMac " << this << " scrollingNodeForPoint " << point << " found " << layersAtPoint.size() << " layers");
#if !LOG_DISABLED
    for (auto *layer : WTF::makeReversedRange(layersAtPoint))
        LOG_WITH_STREAM(Scrolling, stream << " layer " << [layer description] << " scrolling node " << scrollingNodeIDForLayer(layer));
#endif

    for (auto *layer : WTF::makeReversedRange(layersAtPoint)) {
        auto nodeID = scrollingNodeIDForLayer(layer);
        
        auto* scrollingNode = nodeForID(nodeID);
        if (!is<ScrollingTreeScrollingNode>(scrollingNode))
            continue;
        
        if (isScrolledBy(*this, nodeID, layersAtPoint.last()))
            return scrollingNode;

        // FIXME: Hit-test scroll indicator layers.
    }

    LOG_WITH_STREAM(Scrolling, stream << "ScrollingTreeMac " << this << " scrollingNodeForPoint " << point << " found no scrollable layers; using root node");
    return rootScrollingNode;
}

void ScrollingTreeMac::lockLayersForHitTesting()
{
    m_layerHitTestMutex.lock();
}

void ScrollingTreeMac::unlockLayersForHitTesting()
{
    m_layerHitTestMutex.unlock();
}

void ScrollingTreeMac::setWheelEventTestMonitor(RefPtr<WheelEventTestMonitor>&& monitor)
{
    m_wheelEventTestMonitor = WTFMove(monitor);
}

void ScrollingTreeMac::receivedWheelEvent(const PlatformWheelEvent& event)
{
    auto monitor = m_wheelEventTestMonitor;
    if (monitor)
        monitor->receivedWheelEvent(event);
}

void ScrollingTreeMac::deferWheelEventTestCompletionForReason(WheelEventTestMonitor::ScrollableAreaIdentifier identifier, WheelEventTestMonitor::DeferReason reason)
{
    auto monitor = m_wheelEventTestMonitor;
    if (!monitor)
        return;

    LOG_WITH_STREAM(WheelEventTestMonitor, stream << "    (!) ScrollingTreeMac::deferForReason: Deferring on " << identifier << " for reason " << reason);
    monitor->deferForReason(identifier, reason);
}

void ScrollingTreeMac::removeWheelEventTestCompletionDeferralForReason(WheelEventTestMonitor::ScrollableAreaIdentifier identifier, WheelEventTestMonitor::DeferReason reason)
{
    auto monitor = m_wheelEventTestMonitor;
    if (!monitor)
        return;

    LOG_WITH_STREAM(WheelEventTestMonitor, stream << "    (!) ScrollingTreeMac::removeDeferralForReason: Removing deferral on " << identifier << " for reason " << reason);
    monitor->removeDeferralForReason(identifier, reason);
}

#endif // ENABLE(ASYNC_SCROLLING) && ENABLE(SCROLLING_THREAD)
