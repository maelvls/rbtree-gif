/* ===========================================================
 *
 *       Filename:  key.h
 *
 *    Description:
 *
 *        Created:  06-04-2013 22:31
 *
 *         Author:  Mael Valais
 *         Mail  :	mael.valais@univ-tlse3.fr
 *
 * =========================================================== */

#ifndef __key_h__
#define __key_h__

void *key_create(int value);
void key_delete(void *key);
int key_cmp(const void *key1, const void *key2);
int key_equal(const void *key1, const void *key2);
int key_put(void *key); /* XXX DEBUG */

#endif /* !__key_h__ */
