#include "campus_map.h"

#include <QPair>
#include <limits>
#include <queue>

CampusMap::CampusMap(QObject* parent) : QObject{parent} {}

void CampusMap::AddNode(double pos_x, double pos_y) {
  node_count++;
  nodes.emplaceBack(node_count, pos_x, pos_y);
  node_map[node_count] = {node_count, pos_x, pos_y};
  emit NodeAdded({node_count, pos_x, pos_y});
}

void CampusMap::AddEdge(int node_one_id, int node_two_id) {
  edge_count++;
  const Node& n1 = node_map[node_one_id];
  const Node& n2 = node_map[node_two_id];
  double length =
      sqrt(pow(n1.pos_x - n2.pos_x, 2) + pow(n1.pos_y - n2.pos_y, 2));
  edges.emplaceBack(edge_count, node_one_id, node_two_id);
  edge_map[edge_count] = {edge_count, node_one_id, node_two_id, length};
  emit EdgeAdded({edge_count, node_one_id, node_two_id, length});
}

void CampusMap::AddInfo(const QString& name, const QString& description,
                        const QString& pic_path) {
  info_count++;
  infos.emplaceBack(info_count, name, description, pic_path);
  info_map[info_count] = {info_count, name, description, pic_path};
  emit InfoAdded({info_count, name, description, pic_path});
}

void CampusMap::ReadNodeSlot(const Node& node) {
  nodes.push_back(node);
  node_map[node.id] = node;
  node_count++;
}

void CampusMap::ReadEdgeSlot(const Edge& edge) {
  edges.push_back(edge);
  edge_map[edge.id] = edge;
  edge_count++;
}

void CampusMap::ReadInfoSlot(const Info& info) {
  infos.push_back(info);
  info_map[info.id] = info;
  info_count++;
}

void CampusMap::GetNodeIdFromCoordinateSlot(double pos_x, double pos_y,
                                            int request_id) {
  if (map_coordinate.contains({pos_x, pos_y})) {
    emit IdFound(map_coordinate[{pos_x, pos_y}], request_id);
  } else {
    qDebug() << "id not found, please check code";
  }
}

QPair<QVector<int>, QVector<QVector<int>>> CampusMap::FindPath(int start,
                                                               int end) {
  QVector<QVector<double>> matrix = BuildMatrix();
  return QPair<QVector<int>, QVector<QVector<int>>>(
      FindShortestPath(start, end, matrix), FindAllPaths(start, end, matrix));
}

QVector<QVector<double>> CampusMap::BuildMatrix() {
  int maxm = 0;
  for (auto& it : nodes) {
    maxm = it.id > maxm ? it.id : maxm;
  }
  QVector<QVector<double>> matrix(
      maxm + 1,
      QVector<double>(maxm + 1, std::numeric_limits<double>::infinity()));
  for (auto& it : edges) {
    matrix[it.end_node_one_id][it.end_node_two_id] =
        matrix[it.end_node_two_id][it.end_node_one_id] = it.length;
  }
  return matrix;
}

QVector<int> CampusMap::FindShortestPath(
    int node_one_id, int node_two_id, const QVector<QVector<double>>& matrix) {
  int n = matrix.size();
  QVector<double> dist(n, std::numeric_limits<double>::infinity());
  QVector<int> prev(n, -1);
  dist[node_one_id] = 0;

  using NodeDistPair = QPair<int, double>;
  auto compare = [](NodeDistPair left, NodeDistPair right) {
    return left.second > right.second;
  };
  std::priority_queue<NodeDistPair, QVector<NodeDistPair>, decltype(compare)>
      pq(compare);

  pq.push(qMakePair(node_one_id, 0.0));

  while (!pq.empty()) {
    int u = pq.top().first;
    double d = pq.top().second;
    pq.pop();

    if (d > dist[u]) continue;

    for (int v = 0; v < n; ++v) {
      if (matrix[u][v] < std::numeric_limits<double>::infinity()) {
        double alt = dist[u] + matrix[u][v];
        if (alt < dist[v]) {
          dist[v] = alt;
          prev[v] = u;
          pq.push(qMakePair(v, dist[v]));
        }
      }
    }
  }

  QVector<int> path;
  if (dist[node_two_id] == std::numeric_limits<double>::infinity()) {
    return path;  // No path found
  }

  for (int at = node_two_id; at != -1; at = prev[at]) {
    path.prepend(at);
  }

  return path;
}

QVector<QVector<int>> CampusMap::FindAllPaths(
    int start, int end, const QVector<QVector<double>>& matrix) {
  QVector<QVector<int>> allPaths;
  // QVector<int> path;
  // dfs(start, end, matrix, path, allPaths);
  return allPaths;
}

void CampusMap::dfs(int current, int end,
                    const QVector<QVector<double>>& matrix, QVector<int>& path,
                    QVector<QVector<int>>& allPaths) {
  path.append(current);

  if (current == end) {
    allPaths.append(path);
  } else {
    for (int i = 0; i < matrix[current].size(); ++i) {
      if (matrix[current][i] != 0) {  // assuming 0 means no connection
        dfs(i, end, matrix, path, allPaths);
      }
    }
  }

  path.removeLast();
}
