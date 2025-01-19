#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include "WebsocketConnection.hpp"

class DeribitTrader
{
    public:
        DeribitTrader(const std::string& client_id, const std::string& client_secret);
        nlohmann::json get_order_book(const std::string& instrument_name);
        void cancel_order(const std::string& order_id);
        std::string place_order(const std::string& instrument_name, const std::string& side, const std::string& quantity, const std::string& price);
        void update_order(const std::string& order_id, const std::string& quantity);
        std::string get_access_token();
        nlohmann::json get_positions();
        void SymbolSubscribe(const std::string& symbol);
        ~DeribitTrader();
    
    private:
        std::string base_url;
        std::string client_id;
        std::string client_secret;
        std::string access_token;
        Logger logger;
};
