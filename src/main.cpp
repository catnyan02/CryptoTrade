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

    auto trading_buttons = Container::Horizontal({
        Button("Buy",
               [&] {
                   new_user.add_coins(coin_selected,
                                      std::stof(amount_content_));
               }),
        Button("Withdraw",
               [&] {
                   new_user.withdraw_coins(coin_selected,
                                           std::stof(amount_content_));
               }),
    });

    auto trading_component = Container::Horizontal({
        trading,
        coins,
        Container::Vertical({
            amount_,
            Container::Horizontal({
                input_add,
                input,
            }),
            trading_buttons,
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

    auto trading_renderer = Renderer(trading_component, [&] {
        auto trading_win =
            window(text(L"Trading mode"), trading->Render() | frame);
        auto coins_win = window(text(L"Coins"), coins->Render());
        auto amount_win = window(text(L"Amount:"), amount_->Render());
        auto buttons_win =
            window(text(L"Choose action"), trading_buttons->Render());
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
                           }),
                           vbox({
                               buttons_win | size(WIDTH, EQUAL, 60),
                           }),
                           filler(),
                       }),
                       hflow(render_command()) | flex_grow,
                   }) |
                   flex_grow | border;
        };
        return vbox({
            window(text(L"Game Over"),
                   vbox({
                       text(L"Days passed: " + to_wstring(new_user.day + 1)),
                       text(L"Total profit: " +
                            to_wstring(new_user.end_of_the_week()[1])),
                       text(L"Score: " +
                            to_wstring(new_user.end_of_the_week()[2])),
                   })),
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
            window(text(L"Game Over"),
                   vbox({
                       text(L"Days passed: " + to_wstring(new_user.day + 1)),
                       text(L"Total profit: " +
                            to_wstring(new_user.end_of_the_week()[1])),
                       text(L"Score: " +
                            to_wstring(new_user.end_of_the_week()[2])),
                   })),
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
            window(text(L"Game Over"),
                   vbox({
                       text(L"Days passed: " + to_wstring(new_user.day + 1)),
                       text(L"Total profit: " +
                            to_wstring(new_user.end_of_the_week()[1])),
                       text(L"Score: " +
                            to_wstring(new_user.end_of_the_week()[2])),
                   })),
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
            text(L"CryptoTrade day " + to_wstring(new_user.day + 1)) | bold |
                hcenter,
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
