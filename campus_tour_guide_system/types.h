#ifndef TYPES_H
#define TYPES_H

const double RADIUS = 10.0;
const int MAX_SIZE = 2 * 1024 * 1024;

#include <QMetaType>
#include <QString>

struct Node {
  int id;  // use distinguished id to access node and info

  double pos_x;
  double pos_y;
  bool info_valid = false;
  int info_id = 0;
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
  int id;
  QString name;
  QString description;

  Info(int id = 0, const QString& name = "", const QString& description = "")
      : id(id), name(name), description(description) {}
};

struct UpdateFlags {
  bool update_name;
  bool update_description;
  bool update_image;

  UpdateFlags(bool name = false, bool description = false, bool image = false)
      : update_name(name),
        update_description(description),
        update_image(image) {}
};

Q_DECLARE_METATYPE(Node);
Q_DECLARE_METATYPE(Edge);
Q_DECLARE_METATYPE(Info);
Q_DECLARE_METATYPE(UpdateFlags);

#endif  // TYPES_H
