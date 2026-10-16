#pragma once

#include <raylib.h>
#include <stdlib.h>
#include <math.h>

constexpr int MAX_PARTICLES {5000};
constexpr int FPS {60};

struct Particle
{
    Vector2 position;
    Vector2 velocity;

    float radius {10.0f};
    float life_time;

    Color color {PINK};

    bool alive {true};
};

struct CircularBuffer
{
    int head {};
    int tail {};

    Particle* buffer;
};

[[gnu::always_inline]] inline static Particle* add_to_circular_buffer(CircularBuffer* circ_buff)
{
    Particle* particle {nullptr};

    // buffer full check
    if (((circ_buff->head + 1) % MAX_PARTICLES) != circ_buff->tail)
    {
        // add particle to head and move head forward
        particle = &circ_buff->buffer[circ_buff->head];
        circ_buff->head = (circ_buff->head + 1) % MAX_PARTICLES;
    }

    return particle;
}

[[gnu::always_inline]] inline static void emit_particle(CircularBuffer* circ_buff, Vector2& emitter_pos)
{
    Particle* new_particle {add_to_circular_buffer(circ_buff)};

    if (new_particle != nullptr)
    {
        new_particle->position  = emitter_pos;
        new_particle->alive     = true;
        new_particle->life_time = 0.0f;

        float speed {static_cast<float>((rand() % 10) / 5.0f)};

        float direction {static_cast<float>(rand() % 360)};

        Vector2 particle_velocity {speed * cosf(direction * DEG2RAD), speed * sinf(direction * DEG2RAD)};
        new_particle->velocity = particle_velocity;
    }
}

[[gnu::always_inline]] inline static void update_particles(CircularBuffer* circ_buff, 
        const int scr_w, const int scr_h)
{
    for (int i {circ_buff->tail}; i != circ_buff->head; i = (i + 1) % MAX_PARTICLES)
    {
        circ_buff->buffer[i].life_time += 1.0f / FPS;

        // update position and velocity
        circ_buff->buffer[i].position.x += circ_buff->buffer[i].velocity.x;
        circ_buff->buffer[i].velocity.y += 0.2f; // downwards
        circ_buff->buffer[i].position.y += circ_buff->buffer[i].velocity.y;

        Vector2 center {circ_buff->buffer[i].position};
        float radius {circ_buff->buffer[i].radius};

        if ((center.x < -radius) || (center.x > (scr_w + radius)) ||
            (center.y < -radius) || (center.y > (scr_h + radius)))
        {
            circ_buff->buffer[i].alive = false;
        }
    }
}

[[gnu::always_inline]] inline static void update_circular_buffer(CircularBuffer* circ_buff)
{
    while ((circ_buff->tail != circ_buff->head) && !circ_buff->buffer[circ_buff->tail].alive)
    {
        circ_buff->tail = (circ_buff->tail + 1) % MAX_PARTICLES;
    }
}

[[gnu::always_inline]] inline static void draw_particles(CircularBuffer* circ_buff)
{
    for (int i {circ_buff->tail}; i != circ_buff->head; i = (i + 1) % MAX_PARTICLES)
    {
        if (circ_buff->buffer[i].alive)
        {
            DrawCircleV
            (
             circ_buff->buffer[i].position, 
             12.0f,
             BLUE
             // circ_buff->buffer[i].radius, 
             // circ_buff->buffer[i].color
            );
        }
    }
}
