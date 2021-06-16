#include "lib.hpp"
#include <iostream>
#include <experimental/random>

float INITIAL_BALANCE = 1000000;

double COIN_CAP_0 = 245125425.75;
double COIN_CAP_1 = 547241254.67;
double COIN_CAP_2 = 668500347021.10;
double COIN_CAP_3 = 274003657734.54;
double COIN_CAP_4 = 40488480660.85;
double COIN_CAP_5 = 648753313.34;
double COIN_CAP_6 = 137155221.42;

double COIN_SUP_0 = 24512;
double COIN_SUP_1 = 745224;
double COIN_SUP_2 = 18733637;
double COIN_SUP_3 = 116269232;
double COIN_SUP_4 = 130007902538;
double COIN_SUP_5 = 8981290000;
double COIN_SUP_6 = 110551965;


auto crypto_currency::daily_fluctuations() -> bool{
    std::experimental::reseed();
    int range_max_cap = static_cast<int>(market_cap * 0.1);
    std::experimental::reseed();
    int range_max_sup = static_cast<int>(circulating_sup * 0.01);
    int random_cap = std::experimental::randint(0, range_max_cap);
    int random_sup = std::experimental::randint(0, range_max_sup);
    int fall_or_rise = (-1)^(random_sup);
    market_cap += random_cap * fall_or_rise;
    circulating_sup += random_sup;  //missing formula to change circulating supply
    return (market_cap >= 0 and circulating_sup >=0);
}

auto crypto_currency::count_value() -> float{
    auto value = static_cast<float>(market_cap / circulating_sup);
    return value;
}

auto crypto_currency::update_values(float val) -> void{
    daily_values.push_back(val);
}

auto crypto_currency::init_fluctuations() -> bool{
    for (int i = 0; i < 7; i++){
        auto value = count_value();
        update_values(value);
        daily_fluctuations();
    }
    return daily_values.size() == 7;
}

auto crypto_balance::coin_name(unsigned int coin_number) -> std::string{
    if (coin_number < coins.size()){
        return coins[coin_number].coin_name;
    }
            return "error";

}

auto crypto_balance::add_coins(unsigned int coin, float val) -> bool{
    if (coin < coins.size()){
        balance_by_coin[coin] += val;
        return true;
    }         return false;

}

auto crypto_balance::withdraw_coins(unsigned int coin, float val) -> bool{
    if (coin < coins.size()){
        auto current_balance = balance_by_coin[coin];
        if (current_balance >= val){
            balance_by_coin[coin] -= val;
            return true;
        }             return false;

    }         return false;

}

auto user::next_day() -> void{
    day += 1;
    for (auto coin : wallet.coins){
        coin.daily_fluctuations();
        auto new_value = coin.count_value();
        coin.update_values(new_value);
    }

}

auto user::current_value(unsigned int coin) -> float{
    if (coin < wallet.coins.size()){
        auto day_of_metrics = 6 + day;
        auto coin_to_evaluate = wallet.coins[coin];
        return coin_to_evaluate.daily_values[day_of_metrics];
    }
            return -1,01;

}

auto user::add_coins(unsigned int coin, float val) -> bool{
    auto coin_value = current_value(coin);
    float purchase_value = coin_value * val;
    if (balance >= purchase_value){
        balance -= purchase_value;
        wallet.add_coins(coin, val);
        return true;
    }         return false;

}

auto user::withdraw_coins(unsigned int coin, float val) -> bool{
    if (wallet.withdraw_coins(coin, val)){
        auto coin_value = current_value(coin);
        balance += coin_value * val;
        return true;
    }        return false;

}

auto user::count_profit() -> float{
    for ( int i=0; i < 7; i++){
        float to_withdraw = wallet.balance_by_coin[i];
        withdraw_coins(i, to_withdraw);
    }
    float profit = balance - INITIAL_BALANCE;
    return profit;
}

auto user::score() -> std::string{
    auto profit = count_profit();
    if (profit <= 0) {
        return "Failure!";
    } if (profit/INITIAL_BALANCE <= 0.25) {
        return "Not bad...";
    } if (profit/INITIAL_BALANCE <= 0.50) {
        return "Good.";
    } if (profit/INITIAL_BALANCE < 1) {
        return "Excellent!";
    }         return "Expert!!!";


}

//auto user::end_of_the_week() -> bool{
//
//}


//int main() {
//    crypto_balance user_balance {{
//                                     {"krsch_coin", COIN_CAP_0, COIN_SUP_0},
//                                        {"kzmnv_coin", COIN_CAP_1, COIN_SUP_1},
//                                           {"bitcoin", COIN_CAP_2, COIN_SUP_2},
//                                              {"etherium", COIN_CAP_3, COIN_SUP_3},
//                                                 {"dogecoin", COIN_CAP_4, COIN_SUP_4},
//                                                    {"ravencoin", COIN_CAP_5, COIN_SUP_5},
//                                                       {"dodo", COIN_CAP_6, COIN_SUP_6}
//                                 }, {0, 0, 0, 0, 0, 0, 0}};
//    user new_user {user_balance, INITIAL_BALANCE, 0};
//    return 0;
//}
