#include "crow.h"
#include "nlohmann/json.hpp"

#define json nlohmann::json 

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "Hello, Crow!";
        });

    app.port(8080).multithreaded().run();
}
