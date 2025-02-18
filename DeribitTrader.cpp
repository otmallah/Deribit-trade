#include "DeribitTrader.hpp"

DeribitTrader::DeribitTrader(const std::string &client_id, const std::string &client_secret)
    : logger(), connection_pool(10, "test.deribit.com", "443")
{
    this->client_id = client_id;
    this->client_secret = client_secret;
    this->base_url = "https://test.deribit.com/api/v2/";
    this->access_token = this->get_access_token();
    this->logger.log(LogLevel::INFO, "DeribitTrader has been created");
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

    if (auth_response.status_code != 200)
    {
        this->logger.log(LogLevel::ERROR, "Failed to get access token");
        return "";
    }

    // Parse the response to get the access token
    nlohmann::json response = nlohmann::json::parse(auth_response.text);
    this->logger.log(LogLevel::INFO, "Access token fetched successfully");
    return response["result"]["access_token"];
}

nlohmann::json DeribitTrader::get_order_book(const std::string &instrument_name)
{
    cpr::Response response = cpr::Get(
        cpr::Url{this->base_url + "public/get_order_book"},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Parameters{{"instrument_name", instrument_name}, {"depth", "100"}});

    if (response.status_code != 200)
    {
        this->logger.log(LogLevel::ERROR, "Failed to get order book");
        return nlohmann::json::parse("{}");
    }

    this->logger.log(LogLevel::INFO, "Order book fetched successfully");
    nlohmann::json json_response = nlohmann::json::parse(response.text);
    return json_response["result"];
}

nlohmann::json DeribitTrader::get_positions()
{
    if (this->access_token == "")
    {
        this->logger.log(LogLevel::ERROR, "Access token is null");
        return nlohmann::json::parse("{}");
    }

    cpr::Response response = cpr::Get(
        cpr::Url{this->base_url + "private/get_positions"},
        cpr::Header{{"Content-Type", "application/json"}, {"Authorization", "Bearer " + this->access_token}});

    if (response.status_code != 200)
    {
        this->logger.log(LogLevel::ERROR, "Failed to get positions");
        return nlohmann::json::parse("{}");
    }

    nlohmann::json json_response = nlohmann::json::parse(response.text);
    this->logger.log(LogLevel::INFO, "Positions fetched successfully");
    return json_response["result"];
}

void DeribitTrader::cancel_order(const std::string &order_id)
{
    if (this->access_token == "")
    {
        this->logger.log(LogLevel::ERROR, "Access token is null");
        return;
    }

    cpr::Response response = cpr::Post(
        cpr::Url{this->base_url + "private/cancel"},
        cpr::Header{{"Content-Type", "application/json"}, {"Authorization", "Bearer " + this->access_token}},
        cpr::Body{"{\"jsonrpc\": \"2.0\","
                  "\"id\": 42,"
                  "\"params\": {"
                  "\"order_id\": \"" +
                  order_id + "\""
                             "}}"});

    if (response.status_code != 200)
    {
        this->logger.log(LogLevel::ERROR, "Failed to cancel order");
    }

    this->logger.log(LogLevel::INFO, "Order cancelled successfully");
}

std::string DeribitTrader::place_order(const std::string &instrument_name, const std::string &side, const std::string &quantity, const std::string &price)
{
    if (this->access_token == "")
    {
        this->logger.log(LogLevel::ERROR, "Access token is null");
        return "";
    }

    cpr::Response response = cpr::Post(
        cpr::Url{this->base_url + "private/" + side},
        cpr::Header{{"Content-Type", "application/json"}, {"Authorization", "Bearer " + this->access_token}},
        cpr::Body{"{\"instrument_name\": \"" + instrument_name + "\","
                                                                 "\"amount\": " +
                  quantity + ","
                             "\"type\": \"limit\","
                             "\"price\": " +
                  price + ","
                          "\"label\": \"market0000234\","
                          "\"post_only\": false}"});

    if (response.status_code != 200)
    {
        this->logger.log(LogLevel::ERROR, "Failed to place order");
        return "";
    }

    this->logger.log(LogLevel::INFO, "Order placed successfully");
    auto json_response = nlohmann::json::parse(response.text);
    return json_response["result"]["order"]["order_id"];
}

void DeribitTrader::update_order(const std::string &order_id, const std::string &quantity)
{
    if (this->access_token == "")
    {
        this->logger.log(LogLevel::ERROR, "Access token is null");
        return;
    }

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

    if (response.status_code != 200)
    {
        this->logger.log(LogLevel::ERROR, "Failed to update order");
    }

    this->logger.log(LogLevel::INFO, "Order updated successfully");
}

void DeribitTrader::SymbolSubscribe(const std::string &symbol)
{
    WebSocketConnection *ws = this->connection_pool.get_connection();
    ws->SymbolSubscribe(symbol);
    this->connection_pool.return_connection(ws);
}

DeribitTrader::~DeribitTrader()
{
    this->logger.log(LogLevel::INFO, "DeribitTrader destroyed");
}
