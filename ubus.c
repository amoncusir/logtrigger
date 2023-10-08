//
// Created by amoncusir on 05/10/23.
//

#include "ubus.h"

#include <libubus.h>

#include <libubox/uloop.h>
#include <libubox/ustream.h>
#include <libubox/blob.h>
#include <libubox/blobmsg.h>

#include <stdint.h>


static void logread_fd_state_cb(struct ustream* s)
{
    uloop_end();
}

static void logread_fd_cb(struct ubus_request* req, int fd)
{
    static struct ustream_fd test_fd;

    memset(&test_fd, 0, sizeof(test_fd));

    test_fd.stream.notify_read = req->priv;
    test_fd.stream.notify_state = logread_fd_state_cb;
    ustream_fd_init(&test_fd, fd);
}

static void prepare_ubus_settings(struct blob_buf* sett)
{
    blob_buf_init(sett, 0);
    blobmsg_add_u8(sett, "stream", 1);
    blobmsg_add_u8(sett, "oneshot", 0);
    blobmsg_add_u32(sett, "lines", 0);
}

void ubus_log_callback(const char* ubus_socket, event_ubus_fd_handler callback)
{
    if (callback == NULL)
    {
        fprintf(stderr, "NULL pointer callback\n");
        return;
    }

    uint32_t id = 0;
    struct ubus_context* ctx;
    static struct blob_buf settings_log_bus;

    uloop_init();

    ctx = ubus_connect(ubus_socket);
    if (!ctx)
    {
        fprintf(stderr, "Failed to connect to ubus\n");
        uloop_done();
        return;
    }

    ubus_add_uloop(ctx);

    // information to send to the ubus_invoke_async and ubus service
    prepare_ubus_settings(&settings_log_bus);

    struct ubus_request req;

    if (ubus_lookup_id(ctx, "log", &id))
    {
        fprintf(stderr, "Failed to find log object");
        ubus_free(ctx);
        uloop_done();
        return;
    }

    ubus_invoke_async(ctx, id, "read", settings_log_bus.head, &req);

    req.fd_cb = logread_fd_cb;
    req.priv = callback;

    ubus_complete_request_async(ctx, &req);

    // process messages
    uloop_run();

    ubus_free(ctx);
    uloop_done();
}

static void parse_blob_attr_to_log_event(struct blob_attr* msg, struct ubus_log_event* event)
{
    struct blob_attr* parsed_msg[__LOG_MAX];

    blobmsg_parse(log_policy, ARRAY_SIZE(log_policy), parsed_msg, blob_data(msg), blob_len(msg));

    if (!parsed_msg[LOG_ID]
        || !parsed_msg[LOG_PRIO]
        || !parsed_msg[LOG_SOURCE]
        || !parsed_msg[LOG_TIME]
        || !parsed_msg[LOG_MSG])
        return;

    event->data = blobmsg_get_string(parsed_msg[LOG_MSG]);
    event->id = blobmsg_get_u32(parsed_msg[LOG_ID]);
    event->priority = blobmsg_get_u32(parsed_msg[LOG_PRIO]);
    event->source = blobmsg_get_u32(parsed_msg[LOG_SOURCE]);
    event->time = blobmsg_get_u64(parsed_msg[LOG_TIME]);
}

void parse_ustream_to_event_log(struct ustream* stream, int bytes_new, event_log_callback callback)
{
    while (1)
    {
        int len = 0;
        int cur_len = 0;
        struct blob_attr* attributes = (void*) ustream_get_read_buf(stream, &len);

        if (len < sizeof(*attributes))
            break;

        cur_len = blob_len(attributes) + sizeof(*attributes);
        if (len < cur_len)
            break;

        struct ubus_log_event event;
        parse_blob_attr_to_log_event(attributes, &event);

        callback(&event);

        ustream_consume(stream, cur_len);
    }
}
