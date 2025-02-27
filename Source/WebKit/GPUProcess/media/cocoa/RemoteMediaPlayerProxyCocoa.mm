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

#import "config.h"
#import "RemoteMediaPlayerProxy.h"

#if ENABLE(GPU_PROCESS) && PLATFORM(COCOA)

#import "LayerHostingContext.h"
#import "MediaPlayerPrivateRemoteMessages.h"
#import <QuartzCore/QuartzCore.h>
#import <WebCore/IntSize.h>
#import <wtf/MachSendRight.h>

namespace WebKit {

void RemoteMediaPlayerProxy::prepareForPlayback(bool privateMode, WebCore::MediaPlayerEnums::Preload preload, bool preservesPitch, bool prepareForRendering, float videoContentScale, CompletionHandler<void(Optional<LayerHostingContextID>&& inlineLayerHostingContextId, Optional<LayerHostingContextID>&& fullscreenLayerHostingContextId)>&& completionHandler)
{
    m_player->setPrivateBrowsingMode(privateMode);
    m_player->setPreload(preload);
    m_player->setPreservesPitch(preservesPitch);
    m_player->prepareForRendering();
    m_videoContentScale = videoContentScale;
    if (!m_inlineLayerHostingContext)
        m_inlineLayerHostingContext = LayerHostingContext::createForExternalHostingProcess();
    if (!m_fullscreenLayerHostingContext)
        m_fullscreenLayerHostingContext = LayerHostingContext::createForExternalHostingProcess();
    completionHandler(m_inlineLayerHostingContext->contextID(), m_fullscreenLayerHostingContext->contextID());
}

void RemoteMediaPlayerProxy::mediaPlayerFirstVideoFrameAvailable()
{
    // Initially the size of the platformLayer will be 0x0 because we do not provide mediaPlayerContentBoxRect() in this class.
    m_inlineLayerHostingContext->setRootLayer(m_player->platformLayer());
    m_webProcessConnection->send(Messages::MediaPlayerPrivateRemote::FirstVideoFrameAvailable(), m_id);
}

void RemoteMediaPlayerProxy::mediaPlayerRenderingModeChanged()
{
    m_inlineLayerHostingContext->setRootLayer(m_player->platformLayer());
}

void RemoteMediaPlayerProxy::setVideoInlineSizeFenced(const WebCore::IntSize& size, const WTF::MachSendRight& machSendRight)
{
    m_inlineLayerHostingContext->setFencePort(machSendRight.sendRight());
    // We do not want animations here
    [CATransaction begin];
    [CATransaction setDisableActions:YES];
    [m_inlineLayerHostingContext->rootLayer() setFrame:CGRectMake(0, 0, size.width(), size.height())];
    [CATransaction commit];
}

void RemoteMediaPlayerProxy::enterFullscreen(CompletionHandler<void()>&& completionHandler)
{
    auto videoFullscreenLayer = m_player->createVideoFullscreenLayer();
    [videoFullscreenLayer setName:@"Web Video Fullscreen Layer (remote)"];
    [videoFullscreenLayer setPosition:CGPointZero];
    m_fullscreenLayerHostingContext->setRootLayer(videoFullscreenLayer.get());

    m_player->setVideoFullscreenLayer(videoFullscreenLayer.get(), WTFMove(completionHandler));
}

void RemoteMediaPlayerProxy::exitFullscreen(CompletionHandler<void()>&& completionHandler)
{
    m_player->setVideoFullscreenLayer(nullptr, WTFMove(completionHandler));
    m_fullscreenLayerHostingContext->setRootLayer(nullptr);
}

void RemoteMediaPlayerProxy::setVideoFullscreenFrameFenced(const WebCore::FloatRect& rect, const WTF::MachSendRight& machSendRight)
{
    m_fullscreenLayerHostingContext->setFencePort(machSendRight.sendRight());
    m_player->setVideoFullscreenFrame(rect);
}

} // namespace WebKit

#endif // ENABLE(GPU_PROCESS) && PLATFORM(COCOA)
