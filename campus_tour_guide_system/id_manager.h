#ifndef ID_MANAGER_H
#define ID_MANAGER_H

#include <QObject>

/**
 * @brief The IdManager class generates unique id for signal. Use
 * IdManager::instance().getUniqueId() to get unique id.
 */

class IdManager : public QObject {
  Q_OBJECT
 public:
  static IdManager& instance();

 private:
  qint64 current_id = 0;
 signals:
};

#endif  // ID_MANAGER_H
