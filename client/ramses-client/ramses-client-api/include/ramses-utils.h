//  -------------------------------------------------------------------------
//  Copyright (C) 2014 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef RAMSES_RAMSES_UTILS_H
#define RAMSES_RAMSES_UTILS_H

#include "ramses-framework-api/RamsesFrameworkTypes.h"
#include "ramses-client-api/TextureEnums.h"
#include "ramses-framework-api/APIExport.h"
#include <vector>
#include "ramses-client-api/TextureSwizzle.h"

namespace ramses
{
    class RamsesObject;
    class Effect;
    class Node;
    class RamsesClient;
    class Texture2D;
    class UniformInput;
    struct MipLevelData;
    struct CubeMipLevelData;

    /**
     * @brief Temporary functions for convenience. All of these can be implemented on top
     * of the RAMSES Client API, but are offered here as convenience.
     */
    class RAMSES_API RamsesUtils
    {
    public:
        /**
        * @brief Converts object to a compatible object type.
        * Object can be converted to any of its base classes.
        * Eg. MeshNode can be converted to Node, SceneObject, ClientObject or RamsesObject.
        *
        * @param[in] obj RamsesObject to convert.
        * @return Pointer to an object of a specific type,
        *         NULL if object type is not compatible with desired object class.
        */
        template <typename T>
        static const T* TryConvert(const RamsesObject& obj);

        /**
        * @copydoc TryConvert()
        **/
        template <typename T>
        static T* TryConvert(RamsesObject& obj);

        /**
        * @brief Creates a Texture from the given png file.
        *
        * @param[in] pngFilePath Path to the png file to load
        * @param[in] ramsesClient Client the texture object is to be created in
        * @param[in] swizzle Swizzling of texture color channels
        * @param[in] name Name for the created texture
        * @return Created texture object or nullptr on error
        */
        static Texture2D*  CreateTextureResourceFromPng(const char* pngFilePath, RamsesClient& ramsesClient, const TextureSwizzle& swizzle = {}, const char* name = nullptr);

        /**
        * @brief Creates a Texture from the given png memory buffer.
        *
        * @param[in] pngData Buffer with PNG data to load
        * @param[in] ramsesClient Client the texture object is to be created in
        * @param[in] swizzle Swizzling of texture color channels
        * @param[in] name Name for the created texture
        * @return Created texture object or nullptr on error
        */
        static Texture2D*  CreateTextureResourceFromPngBuffer(const std::vector<unsigned char>& pngData, RamsesClient& ramsesClient, const TextureSwizzle& swizzle = {}, const char* name = nullptr);

        /**
        * @brief Generate mip maps from original texture 2D data. You obtain ownership of all the
        *        data returned in the mip map data object.
        * Note, that the original texture data gets copied and represents the first mip map level.
        * @see DeleteGeneratedMipMaps for deleting generated mip maps.
        * @param[in] width Width of the original texture.
        * @param[in] height Height of the original texture.
        * @param[in] bytesPerPixel Number of bytes stored per pixel in the original texture data.
        * @param[in] data Original texture data.
        * @param[out] mipMapCount Number of generated mip map levels.
        * @return generated mip map data. In case width or height are not values to the power of two,
        *         only the original mip map level is part of the result.
        *         You are responsible to destroy the generated data, e.g. by using RamsesUtils::DeleteGeneratedMipMaps
        */
        static MipLevelData* GenerateMipMapsTexture2D(uint32_t width, uint32_t height, uint8_t bytesPerPixel, uint8_t* data, uint32_t& mipMapCount);

        /**
        * @brief Generate mip maps from original texture cube data. You obtain ownership of all the
        *        data returned in the mip map data object.
        * Note, that the original texture data gets copied and represents the first mip map level.
        * @see DeleteGeneratedMipMaps for deleting generated mip maps.
        * @param[in] faceWidth Width of the original texture.
        * @param[in] faceHeight Height of the original texture.
        * @param[in] bytesPerPixel Number of bytes stored per pixel in the original texture data.
        * @param[in] data Original texture data. Face data is expected in order [PX, NX, PY, NY, PZ, NZ]
        * @param[out] mipMapCount Number of generated mip map levels.
        * @return generated mip map data. In case width or height are not values to the power of two,
        *         only the original mip map level is part of the result.
        *         You are responsible to destroy the generated data, e.g. using RamsesUtils::DeleteGeneratedMipMaps
        */
        static CubeMipLevelData* GenerateMipMapsTextureCube(uint32_t faceWidth, uint32_t faceHeight, uint8_t bytesPerPixel, uint8_t* data, uint32_t& mipMapCount);

        /**
        * @brief Deletes mip map data created with RamsesUtils::GenerateMipMapsTexture2D.
        * @param[in, out] data Generated mip map data.
        * @param[in] mipMapCount Number of mip map levels in the generated data.
        */
        static void DeleteGeneratedMipMaps(MipLevelData*& data, uint32_t mipMapCount);

        /**
        * @brief Deletes mip map data created with RamsesUtils::GenerateMipMapsTextureCube.
        * @param[in, out] data Generated mip map data.
        * @param[in] mipMapCount Number of mip map levels in the generated data.
        */
        static void DeleteGeneratedMipMaps(CubeMipLevelData*& data, uint32_t mipMapCount);

        /**
        * @brief Returns the identifier of a node, which is printed in the renderer logs. The identifier is guaranteed to be
        * unique within a Scene until the Node is destroyed. If a Node is destroyed, a newly created Node can get the identifier
        * of the destroyed Node.
        * @param[in] node The node
        * @return the identifier of the given node.
        */
        static nodeId_t GetNodeId(const Node& node);
    };
}

#endif
