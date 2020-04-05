//
// Created by realxie on 2019-11-03.
//

#ifndef ALITA_ENGINE_H
#define ALITA_ENGINE_H

#include "../RHI/Include/RHI.h"
#include "../Base/Macros.h"

#include <functional>
#include <map>

NS_RX_BEGIN

typedef std::function<void(std::uint32_t, float, void*)> FuncTypeEngineUpdateCallback;

struct SchedulerEntity
{
    FuncTypeEngineUpdateCallback callback;
    void* data = nullptr;
};

class World;

class Engine
{
protected:
    Engine(void* data);

public:
    virtual ~Engine();
    
    static Engine* CreateEngine(void* data);
    
    static Engine* GetEngine();

public:
    void Update(float dt);
    
    void RunWithWorld(World* world);
    
    std::uint32_t StartScheduler(FuncTypeEngineUpdateCallback callback, void* data);
    
    void CancelScheduler(std::uint32_t schedulerId);

protected:
    static Engine* engine_;
    World* world_ = nullptr;
    
    std::uint32_t schedulerIDRecorder_ = 0;
    std::map<std::uint32_t, SchedulerEntity> schedulers_;
    
    RHI::Device* gpuDevice_ = nullptr;
};

NS_RX_END


#endif //ALITA_ENGINE_H
