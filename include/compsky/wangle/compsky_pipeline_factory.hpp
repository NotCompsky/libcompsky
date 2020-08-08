#pragma once

#include <compsky/wangle/CStringCodec.h>
#include <compsky/wangle/FrameDecoder.h>
#include <compsky/wangle/compsky_handler.hpp>
#include <wangle/channel/AsyncSocketHandler.h>


typedef wangle::Pipeline<folly::IOBufQueue&,  std::string_view> CompskyPipeline;


template<size_t buf_sz,  class T>
class CompskyPipelineFactory : public wangle::PipelineFactory<CompskyPipeline> {
 public:
	CompskyPipeline::Ptr newPipeline(std::shared_ptr<folly::AsyncTransportWrapper> sock) override {
		auto pipeline = CompskyPipeline::create();
		pipeline->addBack(wangle::AsyncSocketHandler(sock));
		pipeline->addBack(wangle::FrameDecoder());
		pipeline->addBack(wangle::CStringCodec());
		pipeline->addBack(CompskyHandler<buf_sz, T>());
		pipeline->finalize();
		return pipeline;
	}
};
