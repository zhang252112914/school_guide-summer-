#include "campus_map.h"

CampusMap::CampusMap(QObject* parent) : QObject{parent} {}

int CampusMap::AddNode(double pos_x, double pos_y) {
  if (map_coordinate.contains({pos_x, pos_y})) {
    return map_coordinate[{pos_x, pos_y}];
  }
  node_count++;
  Node node(node_count, pos_x, pos_y);
  nodes.append(node);
  node_map[node_count] = node;
  map_coordinate[{pos_x, pos_y}] = node_count;
  emit NodeAdded(node);
  return node_count;
}

void CampusMap::AddEdge(const QPair<double, double>& start,
                        const QPair<double, double>& end) {
  edge_count++;
  double length =
      sqrt(pow(start.first - end.first, 2) + pow(start.second - end.second, 2));
  int node_one_id = AddNode(start.first, start.second);
  int node_two_id = AddNode(end.first, end.second);
  Edge edge(edge_count, node_one_id, node_two_id, length);
  edges.append(edge);
  edge_map[edge_count] = edge;
  emit EdgeAdded(edge);
}

void CampusMap::ReadNodeSlot(const Node& node) {
  nodes.push_back(node);
  node_map[node.id] = node;
  map_coordinate[{node.pos_x, node.pos_y}] = node.id;
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

void CampusMap::AddEdgeSlot(const QVector<QPair<double, double>>& coordinates) {
  if (coordinates.size() <= 1) return;

  QVector<QPair<double, double>>::ConstIterator ci;
  for (ci = coordinates.constBegin(); ci != coordinates.constEnd() - 1; ++ci) {
    QPair<double, double> start = *ci;
    QPair<double, double> end = *(ci + 1);
    AddEdge(start, end);
  }
}

void CampusMap::AddInfoSlot(int node_id,
                            const QMap<QString, QString>& info_pair) {
  if (!info_pair.contains("name") || !info_pair.contains("description") ||
      !info_pair.contains("pic_path")) {
    qDebug() << "info_pair is missing one or more required keys.";
    return;
  }

  if (!node_map.contains(node_id)) {
    qDebug() << "Node ID" << node_id << "does not exist.";
    return;
  }

  Node& node = node_map[node_id];

  if (node.info_valid) {
    qDebug() << "Info already exists.";
    return;
  }

  node.info_id = ++info_count;
  node.info_valid = true;

  QString name = info_pair["name"];
  QString description = info_pair["description"];
  QString pic_path = info_pair["pic_path"];

  Info info(info_count, name, description, pic_path);

  infos.append(info);
  info_map[info_count] = info;
  emit InfoAdded(info);
}

void CampusMap::EditInfoSlot(int node_id,
                             const QMap<QString, QString>& info_pair) {
  if (!info_pair.contains("name") || !info_pair.contains("description") ||
      !info_pair.contains("pic_path")) {
    qDebug() << "info_pair is missing one or more required keys.";
    return;
  }

  if (!node_map.contains(node_id)) {
    qDebug() << "Node ID" << node_id << "does not exist.";
    return;
  }

  const Node& node = node_map[node_id];

  if (!node.info_valid) {
    qDebug() << "Site is not valid";
    return;
  }

  Info& info = info_map[node.info_id];
  info.name = info_pair["name"];
  info.description = info_pair["description"];
  info.pic_path = info_pair["pic_path"];
}

void CampusMap::DeleteInfoSlot(int node_id) {
  if (!node_map.contains(node_id)) {
    qDebug() << "Node ID" << node_id << "does not exist.";
    return;
  }

  Node& node = node_map[node_id];

  if (!node.info_valid) {
    qDebug() << "Site is not valid";
    return;
  }

  int info_id = node.info_id;
  info_map.remove(info_id);
  node.info_id = 0;
  node.info_valid = false;

  emit InfoDeleted(info_id);
}

void CampusMap::GetNodeIdFromCoordinateSlot(double pos_x, double pos_y,
                                            Sender sender) {
  QMap<int, Node>::const_iterator ci;
  for (ci = node_map.constBegin(); ci != node_map.constEnd(); ++ci) {
    auto& node = ci.value();
    if (pow(node.pos_x - pos_x, 2) + pow(node.pos_y - pos_y, 2) <=
        RADIUS * RADIUS) {
      emit IdFound(ci.key(), sender);
      return;
    }
  }
  emit IdNotFound(sender);
}

void CampusMap::GetInfoFromIdSlot(int id, Sender sender) {
  if (info_map.contains(id)) {
    emit InfoFound(info_map[id], sender);
  } else {
    emit InfoNotFound(sender);
  }
}

void CampusMap::GetSiteSlot(Sender sender) {
  QVector<QPair<QPair<double, double>, QString>> sites;
  for (const auto& it : nodes)
    if (it.info_valid) {
      QString name = info_map[it.info_id].name;
      sites.append({{it.pos_x, it.pos_y}, name});
    }
  emit SitesFound(sites, sender);
}

void CampusMap::FindPath(int start, int end) {
  QVector<QVector<double>> matrix = BuildMatrix();
  QVector<int> shortest_path = FindShortestPath(start, end, matrix);
  emit ReturnPathVector(shortest_path);
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

/*
QVector<QVector<int>> CampusMap::FindAllPaths(
    int start, int end, const QVector<QVector<double>>& matrix) {
  QVector<QVector<int>> allPaths;
  // QVector<int> path;
  // dfs(start, end, matrix, path, allPaths);
  return allPaths;
}
*/

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
