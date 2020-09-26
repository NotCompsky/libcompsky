#pragma once


#ifdef COMPSKY_WANGLE_USE_STD_STRINGS
#else
# include "CStringCodec.h"
# include "FrameDecoder.h"
#endif


namespace compsky {
namespace wangler {

#ifdef COMPSKY_WANGLE_USE_STD_STRINGS
typedef std::string str_typ;
typedef wangle::StringCodec WhichStringCodec;
typedef wangle::FixedLengthFrameDecoder WhichFrameDecoder;
# define FRAME_DECODER_PARAM (1024 * 64)
#else
typedef std::string_view str_typ;
typedef CStringCodec WhichStringCodec;
typedef FrameDecoder WhichFrameDecoder;
# define FRAME_DECODER_PARAM
#endif

typedef wangle::HandlerAdapter<const str_typ> ParentHandlerTyp;

}
}
