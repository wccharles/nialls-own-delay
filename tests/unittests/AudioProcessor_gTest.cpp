#include "../../Source/PluginProcessor.h"
#include <gtest/gtest.h>

TEST(ExampleTest, CreatePluginAndGUITest)
{
    juce::ScopedJuceInitialiser_GUI juceInitialiser;
    auto                            messageManager = juce::MessageManager::getInstance();
    messageManager->setCurrentThreadAsMessageThread();

    auto pluginInstance = std::make_unique<ModDelayAudioProcessor>();
    EXPECT_NE(pluginInstance, nullptr);
    auto gui = pluginInstance->createEditorIfNeeded();
    EXPECT_NE(gui, nullptr);

    messageManager->runDispatchLoopUntil(200);

    pluginInstance->editorBeingDeleted(gui);
    delete gui;

    pluginInstance.reset();
    messageManager->deleteInstance();
}
