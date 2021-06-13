#include "catch.hpp"
#include "lib.hpp"

crypto_currency coin {"coin", 139187392.21, 2903870};
crypto_balance wallet {std::vector<crypto_currency> {coin}, {300}};
user new_user{wallet, 1000000, 0};

TEST_CASE("Daily Fluctuations Work") {
    REQUIRE(coin.daily_fluctuations() == true);
}

TEST_CASE("Init Fluctuations Work") {
    REQUIRE(coin.init_fluctuations() == true);
}

TEST_CASE("Add Coins to Crypto Balance Works") {
    REQUIRE(wallet.add_coins(1, 240) == false);
    REQUIRE(wallet.add_coins(0, 240) == true);
}

TEST_CASE("Withdraw Coins to Crypto Balance Works") {
    REQUIRE(wallet.withdraw_coins(0, 250) == true);
    REQUIRE(wallet.withdraw_coins(0, 350) == false);
}

TEST_CASE("User Can Buy Coins") {
    REQUIRE(new_user.add_coins(0, 10) == true);
}

TEST_CASE("User Cannot Buy More Coins Than He Has Money") {
    REQUIRE(new_user.add_coins(0, 1000000000) == false);
}

TEST_CASE("User Can Withdraw Coins") {
    REQUIRE(new_user.withdraw_coins(0, 1) == true);
}

TEST_CASE("User Cannot Withdraw More Coins Than He Has") {
    REQUIRE(new_user.withdraw_coins(0, 10000000000) == false);
}