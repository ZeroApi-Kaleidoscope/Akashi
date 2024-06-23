#include "akashiutils.h"

#include <QTest>

class test_AkashiUtils : public QObject
{
  Q_OBJECT

private Q_SLOTS:
  void integer_data();
  void integer();

  void floating_data();
  void floating();

  void boolean_data();
  void boolean();

  void doublep_data();
  void doublep();
};

void test_AkashiUtils::integer_data()
{
  QTest::addColumn<QString>("content");
  QTest::addColumn<bool>("expected_result");

  QTest::addRow("Integer (good)") << "one" << false;
  QTest::addRow("Integer (bad)") << "1" << true;
}

void test_AkashiUtils::integer()
{
  QFETCH(QString, content);
  QFETCH(bool, expected_result);

  bool result = AkashiUtils::checkArgType<int>(content);
  QCOMPARE(result, expected_result);
}

void test_AkashiUtils::floating_data()
{
  QTest::addColumn<QString>("content");
  QTest::addColumn<bool>("expected_result");

  QTest::addRow("Float (good)") << "test" << false;
  QTest::addRow("Float (bad)") << "3.141" << true;
}

void test_AkashiUtils::floating()
{
  QFETCH(QString, content);
  QFETCH(bool, expected_result);

  bool result = AkashiUtils::checkArgType<float>(content);
  QCOMPARE(result, expected_result);
}

void test_AkashiUtils::boolean_data()
{
  QTest::addColumn<QString>("content");
  QTest::addColumn<bool>("expected_result");

  QTest::addRow("Boolean (random string)") << "test" << true;
  QTest::addRow("Boolean (true/false string)") << "true" << true;
}

void test_AkashiUtils::boolean()
{
  QFETCH(QString, content);
  QFETCH(bool, expected_result);

  bool result = AkashiUtils::checkArgType<bool>(content);
  QCOMPARE(result, expected_result);
}

void test_AkashiUtils::doublep_data()
{
  QTest::addColumn<QString>("content");
  QTest::addColumn<bool>("expected_result");

  QTest::addRow("Double (good)") << "test" << false;
  QTest::addRow("Double (bad)") << "3.141592653589793" << true;
}

void test_AkashiUtils::doublep()
{
  QFETCH(QString, content);
  QFETCH(bool, expected_result);

  bool result = AkashiUtils::checkArgType<double>(content);
  QCOMPARE(result, expected_result);
}

QTEST_APPLESS_MAIN(test_AkashiUtils)

#include "test_akashi_utils.moc"
