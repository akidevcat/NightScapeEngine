#pragma once

namespace NSE
{
    class SceneServer;
    class Scene;

    class EntitySystem : public RefCounted
    {
    public:
        EntitySystem();
        ~EntitySystem() override = default;

        void Enable();
        void Disable();

        const std::string& GetName() const { return _name; }

    protected:
        virtual void OnSetup() {}
        virtual void OnUpdate() {}
        virtual void OnDispose() {}

        virtual void OnEnabled() {}
        virtual void OnDisabled() {}

        void SetName(const std::string& name) { _name = name; }

    private:
        std::string _name = "New Scene System";
        bool _isEnabled = true;

        friend SceneServer;
        friend Scene;
    };
}