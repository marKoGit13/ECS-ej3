#pragma once
#include <memory> // Para std::unique_ptr

// Enum para identificar rápidamente el tipo de evento
enum class EventType {
    Damage,
    Spawn
};


struct Event {
    virtual ~Event() = default;
    virtual EventType getType() const = 0;
    virtual std::unique_ptr<Event> clone() const = 0;
};


struct DamageEvent : public Event {
    EventType getType() const override { return EventType::Damage; }
    std::unique_ptr<Event> clone() const override { 
        return std::make_unique<DamageEvent>(*this); 
    }
};


struct SpawnEvent : public Event {
    EventType getType() const override { return EventType::Spawn; }
    std::unique_ptr<Event> clone() const override { 
        return std::make_unique<SpawnEvent>(*this); 
    }
};