/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Blake W. Felt & Angus Gratton
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/mphal.h"

#if MICROPY_HW_ENABLE_USBDEV
#include "mp_usbd.h"
#ifndef NO_QSTR
#include "device/dcd.h"
#endif

#if !MICROPY_HW_ENABLE_USB_RUNTIME_DEVICE
void mp_usbd_task(void) {
    tud_task_ext(0, false);
}

void mp_usbd_task_callback(mp_sched_node_t *node) {
    (void)node;
    mp_usbd_task();
}

#endif // !MICROPY_HW_ENABLE_USB_RUNTIME_DEVICE

extern void __real_dcd_event_handler(dcd_event_t const *event, bool in_isr);

// If -Wl,--wrap=dcd_event_handler is passed to the linker, then this wrapper
// will be called and allows MicroPython to schedule the TinyUSB task when
// dcd_event_handler() is called from an ISR.
TU_ATTR_FAST_FUNC void __wrap_dcd_event_handler(dcd_event_t const *event, bool in_isr) {
    __real_dcd_event_handler(event, in_isr);
    mp_usbd_schedule_task();
    mp_hal_wake_main_task_from_isr();
}

TU_ATTR_FAST_FUNC void mp_usbd_schedule_task(void) {
    static mp_sched_node_t usbd_task_node;
    mp_sched_schedule_node(&usbd_task_node, mp_usbd_task_callback);
}

void mp_usbd_hex_str(char *out_str, const uint8_t *bytes, size_t bytes_len) {
    size_t hex_len = bytes_len * 2;
    for (int i = 0; i < hex_len; i += 2) {
        static const char *hexdig = "0123456789abcdef";
        out_str[i] = hexdig[bytes[i / 2] >> 4];
        out_str[i + 1] = hexdig[bytes[i / 2] & 0x0f];
    }
    out_str[hex_len] = 0;
}
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(HID_ITF_PROTOCOL_MOUSE))
};

/**
 * @brief String descriptor
 */
const char* hid_string_descriptor[5] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
    "TinyUSB",             // 1: Manufacturer
    "TinyUSB Device",      // 2: Product
    "123456",              // 3: Serials, should use chip ID
    "Example HID interface",  // 4: HID
};

/********* TinyUSB HID callbacks ***************/
// Invoked when received GET HID REPORT DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    // We use only one interface and one HID report descriptor, so we can ignore parameter 'instance'
    return hid_report_descriptor;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
}
#endif // MICROPY_HW_ENABLE_USBDEV
