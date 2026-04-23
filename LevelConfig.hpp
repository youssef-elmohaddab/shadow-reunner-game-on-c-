#ifndef LevelConfig_h
#define LevelConfig_h

#include <algorithm>

struct LevelConfig
{
    int   level;
    float goalX;          // world X where the bunker sits
    float baseSpeed;      // reference player speed
    float timeLimit;      // seconds allowed
    float groundObsSpeed; // spider leftward speed (px/s)
    float airObsSpeed;    // bat/bird leftward speed (px/s)
    float spawnDensity;   // min gap between obstacles (smaller = harder)
};

inline LevelConfig makeLevelConfig(int level)
{
    const float baseSpeed = 350.f;

    const float goalX = 10000.f + 555.f*level;

    const float timeLimit    = goalX / (baseSpeed * (1.f + level * 0.08f));
    const float groundObsSpd = 80.f  + level * 12.f;
    const float airObsSpd    = 160.f + level * 25.f;
    const float spawnDensity = std::max(500.f - level * 35.f, 200.f);

    return { level, goalX, baseSpeed, timeLimit,
             groundObsSpd, airObsSpd, spawnDensity };
}

#endif
