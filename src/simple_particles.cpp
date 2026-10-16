#include "particle.hpp"

constexpr int WIN_WIDTH  {1280};
constexpr int WIN_HEIGHT {720};

constexpr float WIN_CENTER_W {static_cast<float>(WIN_WIDTH)  * 0.5f};
constexpr float WIN_CENTER_H {static_cast<float>(WIN_HEIGHT) * 0.5f};

// constexpr int FPS {60};

#if 0
// move the entire update into a single function, later
void update()
{
}
#endif

int main()
{
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Simple Particle System");

    SetTargetFPS(FPS);

    // RL_CALLOC, is a RayLib macro, which is a C-style custom allocator calloc(num, size)
    Particle* particles = reinterpret_cast<Particle*>(RL_CALLOC(MAX_PARTICLES, sizeof(Particle)));
    CircularBuffer circ_buff {0, 0, particles};

    int emission_rate {-2};

    Vector2 emitter_pos {WIN_CENTER_W, WIN_CENTER_H};

    while (!WindowShouldClose())
    {
        // call before drawing
        if (emission_rate < 0)
        {
            if (rand() % (-emission_rate) == 0)
            {
                emit_particle(&circ_buff, emitter_pos);
            }
        }
        else
        {
            for (int i {}; i <= emission_rate; ++i)
            {
                emit_particle(&circ_buff, emitter_pos);
            }
        }

        update_particles(&circ_buff, WIN_WIDTH, WIN_HEIGHT);
        update_circular_buffer(&circ_buff);

        if (IsKeyPressed(KEY_UP))
        {
            emission_rate += 1;
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            emission_rate -= 1;
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            emitter_pos = GetMousePosition();
        }

        BeginDrawing();
            
        ClearBackground(BLACK);
        DrawText("Simple Particle System", (WIN_CENTER_W - 100), 0, 20, WHITE);

        draw_particles(&circ_buff);

        DrawText(TextFormat("Particles per frame: %d", emission_rate + 1), 0, 0, 20, WHITE);

        DrawFPS(WIN_WIDTH - 80, 10);

        EndDrawing();
    }

    RL_FREE(particles);

    CloseWindow();
}
