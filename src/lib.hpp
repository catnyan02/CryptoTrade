#include <memory>
#include <string>
#include <vector>


struct crypto_currency{
    std::string coin_name;
    double market_cap;
    double circulating_sup;
    std::vector<float> daily_values;

    auto daily_fluctuations() -> bool; //changes market_cap and circulating_sup of a coin
    auto count_value() -> float; //counts today's value of a coin
    auto update_values(float val) -> void; //adds value to daily_values
    auto init_fluctuations() -> bool; //creates daily value for 6 previous days
};

struct crypto_balance{
    std::vector<crypto_currency> coins;
    std::vector<float> balance_by_coin;

    auto coin_name(unsigned int coin_number) -> std::string; //allows to look up the name of the coin by its index in balance_by_coin
    auto add_coins(unsigned int coin, float val) -> bool; //allows to add chosen coins to the wallet
    auto withdraw_coins(unsigned int coin, float val) -> bool; //allows to withdraw chosen coins from the wallet
};

struct user {
    crypto_balance wallet;
    float balance;
    int day;

    auto next_day() -> void; //allows to go to the next day of trading coins
    auto current_value(unsigned int coin) -> float; //allows to look up the current value of a coin
    auto add_coins(unsigned int coin, float val) -> bool; //allows to add chosen coins to the wallet
    auto withdraw_coins(unsigned int coin, float val) -> bool; //allows to withdraw chosen coins from the wallet
    auto count_profit() -> float; //allows to count profit
    auto score() -> std::string; // allows to count the score of a player
    auto end_of_the_week() -> bool; //generates information for last day week recap
};
