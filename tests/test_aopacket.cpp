#include "network/aopacket.h"
#include "network/packet_factory.h"

#include <QObject>
#include <QTest>

/**
 * @brief Unit Tester class for the area-related functions.
 */
class test_AOPacket : public QObject
{
  Q_OBJECT

private Q_SLOTS:
  void init();

  void createPacket();

  void createPacketFromString_data();

  void createPacketFromString();

  void createPacketSubclass_data();

  void createPacketSubclass();
};

void test_AOPacket::init()
{
  AOPacket::registerPackets();
}

void test_AOPacket::createPacket()
{
  AOPacket *packet = PacketFactory::createPacket("HI", {"HDID"});
  QCOMPARE(packet->getPacketInfo().header, "HI");
  QCOMPARE(packet->getContent(), {"HDID"});
}

void test_AOPacket::createPacketFromString_data()
{
  QTest::addColumn<QString>("incoming_packet");
  QTest::addColumn<QString>("expected_header");
  QTest::addColumn<QStringList>("expected_content");

  QTest::newRow("No Escaped fields") << "HI#1234#"
                                     << "HI" << QStringList{"1234"};

  QTest::newRow("Multiple fields") << "ID#34#Akashi#"
                                   << "ID" << QStringList{"34", "Akashi"};

  QTest::newRow("Encoded fields") << "MC#[T<and>T]Objection.opus#0#oldmud0#-1#0#0#"
                                  << "MC" << QStringList{"[T&T]Objection.opus", "0", "oldmud0", "-1", "0", "0"};

  QTest::newRow("Sequence of encoded characters") << "UNIT#<and><and><percent><num><percent><dollar>#"
                                                  << "UNIT" << QStringList{"&&%#%$"};

  QTest::newRow("Unescaped characters") << "MC#20% Cooler#"
                                        << "Unknown" << QStringList{"Unknown"};

  QTest::newRow("Empty packet") << ""
                                << "Unknown" << QStringList{"Unknown"};
  QTest::newRow("Bogus Packet - PR 328") << "ZZ#%@%#@^#@&^#@$^@&$^*@&$*@^$&*@$@^$&*@^$&#^&#@$#%"
                                         << "Unknown" << QStringList{"Unknown"};
}

void test_AOPacket::createPacketFromString()
{
  QFETCH(QString, incoming_packet);
  QFETCH(QString, expected_header);
  QFETCH(QStringList, expected_content);

  AOPacket *packet = PacketFactory::createPacket(incoming_packet);
  QCOMPARE(packet->getPacketInfo().header, expected_header);
  QCOMPARE(packet->getContent(), expected_content);
}

void test_AOPacket::createPacketSubclass_data()
{
  QTest::addColumn<QString>("incoming_packet");
  QTest::addColumn<QString>("expected_header");
  QTest::addColumn<int>("expected_minargs");

  QTest::newRow("askchaa") << "askchaa#"
                           << "askchaa" << 0;
  QTest::newRow("CASEA") << "CASEA#"
                         << "CASEA" << 6;
  QTest::newRow("CC") << "CC#"
                      << "CC" << 3;
  QTest::newRow("CH") << "CH#"
                      << "CH" << 1;
  QTest::newRow("CT") << "CT#"
                      << "CT" << 2;
  QTest::newRow("DE") << "DE#"
                      << "DE" << 1;
  QTest::newRow("EE") << "EE#"
                      << "EE" << 4;
  QTest::newRow("GENERIC") << "GENERIC#"
                           << "GENERIC" << 0;
  QTest::newRow("HI") << "HI#"
                      << "HI" << 1;
  QTest::newRow("HP") << "HP#"
                      << "HP" << 2;
  QTest::newRow("ID") << "ID#"
                      << "ID" << 2;
  QTest::newRow("MC") << "MC#"
                      << "MC" << 2;
  QTest::newRow("MS") << "MS#"
                      << "MS" << 15;
  QTest::newRow("PE") << "PE#"
                      << "PE" << 3;
  QTest::newRow("PW") << "PW#"
                      << "PW" << 1;
  QTest::newRow("RC") << "RC#"
                      << "RC" << 0;
  QTest::newRow("RD") << "RD#"
                      << "RD" << 0;
  QTest::newRow("RM") << "RM#"
                      << "RM" << 0;
  QTest::newRow("RT") << "RT#"
                      << "RT" << 1;
  QTest::newRow("SETCASE") << "SETCASE#"
                           << "SETCASE" << 7;
  QTest::newRow("ZZ") << "ZZ#"
                      << "ZZ" << 0;
}

void test_AOPacket::createPacketSubclass()
{
  QFETCH(QString, incoming_packet);
  QFETCH(QString, expected_header);
  QFETCH(int, expected_minargs);

  AOPacket *packet = PacketFactory::createPacket(incoming_packet);
  QCOMPARE(packet->getPacketInfo().header, expected_header);
  QCOMPARE(packet->getPacketInfo().min_args, expected_minargs);
}

QTEST_APPLESS_MAIN(test_AOPacket)

#include "test_aopacket.moc"
