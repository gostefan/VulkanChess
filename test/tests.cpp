#include <catch2/catch_test_macros.hpp>

namespace {
constexpr int value() { return 4 / 2; }
} // namespace

TEST_CASE("Trivial Test", "[trivial]") { REQUIRE(2 == value()); }