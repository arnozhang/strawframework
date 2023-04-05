/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <vector>

#include "base/def/PlatformDefine.h"
#include "base/data/Buffer.h"


namespace sf {

    namespace file {

        STRAW_FRAMEWORK_API Buffer* getFileContent(const std::string& file);

        STRAW_FRAMEWORK_API std::string getFileStringContent(const std::string& file);
    }


    namespace path {

        STRAW_FRAMEWORK_API void setCurrentPath(const std::string& currPath);

        STRAW_FRAMEWORK_API const std::string& getCurrentPath();

        STRAW_FRAMEWORK_API bool isPathAbsolutely(const std::string& directory);

        STRAW_FRAMEWORK_API bool combineRelativePath(
            const std::string& relativePath,
            std::string& resultPath);

        STRAW_FRAMEWORK_API bool combineRelativeFilePath(
            const std::string& relativeFilePath,
            std::string& resultFilePath);

        STRAW_FRAMEWORK_API bool isPathOrFileExists(const std::string& directory);

        STRAW_FRAMEWORK_API std::vector<std::string> listFiles(
            const std::string directory, bool containSubPath = false);
    }
}
