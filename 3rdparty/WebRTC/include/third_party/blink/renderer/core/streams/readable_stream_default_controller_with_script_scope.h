// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_STREAMS_READABLE_STREAM_DEFAULT_CONTROLLER_WITH_SCRIPT_SCOPE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_STREAMS_READABLE_STREAM_DEFAULT_CONTROLLER_WITH_SCRIPT_SCOPE_H_

#include "third_party/blink/renderer/bindings/core/v8/script_value.h"
#include "third_party/blink/renderer/bindings/core/v8/to_v8_for_core.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "v8/include/v8.h"

namespace blink {

class ReadableStreamDefaultController;
class ScriptState;
class Visitor;

class CORE_EXPORT ReadableStreamDefaultControllerWithScriptScope
    : public GarbageCollected<ReadableStreamDefaultControllerWithScriptScope> {
 public:
  ReadableStreamDefaultControllerWithScriptScope(ScriptState* script_state,
                                                 ScriptValue controller);

  // Users of the ReadableStreamDefaultControllerWithScriptScope can call this
  // to note that the stream has been canceled and thus they don't anticipate
  // using the ReadableStreamDefaultControllerWithScriptScope anymore.
  // (Close/DesiredSize/Enqueue/Error will become no-ops afterward.)
  void NoteHasBeenCanceled();

  void Close();
  double DesiredSize() const;
  void Enqueue(v8::Local<v8::Value> js_chunk) const;
  void Error(v8::Local<v8::Value> js_error);

  // Helper methods
  template <typename ChunkType>
  void Enqueue(ChunkType chunk) const {
    ScriptState::Scope scope(script_state_);
    v8::Local<v8::Value> js_chunk = ToV8(chunk, script_state_);
    Enqueue(js_chunk);
  }

  template <typename ErrorType>
  void Error(ErrorType error) {
    ScriptState::Scope scope(script_state_);
    v8::Local<v8::Value> js_error = ToV8(error, script_state_);
    Error(js_error);
  }

  void Trace(Visitor*) const;

 private:
  const Member<ScriptState> script_state_;
  Member<ReadableStreamDefaultController> controller_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_STREAMS_READABLE_STREAM_DEFAULT_CONTROLLER_WITH_SCRIPT_SCOPE_H_
