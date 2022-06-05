//
// Created by nicomane on 02/06/22.
//

#include "UniformBufferManager.h"

namespace Engine{
    void UniformBufferManager::pushUniformBuffer(int i){
        uniformBuffers.push_back(UniformBuffer(bufferManager));
        uniformBuffers.at(i).createUniformBuffers();
    }




    void UniformBufferManager::update(uint32_t currentImage,glm::mat4 modelMatrix) {
        //UPDATE UNIFORM BUFFER OF ALL DESCRIPTOR SETS
        for(int i=0;i<uniformBuffers.size();i++){
            uniformBuffers[i].updateUniformBuffer(currentImage,modelMatrix);
        }
    }

    void UniformBufferManager::clean() {
        for(int i=0;i<uniformBuffers.size();i++){
            uniformBuffers[i].close();
        }
    }
}
