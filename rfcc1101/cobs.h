#ifndef COBS
#define COBS

/**
 * \brief Encode the given data with COBS-algorithm
 * \param inBuffer :Pointer to the data array to be encoded
 * \param length :Number of bytes to be encoded
 * \param outBuffer :Pointer to store the resulting encoded message (minimum size: worst case: length +1 overhead)
 * \return
 * -1:Length is to long to be encoded with COBS \n
 * else: length of the encoded message \n
*/
int encodeCOBS(unsigned char* inBuffer, int length, unsigned char* outBuffer);

/**
 * \brief Decode an COBS-encoded message
 * \param inBuffer :Pointer to the data array to be decoded
 * \param length :Number of bytes to be decoded
 * \param outBuffer :Pointer to store the resulting decoded message (max. resulting message length: 254)
 * \return
 * -1:Length is to long to be decoded with COBS \n
 * else: length of the decoded message \n
*/
int decodeCOBS(unsigned char* inBuffer, int length, unsigned char* outBuffer);

/**
 * \brief Encode the given data with COBS-algorithm with ZPE
 * \param inBuffer :Pointer to the data array to be encoded
 * \param length :Number of bytes to be encoded
 * \param outBuffer :Pointer to store the resulting encoded message (minimum size: worst case: length +1 overhead)
 * \return
 * -1:Length is to long to be encoded with COBS-ZPE \n
 * else: length of the encoded message \n
*/
int encodeCOBSZPE(unsigned char* inBuffer, int length, unsigned char* outBuffer);

/**
 * \brief Decode an COBS-ZPE-encoded message
 * \param inBuffer :Pointer to the data array to be decoded
 * \param length :Number of bytes to be decoded
 * \param outBuffer :Pointer to store the resulting decoded message (max. resulting message length: 223)
 * \return
 * -1:Length is to long to be decoded with COBS-ZPE \n
 * else: length of the decoded message \n
*/
int decodeCOBSZPE(unsigned char* inBuffer, int length, unsigned char* outBuffer);

#endif
