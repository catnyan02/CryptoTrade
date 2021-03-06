#pragma once
#include <memory>
#include <string>
#include <vector>

/// Crypto Currency
/**
 * Creates crypto coin. Coin stores its name and parameters that account for
 * value.
 */
struct crypto_currency {
    std::string coin_name;  ///< coin's name
    double market_cap;      ///< market capitalization of the coin
    double circulating_sup; ///< circulating supply of the coin
    std::vector<float>
        daily_values; ///< daily values of the coin for a period of time

    auto daily_fluctuations()
        -> bool; ///< randomly changes market capitalization and circulating
                 ///< supply of the coin
    auto count_value() -> float;           ///< returns current value of a coin
    auto update_values(float val) -> void; ///< adds value to daily_values
    auto init_fluctuations()
        -> bool; ///< creates daily_values for 7 days of the gameplay
};

/// Crypto Wallet
/**
 * Creates crypto wallet for user. Crypto wallet can have various coins.
 */
struct crypto_balance {
    std::vector<crypto_currency>
        coins; ///< vector with coins that you want to add to the wallet
    std::vector<float> balance_by_coin; ///< vector with number of respective
                                        ///< coins that user currently has
    auto init_fluctuations()
        -> void; ///< creates daily_values for 7 days of the gameplay
    auto daily_fluctuations()
        -> void; ///< randomly changes market capitalization and circulating
                 ///< supply of all coins
    auto coin_name(unsigned int coin_number)
        -> std::string; ///< looks up the name of the coin by its index in
                        ///< balance_by_coin
    auto add_coins(unsigned int coin, float val)
        -> bool; ///< adds chosen number of coins to respective coin in the
                 ///< user's wallet
    auto withdraw_coins(unsigned int coin, float val)
        -> bool; ///< withdraws chosen number of coins from respective coin in
                 ///< the user's wallet
};

/// User
/**
 * Initializes User. User has a wallet with different types of coins and a
 * startup capital.
 */
struct user {
    crypto_balance wallet; ///< user's wallet with crypto coins
    float balance;         ///< startup capital of user
    int day;               ///< current day of gameplay

    auto next_day() -> void; ///< goes to the next day of trading operations
    auto current_value(unsigned int coin)
        -> float; ///< looks up current value of a coin
    auto add_coins(unsigned int coin, float val)
        -> bool; ///< buys chosen coin and the adds it to the wallet
    auto withdraw_coins(unsigned int coin, float val)
        -> bool; ///< sells chosen coin and withdraws it from the wallet
    auto count_profit() -> float; ///< counts profit
    auto score() -> std::string;  ///< counts user's (player's) score
    auto end_of_the_week()
        -> std::vector<std::string>; ///< generates information for last day
                                     ///< week recap
};
