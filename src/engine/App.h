#ifndef ROCKETSIMULATOR_APP_H
#define ROCKETSIMULATOR_APP_H
#pragma once
#include "commonLibs.h"
#include "Device/DeviceManager.h"
#include "Device/LogicDeviceManager.h"
#include "Presentation/WindowsSurface.h"
#include "Drawing/CommandBuffer.h"
#include "Drawing/FrameBuffer.h"
#include "Presentation/SwapChain.h"
#include "GraphicPipeline/GraphicPipeline.h"
#include "Drawing/Renderer.h"
#include "Drawing/VertexBuffer.h"
#include <memory>

namespace Engine{
    class App {
    private:
        int width;
        int height;
        bool framebufferResized = false;

        std::string name;
        GLFWwindow* window;
        VkInstance instance;
        DeviceManager devicesManager;
        LogicDeviceManager logicDeviceManager;
        WindowsSurface windowsSurface;
        FrameBuffer frameBuffer;
        CommandBuffer commandBuffer;
        VertexBuffer vertexBuffer;
        SwapChain swapChain;
        GraphicPipeline graphicPipeline;
        Renderer renderer;
    public:
        App(){}

        App(int w,int h,std::string win_name)
        {
            width = w;
            height = h;
            name = win_name;
        }
    private:
        void init();
        void initVulkan();
        void initWindow();
        void main();
        void appLoop();
        void close();
        void createInstance();
        void printExtensionNames();
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        /**
          * Cleen the swap chain before recreation
          */
        void cleanupSwapChain();

        GLFWwindow* getWindow();
    public:
        void run();
        void loop();
        /**
        * Recreate the swapchain, called if old swap chain is no more usefull (eg window resize)
        */
        void recreateSwapChain();
        void resetResizeFlag(){
            framebufferResized = false;
        }
        bool getResizeFlag() {
            return framebufferResized;
        }
    };


}

#endif //ROCKETSIMULATOR_APP_H
