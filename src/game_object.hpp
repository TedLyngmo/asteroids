#pragma once

#include "times.hpp"

class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void Move(duration time) = 0;
    virtual void Draw() = 0;
};

class BulletOwner : public GameObject {
public:
    virtual void AddScore(unsigned) = 0;
};
