#include "midi/MidiManager.h"
#include "RtMidi.h"
#include <iostream>
#include <sstream>
#include <iomanip>

namespace gamma {
namespace midi {

MidiManager::MidiManager()
    : _midiIn(nullptr)
    , _isInitialized(false)
    , _isConnected(false)
    , _connectedDeviceIndex(-1) {
}

MidiManager::~MidiManager() {
    shutdown();
}

bool MidiManager::initialize() {
    if (_isInitialized) {
        return true;
    }

    try {
        _midiIn = std::make_unique<RtMidiIn>();
        _isInitialized = true;
        std::cout << "MIDI system initialized successfully" << std::endl;
        return true;
    } catch (RtMidiError& error) {
        std::cerr << "MIDI initialization error: " << error.getMessage() << std::endl;
        return false;
    }
}

void MidiManager::shutdown() {
    disconnect();
    
    if (_midiIn) {
        _midiIn.reset();
    }
    
    _isInitialized = false;
    std::cout << "MIDI system shutdown" << std::endl;
}

std::vector<std::string> MidiManager::getAvailableDevices() {
    std::vector<std::string> devices;
    
    if (!_midiIn) {
        return devices;
    }

    try {
        unsigned int portCount = _midiIn->getPortCount();
        
        for (unsigned int i = 0; i < portCount; i++) {
            std::string portName = _midiIn->getPortName(i);
            devices.push_back(portName);
        }
    } catch (RtMidiError& error) {
        std::cerr << "Error getting MIDI devices: " << error.getMessage() << std::endl;
    }

    return devices;
}

bool MidiManager::connectToDevice(int deviceIndex) {
    if (!_midiIn || deviceIndex < 0) {
        return false;
    }

    try {
        // Disconnect from current device if connected
        disconnect();

        // Get device count
        unsigned int portCount = _midiIn->getPortCount();
        if (static_cast<unsigned int>(deviceIndex) >= portCount) {
            std::cerr << "MIDI device index out of range" << std::endl;
            return false;
        }

        // Connect to the device
        _midiIn->openPort(deviceIndex);
        _midiIn->setCallback(&MidiManager::midiInputCallback, this);
        
        // Don't ignore sysex, timing, or active sensing messages
        _midiIn->ignoreTypes(false, false, false);

        _isConnected = true;
        _connectedDeviceIndex = deviceIndex;
        _connectedDeviceName = _midiIn->getPortName(deviceIndex);

        std::cout << "Connected to MIDI device: " << _connectedDeviceName << std::endl;
        
        // Log connection message
        {
            std::lock_guard<std::mutex> lock(_messageLogMutex);
            _messageLog.emplace_back(std::vector<unsigned char>(), 0.0, 
                "Connected to: " + _connectedDeviceName);
        }
        
        return true;
    } catch (RtMidiError& error) {
        std::cerr << "MIDI connection error: " << error.getMessage() << std::endl;
        _isConnected = false;
        return false;
    }
}

void MidiManager::disconnect() {
    if (_midiIn && _isConnected) {
        try {
            _midiIn->closePort();
            std::cout << "Disconnected from MIDI device: " << _connectedDeviceName << std::endl;
            
            // Log disconnection message
            {
                std::lock_guard<std::mutex> lock(_messageLogMutex);
                _messageLog.emplace_back(std::vector<unsigned char>(), 0.0, 
                    "Disconnected from: " + _connectedDeviceName);
            }
        } catch (RtMidiError& error) {
            std::cerr << "MIDI disconnection error: " << error.getMessage() << std::endl;
        }
    }

    _isConnected = false;
    _connectedDeviceName.clear();
    _connectedDeviceIndex = -1;
}

std::vector<MidiMessage> MidiManager::getRecentMessages(size_t maxMessages) {
    std::lock_guard<std::mutex> lock(_messageLogMutex);
    
    std::vector<MidiMessage> messages;
    
    size_t startIndex = 0;
    if (_messageLog.size() > maxMessages) {
        startIndex = _messageLog.size() - maxMessages;
    }
    
    for (size_t i = startIndex; i < _messageLog.size(); i++) {
        messages.push_back(_messageLog[i]);
    }
    
    return messages;
}

void MidiManager::clearMessageLog() {
    std::lock_guard<std::mutex> lock(_messageLogMutex);
    _messageLog.clear();
}

void MidiManager::setJogWheelCallback(std::function<void(int, float)> callback) {
    _jogWheelCallback = callback;
}

void MidiManager::update() {
    // RtMidi handles input via callbacks, so not much to do here
    // Could be used for periodic cleanup or status updates
}

bool MidiManager::connectToDevice(const std::string& deviceName) {
    if (!_midiIn) {
        return false;
    }

    try {
        // Find device by name
        unsigned int portCount = _midiIn->getPortCount();
        int deviceIndex = -1;
        
        for (unsigned int i = 0; i < portCount; i++) {
            std::string portName = _midiIn->getPortName(i);
            if (portName == deviceName) {
                deviceIndex = static_cast<int>(i);
                break;
            }
        }
        
        if (deviceIndex == -1) {
            std::cerr << "MIDI device not found: " << deviceName << std::endl;
            return false;
        }
        
        return connectToDevice(deviceIndex);
    } catch (RtMidiError& error) {
        std::cerr << "MIDI device search error: " << error.getMessage() << std::endl;
        return false;
    }
}

void MidiManager::refreshDevices() {
    // Force RtMidi to refresh device list by recreating the input object
    if (_midiIn) {
        bool wasConnected = _isConnected;
        std::string previousDevice = _connectedDeviceName;
        
        // Disconnect if connected
        if (_isConnected) {
            disconnect();
        }
        
        try {
            // Recreate MIDI input to refresh device list
            _midiIn.reset();
            _midiIn = std::make_unique<RtMidiIn>();
            
            std::cout << "MIDI device list refreshed" << std::endl;
            
            // Try to reconnect to previous device if it was connected
            if (wasConnected && !previousDevice.empty()) {
                connectToDevice(previousDevice);
            }
        } catch (RtMidiError& error) {
            std::cerr << "MIDI refresh error: " << error.getMessage() << std::endl;
        }
    }
}

void MidiManager::midiInputCallback(double deltatime, std::vector<unsigned char>* message, void* userData) {
    MidiManager* manager = static_cast<MidiManager*>(userData);
    if (manager && message) {
        manager->processMidiMessage(*message, deltatime);
    }
}

void MidiManager::processMidiMessage(const std::vector<unsigned char>& message, double timestamp) {
    if (message.empty()) {
        return;
    }

    std::string description = describeMidiMessage(message);
    
    // Check for jog wheel messages and call callback if set
    if (_jogWheelCallback && message.size() >= 3) {
        unsigned char status = message[0];
        unsigned char cc = message[1];
        unsigned char value = message[2];
        
        // DDJ-REV1 jog wheel mappings
        bool isJogMessage = false;
        int channel = 0;
        float deltaRotation = 0.0f;
        
        if (status == 0xB0) { // Channel 1 CC
            if (cc == 0x21 || cc == 0x22) { // CC21 or CC22 (finger on/off)
                channel = 1;
                isJogMessage = true;
            }
        } else if (status == 0xB1) { // Channel 2 CC
            if (cc == 0x21 || cc == 0x22) { // CC21 or CC22 (finger on/off)
                channel = 2;
                isJogMessage = true;
            }
        }
        
        if (isJogMessage) {
            // Determine rotation direction and amount
            if (value == 0x41) {
                deltaRotation = 0.2f; // Clockwise
            } else if (value == 0x3F) {
                deltaRotation = -0.2f; // Counter-clockwise
            }
            
            if (deltaRotation != 0.0f) {
                _jogWheelCallback(channel, deltaRotation);
            }
        }
    }
    
    // Add to message log
    {
        std::lock_guard<std::mutex> lock(_messageLogMutex);
        _messageLog.emplace_back(message, timestamp, description);
        
        // Keep log size manageable
        if (_messageLog.size() > MAX_LOG_SIZE) {
            _messageLog.pop_front();
        }
    }

    // Debug output
    std::cout << "MIDI: " << description << std::endl;
}

std::string MidiManager::describeMidiMessage(const std::vector<unsigned char>& message) {
    if (message.empty()) {
        return "Empty message";
    }

    std::ostringstream desc;
    
    unsigned char status = message[0];
    unsigned char channel = status & 0x0F;
    unsigned char messageType = status & 0xF0;
    
    // Add raw bytes
    desc << "[";
    for (size_t i = 0; i < message.size(); i++) {
        if (i > 0) desc << " ";
        desc << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(message[i]);
    }
    desc << "] ";
    
    // Decode message type
    switch (messageType) {
        case 0x80: // Note Off
            if (message.size() >= 3) {
                desc << "Note Off: Ch" << (channel + 1) << " Note " << static_cast<int>(message[1]) 
                     << " Vel " << static_cast<int>(message[2]);
            }
            break;
            
        case 0x90: // Note On
            if (message.size() >= 3) {
                desc << "Note On: Ch" << (channel + 1) << " Note " << static_cast<int>(message[1]) 
                     << " Vel " << static_cast<int>(message[2]);
            }
            break;
            
        case 0xB0: // Control Change
            if (message.size() >= 3) {
                desc << "CC: Ch" << (channel + 1) << " CC" << static_cast<int>(message[1]) 
                     << " Val " << static_cast<int>(message[2]);
            }
            break;
            
        case 0xE0: // Pitch Bend
            if (message.size() >= 3) {
                int pitchValue = (message[2] << 7) | message[1];
                desc << "Pitch Bend: Ch" << (channel + 1) << " Value " << pitchValue;
            }
            break;
            
        case 0xF0: // System messages
            desc << "System: ";
            if (status == 0xF8) desc << "Clock";
            else if (status == 0xFA) desc << "Start";
            else if (status == 0xFB) desc << "Continue";
            else if (status == 0xFC) desc << "Stop";
            else if (status == 0xFE) desc << "Active Sensing";
            else if (status == 0xFF) desc << "Reset";
            else desc << "Unknown System";
            break;
            
        default:
            desc << "Unknown message type";
            break;
    }
    
    return desc.str();
}

} // namespace midi
} // namespace gamma