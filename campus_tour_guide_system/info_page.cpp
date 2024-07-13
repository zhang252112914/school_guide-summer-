#include "info_page.h"

#include <QGraphicsPixmapItem>

#include "ui_info_page.h"
InfoPage::InfoPage(QWidget *parent) : QWidget(parent), ui(new Ui::InfoPage) {
  ui->setupUi(this);
}

InfoPage::~InfoPage() { delete ui; }

void InfoPage::displayInfo(const Info &info, Sender sender) {
  // 显示图片
  if (!info.pic_path.isEmpty()) {
    QPixmap image(info.pic_path);
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->addPixmap(image.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation));
  }

  // 显示名称和描述
  QString text =
      QString("Name: %1\nDescription: %2").arg(info.name).arg(info.description);
  ui->textEdit->setText(text);
}
