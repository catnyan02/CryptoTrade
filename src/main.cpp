
#include "lib.hpp"
#include <array>  // for array
#include <chrono> // for operator""s, chrono_literals
#include <cmath>  // for sin
#include <cstdarg>
#include <experimental/random>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <functional> // for ref, reference_wrapper, function
#include <iostream>
#include <memory>  // for allocator, shared_ptr, __shared_ptr_access
#include <string>  // for wstring, basic_string, operator+, to_wstring
#include <thread>  // for sleep_for, thread
#include <utility> // for move
#include <vector>  // for vector

#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp" // for Checkbox, Renderer, Horizontal, Vertical, Input, Menu, Radiobox, Tab, Toggle
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/event.hpp"          // for Event, Event::Custom
#include "ftxui/component/input.hpp"          // for InputBase
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp" // for text, operator|, color, bgcolor, Element, filler, size, vbox, flex, hbox, graph, separator, EQUAL, WIDTH, hcenter, bold, border, window, HEIGHT, Elements, hflow, flex_grow, frame, gauge, LESS_THAN, spinner, dim, GREATER_THAN
#include "ftxui/screen/color.hpp" // for Color, Color::BlueLight, Color::RedLight, Color::Black, Color::Blue, Color::Cyan, Color::CyanLight, Color::GrayDark, Color::GrayLight, Color::Green, Color::GreenLight, Color::Magenta, Color::MagentaLight, Color::Red, Color::White, Color::Yellow, Color::YellowLight, Color::Default

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

auto crypto_currency::daily_fluctuations() -> bool {
    std::experimental::reseed();
    int range_max_cap = static_cast<int>(market_cap * 0.01);
    int range_max_sup = static_cast<int>(circulating_sup);
    int random_cap = std::experimental::randint(0, range_max_cap);
    std::experimental::reseed();
    int random_sup = std::experimental::randint(0, range_max_sup);
    market_cap += random_cap;
    circulating_sup += random_sup;
    return (market_cap >= 0 and circulating_sup >= 0);
}

auto crypto_currency::count_value() -> float {
    auto value = static_cast<float>(market_cap / circulating_sup);
    return value;
}

auto crypto_currency::update_values(float val) -> void {
    daily_values.push_back(val);
}

auto crypto_currency::init_fluctuations() -> bool {
    for (int i = 0; i < 7; i++) {
        auto value = count_value();
        update_values(value);
        daily_fluctuations();
    }
    return daily_values.size() == 7;
}

auto crypto_balance::init_fluctuations() -> void {
    for (int i = 0; i < coins.size(); i++) {
        coins[i].init_fluctuations();
    }
}

auto crypto_balance::daily_fluctuations() -> void {
    for (int i = 0; i < coins.size(); i++) {
        coins[i].daily_fluctuations();
    }
}

auto crypto_balance::coin_name(unsigned int coin_number) -> std::string {
    if (coin_number < coins.size()) {
        return coins[coin_number].coin_name;
    }
    return "error";
}

auto crypto_balance::add_coins(unsigned int coin, float val) -> bool {
    if (coin < coins.size()) {
        balance_by_coin[coin] += val;
        return true;
    }
    return false;
}

auto crypto_balance::withdraw_coins(unsigned int coin, float val) -> bool {
    if (coin < coins.size()) {
        auto current_balance = balance_by_coin[coin];
        if (current_balance >= val) {
            balance_by_coin[coin] -= val;
            return true;
        }
        return false;
    }
    return false;
}

auto user::next_day() -> void {
    day += 1;
    for (auto coin : wallet.coins) {
        coin.daily_fluctuations();
        auto new_value = coin.count_value();
        coin.update_values(new_value);
    }
}

auto user::current_value(unsigned int coin) -> float {
    if (coin < wallet.coins.size()) {
        auto day_of_metrics = 6 + day;
        auto coin_to_evaluate = wallet.coins[coin];
        return coin_to_evaluate.daily_values[day_of_metrics];
    }
    return -1, 01;
}

auto user::add_coins(unsigned int coin, float val) -> bool {
    auto coin_value = current_value(coin);
    float purchase_value = coin_value * val;
    if (balance >= purchase_value) {
        balance -= purchase_value;
        wallet.add_coins(coin, val);
        return true;
    }
    return false;
}

auto user::withdraw_coins(unsigned int coin, float val) -> bool {
    if (wallet.withdraw_coins(coin, val)) {
        auto coin_value = current_value(coin);
        balance -= coin_value * val;
        return true;
    }
    return false;
}

auto user::count_profit() -> float {
    for (int i = 0; i < 7; i++) {
        float to_withdraw = wallet.balance_by_coin[i];
        withdraw_coins(i, to_withdraw);
    }
    float profit = balance - INITIAL_BALANCE;
    return profit;
}

auto user::score() -> std::string {
    auto profit = count_profit();
    if (profit <= 0) {
        return "Failure!";
    }
    if (profit / INITIAL_BALANCE <= 0.25) {
        return "Not bad...";
    }
    if (profit / INITIAL_BALANCE <= 0.50) {
        return "Good.";
    }
    if (profit / INITIAL_BALANCE < 1) {
        return "Excellent!";
    }
    return "Expert!!!";
}

auto user::end_of_the_week() -> std::vector<std::string> {
    auto days = std::to_string(day);
    auto profit = std::to_string(count_profit());
    auto result = score();
    std::vector<std::string> end_results{days, profit, result};
    return end_results;
}

int main(int argc, char *argv[]) {
    crypto_balance user_balance{{{"krsch_coin", COIN_CAP_0, COIN_SUP_0},
                                 {"kzmnv_coin", COIN_CAP_1, COIN_SUP_1},
                                 {"bitcoin", COIN_CAP_2, COIN_SUP_2},
                                 {"etherium", COIN_CAP_3, COIN_SUP_3},
                                 {"dogecoin", COIN_CAP_4, COIN_SUP_4},
                                 {"ravencoin", COIN_CAP_5, COIN_SUP_5},
                                 {"dodo", COIN_CAP_6, COIN_SUP_6}},
                                {0, 0, 0, 0, 0, 0, 0}};
    user new_user{user_balance, INITIAL_BALANCE, 0};
    new_user.wallet.init_fluctuations();
    auto the_end = false;

    using namespace ftxui;

    std::vector<std::wstring> coins_label = {
        L"krsch_coin", L"kzmnv_coin", L"bitcoin", L"etherium",
        L"dogecoin",   L"ravencoin",  L"dodo",
    };

    auto screen = ScreenInteractive::Fullscreen();

    int shift = 0;

    const std::vector<std::wstring> trading_entries = {
        L"withdraw",
        L"buy",
    };

    int trading_selected = 0;
    Component trading = Radiobox(&trading_entries, &trading_selected);

    std::array<bool, 7> options_state = {
        false, false, false, false, false, false, false,
    };
    std::wstring input_add_content;
    Component input_add = Input(&input_add_content, "input files");

    std::vector<std::wstring> input_entries;
    int input_selected = 0;
    Component input = Menu(&input_entries, &input_selected);

    std::wstring amount_content_ = L"";
    Component amount_ = Input(&amount_content_, "amount");

    int coin_selected = 0;
    Component coins = Container::Vertical({
        Radiobox(&coins_label, &coin_selected),
        // Checkbox(&options_label[0], &options_state[0]),
        //     Checkbox(&options_label[1], &options_state[1]),
        //     Checkbox(&options_label[2], &options_state[2]),
        //     Checkbox(&options_label[3], &options_state[3]),
        //     Checkbox(&options_label[4], &options_state[4]),
        //     Checkbox(&options_label[5], &options_state[5]),
        //     Checkbox(&options_label[6], &options_state[6]),
    });

    Component markets = Radiobox(&coins_label, &coin_selected);
    Component rates = Container::Vertical({});

    auto markets_component = Container::Horizontal({
        Container::Vertical({markets}),
        Container::Vertical({rates}),
    });

    // auto coin_data_component = Container::Horizontal(
    // );

    auto trading_component = Container::Horizontal({
        trading,
        coins,
        Container::Vertical({
            amount_,
            Container::Horizontal({
                input_add,
                input,
            }),
        }),
    });

    InputBase::From(input_add)->on_enter = [&] {
        input_entries.push_back(input_add_content);
        input_add_content = L"";
    };

    auto render_command = [&] {
        Elements line;
        // Trading
        line.push_back(text(trading_entries[trading_selected]) | bold);
        // flags
        line.push_back(text(L" "));
        line.push_back(text(coins_label[coin_selected]) | dim);
        // Amount
        if (!amount_content_.empty()) {
            line.push_back(text(L" ") | bold);
            line.push_back(text(amount_content_) | color(Color::BlueLight) |
                           bold);
        }
        // Input
        for (auto &it : input_entries) {
            line.push_back(text(L" " + it) | color(Color::RedLight));
        }
        return line;
    };

    auto trading_buttons = Container::Horizontal({
        Button("Buy", [&] { new_user.next_day(); }),
        Button("Withdraw", [&] { the_end = true; }),
    });

    auto game_over = window(
        text(L"Game Over"),
        vbox({
            text(L"Days passed: " + to_wstring(new_user.end_of_the_week()[0])),
            text(L"Total profit: " + to_wstring(new_user.end_of_the_week()[1])),
            text(L"Score: " + to_wstring(new_user.end_of_the_week()[2])),
        }));

    auto trading_renderer = Renderer(trading_component, [&] {
        auto trading_win =
            window(text(L"Trading mode"), trading->Render() | frame);
        auto coins_win = window(text(L"Coins"), coins->Render());
        auto amount_win = window(text(L"Amount:"), amount_->Render());
        auto input_win =
            window(text(L"Input"),
                   hbox({
                       vbox({
                           hbox({
                               text(L"Add: "),
                               input_add->Render(),
                           }) | size(WIDTH, EQUAL, 20) |
                               size(HEIGHT, EQUAL, 1),
                           filler(),
                       }),
                       separator(),
                       input->Render() | frame | size(HEIGHT, EQUAL, 3) | flex,
                   }));
        if (!the_end) {
            return vbox({
                       hbox({
                           trading_win | size(HEIGHT, LESS_THAN, 6),
                           coins_win,
                           vbox({
                               amount_win | size(WIDTH, EQUAL, 20),
                               input_win | size(WIDTH, EQUAL, 60),
                           }),
                           filler(),
                       }),
                       hflow(render_command()) | flex_grow,
                   }) |
                   flex_grow | border;
        };
        return vbox({
            game_over,
        });
    });

    auto coin_data_renderer = Renderer([&] {
        auto rates = vbox({
            text(to_wstring(
                new_user.wallet.coins[0].daily_values[new_user.day])),
            text(to_wstring(
                new_user.wallet.coins[1].daily_values[new_user.day])),
            text(to_wstring(
                new_user.wallet.coins[2].daily_values[new_user.day])),
            text(to_wstring(
                new_user.wallet.coins[3].daily_values[new_user.day])),
            text(to_wstring(
                new_user.wallet.coins[4].daily_values[new_user.day])),
            text(to_wstring(
                new_user.wallet.coins[5].daily_values[new_user.day])),
            text(to_wstring(
                new_user.wallet.coins[6].daily_values[new_user.day])),
        });
        auto coin_names = vbox({
            text(coins_label[0]),
            text(coins_label[1]),
            text(coins_label[2]),
            text(coins_label[3]),
            text(coins_label[4]),
            text(coins_label[5]),
            text(coins_label[6]),
        });
        if (!the_end) {
            return vbox({
                       hbox({
                           vbox({
                               coin_names | size(WIDTH, EQUAL, 50),
                           }),
                           vbox({
                               rates | size(WIDTH, EQUAL, 50),
                           }),
                           filler(),
                       }),
                       //    hflow(render_command()) | flex_grow,
                   }) |
                   flex_grow | border;
        };
        return vbox({
            game_over,
        });
    });

    auto profile_buttons = Container::Horizontal({
        Button("Next day", [&] { new_user.next_day(); }),
        Button("End Game", [&] { the_end = true; }),
    });

    auto profile_renderer = Renderer(profile_buttons, [&] {
        auto balance_labels = vbox({
            text(L"USD"),
            text(coins_label[0]),
            text(coins_label[1]),
            text(coins_label[2]),
            text(coins_label[3]),
            text(coins_label[4]),
            text(coins_label[5]),
            text(coins_label[6]),
        });
        auto balances = vbox({
            text(to_wstring(new_user.balance)),
            text(to_wstring(new_user.wallet.balance_by_coin[0])),
            text(to_wstring(new_user.wallet.balance_by_coin[1])),
            text(to_wstring(new_user.wallet.balance_by_coin[2])),
            text(to_wstring(new_user.wallet.balance_by_coin[3])),
            text(to_wstring(new_user.wallet.balance_by_coin[4])),
            text(to_wstring(new_user.wallet.balance_by_coin[5])),
            text(to_wstring(new_user.wallet.balance_by_coin[6])),
        });

        if (!the_end) {
            return vbox({
                       hbox({
                           vbox({
                               balance_labels | size(WIDTH, EQUAL, 50),
                           }),
                           vbox({
                               balances | size(WIDTH, EQUAL, 50),
                           }),
                           //    vbox({
                           //        rates | size(WIDTH, EQUAL, 50),
                           //    }),
                           filler(),
                       }),
                       hbox({
                           profile_buttons->Render(),
                       }),
                       //    hflow(render_command()) | flex_grow,
                   }) |
                   flex_grow | border;
        };
        return vbox({
            game_over,
        });
    });

    int tab_index = 0;
    std::vector<std::wstring> tab_entries = {
        L"trade",
        L"coin rates",
        L"profile",
    };
    auto tab_selection = Toggle(&tab_entries, &tab_index);
    auto tab_content = Container::Tab(
        {
            trading_renderer,
            coin_data_renderer,
            profile_renderer,

        },
        &tab_index);

    auto main_container = Container::Vertical({
        tab_selection,
        tab_content,
    });

    auto main_renderer = Renderer(main_container, [&] {
        return vbox({
            text(L"CryptoTrade") | bold | hcenter,
            tab_selection->Render() | hcenter,
            tab_content->Render() | flex,
        });
    });

    std::thread update([&screen, &shift]() {
        for (;;) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(0.05s);
            shift++;
            screen.PostEvent(Event::Custom);
        }
    });

    screen.Loop(main_renderer);

    return 0;
}
