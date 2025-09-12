#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <deque>
#include <mutex>

// Forward declaration to avoid including RtMidi.h in header
class RtMidiIn;

namespace gamma {
namespace midi {

/**
 * @brief Structure to hold MIDI message data
 */
struct MidiMessage {
    std::vector<unsigned char> data;
    double timestamp;
    std::string description;
    
    MidiMessage(const std::vector<unsigned char>& msgData, double time, const std::string& desc)
        : data(msgData), timestamp(time), description(desc) {}
};

/**
 * @brief MIDI Manager for handling DDJ-REV1 and other MIDI controllers
 * 
 * Manages MIDI input devices, processes incoming messages, and provides
 * logging functionality for debugging and setup purposes.
 */
class MidiManager {
public:
    MidiManager();
    ~MidiManager();

    /**
     * @brief Initialize MIDI system
     * @return true if initialization successful
     */
    bool initialize();

    /**
     * @brief Shutdown MIDI system
     */
    void shutdown();

    /**
     * @brief Get list of available MIDI input devices
     * @return vector of device names
     */
    std::vector<std::string> getAvailableDevices();

    /**
     * @brief Connect to a specific MIDI device
     * @param deviceIndex Index of device to connect to
     * @return true if connection successful
     */
    bool connectToDevice(int deviceIndex);

    /**
     * @brief Connect to a specific MIDI device by name
     * @param deviceName Name of device to connect to
     * @return true if connection successful
     */
    bool connectToDevice(const std::string& deviceName);

    /**
     * @brief Refresh the list of available MIDI devices
     */
    void refreshDevices();

    /**
     * @brief Disconnect from current MIDI device
     */
    void disconnect();

    /**
     * @brief Check if connected to a MIDI device
     * @return true if connected
     */
    bool isConnected() const { return _isConnected; }

    /**
     * @brief Get name of currently connected device
     * @return device name or empty string if not connected
     */
    std::string getConnectedDeviceName() const { return _connectedDeviceName; }

    /**
     * @brief Get recent MIDI messages for logging display
     * @param maxMessages Maximum number of messages to return
     * @return vector of recent MIDI messages
     */
    std::vector<MidiMessage> getRecentMessages(size_t maxMessages = 50);

    /**
     * @brief Clear the message log
     */
    void clearMessageLog();

    /**
     * @brief Clear message history (alias for clearMessageLog)
     */
    void clearMessageHistory() { clearMessageLog(); }

    /**
     * @brief Update MIDI system (call each frame)
     */
    void update();

    /**
     * @brief Set callback for jog wheel rotation events
     * @param callback Function to call when jog wheel moves (channel, deltaRotation)
     */
    void setJogWheelCallback(std::function<void(int, float)> callback);

private:
    std::unique_ptr<RtMidiIn> _midiIn;
    bool _isInitialized;
    bool _isConnected;
    std::string _connectedDeviceName;
    int _connectedDeviceIndex;

    // Message logging
    std::deque<MidiMessage> _messageLog;
    std::mutex _messageLogMutex;
    static const size_t MAX_LOG_SIZE = 1000;

    // Callbacks
    std::function<void(int, float)> _jogWheelCallback;

    /**
     * @brief Static callback for MIDI input
     */
    static void midiInputCallback(double deltatime, std::vector<unsigned char>* message, void* userData);

    /**
     * @brief Process incoming MIDI message
     */
    void processMidiMessage(const std::vector<unsigned char>& message, double timestamp);

    /**
     * @brief Convert MIDI message to human-readable description
     */
    std::string describeMidiMessage(const std::vector<unsigned char>& message);
};

} // namespace midi
} // namespace gamma