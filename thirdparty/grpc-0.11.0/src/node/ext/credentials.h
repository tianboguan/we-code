/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef NET_GRPC_NODE_CREDENTIALS_H_
#define NET_GRPC_NODE_CREDENTIALS_H_

#include <node.h>
#include <nan.h>
#include "grpc/grpc.h"
#include "grpc/grpc_security.h"

namespace grpc {
namespace node {

/* Wrapper class for grpc_credentials structs */
class Credentials : public ::node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static bool HasInstance(v8::Handle<v8::Value> val);
  /* Wrap a grpc_credentials struct in a javascript object */
  static v8::Handle<v8::Value> WrapStruct(grpc_credentials *credentials);

  /* Returns the grpc_credentials struct that this object wraps */
  grpc_credentials *GetWrappedCredentials();

 private:
  explicit Credentials(grpc_credentials *credentials);
  ~Credentials();

  // Prevent copying
  Credentials(const Credentials &);
  Credentials &operator=(const Credentials &);

  static NAN_METHOD(New);
  static NAN_METHOD(CreateDefault);
  static NAN_METHOD(CreateSsl);
  static NAN_METHOD(CreateComposite);
  static NAN_METHOD(CreateGce);
  static NAN_METHOD(CreateFake);
  static NAN_METHOD(CreateIam);
  static NAN_METHOD(CreateInsecure);
  static NanCallback *constructor;
  // Used for typechecking instances of this javascript class
  static v8::Persistent<v8::FunctionTemplate> fun_tpl;

  grpc_credentials *wrapped_credentials;
};

}  // namespace node
}  // namespace grpc

#endif  // NET_GRPC_NODE_CREDENTIALS_H_
