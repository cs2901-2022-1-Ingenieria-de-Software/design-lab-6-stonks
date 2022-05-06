#include <iostream>
#include <memory>
#include <vector>
/* Declarations */
class LightCommand;
class RemoteControl;

/* Implementations */
class RemoteControl {
protected:
    static std::shared_ptr<RemoteControl> instance;
public:
    static std::shared_ptr<RemoteControl> getInstance() {
        if (instance == nullptr) {
            instance = std::make_shared<RemoteControl>();
        }
        return instance;
    }
    void show() {
        std::cout << "Remote Control" << std::endl;
    }
};

std::shared_ptr<RemoteControl> RemoteControl::instance = nullptr;

/* Light */
class Light {
protected:
    bool status;
public:
    int intensity;
    Light() {
        intensity = 0;
        status = false;
    }
    void TurnOn() {
        status = true;
    }
    void TurnOff() {
        status = false;
    }
    void executeCommand(std::shared_ptr<LightCommand> command) {
        command->execute();
    }
};

class LightEditor {
public:
    void increseaseIntensity(std::shared_ptr<Light> light) {
        light->TurnOn();
        light->intensity++;
    }
    void decreseaseIntensity(std::shared_ptr<Light> light) {
        light->TurnOn();
        light->intensity--;
    }
};

class LightCommand {
protected:
    std::shared_ptr<Light> light;
    std::shared_ptr<LightEditor> lightEditor;
public:
    LightCommand(std::shared_ptr<LightEditor> LightEditor) {
        this->lightEditor = lightEditor;
    }
    virtual void execute() = 0 ;// do something
};

class IncreaseIntensity : public LightCommand {
public:
    void execute() {
        lightEditor->upIntensity(light);
    }
};

class DecreaseIntensity : public LightCommand {
public:
    void execute() {
        lightEditor->downIntensity(light);
    }
};
