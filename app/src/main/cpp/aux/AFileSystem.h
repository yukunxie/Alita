//
// Created by realxie on 2019-09-26.
//

#ifndef ALITA_AFILESYSTEM_H
#define ALITA_AFILESYSTEM_H

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <string>

#include "types/TData.h"


class AFileSystem
{
public:
    static AFileSystem* getInstance();
    inline void initAssetManager(AAssetManager* mgr) {androidAssetManager_ = mgr;}

    TData readData(const std::string& filepath);

protected:
    AAssetManager* androidAssetManager_ = nullptr;
    static AFileSystem* instance_;
};


#endif //ALITA_AFILESYSTEM_H
