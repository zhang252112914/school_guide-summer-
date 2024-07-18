#include "add_site_page.h"

#include <QMessageBox>

#include "ui_add_site_page.h"

AddSitePage::AddSitePage(QWidget* parent)
    : QWidget(parent), add_site_page(new Ui::AddSitePage) {
  add_site_page->setupUi(this);
  setWindowTitle("Add Site");
  graph = qobject_cast<GraphicsDisplay*>(add_site_page->graph_view);
  if (graph) {
    connect(graph, &GraphicsDisplay::PointClicked, this,
            &AddSitePage::receive_one_node);
  }
  // 设置输入框的提示内容
  add_site_page->name_input->setPlaceholderText("景点名称");
  add_site_page->description_input->setPlaceholderText("景点介绍");
  add_site_page->pic_label->setText("No picture");
  image_changed = false;
}

AddSitePage::~AddSitePage() { delete add_site_page; }

void AddSitePage::on_return_button_clicked() {
  emit BackToManagePage();
  this->hide();
}

void AddSitePage::on_cancel_button_clicked() {
  add_site_page->name_input->clear();
  add_site_page->name_input->setPlaceholderText("景点名称");
  add_site_page->description_input->clear();
  add_site_page->description_input->setPlaceholderText("景点介绍");
  add_site_page->pic_label->setText("No picture");
  image_data_container.clear();
  graph->DeletePointOfAddPage();
  image_changed = false;
  qDebug() << "恢复";
}

void AddSitePage::on_confirm_button_clicked() {
  QString name = add_site_page->name_input->text();
  QString description = add_site_page->description_input->toPlainText();
  // 只有在id有效的时候才会发送添加景点的信号
  qDebug() << name << '\n' << description;
  if (single_node.id != -1) {
    // 如果是新增加，则发送对应信号
    if (!single_node.info_valid) {
      emit AddMessageCollection(single_node.id, name, description,
                                image_data_container);
      qDebug() << "didn't do";
    } else {
      Info new_info = {single_node.info_id, name, description};
      emit EditMessageCollection(new_info, image_data_container,
                                 {true, true, image_changed});
      qDebug() << "do";
    }
    image_changed = false;
  }
  // 反馈给用户一些信息,可能做出一个弹窗会更加合适
  else {
    qDebug() << "The node you select is invalid";
  }
  single_node.id = -1;
  // 恢复原状
  on_cancel_button_clicked();
}

void AddSitePage::on_add_picture_button_clicked() {
  QString filePath =
      QFileDialog::getOpenFileName(nullptr, "Open Image File", QDir::homePath(),
                                   "Images (*.png *.xpm *.jpg)");
  if (!filePath.isEmpty()) {
    QImage image(filePath);

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");  // Save the image in JPG format into the buffer
    QByteArray image_data = buffer.data();

    if (image_data_container.size() > 2097152) {
      image_data = compressImage(image);
    }

    QImage finalimage;
    finalimage.loadFromData(image_data);

    // 展示选中的图片
    QPixmap pixmap = QPixmap::fromImage(image);
    QSize label_size = add_site_page->pic_label->size();
    QPixmap scaled_pixmap = pixmap.scaled(label_size, Qt::KeepAspectRatio,
                                          Qt::SmoothTransformation);
    add_site_page->pic_label->setPixmap(scaled_pixmap);

    image_data_container = image_data;
  }
}

QByteArray AddSitePage::compressImage(const QImage& image) {
  QByteArray imageData;
  QBuffer buffer(&imageData);
  buffer.open(QIODevice::WriteOnly);
  int quality = 100;  // 初始质量设置为100%

  do {
    buffer.reset();
    imageData.clear();
    image.save(&buffer, "JPG",
               quality);  // Save the image in JPG format into the buffer

    if (imageData.size() > 2097152) {
      quality -= 10;  // 如果大于2MB，降低图像质量
    } else {
      break;  // 图像大小在限制范围内，停止压缩
    }
  } while (quality > 0);

  return imageData;
}

void AddSitePage::receive_one_node(double x, double y) {
  pos_x = x;
  pos_y = y;
  qDebug() << "Receive x and y:" << x << ":" << y;
  emit CoordinateToNode(x, y);
}

void AddSitePage::ModifyNode(Node back_node, Sender sender) {
  if (sender == Sender::MANAGE_PAGE) {
    // 去除前面旧的节点信息和图像
    graph->DeletePointOfAddPage();
    single_node = back_node;
    // 依据新的信息绘制点
    graph->PaintBluePoint(single_node);
    // 判断是否是景点，如果是景点就显示已有的信息，如果不是就显示占位符
    if (single_node.info_valid) {
      emit PresentInfoRequest(single_node.id);
    } else {
      add_site_page->name_input->setPlaceholderText("输入景点名称");
      add_site_page->description_input->setPlaceholderText("输入景点介绍");
    }
  }
}

void AddSitePage::PresentInfo(const Info& info, const QByteArray& image_data,
                              Sender sender) {
  if (sender == Sender::MANAGE_PAGE) {
    add_site_page->name_input->setText(info.name);
    add_site_page->description_input->setText(info.description);
    QImage image;
    image.loadFromData(image_data);
    if (!image.isNull()) {
      QPixmap pixmap = QPixmap::fromImage(image);
      QSize labelSize = add_site_page->pic_label->size();
      QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation);
      add_site_page->pic_label->setPixmap(scaledPixmap);
    } else {
      add_site_page->pic_label->setText("No picture or fail to load picture");
    }
  }
}

void AddSitePage::InvalidNode(Sender sender) {
  if (sender == Sender::MANAGE_PAGE) {
    qDebug() << "useless click";
    single_node.id = -1;
  }
}

void AddSitePage::PaintMap(QVector<Node> nodes) {
  qDebug() << "complete painting";
  // 红色表示是景点，黄色表示是普通节点
  graph->PaintForAddSitePage(nodes);
}

void AddSitePage::PaintRequestWrapper() { emit PaintRequest(); }
