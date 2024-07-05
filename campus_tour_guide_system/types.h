#ifndef TYPES_H
#define TYPES_H

#include <QMetaType>
#include <QString>

struct Node {
  bool valid;
  int id;  // use distinguished id to access node and info

  double pos_x;
  double pos_y;
  Node(int id = 0, double pos_x = 0.0, double pos_y = 0.0)
      : id(id), pos_x(pos_x), pos_y(pos_y) {}
};

struct Edge {
  int id;  // use distinguished id to access edge
  int end_node_one_id;
  int end_node_two_id;
  double length;
  Edge(int id = 0, int end_node_one_id = 0, int end_node_two_id = 0,
       double length = 0.0)
      : id(id),
        end_node_one_id(end_node_one_id),
        end_node_two_id(end_node_two_id),
        length(length) {}
};

struct Info {
  int id;  // same id with node
  QString name;
  QString description;
  QString pic_path;

  Info(int id = 0, const QString& name = "", const QString& description = "",
       const QString& pic_path = "")
      : id(id), name(name), description(description), pic_path(pic_path) {}
};

Q_DECLARE_METATYPE(Node);
Q_DECLARE_METATYPE(Edge);
Q_DECLARE_METATYPE(Info);

#endif  // TYPES_H
