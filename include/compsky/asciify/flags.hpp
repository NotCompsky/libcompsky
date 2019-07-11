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
    struct ResetIndex{};
    struct ChangeBuffer{};
    struct ChangeBufferTmp{};
    struct ChangeBufferTmpCount{};
    struct ChangeBufferTmpCountFrom{};
    struct Escape{};
    struct StrLen{};
    struct FillWithLeadingZeros{};
    struct UpToFirstZero{};
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
    }
    namespace to {
        struct AlphaNumeric{};
    }
}
}
}
#endif
