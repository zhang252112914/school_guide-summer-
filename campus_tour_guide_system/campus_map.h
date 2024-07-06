#ifndef CAMPUS_MAP_H
#define CAMPUS_MAP_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QVector>

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

  /**
   * These Three functions are used to store new data during app execution,
   * which will send signal to DatabaseManager.
   */
  void AddNode(double pos_x, double pos_y);
  void AddEdge(int node_one_id, int node_two_id);
  void AddInfo(const QString& name, const QString& description,
               const QString& pic_path);

  /**
   * The following six functions are used to get node content.
   */
  bool IsValid(int id);
  double GetPosX(int id);
  double GetPosY(int id);

  /**
   * The following three functions are used to get edge content.
   */
  int GetNodeOneId(int id);
  int GetNodeTwoId(int id);
  double GetLength(int id);

  /**
   * These three functions are used to get info content.
   */
  QString GetName(int id);
  QString GetInfo(int id);
  QString GetPicPath(int id);

  //下面这一部分是用于路径查询的函数，第一个函数就是接口，直接调用该函数即可

  //调用该函数将会返回pair，其中first是最短路径上节点的vector，第二个参数无效，相应函数已被注释
  QPair<QVector<int>, QVector<QVector<int>>> FindPath(int start, int end);

  //基于campusmap中的edges和nodes建立邻接矩阵
  QVector<QVector<double>> BuildMatrix();

  //深度优先搜索寻找两点之间的所有路径
  void dfs(int current, int end, const QVector<QVector<double>>& matrix,
           QVector<int>& path, QVector<QVector<int>>& allPaths);

  //返回最短路线上的节点
  QVector<int> FindShortestPath(int node_one_id, int node_two_id,
                                const QVector<QVector<double>>& matrix);
  //返回两个节点之间的所有路径
  QVector<QVector<int>> FindAllPaths(int start, int end,
                                     const QVector<QVector<double>>& matrix);

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

 private:
  QVector<Node> nodes;  // store site nodes
  QVector<Edge> edges;  // store edges
  QVector<Info> infos;  // store site info

  /**
   * Maps between primary key and data
   */
  QMap<int, Node> node_map;
  QMap<int, Edge> edge_map;
  QMap<int, Info> info_map;

  /**
   * These three variables keep track of data count.
   */
  int node_count = 0;
  int edge_count = 0;
  int info_count = 0;

 signals:
  /**
   * Signals sent to DatabaseManager
   */
  void NodeAdded(const Node& node);
  void EdgeAdded(const Edge& edge);
  void InfoAdded(const Info& info);
};

#endif  // CAMPUS_MAP_H
