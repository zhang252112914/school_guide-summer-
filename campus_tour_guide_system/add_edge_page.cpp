#include "add_edge_page.h"

#include "ui_add_edge_page.h"

AddEdgePage::AddEdgePage(QWidget *parent)
    : QWidget(parent), add_edge_page(new Ui::AddEdgePage) {
  add_edge_page->setupUi(this);
  setWindowTitle("Add Path");
  graph = add_edge_page->graph_view;
  if (graph) {
    connect(graph, &GraphicsDisplay::PointClicked, this,
            &AddEdgePage::receive_one_node);
  }
  add_edge_page->point_one_label->setText("第一个点：");
  add_edge_page->point_two_label->setText("第二个点：");
  add_edge_page->line_label->setText("边：");
}

AddEdgePage::~AddEdgePage() { delete add_edge_page; }

void AddEdgePage::on_return_button_clicked() {
  this->hide();
  emit BackToManagePage();
}

void AddEdgePage::on_confirm_button_clicked() {
  // QMessageBox::information(this, "提示", "添加成功");
  // emit AddEdge();
  if (n1_clicked && n2_clicked) {
    QPair<double, double> StartP(x1, y1);
    QPair<double, double> EndP(x2, y2);
    QPointF p1(x1, y1);
    QPointF p2(x2, y2);
    graph->AddLine(p1, p2);  // 画线
    emit NewNode(x1, y1);
    emit NewNode(x2, y2);
    emit NewEdge(StartP, EndP);
    QMessageBox::information(this, "提示", "添加成功");
  } else {
    QMessageBox::information(this, "提示", "添加失败");
  }
  on_cancel_button_clicked();
}

void AddEdgePage::on_show_button_clicked() {
  emit GetEdges(Sender::MANAGE_PAGE);
}

void AddEdgePage::on_cancel_button_clicked() {
  add_edge_page->point_one_label->setText("第一个点：");
  add_edge_page->point_two_label->setText("第二个点：");
  add_edge_page->line_label->setText("边：");
  graph->DeletePointsOfAddEdgePage();
  n1_clicked = 0;
  n2_clicked = 0;
  n1_exist = 0;
  n2_exist = 0;
  qDebug() << "恢复";
}

void AddEdgePage::receive_one_node(double x, double y) {  // 考虑点击范围
  if (!n1_clicked && !n2_clicked) {
    x1 = x;
    y1 = y;
    n1_clicked = 1;
    emit FindNode(x1, y1);
    graph->AddBlackPoint(x1, y1);
    QString PText = QString("第一个点：(%1,%2）").arg(x1).arg(y1);
    add_edge_page->point_one_label->setText(PText);
  } else if (n1_clicked && !n2_clicked) {
    x2 = x;
    y2 = y;
    n2_clicked = 1;
    emit FindNode(x2, y2);
    graph->AddBlackPoint(x2, y2);
    QString PText = QString("第二个点：(%1,%2）").arg(x2).arg(y2);
    add_edge_page->point_two_label->setText(PText);
  } else if (n1_clicked && n2_clicked) {
    x1 = x2;
    y1 = y2;
    x2 = x;
    y2 = y;
    emit FindNode(x2, y2);
    graph->DeletePointOneOfAddEdgePage();
    graph->AddBlackPoint(x2, y2);
    QString PText = QString("第一个点：(%1,%2）").arg(x1).arg(y1);
    add_edge_page->point_one_label->setText(PText);
    PText = QString("第二个点：(%1,%2）").arg(x2).arg(y2);
    add_edge_page->point_two_label->setText(PText);
  }
}

void AddEdgePage::ConfirmNode(Node back_node, Sender s) {
  if (s == Sender::MANAGE_PAGE) {
    if (n1_clicked && !n2_clicked) {
      x1 = back_node.pos_x;
      y2 = back_node.pos_y;
      n1_exist = 1;
    } else {
      x2 = back_node.pos_x;
      y2 = back_node.pos_y;
      n2_exist = 1;
    }
    if (n1_exist && !n2_exist) {
      graph->DeletePointOneOfAddEdgePage();
      graph->AddBlackPoint(x1, y1);
      QString PText = QString("第一个点：(%1,%2）").arg(x1).arg(y1);
      add_edge_page->point_one_label->setText(PText);
    } else if (!n1_exist && n2_exist) {
      graph->DeletePointTwoOfAddEdgePage();
      graph->AddBlackPoint(x2, y2);
      QString PText = QString("第二个点：(%1,%2）").arg(x2).arg(y2);
      add_edge_page->point_two_label->setText(PText);
    } else {
      graph->DeletePointsOfAddEdgePage();
      graph->AddBlackPoint(x1, y1);
      graph->AddBlackPoint(x2, y2);
      QString PText = QString("第一个点：(%1,%2）").arg(x1).arg(y1);
      add_edge_page->point_one_label->setText(PText);
      PText = QString("第二个点：(%1,%2）").arg(x2).arg(y2);
      add_edge_page->point_two_label->setText(PText);
    }
  }
}

void AddEdgePage::PaintEdges(  //
    QVector<QPair<QPair<double, double>, QPair<double, double>>> edges) {
  for (auto &edge : edges) {
    x1 = edge.first.first;
    y1 = edge.first.second;
    x2 = edge.second.first;
    y2 = edge.second.second;
    QPointF p1(x1, y1);
    QPointF p2(x2, y2);
    graph->AddPoint(x1, y1);
    graph->AddPoint(x2, y2);
    graph->AddLine(p1, p2);
  }
}
