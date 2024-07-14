#ifndef CAMPUS_MAP_H
#define CAMPUS_MAP_H

#include <QDebug>
#include <QImage>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QString>
#include <QVector>
#include <limits>
#include <queue>

#include "sender_enum.h"
#include "types.h"

/**
 * @brief CampusMap class is used to store site information and address shortest
 * path.
 */
class CampusMap : public QObject {
  friend class MessageMediator;

  Q_OBJECT

 public:
  explicit CampusMap(QObject* parent = nullptr);

 private slots:
  /**
   * @brief ReadNodeSlot This function is used to load the data sent by
   * DatabaseManager into nodes vector. The following two is similar.
   * @param data The data sent by DatabaseManager, which is encapsulated in a
   * QMap object. The key corresponds to variable name.
   */
  void ReadNodeSlot(const Node& node);
  void ReadEdgeSlot(const Edge& edge);
  void ReadInfoSlot(const Info& info);

  void AddEdgeSlot(const QVector<QPair<double, double>>& coordinates);
  void AddInfoSlot(int node_id, const QString& name, const QString& description,
                   const QByteArray& image_data);
  void EditInfoSlot(const Info& new_info, const QByteArray& image_data,
                    const UpdateFlags& flags);
  void DeleteInfoSlot(int node_id);

  /**
   * @brief GetNodeIdFromCoordinate This function receives a coordinate pos_x
   * and pos_y and emit the id of corresponding node.
   * @param pos_x x coordinate value
   * @param pos_y y coordinate value
   * @param sender the enum type of sender class
   */
  // void GetNodeIdFromCoordinateSlot(double pos_x, double pos_y, Sender
  // sender);

  /**
   * @brief GetInfoFromIdSlot This function receives an id and emit the info
   * with this id.
   * @param id the id of requested info
   * @param sender the enum type of sender class
   */
  void GetInfoFromIdSlot(int info_id, Sender sender);

  /**
   * @brief GetSiteCoordinates This function receives a sender enum and emit a
   * vector of coordinates and names of valid sites
   * @param sender the enum type of sender class
   */
  void GetSiteSlot(Sender sender);

  /**
   * @brief SearchNodeSlot This function recieves the coordinate clicked by
   * mouse and search the node within a radius.
   * @param pos_x the x coordinate of mouse clicked point
   * @param pos_y the y coordinate of mouse clicked point
   * @param sender the enum type of sender class
   */
  void SearchNodeSlot(double pos_x, double pos_y, Sender sender);

  void GetEdgeSlot(Sender sender);

  // This function handles the ImageDataFetched signal sent by DatabaseManager.
  void HandleImageDataFetchedSlot(const QByteArray& image_data);
  //给add_site_page返回所有用于绘图的节点的vector
  void ReturnNodesToAddSitePage();

 private:
  /**
   * These Two functions are used to store new data during app execution,
   * which will send signal to DatabaseManager.
   */
  int AddNode(double pos_x, double pos_y);
  void AddEdge(const QPair<double, double>& start,
               const QPair<double, double>& end);

  // 下面这一部分是用于路径查询的函数，第一个函数就是接口，直接调用该函数即可

  // 调用该函数将会返回pair，其中first是最短路径上节点的vector，第二个参数无效，相应函数已被注释
  void FindPath(int start, int end);

  // 基于campusmap中的edges和nodes建立邻接矩阵
  QVector<QVector<double>> BuildMatrix();

  // 深度优先搜索寻找两点之间的所有路径
  void dfs(int current, int end, const QVector<QVector<double>>& matrix,
           QVector<int>& path, QVector<QVector<int>>& allPaths);

  // 返回最短路线上的节点
  QVector<int> FindShortestPath(int node_one_id, int node_two_id,
                                const QVector<QVector<double>>& matrix);
  // 返回两个节点之间的所有路径
  /*
  QVector<QVector<int>> FindAllPaths(int start, int end,
                                     const QVector<QVector<double>>& matrix);
                                     */

  QVector<Node> nodes;  // store site nodes
  QVector<Edge> edges;  // store edges
  QVector<Info> infos;  // store site info

  /**
   * Maps between primary key and data
   */
  QMap<int, Node> node_map;
  QMap<int, Edge> edge_map;
  QMap<int, Info> info_map;

  QMap<QPair<double, double>, int>
      map_coordinate;  // Map between node coordinate and primary key

  /**
   * These three variables keep track of data count.
   */
  int node_count = 0;
  int edge_count = 0;
  int info_count = 0;

  int last_info_id;    // store the last info id when slot function invokes
  Sender last_sender;  // store the last sender when slot function invokes

 signals:
  /**
   * Signals sent to DatabaseManager
   */
  void NodeAdded(const Node& node);
  void EdgeAdded(const Edge& edge);
  void InfoAdded(int node_id, const Info& info, const QByteArray& image_data);
  void InfoEdited(const Info& info, const QByteArray& image_data,
                  const UpdateFlags& flags);
  void InfoDeleted(int info_id);

  /**
   * @brief If GetNodeIdFromCoordinateSlot find corresponding node, IdFound
   * signal will be sent.
   * @param id the id found
   * @param sender the enum type of sender class
   */
  /*
  void IdFound(int id, Sender sender);
  void IdNotFound(Sender sender);
  */

  /**
   * @brief If SearchNodeSlot find corresponding node, NodeFound
   * signal will be sent.
   * @param pos_x the x coordinate of found node
   * @param pos_y the y coordinate of found node
   * @param sender the enum type of sender class
   */
  void NodeFound(const Node& node, Sender sender);
  void NodeNotFound(Sender sender);

  /**
   * @brief If GetInfoFromIdSlot find corresponding info, InfoFound
   * signal will be sent.
   * @param info the info found
   * @param sender the enum type of sender class
   */
  void InfoFound(const Info& info, const QByteArray& image_data, Sender sender);
  void InfoNotFound(Sender sender);

  void EdgesFound(
      const QVector<QPair<QPair<double, double>, QPair<double, double>>>&
          coordinate_pairs,
      Sender sender);

  // 返回装有路径的vector
  void ReturnPathVector(QVector<QPair<double, double>> route);

  void SitesFound(QVector<QPair<QPair<double, double>, QString>> sites,
                  Sender sender);

  void RequestImageData(int info_id);
  void NodesFeedBack(QVector<Node> nodes);
};

#endif  // CAMPUS_MAP_H
