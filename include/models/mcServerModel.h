#pragma once
#include "apiClient.h"

namespace DeskBuddy
{

    class McServerModel
    {
    public:
        McServerModel() = default;
        void parseMcServerStatus(const DynamicJsonDocument &doc);
        bool isValid() const { return !ip.isEmpty() && port != 0; }
        String ip;
        uint16_t port;
        bool online = false;
        String hostname;
        String version; // e.g. "We support: 1.20-1.21"
        int playersOnline = -1;
        int playersMax = -1;
        String motdLine1, motdLine2; // from motd.clean[0..1]
        int protocolVersion = 0;     // protocol.version
        String protocolName;         // protocol.name
        bool hasIcon = false;        // icon field present?
    };
} // namespace DeskBuddy
