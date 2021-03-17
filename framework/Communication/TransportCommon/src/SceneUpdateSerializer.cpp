//  -------------------------------------------------------------------------
//  Copyright (C) 2020 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "TransportCommon/SceneUpdateSerializer.h"
#include "TransportCommon/SingleSceneUpdateWriter.h"

namespace ramses_internal
{
    SceneUpdateSerializer::SceneUpdateSerializer(const SceneUpdate& update, StatisticCollectionScene& sceneStatistics)
        : m_update(update)
        , m_sceneStatistics(sceneStatistics)
    {
    }

    bool SceneUpdateSerializer::writeToPackets(absl::Span<Byte> packetMem, const std::function<bool(size_t)>& writeDoneFunc) const
    {
        SingleSceneUpdateWriter writer(m_update, packetMem, writeDoneFunc, m_sceneStatistics);
        return writer.write();
    }

    const SceneUpdate& SceneUpdateSerializer::getUpdate() const
    {
        return m_update;
    }

    const StatisticCollectionScene& SceneUpdateSerializer::getStatisticCollection() const
    {
        return m_sceneStatistics;
    }
}
