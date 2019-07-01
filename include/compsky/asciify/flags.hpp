#ifndef __ASCIIFY_FLAGS__
#define __ASCIIFY_FLAGS__

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
    struct ChangeBuffer{};
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
