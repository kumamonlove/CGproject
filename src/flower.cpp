#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

#include "base/camera.h"
#include "flower.h"

#define randFloat(from,to) (from + (to - from) * 1.0 * rand() / RAND_MAX)

FlowerSystem::FlowerSystem()
{
    const char *vsCode =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPosition;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"

        "out vec2 TexCoord;\n"
        "out vec3 FragPos;\n"

        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"

        "void main() {\n"
        "	TexCoord = aTexCoord;\n"
        "	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
        "	FragPos = vec3(model * vec4(aPosition, 1.0));\n"
        "}\n";

    const char *fsCode =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 TexCoord;\n"
        "in vec3 FragPos;\n"

        "uniform sampler2D s;\n"

        "void main() {\n"
        "	vec4 texcolor=texture(s, TexCoord);"
        "	if(texcolor.r < 0.5) \n"
        "	discard;\n"
        "	vec4 ambient  =vec4(vec3(texcolor),0);\n"
        "	vec4 res=(ambient);\n"
        "   FragColor = res;"
        "}\n";

    shader = new GLSLProgram;
    shader->attachVertexShader(vsCode);
    shader->attachFragmentShader(fsCode);
    shader->link();
}

void FlowerSystem::update(GLfloat deltaTime)
{
    const int maxLifespan = 3000;
    const int minNewNum = 1;
    const int maxNewNum = 10;

    int newNum = minNewNum + rand() % (maxNewNum - minNewNum + 1);
    if (newNum == 1)
    {
        Flower flower;
        flower.position = glm::vec3(randFloat(-10.0f, 10.0f), randFloat(10.0f, 25.0f), randFloat(-10.0f, 10.0f));
        flower.velocity = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(-1.0f, -2.0f), randFloat(-1.0f, 1.0f));
        flower.rotation = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f));
        flower.rotation_i = randFloat(0.0f, 180);
        flower.statical = false;
        flower.life = maxLifespan;
        flowers.push_back(flower);
    }

    for (auto it = flowers.begin(); it != flowers.end();)
    {
        it->position += deltaTime * it->velocity;

        if (!it->statical && it->position.y <= 0.1f && std::abs(it->position.x) <= 5.0f && std::abs(it->position.z) <= 5.0f)
        {
            it->position.y = 0.1f;
            it->velocity = glm::vec3(0, 0, 0);
            it->rotation_i = 90;
            it->rotation = glm::vec3(1, 0, 0);
            it->statical = true;
            it->rotation_y = 1 + rand() % (180 - 1 + 1);
        }

        it->life--;
        if (it->position.y < 0.0f || it->life == 0)
        {
            it = flowers.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void FlowerSystem::draw(Camera *camera)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);
    if (type == 0)
        glActiveTexture(GL_TEXTURE20);
    else
        glActiveTexture(GL_TEXTURE21);

    texture[type]->bind();
    shader->use();
    if (type == 0)
        shader->setInt("s", 20);
    else
        shader->setInt("s", 21);

    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();

    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    for (int i = 0; i < flowers.size(); i++)
    {
        glm::mat4 model;
        model = glm::translate(glm::mat4(1.0f), flowers[i].position);
        model = glm::rotate(model, flowers[i].rotation_i, flowers[i].rotation);
        if (flowers[i].statical)
            model = glm::rotate(model, flowers[i].rotation_y, glm::vec3(0, 0, 1));

        if (type == 0)
            model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        else
            model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        shader->setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // glDrawElements(GL_POINTS, 6, GL_UNSIGNED_INT, 0);
    }
}
