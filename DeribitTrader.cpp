#include "DeribitTrader.hpp"
#include "WebsocketConnection.hpp"

DeribitTrader::DeribitTrader(const std::string& client_id, const std::string& client_secret)
{
    this->client_id = client_id;
    this->client_secret = client_secret;
    this->base_url = "https://test.deribit.com/api/v2/";
    this->access_token = this->get_access_token();
    std::cout << "DeribitTrader has been created" << std::endl;
}

std::string DeribitTrader::get_access_token()
{
    cpr::Response auth_response = cpr::Post(
        cpr::Url{this->base_url + "public/auth"},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{"{\"jsonrpc\": \"2.0\","
                  "\"method\": \"public/auth\","
                  "\"id\": 42,"
                  "\"params\": {"
                  "\"client_id\": \"" +
                  this->client_id + "\","
                              "\"client_secret\": \"" +
                  this->client_secret + "\","
                                  "\"grant_type\": \"client_credentials\""
                                  "}}"});

    // Parse the response to get the access token
    nlohmann::json response = nlohmann::json::parse(auth_response.text);
    return response["result"]["access_token"];
}

nlohmann::json DeribitTrader::get_order_book(const std::string& instrument_name)
{
    cpr::Response response = cpr::Get(
        cpr::Url{this->base_url + "public/get_order_book"},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Parameters{{"instrument_name", instrument_name}, {"depth", "100"}});
    
    std::cout << "Status Code: " << response.status_code << std::endl;
    std::cout << "Response: " << response.text << std::endl;
    if (response.status_code != 200) {
        std::cout << "Error: " << response.text << std::endl;
        return nlohmann::json::parse("{}");
    }


    nlohmann::json json_response = nlohmann::json::parse(response.text);
    return json_response["result"];
}

nlohmann::json DeribitTrader::get_positions()
{
    cpr::Response response = cpr::Get(
        cpr::Url{this->base_url + "private/get_positions"},
        cpr::Header{{"Content-Type", "application/json"}, {"Authorization", "Bearer " + this->access_token}});
    
    nlohmann::json json_response = nlohmann::json::parse(response.text);
    return json_response["result"];
}

void DeribitTrader::cancel_order(const std::string& order_id)
{
    cpr::Response response = cpr::Post(
        cpr::Url{this->base_url + "private/cancel"},
        cpr::Header{{"Content-Type", "application/json"}, {"Authorization", "Bearer " + this->access_token}},
        cpr::Body{"{\"jsonrpc\": \"2.0\","
                  "\"id\": 42,"
                  "\"params\": {"
                  "\"order_id\": \"" +
                  order_id + "\""
                             "}}"});

    std::cout << "---------- Cancel Order ----------" << std::endl;
    std::cout << "Response: " << response.text << std::endl;
    std::cout << "Status Code: " << response.status_code << std::endl;
}

std::string DeribitTrader::place_order(const std::string& instrument_name, const std::string& side, const std::string& quantity, const std::string& price)
{
    cpr::Response response = cpr::Post(
        cpr::Url{this->base_url + "private/" + side},
        cpr::Header{{"Content-Type", "application/json"}, {"Authorization", "Bearer " + this->access_token}},
        cpr::Body{"{\"instrument_name\": \"" + instrument_name + "\","
                  "\"amount\": " + quantity + ","
                  "\"type\": \"limit\","
                  "\"price\": " + price + ","
                  "\"label\": \"market0000234\","
                  "\"post_only\": false}"}
    );

    if (response.status_code == 200) {
        auto json_response = nlohmann::json::parse(response.text);
        return json_response["result"]["order"]["order_id"];
    } else {
        std::cout << "---------- Place Order ----------" << std::endl;
        std::cout << "Response: " << response.text << std::endl;
        std::cout << "Status Code: " << response.status_code << std::endl;
        return "";
    }

    auto json_response = nlohmann::json::parse(response.text);
    return json_response["result"]["order"]["order_id"];
}

void DeribitTrader::update_order(const std::string& order_id, const std::string& quantity)
{
    cpr::Response response = cpr::Post(
        cpr::Url{this->base_url + "private/edit"},
        cpr::Header{{"Content-Type", "application/json"}, {"Authorization", "Bearer " + this->access_token}},
        cpr::Body{"{\"jsonrpc\": \"2.0\","
                  "\"id\": 42,"
                  "\"params\": {"
                  "\"order_id\": \"" +
                  order_id + "\","
                             "\"amount\": " +
                  quantity +
                  "}}"});

    std::cout << "Response: " << response.text << std::endl;
    std::cout << "Status Code: " << response.status_code << std::endl;
}

void DeribitTrader::SymbolSubscribe(const std::string& symbol)
{
    WebSocketConnection ws("test.deribit.com", "443");
    ws.SymbolSubscribe(symbol);
}

DeribitTrader::~DeribitTrader()
{
    std::cout << "DeribitTrader destroyed" << std::endl;
}