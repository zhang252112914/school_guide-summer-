#ifndef MY_MAP_H
#define MY_MAP_H

#include <QMetatype>
#include <QString>
#include <QVector>

struct node{
    int pos_x;
    int pos_y;             //地图上的坐标，方便mouseevent进行

    int id;            //景点的编号
    QString name;      //景点的名字
    QString info;      //景点的介绍信息
    QString pic_path;  //图片的相对路径

    node(){}

    node(int x, int y, int i, QString n, QString inf, QString pic):pos_x(x), pos_y(y),id(i), name(n) ,info(inf), pic_path(pic){}

};

struct edge{
    int end1;
    int end2;
    int length;

    edge(){}

    edge(int e1, int e2, int l):end1(e1), end2(e2),length(l){}
};

struct my_map{
    QVector<node> vex;
    QVector<edge> edges;

    QVector<QVector<int>> graph;  //邻接矩阵

    my_map(){}

    my_map(QVector<node> v, QVector<edge> e):vex(v), edges(e){
        refresh();
    }

    void add_node(node n){
        vex.push_back(n);
    }

    void add_edge(edge e){
        edges.push_back(e);
    }

    void refresh(){      //一旦有新的内容加入，就重新进行建表
        int n = vex.size();
        graph = QVector<QVector<int>>(n+1,QVector<int>(n+1));
        for(auto &it : edges){
            graph[it.end1][it.end2] = graph[it.end2][it.end1] = it.length;
        }
    }

};

Q_DECLARE_METATYPE(node)
Q_DECLARE_METATYPE(edge)
Q_DECLARE_METATYPE(my_map)

#endif // MY_MAP_H
