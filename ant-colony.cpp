#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

const int ENV_SIZE = 100;
const int NUM_ANTS = 50;
const int MAX_STEPS = 1000; // maximum number of steps in the simulation

struct Environment {
    int food[ENV_SIZE][ENV_SIZE] = {};
    double pheromone[ENV_SIZE][ENV_SIZE] = {};
};

class Ant {
private:
    int x, y;
    bool hasFood;

    void boundPosition() {
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x >= ENV_SIZE) x = ENV_SIZE - 1;
        if (y >= ENV_SIZE) y = ENV_SIZE - 1;
    }

public:
    Ant(int startX, int startY) : x(startX), y(startY), hasFood(false) {}

    void move(Environment &env) {
        int dx = (rand() % 3) - 1;
        int dy = (rand() % 3) - 1;
        x += dx;
        y += dy;
        boundPosition(); // ensure the ant stays within the environment
    }

    void searchForFood(Environment &env) {
        // example logic for finding food (can be more complex)
        if (env.food[x][y] > 0) {
            hasFood = true;
            env.food[x][y]--;
        }
    }

    void returnToNest(Environment &env) {
        // example logic for returning to nest (can be more complex)
        if (x == 0 && y == 0) { // assuming the nest is at (0, 0)
            hasFood = false;
        }
    }

    void layPheromone(Environment &env) {
        env.pheromone[x][y] += 1.0;
    }

    int getX() const { return x; }
    int getY() const { return y; }

    bool getHasFood() const { return hasFood; }
};

void evaporatePheromones(Environment &env) {
    for (int y = 0; y < ENV_SIZE; ++y) {
        for (int x = 0; x < ENV_SIZE; ++x) {
            env.pheromone[x][y] *= 0.99; // reduce pheromone intensity by 1%
        }
    }
}

void renderEnvironment(const Environment &env, const std::vector<Ant> &ants) {
    for (int y = 0; y < ENV_SIZE; ++y) {
        for (int x = 0; x < ENV_SIZE; ++x) {
            bool antFound = false;
            for (const Ant &ant : ants) {
                if (ant.getX() == x && ant.getY() == y) {
                    std::cout << 'A'; // represent an ant
                    antFound = true;
                    break;
                }
            }
            if (!antFound) {
                if (env.food[x][y] > 0) {
                    std::cout << 'F'; // represent food
                } else if (env.pheromone[x][y] > 0) {
                    std::cout << '.'; // represent pheromone
                } else {
                    std::cout << ' '; // empty space
                }
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    srand(time(0));
    Environment env;
    std::vector<Ant> ants;

    for (int i = 0; i < NUM_ANTS; ++i) {
        ants.emplace_back(rand() % ENV_SIZE, rand() % ENV_SIZE);
    }

    for (int timeStep = 0; timeStep < MAX_STEPS; ++timeStep) {
        for (Ant &ant : ants) {
            ant.move(env);
            if (ant.getHasFood()) {
                ant.returnToNest(env);
            } else {
                ant.searchForFood(env);
            }
            ant.layPheromone(env);
        }

        evaporatePheromones(env);

        renderEnvironment(env, ants); // render the environment

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        system("clear");
    }

    return 0;
}
