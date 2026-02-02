#include <cstdint>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <limits>

struct Order {
    int64_t user_id;
    int64_t amount;
    int64_t price;
    bool side;
};

struct BalanceChange {
    int64_t user_id;
    int64_t value;
    std::string currency;
};


class Orderbook {
private:

    std::map<int64_t, std::list<Order>> asks;
    std::map<int64_t, std::list<Order>, std::greater<>> bids;

    static void print_trade(const BalanceChange& change) {
        std::cout << "User " << change.user_id
                  << ": " << (change.value > 0 ? "+" : "") << change.value
                  << " " << change.currency << std::endl;
    }

    void process_buy(Order& order) {
        while (order.amount>0 && !asks.empty()) {
            auto best_ask = asks.begin();

            if (order.price<best_ask->first){break;}

            std::list<Order>& order_list = best_ask->second;
            Order& match_order = order_list.front();

            execute_match(order, match_order);

            if (match_order.amount == 0) {
                order_list.pop_front();
                if (order_list.empty()) {
                    asks.erase(best_ask);
                }
            }
        }
        if (order.amount > 0) {
            bids[order.price].push_back(order);
            std::cout << "Order placed in BIDS (rest: " << order.amount << ")" << std::endl;
        }
    }

    void process_sell(Order& order) {
        while (order.amount > 0 && !bids.empty()) {
            auto best_bid = bids.begin();

            if (order.price>best_bid->first){break;}

            std::list<Order>& order_list = best_bid->second;
            Order& match_order = order_list.front();

            execute_match(order, match_order);

            if (match_order.amount == 0) {
                order_list.pop_front();
                if (order_list.empty()) {
                    bids.erase(best_bid);
                }
            }
        }
        if (order.amount > 0) {
            asks[order.price].push_back(order);
            std::cout << "Order placed in ASKS (rest: " << order.amount << ")" << std::endl;
        }
    }

    void execute_match(Order& order, Order& match_order) {
        int64_t trade_quantity = std::min(order.amount, match_order.amount);
        int64_t trade_price = match_order.price;
        int64_t total_value = trade_quantity * trade_price;

        order.amount -= trade_quantity;
        match_order.amount -= trade_quantity;

        std::cout << "--- MATCH EXECUTED (" << trade_quantity << " UAH @ " << trade_price << " USD) ---" << std::endl;

        if (order.side) {
            print_trade({order.user_id, trade_quantity, "UAH"});
            print_trade({order.user_id, -total_value, "USD"});

            print_trade({match_order.user_id, -trade_quantity, "UAH"});
            print_trade({match_order.user_id, total_value, "USD"});
        } else {
            print_trade({order.user_id, -trade_quantity, "UAH"});
            print_trade({order.user_id, total_value, "USD"});

            print_trade({match_order.user_id, trade_quantity, "UAH"});
            print_trade({match_order.user_id, -total_value, "USD"});
        }
        std::cout << "------------------------------------------" << std::endl;
    }
public:
    void add_order(Order order) {
        if (order.side) {
            process_buy(order);
        } else {
            process_sell(order);
        }
    }

};


int main() {
    Orderbook ob;
    int64_t id, amount, price;
    int side_input;

    std::cout << "Orderbook Started. Enter orders format: [ID] [Amount] [Price] [1=Buy/0=Sell]" << std::endl;

    while (true) {
        std::cout << "> ";
        if (!(std::cin >> id >> amount >> price >> side_input)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Try again." << std::endl;
            continue;
        }

        ob.add_order({id, amount, price, (side_input == 1)});
    }

    return 0;
}