#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <unordered_map>
/* Declarations */
using namespace std;
class Light;
class RemoteControl;
class LightCommand;
class SetIntensity;
// COMMAND INTERFACE
class LightCommand {
protected:
    std::shared_ptr<Light> light;
public:
    explicit LightCommand(std::shared_ptr<Light> _light): light{std::move(_light)} {}
    virtual ~LightCommand()= default;
    virtual void execute() = 0 ;
};
// LIGHT OBJECT
class Light {
private:
    bool status;
    float intensity;
    string color;
public:
    explicit Light(string cng) {
        intensity = 0;
        status = false;
        color = std::move(cng);
    }
    [[nodiscard]] bool isOn() const {
        return status;
    }
    void turnOn() {
        status = true;
    }
    void turnOff() {
        status = false;
    }
    void setIntensity(float _intensity) {
        this->intensity = _intensity;
    }
    void show() const {
        cout << "Color: " << color << ", " << "intensidad: " << intensity << endl;
    }
};
// LIGHT COMMANDS
class SetIntensity : public LightCommand{
private:
    float intensity;
public :
    explicit SetIntensity(std::shared_ptr<Light> light, float intensity):
        LightCommand{std::move(light)},
        intensity{intensity}
        {}
    void execute() override {
        light->setIntensity(intensity);
    }
};
class ShowStatus : public LightCommand {
public:
    explicit ShowStatus(std::shared_ptr<Light> light): LightCommand{std::move(light)} {}
    void execute() override {
        light->show();
    }
};
/* Implementations */
class RemoteControl {
protected:
    static std::shared_ptr<RemoteControl> instance;
    std::unordered_map<std::string,std::shared_ptr<Light>> lights;
public:
    static std::shared_ptr<RemoteControl> getInstance() {
        if (instance == nullptr) {
            instance = std::make_shared<RemoteControl>();
        }
        return instance;
    }
    void createLight(const std::string& light) {
        if (lights.find(light) != lights.end()) throw std::invalid_argument("Luz ya existe");
        instance->lights[light] = make_shared<Light>(light);
    }
    void setLight(const std::string& light, float intensity) {
        if (lights.find(light) == lights.end()) throw std::invalid_argument("Luz no existe");
        SetIntensity(lights[light], intensity).execute();
    }
    void showLight() {
        cout << "Lights Status: " << endl;
        cout << "---------------" << endl;
        for (auto &i: lights) {
            ShowStatus(i.second).execute();
        }
        cout << "---------------" << endl << endl;
    }
};
std::shared_ptr<RemoteControl> RemoteControl::instance = nullptr;
void tests() {
    std::shared_ptr<RemoteControl> control = RemoteControl::getInstance();
    control->createLight("light2");
    control->setLight("light1", 0.5);
    control->showLight();
    auto light_1 = std::make_shared<Light>("light3");
    light_1->setIntensity(0.5);
    light_1->turnOff();
    light_1->turnOn();
    light_1->show();
    SetIntensity setint(light_1, 0.5);
    setint.execute();
}
int main(){
    auto remote = RemoteControl::getInstance();
    // Instanciar Luces
    remote->showLight();
    remote->createLight("Green");
    remote->createLight("Orange");
    remote->createLight("Red");
    remote->createLight("Stop");
    remote->showLight();
    remote->setLight("Green", 0.5);
    remote->setLight("Red", 0.5);
    remote->showLight();
}
