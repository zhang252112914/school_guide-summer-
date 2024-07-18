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

void CampusMap::AddInfoSlot(int node_id, const QString& name,
                            const QString& description,
                            const QByteArray& image_data) {
  if (name.isEmpty()) {
    qDebug() << "Empty name";
    return;
  }

  if (description.isEmpty()) {
    qDebug() << "Empty description";
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

  if (image_data.size() > MAX_SIZE) {
    qDebug() << "image size exceeds 2MB limit";
    return;
  }

  QImage image;
  if (!image.loadFromData(image_data)) {
    qDebug() << "The data is not a valid image";
    return;
  }

  node.info_id = ++info_count;
  node.info_valid = true;

  Info info(info_count, name, description);

  infos.append(info);
  info_map[info_count] = info;
  emit InfoAdded(node_id, info, image_data);
}

void CampusMap::EditInfoSlot(const Info& new_info, const QByteArray& image_data,
                             const UpdateFlags& flags) {
  if (!info_map.contains(new_info.id)) {
    qDebug() << "Info ID" << new_info.id << "does not exist.";
    return;
  }

  if (image_data.size() > MAX_SIZE) {
    qDebug() << "image size exceeds 2MB limit";
    return;
  }

  Info& info = info_map[new_info.id];

  QStringList updates;
  if (flags.update_name) {
    info.name = new_info.name;
    updates.append("name updated");
  }
  if (flags.update_description) {
    info.description = new_info.description;
    updates.append("description updated");
  }
  if (flags.update_image) {
    updates.append("image_data updated");
  }
  if (updates.isEmpty()) {
    qDebug() << "No fields marked for update.";
    return;
  }

  emit InfoEdited(new_info, image_data, flags);
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

/*
void CampusMap::ManageIdSend(double pre_x, double pre_y, double last_x,
                             double last_y) {
  GetNodeIdFromCoordinateSlot(pre_x, pre_y, -1);
  GetNodeIdFromCoordinateSlot(last_x, last_y, -1);
}
*/

void CampusMap::GetInfoFromIdSlot(int info_id, Sender sender) {
  qDebug() << "hi hi";
  last_sender = sender;
  qDebug() << info_id;
  if (info_map.contains(info_id)) {
    qDebug() << "hahaha";
    last_info_id = info_id;
    emit RequestImageData(info_id);
  } else {
    emit InfoNotFound(sender);
  }
}

void CampusMap::GetSiteSlot(Sender sender) {
  last_sender = sender;
  QVector<QPair<QPair<double, double>, QString>> sites;
  for (const auto& it : nodes)
    if (it.info_valid) {
      QString name = info_map[it.info_id].name;
      sites.append({{it.pos_x, it.pos_y}, name});
    }
  if (sites.isEmpty()) {
    qDebug() << "No valid sites to send.";
  }
  emit SitesFound(sites, sender);
}

void CampusMap::SearchNodeSlot(double pos_x, double pos_y, Sender sender) {
  last_sender = sender;
  QMap<int, Node>::const_iterator ci;
  for (ci = node_map.constBegin(); ci != node_map.constEnd(); ++ci) {
    const auto& node = ci.value();
    if (pow(node.pos_x - pos_x, 2) + pow(node.pos_y - pos_y, 2) <=
        RADIUS * RADIUS) {
      emit NodeFound(*ci, sender);
      return;
    }
  }
  emit NodeNotFound(sender);
}

void CampusMap::GetEdgeSlot(Sender sender) {
  QVector<QPair<QPair<double, double>, QPair<double, double>>> coordinate_pairs;
  for (const auto& edge : edges) {
    const Node& n1 = nodes[edge.end_node_one_id - 1];
    const Node& n2 = nodes[edge.end_node_two_id - 1];
    QPair<double, double> coord1 = qMakePair(n1.pos_x, n1.pos_y);
    QPair<double, double> coord2 = qMakePair(n2.pos_x, n2.pos_y);
    coordinate_pairs.append(qMakePair(coord1, coord2));
  }
  emit EdgesFound(coordinate_pairs, sender);
}

void CampusMap::HandleImageDataFetchedSlot(const QByteArray& image_data) {
  emit InfoFound(info_map[last_info_id], image_data, last_sender);
}

void CampusMap::FindPath(int start, int end) {
  QVector<QVector<double>> matrix = BuildMatrix();
  QVector<int> shortest_path = FindShortestPath(start, end, matrix);
  QVector<QPair<double, double>> shortest_path_by_coordinate;
  for (auto it : shortest_path) {
    shortest_path_by_coordinate.push_back(
        QPair<double, double>(node_map[it].pos_x, node_map[it].pos_y));
  }
  emit ReturnPathVector(shortest_path_by_coordinate);
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

void CampusMap::ReturnNodesToAddSitePage() { emit NodesFeedBack(nodes); }
