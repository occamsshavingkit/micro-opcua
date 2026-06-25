/* src/services/discovery.c */
#include "discovery.h"
#include <stddef.h>

opcua_statuscode_t mu_discovery_get_application_description(const mu_server_config_t *config, 
                                                            mu_application_description_t *desc)
{
    if (!config || !desc) return MU_STATUS_BAD_INTERNALERROR;

    desc->application_uri = config->application_uri;
    desc->product_uri = config->product_uri;
    desc->application_name = config->application_name;
    desc->application_type = MU_APPLICATION_TYPE_SERVER;
    desc->discovery_url = config->endpoint_url;

    return MU_STATUS_GOOD;
}

opcua_statuscode_t mu_discovery_get_endpoint_description(const mu_server_config_t *config, 
                                                         mu_endpoint_description_t *desc)
{
    if (!config || !desc) return MU_STATUS_BAD_INTERNALERROR;

    desc->endpoint_url = config->endpoint_url;
    
    mu_discovery_get_application_description(config, &desc->server);
    
    desc->security_mode = MU_MESSAGE_SECURITY_MODE_NONE;
    desc->security_policy_uri = "http://opcfoundation.org/UA/SecurityPolicy#None";
    
    desc->num_user_identity_tokens = 1;
    desc->user_identity_tokens[0].policy_id = "anonymous";
    desc->user_identity_tokens[0].token_type = MU_USER_TOKEN_TYPE_ANONYMOUS;
    desc->user_identity_tokens[0].issued_token_type = NULL;
    desc->user_identity_tokens[0].issuer_endpoint_url = NULL;
    desc->user_identity_tokens[0].security_policy_uri = NULL;
    
    desc->transport_profile_uri = "http://opcfoundation.org/UA-Profile/Transport/uatcp-uasc-uabinary";
    desc->security_level = 0; /* lowest */

    return MU_STATUS_GOOD;
}
