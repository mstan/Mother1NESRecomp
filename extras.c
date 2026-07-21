/*
 * extras.c -- Mother 1 game-specific runner hooks
 *
 * Minimal hook implementation. Mother uses battery-backed SRAM from its iNES
 * header, so save RAM is handled by the shared runner.
 */
#include "game_extras.h"
#include "nes_runtime.h"
#include "interp.h"
#include "debug_server.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern uint16_t g_rti_target;

const char *g_rom_path_for_extras = NULL;
int         g_watchdog_triggered  = 0;
uint32_t    g_watchdog_frame      = 0;
char        g_watchdog_stack_dump[1024] = "";

uint32_t game_get_expected_crc32(void) { return 0x00F0FBBCu; }

const char *game_get_name(void) { return "Mother 1"; }

void game_on_init(void) {
    debug_server_init(4370);
}

void game_on_frame(uint64_t frame_count) { (void)frame_count; }

void game_post_nmi(uint64_t frame_count) { (void)frame_count; }

int game_handle_arg(const char *key, const char *val) {
    (void)key;
    (void)val;
    return 0;
}

const char *game_arg_usage(void) { return NULL; }

void game_run_nmi(void) { func_NMI(); }

static uint16_t mother1_pop_rts_target(void) {
    g_cpu.S++;
    uint8_t lo = g_ram[0x100 + g_cpu.S];
    g_cpu.S++;
    uint8_t hi = g_ram[0x100 + g_cpu.S];
    return (uint16_t)(((uint16_t)hi << 8) | lo) + 1;
}

static uint16_t mother1_last_consumed_rts_target(void) {
    uint8_t lo_idx = (uint8_t)(g_cpu.S - 1);
    uint8_t hi_idx = g_cpu.S;
    uint8_t lo = g_ram[0x100 + lo_idx];
    uint8_t hi = g_ram[0x100 + hi_idx];
    return (uint16_t)(((uint16_t)hi << 8) | lo) + 1;
}

static uint16_t mother1_peek_rts_target(void) {
    uint8_t lo = g_ram[0x100 + ((g_cpu.S + 1) & 0xFF)];
    uint8_t hi = g_ram[0x100 + ((g_cpu.S + 2) & 0xFF)];
    return (uint16_t)(((uint16_t)hi << 8) | lo) + 1;
}

static int mother1_valid_code_target(uint16_t target) {
    return target >= 0x8000;
}

static int mother1_main_trace_enabled(void) {
    static int enabled = -1;
    if (enabled < 0) {
        const char *v = getenv("NESRECOMP_MOTHER1_MAIN_TRACE");
        enabled = (v && *v) ? 1 : 0;
    }
    return enabled;
}

static void mother1_dump_stack_bytes(const char *prefix) {
    if (!mother1_main_trace_enabled())
        return;
    fprintf(stderr, "%s", prefix);
    for (int i = 0; i < 16; ++i)
        fprintf(stderr, " %02X", g_ram[0x100 + ((g_cpu.S + 1 + i) & 0xFF)]);
    fprintf(stderr, "\n");
    fflush(stderr);
}

void game_run_main(void) {
    func_RESET();

    uint16_t target = mother1_pop_rts_target();
    int lap = 0;
    for (;;) {
        uint8_t before_s = g_cpu.S;
        if (mother1_main_trace_enabled()) {
            fprintf(stderr, "[Mother1Main] lap=%d call target=$%04X S=$%02X frame=%llu\n",
                    lap, target, g_cpu.S, (unsigned long long)g_frame_count);
            mother1_dump_stack_bytes("[Mother1Main] stack before:");
        }
        if (target < 0x8000) {
            fprintf(stderr, "[Mother1] Refusing invalid resume target $%04X S=$%02X frame=%llu\n",
                    target, g_cpu.S, (unsigned long long)g_frame_count);
            mother1_dump_stack_bytes("[Mother1Main] invalid stack:");
            nes_dump_dispatch_ring();
            return;
        }
        if (!nes_interp_resume(target)) {
            fprintf(stderr, "[Mother1] Unable to resume RTS target $%04X\n", target);
            return;
        }
        if (mother1_main_trace_enabled()) {
            fprintf(stderr, "[Mother1Main] lap=%d returned target=$%04X beforeS=$%02X afterS=$%02X frame=%llu\n",
                    lap, target, before_s, g_cpu.S, (unsigned long long)g_frame_count);
            mother1_dump_stack_bytes("[Mother1Main] stack after:");
        }
        if (g_rti_target != 0) {
            target = g_rti_target;
            g_rti_target = 0;
        } else if (g_cpu.S < before_s) {
            /* Stack-scripted far calls can RTS through a target they just
             * consumed while leaving a lower-SP continuation frame behind. */
            target = mother1_last_consumed_rts_target();
        } else if ((uint8_t)(g_cpu.S - before_s) >= 2) {
            uint16_t consumed = mother1_last_consumed_rts_target();
            uint16_t next = mother1_peek_rts_target();
            if (!mother1_valid_code_target(consumed) && mother1_valid_code_target(next))
                target = mother1_pop_rts_target();
            else
                target = consumed;
        } else {
            target = mother1_pop_rts_target();
        }
        lap++;
    }
}

int game_dispatch_override(uint16_t addr) {
    (void)addr;
    return 0;
}

uint8_t game_ram_read_hook(uint16_t pc, uint16_t addr, uint8_t val) {
    (void)pc;
    (void)addr;
    return val;
}

void game_fill_frame_record(void *record) { (void)record; }

int game_handle_debug_cmd(const char *cmd, int id, const char *json) {
    (void)cmd;
    (void)id;
    (void)json;
    return 0;
}

void game_post_render(uint32_t *framebuf) { (void)framebuf; }
