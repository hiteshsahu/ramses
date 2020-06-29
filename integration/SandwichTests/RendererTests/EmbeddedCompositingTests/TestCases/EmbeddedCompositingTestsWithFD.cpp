//  -------------------------------------------------------------------------
//  Copyright (C) 2018 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "EmbeddedCompositingTestsWithFD.h"
#include "EmbeddedCompositingTestsFramework.h"
#include "RendererLib/DisplayConfig.h"
#include "DisplayConfigImpl.h"
#include "WaylandUtilities/UnixDomainSocket.h"
#include "WaylandUtilities/WaylandEnvironmentUtils.h"
#include "RendererTestUtils.h"

namespace ramses_internal
{
    void EmbeddedCompositingTestsWithFD::setUpEmbeddedCompositingTestCases(EmbeddedCompositingTestsFramework& testFramework)
    {
        testFramework.createTestCaseWithoutRenderer(FDTest_CanConnectUsingWaylandSocket, *this, "FDTest_CanConnectUsingWaylandSocket");
        testFramework.createTestCaseWithoutRenderer(FDTest_CanConnectUsingWaylandSocket_TwoApplicationsInSequence, *this, "FDTest_CanConnectUsingWaylandSocket_TwoApplicationsInSameTime");
        testFramework.createTestCaseWithoutRenderer(FDTest_WaylandClientCanConnectToSocketBeforeEmbeddedCompositorIsInitialized, *this, "FDTest_WaylandClientCanConnectToSocketBeforeEmbeddedCompositorIsInitialized");
        testFramework.createTestCaseWithoutRenderer(FDTest_WaylandClientCanConnectToSocketBeforeAndAfterEmbeddedCompositorIsInitialized, *this, "FDTest_WaylandClientCanConnectToSocketBeforeAndAfterEmbeddedCompositorIsInitialized");
        testFramework.createTestCaseWithoutRenderer(FDTest_CanConnectUsingWaylandSocketAndWaylandDisplay, *this, "FDTest_CanConnectUsingWaylandSocketAndWaylandDisplay");
    }

    bool EmbeddedCompositingTestsWithFD::runEmbeddedCompositingTestCase(EmbeddedCompositingTestsFramework& testFramework, const RenderingTestCase& testCase)
    {
        UnixDomainSocket socket(m_waylandSocket, WaylandEnvironmentUtils::GetVariable(WaylandEnvironmentVariable::XDGRuntimeDir));

        ramses::RendererConfig rendererConfig = RendererTestUtils::CreateTestRendererConfig();
        rendererConfig.setWaylandEmbeddedCompositingSocketName("");
        rendererConfig.setWaylandEmbeddedCompositingSocketGroup("");
        rendererConfig.setWaylandEmbeddedCompositingSocketFD(socket.createBoundFileDescriptor());

        const UInt32 displayWidth = ramses_internal::IntegrationScene::DefaultDisplayWidth;
        const UInt32 displayHeight = ramses_internal::IntegrationScene::DefaultDisplayHeight;
        const float aspectRatio = static_cast<float>(displayWidth) / displayHeight;

        ramses::DisplayConfig displayConfig = RendererTestUtils::CreateTestDisplayConfig(0, true);

        displayConfig.setWindowRectangle(0, 0, displayWidth, displayHeight);
        displayConfig.setPerspectiveProjection(19.f, aspectRatio, 0.1f, 1500.f);

        switch(testCase.m_id)
        {
        case FDTest_CanConnectUsingWaylandSocket:
        {
            testFramework.initializeRenderer(rendererConfig);

            testFramework.createDisplay(displayConfig);

            testFramework.setEnvironmentVariableWaylandSocket();

            testFramework.startTestApplicationAndWaitUntilConnected();
            testFramework.stopTestApplicationAndWaitUntilDisconnected();

            testFramework.destroyDisplays();
            testFramework.destroyRenderer();
            return true;
        }
        case FDTest_CanConnectUsingWaylandSocket_TwoApplicationsInSequence:
        {
            testFramework.initializeRenderer(rendererConfig);
            testFramework.createDisplay(displayConfig);

            testFramework.setEnvironmentVariableWaylandSocket();

            testFramework.startTestApplicationAndWaitUntilConnected();
            testFramework.stopTestApplicationAndWaitUntilDisconnected();

            //reconnect
            testFramework.setEnvironmentVariableWaylandSocket();

            testFramework.startTestApplicationAndWaitUntilConnected();
            testFramework.stopTestApplicationAndWaitUntilDisconnected();

            testFramework.destroyDisplays();
            testFramework.destroyRenderer();
            return true;
        }
        case FDTest_WaylandClientCanConnectToSocketBeforeEmbeddedCompositorIsInitialized:
        {
            testFramework.setEnvironmentVariableWaylandSocket();
            testFramework.startTestApplication();

            testFramework.initializeRenderer(rendererConfig);
            testFramework.createDisplay(displayConfig);

            testFramework.waitUntilNumberOfCompositorConnections(1);
            testFramework.stopTestApplicationAndWaitUntilDisconnected();

            testFramework.destroyDisplays();
            testFramework.destroyRenderer();
            return true;
        }
        case FDTest_WaylandClientCanConnectToSocketBeforeAndAfterEmbeddedCompositorIsInitialized:
        {
            //before EC init
            testFramework.setEnvironmentVariableWaylandSocket();
            testFramework.startTestApplication();

            testFramework.initializeRenderer(rendererConfig);
            testFramework.createDisplay(displayConfig);

            testFramework.waitUntilNumberOfCompositorConnections(1);
            testFramework.stopTestApplicationAndWaitUntilDisconnected();

            //reconnect
            testFramework.setEnvironmentVariableWaylandSocket();

            testFramework.startTestApplicationAndWaitUntilConnected();
            testFramework.stopTestApplicationAndWaitUntilDisconnected();

            testFramework.destroyDisplays();
            testFramework.destroyRenderer();
            return true;
        }
        case FDTest_CanConnectUsingWaylandSocketAndWaylandDisplay:
        {
            testFramework.initializeRenderer(rendererConfig);
            testFramework.createDisplay(displayConfig);

            //connect using wayland socket
            testFramework.setEnvironmentVariableWaylandSocket();

            testFramework.startTestApplicationAndWaitUntilConnected();
            testFramework.stopTestApplicationAndWaitUntilDisconnected();

            //connect using wayland display
            testFramework.setEnvironmentVariableWaylandDisplay();

            testFramework.startTestApplicationAndWaitUntilConnected();
            testFramework.stopTestApplicationAndWaitUntilDisconnected();

            testFramework.destroyDisplays();
            testFramework.destroyRenderer();
            return true;
        }
        default:
            assert(false);
            return false;
        }
    }
}
