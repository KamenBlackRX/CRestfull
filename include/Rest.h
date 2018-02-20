#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include <cpprest/asyncrt_utils.h>


using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


class Rest
{
    public:
        Rest();
        Rest(utility::string_t url);

        pplx::task<void> open() { return m_listener.open(); }
        pplx::task<void> close() { return m_listener.close(); }

        static void on_initialize(const string_t& address);
        static void on_shutdown();


    private:
        //handles to processes URI request.
        void handle_get(http_request message);
        void handle_post(http_request message);
        void handle_put(http_request message);
        void handle_delete(http_request message);

        // Listener for endpoint
        http_listener m_listener;
        std::unique_ptr<Rest> g_http;
};