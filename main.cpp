#include "Application.hpp"

int main() {
    Application application;
    while (!WindowShouldClose()) {
        application.run();
    }
    return 0;
}
