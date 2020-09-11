#include <stdint.h>
#include <string.h>
#include "sha1.h"
#include "hmac-sha1.h"

void HMAC_SHA1(uint8_t *key, unsigned int key_len, uint8_t *message, unsigned int msg_len, uint8_t *digest)
{
	uint8_t ipad[HMAC_BLOCK_SIZE];
	uint8_t opad[HMAC_BLOCK_SIZE];
	uint8_t tk[SHA1HashSize];

	if (key_len > HMAC_BLOCK_SIZE)
	{
		SHA1(key, key_len, tk);
		key = tk;
		key_len = SHA1HashSize;
	}

	// HMAC_SHA1 = SHA1(key XOR opad, SHA1(key XOR ipad, message))

	memset(ipad, 0, HMAC_BLOCK_SIZE);
	memset(opad, 0, HMAC_BLOCK_SIZE);
	memcpy(ipad, key, key_len);
	memcpy(opad, key, key_len);

	for (int i = 0; i < HMAC_BLOCK_SIZE; i++)
	{
		ipad[i] ^= 0x36;
		opad[i] ^= 0x5c;
	}

	// Inner SHA1
	SHA1Context sha;
	SHA1Reset(&sha);
	SHA1Input(&sha, ipad, HMAC_BLOCK_SIZE);
	SHA1Input(&sha, message, msg_len);
	SHA1Result(&sha, digest);

	// Outer SHA1
	SHA1Reset(&sha);
	SHA1Input(&sha, opad, HMAC_BLOCK_SIZE);
	SHA1Input(&sha, digest, SHA1HashSize);
	SHA1Result(&sha, digest);
}
