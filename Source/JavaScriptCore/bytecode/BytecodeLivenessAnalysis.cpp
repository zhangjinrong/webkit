/*
 * Copyright (C) 2013-2017 Apple Inc. All rights reserved.
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
#include "BytecodeLivenessAnalysis.h"

#include "BytecodeKills.h"
#include "BytecodeLivenessAnalysisInlines.h"
#include "BytecodeUseDef.h"
#include "CodeBlock.h"
#include "FullBytecodeLiveness.h"
#include "HeapInlines.h"
#include "InterpreterInlines.h"
#include "PreciseJumpTargets.h"

namespace JSC {

BytecodeLivenessAnalysis::BytecodeLivenessAnalysis(CodeBlock* codeBlock)
    : m_graph(codeBlock, codeBlock->instructions())
{
    runLivenessFixpoint(codeBlock, codeBlock->instructions(), m_graph);

    if (UNLIKELY(Options::dumpBytecodeLivenessResults()))
        dumpResults(codeBlock);
}

void BytecodeLivenessAnalysis::getLivenessInfoAtBytecodeIndex(CodeBlock* codeBlock, BytecodeIndex bytecodeIndex, FastBitVector& result)
{
    BytecodeBasicBlock* block = m_graph.findBasicBlockForBytecodeOffset(bytecodeIndex.offset());
    ASSERT(block);
    ASSERT(!block->isEntryBlock());
    ASSERT(!block->isExitBlock());
    result.resize(block->out().numBits());
    computeLocalLivenessForBytecodeIndex(codeBlock, codeBlock->instructions(), m_graph, *block, bytecodeIndex, result);
}

FastBitVector BytecodeLivenessAnalysis::getLivenessInfoAtBytecodeIndex(CodeBlock* codeBlock, BytecodeIndex bytecodeIndex)
{
    FastBitVector out;
    getLivenessInfoAtBytecodeIndex(codeBlock, bytecodeIndex, out);
    return out;
}

void BytecodeLivenessAnalysis::computeFullLiveness(CodeBlock* codeBlock, FullBytecodeLiveness& result)
{
    FastBitVector out;

    result.m_beforeUseVector.resize(codeBlock->instructions().size());
    result.m_afterUseVector.resize(codeBlock->instructions().size());
    
    for (BytecodeBasicBlock& block : m_graph.basicBlocksInReverseOrder()) {
        if (block.isEntryBlock() || block.isExitBlock())
            continue;
        
        out = block.out();
        
        auto use = [&] (unsigned bitIndex) {
            // This is the use functor, so we set the bit.
            out[bitIndex] = true;
        };

        auto def = [&] (unsigned bitIndex) {
            // This is the def functor, so we clear the bit.
            out[bitIndex] = false;
        };

        auto& instructions = codeBlock->instructions();
        unsigned cursor = block.totalLength();
        for (unsigned i = block.delta().size(); i--;) {
            cursor -= block.delta()[i];
            BytecodeIndex bytecodeIndex = BytecodeIndex(block.leaderOffset() + cursor);

            stepOverInstructionDef(codeBlock, instructions, m_graph, bytecodeIndex, def);
            stepOverInstructionUseInExceptionHandler(codeBlock, instructions, m_graph, bytecodeIndex, use);
            result.m_afterUseVector[bytecodeIndex.offset()] = out; // AfterUse point.
            stepOverInstructionUse(codeBlock, instructions, m_graph, bytecodeIndex, use);
            result.m_beforeUseVector[bytecodeIndex.offset()] = out; // BeforeUse point.
        }
    }
}

void BytecodeLivenessAnalysis::computeKills(CodeBlock* codeBlock, BytecodeKills& result)
{
    UNUSED_PARAM(result);
    FastBitVector out;

    result.m_codeBlock = codeBlock;
    result.m_killSets = makeUniqueArray<BytecodeKills::KillSet>(codeBlock->instructions().size());
    
    for (BytecodeBasicBlock& block : m_graph.basicBlocksInReverseOrder()) {
        if (block.isEntryBlock() || block.isExitBlock())
            continue;
        
        out = block.out();
        
        unsigned cursor = block.totalLength();
        for (unsigned i = block.delta().size(); i--;) {
            cursor -= block.delta()[i];
            BytecodeIndex bytecodeIndex = BytecodeIndex(block.leaderOffset() + cursor);
            stepOverInstruction(
                codeBlock, codeBlock->instructions(), m_graph, bytecodeIndex,
                [&] (unsigned index) {
                    // This is for uses.
                    if (out[index])
                        return;
                    result.m_killSets[bytecodeIndex.offset()].add(index);
                    out[index] = true;
                },
                [&] (unsigned index) {
                    // This is for defs.
                    out[index] = false;
                });
        }
    }
}

void BytecodeLivenessAnalysis::dumpResults(CodeBlock* codeBlock)
{
    dataLog("\nDumping bytecode liveness for ", *codeBlock, ":\n");
    const auto& instructions = codeBlock->instructions();
    unsigned i = 0;

    unsigned numberOfBlocks = m_graph.size();
    Vector<FastBitVector> predecessors(numberOfBlocks);
    for (BytecodeBasicBlock& block : m_graph)
        predecessors[block.index()].resize(numberOfBlocks);
    for (BytecodeBasicBlock& block : m_graph) {
        for (unsigned successorIndex : block.successors()) {
            unsigned blockIndex = block.index();
            predecessors[successorIndex][blockIndex] = true;
        }
    }

    auto dumpBitVector = [] (FastBitVector& bits) {
        for (unsigned j = 0; j < bits.numBits(); j++) {
            if (bits[j])
                dataLogF(" %u", j);
        }
    };

    for (BytecodeBasicBlock& block : m_graph) {
        dataLogF("\nBytecode basic block %u: %p (offset: %u, length: %u)\n", i++, &block, block.leaderOffset(), block.totalLength());

        dataLogF("Predecessors:");
        dumpBitVector(predecessors[block.index()]);
        dataLogF("\n");

        dataLogF("Successors:");
        FastBitVector successors;
        successors.resize(numberOfBlocks);
        for (unsigned successorIndex : block.successors())
            successors[successorIndex] = true;
        dumpBitVector(successors); // Dump in sorted order.
        dataLogF("\n");

        if (block.isEntryBlock()) {
            dataLogF("Entry block %p\n", &block);
            continue;
        }
        if (block.isExitBlock()) {
            dataLogF("Exit block: %p\n", &block);
            continue;
        }
        for (unsigned bytecodeOffset = block.leaderOffset(); bytecodeOffset < block.leaderOffset() + block.totalLength();) {
            const auto currentInstruction = instructions.at(bytecodeOffset);

            dataLogF("Live variables:");
            FastBitVector liveBefore = getLivenessInfoAtBytecodeIndex(codeBlock, BytecodeIndex(bytecodeOffset));
            dumpBitVector(liveBefore);
            dataLogF("\n");
            codeBlock->dumpBytecode(WTF::dataFile(), currentInstruction);

            bytecodeOffset += currentInstruction->size();
        }

        dataLogF("Live variables:");
        FastBitVector liveAfter = block.out();
        dumpBitVector(liveAfter);
        dataLogF("\n");
    }
}

template<typename EnumType1, typename EnumType2>
constexpr bool enumValuesEqualAsIntegral(EnumType1 v1, EnumType2 v2)
{
    using IntType1 = typename std::underlying_type<EnumType1>::type;
    using IntType2 = typename std::underlying_type<EnumType2>::type;
    if constexpr (sizeof(IntType1) > sizeof(IntType2))
        return static_cast<IntType1>(v1) == static_cast<IntType1>(v2);
    else
        return static_cast<IntType2>(v1) == static_cast<IntType2>(v2);
}

Bitmap<maxNumCheckpointTmps> tmpLivenessForCheckpoint(const CodeBlock& codeBlock, BytecodeIndex bytecodeIndex)
{
    Bitmap<maxNumCheckpointTmps> result;
    uint8_t checkpoint = bytecodeIndex.checkpoint();

    if (!checkpoint)
        return result;

    switch (codeBlock.instructions().at(bytecodeIndex)->opcodeID()) {
    case op_call_varargs:
    case op_tail_call_varargs:
    case op_construct_varargs: {
        static_assert(enumValuesEqualAsIntegral(OpCallVarargs::makeCall, OpTailCallVarargs::makeCall) && enumValuesEqualAsIntegral(OpCallVarargs::argCountIncludingThis, OpTailCallVarargs::argCountIncludingThis));
        static_assert(enumValuesEqualAsIntegral(OpCallVarargs::makeCall, OpConstructVarargs::makeCall) && enumValuesEqualAsIntegral(OpCallVarargs::argCountIncludingThis, OpConstructVarargs::argCountIncludingThis));
        if (checkpoint == OpCallVarargs::makeCall)
            result.set(OpCallVarargs::argCountIncludingThis);
        return result;
    }
    case op_iterator_open: {
        return result;
    }
    case op_iterator_next: {
        result.set(OpIteratorNext::nextResult);
        return result;
    }
    default:
        break;
    }
    RELEASE_ASSERT_NOT_REACHED();
}

} // namespace JSC
