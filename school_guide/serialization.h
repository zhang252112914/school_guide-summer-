#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <QMetaType>
#include <QString>
#include "my_map.h"

class serialization
{
public:
    serialization();

    serialization(my_map &cur_map);

    static void init_database();  //负责数据库的初始化，包括连接和各表的检查

    static bool is_table_exist(QString table_name);   //用于判断某类型的数据对应的表是否已经建好

    static void serialize_node(my_map &cur_map);   //序列化

    static void deserialize_node(my_map &cur_map);      //反序列化，取出数据构造运行时的数据结构

    static void serialize_edge(my_map &cur_map);

    static void deserialize_edge(my_map &cur_map);

    static void create_node_table();    //创建对应数据类型的表

    static void create_edge_table();
};

Q_DECLARE_METATYPE(serialization)

#endif // SERIALIZATION_H
