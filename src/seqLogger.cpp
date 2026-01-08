#include "seqLogger.h"

namespace DeskBuddy
{
    SeqLogger SeqLogger::seqLogger_;

    SeqLogger *SeqLogger::GetInstance()
    {
        return &seqLogger_;
    }

    void SeqLogger::log(const char *severity, const char *message, const char *component)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            // Not connected to WiFi, cannot log
            return;
        }
        HTTPClient http;
        http.begin(SEQ_URL);
        http.addHeader("Content-Type", "application/json");

        String jsonPayload =
            "{ \"Events\": [ {"
            "\"Timestamp\":\"" +
            iso8601UtcNow() + "\","
                              "\"Level\":\"" +
            String(severity) + "\","
                               "\"MessageTemplate\":\"" +
            jsonEscape(String(message)) + "\","
                                          "\"Properties\":{"
                                          "\"Component\":\"" +
            jsonEscape(String(component)) + "\""
                                            "}"
                                            "} ] }";

        int httpResponseCode = http.POST(jsonPayload);
        http.end();
    }

    String SeqLogger::iso8601UtcNow()
    {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            // fallback if time isn't set yet
            return "1970-01-01T00:00:00Z";
        }
        char buf[30];
        // Example: 2026-01-08T12:34:56Z
        strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
        return String(buf);
    }

    String SeqLogger::jsonEscape(const String &s)
    {
        String out;
        out.reserve(s.length() + 8);
        for (size_t i = 0; i < s.length(); i++)
        {
            char c = s[i];
            switch (c)
            {
            case '\\':
                out += "\\\\";
                break;
            case '"':
                out += "\\\"";
                break;
            case '\n':
                out += "\\n";
                break;
            case '\r':
                out += "\\r";
                break;
            case '\t':
                out += "\\t";
                break;
            default:
                out += c;
                break;
            }
        }
        return out;
    }
}