//
// Created by nicomane on 02/06/22.
//

#include "App.h"

namespace Engine{

    void App::updateCamera(float dt){
        if(glfwGetKey(window, GLFW_KEY_A)) {
            Camera::currentCam->onA(dt);
        }
        if(glfwGetKey(window, GLFW_KEY_S)) {
            Camera::currentCam->onS(dt);
        }
        if(glfwGetKey(window, GLFW_KEY_W)) {
            Camera::currentCam->onW(dt);
        }
        if(glfwGetKey(window, GLFW_KEY_D)) {
            Camera::currentCam->onD(dt);
        }
        if(glfwGetKey(window, GLFW_KEY_E)) {
            Camera::currentCam->onE(dt);
        }
        if(glfwGetKey(window, GLFW_KEY_Q)) {
            Camera::currentCam->onQ(dt);
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN)) {
			Camera::currentCam->onUp(dt);
		}
		if(glfwGetKey(window, GLFW_KEY_UP)) {
			Camera::currentCam->onDown(dt);
		}
        if(glfwGetKey(window, GLFW_KEY_LEFT)) {
            Camera::currentCam->onLeft(dt);
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT)) {
            Camera::currentCam->onRight(dt);
        }

        if(glfwGetKey(window, GLFW_KEY_SPACE)) {
            Camera::currentCam->onSpace(dt);
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
            Camera::currentCam->onShift(dt);
        }
        if(glfwGetKey(window, GLFW_KEY_P)){
            float inc = Camera::currentCam->farPlane + 0.1;
            Camera::currentCam->setFarPlane(inc);
            std::cout<<"ADD ZOOM -> " <<Camera::currentCam->farPlane<<"\n";
        }

        if(glfwGetKey(window, GLFW_KEY_M)){
            float inc = Camera::currentCam->farPlane - 0.1;
            if(inc < 0 ) inc = 0;
            Camera::currentCam->setFarPlane(inc);
            std::cout<<"REMOVE ZOOM -> " <<Camera::currentCam->farPlane<<"\n";
        }

    }
    void App::updateScene(uint32_t currentFrame) {
        //TODO CREATE A CLASS TO MANAGE TIME
        static float lastTime = 0.0f;
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        float deltaT = time - lastTime;
        lastTime = time;

        /********UPDATE CAMERA*************/
        updateCamera(deltaT);

        /********UPDATE MOVABLE OBJECT*************/
        //TODO ACCESS "MOVABLE OBJECTS IN A SEPARATE LOOP"
        for (auto entity : *movableEntities)
        {
            entity->update(deltaT);
        }
        /********CALCULATE STATIC MESHES POSITION*************/
        updateMeshesPos(currentFrame,time);

    }
    //TODO ADD A GRAPHIC PIPELINE WITH ONLY VERTEX/INDEX WITH NO TEXTURE (FOR THE TERRAIN GENERATOR)
    void App::updateMeshesPos(int currentFrame,float time) {
        //******UPDATE LIGHTS DATA THROUGH UNIFORM BUFFER************
        globalDescriptor->getUniformBuffer().updateGlobal(currentFrame);

        //*******SIMPLY UPDATE UNIFOMR BUFFERS OF ALL MESHES**********
        for (auto mesh : *Mesh::meshes)
        {
            mesh->updateUniformBuffer(currentFrame);
        }

        double xpos, ypos;
        bool click = clickHandle(time,&xpos,&ypos);
        /********Buttons  CLICK Handle*************************/
        for(auto mesh : UImanager::interface.getUI()){
            mesh->updateUniformBuffer(currentFrame);
            if(click) {
                mesh->isClicked(xpos,ypos);
            }
        }


    }

    /**
     * Handle click by returning true if click occurred, and copying x,y coordinates on param
     * @param time
     * @param xpos
     * @param ypos
     * @return
     */
    bool App::clickHandle(float time,double * xpos,double * ypos){
        glfwGetCursorPos(window, xpos, ypos);
        bool click = false;
        glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            click = antiDebounce(time);
        }

        return click;

    }

    /**
     * Check if there is a debounce on this click (confront lastTimeClick)
     * @param time
     * @return
     */
    bool App::antiDebounce(float time){
        static float lastClickTime = 0.0f;

        float delta = time - lastClickTime;
        if(delta > 0.33){
            lastClickTime = time;
            return true;
        }
        return false;
    }

    void App::customInit() {

    }
}