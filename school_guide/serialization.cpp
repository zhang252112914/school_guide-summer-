#include "serialization.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QVector>

serialization::serialization() {
    init_database();
}

//这个主要是用来实现基本上所有条件的准备，即进行完这一步之后，运行时的所有数据都准备好了
serialization::serialization(my_map &cur_map){
    init_database();
    deserialize_node(cur_map);
    deserialize_edge(cur_map);
    cur_map.refresh();
}

void serialization::init_database(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QString dbPath = QDir::currentPath() + QDir::separator() + "my_database.db";
    db.setDatabaseName(dbPath);

    // 尝试打开数据库连接
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
    }

    if(!is_table_exist("node_table")){
        create_node_table();
    }

    if(!is_table_exist("edge_table")){

    }


}

bool serialization::is_table_exist(QString table_name){
    QSqlQuery query;
    query.prepare("PRAGMA table_info(" + table_name + ")");
    if(query.exec()){
        qDebug() << "can't check the table\n";
        return false;
    }
    return query.next();
}

//下面就是对于数据存取的序列化和反序列化
void serialization::serialize_node(my_map &cur_map){
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open";
        return;
    }

    for(auto &n: cur_map.vex){
        QSqlQuery query;
        query.prepare("INSERT INTO node_table (pos_x, pos_y, id, name, info, pic_path) VALUES (?, ?, ?, ?, ?, ?)");
        query.addBindValue(n.pos_x);
        query.addBindValue(n.pos_y);
        query.addBindValue(n.id);
        query.addBindValue(n.name);
        query.addBindValue(n.info);
        query.addBindValue(n.pic_path);
        if (!query.exec()) {
            qDebug() << "Error: Failed to insert data into table." << query.lastError();
        }
    }
}

void serialization::deserialize_node(my_map &cur_map){
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open";
        return;
    }

    QSqlQuery query("SELECT pos_x, pos_y, id, name, info, pic_path FROM node_table");

    while (query.next()) {
        int pos_x = query.value(0).toInt();
        int pos_y = query.value(1).toInt();
        int id = query.value(2).toInt();
        QString name = query.value(3).toString();
        QString info = query.value(4).toString();
        QString pic_path = query.value(5).toString();
        cur_map.vex.emplace_back(pos_x, pos_y, id, name, info, pic_path);
    }

}

void serialization::serialize_edge(my_map &cur_map){
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open";
        return;
    }

    for(auto &e: cur_map.edges){
        QSqlQuery query;
        query.prepare("INSERT INTO edge_table (e1, e2, len) VALUES (?, ?, ?)");
        query.addBindValue(e.end1);
        query.addBindValue(e.end2);
        query.addBindValue(e.length);

        if (!query.exec()) {
            qDebug() << "Error: Failed to insert data into table." << query.lastError();
        }
    }

}

void serialization::deserialize_edge(my_map &cur_map){
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open";
        return;
    }

    QSqlQuery query("SELECT e1, e2, len FROM edge_table");

    while (query.next()) {
        int e1 = query.value(0).toInt();
        int e2 = query.value(1).toInt();
        int len = query.value(2).toInt();
        cur_map.edges.emplace_back(e1,e2,len);
    }
}


//这里主要是针对还没有初始化的数据库进行建表，一般来说只有第一次运行的时候才会发生
void serialization::create_node_table(){
    QSqlQuery query;

    QString createTableQuery = R"(
            CREATE TABLE node_table (
                pos_x INTEGER,
                pos_y INTEGER,
                id INTEGER PRIMARY KEY,
                name TEXT,
                info TEXT,
                pic_path TEXT
            )
        )";

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error: Failed to create table." << query.lastError();
    }
}

void serialization::create_edge_table(){
    QSqlQuery query;

    QString createTableQuery = R"(
            CREATE TABLE edge_table (
                e1 INTEGER,
                e2 INTEGER,
                len INTEGER
            )
        )";
    if (!query.exec(createTableQuery)) {
        qDebug() << "Error: Failed to create table." << query.lastError();
    }
}
