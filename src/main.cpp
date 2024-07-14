#include "config_manager.h"
#include "server.h"

#include <cstdlib>

#include <QCoreApplication>
#include <QDebug>

Server *server;

void cleanup()
{
  server->deleteLater();
}

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("akashi");
  QCoreApplication::setApplicationVersion("honeydew hotfix (1.8.1)");
  std::atexit(cleanup);

  // Verify server configuration is sound.
  if (!ConfigManager::verifyServerConfig())
  {
    qCritical() << "config.ini is invalid!";
    qCritical() << "Exiting server due to configuration issue.";
    exit(EXIT_FAILURE);
    QCoreApplication::quit();
  }
  else
  {
    server = new Server(ConfigManager::serverPort());
    server->start();
  }

  return app.exec();
}
