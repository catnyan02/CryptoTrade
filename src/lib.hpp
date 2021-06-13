#include <memory>
#include <string>
#include <vector>

float INITIAL_BALANCE = 1000

// Balance of cryptocoin wallet. coin_names - names of coins with regard to their positions in the vector of their balances.
struct crypto_balance{
    std::vector<std::string> coin_names = { "krsch_coin", "kzmnv_coin", "bitcoin", "etherium", "dogecoin",
                                            "ravencoin", "dodo" };
    std::vector<float> balance_by_coin = {0, 0, 0, 0, 0, 0, 0};

    auto coin_number(string coin_name) -> int; //allows to look up the index of the coin in balance_by_coin
    auto current_value(int coin) -> float; //allows to look up the current value of a coin
    auto add_coins(int coin, int val) -> bool; //allows to add chosen coins to the wallet
    auto withdraw_coins(int coin, int val) -> bool; //allows to withdraw chosen coins from the wallet
};

struct user {
    struct crypto_balance wallet;
    float balance = INITIAL_BALANCE;
    int day = 0;

    auto next_day() -> bool; //allows to go to the next day of trading coins
    auto add_coins(int coin, int val) -> bool; //allows to add chosen coins to the wallet
    auto withdraw_coins(int coin, int val) -> bool; //allows to withdraw chosen coins from the wallet
    auto count_profit() -> float; //allows to count profit
    auto score() -> std::string; // allows to count the score of a player
    auto end_of_the_week() -> bool; //generates information for last day week recap
};

struct crypto_currency{
    int coin;
    float market_cap;
    float circulating_sup;
    std::vector<float> daily_values;

    auto daily_fluctuations() -> bool; //changes market_cap and circulating_sup of a coin
    auto count_value() -> float; //counts today's value of a coin
    auto update_values(float val) -> bool; //adds value to daily_values
    auto init_fluctuations() -> float; //creates daily value for 6 previous days
};