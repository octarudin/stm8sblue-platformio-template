#include "kernel/cooperative_scheduler.h"

typedef struct {
    scheduler_task_fn_t function;
    uint16_t period_ms;
    uint16_t next_run_ms;
    uint8_t enabled;
} scheduler_task_t;

static scheduler_task_t tasks[SCHEDULER_MAX_TASKS];
static uint8_t task_count;
static uint16_t registration_time_ms;

static uint8_t scheduler_time_reached(uint16_t now_ms, uint16_t deadline_ms)
{
    return (int16_t)(now_ms - deadline_ms) >= 0;
}

void scheduler_init(uint16_t now_ms)
{
    uint8_t index;

    task_count = 0U;
    registration_time_ms = now_ms;

    for (index = 0U; index < SCHEDULER_MAX_TASKS; index++) {
        tasks[index].function = (scheduler_task_fn_t)0;
        tasks[index].period_ms = 0U;
        tasks[index].next_run_ms = 0U;
        tasks[index].enabled = 0U;
    }
}

scheduler_task_id_t scheduler_add(
    scheduler_task_fn_t task,
    uint16_t period_ms,
    uint16_t initial_delay_ms
)
{
    scheduler_task_id_t task_id;

    if ((task == (scheduler_task_fn_t)0) ||
        (period_ms == 0U) ||
        (period_ms > SCHEDULER_MAX_INTERVAL_MS) ||
        (initial_delay_ms == 0U) ||
        (initial_delay_ms > SCHEDULER_MAX_INTERVAL_MS) ||
        (task_count >= SCHEDULER_MAX_TASKS)) {
        return SCHEDULER_INVALID_TASK_ID;
    }

    task_id = task_count;
    tasks[task_id].function = task;
    tasks[task_id].period_ms = period_ms;
    tasks[task_id].next_run_ms = registration_time_ms + initial_delay_ms;
    tasks[task_id].enabled = 1U;
    task_count++;

    return task_id;
}

uint8_t scheduler_set_enabled(
    scheduler_task_id_t task_id,
    uint8_t enabled,
    uint16_t now_ms
)
{
    if (task_id >= task_count) {
        return 0U;
    }

    tasks[task_id].enabled = enabled != 0U;

    if (tasks[task_id].enabled != 0U) {
        tasks[task_id].next_run_ms = now_ms + tasks[task_id].period_ms;
    }

    return 1U;
}

void scheduler_run(uint16_t now_ms)
{
    uint8_t index;
    scheduler_task_t *task;

    for (index = 0U; index < task_count; index++) {
        task = &tasks[index];

        if ((task->enabled == 0U) ||
            (scheduler_time_reached(now_ms, task->next_run_ms) == 0U)) {
            continue;
        }

        /* Pertahankan fase, tetapi lewati eksekusi yang tertinggal. */
        do {
            task->next_run_ms += task->period_ms;
        } while (scheduler_time_reached(now_ms, task->next_run_ms) != 0U);

        task->function();
    }
}

uint8_t scheduler_task_count(void)
{
    return task_count;
}
