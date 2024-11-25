#include "lnx_input.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include "../shared/baseInput.h"
#include <dirent.h> // For opendir(), readdir(), closedir()
#include <poll.h>


// Constructor
LnxInput::LnxInput()
{
    m_running = true;
    // Detect keyboard and mouse devices
    m_keyboardDevice = "/dev/input/" + FindInputDevice("keyboard");
    //m_keyboardDevice = "/dev/input/event1"; // for some reason linux defaults to event2 instead of event1 which isnt the one needed

    m_mouseDevice = "/dev/input/" + FindInputDevice("mouse");

    // Log detected devices for debugging
    std::cout << "Keyboard Device: " << m_keyboardDevice << std::endl;
    std::cout << "Mouse Device: " << m_mouseDevice << std::endl;

    // Validate detected devices
    if (m_keyboardDevice.empty() || m_mouseDevice.empty())
    {
        std::cerr << "Error: Failed to find valid input devices!" << std::endl;
        m_running = false;
        return;
    }

    // Start the input thread
    m_inputThread = std::thread(&LnxInput::InputThread, this);
}

// Destructor
LnxInput::~LnxInput()
{
    // Stop the input thread safely
    m_running = false;
    if (m_inputThread.joinable())
    {
        m_inputThread.join();
    }
}

// Find Input Device
std::string LnxInput::FindInputDevice(const std::string& deviceType)
{
    DIR* dir = opendir("/dev/input/");
    if (!dir)
    {
        std::cerr << "Error: Unable to open /dev/input directory" << std::endl;
        return "";
    }

    struct dirent* entry;
    std::string eventFile;

    while ((entry = readdir(dir)) != nullptr)
    {
        // Only consider files named "event*"
        if (std::string(entry->d_name).find("event") != std::string::npos)
        {
            std::string eventPath = "/dev/input/" + std::string(entry->d_name);

            // Open the event file
            int fd = open(eventPath.c_str(), O_RDONLY);
            if (fd < 0)
            {
                continue; // Skip if the file cannot be opened
            }

            // Get the device name
            char name[256] = "Unknown";
            ioctl(fd, EVIOCGNAME(sizeof(name)), name);
            close(fd);

            // Match the device type (keyboard or mouse)
            if (deviceType == "keyboard")
            {
                if (std::string(name).find("Keyboard") != std::string::npos ||
                    std::string(name).find("keyboard") != std::string::npos ||
                    std::string(name).find("kbd") != std::string::npos ||
                    std::string(name).find("Input") != std::string::npos) // Broader match
                {
                    eventFile = std::string(entry->d_name);
                    std::cout << "Detected Keyboard: " << name << " -> " << eventFile << std::endl;
                    break;
                }
            }

            else if (deviceType == "mouse" &&
                (std::string(name).find("Mouse") != std::string::npos ||
                    std::string(name).find("mouse") != std::string::npos ||
                    std::string(name).find("pointer") != std::string::npos))
            {
                eventFile = std::string(entry->d_name);
                std::cout << "Detected Mouse: " << name << " -> " << eventFile << std::endl;
                break;
            }
        }
    }

    closedir(dir);
    return eventFile;
}

// Input Thread
void LnxInput::InputThread()
{
    int keyboardFd = open(m_keyboardDevice.c_str(), O_NONBLOCK);
    int mouseFd = open(m_mouseDevice.c_str(), O_NONBLOCK);

    if (keyboardFd < 0 || mouseFd < 0)
    {
        std::cerr << "Error: Unable to open input devices" << std::endl;
        m_running = false;
        return;
    }

    struct pollfd fds[2];
    fds[0].fd = keyboardFd;
    fds[0].events = POLLIN;
    fds[1].fd = mouseFd;
    fds[1].events = POLLIN;

    struct input_event event;

    while (m_running)
    {
        int pollResult = poll(fds, 2, -1); // Wait indefinitely for an event
        if (pollResult > 0)
        {

            // Check keyboard events
            if (fds[0].revents & POLLIN)
            {
                while (read(keyboardFd, &event, sizeof(event)) > 0)
                {
                    if (event.type == EV_KEY) // Handle key press/release
                    {
                        m_keyState[event.code] = event.value > 0; // Pressed or released
                        std::cout << "Keyboard Event - Key: " << event.code
                        << " State: " << event.value << std::endl;
                    }
                }
            }

            // Check mouse events
            if (fds[1].revents & POLLIN)
            {
                while (read(mouseFd, &event, sizeof(event)) > 0)
                {
                    if (event.type == EV_REL)
                    {
                        if (event.code == REL_X) m_mouseState.x += event.value;
                        if (event.code == REL_Y) m_mouseState.y += event.value;
                    }
                    else if (event.type == EV_KEY)
                    {
                        m_mouseState.buttonState |= (1 << event.code);
                        if (event.value == 0) // Button released
                        {
                            m_mouseState.buttonState &= ~(1 << event.code);
                        }
                        std::cout << "Mouse Event - Button: " << event.code
                        << " State: " << event.value << std::endl;
                    }
                }
            }
        }
    }

    close(keyboardFd);
    close(mouseFd);
}


// Check if a key is pressed
bool LnxInput::IsKeyPressed(KeyBoardKey key) const
{
    int linuxKeyCode = static_cast<int>(key); // Assuming your KeyBoardKey enum maps to Linux key codes
    auto it = m_keyState.find(linuxKeyCode);
    return it != m_keyState.end() && it->second;
}

// Poll a single key press
BaseInput::KeyBoardKey LnxInput::PollKeyBoardInput()
{
    for (const auto& [key, state] : m_keyState)
    {
        if (state)
        {
            return static_cast<KeyBoardKey>(key);
        }
    }
    return BaseInput::KeyBoardKey::CURRENT_KEY_UNKNOWN;
}

// Check if a mouse button is pressed
bool LnxInput::IsMouseButtonPressed(int button) const
{
    return m_mouseState.buttonState & (1 << button);
}

// Poll mouse state
BaseInput::MouseInfo LnxInput::PollMouse()
{
    return m_mouseState;
}
