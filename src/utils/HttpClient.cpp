#include "utils/Result.hpp"
#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#include <utils/HttpClient.hpp>
namespace cc::utils {

Result<std::string>
HttpClient::get(const std::string& url,
                const std::vector<std::pair<std::string, std::string>>& headers) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return Result<std::string>::fail(ErrorCode::NetworkError, "Failed");
    }
    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(
        curl, CURLOPT_WRITEFUNCTION, +[](void* ptr, size_t sz, size_t nm, void* ud) -> size_t {
            auto* out = static_cast<std::string*>(ud);
            out->append(static_cast<char*>(ptr), sz * nm);
            return sz * nm;
        });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    // timeouts
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, static_cast<long>(timeoutMs_));
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, static_cast<long>(timeoutMs_ / 2));

    // HTTPS sanity
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    // Follow redirects (e.g., http -> https, CDN hops)
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS,
                     10L); // cap redirect chain
#if LIBCURL_VERSION_NUM >= 0x073800
                           // allow only http/https redirects (defense-in-depth)
    curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS_STR, "http,https");
#endif
    // headers (Accept, User-Agent, etc.)
    struct curl_slist* chunk = nullptr;
    for (const auto& h : headers) {
        std::string line = h.first + ": " + h.second;
        chunk = curl_slist_append(chunk, line.c_str());
    }
    if (chunk)
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    if (!userAgent_.empty()) {
        curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent_.c_str());
    }

    // perform

    const CURLcode rc = curl_easy_perform(curl);

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (chunk)
        curl_slist_free_all(chunk);
    curl_easy_cleanup(curl);

    if (rc != CURLE_OK) {
        return Result<std::string>::fail(ErrorCode::NetworkError, curl_easy_strerror(rc));
    }
    if (http_code < 200 || http_code >= 300) {
        return Result<std::string>::fail(ErrorCode::NetworkError,
                                         "HTTP " + std::to_string(http_code));
    }

    return Result<std::string>::ok(std::move(response));
}

Result<std::string> HttpClient::getJson(const std::string& url) {
    std::vector<std::pair<std::string, std::string>> headers{{"Accept", "application/json"}};
    return this->get(url, headers);
}
void HttpClient::setTimeoutMs(int ms) {
    this->timeoutMs_ = ms;
}
void HttpClient::setUserAgents(const std::string& ua) {
    this->userAgent_ = ua;
}
}; // namespace cc::utils
