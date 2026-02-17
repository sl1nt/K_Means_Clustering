#include "Application.hpp"

void Application::run() {
    draw();
}

void Application::update() {
    if (m_firstStepCompleted) {
        for (auto&[m_position, m_color] : m_centers) {
            m_position = getCenterPosition(m_color);
        }
    }

    for (auto& point: m_points) {
        Point closestCenter = m_centers[0];
        double closestDistance = distance(point, closestCenter);
        for (auto& center : m_centers) {
            if (distance(point, center) < closestDistance) {
                closestCenter = center;
                closestDistance = distance(point, center);
            }
        }
        point.m_color = closestCenter.m_color;
    }
    m_firstStepCompleted = true;
}


void Application::draw() {
    BeginDrawing();
    ClearBackground(Color(50, 50, 70));

    for (const auto&[position, color] : m_points) {
        DrawCircle(position.x, position.y, 4.0f, color);
    }

    rlImGuiBegin();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !ImGui::GetIO().WantCaptureMouse) {
        addPoint(GetMousePosition());
    }

    if (!m_started) {
        static int numberOfSections{2};
        ImGui::SliderInt("Number of sections", &numberOfSections, 2, 7);
        if (ImGui::Button("Start")) {
            start(numberOfSections);
        }
    }
    else if (m_started) {
        if (ImGui::Button("Step")) {
            update();
        }
        auto [x, y, width, height] = getBoundingBox();
        DrawRectangleLines(x, y, width, height, LIGHTGRAY);
        drawCenters();
    }

    rlImGuiEnd();
    EndDrawing();
}

void Application::start(const int numberOfSections) {
    m_numberOfSections = numberOfSections;
    m_started = true;

    for (int i = 0; i < numberOfSections; i++) {
        m_centers.emplace_back(Point{
            getRandomPosition(getBoundingBox()),
            getColorByIndex(i),
        });
    }

}

Application::Application() {
    InitWindow(screenWidth, screenHeight, "Screen");
    rlImGuiSetup(true);

    SetTargetFPS(60);
}

Application::~Application() {
    rlImGuiShutdown();
    CloseWindow();
}

void Application::drawCenters() const {
    for (const auto&[m_position, m_color]: m_centers) {
        DrawCircle(m_position.x, m_position.y, 9.0f, m_color);
    }
}

Rectangle Application::getBoundingBox() const {
    assert(m_started);

    int minX = screenWidth;
    int minY = screenHeight;
    int maxX = 0;
    int maxY = 0;
    for (const auto& [position, _] : m_points) {
        if (position.x < minX) {
            minX = position.x;
        }
        if (position.x > maxX) {
            maxX = position.x;
        }
        if (position.y < minY) {
            minY = position.y;
        }
        if (position.y > maxY) {
            maxY = position.y;
        }
    }

    return Rectangle{
        static_cast<float>(minX - boundingBoxOffset),
        static_cast<float>(minY - boundingBoxOffset),
        static_cast<float>(maxX - minX + boundingBoxOffset * 2),
        static_cast<float>(maxY - minY + boundingBoxOffset * 2)
    };
}

bool operator==(const Color& lhs, const Color& rhs) {
    return lhs.a == rhs.a && lhs.b == rhs.b && lhs.g == rhs.g && lhs.r == rhs.r;
};

Position Application::getCenterPosition(const Color color) const {
    int sumX{0};
    int sumY{0};
    int count{0};
    bool hasFailed{false};

    for (const auto& [point, col]: m_points) {
        if (color == col) {
            sumX += point.x;
            sumY += point.y;
            count++;
            hasFailed = true;
        }
    }

    assert(hasFailed && "failed");

    return { sumX/count, sumY/count };
}

Color Application::getColorByIndex(const int index) {
    assert(index < 7 && index >= 0);
    switch (index) {
        case 0: return RED;
        case 1: return GREEN;
        case 2: return BLUE;
        case 3: return YELLOW;
        case 4: return PURPLE;
        case 5: return ORANGE;
        case 6: return BROWN;
        default: static_assert("unreachable");
    }
    static_assert("unreachable");
    return RED;
}

bool IsSameRectangle(const Rectangle& first, const Rectangle& second) {
    return first.x == second.x && first.y == second.y && first.width == second.width && first.height == second.height;
}

Position Application::getRandomPosition(const Rectangle box) {
    static std::random_device rd;
    std::mt19937 gen(rd());

    if (box.x == 0 && box.y == 0 && box.width == 0 && box.height == 0) {
        std::uniform_int_distribution<> distX(0 + 15, screenWidth - 15);
        std::uniform_int_distribution<> distY(0 + 15, screenHeight - 15);
        return Position{ distX(gen), distY(gen) };
    }
    std::uniform_int_distribution<> distX(
        static_cast<int>(box.x) + 15,
        static_cast<int>(box.x + box.width) - 15
    );
    std::uniform_int_distribution<> distY(
        static_cast<int>(box.y) + 15,
        static_cast<int>(box.y + box.height) - 15
    );
    return Position{ distX(gen), distY(gen) };
}

void Application::addPoint() {
    m_points.emplace_back(Point{
        getRandomPosition(),
        LIGHTGRAY
    });
}

void Application::addPoint(const Vector2 position) {
    m_points.emplace_back(Point{{
        static_cast<int>(position.x),
        static_cast<int>(position.y)},
        LIGHTGRAY
    });
}
