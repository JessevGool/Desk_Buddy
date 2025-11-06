#include "pages/minecraftServerInfoPage.h"

namespace DeskBuddy
{
    void MinecraftServerInfoPage::draw(Adafruit_ILI9341 &display)
    {
        drawTitle(display);
        display.setTextSize(1);
        display.setCursor(0, 30);
        display.println(F("Minecraft Server Info"));
        display.println(F("This is the second page!"));
        display.println(F("Use joystick right to switch"));
        display.println(F("back to Main page."));

        DeskBuddy::ApiClient::RequestParams rp;
        rp.extraHeaders =
            "User-Agent: DeskBuddy/1.0\r\n"
            "Accept: application/json\r\n"
            "Accept-Encoding: identity\r\n"; // <-- disable gzip/br

        DynamicJsonDocument doc(16384); // or use filter (shown below)
        int code = 0;

        bool ok = _client.getJson(
            "https://api.mcsrvstat.us/3/77.175.237.225:25565",
            doc, &code, rp);

        display.printf("HTTP Code: %d\n", code);
        display.printf(doc.as<String>().c_str());
    }
}
