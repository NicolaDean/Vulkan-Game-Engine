#include "RocketSimulator.h"

namespace RocketSimulator{

    /***
     * USER CAN LOAD MODELS AND MODIFY CAMERA BEFORE PROGRAM START HERE
     *
     * WE CAN ALSO DEFINE CUSTOM PIPELINES IF WE WANT ONE DIFFERENT FROM THE DEFAULT
     *
     * (models can bind the default descriptor or bind a new one defined here)
     * /REMEMBER TO MODIFY THE NUMBER OF MODELS IN THE CREATE POOL FUNCTION WHEN ADDING MODEL
     */
    void RocketSimulator::customInit() {

        /****************CAMERA SETTINGS*******************************************/
        Camera * cam0 = new Camera(LOOK_IN_DIRECTION,ORTOGONALE);
        cam0->setPosition(glm::vec3(50,1,0));
        Camera::addCamera(cam0); //CAM 0
       // Camera::currentCam->setPosition({1,1,-1});
        Camera::currentCam->setAngle({0.1,0,0});
        Camera::currentCam->setNearPlane(0.1);
        Camera::currentCam->setFarPlane(21.0);

        Camera * cam1 = new Camera(LOOK_IN_DIRECTION,ORTOGONALE);
        Camera::addCamera(cam1); //CAM 1

        Camera::switchCamera(0); //SET CAM 0 as current Cam
        /****************CUSTOM DESCRIPTOR LAYOUTS*********************************/

        //Descriptor For Terrain: (Only Uniform Buffer, No Texture)
        DescriptorManager* terrainDescriptor = descriptorFactory();
        terrainDescriptor->pushBindingDescriptor({0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT});
        terrainDescriptor->createDescriptorSetLayouts();

        /****************CUSTOM PIPELINES******************************************/
        std::cout<<"Creating new Terrain Pipeline\n";
       GraphicPipelineCustom* terrainPipeline = pipelineFactory("./src/Shaders/compiledShaders/vertNoTexture.spv",
                                                   "./src/Shaders/compiledShaders/fragNoTexture.spv",
                                                   {terrainDescriptor});
       GraphicPipelineCustom* skyboxPipeline = pipelineFactory("./src/Shaders/compiledShaders/vertSkybox.spv",
                                                                 "./src/Shaders/compiledShaders/fragSkybox.spv",
                                                                 {&descManager});

       /****************LOAD ALL MODELS OF THE APP**********************************/
        //PLANE
        Map * map = new Map(50,100,100,bufferManager);
        map->init();
        map->initDescriptor(terrainDescriptor);
        map->bindPipeline(terrainPipeline);
        map->setPos(glm::vec3(-15.0f,-0.5f,-15.0f));
        map->populateMapWithRandomObject(&descManager,&graphicPipelineCustom);
        Mesh::meshes->push_back(map);

        //MODEL 2 ROCKET
        Rocket* m2 = new Rocket(bufferManager);
        m2->init();
        m2->bindPipeline(&graphicPipelineCustom);
        m2->initDescriptor(&descManager);
        m2->setPos(glm::vec3(1.0f,1.0f,1.0f));
        m2->setScale(0.03);
        m2->setAngles(glm::vec3(1.0f,0.0f,0.0f));
        Mesh::meshes->push_back(m2);
        this->subscribeMovable(m2);
        m2->trajectory(glm::vec3(5.0f,1.0f,5.0f),3.0f,0.5);

        Skybox* sky =new  Skybox(bufferManager,"./src/Textures/Sky_Night/Night");
        //Skybox* sky = new  Skybox(bufferManager,"./src/Textures/Skybox_Default/Skybox");
        std::cout<<"ADDRESS:" <<sky<<"\n";
        sky->init();
        sky->initDescriptor(&descManager);
        sky->bindPipeline(skyboxPipeline);
        Mesh::meshes->push_back(sky);

        //BEFOR PASSING A FUNCTION TO BUTTON NEED TO BIND THE FUNCTION TYPE (eg Rocket::launch) and OBJECT POINTER (eg m2)
        auto onLaunchClick = std::bind(&Rocket::launch, m2); //Bind method launch of Rocket to actual object m2
        //auto onCameraClick = std::bind(&Camera::change, m2); //Bind method launch of Rocket to actual object m2

        UImanager::addButton("./src/Textures/UI/launch.png", onLaunchClick,-0.9f,0.9f,0.2f,0.2f);
        UImanager::addButton("./src/Textures/UI/moon.png", onLaunchClick,-0.7f,0.9f,0.2f,0.2f);
        UImanager::addButton("./src/Textures/UI/target.png", onLaunchClick,-0.5f,0.9f,0.2f,0.2f);

    }

}