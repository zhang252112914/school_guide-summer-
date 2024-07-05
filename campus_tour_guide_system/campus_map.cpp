#include "campus_map.h"

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
