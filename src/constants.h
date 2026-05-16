#ifndef SRC_CONSTANTS_H
#define SRC_CONSTANTS_H

// Window & Screen dimensions
inline constexpr int SCREEN_WIDTH = 800;
inline constexpr int SCREEN_HEIGHT = 600;
inline constexpr const char *WINDOW_TITLE = "Complex AI Top Down";

// Player
inline constexpr float PLAYER_SPEED = 300.0f;
inline constexpr float PLAYER_SIZE = 50.0f;
inline constexpr int PLAYER_MAX_HP = 5;
inline constexpr float PLAYER_INVULN_TIME_MS = 1000.0f;
inline constexpr float PLAYER_FIRE_COOLDOWN_MS = 150.0f;

// Player Bullet
inline constexpr float BULLET_SPEED = 500.0f;
inline constexpr float BULLET_WIDTH = 10.0f;
inline constexpr float BULLET_HEIGHT = 20.0f;

// Enemy type parameters
inline constexpr float ENEMY_NORMAL_SPEED = 150.0f;
inline constexpr float ENEMY_FAST_SPEED = 250.0f;
inline constexpr float ENEMY_TANK_SPEED = 80.0f;
inline constexpr float ENEMY_NORMAL_SIZE = 40.0f;
inline constexpr float ENEMY_FAST_SIZE = 30.0f;
inline constexpr float ENEMY_TANK_SIZE = 60.0f;
inline constexpr int ENEMY_NORMAL_HP = 3;
inline constexpr int ENEMY_FAST_HP = 1;
inline constexpr int ENEMY_TANK_HP = 10;

// Enemy behavior
inline constexpr float ENEMY_MOVE_TO_FORMATION_SPEED_RATIO = 3.0f;
inline constexpr float ENEMY_SWAY_AMPLITUDE = 40.0f;
inline constexpr float ENEMY_DIVE_SPEED = 350.0f;
inline constexpr float ENEMY_EVADE_SPEED = 350.0f;
inline constexpr float ENEMY_EVADE_TIME_MS = 400.0f;
inline constexpr float ENEMY_EVADE_RANGE = 150.0f;
inline constexpr float ENEMY_FOLLOW_OFFSET_Y = 150.0f;
inline constexpr float ENEMY_FOLLOW_SPEED_RATIO = 0.8f;

// Enemy bullet
inline constexpr float ENEMY_BULLET_SPEED = 200.0f;
inline constexpr float ENEMY_BULLET_WIDTH = 8.0f;
inline constexpr float ENEMY_BULLET_HEIGHT = 16.0f;

// Wave spawning
inline constexpr int WAVE_COLUMNS = 6;
inline constexpr float WAVE_SPAWN_INTERVAL_MS = 6000.0f;
inline constexpr float WAVE_START_X = 150.0f;
inline constexpr float WAVE_SPACING_X = 100.0f;
inline constexpr float WAVE_START_Y = 40.0f;

// Particles
inline constexpr float PARTICLE_SIZE = 2.0f;
inline constexpr float PARTICLE_SPEED_MIN = 50.0f;
inline constexpr float PARTICLE_SPEED_MAX = 150.0f;
inline constexpr float PARTICLE_LIFETIME_MIN_MS = 300.0f;
inline constexpr float PARTICLE_LIFETIME_MAX_MS = 500.0f;

// Performance Overlay
inline constexpr int OVERLAY_PANEL_WIDTH = 200;
inline constexpr int OVERLAY_PANEL_HEIGHT = 100;
inline constexpr int OVERLAY_MARGIN = 10;
inline constexpr int PERF_HISTORY_SIZE = 100;
inline constexpr float FPS_TARGET_MS = 16.6f;

// Collision threshold
inline constexpr float COLLISION_EPSILON = 0.001f;

#endif // SRC_CONSTANTS_H
