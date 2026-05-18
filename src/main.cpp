#include "mbed.h"
#include "events/EventQueue.h"

#include "app_config.h"
#include "state_machine.h"
#include "motion_mpu6050.h"
#include "lora_app.h"

// ==================================================
// Global Hardware
// ==================================================

DigitalOut buzzer(D7, 0);

// EventQueue global (wird von anderen Dateien verwendet)
EventQueue evq(64 * EVENTS_EVENT_SIZE);

// ==================================================
// Buzzer Funktionen (werden vom State Machine genutzt)
// ==================================================

void buzzer_off()
{
    buzzer = 0;
}

void buzzer_beep_ms(EventQueue& queue, std::chrono::milliseconds duration)
{
    buzzer = 1;
    queue.call_in(duration, buzzer_off);
}

// Alarm: periodisches Beepen
static int alarm_beep_id = 0;

void buzzer_start_alarm(EventQueue& queue)
{
    if (alarm_beep_id != 0)
        return;

    alarm_beep_id = queue.call_every(2s, []() {
        buzzer = 1;
        evq.call_in(120ms, buzzer_off);
    });
}

void buzzer_stop_alarm(EventQueue& queue)
{
    if (alarm_beep_id != 0)
    {
        queue.cancel(alarm_beep_id);
        alarm_beep_id = 0;
    }
    buzzer = 0;
}

// ==================================================
// Debug – MPU Monitoring
// ==================================================

static int motion_debug_id = 0;

static void motion_debug_task()
{
    float g = motion_get_abs_g();

    printf("[MPU] ACC=%.2f g\r\n", g);

    // Bewegungsschwelle (für Debug sichtbar)
    if (g > 1.20f)
    {
        printf("[MPU] MOTION DETECTED\r\n");
    }
}

// ==================================================
// main
// ==================================================

int main()
{
    printf("\r\n=============================\r\n");
    printf("E-Bike Anti-Theft Firmware\r\n");
    printf("=============================\r\n");

    // ------------------------------
    // MPU Init
    // ------------------------------
    if (!motion_init())
    {
        printf("[MPU] init ERROR\r\n");
    }
    else
    {
        printf("[MPU] init OK\r\n");
    }

    motion_set_enabled(false);

    // Debug: Beschleunigung alle 500 ms ausgeben
    motion_debug_id = evq.call_every(500ms, motion_debug_task);

    // ------------------------------
    // LoRa Init
    // ------------------------------
    printf("[LoRa] init...\r\n");
    lora_init(evq);

    // ------------------------------
    // State Machine starten
    // ------------------------------
    state_machine_init(evq);

    printf("[SYSTEM] READY\r\n");

    // ------------------------------
    // Scheduler starten
    // ------------------------------
    evq.dispatch_forever();
}
