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

        DynamicJsonDocument doc(16384); // or use filter (shown below)
        int code = 0;

        bool ok = _client.getJson(
            "https://api.mcsrvstat.us/3/play.cubecraft.net",
            doc, &code);

        if (!ok)
        {
            display.println("Request failed!");
            return;
        }

        if (doc.size() == 0)
        {
            display.println("JSON document is empty!");
            return;
        }

        String jsonString;
        size_t len = serializeJson(doc, jsonString);
        McServerModel status;
        status.parseMcServerStatus(doc);
        display.printf("IP: %s\n", status.ip.c_str());
        display.printf("Port: %u\n", status.port);
        display.printf("Online: %s\n", status.online ? "Yes" : "No");
        display.printf("Players: %d/%d\n", status.playersOnline, status.playersMax);
        
        ProgressBar pb = ProgressBar(display, 50, 150, 200, 20);
        int playerPercentage = status.playersOnline * 100 / status.playersMax;
        pb.setProgress(playerPercentage);
        pb.draw();
    }
}
