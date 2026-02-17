#pragma once

#include <raylib.h>
#include <vector>
#include <print>
#include <random>
#include <imgui.h>

#include "external/rlImGui-main/rlImGui.h"

struct Position {
    int x{}, y{};
};

struct Point {
    Position m_position{};
    Color m_color{};
};

inline double distance(Point a, Point b) {
    const double dx = a.m_position.x - b.m_position.x;
    const double dy = a.m_position.y - b.m_position.y;
    return std::sqrt(dx * dx + dy * dy);
}

class Application {
public:
    Application();
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    void run();

    static Color getColorByIndex(int index);
    static Position getRandomPosition(Rectangle box = {0, 0, 0, 0});

private:
    void draw();
    void update();
    void addPoint();
    void addPoint(Vector2 position);
    void start(int numberOfSections);

    void drawCenters() const;
    [[nodiscard]] Position getCenterPosition(Color color) const;
    [[nodiscard]] Rectangle getBoundingBox() const;

private:
    Camera2D m_camera{};

    bool m_step{false};
    bool m_started{false};
    int m_numberOfSections{};
    bool m_firstStepCompleted{false};

    static constexpr int screenWidth = 1920;
    static constexpr int screenHeight = 1080;

    static constexpr size_t boundingBoxOffset{15};

private:
    std::vector<Point> m_points{};
    std::vector<Point> m_centers{};
};
