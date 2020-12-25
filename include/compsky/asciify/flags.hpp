#ifndef LIBCOMPSKY_ASCIIFY_FLAGS_HPP
#define LIBCOMPSKY_ASCIIFY_FLAGS_HPP

namespace compsky {
namespace asciify {
namespace flag {
    namespace concat {
        struct Start{};
        struct End{};
    }
    namespace prefix {
        struct Start{};
        struct End{};
    }
	struct EnvExpand{};
	struct [[deprecated("Use compsky::asciify::flag::until::NullOr instead")]] UntilNullOr{};
	namespace until {
		struct NullOr{};
		struct NullOrNthChar{};
	}
    struct Escape{};
	struct Escape3{};
	struct JSONEscape{};
	struct Repeat{};
    struct StrLen{};
    struct FillWithLeadingZeros{};
    struct UpToFirstZero{};
    struct LiteralChar{};
	struct AlphaNumeric{};
	struct TerminatedBy{};
	template<unsigned N>
	struct Zip{};
	struct NElements{};
	struct Hex{};
	namespace grammatical_case {
		struct Lower{};
		struct Upper{};
	}
	namespace esc {
		namespace URI_until_space {
			// WARNING: AFAIK, in URLs, one should have e.g. spaces %20 before the ? and + after.
			// This is for the former case
			struct Unescape{};
		}
		struct SpacesAndNonAscii{};
		struct DoubleQuote{};
		struct Null{};
	}
    namespace ensure {
        struct BetweenZeroAndOneInclusive{};
        struct BetweenZeroAndOneExclusive{};
        
        // Legacy
        typedef BetweenZeroAndOneInclusive BetweenZeroAndOne;
        
        struct BetweenZeroAndTenLeftInclusive{};
    }
    namespace guarantee {
        struct BetweenZeroAndOneInclusive{};
        struct BetweenZeroAndOneExclusive{};
        
        // Legacy
        typedef BetweenZeroAndOneInclusive BetweenZeroAndOne;
        
        struct BetweenZeroAndTenLeftInclusive{};
		
		struct Positive{};
    }
    namespace to {
        struct AlphaNumeric{};
    }
}
}
}
#endif
