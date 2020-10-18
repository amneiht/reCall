/*
 * g711.h
 *
 *  Created on: Sep 8, 2020
 *      Author: dccan
 */
#ifndef G711_H_
#define G711_H_

#include <bits/stdint-intn.h>

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Description:
 *  Encodes a 16-bit signed integer using the A-Law.
 * Parameters:
 *  number - the number who will be encoded
 * Returns:
 *  The encoded number
 */
int8_t ALaw_Encode(int16_t number);
/*
 * Description:
 *  Decodes an 8-bit unsigned integer using the A-Law.
 * Parameters:
 *  number - the number who will be decoded
 * Returns:
 *  The decoded number
 */
int16_t ALaw_Decode(int8_t number);
/*
 * Description:
 *  Encodes a 16-bit signed integer using the mu-Law.
 * Parameters:
 *  number - the number who will be encoded
 * Returns:
 *  The encoded number
 */
int8_t MuLaw_Encode(int16_t number);
/*
 * Description:
 *  Decodes an 8-bit unsigned integer using the mu-Law.
 * Parameters:
 *  number - the number who will be decoded
 * Returns:
 *  The decoded number
 */
int16_t MuLaw_Decode(int8_t number);

void UlawArrayEncode(int16_t *in, int size, int8_t *out);
void UlawArrayDecode(int8_t *in, int size, int16_t *out);
void AlawArrayEncode(int16_t *in, int size, int8_t *out);
void AlawArrayDecode(int8_t *in, int size, int16_t *out);


#ifdef __cplusplus
}
#endif
#endif /* G711_H_ */
