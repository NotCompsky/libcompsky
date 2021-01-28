extern "C" {
# include <libavformat/avformat.h>
# include <libavcodec/avcodec.h>
}
#include <exception>
#include <compsky/macros/likely.hpp>


namespace compsky {
namespace ffmpeg {


class OpenErr : public std::exception {
	const char* what() const throw(){
		return "Cannot open file";
	}
};

class CannotFindStreamErr : public std::exception {
	const char* what() const throw(){
		return "Cannot open file";
	}
};


class AVFile {
 private:
	AVFormatContext* fmt_ctx;
	AVCodecContext* video_ctx;
 public:
	AVFile(const char* const _path)
	: fmt_ctx(nullptr)
	{
		if (unlikely(avformat_open_input(&this->fmt_ctx, _path, nullptr, nullptr) != 0))
			throw OpenErr();
		if (unlikely(avformat_find_stream_info(this->fmt_ctx, NULL) < 0))
			throw CannotFindStreamErr();
	}
	~AVFile(){
		avformat_close_input(&this->fmt_ctx);
	}
	
	bool store_video_stream(){
		for (auto i = 0;  i < this->fmt_ctx->nb_streams;  ++i){
			if (this->fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
				this->video_ctx = this->fmt_ctx->streams[i]->codec;
				return false;
			}
		}
		return true;
	}
	
	unsigned width() const {
		return this->video_ctx->width;
	}
	
	unsigned height() const {
		return this->video_ctx->height;
	}
	
	uint64_t duration() const {
		return this->fmt_ctx->duration / 1000000;
	}
};



}
}
