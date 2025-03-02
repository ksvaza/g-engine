#include "../include/engine.hpp"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <winlib.hpp>
#include <hwinputs.hpp>
#include <render.hpp>
#include <mesh.hpp>
#include <shader.hpp>
#include <laygui.hpp>
#include <uibutton.hpp>
#include <uislider.hpp>
#include <uitext.hpp>
#include <texture.hpp>
#include <textureatlas.hpp>

namespace Gengine
{
    int Engine::Update(float deltaTime)
    {
        return 0;
    }
    void Engine::Initialise()
    {
        Gwindow.InitialiseWindow(&window, 864, 486, "Hello, Triangle!");
        Input.Initialise(window, &Gwindow);
        Input.SetTestMode(0);
    }
    void Engine::Terminate()
    {
        Gwindow.Terminate();
    }
    int Engine::Run()
    {
        Shader shader;
        shader.Read("shaders/UIshader_vertex.glsl", GL_VERTEX_SHADER);
        shader.Read("shaders/UIshader_fragment.glsl", GL_FRAGMENT_SHADER);
        shader.Compile();

        Layout.SetUIshader(shader);
        Layout.SetInput(&Input);
        Layout.SetWindow(&Gwindow);
        glm::mat4 viewMatrix = glm::mat4(1.0);
        glm::mat4 projectionMatrix = OrthographicMatrix(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
        Layout.SetUIviewMatrix(viewMatrix);
        Layout.SetUIprojectionMatrix(projectionMatrix);
        
        Texture baltsTexture;
        baltsTexture.Load("textures/baltsApalsStarpMums.bmp");
        baltsTexture.LoadData("textures/baltsApalsStarpMums.bmp");

        Texture greenTexture;
        greenTexture.Load("textures/zalsStarpMums.bmp");
        greenTexture.LoadData("textures/zalsStarpMums.bmp");

        Texture redTexture;
        redTexture.LoadData("textures/sarkansApalsStarpMums.bmp");

        TextureAtlas atlas;
        atlas.AddTexture(&greenTexture);
        atlas.AddTexture(&baltsTexture);
        atlas.AddTexture(&redTexture);
        atlas.Bake("textures/white.bmp");

        G_UIelement leftPanel;
        {
            Mesh leftPanelMesh;
            MeshGen.RegularShape(&leftPanelMesh, G_RECTANGLE);
            leftPanelMesh.transform = NewTransform();
            leftPanelMesh.transform.position = glm::vec3(0.0, /*44*/0.0, 0.0);
            leftPanelMesh.transform.scale = glm::vec3(400.0, /*200.0*/1080.0, 1.0);
            leftPanelMesh.SetColour(glm::vec4(0.5, 0.5, 0.5, 1.0));

            Layout.CreateElement(&leftPanel, G_PARENT);
            leftPanel.mesh = leftPanelMesh;
            leftPanel.transform = NewTransform();
            leftPanel.transform.position = glm::vec3(200.0, 540.0, 0.0);
        }

        GUI_button leftPanelTopButton;
        leftPanelTopButton.Create(glm::vec2(380.0, 50.0), glm::vec2(0.0, 505.0), glm::vec4(0.7, 0.7, 0.7, 1.0));
        leftPanelTopButton.SetReferenceLayout(&Layout);
        leftPanelTopButton.AddAsChild(&leftPanel);

        GUI_slider leftPanelSlider;
        leftPanelSlider.Create(0.0, 5.0, GUI_HORIZONTAL, glm::vec2(320.0, 50.0), glm::vec2(30.0, 50.0), glm::vec2(-30.0, 445.0), glm::vec4(0.4, 0.4, 0.4, 1.0), glm::vec4(0.7, 0.7, 0.7, 1.0));
        leftPanelSlider.SetReferenceLayout(&Layout);
        leftPanelSlider.AddAsChild(&leftPanel);

        GUI_button leftPanelSliderButton;
        leftPanelSliderButton.Create(glm::vec2(50.0, 50.0), glm::vec2(165.0, 445.0), glm::vec4(0.7, 0.7, 0.7, 1.0));
        leftPanelSliderButton.SetReferenceLayout(&Layout);
        leftPanelSliderButton.AddAsChild(&leftPanel);

        G_UIelement leftPanelSquare;
        {
            Mesh leftPanelSquareMesh;
            MeshGen.RegularShape(&leftPanelSquareMesh, G_RECTANGLE);
            leftPanelSquareMesh.transform = NewTransform();
            leftPanelSquareMesh.transform.position = glm::vec3(0.0, 0.0, 0.0);
            leftPanelSquareMesh.transform.scale = glm::vec3(200.0, 200.0, 1.0);
            leftPanelSquareMesh.SetColour(glm::vec4(1.0));
            leftPanelSquareMesh.AddTexture(baltsTexture);

            Layout.CreateElement(&leftPanelSquare, G_MESH);
            leftPanelSquare.mesh = leftPanelSquareMesh;
            leftPanelSquare.transform = NewTransform();
            leftPanelSquare.transform.position = glm::vec3(0.0, 0.0, 0.0);
        }
        Layout.AddChild(&leftPanel, &leftPanelSquare);

        Texture font;
        font.LoadData("fonts/SUS-8.png");

        TextFont susFont; // fonts/SUS-8
        susFont.Load("fonts/JetBrainsMonoRegular.fnt");

        GUI_text textTest;
        textTest.Create("Hello, World!", glm::vec2(380.0, 50.0), glm::vec2(-184.0, 28.0), glm::vec4(1.0, 1.0, 1.0, 1.0), susFont, 40);
        textTest.SetReferenceLayout(&Layout);
        textTest.AddAsChild(leftPanelTopButton.Element());

        TextFont fontArial;
        fontArial.Load("fonts/arial.fnt");
        TextFont fontJetBrainsMonoRegular;
        fontJetBrainsMonoRegular.Load("fonts/JetBrainsMonoRegular.fnt");
        TextFont fontPerpetua;
        fontPerpetua.Load("fonts/Perpetua.fnt");
        TextFont fontTimesNewRoman;
        fontTimesNewRoman.Load("fonts/TimesNewRoman.fnt");

        G_UIelement textBox;
        {
            Mesh textBoxMesh;
            MeshGen.Rectangle(&textBoxMesh, glm::vec2(760.0, 180.0));
            textBoxMesh.transform = NewTransform();
            textBoxMesh.SetColour(glm::vec4(0.8, 0.7, 0.5, 1.0));

            Layout.CreateElement(&textBox, G_PARENT);
            textBox.mesh = textBoxMesh;
            textBox.transform = NewTransform();
            textBox.transform.position = glm::vec3(190.0, -440.0, 0.0);
        }

        GUI_text textArial;
        textArial.Create("Hello, Arial!", glm::vec2(380.0, 50.0), glm::vec2(-380.0, 80.0), glm::vec4(1.0, 1.0, 1.0, 1.0), fontArial, 40);
        textArial.AddAsChild(&textBox);
        GUI_text textJetBrainsMonoRegular;
        textJetBrainsMonoRegular.Create("Hello, JetBrains Mono Regular!", glm::vec2(380.0, 50.0), glm::vec2(-380.0, 40.0), glm::vec4(1.0, 1.0, 1.0, 1.0), fontJetBrainsMonoRegular, 40);
        textJetBrainsMonoRegular.AddAsChild(&textBox);
        GUI_text textPerpetua;
        textPerpetua.Create("Hello, Perpetua!", glm::vec2(380.0, 50.0), glm::vec2(-380.0, 0.0), glm::vec4(1.0, 1.0, 1.0, 1.0), fontPerpetua, 40);
        textPerpetua.AddAsChild(&textBox);
        GUI_text textTimesNewRoman;
        textTimesNewRoman.Create("Hello, Times New Roman!", glm::vec2(380.0, 50.0), glm::vec2(-380.0, -40.0), glm::vec4(1.0, 1.0, 1.0, 1.0), fontTimesNewRoman, 40);
        textTimesNewRoman.AddAsChild(&textBox);

        Layout.AddChild(&leftPanel, &textBox);

        Layout.AddElement(&leftPanel);

        G_UIelement testTexturing;
        {
            Mesh testTexturingMesh;
            MeshGen.RegularShape(&testTexturingMesh, G_RECTANGLE);
            testTexturingMesh.transform = NewTransform();
            testTexturingMesh.transform.scale = glm::vec3(200.0, 200.0, 1.0);
            testTexturingMesh.SetColour(glm::vec4(1.0));

            Layout.CreateElement(&testTexturing, G_MESH);
            testTexturing.mesh = testTexturingMesh;
            testTexturing.transform = NewTransform();
            testTexturing.transform.position = glm::vec3(960.0, 540.0, 0.0);

            G_UIelement testTexturingSprite1;
            Mesh testTexturingSprite1Mesh;
            MeshGen.RegularShape(&testTexturingSprite1Mesh, G_RECTANGLE);
            testTexturingSprite1Mesh.transform = NewTransform();
            testTexturingSprite1Mesh.transform.scale = glm::vec3(100.0, 100.0, 1.0);
            testTexturingSprite1Mesh.SetColour(glm::vec4(1.0));
            testTexturingSprite1Mesh.AddTexture(redTexture);
            testTexturingSprite1Mesh.FillTextureID(0);
            Layout.CreateElement(&testTexturingSprite1, G_MESH);
            testTexturingSprite1.mesh = testTexturingSprite1Mesh;
            testTexturingSprite1.transform = NewTransform();
            testTexturingSprite1.transform.position = glm::vec3(-50.0, 0.0, 0.0);
            Layout.AddChild(&testTexturing, &testTexturingSprite1);

            G_UIelement testTexturingSprite2;
            Mesh testTexturingSprite2Mesh;
            MeshGen.RegularShape(&testTexturingSprite2Mesh, G_RECTANGLE);
            testTexturingSprite2Mesh.transform = NewTransform();
            testTexturingSprite2Mesh.transform.scale = glm::vec3(100.0, 100.0, 1.0);
            testTexturingSprite2Mesh.SetColour(glm::vec4(1.0));
            testTexturingSprite2Mesh.AddTexture(greenTexture);
            testTexturingSprite2Mesh.FillTextureID(0);
            Layout.CreateElement(&testTexturingSprite2, G_MESH);
            testTexturingSprite2.mesh = testTexturingSprite2Mesh;
            testTexturingSprite2.transform = NewTransform();
            testTexturingSprite2.transform.position = glm::vec3(50.0, 0.0, 0.0);
            Layout.AddChild(&testTexturing, &testTexturingSprite2);
        }
        Layout.AddElement(&testTexturing);

        GUI_text fpsCounter;
        char* fpsString = (char*)malloc(20);
        fpsCounter.Create("FPS: 60.00", glm::vec2(100.0, 50.0), glm::vec2(0.0, 1080.0), glm::vec4(1.0, 0.6, 0.0, 1.0), susFont, 50);
        fpsCounter.SetReferenceLayout(&Layout);
        fpsCounter.AddToLayout();
        
        Layout.Compile();
        
        const uint8_t fpsSmoothness = 20;
        float fpsArray[fpsSmoothness];
        for (uint8_t i = 0; i < fpsSmoothness; i++) { fpsArray[i] = 60.0; }

        TotalTime = 0.0f;
        float deltaTime = 0.0f;
        float lastAngle = 0.0f;
        uint32_t timeSinceLastAngle = 0;
        while (!glfwWindowShouldClose(window))
        {
            glfwSetTime((double)0.0);

            Update(deltaTime);

            Render.Clear(glm::vec4(1.0));
            Layout.Update();
            Input.Update();

            // Calculate the FPS
            for (uint8_t i = 0; i < fpsSmoothness - 1; i++) { fpsArray[i] = fpsArray[i + 1]; }
            fpsArray[fpsSmoothness - 1] = 1.0 / deltaTime;
            float fpsSum = 0.0;
            for (uint8_t i = 0; i < fpsSmoothness; i++) { fpsSum += fpsArray[i]; }
            float fps = fpsSum / fpsSmoothness;

            // Update the FPS counter
            sprintf(fpsString, "FPS: %3.1f", fps);
            fpsCounter.textContent = fpsString;
            fpsCounter.UpdateText();
            Layout.RecalculateSupermesh(fpsCounter.TextElement());

            // Update the button text
            /*textTest.textContent = fpsString;
            textTest.UpdateText();
            Layout.RecalculateSupermesh(textTest.TextElement());*/

            // Korrigieren die Mesh aus Mouse Position
            glm::vec3 deltaMouseWorldPos = glm::vec3(Input.ConvertPixelToWorldSpace(Input.Mouse.MouseDeltaPosition, glm::vec2(Gwindow.Width, Gwindow.Height), viewMatrix, projectionMatrix), 0.0);
            glm::vec3 mouseWorldPos = glm::vec3(Input.ConvertPixelToWorldSpace(Input.Mouse.MousePosition, glm::vec2(Gwindow.Width, Gwindow.Height), viewMatrix, projectionMatrix), 0.0);
            //printf("Mouse world position: %f, %f\n", deltaMouseWorldPos.x, deltaMouseWorldPos.y);
            if (leftPanelSliderButton.IsPressed())
            {
                /*float speed = leftPanelSlider.Value();
                if (Input.Keyboard.Key[GLFW_KEY_UP])
                {
                    leftPanel.transform.position.y += deltaTime * speed * 100.0;
                }
                if (Input.Keyboard.Key[GLFW_KEY_DOWN])
                {
                    leftPanel.transform.position.y += deltaTime * speed * -100.0;
                }
                if (Input.Keyboard.Key[GLFW_KEY_LEFT])
                {
                    leftPanel.transform.position.x += deltaTime * speed * -100.0;
                }
                if (Input.Keyboard.Key[GLFW_KEY_RIGHT])
                {
                    leftPanel.transform.position.x += deltaTime * speed * 100.0;
                }*/

                // Calculate the relative position of the mouse to the leftPanelSquare in the middle of the left panel
                glm::vec3 relativeMousePos = mouseWorldPos - leftPanel.transform.position;

                // Calculate the angle of the mouse position relative to the leftPanelSquare
                float angle = atan2(relativeMousePos.y, relativeMousePos.x) * 180.0 / 3.14159265359;

                //printf("Delta angle: %f\n", angle - lastAngle);

                // Apply the delta angle to the leftPanelSquare
                if (timeSinceLastAngle <= 1)
                {
                    leftPanel.transform.rotation.z += angle - lastAngle;
                    Layout.RecalculateSupermesh(&leftPanelSquare);
                    
                }
                lastAngle = angle;
                timeSinceLastAngle = 0;
            }
            else if (leftPanelTopButton.IsPressed())
            {
                leftPanel.transform.position += deltaMouseWorldPos;
            }
            timeSinceLastAngle++;

            /*leftPanel.transform.rotation = glm::vec3(0.0, 0.0, 360.0 * leftPanelSlider.Value() / 5.0);*/
            
            Layout.DrawElements();

            glfwSwapBuffers(window);
            glfwPollEvents();
            
            TotalTime += deltaTime;
            deltaTime = (float)glfwGetTime();
        }

        // The code for termination is suspiciously slow, probably due to the recursive deletion of elements // -- fixed
        // crashes instead of terminating // -- fixed
        shader.Delete();
        Layout.RemoveElement(&leftPanel);
        Layout.DeleteElement(&leftPanel);

        // This does not print // -- fixed
        printf("Window closed after %f seconds\n", TotalTime);

        return 0;
    }
}
