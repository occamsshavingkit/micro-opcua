/* src/core/service_dispatch.c */
#include "service_dispatch.h"
#include "server_internal.h"
#include "micro_opcua/encoding.h"
#include "../services/discovery.h"
#include "../services/session.h"
#include "../services/secure_channel.h"
#include "../services/browse.h"
#include "../services/read.h"
#include "../services/service_header.h"
#include <stddef.h>

static const mu_service_handler_t g_supported_services[] = {
    { MU_ID_FINDSERVERSREQUEST,        MU_ID_FINDSERVERSRESPONSE,        false },
    { MU_ID_GETENDPOINTSREQUEST,       MU_ID_GETENDPOINTSRESPONSE,       false },
    { MU_ID_OPENSECURECHANNELREQUEST,  MU_ID_OPENSECURECHANNELRESPONSE,  false },
    { MU_ID_CLOSESECURECHANNELREQUEST, MU_ID_CLOSESECURECHANNELRESPONSE, false },
    { MU_ID_CREATESESSIONREQUEST,      MU_ID_CREATESESSIONRESPONSE,      false }, /* Does not require an activated session */
    { MU_ID_ACTIVATESESSIONREQUEST,    MU_ID_ACTIVATESESSIONRESPONSE,    false }, /* Does not require an activated session to activate */
    { MU_ID_CLOSESESSIONREQUEST,       MU_ID_CLOSESESSIONRESPONSE,       true  },
    { MU_ID_BROWSEREQUEST,             MU_ID_BROWSERESPONSE,             true  },
    { MU_ID_READREQUEST,               MU_ID_READRESPONSE,               true  }
};

static const size_t g_num_supported_services = sizeof(g_supported_services) / sizeof(g_supported_services[0]);

const mu_service_handler_t* mu_get_service_handler(opcua_uint32_t request_id) {
    for (size_t i = 0; i < g_num_supported_services; ++i) {
        if (g_supported_services[i].request_id == request_id) {
            return &g_supported_services[i];
        }
    }
    return NULL;
}

/* Write the common response prefix: response-type NodeId + ResponseHeader. */
static opcua_statuscode_t write_response_prefix(mu_binary_writer_t *w,
                                                opcua_uint32_t response_type_id,
                                                opcua_uint32_t request_handle,
                                                opcua_statuscode_t service_result)
{
    mu_nodeid_t type = { 0, MU_NODEID_NUMERIC, { response_type_id } };
    opcua_statuscode_t s = mu_binary_write_nodeid(w, &type);
    if (s != MU_STATUS_GOOD) return s;

    mu_response_header_t rh;
    rh.timestamp = 0;
    rh.request_handle = request_handle;
    rh.service_result = service_result;
    return mu_response_header_encode(w, &rh);
}

/* OpenSecureChannel (OPC 10000-4 5.6.2.2): decode the request, open/renew the
   channel, and encode OpenSecureChannelResponse (ChannelSecurityToken + nonce). */
static opcua_statuscode_t handle_open_secure_channel(mu_server_t *server,
                                                     mu_binary_reader_t *r,
                                                     mu_binary_writer_t *w,
                                                     size_t *response_length)
{
    mu_request_header_t req;
    opcua_statuscode_t s = mu_request_header_decode(r, &req);
    if (s != MU_STATUS_GOOD) return s;

    opcua_uint32_t client_proto, request_type, security_mode, requested_lifetime;
    mu_bytestring_t client_nonce;
    s = mu_binary_read_uint32(r, &client_proto);       if (s != MU_STATUS_GOOD) return s;
    s = mu_binary_read_uint32(r, &request_type);       if (s != MU_STATUS_GOOD) return s;
    s = mu_binary_read_uint32(r, &security_mode);      if (s != MU_STATUS_GOOD) return s;
    s = mu_binary_read_bytestring(r, &client_nonce);   if (s != MU_STATUS_GOOD) return s;
    s = mu_binary_read_uint32(r, &requested_lifetime); if (s != MU_STATUS_GOOD) return s;

    opcua_uint32_t revised = 0;
    s = mu_secure_channel_open(&server->secure_channel, NULL, requested_lifetime, &revised);
    if (s != MU_STATUS_GOOD) return s;

    s = write_response_prefix(w, MU_ID_OPENSECURECHANNELRESPONSE, req.request_handle, MU_STATUS_GOOD);
    if (s != MU_STATUS_GOOD) return s;

    opcua_datetime_t now = server->config.time_adapter.get_time
        ? server->config.time_adapter.get_time(server->config.time_adapter.context) : 0;

    s = mu_binary_write_uint32(w, 0);                              if (s != MU_STATUS_GOOD) return s; /* ServerProtocolVersion */
    s = mu_binary_write_uint32(w, server->secure_channel.channel_id); if (s != MU_STATUS_GOOD) return s; /* SecurityToken.ChannelId */
    s = mu_binary_write_uint32(w, server->secure_channel.token_id);   if (s != MU_STATUS_GOOD) return s; /* SecurityToken.TokenId */
    s = mu_binary_write_int64(w, now);                            if (s != MU_STATUS_GOOD) return s; /* SecurityToken.CreatedAt */
    s = mu_binary_write_uint32(w, revised);                       if (s != MU_STATUS_GOOD) return s; /* SecurityToken.RevisedLifetime */

    mu_bytestring_t server_nonce = { 0, NULL }; /* empty for SecurityPolicy None */
    s = mu_binary_write_bytestring(w, &server_nonce);             if (s != MU_STATUS_GOOD) return s;

    *response_length = w->position;
    return MU_STATUS_GOOD;
}

opcua_statuscode_t mu_service_dispatch(
    mu_server_t *server,
    opcua_uint32_t request_id,
    const opcua_byte_t *request_body,
    size_t request_length,
    opcua_byte_t *response_body,
    size_t *response_length)
{
    if (!server || !request_body || !response_body || !response_length) return MU_STATUS_BAD_INTERNALERROR;

    const mu_service_handler_t *handler = mu_get_service_handler(request_id);
    if (handler == NULL) {
        return MU_STATUS_BAD_SERVICEUNSUPPORTED;
    }

    if (request_id != MU_ID_OPENSECURECHANNELREQUEST) {
        if (!server->secure_channel.is_open) {
            /* If we haven't even opened a secure channel, we can't process requests over it */
            /* OPC UA Part 4, 7.38.2: Bad_SecureChannelIdInvalid */
            return MU_STATUS_BAD_SECURECHANNELIDINVALID; 
        }
    }

    if (handler->requires_session) {
        if (server->session.state != MU_SESSION_STATE_ACTIVATED) {
            return MU_STATUS_BAD_SESSIONIDINVALID;
        }
    }

    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, request_body, request_length);
    mu_binary_writer_t writer;
    mu_binary_writer_init(&writer, response_body, *response_length);

    switch (request_id) {
        case MU_ID_OPENSECURECHANNELREQUEST:
            return handle_open_secure_channel(server, &reader, &writer, response_length);
        default:
            /* Not yet wired (discovery/session/browse/read): succeed with no body. */
            *response_length = 0;
            return MU_STATUS_GOOD;
    }
}
