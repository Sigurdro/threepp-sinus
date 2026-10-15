#include "threepp/threepp.hpp"
#include <vector>
#include <cmath>

using namespace threepp;

int main() {
    Canvas canvas("threepp");
    GLRenderer renderer(canvas);

    auto scene = Scene::create();
    scene->background = Color::aliceblue;

    auto camera = PerspectiveCamera::create(
        60, canvas.aspect(), 0.1f, 1000
    );
    camera->position.z = 15;

    canvas.onWindowResize([&](WindowSize size) {
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
    });

    auto light = DirectionalLight::create();
    light->position.set(1, 2, 3);
    scene->add(light);
    scene->add(AmbientLight::create(0xffffff, 0.4f));

    const int antall = 8;
    const float avstand = 1.5f;

    std::vector<std::shared_ptr<Mesh>> bokser;

    auto geometry = BoxGeometry::create(1, 1, 1);
    std::vector<std::shared_ptr<MeshStandardMaterial>> materialer;

    for (int i = 0; i < antall; ++i) {
        auto material = MeshStandardMaterial::create();
        material->color = Color::blue;

        auto box = Mesh::create(geometry, material);

        box->position.x = (i - (antall - 1) / 2.0f) * avstand;

        scene->add(box);
        bokser.push_back(box);
        materialer.push_back(material);
    }

    int tastetrykk = 0;

    canvas.onKeyPressed([&](KeyEvent evt) {
        if (evt.key == Key::SPACE) {
            const int gruppe = tastetrykk % 2;
            const bool brukOransje = (tastetrykk / 2) % 2 == 0;

            for (int i = gruppe; i < antall; i += 2) {
                materialer[i]->color =
                    brukOransje ? Color::orange : Color::blue;
            }

            ++tastetrykk;
        }
    });

    Clock clock;

    const float amplitude = 1.0f;
    const float frekvens = 0.5f;

    canvas.animate([&] {
        const float t = clock.getElapsedTime();

        for (int i = 0; i < antall; ++i) {
            const float fase = (i % 2 == 0) ? 0.0f : math::PI;

            bokser[i]->position.y =
                amplitude * std::sin(2.0f * math::PI * frekvens * t + fase);
        }

        renderer.render(*scene, *camera);
    });
}