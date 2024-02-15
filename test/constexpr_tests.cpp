#include <catch2/catch_test_macros.hpp>

namespace {
	constexpr int value() {
		return 4 / 2;
	}
}

TEST_CASE("Trivial test", "[trivial]")
{
  STATIC_REQUIRE(2 == value());
}