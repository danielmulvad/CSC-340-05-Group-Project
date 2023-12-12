#include "./BaseMessenger.h"
#include <iostream>

/**
 * Constructor for BaseMessenger.
 * Initializes the messenger in a non-running state.
 */
BaseMessenger::BaseMessenger() : running(false) {}

/**
 * Destructor for BaseMessenger.
 * Performs any necessary cleanup when the messenger object is destroyed.
 */
BaseMessenger::~BaseMessenger() {}

/**
 * Processes a received message.
 *
 * This function iterates through registered message handlers and invokes the appropriate one
 * if the message content matches a registered path. If the debug flag is set, it prints
 * information about the message processing.
 *
 * @param connectionId The ID of the connection from which the message was received.
 * @param msg The message object containing the content to be processed.
 * @param debug A boolean flag that, when true, enables printing debug information.
 */
void BaseMessenger::processReceivedMessage(const int &connectionId, const Message &msg, const bool debug)
{
    for (const auto &entry : router)
    {
        if (msg.content.find(entry.first) != std::string::npos)
        {
            if (debug)
            {
                std::cout << "Received a message: " << msg.toString() << std::endl;
            }
            entry.second(connectionId, msg);
            return;
        }
    }
    std::cout << "Received an unhandled message: " << msg.toString() << std::endl;
};

/**
 * Registers a handler for a specific message path.
 *
 * Adds a new message handler to the messenger's routing table. Each handler is associated
 * with a specific path, and incoming messages are dispatched to the corresponding handler
 * based on their content.
 *
 * @param path The path (or key) to associate with the handler.
 * @param handler The MessageHandler function to be called when a message with the given path is received.
 */
void BaseMessenger::registerHandler(const std::string &path, MessageHandler handler)
{
    router.emplace(path, std::move(handler));
}
