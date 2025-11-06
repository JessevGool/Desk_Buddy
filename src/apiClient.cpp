#include "ApiClient.h"

namespace DeskBuddy
{
    static ApiClient *g_instance = nullptr;

    // ---- Singleton init overloads ----
    void ApiClient::init()
    {
        if (!g_instance)
            g_instance = new ApiClient();
        g_instance->_base = String();
        g_instance->_opt = Options{};
    }

    void ApiClient::init(const String &baseUrl)
    {
        if (!g_instance)
            g_instance = new ApiClient();
        g_instance->_base = baseUrl;
        g_instance->_opt = Options{};
    }

    void ApiClient::init(const String &baseUrl, const Options &opt)
    {
        if (!g_instance)
            g_instance = new ApiClient();
        g_instance->_base = baseUrl;
        g_instance->_opt = opt;
    }

    ApiClient &ApiClient::instance()
    {
        if (!g_instance)
            init(); // safe default
        return *g_instance;
    }

    void ApiClient::setDefaultHeader(const String &key, const String &value)
    {
        _extraHeaders += key;
        _extraHeaders += F(": ");
        _extraHeaders += value;
        _extraHeaders += F("\r\n");
    }

    // Convenience overloads forward to full versions with default RequestParams()
    bool ApiClient::getJson(const String &urlOrPath, DynamicJsonDocument &out, int *httpCode)
    {
        return getJson(urlOrPath, out, httpCode, RequestParams());
    }

    bool ApiClient::postJson(const String &urlOrPath, const DynamicJsonDocument &body, DynamicJsonDocument &out, int *httpCode)
    {
        return postJson(urlOrPath, body, out, httpCode, RequestParams());
    }

    bool ApiClient::getJson(const String &urlOrPath, DynamicJsonDocument &out, int *httpCode, const RequestParams &rp)
    {
        return requestJson("GET", urlOrPath, nullptr, out, httpCode, rp);
    }

    bool ApiClient::postJson(const String &urlOrPath, const DynamicJsonDocument &body, DynamicJsonDocument &out, int *httpCode, const RequestParams &rp)
    {
        String payload;
        serializeJson(body, payload);
        return requestJson("POST", urlOrPath, &payload, out, httpCode, rp);
    }

    bool ApiClient::requestJson(const char *method,
                                const String &urlOrPath,
                                const String *body,
                                DynamicJsonDocument &out,
                                int *codeOut,
                                const RequestParams &rp)
    {
        String response;
        DeserializationError lastErr = DeserializationError::Ok;

        auto onStream = [&](Stream &s)
        {
            // Read the entire response body into a String for debugging
            while (s.available())
            {
                response += (char)s.read();
                // Optional safety limit:
                if (response.length() > 32768)
                    break;
            }

            // Try to parse the full response
            lastErr = deserializeJson(out, response);
            return !lastErr;
        };

        bool ok = request(method, urlOrPath, body, onStream, codeOut, rp);

        // Always print response if something went wrong
        if (!ok || lastErr)
        {
            Serial.println(F("---- Raw response start ----"));
            Serial.println(response);
            Serial.println(F("---- Raw response end ----"));
            Serial.printf("JSON parse error: %s\n", lastErr.c_str());
        }

        return ok;
    }

    void ApiClient::prepareClient()
    {
        if (_opt.insecure)
            _secure.setInsecure();
        else if (_opt.ca_pem)
            _secure.setCACert(_opt.ca_pem);
    }

    void ApiClient::addStdHeaders(HTTPClient &http, bool hasBody, const RequestParams &rp)
    {
        if (hasBody)
            http.addHeader(F("Content-Type"), F("application/json"));

        // global defaults
        int start = 0;
        while (start < _extraHeaders.length())
        {
            int end = _extraHeaders.indexOf('\n', start);
            if (end < 0)
                end = _extraHeaders.length();
            String line = _extraHeaders.substring(start, end);
            int colon = line.indexOf(':');
            if (colon > 0)
            {
                String k = line.substring(0, colon);
                String v = line.substring(colon + 1);
                v.trim();
                http.addHeader(k, v);
            }
            start = end + 1;
        }

        // opt-in auth
        if (rp.useBearer)
        {
            if (rp.bearer && rp.bearer[0] != '\0')
                http.addHeader(F("Authorization"), String(F("Bearer ")) + rp.bearer);
            else if (_bearer.length())
                http.addHeader(F("Authorization"), String(F("Bearer ")) + _bearer);
        }

        // per-request headers
        int s2 = 0;
        while (s2 < rp.extraHeaders.length())
        {
            int e2 = rp.extraHeaders.indexOf('\n', s2);
            if (e2 < 0)
                e2 = rp.extraHeaders.length();
            String line = rp.extraHeaders.substring(s2, e2);
            int colon = line.indexOf(':');
            if (colon > 0)
            {
                String k = line.substring(0, colon);
                String v = line.substring(colon + 1);
                v.trim();
                http.addHeader(k, v);
            }
            s2 = e2 + 1;
        }
    }

    bool ApiClient::isAbsoluteUrl(const String &s) const
    {
        return s.startsWith(F("http://")) || s.startsWith(F("https://"));
    }

    String ApiClient::resolveUrl(const String &urlOrPath) const
    {
        if (isAbsoluteUrl(urlOrPath))
            return urlOrPath;
        return joinUrl(urlOrPath);
    }

    String ApiClient::joinUrl(const String &path) const
    {
        if (_base.length() == 0)
            return path;
        if (path.length() == 0)
            return _base;

        const bool baseEndsSlash = _base.endsWith("/");
        const bool pathStartsSlash = path.startsWith("/");

        if (baseEndsSlash && pathStartsSlash)
            return _base + path.substring(1);
        if (!baseEndsSlash && !pathStartsSlash)
            return _base + "/" + path;
        return _base + path;
    }

    bool ApiClient::beginForUrl(HTTPClient &http, const String &url)
    {
        if (url.startsWith(F("https://")))
            return http.begin(_secure, url);
        if (url.startsWith(F("http://")))
            return http.begin(_plain, url);
        // fallback attempt after join
        String r = resolveUrl(url);
        if (r.startsWith(F("https://")))
            return http.begin(_secure, r);
        if (r.startsWith(F("http://")))
            return http.begin(_plain, r);
        return false;
    }

    // Template specialization is in the header, but we need to make sure
    // the implementation actually works. The template in the header should handle this.
}
