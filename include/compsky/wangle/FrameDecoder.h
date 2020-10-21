/*
 * Copyright 2017-present Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * WARNING
 * 
 * This file has been modified from its original state. (4.b)
 * 
 * WARNING
 */

#pragma once

#include <wangle/codec/ByteToMessageDecoder.h>


namespace compsky {
namespace wangler {

/**
 * A decoder that passes the entire received IOBufQueue.
 */
class FrameDecoder : public wangle::InboundHandler<folly::IOBufQueue&, std::unique_ptr<folly::IOBuf>> {
 public:
	void transportActive(Context* ctx) override {
		transportActive_ = true;
		ctx->fireTransportActive();
	}

	void transportInactive(Context* ctx) override {
		transportActive_ = false;
		ctx->fireTransportInactive();
	}
	
	void read(Context* ctx, folly::IOBufQueue& buf) {
		constexpr size_t max_chainlength_length_set_by_lib = 2048;
		size_t buf_chain_len;
		
		buf_chain_len = buf.chainLength();
		// Just to tell everyone where it terminates
		// TODO: Look into read-only methods to transfer knowledge of length of data
		std::unique_ptr<folly::IOBuf> frame = buf.splitAtMost(std::numeric_limits<size_t>::max());
		
		if (this->result){
			this->result->appendChain(std::move(frame));
		} else {
			this->result = std::move(frame);
		}
		
		if (buf_chain_len != max_chainlength_length_set_by_lib){
			this->result->coalesceWithHeadroomTailroom(0, 1); // WARNING: This pastes each chunk in the wrong order! // TODO: Fix
			*this->result->writableTail() = 0;
			ctx->fireRead(std::move(this->result));
		}
	}
  
 private:
	bool transportActive_ = true;
	std::unique_ptr<folly::IOBuf> result;
};

} // namespace wangler
}
