#pragma once

#include <stdint.h>

#define NDEF_URI_CODE_IDENTIFIER_COUNT 36

struct NdefUriPrefix
{
    uint8_t code;
    const char *prefix;
};

struct NdefUriPayload
{
    uint8_t *data;
    uint8_t length;
};

/*
 * @brief Build an NDEF URI payload from a URI string.
 */
struct NdefUriPayload *build_ndef_uri_payload(const char *uri);

/*
 * @brief Build a URI string from an NDEF URI payload.
 */
char *build_uri_from_ndef_uri_payload(struct NdefUriPayload *payload);

/*
 * @brief Free memory of an NDEF URI payload struct.
 */
void free_ndef_uri_payload(struct NdefUriPayload *payload);