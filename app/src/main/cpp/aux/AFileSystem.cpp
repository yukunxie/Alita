//
// Created by realxie on 2019-09-26.
//

#include "AFileSystem.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <string>
#include <vector>

AFileSystem* AFileSystem::instance_ = nullptr;

AFileSystem* AFileSystem::getInstance()
{
    if (instance_ == nullptr)
    {
        auto tmp = new AFileSystem();
        instance_ = tmp;
    }
    return instance_;
}


TData AFileSystem::readData(const std::string& filepath)
{
    AAsset* file = AAssetManager_open(androidAssetManager_, filepath.c_str(), AASSET_MODE_BUFFER);
    size_t fileLength = AAsset_getLength(file);

    TData data(fileLength, 0);
    AAsset_read(file, data.data(), data.size());

    return std::move(data);
}