#pragma once

struct scene
{
    virtual ~scene() = default;
    virtual void Init() = 0;
    virtual void Update(float delta_time) = 0;
};
