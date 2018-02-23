#include "../include/Rest.h"
#include "../include/FileLogger.h"
//Global pointer for HTTP listner
std::unique_ptr<Rest> g_http;

// Create Constructor using RAII and bind all handles to required functions.
Rest::Rest(utility::string_t url) : m_listener(url)
{
    m_listener.support(methods::GET, std::bind(&Rest::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&Rest::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&Rest::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&Rest::handle_delete, this, std::placeholders::_1));
}
/* Create bind request functions. */
void Rest::handle_get(http_request message)
{
    std::cout << message.to_string() << std::endl;
    FileLogger::Instance()->writeToFile(message.to_string());
    message.reply(status_codes::OK);
}

void Rest::handle_post(http_request message)
{
    std::cout << message.to_string() << std::endl;
    FileLogger::Instance()->writeToFile(message.to_string());
    message.reply(status_codes::OK);
}

void Rest::handle_delete(http_request message)
{
    std::cout << message.to_string() << std::endl;
    FileLogger::Instance()->writeToFile(message.to_string());
    message.reply(status_codes::OK);
}

void Rest::handle_put(http_request message)
{
    std::cout << message.to_string() << std::endl;
    FileLogger::Instance()->writeToFile(message.to_string());
    message.reply(status_codes::OK);
}


/* Initialize the listner with default bindins.
* can be virtual and has his own pattern
*/
void Rest::on_initialize(const string_t& address)
{

    // Build listener path and use it
    uri_builder uri(address);
    uri.append_path("/api");

    auto addr = uri.to_uri().to_string();
    g_http = std::unique_ptr<Rest>(new Rest(addr));
    g_http->open().wait();

    std::cout << utility::string_t("Listening for request at: ") << addr << std::endl;
    return;
}

void Rest::on_shutdown()
{
    g_http->close().wait();
}
