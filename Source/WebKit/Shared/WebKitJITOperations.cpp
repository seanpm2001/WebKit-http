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

#include "config.h"
#include "WebKitJITOperations.h"

#include <JavaScriptCore/JITOperationList.h>

namespace WebKit {

#if ENABLE(JIT_OPERATION_VALIDATION)
extern const uintptr_t startOfHostFunctionsInWebKit __asm("section$start$__DATA_CONST$__jsc_host");
extern const uintptr_t endOfHostFunctionsInWebKit __asm("section$end$__DATA_CONST$__jsc_host");
extern const uintptr_t startOfJITOperationsInWebKit __asm("section$start$__DATA_CONST$__jsc_ops");
extern const uintptr_t endOfJITOperationsInWebKit __asm("section$end$__DATA_CONST$__jsc_ops");
#endif

void populateJITOperations()
{
#if ENABLE(JIT_OPERATION_VALIDATION)
    static std::once_flag onceKey;
    std::call_once(onceKey, [] {
        JSC::JITOperationList::populatePointersInEmbedder(&startOfHostFunctionsInWebKit, &endOfHostFunctionsInWebKit, &startOfJITOperationsInWebKit, &endOfJITOperationsInWebKit);
    });
#endif
}

}
