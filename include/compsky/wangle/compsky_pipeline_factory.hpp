#pragma once

#include "typedefs.hpp" // for str_typ
#include <compsky/wangle/compsky_handler.hpp>
#include <wangle/channel/AsyncSocketHandler.h>
#include <wangle/codec/StringCodec.h>
#include <wangle/codec/FixedLengthFrameDecoder.h> // TODO: Use custom FrameDecoder.h


namespace compsky {
namespace wangler {


typedef wangle::Pipeline<folly::IOBufQueue&, str_typ> CompskyPipeline;


template<size_t buf_sz,  class T>
class CompskyPipelineFactory : public wangle::PipelineFactory<CompskyPipeline> {
 public:
	CompskyPipeline::Ptr newPipeline(std::shared_ptr<folly::AsyncTransportWrapper> sock) override {
		auto pipeline = CompskyPipeline::create();
		pipeline->addBack(wangle::AsyncSocketHandler(sock));
		pipeline->addBack(WhichFrameDecoder(FRAME_DECODER_PARAM));
		pipeline->addBack(WhichStringCodec());
		pipeline->addBack(CompskyHandler<buf_sz, T>());
		pipeline->finalize();
		return pipeline;
	}
};


}
}
