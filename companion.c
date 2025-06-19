#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define IDLE_THRESHOLD 60 // 1 minutes in seconds

// Array of friendly messages
const char* messages[] = {
    "Rest your eyes!",
    "Do a quick stretch!",
    "Shake off the stiffness!",
    "Take a quick walk!",
    "Time for a mini-break!"
};

// Function to get system idle time in seconds
int get_idle_time() {
    LASTINPUTINFO lii;
    lii.cbSize = sizeof(LASTINPUTINFO);
    GetLastInputInfo(&lii);
    DWORD tickCount = GetTickCount();
    return (tickCount - lii.dwTime) / 1000;
}

// Log message to file with status and timestamp
void log_activity(const char *status, const char *message) {
    FILE *log = fopen("companion.log", "a");
    if (log == NULL) {
        perror("Unable to open log file");
        return;
    }

    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%b %d, %I:%M %p", local); // e.g., Apr 30, 01:45 PM

    fprintf(log, "[%s] STATUS: %s | MESSAGE: %s\n", time_str, status, message);
    fclose(log);
}

int main() {
    int message_counter = 0;
    time_t last_check_time = time(NULL);
    int message_shown = 0;

    printf("Hello, I am your Linux Companion!\n");

    while (1) {
        int idle_time = get_idle_time();
        time_t current_time = time(NULL);

        if (current_time - last_check_time >= 60) { // Check every minute
            last_check_time = current_time;

            if (idle_time >= IDLE_THRESHOLD) {
                if (!message_shown) {
                    const char* msg = messages[message_counter];
                    printf("%s (Idle 1 min)\n", msg);
                    log_activity("IDLE", msg);
                    message_counter = (message_counter + 1) % (sizeof(messages) / sizeof(messages[0]));
                    message_shown = 1;  // Prevent repeat
                }
            } else {
                // User became active again
                message_shown = 0;
            }
        }

        Sleep(1000); // Wait 1 second
    }

    return 0;
}
