#include "NSE.h"
#include "../../../Catch2/catch_amalgamated.hpp"

TEST_CASE("Ref basic testing", "[Ref]")
{
    NSE::Ref<NSE::Scene> ref;
    {
        NSE::SRef<NSE::Scene> sref;
        REQUIRE(!ref.Alive());
        sref = NSE::SRef{new NSE::Scene{}};
        ref = sref;
        REQUIRE(ref.Alive());
        NSE::Ref<NSE::RefCounted> refBase{ref};
    }
    REQUIRE(!ref.Alive());
}