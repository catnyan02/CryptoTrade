#include "lib.hpp"
#include <iostream>

float INITIAL_BALANCE = 1000;

float COIN_CAP_0 = ;
float COIN_CAP_1 = ;
float COIN_CAP_2 = ;
float COIN_CAP_3 = ;
float COIN_CAP_4 = ;
float COIN_CAP_5 = ;
float COIN_CAP_6 = ;

float COIN_SUP_0 = ;
float COIN_SUP_1 = ;
float COIN_SUP_2 = ;
float COIN_SUP_3 = ;
float COIN_SUP_4 = ;
float COIN_SUP_5 = ;
float COIN_SUP_6 = ;


auto crypto_currency::daily_fluctuations() -> bool{
    market_cap += ;  //missing formula to change market cap
    circulating_sup += ;  //missing formula to change circulating supply
    if (market_cap >= 0 and circulating_sup >=0){
        return true;
    }
    else{
        return false;
    }
}

auto crypto_currency::count_value() -> float{
    float value = market_cap / circulating_sup;
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
    if (daily_values.size() == 7){
        return true;
    } else{
        return false;
    }
}

auto crypto_balance::coin_name(int coin_number) -> std::string{
    if (coin_number < coins.size()){
        return coins[coin_number].coin_name;
    }
    else{
        return "error";
    }
}

auto crypto_balance::add_coins(int coin, float val) -> void{
    if (coin < coins.size()){
        balance_by_coin[coin_number] += val;
        return true;
    } else{
        return false;
    }
}

auto crypto_balance::withdraw_coins(int coin, float val) -> bool{
    if (coin < coins.size()){
        auto current_balance = balance_by_coin[coin_number];
        if (current_balance >= val){
            balance_by_coin[coin_number] -= val;
            return true;
        } else{
            return false
        }
    } else{
        return false;
    }
}

auto user::next_day() -> void{
    day += 1;
    for (auto coin : wallet.coins){
        coin.daily_fluctuations();
        auto new_value = coin.count_value();
        coin.update_values(new_value);
    }

}

auto user::current_value(int coin) -> float{
    if (coin_number < wallet.coins.size()){
        int day_of_metrics = 6 + day;
        return wallet.coins[coin_number].daily_values[day_of_metrics];
    }
    else{
        return -1;
    }
}

auto user::add_coins(int coin, float val) -> bool{
    auto coin_value = current_value(coin);
    float purchase_value = coin_value * val;
    if (balance >= purchase_value){
        balance -= purchase_value;
        wallet.add_coins(coin, val);
        return true;
    } else{
        return false;
    }
}

auto user::withdraw_coins(int coin, float val) -> bool{
    if (wallet.withdraw_coins(coin, val)){
        auto coin_value = current_value(coin);
        balance += coin_value * val;
        return true;
    }else{
        return false;
    }
}

auto user::count_profit() -> float{
    for ( int i=0; i < 7; i++){
        to_withdraw = wallet.balance_by_coin[i];
        withdraw_coins(i, to_withdraw);
    }
    profit = balance - INITIAL_BALANCE
}

auto user::score() -> std::string{

}

auto user::end_of_the_week() -> bool;{

}


int main() {
    crypto_balance user_balance {{
                                     {"krsch_coin", COIN_CAP_0, COIN_SUP_0},
                                        {"kzmnv_coin", COIN_CAP_1, COIN_SUP_1},
                                           {"bitcoin", COIN_CAP_2, COIN_SUP_2},
                                              {"etherium", COIN_CAP_3, COIN_SUP_3},
                                                 {"dogecoin", COIN_CAP_4, COIN_SUP_4},
                                                    {"ravencoin", COIN_CAP_5, COIN_SUP_5},
                                                       {"dodo", COIN_CAP_6, COIN_SUP_6}
                                 }, {0, 0, 0, 0, 0, 0, 0}};
    user new_user {user_balance, INITIAL_BALANCE, 0};
    return 0;
}
