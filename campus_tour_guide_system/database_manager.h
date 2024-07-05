#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QDebug>
#include <QDir>
#include <QMap>
#include <QMetaType>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include "types.h"

class CampusMap;

/**
 * @brief The DatabaseManager class handles database CRUD and communicates with
 * CampusMap class.
 */

class DatabaseManager : public QObject {
  Q_OBJECT
 public:
  explicit DatabaseManager(const QString& path, QObject* parent = nullptr);
  ~DatabaseManager();

  /**
   * @brief SetCampusMap Set private member campus_map
   * @param cp pointer to CampusMap object
   */
  void SetCampusMap(const CampusMap* cp);

  /**
   * These three functions are used to translate data stored in database to the
   * type stored in CampusMap object. The data transfer is implemented using
   * signal.
   */
  void DeserializeNodes();
  void DeserializeEdges();
  void DeserializeInfos();

 private slots:
  /**
   * These Three Slot function handles new data serialization.
   */
  void SerializeNodeSlot(const Node& node);
  void SerializeEdgeSlot(const Edge& edge);
  void SerializeInfoSlot(const Info& info);

 private:
  QSqlDatabase db;
  const CampusMap* campus_map;

 signals:
  /**
   * These three signals will captured by CampusMap object to load map data.
   */
  void NodeLoaded(const Node& node);
  void EdgeLoaded(const Edge& edge);
  void InfoLoaded(const Info& info);
};

#endif  // DATABASE_MANAGER_H
