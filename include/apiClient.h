// ApiClient.h (put in a tab/file)
#include <WiFi.h>
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
        uint8_t retries = 2;  // simple retry count
        bool insecure = true; // set false + provide CA for production
        const char *ca_pem = nullptr;
        
    };

    ApiClient(String baseUrl, Options opt) : _base(baseUrl), _opt(opt) {}

    void setBearer(String token);
    void setDefaultHeader(String key, String value);

    // GET -> JSON
    bool getJson(const String &path, DynamicJsonDocument &out, int *httpCode = nullptr);

    // POST JSON -> JSON
    bool postJson(const String &path, const DynamicJsonDocument &body, DynamicJsonDocument &out, int *httpCode = nullptr);

    // Generic (stream handler). Handy for large downloads.
    template <typename Fn>
    bool request(const char *method, const String &path, const String *rawBody, Fn onStream, int *httpCode = nullptr)
    {
        prepareClient();
        HTTPClient http;
        String url = joinUrl(path);
        http.setTimeout(_opt.timeout_ms);
        bool begun = http.begin(_secure, url);
        if (!begun)
            return false;

        addStdHeaders(http, rawBody != nullptr);

        int code = -1;
        for (uint8_t attempt = 0; attempt <= _opt.retries; ++attempt)
        {
            code = rawBody ? http.sendRequest(method, *rawBody) : http.sendRequest(method);
            if (code > 0)
                break;                  // exit on success or server error
            delay(150 * (attempt + 1)); // tiny backoff on transport errors
        }
        if (httpCode)
            *httpCode = code;

        bool ok = (code >= 200 && code < 300);
        if (ok)
        {
            Stream &s = http.getStream();
            ok = onStream(s);
        }
        else if (code > 0)
        {
            // Optional: read error body if you want: String err = http.getString();
        }
        http.end();
        return ok;
    }

private:
    bool requestJson(const char *method, const String &path, const String *body, DynamicJsonDocument &out, int *codeOut);

    void prepareClient();

    void addStdHeaders(HTTPClient &http, bool hasBody);

    String joinUrl(const String &path) const;

    String _base;
    String _bearer;
    String _extraHeaders;
    WiFiClientSecure _secure;
    Options _opt;
};
}