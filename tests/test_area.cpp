#include "area_data.h"

#include "config_manager.h"

#include <QtTest>

Q_DECLARE_METATYPE(AreaData::Side);

class test_Area : public QObject
{
  Q_OBJECT

private:
  ServerConfig *m_config;
  AreaData *m_area;

private Q_SLOTS:

  void init();

  void cleanup();

  void clientJoinLeave();

  void areaStatuses_data();

  void areaStatuses();

  void changeHP_data();

  void changeHP();

  void changeCharacter();

  void testimony();
};

void test_Area::init()
{
  m_config = new ServerConfig;
  m_area = new AreaData("Test Area", 0, nullptr, *m_config);
}

void test_Area::cleanup()
{
  m_area->deleteLater();
  m_config->deleteLater();
}

void test_Area::clientJoinLeave()
{
  {
    // There must be exactly one client in the area, and it must have a charid of 5 and userid 0.
    m_area->clientJoinedArea(5, 0);

    QCOMPARE(m_area->joinedIDs().size(), 1);
    QCOMPARE(m_area->charactersTaken().at(0), 5);
  }
  {
    // No clients must be left in the area.
    m_area->clientLeftArea(5, 0);

    QCOMPARE(m_area->joinedIDs().size(), 0);
  }
}

void test_Area::areaStatuses_data()
{
  QTest::addColumn<QString>("statusCall");
  QTest::addColumn<AreaData::Status>("expectedStatus");
  QTest::addColumn<bool>("isSuccessful");

  QTest::newRow("Idle") << "idle" << AreaData::Status::IDLE << true;
  QTest::newRow("RP") << "rp" << AreaData::Status::RP << true;
  QTest::newRow("Casing") << "casing" << AreaData::Status::CASING << true;
  QTest::newRow("Looking for players (long)") << "looking-for-players" << AreaData::Status::LOOKING_FOR_PLAYERS << true;
  QTest::newRow("Looking for players (short)") << "lfp" << AreaData::Status::LOOKING_FOR_PLAYERS << true;
  QTest::newRow("Gaming") << "gaming" << AreaData::Status::GAMING << true;
  QTest::newRow("Recess") << "recess" << AreaData::Status::RECESS << true;
  QTest::newRow("Nonsense") << "blah" << AreaData::Status::IDLE << false;
}

void test_Area::areaStatuses()
{
  QFETCH(QString, statusCall);
  QFETCH(AreaData::Status, expectedStatus);
  QFETCH(bool, isSuccessful);

  bool l_success = m_area->changeStatus(statusCall);

  QCOMPARE(m_area->status(), expectedStatus);
  QCOMPARE(l_success, isSuccessful);
}

void test_Area::changeHP_data()
{
  QTest::addColumn<AreaData::Side>("side");
  QTest::addColumn<int>("setHP");
  QTest::addColumn<int>("expectedHP");

  QTest::newRow("Set = Expected (DEF)") << AreaData::Side::DEFENCE << 3 << 3;
  QTest::newRow("Set = Expected (PRO)") << AreaData::Side::PROSECUTOR << 5 << 5;
  QTest::newRow("Below Zero (DEF)") << AreaData::Side::DEFENCE << -5 << 0;
  QTest::newRow("Below Zero (PRO)") << AreaData::Side::PROSECUTOR << -7 << 0;
  QTest::newRow("Above Ten (DEF)") << AreaData::Side::DEFENCE << 12 << 10;
  QTest::newRow("Above Ten (PRO)") << AreaData::Side::PROSECUTOR << 14 << 10;
}

void test_Area::changeHP()
{
  QFETCH(AreaData::Side, side);
  QFETCH(int, setHP);
  QFETCH(int, expectedHP);

  m_area->changeHP(side, setHP);

  if (AreaData::Side::DEFENCE == side)
  {
    QCOMPARE(expectedHP, m_area->defHP());
  }
  else
  {
    QCOMPARE(expectedHP, m_area->proHP());
  }
}

void test_Area::changeCharacter()
{
  {
    // A client with a charid of 6 and userid 0 joins. There's only them in there.
    m_area->clientJoinedArea(6, 0);

    QCOMPARE(m_area->charactersTaken().size(), 1);
    QCOMPARE(m_area->charactersTaken().at(0), 6);
  }
  {
    // Charid 7 is marked as taken. No other client in the area still.
    // Charids 6 and 7 are taken.
    m_area->changeCharacter(-1, 7);

    QCOMPARE(m_area->playerCount(), 1);
    QCOMPARE(m_area->charactersTaken().size(), 2);
    QCOMPARE(m_area->charactersTaken().at(0), 6);
    QCOMPARE(m_area->charactersTaken().at(1), 7);
  }
  {
    // Client switches to charid 8.
    // Charids 8 and 7 are taken.
    m_area->changeCharacter(6, 8);

    QCOMPARE(m_area->playerCount(), 1);
    QCOMPARE(m_area->charactersTaken().size(), 2);
    QCOMPARE(m_area->charactersTaken().at(0), 7);
    QCOMPARE(m_area->charactersTaken().at(1), 8);
  }
  {
    // Client attempts to switch to 7.
    // Nothing changes, as it is already taken.

    m_area->changeCharacter(8, 7);

    QCOMPARE(m_area->playerCount(), 1);
    QCOMPARE(m_area->charactersTaken().size(), 2);
    QCOMPARE(m_area->charactersTaken().at(0), 7);
    QCOMPARE(m_area->charactersTaken().at(1), 8);
  }
  {
    // Charid 7 is unlocked for use.
    // Charid 8 is taken.
    m_area->changeCharacter(7, -1);

    QCOMPARE(m_area->playerCount(), 1);
    QCOMPARE(m_area->charactersTaken().size(), 1);
    QCOMPARE(m_area->charactersTaken().at(0), 8);
  }
}

void test_Area::testimony()
{
  QVector<QStringList> l_testimony = {
      {"A"},
      {"B"},
      {"C"},
      {"D"},
      {"E"},
  };

  {
    // Add all statements, and check that they're added.
    for (const auto &l_statement : l_testimony)
    {
      m_area->recordStatement(l_statement);

      QCOMPARE(l_statement, m_area->testimony().at(m_area->statement() - 1));
    }
  }
  {
    // Restart testimony, advance two times.
    m_area->jumpToStatement(1);

    for (int i = 1; i < l_testimony.size() - 1; i++)
    {
      const auto &l_results = m_area->jumpToStatement(m_area->statement() + 1);

      QCOMPARE(l_results.first, l_testimony.at(i + 1));
      QCOMPARE(l_results.second, AreaData::TestimonyProgress::OK);
    }
  }
  {
    // Next advancement loops the testimony.
    const auto &l_results = m_area->jumpToStatement(m_area->statement() + 1);

    QCOMPARE(l_results.first, l_testimony.at(1));
    QCOMPARE(l_results.second, AreaData::TestimonyProgress::LOOPED);
  }
  {
    // Going back makes the testimony stay at the first statement.
    const auto &l_results = m_area->jumpToStatement(m_area->statement() - 1);

    QCOMPARE(l_results.first, l_testimony.at(1));
    QCOMPARE(l_results.second, AreaData::TestimonyProgress::STAYED_AT_FIRST);
  }
}

QTEST_APPLESS_MAIN(test_Area)

#include "test_area.moc"
