#include "packet/packet_zz.h"

#include "area_data.h"
#include "config_manager.h"
#include "packet/packet_factory.h"
#include "server.h"

#include <QQueue>

PacketZZ::PacketZZ(QStringList &contents) :
    AOPacket(contents)
{
}

PacketInfo PacketZZ::getPacketInfo() const
{
    PacketInfo info{
        .acl_permission = ACLRole::Permission::NONE,
        .min_args = 0,
        .header = "ZZ"};
    return info;
}

void PacketZZ::handlePacket(AreaData *area, AOClient &client) const
{
    QString l_name = client.name();
    if (client.name().isEmpty())
        l_name = client.character();

    QString l_areaName = area->name();

    QString l_modcallNotice = "!!!MODCALL!!!\nArea: " + l_areaName + "\nCaller: " + l_name + "\n";

    if (m_content.size() > 0 && !m_content[0].isEmpty())
        l_modcallNotice.append("Reason: " + m_content[0]);
    else
        l_modcallNotice.append("No reason given.");

    const QVector<AOClient *> l_clients = client.getServer()->getClients();
    for (AOClient *l_client : l_clients) {
        if (l_client->m_authenticated)
            l_client->sendPacket(PacketFactory::createPacket("ZZ", {l_modcallNotice}));
    }
    Q_EMIT client.logModcall((client.character() + " " + client.characterName()), client.m_ipid, client.name(), client.getServer()->getAreaById(client.areaId())->name());

    if (ConfigManager::discordModcallWebhookEnabled()) {
        QString l_name = client.name();
        if (client.name().isEmpty())
            l_name = client.character();

        QString l_areaName = area->name();
        Q_EMIT client.getServer()->modcallWebhookRequest(l_name, l_areaName, m_content.value(0), client.getServer()->getAreaBuffer(l_areaName));
    }
}
