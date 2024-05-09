//
// Created by sel on 08/05/2024.
//

#include "main/cpp/menu/headers/menu.h"

#include "api_rest_fetch.h"

size_t api_rest_fetch::write_callback(void *contents, size_t size, size_t nmemb, std::string *response) noexcept
{
    size_t total_size = size * nmemb;
    response->append((char*)contents, total_size);
    return total_size;
}


std::string api_rest_fetch::generate_request(const std::string &url, const std::string &request_type, const std::string& post_data) noexcept
{
    CURL *curl = curl_easy_init();
    CURLcode res;
    std::string response_buffer = "";

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request_type.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_buffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            std::cerr << "err : " << curl_easy_strerror(res) << std::endl;

        curl_easy_cleanup(curl);
    }

    return response_buffer;
}

std::string api_rest_fetch::_get(const std::string& url) noexcept
{
    return generate_request(url, "GET");
}

std::string api_rest_fetch::_post(const std::string& url, const std::string& data) noexcept
{
    return generate_request(url, "POST");
}

std::string api_rest_fetch::_delete(const std::string& url) noexcept
{
    return generate_request(url, "DELETE");
}