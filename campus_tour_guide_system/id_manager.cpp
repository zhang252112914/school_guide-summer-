#include "id_manager.h"

static IdManager& instance() {
  static IdManager _instance;
  return _instance;
}
