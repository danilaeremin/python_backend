// Copyright 2025, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <condition_variable>
#include <mutex>

#include "pb_utils.h"

namespace triton { namespace backend { namespace python {

class PbBLSCancel {
 public:
  PbBLSCancel(void* infer_playload_id)
      : updating_(false), infer_playload_id_(infer_playload_id),
        is_cancelled_(false)
  {
  }
  DISALLOW_COPY_AND_ASSIGN(PbBLSCancel);

  void SaveToSharedMemory(std::unique_ptr<SharedMemoryManager>& shm_pool);
  bi::managed_external_buffer::handle_t ShmHandle();
  CancelBLSRequestMessage* ShmPayload();

  void Cancel();
  void ReportIsCancelled(bool is_cancelled);

 private:
  AllocatedSharedMemory<CancelBLSRequestMessage> cancel_shm_;

  std::mutex mu_;
  std::condition_variable cv_;
  bool updating_;

  void* infer_playload_id_;
  bool is_cancelled_;
};

}}};  // namespace triton::backend::python
