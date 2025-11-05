#include "ApiClient.h"
#include <stdexcept>

namespace DeskBuddy
{
    void ApiClient::setBearer(String token) { _bearer = token; }
    void ApiClient::setDefaultHeader(String key, String value) { _extraHeaders += key + ": " + value + "\r\n"; }
    bool ApiClient::getJson(const String &path, DynamicJsonDocument &out, int *httpCode)
    {
        return requestJson("GET", path, nullptr, out, httpCode);
    }
    bool ApiClient::postJson(const String &path, const DynamicJsonDocument &body, DynamicJsonDocument &out, int *httpCode)
    {
        String payload;
        serializeJson(body, payload);
        return requestJson("POST", path, &payload, out, httpCode);
    }
     bool ApiClient::requestJson(const char *method, const String &path, const String *body, DynamicJsonDocument &out, int *codeOut)
    {
        return request(method, path, body, [&](Stream &s)
                       {
      DeserializationError err = deserializeJson(out, s);
      return !err; }, codeOut);
    }
    void ApiClient::prepareClient()
    {
        if (_opt.insecure)
        {
            _secure.setInsecure();
        }
        else if (_opt.ca_pem)
        {
            _secure.setCACert(_opt.ca_pem);
        }
    }

    void ApiClient::addStdHeaders(HTTPClient &http, bool hasBody)
    {
        http.addHeader("Accept", "application/json");
        if (hasBody)
            http.addHeader("Content-Type", "application/json");
        if (_bearer.length())
            http.addHeader("Authorization", "Bearer " + _bearer);

        // Custom default headers (one-per-line "Key: Value\r\n")
        if (_extraHeaders.length())
        {
            // HTTPClient has no bulk-headers API; parse & add a few common lines if you want.
            // Minimal approach: rely on addHeader above per call,
            // or keep _extraHeaders unused if not needed.
        }
    }

    String ApiClient::joinUrl(const String &path) const
    {
        if (path.length() && path[0] == '/')
            return _base + path;
        if (_base.endsWith("/"))
            return _base + path;
        return _base + "/" + path;
    }
}
