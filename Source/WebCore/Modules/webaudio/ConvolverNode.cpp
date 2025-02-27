/*
 * Copyright (C) 2010, Google Inc. All rights reserved.
 * Copyright (C) 2016, Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if ENABLE(WEB_AUDIO)

#include "ConvolverNode.h"

#include "AudioBuffer.h"
#include "AudioNodeInput.h"
#include "AudioNodeOutput.h"
#include "Reverb.h"
#include <wtf/IsoMallocInlines.h>

// Note about empirical tuning:
// The maximum FFT size affects reverb performance and accuracy.
// If the reverb is single-threaded and processes entirely in the real-time audio thread,
// it's important not to make this too high.  In this case 8192 is a good value.
// But, the Reverb object is multi-threaded, so we want this as high as possible without losing too much accuracy.
// Very large FFTs will have worse phase errors. Given these constraints 32768 is a good compromise.
const size_t MaxFFTSize = 32768;

namespace WebCore {

WTF_MAKE_ISO_ALLOCATED_IMPL(ConvolverNode);

ConvolverNode::ConvolverNode(AudioContext& context, float sampleRate)
    : AudioNode(context, sampleRate)
{
    setNodeType(NodeTypeConvolver);

    addInput(makeUnique<AudioNodeInput>(this));
    addOutput(makeUnique<AudioNodeOutput>(this, 2));

    // Node-specific default mixing rules.
    m_channelCount = 2;
    m_channelCountMode = ClampedMax;
    m_channelInterpretation = AudioBus::Speakers;
    
    initialize();
}

ConvolverNode::~ConvolverNode()
{
    uninitialize();
}

void ConvolverNode::process(size_t framesToProcess)
{
    AudioBus* outputBus = output(0)->bus();
    ASSERT(outputBus);

    // Synchronize with possible dynamic changes to the impulse response.
    std::unique_lock<Lock> lock(m_processMutex, std::try_to_lock);
    if (!lock.owns_lock()) {
        // Too bad - the try_lock() failed. We must be in the middle of setting a new impulse response.
        outputBus->zero();
        return;
    }

    if (!isInitialized() || !m_reverb.get())
        outputBus->zero();
    else {
        // Process using the convolution engine.
        // Note that we can handle the case where nothing is connected to the input, in which case we'll just feed silence into the convolver.
        // FIXME: If we wanted to get fancy we could try to factor in the 'tail time' and stop processing once the tail dies down if
        // we keep getting fed silence.
        m_reverb->process(input(0)->bus(), outputBus, framesToProcess);
    }
}

void ConvolverNode::reset()
{
    auto locker = holdLock(m_processMutex);
    if (m_reverb)
        m_reverb->reset();
}

void ConvolverNode::initialize()
{
    if (isInitialized())
        return;

    AudioNode::initialize();
}

void ConvolverNode::uninitialize()
{
    if (!isInitialized())
        return;

    m_reverb = nullptr;
    AudioNode::uninitialize();
}

ExceptionOr<void> ConvolverNode::setBuffer(AudioBuffer* buffer)
{
    ASSERT(isMainThread());
    
    if (!buffer)
        return { };

    if (buffer->sampleRate() != context().sampleRate())
        return Exception { NotSupportedError };

    unsigned numberOfChannels = buffer->numberOfChannels();
    size_t bufferLength = buffer->length();

    // The current implementation supports only 1-, 2-, or 4-channel impulse responses, with the
    // 4-channel response being interpreted as true-stereo (see Reverb class).
    bool isChannelCountGood = (numberOfChannels == 1 || numberOfChannels == 2 || numberOfChannels == 4) && bufferLength;

    if (!isChannelCountGood)
        return Exception { NotSupportedError };

    // Wrap the AudioBuffer by an AudioBus. It's an efficient pointer set and not a memcpy().
    // This memory is simply used in the Reverb constructor and no reference to it is kept for later use in that class.
    auto bufferBus = AudioBus::create(numberOfChannels, bufferLength, false);
    for (unsigned i = 0; i < numberOfChannels; ++i)
        bufferBus->setChannelMemory(i, buffer->channelData(i)->data(), bufferLength);

    bufferBus->setSampleRate(buffer->sampleRate());

    // Create the reverb with the given impulse response.
    bool useBackgroundThreads = !context().isOfflineContext();
    auto reverb = makeUnique<Reverb>(bufferBus.get(), AudioNode::ProcessingSizeInFrames, MaxFFTSize, 2, useBackgroundThreads, m_normalize);

    {
        // Synchronize with process().
        auto locker = holdLock(m_processMutex);
        m_reverb = WTFMove(reverb);
        m_buffer = buffer;
    }

    return { };
}

AudioBuffer* ConvolverNode::buffer()
{
    ASSERT(isMainThread());
    return m_buffer.get();
}

double ConvolverNode::tailTime() const
{
    return m_reverb ? m_reverb->impulseResponseLength() / static_cast<double>(sampleRate()) : 0;
}

double ConvolverNode::latencyTime() const
{
    return m_reverb ? m_reverb->latencyFrames() / static_cast<double>(sampleRate()) : 0;
}

} // namespace WebCore

#endif // ENABLE(WEB_AUDIO)
