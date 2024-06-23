#include "acl_roles_handler.h"

#include <QDebug>
#include <QFile>
#include <QTest>

class test_ACLRolesHandler : public QObject
{
  Q_OBJECT

private:
  ACLRolesHandler *m_handler;

private Q_SLOTS:
  void init()
  {
    m_handler = new ACLRolesHandler;
    m_handler->insertRole(QString("FakeRole"), ACLRole(ACLRole::KICK));
  }

  void cleanup()
  {
    m_handler->deleteLater();
    m_handler = nullptr;
  }

  void checkReadOnlyRole_data()
  {
    QTest::addColumn<QString>("role_id");
    QTest::addColumn<QMap<ACLRole::Permission, bool>>("permissions");

    QTest::newRow("Default Role") << ACLRolesHandler::NONE_ID << QMap<ACLRole::Permission, bool>{{ACLRole::NONE, true},     {ACLRole::KICK, false}, {ACLRole::BAN, false},  {ACLRole::BGLOCK, false},   {ACLRole::MODIFY_USERS, false},     {ACLRole::CM, false},           {ACLRole::GLOBAL_TIMER, false},  {ACLRole::EVI_MOD, false},     {ACLRole::MOTD, false},    {ACLRole::ANNOUNCE, false},
                                                                                                 {ACLRole::MODCHAT, false}, {ACLRole::MUTE, false}, {ACLRole::UNCM, false}, {ACLRole::SAVETEST, false}, {ACLRole::FORCE_CHARSELECT, false}, {ACLRole::BYPASS_LOCKS, false}, {ACLRole::IGNORE_BGLIST, false}, {ACLRole::SEND_NOTICE, false}, {ACLRole::JUKEBOX, false}, {ACLRole::SUPER, false}};
    QTest::newRow("Super Role") << ACLRolesHandler::SUPER_ID << QMap<ACLRole::Permission, bool>{{ACLRole::NONE, true},    {ACLRole::KICK, true}, {ACLRole::BAN, true},  {ACLRole::BGLOCK, true},   {ACLRole::MODIFY_USERS, true},     {ACLRole::CM, true},           {ACLRole::GLOBAL_TIMER, true},  {ACLRole::EVI_MOD, true},     {ACLRole::MOTD, true},    {ACLRole::ANNOUNCE, true},
                                                                                                {ACLRole::MODCHAT, true}, {ACLRole::MUTE, true}, {ACLRole::UNCM, true}, {ACLRole::SAVETEST, true}, {ACLRole::FORCE_CHARSELECT, true}, {ACLRole::BYPASS_LOCKS, true}, {ACLRole::IGNORE_BGLIST, true}, {ACLRole::SEND_NOTICE, true}, {ACLRole::JUKEBOX, true}, {ACLRole::SUPER, true}};
  }

  void checkReadOnlyRole()
  {
    using Permissions = QMap<ACLRole::Permission, bool>;
    QFETCH(QString, role_id);
    QFETCH(Permissions, permissions);

    QCOMPARE(m_handler->roleExists(role_id), true);

    ACLRole role = m_handler->getRoleById(role_id);
    for (auto it = permissions.begin(); it != permissions.end(); ++it)
    {
      QCOMPARE(role.checkPermission(it.key()), it.value());
    }
  }

  void addRole_data()
  {
    QTest::addColumn<QString>("role_id");
    QTest::addColumn<bool>("result");

    QTest::newRow("Add role") << QString("NewRole") << true;
    QTest::newRow("Fail to add role") << QString("FakeRole") << false;
    QTest::newRow("Fail to add read-only role") << ACLRolesHandler::SUPER_ID << false;
  }

  void addRole()
  {
    QFETCH(QString, role_id);
    QFETCH(bool, result);

    QCOMPARE(m_handler->insertRole(role_id, ACLRole()), result);
  }

  void removeRole_data()
  {
    QTest::addColumn<QString>("role_id");
    QTest::addColumn<bool>("result");

    QTest::newRow("Remove role") << QString("FakeRole") << true;
    QTest::newRow("Fail to remove read-only role") << ACLRolesHandler::SUPER_ID << false;
  }

  void removeRole()
  {
    QFETCH(QString, role_id);
    QFETCH(bool, result);

    QCOMPARE(m_handler->removeRole(role_id), result);
  }

  void removeAllRoles_data()
  {
    QTest::addColumn<QString>("role_id");
    QTest::addColumn<bool>("result");

    QTest::newRow("Remove existing role") << QString("FakeRole") << false;
    QTest::newRow("Fail to remove default role") << ACLRolesHandler::SUPER_ID << true;
  }

  void removeAllRoles()
  {
    QFETCH(QString, role_id);
    QFETCH(bool, result);

    m_handler->clearRoles();
    QCOMPARE(m_handler->roleExists(role_id), result);
  }

  void checkRolePermission_data()
  {
    QTest::addColumn<QString>("role_id");
    QTest::addColumn<ACLRole::Permission>("permission");
    QTest::addColumn<bool>("result");

    QTest::newRow("Null permission (none)") << QString("FakeRole") << ACLRole::NONE << true;
    QTest::newRow("May kick users") << QString("FakeRole") << ACLRole::KICK << true;
    QTest::newRow("May not ban users") << QString("FakeRole") << ACLRole::BAN << false;
  }

  void checkRolePermission()
  {
    QFETCH(QString, role_id);
    QFETCH(ACLRole::Permission, permission);
    QFETCH(bool, result);

    ACLRole role = m_handler->getRoleById(role_id);
    QCOMPARE(role.checkPermission(permission), result);
  }

  void loadRolesFromIni()
  {
    QDir::setCurrent(KAL_AKASHI_TEST_DATA_PATH);
    QCOMPARE(m_handler->loadFile("config/acl_roles.ini"), true);

    QString l_role_id = "moderator";
    QCOMPARE(m_handler->roleExists(l_role_id), true);
    ACLRole l_role = m_handler->getRoleById(l_role_id);

    QCOMPARE(l_role.checkPermission(ACLRole::NONE), true);
    QCOMPARE(l_role.checkPermission(ACLRole::KICK), true);
    QCOMPARE(l_role.checkPermission(ACLRole::BAN), true);
    QCOMPARE(l_role.checkPermission(ACLRole::BGLOCK), true);
    QCOMPARE(l_role.checkPermission(ACLRole::MODIFY_USERS), false);
    QCOMPARE(l_role.checkPermission(ACLRole::CM), false);
    QCOMPARE(l_role.checkPermission(ACLRole::GLOBAL_TIMER), false);
    QCOMPARE(l_role.checkPermission(ACLRole::EVI_MOD), false);
    QCOMPARE(l_role.checkPermission(ACLRole::MOTD), false);
    QCOMPARE(l_role.checkPermission(ACLRole::ANNOUNCE), false);
    QCOMPARE(l_role.checkPermission(ACLRole::MODCHAT), true);
    QCOMPARE(l_role.checkPermission(ACLRole::MUTE), true);
    QCOMPARE(l_role.checkPermission(ACLRole::UNCM), false);
    QCOMPARE(l_role.checkPermission(ACLRole::SAVETEST), false);
    QCOMPARE(l_role.checkPermission(ACLRole::FORCE_CHARSELECT), false);
    QCOMPARE(l_role.checkPermission(ACLRole::BYPASS_LOCKS), true);
    QCOMPARE(l_role.checkPermission(ACLRole::IGNORE_BGLIST), true);
    QCOMPARE(l_role.checkPermission(ACLRole::SEND_NOTICE), false);
    QCOMPARE(l_role.checkPermission(ACLRole::JUKEBOX), false);
    QCOMPARE(l_role.checkPermission(ACLRole::SUPER), false);
  }
};

QTEST_APPLESS_MAIN(test_ACLRolesHandler)

#include "test_acl_roles_handler.moc"
