#pragma once
#include <Arduino.h>
#include <HTTPClient.h>

#include "secrets.h"

namespace DeskBuddy
{
    class SeqLogger
    {

    protected:
        SeqLogger() = default;
        ~SeqLogger() = default;
        static SeqLogger seqLogger_;

    public:

        SeqLogger(SeqLogger &other) = delete;
        void operator=(const SeqLogger &) = delete;
        static SeqLogger *GetInstance();

        static void log(const char* severity, const char* message, const char* component);
    private:
        static String iso8601UtcNow();
        static String jsonEscape(const String &s); 
    };
}