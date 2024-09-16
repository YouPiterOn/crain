#pragma once

namespace crain {
  struct ServerConfig {
    int timeout = 100;
    size_t poolSize = 5;
  };
}