#include "id_manager.h"

IdManager& IdManager::instance() {
  static IdManager _instance;
  return _instance;
}

qint64 IdManager::GetUniqueId() { return ++current_id; }
