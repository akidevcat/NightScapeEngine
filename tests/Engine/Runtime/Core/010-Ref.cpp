#include "NSE.h"
#include <catch2/catch_all.hpp>

#include "Engine/Runtime/Render/Mesh.h"

TEST_CASE("Ref basic testing", "[Auto]")
{
    NSE::Ref<NSE::Mesh> ref;
    {
        NSE::SRef<NSE::Mesh> sref;
        REQUIRE(!ref.Alive());
        sref = NSE::SRef{new NSE::Mesh{}};
        ref = sref;
        REQUIRE(ref.Alive());
        NSE::Ref<NSE::RefCounted> refBase{ref};
    }
    REQUIRE(!ref.Alive());
}