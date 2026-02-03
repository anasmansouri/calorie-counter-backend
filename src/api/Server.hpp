#pragma once
#include <cstdint>
#include <memory>

#include "services/FoodService.hpp"
#include "services/MealService.hpp"
#include "utils/Json_utils.hpp"
#include <crow.h>

#include <cstdlib>
#include <mutex>
#include <thread>

namespace cc::services {
class FoodService;
}
namespace cc::services {
class AuthService;
}

namespace cc::api {

class Server {
  public:
    Server(int port,
         std::shared_ptr<cc::services::FoodService> joodService ,
         std::shared_ptr<cc::services::MealService> mealService);
    ~Server();

    void setupRoutes();
    void start();
    void stop();
    void enableCors(bool enable);

  private:
    std::thread server_thread;
    crow::SimpleApp app;
    int port_;
    bool cors_ = false;
    std::shared_ptr<cc::services::FoodService> foodService_;
    std::shared_ptr<cc::services::MealService> mealService_;
    std::mutex m_;
    std::condition_variable cv_;
    bool running_ = false;
    std::once_flag once;
};

}
