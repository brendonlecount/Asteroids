#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include "GameObject.h"

using namespace std;
using namespace sf;

GameObject::GameObject(RenderWindow* window, float radius, float mass, Vector2f position, Vector2f velocity) {
	this->window = window;
	shape.setOrigin(Vector2f(radius, radius));
	shape.setRadius(radius);
	shape.setPosition(position);
	this->velocity = velocity;
	this->mass = mass;
}

void GameObject::ApplyVelocity(float deltaTime) {
	Vector2f newPosition = shape.getPosition() + velocity * deltaTime;
	Vector2f windowSize = (Vector2f)window->getSize();
	if (newPosition.x > windowSize.x) {
		newPosition.x -= windowSize.x;
	}
	else if (newPosition.x < 0.f) {
		newPosition.x += windowSize.x;
	}
	if (newPosition.y > windowSize.y) {
		newPosition.y -= windowSize.y;
	}
	else if (newPosition.y < 0.f) {
		newPosition.y += windowSize.y;
	}

	shape.setPosition(newPosition);
}