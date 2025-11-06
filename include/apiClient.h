#pragma once
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

namespace DeskBuddy
{
    class ApiClient
    {
    public:
        struct Options
        {
            uint16_t timeout_ms = 7000;
            uint8_t  retries    = 2;
            bool     insecure   = true;
            const char *ca_pem  = nullptr;
        };

        struct RequestParams
        {
            bool useBearer      = false;    // opt-in
            const char *bearer  = nullptr;  // one-off override
            String extraHeaders;            // "Key: Value\r\n"
        };

        // ---- Singleton ----
        static void init();                                    // no base, defaults
        static void init(const String &baseUrl);               // base only
        static void init(const String &baseUrl, const Options &opt); // full

        static ApiClient &instance();

        // ---- Instance API ----
        void setBaseUrl(const String &baseUrl) { _base = baseUrl; }
        void setOptions(const Options &opt)     { _opt = opt; }
        void setBearer(const String &token)     { _bearer = token; }
        void setDefaultHeader(const String &key, const String &value);

        // GET -> JSON
        bool getJson(const String &urlOrPath,
                     DynamicJsonDocument &out,
                     int *httpCode); // convenience (no RequestParams)

        bool getJson(const String &urlOrPath,
                     DynamicJsonDocument &out,
                     int *httpCode,
                     const RequestParams &rp); // full control

        // POST JSON -> JSON
        bool postJson(const String &urlOrPath,
                      const DynamicJsonDocument &body,
                      DynamicJsonDocument &out,
                      int *httpCode); // convenience

        bool postJson(const String &urlOrPath,
                      const DynamicJsonDocument &body,
                      DynamicJsonDocument &out,
                      int *httpCode,
                      const RequestParams &rp); // full control

        // Generic (stream handler)
        template <typename Fn>
        bool request(const char *method,
                     const String &urlOrPath,
                     const String *rawBody,
                     Fn onStream,
                     int *httpCode) // convenience wrapper
        {
            return request(method, urlOrPath, rawBody, onStream, httpCode, RequestParams());
        }

        template <typename Fn>
        bool request(const char *method,
                     const String &urlOrPath,
                     const String *rawBody,
                     Fn onStream,
                     int *httpCode,
                     const RequestParams &rp) // full control
        {
            prepareClient();

            HTTPClient http;
            const String url = resolveUrl(urlOrPath);
            http.setTimeout(_opt.timeout_ms);

            if (!beginForUrl(http, url)) return false;

            addStdHeaders(http, rawBody != nullptr, rp);

            int code = -1;
            for (uint8_t attempt = 0; attempt <= _opt.retries; ++attempt)
            {
                code = rawBody ? http.sendRequest(method, *rawBody) : http.sendRequest(method);
                if (code > 0) break;
                delay(150 * (attempt + 1));
            }
            if (httpCode) *httpCode = code;

            bool ok = (code >= 200 && code < 300);
            if (ok)
            {
                Stream &s = http.getStream();
                ok = onStream(s);
            }
            http.end();
            return ok;
        }

    private:
        ApiClient() = default;
        ApiClient(const ApiClient&) = delete;
        ApiClient& operator=(const ApiClient&) = delete;

        bool requestJson(const char *method,
                         const String &urlOrPath,
                         const String *body,
                         DynamicJsonDocument &out,
                         int *codeOut,
                         const RequestParams &rp);

        void prepareClient();
        void addStdHeaders(HTTPClient &http, bool hasBody, const RequestParams &rp);

        String resolveUrl(const String &urlOrPath) const;
        bool   isAbsoluteUrl(const String &s) const;
        String joinUrl(const String &path) const;
        bool   beginForUrl(HTTPClient &http, const String &url);

        String _base;
        String _bearer;
        String _extraHeaders;
        WiFiClient _plain;
        WiFiClientSecure _secure;
        Options _opt;
    };
}
