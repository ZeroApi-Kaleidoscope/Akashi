#include "config_manager.h"

#include <QString>
#include <QTest>

class test_ConfigManager : public QObject
{
    Q_OBJECT

    typedef QMap<QString, QPair<QString, int>> MusicList;

  private:
    ConfigManager *m_config = nullptr;

  private Q_SLOTS:
    void init() { m_config = new ConfigManager; }

    void cleanup() { delete m_config; }

    void verifyServerConfig();

    void bindIP();

    void charlist();

    void backgrounds();

    void musiclist();

    void ordered_songs();

    /**
     * @brief Tests the loading routine for the musiclist.
     *
     * @details Handles regression testing for a bug fixed in PR#314 in which a
     * musiclist would be loaded twice and the old root instance was not cleared,
     * causing each reload to append the musiclist to the old list.
     */
    void regression_pr_314();

    void CommandInfo();

    void iprangeBans();

    void maxPlayers();

    void serverPort();

    void serverDescription();

    void serverName();

    void motd();

    void webaoEnabled();

    void webaoPort();

    void authType();

    void modpass();

    void logBuffer();

    void loggingType();

    void maxStatements();

    void multiClientLimit();

    void maxCharacters();

    void messageFloodguard();

    void globalMessageFloodguard();

    void assetUrl();

    void diceMaxValue();

    void diceMaxDice();

    void discordWebhookEnabled();

    void discordModcallWebhookEnabled();

    void discordModcallWebhookUrl();

    void discordModcallWebhookContent();

    void discordModcallWebhookSendFile();

    void discordBanWebhookEnabled();

    void discordBanWebhookUrl();

    void discordUptimeEnabled();

    void discordUptimeTime();

    void discordUptimeWebhookUrl();

    void discordWebhookColor();

    void passwordRequirements();

    void passwordMinLength();

    void passwordMaxLength();

    void passwordRequireMixCase();

    void passwordRequireNumbers();

    void passwordRequireSpecialCharacters();

    void passwordCanContainUsername();

    void afkTimeout();

    void magic8BallAnswers();

    void praiseList();

    void reprimandsList();

    void gimpList();

    void cdnList();

    void advertiseServer();

    void advertiserDebug();

    void advertiserIP();

    void advertiserHostname();

    void advertiserCloudflareMode();
};

/**
 * @brief Tests if the config folder is complete. Fails when a config file is missing.
 */
void test_ConfigManager::verifyServerConfig()
{
    // If the sample folder is not renamed or a file is missing, we fail the test.
    QCOMPARE(ConfigManager::verifyServerConfig(), true);

    // We remove a config file and test again. This should now fail as cdns.txt is missing.
    QCOMPARE(QFile("config/text/cdns.txt").remove(), true);
    QCOMPARE(ConfigManager::verifyServerConfig(), false);

    // We rebuild the file.
    QFile cdns_config("config/text/cdns.txt");
    if (cdns_config.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream write_stream(&cdns_config);
        write_stream << "cdn.discord.com";
        cdns_config.close();
    }
    else {
        qDebug() << "Unable to recreate cdns config file.";
    }
}

/**
 * @brief Retrieves the IPs the servers binds to in string format
 */
void test_ConfigManager::bindIP()
{
    QCOMPARE(ConfigManager::bindIP(), "all");
}

void test_ConfigManager::charlist()
{
    // We check that the list is unsorted and exactly as defined in the text file.
    QStringList l_characters = ConfigManager::charlist();
    QCOMPARE(l_characters.at(0), "Zak");
    QCOMPARE(l_characters.at(1), "Adrian");
    QCOMPARE(l_characters.at(2), "Armstrong");
    QCOMPARE(l_characters.at(3), "Butz");
    QCOMPARE(l_characters.at(4), "Diego");
}

void test_ConfigManager::backgrounds()
{
    // We check that the list is unsorted and exactly as defined in the text file.
    QStringList l_backgrounds = ConfigManager::backgrounds();

    QCOMPARE(l_backgrounds.at(0), "Anime");
    QCOMPARE(l_backgrounds.at(1), "Zetta");
    QCOMPARE(l_backgrounds.at(2), "default");
    QCOMPARE(l_backgrounds.at(3), "birthday");
    QCOMPARE(l_backgrounds.at(4), "Christmas");
}

void test_ConfigManager::musiclist()
{
    MusicList l_musiclist = ConfigManager::musiclist();
    qDebug() << l_musiclist;
    QPair<QString, int> l_contents;
    // The musiclist is structured as {DisplayName , {Songname, Duration}}.

    // Categories have no duration and their alias is always the category name.
    l_contents = l_musiclist.value("==Samplelist==");
    QCOMPARE(l_contents.first, "==Samplelist==");
    QCOMPARE(l_contents.second, 0);

    // The displayname is shown in the musiclist. The realname is what is send to the client when it wants to play the song.
    l_contents = l_musiclist.value("Announce The Truth (JFA).opus");
    QCOMPARE(l_contents.first, "https://localhost/Announce The Truth (JFA).opus");
    QCOMPARE(l_contents.second, 98);
}

void test_ConfigManager::ordered_songs()
{
    QStringList l_ordered_musiclist = ConfigManager::ordered_songs();
    QCOMPARE(l_ordered_musiclist.at(0), "==Samplelist==");
    QCOMPARE(l_ordered_musiclist.at(1), "Announce The Truth (AA).opus");
    QCOMPARE(l_ordered_musiclist.at(2), "Announce The Truth (AJ).opus");
    QCOMPARE(l_ordered_musiclist.at(3), "Announce The Truth (JFA).opus");
}

void test_ConfigManager::regression_pr_314()
{
    // Populate songlist.
    Q_UNUSED(ConfigManager::musiclist());

    // Tests for regression where a reload of the songlist would cause the list to duplicate.
    QStringList l_list = ConfigManager::ordered_songs();

    // We have a populated ordered list and it has a valid size.
    QCOMPARE(l_list.isEmpty(), false);
    QCOMPARE(l_list.size(), 4);

    // We are reloading the songlist. The size should be the same and the list has not changed.
    Q_UNUSED(ConfigManager::musiclist());
    QCOMPARE(l_list.size(), ConfigManager::ordered_songs().size());
    QCOMPARE(l_list, ConfigManager::ordered_songs());
}

void test_ConfigManager::CommandInfo()
{
    // Prepare command help cache.
    ConfigManager::loadCommandHelp();
    ConfigManager::help l_help;

    // Load the sample command information.
    l_help = ConfigManager::commandHelp("foo");
    QCOMPARE(l_help.text, "A sample explanation.");
    QCOMPARE(l_help.usage, "/foo <bar> [baz|qux]");

    l_help = ConfigManager::commandHelp("login");
    QCOMPARE(l_help.text, "Activates the login dialogue to enter your credentials. This command takes no arguments.");
    QCOMPARE(l_help.usage, "/login");
}

void test_ConfigManager::iprangeBans()
{
    QStringList l_ipranges = ConfigManager::iprangeBans();
    QCOMPARE(l_ipranges.at(0), "192.0.2.0/24");
    QCOMPARE(l_ipranges.at(1), "198.51.100.0/24");
}

void test_ConfigManager::maxPlayers()
{}

void test_ConfigManager::serverPort()
{}

void test_ConfigManager::serverDescription()
{}

void test_ConfigManager::serverName()
{}

void test_ConfigManager::motd()
{}

void test_ConfigManager::webaoEnabled()
{}

void test_ConfigManager::webaoPort()
{}

void test_ConfigManager::authType()
{}

void test_ConfigManager::modpass()
{}

void test_ConfigManager::logBuffer()
{}

void test_ConfigManager::loggingType()
{}

void test_ConfigManager::maxStatements()
{}

void test_ConfigManager::multiClientLimit()
{}

void test_ConfigManager::maxCharacters()
{}

void test_ConfigManager::messageFloodguard()
{}

void test_ConfigManager::globalMessageFloodguard()
{}

void test_ConfigManager::assetUrl()
{}

void test_ConfigManager::diceMaxValue()
{}

void test_ConfigManager::diceMaxDice()
{}

void test_ConfigManager::discordWebhookEnabled()
{}

void test_ConfigManager::discordModcallWebhookEnabled()
{}

void test_ConfigManager::discordModcallWebhookUrl()
{}

void test_ConfigManager::discordModcallWebhookContent()
{}

void test_ConfigManager::discordModcallWebhookSendFile()
{}

void test_ConfigManager::discordBanWebhookEnabled()
{}

void test_ConfigManager::discordBanWebhookUrl()
{}

void test_ConfigManager::discordUptimeEnabled()
{}

void test_ConfigManager::discordUptimeTime()
{}

void test_ConfigManager::discordUptimeWebhookUrl()
{}

void test_ConfigManager::discordWebhookColor()
{}

void test_ConfigManager::passwordRequirements()
{}

void test_ConfigManager::passwordMinLength()
{}

void test_ConfigManager::passwordMaxLength()
{}

void test_ConfigManager::passwordRequireMixCase()
{}

void test_ConfigManager::passwordRequireNumbers()
{}

void test_ConfigManager::passwordRequireSpecialCharacters()
{}

void test_ConfigManager::passwordCanContainUsername()
{}

void test_ConfigManager::afkTimeout()
{}

void test_ConfigManager::magic8BallAnswers()
{}

void test_ConfigManager::praiseList()
{}

void test_ConfigManager::reprimandsList()
{}

void test_ConfigManager::gimpList()
{}

void test_ConfigManager::cdnList()
{}

void test_ConfigManager::advertiseServer()
{}

void test_ConfigManager::advertiserDebug()
{}

void test_ConfigManager::advertiserIP()
{}

void test_ConfigManager::advertiserHostname()
{}

void test_ConfigManager::advertiserCloudflareMode()
{}

QTEST_APPLESS_MAIN(test_ConfigManager)

#include "test_config_manager.moc"
