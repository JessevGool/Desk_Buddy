#include "models/mcServerModel.h"
namespace DeskBuddy
{

    void McServerModel::parseMcServerStatus(const DynamicJsonDocument &doc)
    {
        this->ip = doc["ip"] | "";
        this->port = doc["port"] | 0;
        this->online = doc["online"] | false;

        if (!this->online)
            return; // offline case done

        // Online-only fields
        this->hostname = doc["hostname"] | "";
        this->version = doc["version"] | "";

        JsonVariantConst players = doc["players"];
        if (!players.isNull())
        {
            this->playersOnline = players["online"] | -1;
            this->playersMax = players["max"] | -1;
        }

        JsonVariantConst motd = doc["motd"]["clean"];
        if (motd.is<JsonArrayConst>())
        {
            this->motdLine1 = motd[0] | "";
            this->motdLine2 = motd[1] | "";
        }
        else
        {
            // fallback if 'clean' missing: try 'raw'
            JsonVariantConst raw = doc["motd"]["raw"];
            if (raw.is<JsonArrayConst>())
            {
                this->motdLine1 = raw[0] | "";
                this->motdLine2 = raw[1] | "";
            }
        }

        JsonVariantConst proto = doc["protocol"];
        if (!proto.isNull())
        {
            this->protocolVersion = proto["version"] | 0;
            this->protocolName = proto["name"] | "";
        }

        this->hasIcon = !doc["icon"].isNull();

        return;
    }
}