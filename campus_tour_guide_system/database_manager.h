#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QDebug>
#include <QDir>
#include <QMap>
#include <QMetaType>
#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include "types.h"

/**
 * @brief The DatabaseManager class handles database CRUD and communicates with
 * CampusMap class.
 */

class DatabaseManager : public QObject {
  friend class MessageMediator;

  Q_OBJECT

 public:
  explicit DatabaseManager(const QString& config_file_path,
                           QObject* parent = nullptr);
  ~DatabaseManager();

  bool IsOpen() const;

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
  void SerializeInfoSlot(int node_id, const Info& info,
                         const QByteArray& image_data);

  void UpdateInfoSlot(const Info& info, const QByteArray& image_data,
                      const UpdateFlags& flags);
  void DeleteInfoSlot(int info_id);
  void FetchImageData(int info_id);

 private:
  QSqlDatabase db;

 signals:
  /**
   * These three signals will captured by CampusMap object to load map data.
   */
  void NodeLoaded(const Node& node);
  void EdgeLoaded(const Edge& edge);
  void InfoLoaded(const Info& info);

  void ImageDataFetched(const QByteArray& image_data);
};

#endif  // DATABASE_MANAGER_H
