//
// Created by amoncusir on 05/10/23.
//

#ifndef LOGTRIGGER_SRC_UBUS_H
#define LOGTRIGGER_SRC_UBUS_H

#define LOG_LINE_SIZE        1024

#include <libubox/ustream.h>
#include <libubox/blobmsg.h>

#ifdef __cplusplus

#include <cstdint>

#else

#include <stdint.h>

#endif

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    LOG_MSG,
    LOG_ID,
    LOG_PRIO,
    LOG_SOURCE,
    LOG_TIME,
    __LOG_MAX
};

const struct blobmsg_policy log_policy[] = {
    [LOG_MSG] = {.name = "msg", .type = BLOBMSG_TYPE_STRING},
    [LOG_ID] = {.name = "id", .type = BLOBMSG_TYPE_INT32},
    [LOG_PRIO] = {.name = "priority", .type = BLOBMSG_TYPE_INT32},
    [LOG_SOURCE] = {.name = "source", .type = BLOBMSG_TYPE_INT32},
    [LOG_TIME] = {.name = "time", .type = BLOBMSG_TYPE_INT64},
};

struct ubus_log_event
{
    char* data;
    uint_least32_t id;
    uint_least32_t priority;
    uint_least32_t source;
    uint_least64_t time;
};

typedef void(* event_ubus_fd_handler)(struct ustream* s, int b);
typedef void(* event_log_callback)(struct ubus_log_event* event);

void ubus_log_callback(const char* ubus_socket, event_ubus_fd_handler callback);

void parse_ustream_to_event_log(struct ustream* s, int b, event_log_callback callback);

#ifdef __cplusplus
}
#endif

#endif //LOGTRIGGER_SRC_UBUS_H
